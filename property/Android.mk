LOCAL_PATH := $(call my-dir)

# common variables
##################

property_exported_includes_folder := property
property_exported_includes_files := \
    PropertyBase.h \
    Property.h

property_src_files := \
    PropertyBase.cpp \
    Property.cpp


# build for target
##################
include $(CLEAR_VARS)

LOCAL_COPY_HEADERS_TO := $(property_exported_includes_folder)
LOCAL_COPY_HEADERS := $(property_exported_includes_files)

LOCAL_CFLAGS := -DDEBUG

LOCAL_SRC_FILES := $(property_src_files)

LOCAL_C_INCLUDES += \
    external/stlport/stlport \
    bionic

LOCAL_SHARED_LIBRARIES := libstlport libcutils

LOCAL_MODULE := libproperty
LOCAL_MODULE_TAGS := optional

TARGET_ERROR_FLAGS += -Wno-non-virtual-dtor

include $(BUILD_SHARED_LIBRARY)


# build for host test
#####################

include $(CLEAR_VARS)

LOCAL_COPY_HEADERS_TO := $(property_exported_includes_folder)
LOCAL_COPY_HEADERS := $(property_exported_includes_files)

LOCAL_SRC_FILES := $(property_src_files)

LOCAL_MODULE := libproperty_static_host
LOCAL_MODULE_TAGS := optional

include $(BUILD_HOST_STATIC_LIBRARY)


# build for target test
#######################

include $(CLEAR_VARS)

LOCAL_COPY_HEADERS_TO := $(property_exported_includes_folder)
LOCAL_COPY_HEADERS := $(property_exported_includes_files)

LOCAL_SRC_FILES := $(property_src_files)

LOCAL_C_INCLUDES += \
    external/stlport/stlport \
    bionic

LOCAL_SHARED_LIBRARIES := libstlport libcutils

LOCAL_MODULE := libproperty_static
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)

