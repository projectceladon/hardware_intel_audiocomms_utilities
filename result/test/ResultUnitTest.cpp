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

#include "result/Result.hpp"

#include <gtest/gtest.h>

struct TestResult
{
    enum Code
    {
        A = 666,
        B,
        C,
        D
    };

    static const Code success;
    static const Code defaultError;

    static std::string codeToString(const Code &code)
    {
        switch (code) {
        case A:
            return "A";
        case B:
            return "B";
        case C:
            return "C";
        case D:
            return "D";
        }
        /** @fixme: ASSERT? */
        return "";
    }
};

const TestResult::Code TestResult::success = A;
const TestResult::Code TestResult::defaultError = B;

typedef audio_comms::cme::result::Result<TestResult> Result;

TEST(Result, defaultValueConstructor)
{
    Result aResult;

    /* Note, this also tests operator== */
    EXPECT_EQ(Result(TestResult::defaultError), aResult);
    EXPECT_EQ("", aResult.getMessage());
}

TEST(Result, someValueConstructor)
{
    Result aResult(TestResult::A);

    EXPECT_EQ(Result(TestResult::A), aResult);
    EXPECT_EQ("", aResult.getMessage());
}

TEST(Result, convertToCode)
{
    {
        Result aResult(TestResult::success);
        EXPECT_TRUE(aResult == TestResult::success);
    }

    {
        Result aResult(TestResult::C);
        EXPECT_FALSE(aResult.isSuccess());
    }
}

TEST(Result, isFailure)
{
    {
        Result aResult(TestResult::success);
        EXPECT_FALSE(aResult.isFailure());
        aResult << "that should not change success state";
        EXPECT_FALSE(aResult.isFailure());
    }

    {
        Result aResult(TestResult::D);
        EXPECT_TRUE(aResult.isFailure());
        aResult << "that should not change failure state";
        EXPECT_TRUE(aResult.isFailure());
    }
}

TEST(Result, isSuccess)
{
    {
        Result aResult(TestResult::D);
        EXPECT_FALSE(aResult.isSuccess());
        aResult << "that should not change failure state";
        EXPECT_FALSE(aResult.isSuccess());
    }

    {
        Result aResult(TestResult::success);
        EXPECT_TRUE(aResult.isSuccess());
        aResult << "that should not change success state";
        EXPECT_TRUE(aResult.isSuccess());
    }
}

TEST(Result, equalityOperator)
{
    Result aResult(TestResult::success);
    aResult << " Now error message differs.";
    EXPECT_EQ(Result(TestResult::success),  aResult);
}

TEST(Result, getErrorCode)
{
    Result aResult(TestResult::success);
    EXPECT_EQ(TestResult::success, aResult.getErrorCode());
}

TEST(Result, formatSuccess)
{
    Result aResult(TestResult::success);
    EXPECT_EQ("Success", aResult.format());
}

TEST(Result, formatError)
{
    Result aResult(TestResult::C);
    EXPECT_EQ("Code 668: C", aResult.format());
}

TEST(Result, useWithStream)
{
    Result aResult(TestResult::B);
    aResult << "gnii";
    EXPECT_EQ("gnii",  aResult.getMessage());
    EXPECT_EQ("Code 667: B (gnii)",  aResult.format());
}

TEST(Result, successSingleton)
{
    EXPECT_EQ(Result::success(), Result(TestResult::success));
}

TEST(Result, constructFromfailureRes)
{
    Result bResult(TestResult::B);
    bResult << "gnii";
    Result trans(bResult, TestResult::D);
    trans << "gnaa";
    EXPECT_EQ("Code 669: D (Code 667: B (gnii)gnaa)",  trans.format());
}

TEST(Result, constructFromSuccessRes)
{
    Result trans(Result::success(), TestResult::D, TestResult::C);
    EXPECT_EQ(TestResult::C, trans);
}

TEST(Result, resultStream)
{
    Result bResult(TestResult::B);
    EXPECT_EQ("Code 667: B (gnii: Code 669: D (gnuu))",
              (Result(TestResult::B) << "gnii"
                                     << (Result(TestResult::D) << "gnuu")
              ).format());
}
