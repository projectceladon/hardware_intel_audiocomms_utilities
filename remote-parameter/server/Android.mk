# Copyright 2014 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

# Common variables
##################

remote_param_server_src_files := \
    RemoteParameterServer.cpp \
    RemoteParameterImpl.cpp \
    RemoteParameter.cpp \
    RemoteParameterString.cpp


remote_param_server_includes_dir := \
    $(LOCAL_PATH)/include

remote_param_server_includes_dir_host := \
    $(remote_param_server_includes_dir) \
    bionic/libc/kernel/common

remote_param_server_includes_dir_target := \
    $(remote_param_server_includes_dir) \
    external/stlport/stlport \
    bionic

remote_param_server_shared_lib_target += \
    libstlport \
    libcutils \
    libevent-listener

remote_param_server_static_lib += \
    libaudio_comms_utilities

remote_param_server_static_lib_host += \
    $(foreach lib, $(remote_param_server_static_lib), $(lib)_host) \
    libevent-listener_static_host

remote_param_server_static_lib_target += \
    $(remote_param_server_static_lib)

remote_param_server_import_c_include_dirs_from_static_lib := \
    libremote-parameter-common

remote_param_server_import_c_include_dirs_from_static_lib_host += \
    $(foreach lib, $(remote_param_server_import_c_include_dirs_from_static_lib), $(lib)_host)

remote_param_server_import_c_include_dirs_from_static_lib_target += \
    $(remote_param_server_import_c_include_dirs_from_static_lib)

remote_param_server_import_c_include_dirs_from_static_lib_gcov_host := \
    $(foreach lib, $(remote_param_server_import_c_include_dirs_from_static_lib), $(lib)_gcov_host)

remote_param_server_import_c_include_dirs_from_static_lib_gcov_target := \
    $(foreach lib, $(remote_param_server_import_c_include_dirs_from_static_lib), $(lib)_gcov)

remote_param_server_cflags := -Wall -Werror -Wno-unused-parameter

#######################################################################
# Compile macro

define make_remote_param_server_lib
$( \
    $(eval LOCAL_EXPORT_C_INCLUDE_DIRS := $(remote_param_server_includes_dir)) \
    $(eval LOCAL_C_INCLUDES += $(remote_param_server_includes_dir_$(1))) \
    $(eval LOCAL_STATIC_LIBRARIES += $(remote_param_server_static_lib_$(1))) \
    $(eval LOCAL_SHARED_LIBRARIES := $(remote_param_server_shared_lib_$(1))) \
    $(eval LOCAL_SRC_FILES := $(remote_param_server_src_files)) \
    $(eval LOCAL_CFLAGS += $(remote_param_server_cflags)) \
    $(eval LOCAL_MODULE_TAGS := optional) \
)
endef

define add_gcov
$( \
    $(eval LOCAL_CFLAGS += -O0 --coverage) \
    $(eval LOCAL_LDFLAGS += --coverage) \
)
endef

# Build for targets
##################

include $(CLEAR_VARS)
LOCAL_MODULE := libremote-parameter-server
$(call make_remote_param_server_lib,target)
LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := \
    $(remote_param_server_import_c_include_dirs_from_static_lib_target)
include $(BUILD_STATIC_LIBRARY)


# Build for host test
#####################
include $(CLEAR_VARS)
$(call make_remote_param_server_lib,host)
LOCAL_MODULE :=  libremote-parameter-server_host
LOCAL_LDFLAGS += -pthread
LOCAL_STATIC_LIBRARIES += $(remote_param_server_import_c_include_dirs_from_static_lib_host)
include $(BUILD_HOST_STATIC_LIBRARY)

# Functional test target with gcov
##################################
include $(CLEAR_VARS)
LOCAL_MODULE := libremote-parameter-server_gcov
$(call make_remote_param_server_lib,target)
$(call add_gcov)
LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := \
    $(remote_param_server_import_c_include_dirs_from_static_lib_gcov_target)
include $(BUILD_STATIC_LIBRARY)

# Functional test host with gcov
##################################
include $(CLEAR_VARS)
$(eval LOCAL_LDFLAGS += -pthread)
LOCAL_MODULE := libremote-parameter-server_gcov_host
$(call make_remote_param_server_lib,host)
$(call add_gcov)
LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := \
    $(remote_param_server_import_c_include_dirs_from_static_lib_gcov_host)
include $(BUILD_HOST_STATIC_LIBRARY)
