/* EventThread.h
**
** Copyright 2011-2014 Intel Corporation
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
#pragma once

#include <stdint.h>

class IEventListener
{
public:
    virtual bool onEvent(int iFd) = 0; // return true if Fd sets has changed
    virtual bool onError(int iFd) = 0; // return true if Fd sets has changed
    virtual bool onHangup(int iFd) = 0; // return true if Fd sets has changed
    virtual void onAlarm() = 0;
    virtual void onPollError() = 0;

    /**
     * Callback upon a trig request to process an event from the event thread context
     * (to avoid borrow the context of a caller for example).
     *
     * @param[in] context pointer that was given by the client of the event thread through trig.
     *                    Note that is may be NULL.
     * @param[in] eventId that was given by the client of the event thread through trig.
     *                    NOTE: this parameter is DEPRECATED, for maintained for compatibility.
     *
     * @return true if the list of file descripter polled has changed, false otherwise.
     * @deprecated Using this function with 2 parameters is deprecated, only context shall remain
     *             as a parameter of this function.
     */
    virtual bool onProcess(void *context, uint32_t eventId) = 0;

protected:
    virtual ~IEventListener() {}
};
