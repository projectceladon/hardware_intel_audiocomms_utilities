#
# Copyright 2013-2015 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

#######################################################################
# Build for target with flush from property
include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := GcovFlushWithProp.cpp

LOCAL_MODULE := gcov_flush_with_prop
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := tests

LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_STATIC_LIBRARIES := \
    libproperty \
    libaudio_comms_convert \
    libaudio_comms_utilities

include external/stlport/libstlport.mk
include $(BUILD_STATIC_LIBRARY)

#######################################################################
# Build for target

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

LOCAL_SRC_FILES := GcovFlush.cpp
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \

LOCAL_MODULE := gcov_flush
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

LOCAL_STATIC_LIBRARIES := \
                     libproperty \
                     libaudio_comms_convert \
                     libaudio_comms_utilities

include external/stlport/libstlport.mk
include $(BUILD_STATIC_LIBRARY)

#######################################################################
# Build for host

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

LOCAL_SRC_FILES := GcovFlush.cpp
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    bionic/libc/kernel/common

LOCAL_MODULE := gcov_flush
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := tests


include $(BUILD_HOST_STATIC_LIBRARY)


