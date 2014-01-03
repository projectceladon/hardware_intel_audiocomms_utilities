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

#include "serializer/framework/Serializer.hpp"

#include <gtest/gtest.h>

using namespace audio_comms::cme::serializer;

template <class Result>
void ASSERT_RESULT_SUCCESS(Result res)
{
    ASSERT_TRUE(res.isSuccess()) << res.format();
}
template <class Result>
void ASSERT_RESULT_FAILURE(Result res)
{
    ASSERT_FALSE(res.isSuccess()) << res.format();
}

/** Parse a xml string and return a node to the root element */
TiXmlNode *parseXml(TiXmlDocument &doc, const char *xml)
{
    doc.Parse(xml);
    EXPECT_FALSE(doc.Error()) << doc.ErrorDesc();
    return doc.RootElement();
}

class ToXml : public testing::Test
{
protected:
    TiXmlNode *_xmlNode;
    TiXmlDocument _doc;
    TiXmlOutStream _stream;
};
class FromXml : public ToXml
{
public:
    FromXml() : _handle(NULL) {}

protected:
    TiXmlText *_textXml;
    TiXmlHandle _handle;
};

// Empty node (de)serialisation
struct EmptyElem {};

struct EmptyElemTrait
{
    static const char *tag;
    typedef EmptyElem Element;
    typedef TYPELIST0 Children;
};
const char *EmptyElemTrait::tag = "Empty";

TEST_F(ToXml, EmptyElement)
{
    ASSERT_RESULT_SUCCESS(Serializer<EmptyElemTrait>::toXml(EmptyElem(), _xmlNode));

    _doc.LinkEndChild(_xmlNode);

    _stream << _doc;
    EXPECT_EQ(std::string("<Empty />"), _stream.c_str());
}

TEST_F(FromXml, EmptyElement)
{
    EmptyElem emptyElem;
    ASSERT_RESULT_SUCCESS(Serializer<EmptyElemTrait>::fromXml(TiXmlElement("Empty"),
                                                              emptyElem));
}

TEST_F(FromXml, WrongElement)
{
    EmptyElem emptyElem;
    ASSERT_RESULT_FAILURE(Serializer<EmptyElemTrait>::fromXml(TiXmlElement("WrongTag"),
                                                              emptyElem));
}

// Empty node (de)serialisation
class NotSoEmptyElem
{
public:
    void setEmptyElem(EmptyElem emptyElem) { _emptyElem = emptyElem; }
    EmptyElem getEmptyElem() const { return _emptyElem; }

private:
    EmptyElem _emptyElem;
};

struct NotSoEmptyElemTrait
{
    static const char *tag;
    typedef NotSoEmptyElem Element;
    typedef Child<EmptyElemTrait,
                  typeof( &Element::getEmptyElem), &Element::getEmptyElem,
                  typeof( &Element::setEmptyElem), &Element::setEmptyElem, false> Child1;
    typedef TYPELIST1 (Child1) Children;
};
const char *NotSoEmptyElemTrait::tag = "NotSoEmpty";

TEST_F(ToXml, NotSoEmptyElement)
{
    ASSERT_RESULT_SUCCESS(Serializer<NotSoEmptyElemTrait>::toXml(NotSoEmptyElem(), _xmlNode));

    _doc.LinkEndChild(_xmlNode);

    _stream << _doc;
    EXPECT_EQ(std::string("<NotSoEmpty><Empty /></NotSoEmpty>"), _stream.c_str());
}

TEST_F(FromXml, NotSoEmptyElement)
{
    NotSoEmptyElem emptyElem;
    _xmlNode = parseXml(_doc, "<NotSoEmpty><Empty /></NotSoEmpty>");
    ASSERT_TRUE(_xmlNode != NULL);

    ASSERT_RESULT_SUCCESS(Serializer<NotSoEmptyElemTrait>::fromXml(*_xmlNode,
                                                                   emptyElem));
}
