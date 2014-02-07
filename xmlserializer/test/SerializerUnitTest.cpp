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

#include "xmlserializer/Serializer.hpp"
#include "serializer/framework/TextNode.hpp"
#include "serializer/framework/NamedTextTrait.hpp"

#include <gtest/gtest.h>
#include <string>

using namespace audio_comms::cme::xmlserializer;
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

class ToXml : public testing::Test
{
public:
    template <class Trait, typename Elem>
    void toXml(Elem elem)
    {
        ASSERT_RESULT_SUCCESS(XmlTraitSerializer<Trait>::toXml(elem, _xmlNode));
        _doc.LinkEndChild(_xmlNode);
        _stream << _doc;
        _xml = _stream.c_str();
    }

protected:
    TiXmlNode *_xmlNode;
    TiXmlDocument _doc;
    TiXmlOutStream _stream;
    std::string _xml;
};
class FromXml : public ToXml
{
public:
    FromXml() : _handle(NULL) {}

    /** Parse a xml string and return a node to the root element */
    void parseXml(const std::string xml)
    {
        _doc.Parse(xml.c_str());
        EXPECT_FALSE(_doc.Error()) << _doc.ErrorDesc() << " While parsing: " << xml;
        _xmlNode = _doc.RootElement();
        ASSERT_TRUE(_xmlNode != NULL);
    }

    template <class Trait>
    void dererializeString(const std::string &refStr, bool success = true)
    {
        std::string tag = Trait::tag;
        parseXml("<" + tag + ">" + refStr + "</" + tag + ">");

        typename Trait::Element str;
        Result res = XmlTraitSerializer<Trait>::fromXml(*_xmlNode, str);
        if (success) {
            ASSERT_RESULT_SUCCESS(res);
        } else {
            ASSERT_RESULT_FAILURE(res);
        }
        EXPECT_EQ(refStr, str);
    }

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
    toXml<EmptyElemTrait>(EmptyElem());

    EXPECT_EQ("<Empty />", _xml);
}

TEST_F(FromXml, EmptyElement)
{
    EmptyElem emptyElem;
    ASSERT_RESULT_SUCCESS(XmlTraitSerializer<EmptyElemTrait>::fromXml(TiXmlElement("Empty"),
                                                              emptyElem));
}

TEST_F(FromXml, WrongElement)
{
    EmptyElem emptyElem;
    ASSERT_RESULT_FAILURE(XmlTraitSerializer<EmptyElemTrait>::fromXml(TiXmlElement("WrongTag"),
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
    toXml<NotSoEmptyElemTrait>(NotSoEmptyElem());
    EXPECT_EQ("<NotSoEmpty><Empty /></NotSoEmpty>", _xml);
}

TEST_F(FromXml, NotSoEmptyElement)
{
    NotSoEmptyElem emptyElem;
    parseXml("<NotSoEmpty><Empty /></NotSoEmpty>");

    ASSERT_RESULT_SUCCESS(XmlTraitSerializer<NotSoEmptyElemTrait>::fromXml(*_xmlNode,
                                                                   emptyElem));
}

// Text node (de)serialisation
struct IntElem {};

typedef TextTrait<int> IntTrait;

TEST_F(ToXml, textNode)
{
    toXml<IntTrait>(10);
    EXPECT_EQ("10", _xml);
}

TEST_F(FromXml, textNode)
{
    int result;
    ASSERT_RESULT_SUCCESS(XmlTraitSerializer<IntTrait>::fromXml(TiXmlText("10"), result));
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
    toXml<ParamTrait<> >(Param(20));
    EXPECT_EQ("<Param>20</Param>", _xml);
}

TEST_F(FromXml, EmptyParam)
{
    parseXml("<Param></Param>");

    Param param;
    ASSERT_RESULT_FAILURE(XmlTraitSerializer<ParamTrait<> >::fromXml(*_xmlNode, param));
}

TEST_F(FromXml, Param)
{
    parseXml("<Param>20</Param>");

    Param param;
    ASSERT_RESULT_SUCCESS(XmlTraitSerializer<ParamTrait<> >::fromXml(*_xmlNode, param));
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
    toXml<BigTrait>(big);

    EXPECT_EQ("<Big>"
              "<Param1>20</Param1>"
              "<Param2>30</Param2>"
              "</Big>", _xml);
}

TEST_F(FromXml, BigElem)
{
    parseXml("<Big>"
             "<Param1>10</Param1>"
             "<Param2>20</Param2>"
             "</Big>");

    BigElem big;
    ASSERT_RESULT_SUCCESS(XmlTraitSerializer<BigTrait>::fromXml(*_xmlNode, big));
    EXPECT_EQ(10, big.getParam1().getValue());
    EXPECT_EQ(20, big.getParam2().getValue());
}

char StringTag[] = "String";
typedef NamedTextTrait<std::string, StringTag> StringTrait;

TEST_F(ToXml, String)
{
    const StringTrait::Element str(":/");
    toXml<StringTrait>(str);

    EXPECT_EQ("<String>:/</String>", _xml);
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
