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

#include <sstream>
#include <string>

namespace audio_comms
{
namespace cme
{
namespace serializer
{

/** Convert an element of type Source to string. */
template <typename Source>
static bool toString(const Source &source, std::string &dest)
{
    std::ostringstream stream;
    stream << source;
    dest = stream.str();
    return not stream.fail();

}

/** For boolean, sepecialize to return "true" or "false". */
template <>
bool toString<bool>(const bool &source, std::string &dest)
{
    if (source) {
        dest = "true";
    } else {
        dest = "false";
    }
    return true;
}

} // namespace serializer
} // namespace cme
} // namespace audio_comms
