#pragma once
#include "asset_manager.h"
#include <jni.h>
extern "C" AAssetManager* AAssetManager_fromJava(JNIEnv*,jobject);
