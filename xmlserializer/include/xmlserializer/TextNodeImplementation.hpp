/**
 * @section License
 *
 * Copyright 2013-2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "serializer/framework/TextNode.hpp"

namespace audio_utilities
{
namespace utilities
{
namespace xmlserializer
{

namespace detail
{
/** Specialisation when accessing to a text node. */
template <class TextType, class Convertor>
struct ChildAccess<serializer::TextTrait<TextType, Convertor> >
{
    static Result get(const tinyxml2::XMLNode &xmlParent, const tinyxml2::XMLNode * &xmlChild)
    {
        xmlChild = xmlParent.FirstChild();
        // The node may have children (a comment node for instance): only the first text node child
        // should be the one we want to deserialize.
        while (xmlChild != NULL) {

            if (xmlChild->ToText()) {

                return Result::success();
            }
            xmlChild = xmlChild->NextSibling();
        }
        return Result(childNotFound) << "Node \"" << xmlParent.Value()
                                     << "\" does not contain an expected text node child";
    }
};

} // namespace detail

/** Specialisation for Text nodes. */
template <class TextType, class Convertor>
class XmlTraitSerializer<serializer::TextTrait<TextType, Convertor> >
{
public:
    static Result toXml(const TextType &rawtext, tinyxml2::XMLNode * &xmlText)
    {
        std::string text;
        if (not Convertor::toString(rawtext, text)) {
            return Result(conversionFailed) << "Unable to convert \"" << &rawtext
                                            << "\" to string.";
        }
        tinyxml2::XMLDocument doc;
        xmlText = doc.NewText(text.c_str());
        return Result::success();
    }

    static Result fromXml(const tinyxml2::XMLNode &xmlText, TextType &rawText)
    {
        const tinyxml2::XMLText *textNode = xmlText.ToText();
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

} // namespace xmlserializer
} // namespace utilities
} // namespace audio_utilities
