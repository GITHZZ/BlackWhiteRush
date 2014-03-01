LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BackDrop.cpp \
                   ../../Classes/Barrier.cpp \
                   ../../Classes/SoundResources.h \
                   ../../Classes/GameEmitter.cpp \
                   ../../Classes/GameLogic.cpp \
                   ../../Classes/GameObject.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/Land.cpp \
                   ../../Classes/ProceedController.cpp \
                   ../../Classes/ProceedPause.cpp \
                   ../../Classes/ProceedScene.cpp \
                   ../../Classes/ProceedView.cpp \
                   ../../Classes/PropBox.cpp \
                   ../../Classes/Props.cpp \
                   ../../Classes/Role.cpp \
                   ../../Classes/StartScene.cpp \
                   ../../Classes/ProceedEnd.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
				 	$(LOCAL_PATH)/../../../cocos2dx\
					$(LOCAL_PATH)/../../../cocos2dx/platform\
					$(LOCAL_PATH)/../../../cocos2dx/include\
  				    $(LOCAL_PATH)/../../../cocosDension/include\
  				    $(LOCAL_PATH)/../../../external
 
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)