/*
 * Copyright 2014 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "result/ErrnoResult.hpp"
#include <result/Result.hpp>
#include <AudioCommsAssert.hpp>
#include <cmath>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits>


namespace audio_comms
{
namespace utilities
{
namespace signal_processing
{

template <class T>
class SignalProcessing
{

public:

    enum FailureCode
    {
        Success = 999,
        Unknown,
        InvalidArg,
        NoSuchFile,
        InvalidFileLength,
        MemoryError,
        ConstSignal
    };

    struct CrossCorrelationResult
    {
        double  coefficient;
        ssize_t delay;
    };

    struct SignalProcStatus
    {
        typedef FailureCode Code;

        /** Enum coding the failures that can occur in the class methods. */
        const static Code success = Success;
        const static Code defaultError = Unknown;

        static std::string codeToString(const Code &code)
        {
            switch (code) {
            case Success:
                return "Success";
            case Unknown:
                return "Unknown error";
            case InvalidArg:
                return "Invalid argument";
            case NoSuchFile:
                return "No such file";
            case InvalidFileLength:
                return "Invalid file length";
            case MemoryError:
                return "Memory error";
            case ConstSignal:
                return "Cross Correlation of a const signal (silence)";
            }
            /* Unreachable, prevents gcc to complain */
            return "Invalid error (Unreachable)";
        }
    };

    /** The type of the method returns. */
    typedef utilities::result::Result<SignalProcStatus> Result;

    static Result cross_correlate(const char *fileA,
                                  const char *fileB,
                                  CrossCorrelationResult &result,
                                  ssize_t minDelay = 0,
                                  ssize_t maxDelay = 500);

    /** Normalized cross corelation.
     *
     *  @return the normalized cross correlation between the 2 signals A and B.
     */
    static Result cross_correlate(const T *signalA,
                                  const T *signalB,
                                  size_t valueNb,
                                  CrossCorrelationResult &result,
                                  ssize_t minDelay,
                                  ssize_t maxDelay);

    /** Calculate the mean (average) of a signal. */
    static double mean(const T *signal, size_t valueNb);

    /** Calculate the variance of a signal. */
    static double variance(double mean, const T *signal, size_t valueNb);

    /** Calculate the sum((A(t) - meanA)*(B(t + offsetB) - meanB))
     *
     *  @param[in] meanA is the signal A geometric mean
     *  @param[in] meanB is the signal B geometric mean
     *  @param[in] signalA is the signal A(t) define in [0, valueNb[
     *  @param[in] signalB is the signal B(t) define in [0, valueNb[
     *  @param[in] valueNb is the number of values if the signals A and B
     *  @param[in] offsetB is the
     *
     *  @return the sum.
     */
    static double normalizedOffsetProduct(
        double meanA, double meanB,
        const T *signalA, const T *signalB,
        size_t valueNb, ssize_t offsetB);
};


/** Helper class to limit instantiation of templates */
template <typename T>
struct ProcessingAllowed;

/** List of allowed types for audio processing*/
template <>
struct ProcessingAllowed<int8_t> {};
template <>
struct ProcessingAllowed<int16_t> {};
template <>
struct ProcessingAllowed<int32_t> {};


template <class T>
typename SignalProcessing<T>::Result SignalProcessing<T>::cross_correlate(
    const char *fileA, const char *fileB,
    CrossCorrelationResult &result,
    ssize_t minDelay, ssize_t maxDelay)
{
    /* Check that processing with that type is allowed.
     * If this fails, this means that this template was not intended to be used
     * with this type, thus that the result is undefined. */
    ProcessingAllowed<T>();

    int fa;
    int fb;
    struct stat sa;
    struct stat sb;
    T *signalA;
    T *signalB;
    Result status;
    using utilities::result::ErrnoResult;

    fa = open(fileA, O_RDONLY);
    if (fa < 0) {
        status = Result(NoSuchFile) << fileA << ErrnoResult(errno);
        goto end;
    }
    fb = open(fileB, O_RDONLY);
    if (fb < 0) {
        status = Result(NoSuchFile) << fileB << ErrnoResult(errno);
        goto closeFileA;
    }

    if ((fstat(fa, &sa) == -1) || (fstat(fb, &sb) == -1)) {
        status = Result(Unknown) << "fstat fails" << ErrnoResult(errno);
        goto closeFileB;
    }

    if (sa.st_size != sb.st_size) {
        status = Result(InvalidFileLength)
                 << "Both files should has the same length: "
                 << sa.st_size << " (" << fileA << ")" << " vs "
                 << sb.st_size << " (" << fileB << ")";
        goto closeFileB;
    }

    /* Map file A */
    signalA = static_cast<T *>(mmap(NULL, sa.st_size, PROT_READ, MAP_PRIVATE, fa, 0));
    if (signalA == MAP_FAILED) {
        status = Result(MemoryError) << "while mapping " << fileA << ErrnoResult(errno);
        goto closeFileB;
    }
    /* Map file B */
    signalB = static_cast<T *>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fb, 0));
    if (signalB == MAP_FAILED) {
        status = Result(MemoryError) << "while mapping " << fileB << ErrnoResult(errno);
        goto unMapSignalA;
    }

    /* Cross correlate */
    status = cross_correlate(signalA, signalB, sa.st_size / sizeof(T), result,  minDelay, maxDelay);

    /* Free */
    munmap(signalB, sb.st_size);
