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
#include <sstream>


namespace audio_comms
{
namespace utilities
{
namespace xmlserializer
{

/**
 * XmlSerializer
 * Class that handles xml serialization/deserialization
 * The version numbering follows the Semantic Versioning scheme
 */
class XmlSerializer : public serializer::Serializer<Result>
{
public:
    typedef xmlserializer::Result Result;

    /**
     * Creates a XmlSerializer
     * This class expect a document to be of the format:
     *
     * <?xml version="1.0" ?>
     * <ROOT_ELEMENT_NAME version="1.2.3"> <! major is 1, minor 2, patch 3 >
     * [... some xml data ]
     * </ROOT_ELEMENT_NAME>
     *
     * @param[in] rootElementName  the name of the root element
     * @param[in] major   the major number of the xml format version
     * @param[in] minor   the minor number of the xml format version
     * @param[in] patch   the patch number of the xml format version
     */
    XmlSerializer(const std::string &rootElementName,
                  unsigned short major, unsigned short minor,
                  unsigned short patch)
        : mRootElementName(rootElementName)
    {
        std::ostringstream oss;
        oss << major << "." << minor << "." << patch;
        mVersion = oss.str();
    }

    template <class Class>
    Result serialize(const Class &c, std::string &serializedCmd)
    {
        TiXmlOutStream stream;
        TiXmlDocument doc;
        doc.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));
        TiXmlElement root(mRootElementName.c_str());

        root.SetAttribute("version", mVersion.c_str());

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

    /**
     * deserialize xml data contained in a string.
     * If the parser is successful, Result::success is returned ans the class passed as reference
     * is valid.
     * In case an error is returned, the class is NOT valid.
     * In case the version or the root element does not match, conversionFailed error is returned.
     *
     * @tparam Class   the class type to deserialize into
     * @param[in] str  the string containing the xml document
     * @param[out] c   the result class filled from the xml parsing
     *
     * @return a result describing the error
     */
    template <class Class>
    Result deserialize(const std::string &str, Class &c)
    {
        TiXmlDocument doc;
        doc.Parse(str.c_str(), NULL);

        const TiXmlElement *rootTag = doc.FirstChildElement(mRootElementName.c_str());
        if (rootTag == NULL) {
            return Result(conversionFailed) << "No '" << mRootElementName.c_str()
                                            << "' root element found in xml data.";
        }

        const char *version = rootTag->Attribute("version");
        if (version == NULL) {
            return Result(conversionFailed) << "No version found in root node '"
                                            << mRootElementName.c_str();
        }

        if (version != mVersion) {
            return Result(conversionFailed) << "Version does not match; expect '" << mVersion
                                            << "' got '" << version << "'.";
        }

        /* Sanity check, there should NOT be any other node after de command's one */
        if (rootTag->NextSiblingElement() != NULL) {
            return Result(conversionFailed) << "No command tag found.";
        }

        return XmlTraitSerializer<serializer::ClassSerializationTrait<Class> >::fromXml(
            *rootTag->FirstChild(),
            c);
    }

private:
    std::string mRootElementName;
    std::string mVersion;
};

} // namespace xmlserializer
} // namespace utilities
} // namespace audio_comms
