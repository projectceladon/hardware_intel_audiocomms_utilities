/*
 * INTEL CONFIDENTIAL
 * Copyright © 2013 Intel
 * Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel’s prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 *
 */

#include <GcovFlushWithProp.h>
#include <Property.h>
#define LOG_TAG "GcovFlushWithProp"
#include <utils/Log.h>

extern "C" void __gcov_flush();

GcovFlushWithProp::GcovFlushWithProp()
    : stopThread(false)
{
    // On constructor start the thread
    pthread_create(&thread, NULL, staticThread, this);
}

GcovFlushWithProp::~GcovFlushWithProp()
{
    // Ask to stop the thread
    stopThread = true;
    pthread_join(thread, NULL);

    // Force gcov flush at the end of the main or shared lib
    // But this code is never call in a system shared lib
    LOGD("GcovFlushWithProp __gcov_flush begin");
    __gcov_flush();
    LOGD("GcovFlushWithProp __gcov_flush end");
}

GcovFlushWithProp &GcovFlushWithProp::getInstance()
{
    static GcovFlushWithProp instance;
    return instance;
}

void *GcovFlushWithProp::staticThread(void *that)
{
    static_cast<GcovFlushWithProp *>(that)->run();
    return NULL;
}

void GcovFlushWithProp::run()
{
    // Use the property "gcov.flush.force" to communicate between the Android
    // shell and this thread.
    TProperty<bool> gcovFlushForceProp = TProperty<bool>("gcov.flush.force", false);
    bool hasFlush = true;
    while (!stopThread)
    {
        // Read the property value to take into account Android shell modification
        bool gcovFlushForcePropValue = gcovFlushForceProp.getValue();
        // If the property become true
        if (!hasFlush && gcovFlushForcePropValue)
        {
            // gcov flush
            LOGD("GcovFlushWithProp __gcov_flush begin !!!");
            __gcov_flush();
            hasFlush = true;
            LOGD("GcovFlushWithProp __gcov_flush end !!!");
        }
        // If the property become false
        else if (hasFlush && !gcovFlushForcePropValue)
        {
            hasFlush = false;
            LOGD("GcovFlushWithProp wait property \"gcov.flush.force\" is \"true\"");
        }
        // Wait a little between property read
        // to let the processor to others threads.
        // 1 second is a good compromised between the consumed time by the
        // processor during all test and the reacted time of the manual
        // modification of this property.
        sleep(1);
    }
}

