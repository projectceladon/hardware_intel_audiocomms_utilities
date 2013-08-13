/*
 * @file
 * audio comms assert facilities.
 *
 * @section License
 *
 * Copyright 2013 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef AUDIOCOMMS_ASSERT_H
#define AUDIOCOMMS_ASSERT_H

/**
 * Macro to wrap gcc builtin function to specify that a condition is unlikely
 * to happen. */
#define audio_comms_unlikely(cond) __builtin_expect((cond), 0)

#ifdef __cplusplus

/**
 * This AUDIOCOMMS_COMPILE_TIME_ASSERT MACRO will fail to compile if the condition is false.
 * Currently it only works in functions
 * Example:
 *     Put "AUDIOCOMMS_COMPILE_TIME_ASSERT(1==0)" your C++ source.
 * On compilation you will get error that looks like:
 * error: 'compileTimeAssertFailure<false>::compileTimeAssertFailure()' is private
 *      YOUR_SOURCE_FILE.cpp:YOUR_ASSERT_LINE: error: within this context
 */
#define AUDIOCOMMS_COMPILE_TIME_ASSERT(c) audio_comms::utilities::compileTimeAssertFailure<c>()

namespace audio_comms
{

namespace utilities
{

template <bool condition>
class compileTimeAssertFailure;

// If true do nothing
template <>
class compileTimeAssertFailure<true> {};

// If false instanciate with private constructor
template <>
class compileTimeAssertFailure<false>
{
    compileTimeAssertFailure();
};

} // namespace utilities

} // namespace audio_comms

#include <iostream>
/**
 * Checks a condition and abort if it is not met.
 * This assert is ALWAYS activated (even in release mode), thus it must be used
 * wisely.
 *
 * @param[in] cond   the condition to check
 * @param[in] iostr  an iostream giving some details about the error
 */
#define AUDIOCOMMS_ASSERT(cond, iostr) \
 do { \
   if (audio_comms_unlikely(!(cond))) { \
       std::cerr << __BASE_FILE__ ":" << __LINE__ << ": Assertion " #cond " failed: " \
                 << iostr << std::endl; \
       abort(); \
   } \
 } while (false)

#else // __cplusplus

/**
  *  This AUDIOCOMMS_COMPILE_TIME_ASSERT MACRO will fail to compile if the condition is false.
  *  Currently it only works in functions
  *  Example:
  *      Put "AUDIOCOMMS_COMPILE_TIME_ASSERT(1==0)" your C source.
  *  On compilation you will get the following error:
  *      YOUR_SOURCE_FILE.c:YOUR_ASSERT_LINE: error: duplicate case value
  *      YOUR_SOURCE_FILE.c:YOUR_ASSERT_LINE: error: previously used here
  */
#define AUDIOCOMMS_COMPILE_TIME_ASSERT(expr)   \
    switch(0) { \
        case 0: \
        case expr: \
        ; \
    }

/**
 * The macro AUDIO_COMMS_MKSTR(a) makes a string from an integer value.
 * For example, the AUDIO_COMMS_MKSTR(11) will be replaced by "11".
 *
 * The important point here is that we need 2 macros to do this job properly on
 * constants given by #define.
 * For example if we have #define AUDIO 11 , the
 * AUDIO_COMMS_MK_ESCAPED_STR(AUDIO) will be replaced by \"AUDIO\" and _NOT_
 * "AUDIO". The difference is that calling it in
 * printf(AUDIO_COMMS_MK_ESCAPED_STR(AUDIO)); will be replaced by
 * printf(\"AUDIO\"); and that is obviously not correct (\"AUDIO\" is a string,
 * but a string must be surrounded by "", and that is not the case.
 *
 * So the aim of calling this macro through another one is that the second
 * macro will interpret \" as " and give the good result "AUDIO". So even if
 * this is quite strange these two macros must be kept.
 *
 * This macro is used for example in the following situation :
 * #define AUDIO 11
 * char toto[AUDIO+1];
 * sscanf(str, "%s", toto); <- does not specify the max size of the string
 * sscanf(str, "%"AUDIO_COMMS_MKSTR(AUDIO)"s", toto); does the trick because
 * will be replaced by sscanf(str, "%11s", toto);
 * or to make string of filename:line number:
 *   __BASE_FILE__ ":" audio_comms_mkstr(__LINE__)
 *
 * @param[in] x   expression to make string of
 */
#define AUDIO_COMMS_MK_ESCAPED_STR(x) #x
#define AUDIO_COMMS_MKSTR(x) AUDIO_COMMS_MK_ESCAPED_STR(x)

#include <stdio.h>
#include <stdlib.h> /* assert */
/**
 * Checks a condition and abort if it is not met.
 * This assert is ALWAYS activated (even in release mode), thus it must be used
 * wisely.
 *
 * @param[in] cond  the condition to check
 * @param[in] fmt   format of the error to display (printf like)
 * @param[in] args  arguments of the error to display
 */
#define AUDIOCOMMS_ASSERT(cond, fmt, args...) \
 do { \
   if (audio_comms_unlikely(!(cond))) { \
       fprintf(stderr, __BASE_FILE__ ":" audio_comms_mkstr(__LINE__) \
               ": Assertion " #cond " failed: " fmt, ## args); \
       fprintf(stderr, "\n"); \
       abort(); \
   } \
 } while (0)

#endif // __cplusplus

#endif // AUDIOCOMMS_ASSERT_H

