LOCAL_PATH := $(call my-dir)

# Common variables
##################

event_thread_exported_includes_folder := event-listener
event_thread_exported_includes_files := \
    EventListener.h \
    EventThread.h

event_thread_src_files := \
    EventThread.cpp

event_thread_cflags := -DDEBUG

# Build for target
##################

include $(CLEAR_VARS)
LOCAL_COPY_HEADERS_TO := $(event_thread_exported_includes_folder)
LOCAL_COPY_HEADERS := $(event_thread_exported_includes_files)

LOCAL_SRC_FILES := $(event_thread_src_files)

LOCAL_C_INCLUDES += \
        external/stlport/stlport \
        bionic

LOCAL_CFLAGS := $(event_thread_cflags)
TARGET_ERROR_FLAGS += -Wno-non-virtual-dtor

LOCAL_SHARED_LIBRARIES := libstlport libcutils

LOCAL_MODULE := libevent-listener
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# Build for host test
#####################

include $(CLEAR_VARS)

LOCAL_COPY_HEADERS_TO := $(event_thread_exported_includes_folder)
LOCAL_COPY_HEADERS := $(event_thread_exported_includes_files)

LOCAL_SRC_FILES := $(event_thread_src_files)

LOCAL_MODULE := libevent-listener_static_host
LOCAL_MODULE_TAGS := optional

include $(BUILD_HOST_STATIC_LIBRARY)

