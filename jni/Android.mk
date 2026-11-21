LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := retro
LOCAL_SRC_FILES := ../src/core.c ../src/machine.c ../src/psg.c
LOCAL_CFLAGS := -std=c11 -O2 -fvisibility=hidden
include $(BUILD_SHARED_LIBRARY)
