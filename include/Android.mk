LOCAL_PATH := $(call my-dir)

# Audioutilities headers export
include $(CLEAR_VARS)
LOCAL_COPY_HEADERS_TO := audioutilities-include
LOCAL_COPY_HEADERS := \
    AudioBand.h \
    AudioThrottle.h \

LOCAL_USE_VNDK := true
include $(BUILD_COPY_HEADERS)
