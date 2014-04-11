LOCAL_PATH := $(call my-dir)

# common variables
##################

property_src_files := \
    PropertyBase.cpp \
    Property.cpp


# build for target
##################
include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/

LOCAL_CFLAGS := -DDEBUG

LOCAL_SRC_FILES := $(property_src_files)

LOCAL_C_INCLUDES += \
    $(call include-path-for, stlport) \
    bionic

LOCAL_SHARED_LIBRARIES := libstlport libcutils

LOCAL_MODULE := libproperty
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# build for target test
#######################

include $(CLEAR_VARS)

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/

LOCAL_SRC_FILES := $(property_src_files)

LOCAL_C_INCLUDES += \
    $(call include-path-for, stlport) \
    bionic

LOCAL_SHARED_LIBRARIES := libstlport libcutils

LOCAL_MODULE := libproperty_static
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)

