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

#######################################################################
# target & host common part

common_test_src_files := \
    test/ResultUnitTest.cpp \
    test/ErrnoResultUnitTest.cpp \

base_name := cmeresult
common_library_local_module := lib$(base_name)

common_c_includes := \
    $(LOCAL_PATH)/include \

common_c_flags := \
    -Wall \
    -Werror \
    -Wextra

#######################################################################
# target cme common library

include $(CLEAR_VARS)
LOCAL_MODULE    := $(common_library_local_module)

LOCAL_C_INCLUDES := \
    $(call include-path-for, stlport) \
    bionic \

LOCAL_CFLAGS := $(common_c_flags)

LOCAL_SHARED_LIBRARIES := libstlport

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY)

#======================================================================
# host cme common library

include $(CLEAR_VARS)
LOCAL_MODULE    := $(common_library_local_module)_host

LOCAL_CFLAGS := $(common_c_flags)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include

include $(BUILD_HOST_STATIC_LIBRARY)

#######################################################################
common_c_flags := $(common_c_flags) -ggdb3 -O0
common_ut_local_module    := $(base_name)UnitTest
#---------------------------------------------------------------------
# host cme common test

include $(CLEAR_VARS)

LOCAL_MODULE := $(common_ut_local_module)_host

LOCAL_SRC_FILES := $(common_test_src_files)
LOCAL_C_INCLUDES := $(common_c_includes)

LOCAL_CFLAGS := $(common_c_flags) --coverage

LOCAL_LDFLAGS := --coverage

LOCAL_STRIP_MODULE := false

include $(BUILD_HOST_NATIVE_TEST)

EXECUTABLE_PATH := $(LOCAL_INSTALLED_MODULE)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../runExecutable.mk

#======================================================================
# target unit test

include $(CLEAR_VARS)

LOCAL_MODULE := $(common_ut_local_module)

LOCAL_SRC_FILES := $(common_test_src_files)

LOCAL_C_INCLUDES := \
    $(call include-path-for, stlport) \
    bionic \
    $(common_c_includes)

LOCAL_CFLAGS := $(common_c_flags) --coverage

LOCAL_LDFLAGS := --coverage

LOCAL_STRIP_MODULE := false

LOCAL_SHARED_LIBRARIES := libstlport

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA_NATIVE_TESTS)/audio_cme

include $(BUILD_NATIVE_TEST)

