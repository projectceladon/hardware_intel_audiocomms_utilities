/*
 * @file
 * Mutual exclusion class an utilities.
 *
 * @section License
 *
 * Copyright 2015-2017 Intel Corporation
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

#include "AudioUtilitiesAssert.hpp"
#include "Mutex.hpp"

#include <pthread.h>
#include <cerrno>
#include <string.h>

namespace audio_utilities
{
namespace utilities
{

class ConditionVariable
{
public:
    ConditionVariable()
    {
        int err = pthread_cond_init(&_cond, NULL);
        AUDIOUTILITIES_ASSERT(err == 0, "Unable to initialize condition variable @"
                          << ": " << strerror(err) << "(" << err << ")");
    }

    ~ConditionVariable()
    {
        int err = pthread_cond_destroy(&_cond);
        AUDIOUTILITIES_ASSERT(err == 0, "Unable to destroy condition variable @"
                          << static_cast<const void *>(&_cond)
                          << ": " << strerror(err) << "(" << err << ")");
    }

    /**
     * Unblocks at least one of the threads that are blocked on the specified condition variable
     */
    void signal()
    {
        int err = pthread_cond_signal(&_cond);
        AUDIOUTILITIES_ASSERT(err == 0, "Unable to signal condition variable @"
                          << static_cast<const void *>(&_cond)
                          << ": " << strerror(err) << "(" << err << ")");
    }

    /**
     * Unblocks all threads that are blocked on the specified condition variable
     */
    void broadcast()
    {
        int err = pthread_cond_broadcast(&_cond);
        AUDIOUTILITIES_ASSERT(err == 0, "Unable to broadcast condition variable @"
                          << static_cast<const void *>(&_cond)
                          << ": " << strerror(err) << "(" << err << ")");
    }

    /**
     * block the caller on a condition variable
     *
     * @param[in|out] _mutex The mutex that will be unlock when the waiting starts. It will be
     *                       lock again the the waiting ends.
     */
    void wait(Mutex &mutex)
    {
        int err = pthread_cond_wait(&_cond, &mutex._mutex);
        AUDIOUTILITIES_ASSERT(err == 0, "Unable to wait for condition variable @"
                          << static_cast<const void *>(&_cond)
                          << ": " << strerror(err) << "(" << err << ")");
    }

private:
    pthread_cond_t _cond; /**< internal condition variable*/
};

}
}
