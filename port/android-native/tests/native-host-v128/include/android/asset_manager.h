#pragma once
#include <stddef.h>
#include <sys/types.h>
//Desktop implementations of only the NDK filesystem API used by this app.
struct AAssetManager;
struct AAsset;
enum { AASSET_MODE_UNKNOWN=0,AASSET_MODE_RANDOM=1,AASSET_MODE_STREAMING=2,AASSET_MODE_BUFFER=3 };
extern "C" {
AAsset* AAssetManager_open(AAssetManager*,const char*,int);
int AAsset_read(AAsset*,void*,size_t);
off64_t AAsset_getLength64(AAsset*);
int AAsset_openFileDescriptor64(AAsset*,off64_t*,off64_t*);
void AAsset_close(AAsset*);
}
