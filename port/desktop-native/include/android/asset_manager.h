#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#define AASSET_MODE_UNKNOWN 0
#define AASSET_MODE_RANDOM 1
#define AASSET_MODE_STREAMING 2
#define AASSET_MODE_BUFFER 3

struct AAssetManager {
    std::filesystem::path root;
    std::mutex cache_mutex;
    std::unordered_map<std::string,
        std::unordered_map<std::string, std::filesystem::path>> directory_entries;
};

struct AAsset {
    std::ifstream stream;
    std::int64_t length{};
};

// Desktop-only counterpart to the NDK asset API. Paths are resolved relative
// to the configured asset root, with Android-style case-sensitive components.
AAssetManager* AAssetManager_createFromFilesystem(const char* root);
void AAssetManager_delete(AAssetManager* manager);
AAsset* AAssetManager_open(AAssetManager* manager, const char* filename, int mode);
std::int64_t AAsset_getLength64(AAsset* asset);
int AAsset_read(AAsset* asset, void* buffer, std::size_t count);
void AAsset_close(AAsset* asset);
