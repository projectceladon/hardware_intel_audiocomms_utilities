/*
 * INTEL CONFIDENTIAL
 *
 * Copyright (c) 2014-2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors.
 *
 * Title to the Material remains with Intel Corporation or its suppliers and
 * licensors. The Material contains trade secrets and proprietary and
 * confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and treaty
 * provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or disclosed
 * in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

#pragma once

#include <string>

/**
 * Modem state class wraps the modem state notifications from MAMGR
 * to a suitable state for audio HAL
 */
class ModemState
{
public:
    typedef enum
    {
        Up,
        Down,
        Starting
    } State;

    /**
     * Converts a numerical value to a literal value
     * For that purpose, the string returned is an utility
     * for the modem proxy of the audio HAL
     *
     * @param[in] modemState enum value to be converted to string
     * @return string containing the modem state
     */
    static const std::string &toLiteral(State modemState)
    {

        static const std::string modemStateUp("Up");
        static const std::string modemStateDown("Down");
        static const std::string modemStateStarting("Starting");
        static const std::string modemStateUnknown("Unknown");

        switch (modemState) {

        case Up:
            return modemStateUp;
        case Down:
            return modemStateDown;
        case Starting:
            return modemStateStarting;
        }

        return modemStateUnknown;
    }
};
