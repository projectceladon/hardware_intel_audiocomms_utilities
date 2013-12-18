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

/** @TODO: Put in common ? */

/** Typelist to be used in metaprograming.
 *
 * Sometime it is usefull to get multiple types as template param. As this is
 * not part of the syntax (at least before c++11), type list solves this problem.
 *
 * To iterate on a Typelist, use head tail recursion and pattern matching.
 */
template <class H, class T>
struct TypeList
{
    typedef H head;
    typedef T tail;
};

namespace detail
{
/** The last element of a TypeList is always a TypeNull.
 *
 * This type is implementation specific, and should never be referenced directly.
 */
struct TypeNull;
} // detail

#define TYPELIST0 \
    audio_comms::cme::detail::TypeNull
#define TYPELIST1(A) \
    audio_comms::cme::TypeList < A, TYPELIST0 >
#define TYPELIST2(A, B) \
    audio_comms::cme::TypeList < A, TYPELIST1(B) >
#define TYPELIST3(A, B, C) \
    audio_comms::cme::TypeList < A, TYPELIST2(B, C) >
#define TYPELIST4(A, B, C, D) \
    audio_comms::cme::TypeList < A, TYPELIST3(B, C, D) >
#define TYPELIST5(A, B, C, D, E) \
    audio_comms::cme::TypeList < A, TYPELIST4(B, C, D, E) >
#define TYPELIST6(A, B, C, D, E, F) \
    audio_comms::cme::TypeList < A, TYPELIST5(B, C, D, E, F) >
#define TYPELIST7(A, B, C, D, E, F, G) \
    audio_comms::cme::TypeList < A, TYPELIST6(B, C, D, E, F, G) >
#define TYPELIST8(A, B, C, D, E, F, G, H) \
    audio_comms::cme::TypeList < A, TYPELIST7(B, C, D, E, F, G, H) >
#define TYPELIST9(A, B, C, D, E, F, G, H, I) \
    audio_comms::cme::TypeList < A, TYPELIST8(B, C, D, E, F, G, H, I) >
#define TYPELIST10(A, B, C, D, E, F, G, H, I, J) \
    audio_comms::cme::TypeList < A, TYPELIST9(B, C, D, E, F, G, H, I, J) >
#define TYPELIST11(A, B, C, D, E, F, G, H, I, J, K) \
    audio_comms::cme::TypeList < A, TYPELIST10(B, C, D, E, F, G, H, I, J, K) >

} // namespace cme
} // namespace audio_comms
