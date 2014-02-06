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

namespace audio_comms
{
namespace cme
{
namespace serializer
{

/** Trait that can be used to serialized any Collection instance.
 *
 * tparam _tag names the collection
 * tparam Collection is the collection type to serialize. It should implement
 *                   like some stl containors (as std::list):
 *                   - begin, end and push_back methods
 *                   - iterator, const_iterator and value_type types
 * tparam _ItemTrait is a trait to serialize Collection::value_type.
 * tparam takeOwnership if the Collection::push_back method takes the ownership
 *                      of the pushed item (ie. if it should not be deleted
 *                      after), @see Child.
 */
template <const char *_tag, class Collection, class _ItemTrait,
          bool takeOwnership = false>
struct CollectionTrait
{
    static const char *tag;
    typedef Collection Element;
};
template <const char *_tag, class C, class I, bool o>
const char *CollectionTrait<_tag, C, I, o>::tag = _tag;

} // namespace serializer
} // namespace cme
} // namespace audio_comms
