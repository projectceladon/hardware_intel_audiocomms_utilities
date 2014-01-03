/*
 * INTEL CONFIDENTIAL
 *
 * Copyright 2013 Intel Corporation
 * All Rights Reserved.
 *
 * The source code contained or described herein and all documents related
 * to the source code ("Material") are owned by Intel Corporation or its
 * suppliers or licensors.
 * Title to the Material remains with Intel Corporation or its suppliers and
 * licensors. The Material contains trade secrets and proprietary and
 * confidential information of Intel or its suppliers and licensors.
 * The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */
#pragma once

#include "serializer/framework/ConvertToString.hpp"
#include <convert.hpp>
#include <string>

namespace audio_comms
{
namespace cme
{
namespace serializer
{

/** Policy class that handle T from/to string conversion. */
template <class T>
struct Convertor
{
    static bool toString(const T &raw, std::string &str)
    {
        return audio_comms::cme::serializer::toString(raw, str);
    }
    static bool fromString(const std::string &str, T &raw)
    {
        return utilities::convertTo(str, raw);
    }
};

/** Specialisation for string, conversion is forwarding. */
template <>
struct Convertor<std::string>
{
    static bool toString(const std::string &raw, std::string &str)
    {
        str = raw;
        return true;
    }
    static bool fromString(const std::string &str, std::string &raw)
    {
        raw = str;
        return true;
    }
};


/** Policy class conversion with a cast from RealType to ConversionType.
 *
 * When using Convertor convert first RealType to ConversionType.
 */
template <class RealType, class ConversionType>
struct CastConvertor
{
    static bool toString(const RealType &raw, std::string &str)
    {
        return Convertor<ConversionType>::toString(
            reinterpret_cast<const ConversionType &>(raw),
            str);
    }
    static bool fromString(const std::string &str, RealType &raw)
    {
        return Convertor<ConversionType>::fromString(
            str,
            reinterpret_cast<ConversionType &>(raw));
    }
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
