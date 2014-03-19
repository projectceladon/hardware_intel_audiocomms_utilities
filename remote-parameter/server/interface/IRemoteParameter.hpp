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

#include <stdint.h>
#include <sys/types.h>

class IRemoteParameter
{
public:
    /**
     * Set parameter.
     *
     * @param[in] data value to set (as a buffer).
     * @param[in] size in bytes of the buffer to set.
     *
     * @return true if set is successful, false otherwise.
     */
    virtual bool set(const uint8_t *data, size_t size) = 0;

    /**
     * Get parameter.
     *
     * @param[out] data value to be written to (as a buffer).
     * @param[in] size in bytes of the buffer to get.
     *
     * @return true if get is successful, false otherwise.
     */
    virtual void get(uint8_t *data, size_t &size) const = 0;

protected:
    virtual ~IRemoteParameter() {}
};
