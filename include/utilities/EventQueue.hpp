/* @file
 * Event queue class
 *
 * @section License
 *
 * Copyright 2014-2017 Intel Corporation
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

#include <Mutex.hpp>
#include <Semaphore.hpp>
#include <queue>

namespace audio_utilities
{
namespace utilities
{

/* Helper class to handle event in a queue.
 * This class IS thread safe but does not guaranty equal repartition between concurrent threads
 * that may be waiting simultaneously.
 * \tparam Event  the type of event to store in the queue
 */
template <class Event>
class EventQueue
{
public:
    EventQueue() : mEventSemaphore(0) {}

    /** Post a new event in the queue
     * \param[in] event  the event to post in queue
     */
    void post(Event event)
    {
        audio_utilities::utilities::Mutex::Locker locker(mQueueMutex);
        mEventQueue.push(event);
        mEventSemaphore.post();
    }

    /** Wait for a new event
     *  This function blocks the caller until a new event is available.
     */
    Event wait()
    {
        mEventSemaphore.wait();
        audio_utilities::utilities::Mutex::Locker locker(mQueueMutex);
        Event event = mEventQueue.front();
        mEventQueue.pop();
        return event;
    }

private:
    audio_utilities::utilities::Mutex mQueueMutex; /*< queue lock */
    std::queue<Event> mEventQueue; /*< queue itself */
    audio_utilities::utilities::Semaphore mEventSemaphore; /*< event semaphore */
};

} // utilities
} // audio_utilities
