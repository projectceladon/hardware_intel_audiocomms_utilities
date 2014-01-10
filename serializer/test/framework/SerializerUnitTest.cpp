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
#include "serializer/framework/TextNode.hpp"
#include "serializer/framework/NamedTextTrait.hpp"

#include <gtest/gtest.h>
#include <string>

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
TiXmlNode *parseXml(TiXmlDocument &doc, const std::string xml)
{
    doc.Parse(xml.c_str());
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

// Text node (de)serialisation
struct IntElem {};

typedef TextTrait<int> IntTrait;

TEST_F(ToXml, textNode)
{
    ASSERT_RESULT_SUCCESS(Serializer<IntTrait>::toXml(10, _xmlNode));

    _doc.LinkEndChild(_xmlNode);

    _stream << _doc;
    EXPECT_EQ(std::string("10"), _stream.c_str());
}

TEST_F(FromXml, textNode)
{
    int result;
    ASSERT_RESULT_SUCCESS(Serializer<IntTrait>::fromXml(TiXmlText("10"), result));
    ASSERT_EQ(10, result);
}

// Element node with one text node (de)serialisation
struct Param
{
    explicit Param(int param) : _param(param) {}
    Param() : _param(66) {}
    const int &getValue() const { return _param; }
    void setValue(int param) { _param = param; }
    int _param;
};

char paramDefaultTag[] = "Param";
template <char *_tag = paramDefaultTag>
struct ParamTrait
{
    static const char *tag;
    typedef Param Element;
    typedef Child<IntTrait,
                  typeof( &Element::getValue), &Element::getValue,
                  typeof( &Element::setValue), &Element::setValue, false> Child1;
    typedef TYPELIST1 (Child1) Children;
};

template <char *_tag>
const char *ParamTrait<_tag>::tag = _tag;

TEST_F(ToXml, Param)
{
    ASSERT_RESULT_SUCCESS(Serializer<ParamTrait<> >::toXml(Param(20), _xmlNode));

    _doc.LinkEndChild(_xmlNode);

    _stream << _doc;
    EXPECT_EQ(std::string("<Param>20</Param>"), _stream.c_str());
}

TEST_F(FromXml, EmptyParam)
{
    _xmlNode = parseXml(_doc, "<Param></Param>");
    ASSERT_TRUE(_xmlNode != NULL);

    Param param;
    ASSERT_RESULT_FAILURE(Serializer<ParamTrait<> >::fromXml(*_xmlNode, param));
}

TEST_F(FromXml, Param)
{
    _xmlNode = parseXml(_doc, "<Param>20</Param>");
    ASSERT_TRUE(_xmlNode != NULL);

    Param param;
    ASSERT_RESULT_SUCCESS(Serializer<ParamTrait<> >::fromXml(*_xmlNode, param));
    EXPECT_EQ(20, param.getValue());
}

// Tree (de)serialisation
struct BigElem
{
    BigElem(Param param1, Param param2) : _param1(param1), _param2(param2) {}
    BigElem() : _param1(66), _param2(99) {}
    const Param &getParam1() const { return _param1; }
    const Param &getParam2() const { return _param2; }
    void setParam1(Param param) { _param1 = param; }
    void setParam2(Param param) { _param2 = param; }
    Param _param1;
    Param _param2;
};

char param1Tag[] = "Param1";
char param2Tag[] = "Param2";

struct BigTrait
{
    static const char *tag;
    typedef BigElem Element;
    typedef Child<ParamTrait<param1Tag>,
                  typeof( &Element::getParam1), &Element::getParam1,
                  typeof( &Element::setParam1), &Element::setParam1, false> Child1;
    // Use a function accessor instead of the method
    static const Param &getChild2(const Element &elem) { return elem._param2; }
    static bool setChild2(Element &elem, const Param &val)
    {
        elem.setParam2(val);
        delete &val;
        return true;
    }
    typedef Child<ParamTrait<param2Tag>,
                  typeof( &getChild2), &getChild2,
                  typeof( &setChild2), &setChild2, true> Child2;

    typedef TYPELIST2 (Child1, Child2) Children;
};
const char *BigTrait::tag = "Big";

TEST_F(ToXml, BigElem)
{
    const BigElem big(Param(20), Param(30));
    ASSERT_RESULT_SUCCESS(Serializer<BigTrait>::toXml(big, _xmlNode));

    _doc.LinkEndChild(_xmlNode);

    _stream << _doc;
    EXPECT_EQ(std::string("<Big>"
                          "<Param1>20</Param1>"
                          "<Param2>30</Param2>"
                          "</Big>"),
              _stream.c_str());
}

TEST_F(FromXml, BigElem)
{
    _xmlNode = parseXml(_doc, "<Big>"
                              "<Param1>10</Param1>"
                              "<Param2>20</Param2>"
                              "</Big>");
    ASSERT_TRUE(_xmlNode != NULL);

    BigElem big;
    ASSERT_RESULT_SUCCESS(Serializer<BigTrait>::fromXml(*_xmlNode, big));
    EXPECT_EQ(10, big.getParam1().getValue());
    EXPECT_EQ(20, big.getParam2().getValue());
}

template <class Trait>
void dererializeString(const std::string &refStr, bool success = true)
{
    TiXmlDocument doc;
    TiXmlNode *xmlNode = parseXml(doc, "<String>" + refStr + "</String>");
    ASSERT_TRUE(xmlNode != NULL);

    typename Trait::Element str;
    Result res = Serializer<Trait>::fromXml(*xmlNode, str);
    if (success) {
        ASSERT_RESULT_SUCCESS(res);
    } else {
        ASSERT_RESULT_FAILURE(res);
    }
    EXPECT_EQ(refStr, str);
}

char StringTag[] = "String";
typedef NamedTextTrait<std::string, StringTag> StringTrait;

TEST_F(ToXml, String)
{
    const StringTrait::Element str(":/");
    ASSERT_RESULT_SUCCESS(Serializer<StringTrait>::toXml(str, _xmlNode));

    _doc.LinkEndChild(_xmlNode);

    _stream << _doc;
    EXPECT_EQ(std::string("<String>:/</String>"),
              _stream.c_str());
}

TEST_F(FromXml, String)
{
    dererializeString<StringTrait>(":/");
}

TEST_F(FromXml, Empty_String)
{
    dererializeString<StringTrait>("", false);
}

typedef NamedTextTrait<std::string, StringTag, true> OptionalStringTrait;

TEST_F(FromXml, Optional_String)
{
    dererializeString<OptionalStringTrait>(":/");
}

TEST_F(FromXml, Optional_Empty_String)
{
    dererializeString<OptionalStringTrait>("");
}
