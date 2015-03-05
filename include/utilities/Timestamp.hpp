/* @section License
 *
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

#include <stdint.h>
#include <string>

/** Represent a timestamp aquired from a clock. */
template <clockid_t clock = CLOCK_MONOTONIC>
class Timestamp
{
public:
    /** Construct a timestamp from the current time. */
    Timestamp() : mTime(getCurrentTime()) {}

    /** Construct a timestamp from a microsecond time. */
    explicit Timestamp(uint64_t time) : mTime(time) {}

    /** @return the managed timestamp in microsecond. */
    uint64_t get() const { return mTime; }

    /** @return the current time in microsecond. */
    static uint64_t getCurrentTime()
    {
        struct timespec tp;
        clock_gettime(clock, &tp);
        return tp.tv_sec * 1000000 + tp.tv_nsec / 1000;
    }

private:
    const uint64_t mTime;
};
