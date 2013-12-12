/*
 ********************************************************************************
 *                              INTEL CONFIDENTIAL
 *   Copyright(C) 2013 Intel Corporation. All Rights Reserved.
 *   The source code contained  or  described herein and all documents related to
 *   the source code ("Material") are owned by Intel Corporation or its suppliers
 *   or licensors.  Title to the  Material remains with  Intel Corporation or its
 *   suppliers and licensors. The Material contains trade secrets and proprietary
 *   and  confidential  information of  Intel or its suppliers and licensors. The
 *   Material  is  protected  by  worldwide  copyright  and trade secret laws and
 *   treaty  provisions. No part of the Material may be used, copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way without Intel's prior express written permission.
 *   No license  under any  patent, copyright, trade secret or other intellectual
 *   property right is granted to or conferred upon you by disclosure or delivery
 *   of the Materials,  either expressly, by implication, inducement, estoppel or
 *   otherwise.  Any  license  under  such  intellectual property  rights must be
 *   express and approved by Intel in writing.
 *
 ********************************************************************************
 */

#include "common/Thread.hpp"

#include <AudioCommsAssert.hpp>

namespace audio_comms
{
namespace cme
{
namespace common
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
}


} // namespace audio_comms
} // namespace cme
} // namespace common
