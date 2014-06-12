/**
 * @section License
 *
 * Copyright 2013-2014 Intel Corporation
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

#include "xmlserializer/Result.hpp"
#include "xmlserializer/XmlSerializer.hpp"
#include "serializer/Serializer.hpp"
#include "serializer/framework/TextNode.hpp"
#include "serializer/framework/NamedTextTrait.hpp"
#include "serializer/framework/Collection.hpp"
#include <utilities/UniquePtr.hpp>


namespace audio_comms
{
namespace utilities
{
namespace xmlserializer
{

class XmlSerializer : public serializer::Serializer<Result>
{
public:
    typedef xmlserializer::Result Result;

    template <class Class>
    static Result serialize(const Class &c, std::string &serializedCmd)
    {
        TiXmlOutStream stream;
        TiXmlDocument doc;
        doc.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));
        TiXmlElement root("ACME");
        root.SetAttribute("version", "0.0.1");
        UniquePtr<TiXmlNode> xmlElement;
        Result res = XmlTraitSerializer<serializer::ClassSerializationTrait<Class> >
                     ::toXml(c, xmlElement.getRefToSet());

        if (res.isFailure()) {
            serializedCmd = "";
            return res;
        }

        root.LinkEndChild(xmlElement.release());

        doc.InsertEndChild(root);

        stream << doc;
        serializedCmd = stream.c_str();
        return Result::success();
    }

    template <class Class>
    static Result deserialize(const std::string &str, Class &c)
    {
        TiXmlDocument doc;
        doc.Parse(str.c_str(), NULL);

        const TiXmlNode *acmeTag = doc.FirstChildElement("ACME");
        if (acmeTag == NULL) {
            return Result(conversionFailed) << "No ACME root element.";
        }

        /* Sanity check, there should NOT be any other node after de command's
         * one */
        if (acmeTag->NextSiblingElement() != NULL) {
            return Result(conversionFailed) << "No command tag found.";
        }

        return XmlTraitSerializer<serializer::ClassSerializationTrait<Class> >::fromXml(
            *acmeTag->FirstChild(),
            c);
    }
};

} // namespace xmlserializer
} // namespace utilities
} // namespace audio_comms
