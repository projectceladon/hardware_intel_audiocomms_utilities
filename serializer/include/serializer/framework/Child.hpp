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

#include "serializer/framework/GetterHelper.hpp"
#include "serializer/framework/SetterHelper.hpp"

namespace audio_comms
{
namespace cme
{
namespace serializer
{

/** Template used to describe a class child (member). */
template <typename _ChildTrait,
          typename _Getter, _Getter _getter,
          typename _Setter, _Setter _setter, bool _takeOwnership>
struct Child
{
    typedef _ChildTrait ChildTrait;
    typedef GetterHelper<_Getter, _getter> Getter;
    typedef SetterHelper<_Setter, _setter> Setter;
    static const bool takeOwnership = _takeOwnership;
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
