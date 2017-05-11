#
#
# Copyright (C) Intel 2013-2017
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

# Build for target
##################

include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := EventThread.cpp
LOCAL_CFLAGS := -Wall -Werror -Wextra
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_STATIC_LIBRARIES := libaudio_utilities

LOCAL_MODULE := libevent-listener
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# Build static lib
###########################

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := EventThread.cpp
LOCAL_STATIC_LIBRARIES := libaudio_utilities

LOCAL_MODULE := libevent-listener_static
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)

# Build for host test
#####################

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SRC_FILES := EventThread.cpp
LOCAL_STATIC_LIBRARIES := libaudio_utilities_host

LOCAL_MODULE := libevent-listener_static_host
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include $(BUILD_HOST_STATIC_LIBRARY)


