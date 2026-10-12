#pragma once
#include <string>
#include <sys/types.h>
struct AAssetManager {std::string supplied_zip_path;};
struct AAsset {int fd{-1};};
inline constexpr int AASSET_MODE_RANDOM=1;
AAsset* AAssetManager_open(AAssetManager*,const char*,int);
int AAsset_openFileDescriptor64(AAsset*,off64_t*,off64_t*);
void AAsset_close(AAsset*);
