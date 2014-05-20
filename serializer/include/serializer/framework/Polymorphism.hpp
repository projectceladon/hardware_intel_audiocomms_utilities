/*
 * INTEL CONFIDENTIAL
 *
 * Copyright 2014 Intel Corporation
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

/** Trait to serialize a polymorphic pointer.
 *
 * NULL pointers are NOT supported.
 *
 * A polymorphic pointer is a pointer with a dynamic type of Base but a
 * static type of one of Base's derived class (or itself).
 * To serialize it, it's static type must be identified. As it can not be found
 * implicitly, the user must provide the expected possible morphisms.
 *
 * @tparam tag is a string identifying the polymorphism pointer described in this trait.
 * @tparam Base* is the dynamic type of the polymorphic pointer.
 * @tparam SuportedClasslist is a trait list (TypeList) describing
 *         the supported pointer morphisms.
 *         Each element of this list (the morphism traits) must contain the
 *         following static fields:
 *           - Derived is a class that can be static_cast to and from Base.
 *           - DerivedTrait is the relevant trait to serialize the Derived class.
 *           - bool isOf (const Base&) function that,
 *                                     given the polymorphism pointer,
 *                                     returns true if the pointer can be
 *                                     safely static_cast to Derived and
 *                                     serialized using the DerivedTrait.
 *                                     I.e. that serializing the pointer as
 *                                     a class Derived is lossless.
 *
 * The meaning of the Child::takeOwnership changes when Child::ChildTrait is a
 * PolymorphismTrait. Usually it is true if the setter takes the Trait::Element's
 * ownership, false otherwise.
 * A setter receives an Trait::Element&, aka Base *& for a PolymorphismTrait.
 * Nevertheless, a polymorphic setter must NOT take the ownership of the
 * Trait::Element, aka Base *, aka the pointer, but can only take the ownership
 * of the pointee.
 * If the pointee ownership is taken, Child::takeOwnership must be true.
 *
 * Example:
 *   - Given a base class B and two derived class D1 and D2.
 *   - Given the BTrait, D1Trait, D2Trait, the traits to serialize those objects.
 *
 *   The corresponding BRefTrait to serialize a polymorphic pointer to B would be:
 *
 * @code{.cpp}
 *
 *   // Starting by defining the morphism traits.
 *   class BMorphTrait {
 *       typedef BTrait DerivedTrait;
 *       bool isOf(const B &b) {
 *           // isOfTypeB is an arbitrary function that should return true
 *           // if b is really (static type) a B.
 *           // It can be implemented using rtti, enum or any other way.
 *           return isOfTypeB(b);
 *       }
 *   class D1MorphTrait {
 *       typedef D1Trait DerivedTrait;
 *       bool isOf(const B &b) { return isOfTypeD1(b); }
 *   }
 *   class D2MorphTrait {
 *       typedef D2Trait DerivedTrait;
 *       bool isOf(const B &b) { return isOfTypeD2(b); }
 *   }
 *
 *   // Define a tag to identify our polymorphic pointer
 *   const char BPolyTag[] = "BOrDerived";
 *
 *   // Define the Trait
 *   typedef PolymorphismTrait<BPolyTag, TYPELIST3(D1,D2,B) > BRefTrait;
 *
 * @endcode
 */
template <const char *mTag, class Base, class SuportedTypelist>
struct PolymorphismTrait
{
    /** Polymorphic pointer tag (unique identifier within siblings). */
    static const char *tag;
    /** Dynamic type of the polymorphic pointer. */
    typedef Base *Element;
};

template <const char *mTag, class B, class S>
const char *PolymorphismTrait<mTag, B, S>::tag = mTag;

} // namespace serializer
} // namespace cme
} // namespace audio_comms
