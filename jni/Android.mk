LOCAL_PATH := $(call my-dir)

#include $(LOCAL_PATH)/sdl/Android.mk

#LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := src

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/sdl_ttf

# Add your application source files here...
LOCAL_SRC_FILES := demo.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)

MP := $(LOCAL_PATH)
include $(MP)/sdl/Android.mk
include $(MP)/sdl_ttf/Android.mk