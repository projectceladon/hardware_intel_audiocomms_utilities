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

/** Getter helper
 * Transforms a method getter to a function getter.
 * If M is already a function getter forwards it.
 */
template <class M, M m>
struct GetterHelper;

// Method getter
template <class Ret, class Class, Ret(Class::*getter) ()>
struct GetterHelper<Ret(Class::*) (), getter>
{
    typedef Ret (*type) (Class &instance);
    static Ret function(Class &instance)
    {
        return (instance.*getter)();
    }
};

// Const method getter
template <class Ret, class Class, Ret(Class::*getter) () const>
struct GetterHelper<Ret(Class::*) () const, getter>
{
    typedef Ret (*type) (const Class &);
    static Ret function(const Class &instance)
    {
        return (instance.*getter)();
    }
};

// Function getter
template <class Ret, class Class, Ret(*getter) (Class)>
struct GetterHelper<Ret (*)(Class), getter>
{
    typedef Ret (*type) (Class &);
    static Ret function(Class &instance)
    {
        return (*getter)(instance);
    }
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
