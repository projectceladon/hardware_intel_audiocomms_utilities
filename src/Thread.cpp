/* Thread.cpp
**
** Copyright 2013-2015 Intel Corporation
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "utilities/Thread.hpp"
#include <sys/prctl.h>
#include <AudioCommsAssert.hpp>
#include <sys/syscall.h>
#include <sstream>

namespace audio_comms
{
namespace utilities
{

/**
 * This function is just a helper that is used to be passed to pthread_create
 * which expects a void *(*)(void *) function. The thread object MUST be passed
 * as argument.
 * @param[in] arg  a pointer on the thread object (this is mandatory)
 */
void *Thread::threadMainHelper(void *arg)
{
    AUDIOCOMMS_ASSERT(arg != NULL, "argument is expected to be the thread object.");
    Thread *t = reinterpret_cast<typeof t>(arg);
    t->loop();
    return NULL;
}

bool Thread::start()
{
    if (_thread != 0) {
        /* Thread is already started */
        return false;
    }

    return pthread_create(&_thread, NULL, threadMainHelper, this) == 0;
}

void Thread::stop()
{
    _stopRequested = true;
    shutdown();
    /* You may notice that there is no pthread_cancel() in here. This is done
     * on purpose and expects the processing function to be non blocking. */
    pthread_join(_thread, NULL);
    _thread = 0;
    _stopRequested = false;
}

void Thread::selfAbort()
{
    _stopRequested = true;
}

bool Thread::setDebugName()
{
    if (mName.empty()) {
        // The thread name is inherited from is parent if it is not set
        return true;
    }

    // pthread_setname_np should be used but is not available
    // in android's pthread host version.
    return prctl(PR_SET_NAME, mName.c_str()) == 0;
}

std::string Thread::getCurrentThreadName()
{
    char name[MAX_TASK_COMM_LEN + 1];
    // pthread_getname_np should be used but is not available
    // in android's pthread host version.
    prctl(PR_GET_NAME, name);
    name[MAX_TASK_COMM_LEN] = '\0';
    return name;
}

std::string Thread::getTid()
{
    std::ostringstream oss;
    oss << syscall(__NR_gettid);
    return oss.str();
}

} // namespace utilities
} // namespace audio_comms
