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

#include "serializer/framework/ConvertToString.hpp"
#include <gtest/gtest.h>

using namespace audio_comms::cme::serializer;


template <typename Source>
void TestConversion(const Source &src, std::string destExpected, bool success = true)
{
    std::string dest;
    EXPECT_TRUE(toString(src, dest));
    if (success) {
        EXPECT_EQ(destExpected, dest);
    } else {
        EXPECT_NE(destExpected, dest);
    }
}

TEST(Convert, Int)
{
    TestConversion<int>(20, "20");
    TestConversion<int>(100, "one hundred", false);

    TestConversion<int>(-200, "-200");
    TestConversion<int>(-1000, "1", false);
}

TEST(Convert, Bool)
{
    TestConversion<bool>(true, "true");
    TestConversion<bool>(false, "false");
    TestConversion<bool>(false, "none", false);
}

TEST(Convert, Chars)
{
    TestConversion<const char *>("", "");
    TestConversion<const char *>("toto", "toto");
    TestConversion<const char *>("toto", "tito", false);
}

TEST(Convert, String)
{
    TestConversion<std::string>("", "");
    TestConversion<std::string>("bou", "bou");
    TestConversion<std::string>("bar", "bone", false);
}
