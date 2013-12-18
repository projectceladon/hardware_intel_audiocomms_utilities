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

#include "IRemoteParameter.hpp"
#include <AudioCommsAssert.hpp>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>

/**
 * Server side Remote Parameter object.
 * This class may be used if parameter handled as an array of char
 */
class RemoteParameterBase : public IRemoteParameter
{
public:
    RemoteParameterBase(const std::string &parameterName, uint32_t size)
        : _name(parameterName),
          _size(size)
    {}

    virtual ~RemoteParameterBase() {}

    /**
     * Get the name of the parameter.
     *
     * @return name of the parameter.
     */
    const std::string getName() const { return _name; }

    /**
     * Get the size of the parameter.
     *
     * @return size of the parameter.
     */
    uint32_t getSize() const { return _size; }

private:
    /**
     * Accessors from IRemoteParameter interface.
     */
    virtual bool set(const uint8_t *data, uint32_t size) = 0;
    virtual void get(uint8_t *data, uint32_t size) const = 0;

    std::string _name; /**< Parameter Name. */
    uint32_t _size; /**< Parameter Size. */
};

/**
 * Server side Remote Parameter template.
 * This class may be used if parameter handled as an typed parameter.
 */
template <typename TypeParam>
class RemoteParameter : public RemoteParameterBase
{
public:
    RemoteParameter(const std::string &parameterName)
        : RemoteParameterBase(parameterName, sizeof(TypeParam))
    {}

    /**
     * Set typed parameter.
     *
     * @tparam type of the parameter
     * @param[in] data value to set
     *
     * @return true if set is successful, false otherwise.
     */
    virtual bool set(const TypeParam &data) = 0;

    /**
     * Get typed parameter.
     *
     * @tparam type of the parameter
     * @param[out] data value to get
     *
     * @return true if set is successful, false otherwise.
     */
    virtual void get(TypeParam &data) const = 0;
};

/**
 * Server side Remote Parameter template specialization.
 * This class may be used if parameter handled as an unsigned integer
 */
template <>
class RemoteParameter<uint32_t> : public RemoteParameterBase
{
public:
    RemoteParameter(const std::string &parameterName)
        : RemoteParameterBase(parameterName, sizeof(uint32_t))
    {}

    /**
     * Set unsigned integer parameter.
     *
     * @param[in] data value to set
     *
     * @return true if set is successful, false otherwise.
     */
    virtual bool set(const uint32_t &data) = 0;

    /**
     * Get unsigned integer parameter.
     *
     * @param[out] data value to set
     *
     * @return true if set is successful, false otherwise.
     */
    virtual void get(uint32_t &data) const = 0;

private:
    virtual bool set(const uint8_t *data, uint32_t size)
    {
        uint32_t typedData;
        AUDIOCOMMS_ASSERT(size == sizeof(typedData), "received size does not match");
        memcpy(static_cast<void *>(&typedData), static_cast<const void *>(data), sizeof(typedData));
        return set(typedData);
    }

    virtual void get(uint8_t *data, uint32_t size) const
    {
        uint32_t typedData;
        AUDIOCOMMS_ASSERT(size == sizeof(typedData), "received size does not match");
        get(typedData);
        memcpy(static_cast<void *>(data), static_cast<void *>(&typedData), sizeof(typedData));
    }
};
