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

#include "result/ErrnoResult.hpp"

#include <gtest/gtest.h>

typedef audio_comms::cme::result::ErrnoResult ErrnoResult;

TEST(ErrnoResult, defaultValueConstructor)
{
    ErrnoResult errResult;
    EXPECT_EQ("Code -666: Unknown error -666", errResult.format());
}

TEST(ErrnoResult, sucess)
{
    ErrnoResult errResult = ErrnoResult::success();
    EXPECT_EQ(0, errResult.getErrorCode());

}
TEST(ErrnoResult, error)
{
    ErrnoResult errResult(EPERM);
    EXPECT_EQ("Code 1: Operation not permitted", errResult.format());
}
