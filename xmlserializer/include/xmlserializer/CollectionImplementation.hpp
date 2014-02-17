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

#include "serializer/framework/Collection.hpp"
#include "serializer/framework/ConvertToString.hpp"

#include "common/TypeTraits.hpp"

namespace audio_comms
{
namespace cme
{
namespace xmlserializer
{

namespace detail
{

template <const char *_tag, class Collection, class ItemTrait, bool takeOwnership>
class SerializerChildren<serializer::CollectionTrait<_tag, Collection, ItemTrait,
                                                     takeOwnership> >
{
private:
    template <class A, class B>
    class AssertSame;
    template <class A>
    class AssertSame<A, A>
    {
    };

    AssertSame<typename Collection::value_type, typename ItemTrait::Element>
    Error_CollectionValueType_and_ItemTraitElement_should_be_the_same;

public:
    typedef typename Collection::iterator Iterator;
    typedef typename Collection::const_iterator ConstIterator;
    typedef typename Collection::value_type Item;

    static Result toXml(const Collection &collection,
                        TiXmlNode &xmlCollection)
    {
        size_t nb = 1;
        for (ConstIterator it = collection.begin(); it != collection.end(); it++) {
            /** Construct a constant ItemNb.
             * As it is constant the Item reference member (given at construction)
             * will never be modified. Remove constness because the constructor
             * must take a non const reference in case of non const context
             * (as during deserialization).
             */
            const ItemNb itemNb(const_cast<Item &>(*it));

            // Create the <nb>...</nb> xml node
            TiXmlNode *xmlItemNb;
            // Serialize the ItemNb
            Result res = XmlTraitSerializer<ItemNbTrait>::toXml(itemNb, xmlItemNb,
                                                                formatIndex(nb).c_str());
            if (res.isFailure()) {
                return res << " (While deserializing collection item << " << nb << ")";
            }
            // Add serialized ItemNb to it's parent
            xmlCollection.LinkEndChild(xmlItemNb);
            ++nb;
        }
        return Result::success();
    }

    static Result fromXml(const TiXmlNode &xmlCollection, Collection &collection)
    {
        size_t nb = 1;
        Iterator it = collection.begin();

        // For each <nb>...</nb> node
        const TiXmlNode *xmlItemNb;
        while ((xmlItemNb = xmlCollection.FirstChild(formatIndex(nb).c_str())) != NULL) {
            Item item;
            ItemNb itemNb(item);
            Result res = XmlTraitSerializer<ItemNbTrait>::fromXml(*xmlItemNb, itemNb,
                                                                  formatIndex(nb).c_str());
            if (res.isFailure()) {
                return res << " (While deserializing collection item" << nb << ")";
            }
            collection.push_back(item);
            ++nb;
            ++it;
        }
        return Result::success();
    }

private:
    static std::string formatIndex(int nb)
    {
        std::string nbStr;
        serializer::toString(nb, nbStr);
        // Xml tag must not start with a number
        return "e" + nbStr;
    }

    class ItemNb
    {
    public:
        ItemNb(Item &item) : _item(item) {}

        typedef const Item & (ItemNb::*Get)() const;
        const Item &get() const { return _item; }

        typedef void (ItemNb::*Set)(Item &value) const;
        void set(Item &value) const { _item = value; }

    private:
        Item &_item;
    };

    struct ItemNbTrait
    {
        typedef ItemNb Element;
        typedef serializer::Child<ItemTrait,
                                  typename Element::Get, &Element::get,
                                  typename Element::Set, &Element::set, takeOwnership
                                  > ItemChildTrait;
        typedef TYPELIST1 (ItemChildTrait) Children;
    };
};

} // namespace detail
} // namespace serializer
} // namespace cme
} // namespace audio_comms
