#
#
# Copyright (C) Intel 2013-2015
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

###########################
# convert static lib target

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(LOCAL_PATH)/ \
    $(LOCAL_PATH)/include/

LOCAL_MODULE := libaudio_comms_convert
LOCAL_MODULE_OWNER := intel

include $(BUILD_STATIC_LIBRARY)

#########################
# convert static lib host

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(LOCAL_PATH)/ \
    $(LOCAL_PATH)/include/


LOCAL_MODULE := libaudio_comms_convert_host
LOCAL_MODULE_OWNER := intel

LOCAL_CFLAGS = -O0 --coverage

LOCAL_LDFLAGS = --coverage

LOCAL_MODULE_TAGS := tests

include $(BUILD_HOST_STATIC_LIBRARY)

##############################
# build hack for getting headers (target version)

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(LOCAL_PATH)/ \
    $(LOCAL_PATH)/include/

LOCAL_MODULE := libaudio_comms_convert_includes
LOCAL_MODULE_OWNER := intel

LOCAL_MODULE_TAGS := tests

include $(BUILD_STATIC_LIBRARY)

##############################
# build hack for getting headers (host version)

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_MODULE := libaudio_comms_convert_includes_host
LOCAL_MODULE_OWNER := intel

LOCAL_MODULE_TAGS := tests

include $(BUILD_HOST_STATIC_LIBRARY)

