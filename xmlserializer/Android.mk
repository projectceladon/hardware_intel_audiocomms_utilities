################################################################################
#                              INTEL CONFIDENTIAL
#   Copyright(C) 2013-2014 Intel Corporation. All Rights Reserved.
#   The source code contained  or  described herein and all documents related to
#   the source code ("Material") are owned by Intel Corporation or its suppliers
#   or licensors.  Title to the  Material remains with  Intel Corporation or its
#   suppliers and licensors. The Material contains trade secrets and proprietary
#   and  confidential  information of  Intel or its suppliers and licensors. The
#   Material  is  protected  by  worldwide  copyright  and trade secret laws and
#   treaty  provisions. No part of the Material may be used, copied, reproduced,
#   modified, published, uploaded, posted, transmitted, distributed or disclosed
#   in any way without Intel's prior express written permission.
#   No license  under any  patent, copyright, trade secret or other intellectual
#   property right is granted to or conferred upon you by disclosure or delivery
#   of the Materials,  either expressly, by implication, inducement, estoppel or
#   otherwise.  Any  license  under  such  intellectual property  rights must be
#   express and approved by Intel in writing.
#
################################################################################


LOCAL_PATH := $(call my-dir)

#-----------------------------------------------------------------------
# target & host common part

common_library_src_files := \

common_library_test_src_files := \
    test/SerializerUnitTest.cpp \

common_base_name := cmexmlserializer
common_library_local_module := lib$(common_base_name)

common_static_libs := \
    libcmeserializer \
    libcmeresult \

common_static_libs_host := \
    libcmeserializer_host \
    libcmeresult_host \
    libtinyxml \

common_shared_libs := \
    libstlport \
    libtinyxml \
    libcmecommon

common_shared_libs_host := \
    libcmecommon_host

common_library_c_includes := \
    external/tinyxml/ \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/src \

common_header_lib := \
    libaudio_comms_convert \
    libaudio_comms_utilities

common_header_lib_host := \
    libaudio_comms_convert_host \
    libaudio_comms_utilities_host

common_c_flags := \
    -Wall \
    -Werror \
    -Wextra

#######################################################################
# target cme serializer library

include $(CLEAR_VARS)
LOCAL_MODULE    := $(common_library_local_module)
LOCAL_SRC_FILES := $(common_library_src_files)

LOCAL_C_INCLUDES := \
    $(call include-path-for, stlport) \
    bionic \
    $(common_library_c_includes) \

LOCAL_CFLAGS := $(common_c_flags)

LOCAL_STATIC_LIBRARIES := $(common_static_libs)

LOCAL_SHARED_LIBRARIES := $(common_shared_libs)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := $(common_header_lib)

include $(BUILD_STATIC_LIBRARY)

#======================================================================
# host cme serializer library

include $(CLEAR_VARS)
LOCAL_MODULE    := $(common_library_local_module)_host
LOCAL_SRC_FILES := $(common_library_src_files)

LOCAL_C_INCLUDES := $(common_library_c_includes)

LOCAL_CFLAGS := $(common_c_flags)

LOCAL_STATIC_LIBRARIES := $(common_static_libs_host)

LOCAL_SHARED_LIBRARIES := $(common_shared_libs_host)

LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := $(common_header_lib_host)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

include $(BUILD_HOST_STATIC_LIBRARY)

#######################################################################
#Unit test
common_c_flags := $(common_c_flags) -O0 --coverage

#=====================================================================
# host cme api test

include $(CLEAR_VARS)

LOCAL_MODULE := $(common_base_name)_unit_test_host

LOCAL_SRC_FILES := \
    $(common_library_test_src_files)

LOCAL_C_INCLUDES := $(common_library_c_includes)

LOCAL_CFLAGS := $(common_c_flags)

LOCAL_LDFLAGS := --coverage

LOCAL_STRIP_MODULE := false

LOCAL_STATIC_LIBRARIES := \
    $(common_library_local_module)_host \
    $(common_static_libs_host)

LOCAL_SHARED_LIBRARIES := \
    $(common_shared_libs_host) \

LOCAL_IMPORT_C_INCLUDE_DIRS_FROM_STATIC_LIBRARIES := $(common_header_lib_host)

include $(BUILD_HOST_NATIVE_TEST)

EXECUTABLE_PATH := $(LOCAL_INSTALLED_MODULE)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../runExecutable.mk
