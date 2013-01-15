LOCAL_PATH := $(call my-dir)

# Interface provider headers export
include $(CLEAR_VARS)
LOCAL_COPY_HEADERS_TO := interface-provider
LOCAL_COPY_HEADERS := \
    Interface.h \
    InterfaceImplementer.h \
    InterfaceProvider.h \
    InterfaceProviderImpl.h
include $(BUILD_COPY_HEADERS)

# Interface provider library headers export
include $(CLEAR_VARS)
LOCAL_COPY_HEADERS_TO := interface-provider-lib
LOCAL_COPY_HEADERS := \
    InterfaceProviderLib.h
include $(BUILD_COPY_HEADERS)

# Interface provider
include $(CLEAR_VARS)

LOCAL_CFLAGS := \
        -DDEBUG

LOCAL_SRC_FILES := \
        InterfaceProviderImpl.cpp \
        InterfaceProviderLib.cpp

LOCAL_C_INCLUDES += \
        system/core/include/cutils

LOCAL_C_INCLUDES += \
        external/stlport/stlport/ \
        bionic/libstdc++ \
        bionic/

LOCAL_SHARED_LIBRARIES := libstlport libcutils libdl

LOCAL_MODULE := libinterface-provider
LOCAL_MODULE_TAGS := optional

TARGET_ERROR_FLAGS += -Wno-non-virtual-dtor

include $(BUILD_SHARED_LIBRARY)

# Interface provider library
include $(CLEAR_VARS)

LOCAL_CFLAGS := \
        -DDEBUG

LOCAL_SRC_FILES := \
        InterfaceProviderLib.cpp

LOCAL_C_INCLUDES += \
        system/core/include/cutils

LOCAL_C_INCLUDES += \
        external/stlport/stlport/ \
        bionic/libstdc++ \
        bionic/

LOCAL_SHARED_LIBRARIES := libstlport libcutils libdl

LOCAL_MODULE := libinterface-provider-lib
LOCAL_MODULE_TAGS := optional

TARGET_ERROR_FLAGS += -Wno-non-virtual-dtor

include $(BUILD_SHARED_LIBRARY)
