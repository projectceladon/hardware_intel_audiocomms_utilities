/*******************************************************************************
** INTEL CONFIDENTIAL
**
** Copyright (2014) Intel Corporation All Rights Reserved.
**
** The source code contained or described herein and all documents related to
** the source code ("Material") are owned by Intel Corporation or its suppliers
** or licensors.
**
** Title to the Material remains with Intel Corporation or its suppliers and
** licensors. The Material contains trade secrets and proprietary and
** confidential information of Intel or its suppliers and licensors. The
** Material is protected by worldwide copyright and trade secret laws and treaty
** provisions. No part of the Material may be used, copied, reproduced,
** modified, published, uploaded, posted, transmitted, distributed, or disclosed
** in any way without Intel's prior express written permission.
**
** No license under any patent, copyright, trade secret or other intellectual
** property right is granted to or conferred upon you by disclosure or delivery
** of the Materials, either expressly, by implication, inducement, estoppel or
** otherwise. Any license under such intellectual property rights must be
** express and approved by Intel in writing.
*******************************************************************************/

#include "RemoteParameterSubsystem.hpp"
#include "SubsystemObjectFactory.h"
#include "TRemoteParameter.hpp"

const char *const RemoteParameterSubsystem::mKeyAmend1 = "Amend1";
const char *const RemoteParameterSubsystem::mKeyAmend2 = "Amend2";
const char *const RemoteParameterSubsystem::mKeyAmend3 = "Amend3";

const char *const RemoteParameterSubsystem::mStringRemoteParameter = "StringRemoteParameter";
const char *const RemoteParameterSubsystem::mBooleanRemoteParameter = "BooleanRemoteParameter";
const char *const RemoteParameterSubsystem::mIntegerRemoteParameter = "IntegerRemoteParameter";


RemoteParameterSubsystem::RemoteParameterSubsystem(const string &name) : CSubsystem(name)
{
    // Provide mapping keys to upper layer
    addContextMappingKey(mKeyAmend1);
    addContextMappingKey(mKeyAmend2);
    addContextMappingKey(mKeyAmend3);

    // Provide creators to upper layer
    addSubsystemObjectFactory(
        new TSubsystemObjectFactory<TRemoteParameter<std::string> >(
            mStringRemoteParameter,
            0)
        );

    addSubsystemObjectFactory(
        new TSubsystemObjectFactory<TRemoteParameter<bool> >(
            mBooleanRemoteParameter,
            0)
        );

    addSubsystemObjectFactory(
        new TSubsystemObjectFactory<TRemoteParameter<uint32_t> >(
            mIntegerRemoteParameter, 0)
        );
}
