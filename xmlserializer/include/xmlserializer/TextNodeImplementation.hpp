/*
 * INTEL CONFIDENTIAL
 *
 * Copyright 2013-2014 Intel Corporation
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

namespace audio_comms
{
namespace cme
{
namespace xmlserializer
{

namespace detail
{
/** Specialisation when accessing to a text node. */
template <class TextType, class Convertor>
struct ChildAccess<serializer::TextTrait<TextType, Convertor> >
{
    static Result get(const TiXmlNode &xmlParent, const TiXmlNode * &xmlChild)
    {
        xmlChild = xmlParent.FirstChild();
        if (xmlChild == NULL) {
            return Result(childNotFound) << "Node \"" << xmlParent.Value()
                                         << "\" is empy, expected a text node child";
        }
        return Result::success();
    }
};

} // namespace detail

/** Specialisation for Text nodes. */
template <class TextType, class Convertor>
class XmlTraitSerializer<serializer::TextTrait<TextType, Convertor> >
{
public:
    static Result toXml(const TextType &rawtext, TiXmlNode * &xmlText)
    {
        std::string text;
        if (not Convertor::toString(rawtext, text)) {
            return Result(conversionFailed) << "Unable to convert \"" << &rawtext
                                            << "\" to string.";
        }
        xmlText = new TiXmlText(text.c_str());
        return Result::success();
    }

    static Result fromXml(const TiXmlNode &xmlText, TextType &rawText)
    {
        const TiXmlText *textNode = xmlText.ToText();
        if (textNode == NULL) {
            return Result(unexpectedNodeType) << "Expected an xml text node";
        }
        std::string text(textNode->Value());
        if (not Convertor::fromString(text, rawText)) {
            return Result(conversionFailed) << "Unable to convert from string \""
                                            << text << "\".";
        }
        return Result::success();
    }
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
