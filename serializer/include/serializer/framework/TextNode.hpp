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

#include "serializer/framework/ConvertorPolicy.hpp"

namespace audio_comms
{
namespace cme
{
namespace serializer
{

namespace detail
{
/** Define the default convertor for text nodes. */
template <typename T>
struct TextNodeDefaultConvertor
{
    typedef Convertor<T> Type;
};
} // namespace detail

/** Use this template to create the trait of stringable types.
 *
 * Used mainly to serialize basic types.
 */
template <typename Type,
          class Convertor = typename detail::TextNodeDefaultConvertor<Type>::Type>
struct TextTrait
{
    typedef Type Element;
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
