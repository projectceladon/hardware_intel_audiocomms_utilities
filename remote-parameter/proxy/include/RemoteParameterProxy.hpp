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

#include "RemoteParameterProxyImpl.hpp"
#include <string>
#include <stdint.h>
#include <sys/types.h>

/**
 * Client side Remote Parameter template.
 * This class may be used if parameter handled as an typed parameter.
 */
template <typename TypeParam>
class RemoteParameterProxy : public RemoteParameterProxyImpl
{
public:
    RemoteParameterProxy(const std::string &parameterName)
        : RemoteParameterProxyImpl(parameterName, sizeof(TypeParam))
    {}

    /**
     * Set Parameter accessor
     *
     * @tparam TypeParam type of the parameter
     * @param[in] data parameter value to set
     * @param[out] error human readable error, set if return code is false.
     *
     * @return true if success, false otherwise and error code is set.
     */
    bool set(const TypeParam &data, std::string &error);

    /**
     * Get Parameter accessor
     *
     * @tparam TypeParam type of the parameter
     * @param[out] data parameter value to get
     * @param[out] error human readable error, set if return code is false.
     *
     * @return true if success, false otherwise and error code is set.
     */
    bool get(TypeParam &data, std::string &error);
};

/**
 * Client side Remote Parameter template specialization.
 * This class may be used if parameter handled as an unsigned integer
 */
template <>
class RemoteParameterProxy<uint32_t>  : public RemoteParameterProxyImpl
{
public:
    RemoteParameterProxy(const std::string &parameterName)
        : RemoteParameterProxyImpl(parameterName, sizeof(uint32_t))
    {}

    /**
     * Set as an integer Parameter accessor
     *
     * @param[in] data parameter value to set
     * @param[out] error human readable error, set if return code is false.
     *
     * @return true if success, false otherwise and error code is set.
     */
    bool setInteger(const uint32_t &data, std::string &error)
    {
        return write(reinterpret_cast<const uint8_t *>(&data), sizeof(data), error);
    }

    /**
     * Get as an integer Parameter accessor
     *
     * @param[out] data parameter value to get
     * @param[out] error human readable error, set if return code is false.
     *
     * @return true if success, false otherwise and error code is set.
     */
    bool getInteger(uint32_t &data, std::string &error)
    {
        return read(reinterpret_cast<uint8_t *>(&data), sizeof(data), error);
    }
};
