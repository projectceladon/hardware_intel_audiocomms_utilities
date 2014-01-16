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

#include "serializer/framework/Serializer.hpp"

namespace audio_comms
{
namespace cme
{
namespace serializer
{

class SerializerGeneric
{
public:
    /**
     * This template class has to be specialized to describe the way the class
     * Class has to be serialized
     */
    template <class Class>
    class ClassSerializationTrait;

    template <class Class>
    static Result serialize(const Class &c, std::string &serializedCmd)
    {
        TiXmlOutStream stream;
        TiXmlDocument doc;
        doc.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));
        TiXmlElement *root = new TiXmlElement("ACME");
        root->SetAttribute("version", "0.0.1");
        TiXmlNode *xmlElement;
        Result res = Serializer<ClassSerializationTrait<Class> >::toXml(c, xmlElement);
        if (res != Result::success()) {
            serializedCmd = "";
            return res;
        }

        root->LinkEndChild(xmlElement);

        doc.LinkEndChild(root);

        stream << doc;
        serializedCmd = stream.c_str();
        return Result::success();
    }

    template <class Class>
    static Result deserialize(const std::string &str, Class &c)
    {
        TiXmlDocument doc;
        doc.Parse(str.c_str(), NULL);

        const TiXmlNode *acmeTag = doc.FirstChild("ACME");
        if (acmeTag == NULL) {
            return Result(conversionFailed) << "No ACME root element.";
        }

        /* Sanity check, there should NOT be any other node after de command's
         * one */
        if (acmeTag->NextSiblingElement() != NULL) {
            return Result(conversionFailed) << "No command tag found.";
        }

        return Serializer<ClassSerializationTrait<Class> >::fromXml(*acmeTag->FirstChild(), c);
    }
};

} // namespace serializer
} // namespace cme
} // namespace audio_comms
