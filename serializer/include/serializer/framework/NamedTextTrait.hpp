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

#include "serializer/framework/TextNode.hpp"
#include "serializer/framework/Child.hpp"
#include "common/TypeList.hpp"

namespace audio_comms
{
namespace cme
{
namespace serializer
{

/** Wrap a text node around xml element.
 *
 * Sometime it is useful to surround a text node with an xml element.
 * Example:
 *  - using a TextTrait<int> will be serialized to "20".
 *  - using a NamedTextTrait<int, "num"> will be serialized in "<num>20</num>"
 */
template <typename T, const char *_tag,
          class Convertor = typename detail::TextNodeDefaultConvertor<T>::Type>
struct NamedTextTrait
{
    static const char *tag;
    typedef T Element;

    static const T &get(const Element &e) { return e; }

    static void set(Element &e, const T &newValue) { e = newValue; }

    typedef const T & (*Get)(const Element &);
    typedef void (*Set)(Element &, const T &);

    typedef Child<TextTrait<T, Convertor>,
                  Get, &NamedTextTrait<T, _tag>::get,
                  Set, &NamedTextTrait<T, _tag>::set, false> Child1;
    typedef TYPELIST1 (Child1) Children;
};
template <typename T, const char *_tag, class C>
const char *NamedTextTrait<T, _tag, C>::tag = _tag;

} // namespace serializer
} // namespace cme
} // namespace audio_comms