unMapSignalA:
    munmap(signalA, sa.st_size);
closeFileB:
    close(fb);
closeFileA:
    close(fa);
end:
    return status;
}

template <class T>
double SignalProcessing<T>::mean(const T *signal, size_t valueNb)
{
    /* Check that processing with that type is allowed.
     * If this fails, this means that this template was not intended to be used
     * with this type, thus that the result is undefined. */
    ProcessingAllowed<T>();

    double sum = 0;
    for (size_t i = 0; i < valueNb; i++) {
        sum += signal[i];
    }
    return sum / valueNb;
}

template <class T>
double SignalProcessing<T>::variance(double mean, const T *signal, size_t valueNb)
{
    /* Check that processing with that type is allowed.
     * If this fails, this means that this template was not intended to be used
     * with this type, thus that the result is undefined. */
    ProcessingAllowed<T>();

    double variance = 0;
    for (size_t i = 0; i < valueNb; i++) {
        variance += pow(signal[i] - mean, 2);
    }
    return variance;
}

template <class T>
double SignalProcessing<T>::normalizedOffsetProduct(double meanA, double meanB,
                                                    const T *signalA, const T *signalB,
                                                    size_t valueNb, ssize_t offsetB)
{
    /* Check that processing with that type is allowed.
     * If this fails, this means that this template was not intended to be used
     * with this type, thus that the result is undefined. */
    ProcessingAllowed<T>();

    size_t startIndexA = std::max(0, offsetB);
    size_t stopIndexA = valueNb + std::min(0, offsetB);

    double normProd = 0;
    for (size_t indexA = startIndexA; indexA < stopIndexA; indexA++) {
        size_t indexB = indexA - offsetB;
        normProd += (signalA[indexA] - meanA) * (signalB[indexB] - meanB);
    }
    return normProd;
}

template <class T>
typename SignalProcessing<T>::Result SignalProcessing<T>::cross_correlate(
    const T *signalA,
    const T *signalB,
    size_t valueNb,
    CrossCorrelationResult &result,
    ssize_t minDelay,
    ssize_t maxDelay)
{
    /* Check that processing with that type is allowed.
     * If this fails, this means that this template was not intended to be used
     * with this type, thus that the result is undefined. */
    ProcessingAllowed<T>();

    double meanA = mean(signalA, valueNb);
    double meanB = mean(signalB, valueNb);

    double varianceA = variance(meanA, signalA, valueNb);
    double varianceB = variance(meanB, signalB, valueNb);

    double denom = sqrt(varianceA * varianceB);

    if (denom == 0) {
        /** if denom is 0, the cross correlation can't work because at least one
         *  signal is constant, so the variance is zero
         */
        return Result(ConstSignal);
    }

    result.coefficient = 0;
    // Will be overwriten as for any x, x > maxCorrelationCoef (result.coefficient)
    result.delay = std::numeric_limits<ssize_t>::max();

    for (ssize_t delay = minDelay; delay <= maxDelay; delay++) {
        double correlationCoef = normalizedOffsetProduct(meanA, meanB,
                                                         signalA, signalB,
                                                         valueNb, -delay);
        correlationCoef /= denom;

        if (correlationCoef > result.coefficient) {
            result.coefficient = correlationCoef;
            result.delay = delay;
        }
    }

    return Result::success();

}

}
}
}
