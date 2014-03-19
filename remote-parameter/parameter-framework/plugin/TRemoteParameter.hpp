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

#include "FormattedSubsystemObject.h"
#include "InstanceConfigurableElement.h"
#include "MappingContext.h"
#include <RemoteParameterProxy.hpp>
#include "RemoteParameterMappingKeys.hpp"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

template <typename type>
class TRemoteParameter : public CFormattedSubsystemObject
{
public:
    explicit TRemoteParameter(const string &mappingValue,
                              CInstanceConfigurableElement *instanceConfigurableElement,
                              const CMappingContext &context)
        : CFormattedSubsystemObject(instanceConfigurableElement,
                                    mappingValue,
                                    MappingKeyAmend1,
                                    (MappingKeyAmendEnd - MappingKeyAmend1 + 1),
                                    context),
          mRemoteParameterProxy(new RemoteParameterProxy<type>(getFormattedMappingValue()))
    {
        log_info("%s: \"%s\"", __FUNCTION__, getFormattedMappingValue().c_str());
#ifdef __USE_GCOV__
        static GcovFlush gcovFlushInstance;
#endif // __USE_GCOV__
    }

    virtual ~TRemoteParameter()
    {

        delete mRemoteParameterProxy;
    }

protected:
    virtual bool receiveFromHW(std::string &error)
    {
        type data;
        if (!mRemoteParameterProxy->get(data, error)) {

            log_info("%s for \"%s\" error: \"%s\"", __FUNCTION__,
                     getFormattedMappingValue().c_str(), error.c_str());
            return false;
        }
        if (sizeof(data) > getSize()) {

            error = "value exceed parameter size";
            log_info("%s for \"%s\" error: \"%s\"", __FUNCTION__,
                     getFormattedMappingValue().c_str(), error.c_str());
            return false;
        }
        blackboardWrite(static_cast<void *>(&data), getSize());

        return true;
    }

    virtual bool sendToHW(std::string &error)
    {
        type data;
        // Retrieve blackboard
        blackboardRead(static_cast<void *>(&data), getSize());

        if (!mRemoteParameterProxy->set(data, error)) {

            log_info("%s for \"%s\" error: \"%s\"", __FUNCTION__,
                     getFormattedMappingValue().c_str(), error.c_str());
            return false;
        }
        return true;
    }

private:
    RemoteParameterProxy<type> *mRemoteParameterProxy; /**< Remote Parameter Proxy handle. */

};

template <>
class TRemoteParameter<std::string> : public CFormattedSubsystemObject
{

public:
    explicit TRemoteParameter<std::string>(const string &mappingValue,
                                           CInstanceConfigurableElement *
                                           instanceConfigurableElement,
                                           const CMappingContext &context)
    : CFormattedSubsystemObject(instanceConfigurableElement,
                                mappingValue,
                                MappingKeyAmend1,
                                (MappingKeyAmendEnd - MappingKeyAmend1 + 1),
                                context),
    mRemoteParameterProxy(new RemoteParameterProxy<std::string>(getFormattedMappingValue()))
    {
        log_info("%s: \"%s\"", __FUNCTION__, getFormattedMappingValue().c_str());
#ifdef __USE_GCOV__
        static GcovFlush gcovFlushInstance;
#endif // __USE_GCOV__
    }

    virtual ~TRemoteParameter()
    {
        delete mRemoteParameterProxy;
    }

protected:
    virtual bool receiveFromHW(std::string &error)
    {
        std::string data;
        if (!mRemoteParameterProxy->get(data, error)) {

            log_info("%s for \"%s\" error: \"%s\"", __FUNCTION__,
                     getFormattedMappingValue().c_str(), error.c_str());
            return false;
        }
        if (data.size() > getSize() - 1) {

            error = "value exceed parameter size";
            log_info("%s for \"%s\" error: \"%s\"", __FUNCTION__,
                     getFormattedMappingValue().c_str(), error.c_str());
            return false;
        }
        log_info("%s for \"%s\":  %s", __FUNCTION__, getFormattedMappingValue().c_str(),
                 data.c_str());
        blackboardWrite(data.c_str(), data.size() + 1);

        return true;
    }

    virtual bool sendToHW(std::string &error)
    {
        char data[getSize()];

        // Retrieve blackboard
        blackboardRead(data, getSize());

        if (!mRemoteParameterProxy->set(data, error)) {

            log_info("%s for \"%s\" error: \"%s\"", __FUNCTION__,
                     getFormattedMappingValue().c_str(), error.c_str());
            return false;
        }
        log_info("%s for \"%s\":  %s", __FUNCTION__, getFormattedMappingValue().c_str(), data);
        return true;
    }

private:
    RemoteParameterProxy<std::string> *mRemoteParameterProxy; /**< Remote Parameter Proxy handle. */

};
