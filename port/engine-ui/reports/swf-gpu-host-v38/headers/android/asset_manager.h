#pragma once
#include <sys/types.h>
#include <stddef.h>
struct AAssetManager;struct AAsset;
#define AASSET_MODE_BUFFER 3
extern "C" {
AAsset* AAssetManager_open(AAssetManager*,const char*,int);
off64_t AAsset_getLength64(AAsset*);
int AAsset_read(AAsset*,void*,size_t);
void AAsset_close(AAsset*);
}
