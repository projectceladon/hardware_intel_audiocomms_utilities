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

#include "serializer/framework/Polymorphism.hpp"

namespace audio_comms
{
namespace cme
{
namespace xmlserializer
{

namespace detail
{

/** Travel through PolymorphismTrait's SuportedTypelist
 * to find a matching morphism and {de}serialize it.
 */
template <class Base, class SuportedTypelist>
class Morph;

/** Pattern match polymorphic trait and handle it. */
template <const char *tag, class Base, class SuportedTypelist>
class SerializerChildren<serializer::PolymorphismTrait<tag, Base, SuportedTypelist> >
{
public:
    static Result toXml(const Base *const &element, TiXmlNode &xmlElement)
    {
        if (element == NULL) {
            return Result(invalidArgument) << "NULL pointer are not supported";
        }
        return detail::Morph<Base, SuportedTypelist>::toXml(*element, xmlElement)
               << " (While serializing polymorphic pointer)";
    }

    static Result fromXml(const TiXmlNode &xmlElement, Base * &element)
    {
        const TiXmlNode *xmlMorph = xmlElement.FirstChildElement();
        if (xmlMorph == NULL) {
            return Result(childNotFound) << "Expected a child node.";
        }
        if (xmlMorph->NextSiblingElement() != NULL) {
            return Result(childNotFound) << "Expected only one child node.";
        }
        return detail::Morph<Base, SuportedTypelist>::fromXml(*xmlMorph, element)
               << " (While deserializing polymorphic pointer)";
    }
};

template <class Base>
class Morph<Base, TYPELIST0>
{
public:
    static Result toXml(const Base &, TiXmlNode &)
    {
        return Result(invalidArgument) << "Unsupported morph";
    }

    static Result fromXml(const TiXmlNode &xmlMorph, Base * &)
    {
        return Result(invalidArgument)
               << "Tag \"" << xmlMorph.Value() << "\" is not a supported morph";
    }
};

template <class Base, class H, class T>
class Morph<Base, TypeList<H, T> >
{
public:
    static Result toXml(const Base &element, TiXmlNode &xmlPolyMorph)
    {
        if (not H::isOf(element)) {
            // Try the next supported type
            return Morph<Base, T>::toXml(element, xmlPolyMorph);
        }
        TiXmlNode *xmlMorph = NULL;
        // Now that the type is determined, continue serialisation
        // as a Derived
        Result res = XmlTraitSerializer<typename H::DerivedTrait>::toXml(
            downCast(element), xmlMorph);
        if (res.isFailure()) {
            return res;
        }
        xmlPolyMorph.LinkEndChild(xmlMorph);
        return Result::success();
    }

    static Result fromXml(const TiXmlNode &xmlMorph, Base * &element)
    {
        if (strcmp(H::DerivedTrait::tag, xmlMorph.Value()) != 0) {
            // Try the next supported type
            return Morph<Base, T>::fromXml(xmlMorph, element);
        }
        // Now that the type is determined, continue deserialisation
        // as a Derived
        element = new Derived;
        return XmlTraitSerializer<typename H::DerivedTrait>::fromXml(
            xmlMorph, downCast(*element));
    }

private:
    typedef typename H::DerivedTrait::Element Derived;

    /* Convert a Base to derived.
     *
     * @see PolymorphismTrait
     */
    static Derived &downCast(Base &base)
    {
        return static_cast<Derived &>(base);
    }

    /** Const version of downCast. */
    static const Derived &downCast(const Base &base)
    {
        return static_cast<const Derived &>(base);
    }
};


/** Pattern match polymorphic trait to handle the ownership particularity. */
template <const char *tag, class Base, class SuportedTypelist, bool takeOwnership>
struct Deleter<serializer::PolymorphismTrait<tag, Base, SuportedTypelist>, takeOwnership>
{
    static void del(Base **element)
    {
        if (not takeOwnership) {
            delete *element;
        }
        delete element;
    }
};


} // namespace detail
} // namespace serializer
} // namespace cme
} // namespace audio_comms
