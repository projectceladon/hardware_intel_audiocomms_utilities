# Copyright (c) 2014, Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
#
# - Neither the name of Intel Corporation nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

LOCAL_PATH := $(call my-dir)

common_src_files := \
    RemoteParameterSubsystemBuilder.cpp \
    RemoteParameterSubsystem.cpp

common_cflags := \
    -Wall \
    -Werror \
    -Wextra \
    -Wno-unused-parameter    # Needed to workaround STL bug

common_local_import_c_include_dirs_from_static_libraries := \
    libutility \
    libparameter_includes \
    libxmlserializer \
    libremote-parameter-proxy

common_local_import_c_include_dirs_from_static_libraries_host := \
    $(foreach lib, $(common_local_import_c_include_dirs_from_static_libraries), $(lib)_host)

common_module_tags := optional

#######################################################################
# libremoteparameter-subsystem (target build)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(common_src_files)

LOCAL_C_INCLUDES := \
    $(call include-path-for, stlport) \
    bionic

LOCAL_SHARED_LIBRARIES := libstlport libparameter libcutils

LOCAL_STATIC_LIBRARIES := \
    libremote-parameter-proxy  \
    libremote-parameter-common \
    libaudio_comms_utilities

LOCAL_CFLAGS := $(common_cflags)
LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := \
    $(common_local_import_c_include_dirs_from_static_libraries)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/parameter-framework-plugins/Audio
LOCAL_MODULE_TAGS := $(common_module_tags)
LOCAL_MODULE := libremoteparameter-subsystem
include $(BUILD_SHARED_LIBRARY)


#######################################################################
# Host build without mock for functionnal tests
ifeq ($(audiocomms_test_host),true)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(common_src_files)

LOCAL_C_INCLUDES := $(common_c_includes) \
    bionic/libc/kernel/common

LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := \
    $(common_local_import_c_include_dirs_from_static_libraries_host)

LOCAL_SHARED_LIBRARIES := \
    $(common_shared_libraries) \
    libparameter_host

LOCAL_STATIC_LIBRARIES := \
    libremote-parameter-proxy_host  \
    libremote-parameter-common_host \
    libaudio_comms_utilities_host \
    log_mock_host \
    libgmock_host \
    libgtest_host \
    libgmock_main_host \
    libcutils

LOCAL_CFLAGS := \
    $(common_cflags) \
    -O0 \
    -ggdb

LOCAL_LDFLAGS := -lpthread

LOCAL_MODULE := libremoteparameter-subsystem_host

LOCAL_MODULE_TAGS := $(common_module_tags)

include $(BUILD_HOST_SHARED_LIBRARY)

endif # $(audiocomms_test_host)

#######################################################################
# Host build with coverage and mock for unit tests
ifeq ($(audiocomms_test_gcov_host),true)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(common_src_files)

LOCAL_C_INCLUDES := $(common_c_includes) \
    bionic/libc/kernel/common

LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := \
    $(common_local_import_c_include_dirs_from_static_libraries_host)

LOCAL_SHARED_LIBRARIES := \
    $(common_shared_libraries) \
    libparameter_host

LOCAL_STATIC_LIBRARIES := \
    libaudio_comms_utilities_host \
    libremoteparameter-proxy_mock_gcov_host \
    libgmock_host \
    libgtest_host \
    libcutils \
    gcov_flush_host

LOCAL_CFLAGS := $(common_cflags) -O0 -ggdb -D__USE_GCOV__ --coverage -include GcovFlush.hpp

LOCAL_LDFLAGS := -lpthread --coverage

LOCAL_MODULE := libremoteparameter-subsystem_gcov_host

LOCAL_MODULE_TAGS := $(common_module_tags)

include $(BUILD_HOST_SHARED_LIBRARY)

endif # $(audiocomms_test_gcov_host)


