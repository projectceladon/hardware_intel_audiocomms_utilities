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

#include <result/Result.hpp>

namespace audio_comms
{
namespace cme
{
namespace serializer
{

enum ResultCode
{
    successCode = 66,
    unknown,
    wrongXmlNode,
    childNotFound,
    unexpectedNodeType,
    conversionFailed
};

struct ResultTrait
{
    typedef ResultCode Code;
    /** Enum coding the failures that can occur in the class methods. */
    static const Code success = successCode;
    static const Code defaultError = unknown;

    static std::string codeToString(const Code &code)
    {
        switch (code) {
        case successCode:
            return "Success";
        case unknown:
            return "Unknown error";
        case wrongXmlNode:
            return "Wrong XML node";
        case childNotFound:
            return "Child not found";
        case unexpectedNodeType:
            return "Unexpected node type";
        case conversionFailed:
            return "Conversion failed";
        }
        /* Unreachable, prevents gcc to complain */
        return "Invalid error (Unreachable)";
    }
};

typedef result::Result<ResultTrait> Result;

} // namespace serializer
} // namespace cme
} // namespace audio_comms
