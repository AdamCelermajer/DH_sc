#pragma once
#include <android/asset_manager.h>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::android_ui {
// Modern native APK backing for exact original resource URIs. The caller owns
// its logical resource directory; this service does not guess by basename.
class OriginalUiAssets {
public:
    explicit OriginalUiAssets(AAssetManager* manager=nullptr):manager_(manager){}
    void manager(AAssetManager* value){manager_=value;}
    bool read(std::string_view uri,std::vector<std::uint8_t>& out,std::string& error) const;
private:
    AAssetManager* manager_{};
};
}
