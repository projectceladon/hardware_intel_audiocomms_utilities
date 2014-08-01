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

#include <poll.h>
#include <pthread.h>
#include <stdint.h>
#include <stddef.h>
#include <list>

using namespace std;

class IEventListener;

class CEventThread
{
private:
    enum PipeMsg
    {
        EProcess,
        EExit,

        ENbPipeMsg
    };

    struct Message
    {
        void *context;
        uint32_t eventId;
        uint32_t msg;
    };

    struct SFd
    {
        SFd(uint32_t uiClientFdId, int iFd,
            bool bToListenTo) : _uiClientFdId(uiClientFdId), _iFd(iFd), _bToListenTo(bToListenTo) {}
        uint32_t _uiClientFdId;
        int _iFd;
        bool _bToListenTo;
    };

    typedef list<SFd>::iterator SFdListIterator;
    typedef list<SFd>::const_iterator SFdListConstIterator;

public:
    // Construction
    CEventThread(IEventListener *pEventListener, bool bLogsOn = true);
    ~CEventThread();
    // Add open FDs
    void addOpenedFd(uint32_t uiFdClientId, int iFd, bool bToListenTo = false);

    // Get FD
    int getFd(uint32_t uiClientFdId) const;

    // Remove and close all FDs
    void closeAndRemoveFds();

    // Remove and close FD from index
    void closeAndRemoveFd(uint32_t uiClientFdId);

    /// Alarm management
    // Start an alarm which will trig onAlarm() in 'last' ms from now (must be called from the EventThread thread context)
    void startAlarm(uint32_t uiDurationMs);
    // Clear the alarm (must be called from the EventThread thread context)
    void cancelAlarm();

    // Thread start
    bool start();
    // Thread stop
    void stop();

    /**
     * Returns the state of the event thread. When started, i.e. the event thread is listening
     * to event from its list of file descriptor and from its pipe, no file descriptor can be
     * added outside the context of the event thread. The only way to change this list is to append
     * remove file descriptor upon call back of the event thread and returning true to notify
     * of the change.
     *
     * @return true if started false otherwise.
     */
    bool isStarted() const { return _bIsStarted; }

    /**
     * Trig an execution context change. The caller of this function has to process an event within
     * theevent thread context in order to avoid borrowing the execution context of another thread
     * for example.
     *
     * @param[in] context pointer given by the client of the event thread.
     *                    Note that is may be NULL.
     * @param[in] eventId given by the client of the event thread.
     *                    NOTE: this parameter is DEPRECATED, for maintained for compatibility.
     * @deprecated Using this function with 2 parameters is deprecated, only context shall remain
     *             as a parameter of this function.
     */
    void trig(void *context, uint32_t eventId = -1);

    // Context check
    bool inThreadContext() const;

    // Logs activation
    void setLogsState(bool bLogsOn);
    bool isLogsOn() const;

private:
    // Thread
    static void *thread_func(void *pData);
    // Run
    void run();
    // Add FD
    void addListenedFd(int iFd);
    // Remove FD
    void removeListenedFd(int iFd);
    // Poll FD computation
    void buildPollFds(struct pollfd *paPollFds) const;
    // Get current date in milliseconds
    static int64_t getCurrentDateMs();

private:
    // Listener
    IEventListener *_pEventListener;
    // State
    bool _bIsStarted; /**< State of the event thread. */
    // Thread id
    pthread_t _ulThreadId;
    // Inband pipe
    int _aiInbandPipe[2];
    // FD list
    list<SFd> _sFdList;
    // Polled FD count
    uint32_t _uiNbPollFds;
    // Alarm date in milliseconds
    int64_t _iAlarmMs;
    // Thread context
    bool _bThreadContext;
    // EventThread logs enabled
    bool _bLogsOn;
};
