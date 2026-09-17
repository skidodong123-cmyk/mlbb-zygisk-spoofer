LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mlbb_spoofer
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS := -llog
LOCAL_CPPFLAGS := -std=c++17 -fvisibility=hidden -fPIC
include $(BUILD_SHARED_LIBRARY)
