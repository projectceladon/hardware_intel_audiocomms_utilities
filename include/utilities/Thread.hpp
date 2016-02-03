/*
 * Copyright 2013-2016 Intel Corporation
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

#include <AudioNonCopyable.hpp>
#include <AudioCommsAssert.hpp>

#include <pthread.h>

namespace audio_comms
{
namespace utilities
{

/**
 * Helper class to spawn and handle a thread
 *
 * This class is NOT thread safe.
 * The methods start, stop, isStarted and the destructor
 * MUST not be called simultaneously.
 */
class Thread : private audio_comms::utilities::NonCopyable
{
public:
    /** Create a thread with a name.
     * @param[in] name The name that the thread should give himself.
     *                 "" for default (inherit parent name).
     *                 If longer than MAX_TASK_COMM_LEN, will be truncated.
     */
    Thread(const std::string name)
        : _stopRequested(false), _thread(0), mName(name)
    {
        if (mName.length() > MAX_TASK_COMM_LEN) {
            mName.resize(MAX_TASK_COMM_LEN);
        }
    }

    virtual ~Thread()
    {
        AUDIOCOMMS_ASSERT(_thread == 0, "Can not destroy a running thread");
    }

    /**
     * Start the thread.
     * Calling this function on an already started thread will return false.
     *
     * @return true on success false otherwise
     */
    virtual bool start();

    /** Ask for the tread to stop and waits until it is.
     * Note: this is a collaborative stop, which means that if the processing
     * function is blocking, the thread will stop on processing completion.
     * This is user responsibility to prevent deadlocks in processing.
     */
    virtual void stop();

    /**
     * @return true is the thread has been started
     */
    bool isStarted() const { return _thread != 0; }

    /** @return the name of the tread in witch the function is called. */
    static std::string getCurrentThreadName();

    /** @return the kernel thread Id in which the function is called. */
    static std::string getTid();

protected:
    /** Tells if the thread is requested to stop
     * Note: This does NOT mean that the thread is stopped. The thread is
     * guaranteed to be stopped only when stop() returns.
     * @return true if the thread was requested to stop.*/
    bool isStopRequested() const { return _stopRequested; }

    /**
     * This method allows the Thread to terminate itself.
     * The subclasse can call this method from its processing() method in order
     * to abort its execution. Once selfAbort() is called, the processing() will
     * never be recalled as soon as it returns.
     * The Thread user can safely call the stop() method while the thread has aborted itself.
     */
    void selfAbort();

    /** Max thread name length.
     *
     *  16 with the terminating null char.
     *  @note: Not exposed by kernel headers
     */
    static const size_t MAX_TASK_COMM_LEN = 15;

private:
    /** The processing function which will repeatedly be called by the thread.
     *
     * It will be called at least once.
     */
    virtual void processing() = 0;

    /**
     * This method can be overloaded to perform shutdown stuff
     */
    virtual void shutdown() {}

    void loop()
    {
        setDebugName();
        do {
            processing();
        } while (!_stopRequested);
    }

    /** Set the current thread name to ease debug.
     *
     * Must not be longer than MAX_TASK_COMM_LEN characters or it will be
     * truncated.
     */
    bool setDebugName();

    static void *threadMainHelper(void *);

    volatile bool _stopRequested; /**< Should the thread keep running? */
    pthread_t _thread;           /**< the thread handle */
    std::string mName;
};

} // namespace utilities
} // namespace audio_comms
