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

#include <string>

namespace audio_comms
{
namespace cme
{
namespace serializer
{

/**
 * This template class has to be specialized to describe the way the class
 * Class has to be serialized
 */
template <class Class>
class ClassSerializationTrait;

template<class Result>
class Serializer
{
public:
    template <class Class>
    static Result serialize(const Class &c, std::string &serializedCmd);

    template <class Class>
    static Result deserialize(const std::string &str, Class &c);
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
