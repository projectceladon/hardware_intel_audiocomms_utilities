LOCAL_PATH := $(call my-dir)

# Common variables
##################
interface_provider_src_files := \
        InterfaceProviderImpl.cpp \
        InterfaceProviderLib.cpp

# Interface provider
include $(CLEAR_VARS)

LOCAL_CFLAGS := \
        -DDEBUG

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
        $(interface_provider_src_files)

LOCAL_SHARED_LIBRARIES := libcutils libdl

LOCAL_MODULE := libinterface-provider
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include external/stlport/libstlport.mk

include $(BUILD_SHARED_LIBRARY)

# Interface provider library
include $(CLEAR_VARS)

LOCAL_CFLAGS := \
        -DDEBUG

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
        InterfaceProviderLib.cpp

LOCAL_SHARED_LIBRARIES := libcutils libdl

LOCAL_MODULE := libinterface-provider-lib
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional

include external/stlport/libstlport.mk

include $(BUILD_SHARED_LIBRARY)

# Build for host test
###########################
# Interface provider
include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SRC_FILES := \
        $(interface_provider_src_files)
LOCAL_MODULE := libinterface-provider_static_host
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional
include $(BUILD_HOST_STATIC_LIBRARY)

# Interface provider lib
include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SRC_FILES := \
        InterfaceProviderLib.cpp
LOCAL_MODULE := libinterface-provider-lib_static_host
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional
include $(BUILD_HOST_STATIC_LIBRARY)

# Build static lib
###########################
# Interface provider
include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SRC_FILES := \
        $(interface_provider_src_files)
LOCAL_MODULE := libinterface-provider_static
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional
include external/stlport/libstlport.mk
include $(BUILD_STATIC_LIBRARY)

# Interface provider lib
include $(CLEAR_VARS)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
LOCAL_SRC_FILES := \
        InterfaceProviderLib.cpp
LOCAL_MODULE := libinterface-provider-lib_static
LOCAL_MODULE_OWNER := intel
LOCAL_MODULE_TAGS := optional
include external/stlport/libstlport.mk
include $(BUILD_STATIC_LIBRARY)

