#pragma once
#include <android/asset_manager.h>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include "original_cache_assets_v1.hpp"

namespace dh2::android_ui {
// Modern native APK backing for exact original resource URIs. The caller owns
// its logical resource directory; this service does not guess by basename.
class OriginalUiAssets {
public:
    explicit OriginalUiAssets(AAssetManager* manager=nullptr):manager_(manager),cache_(manager){}
    void manager(AAssetManager* value){manager_=value;cache_.manager(value);}
    bool mount_cache(std::size_t& files,std::string& error) const{return cache_.mount(files,error);}
    bool verify_cache(std::string& error) const{return cache_.verify_samples(error);}
    bool read(std::string_view uri,std::vector<std::uint8_t>& out,std::string& error) const;
    bool read_cache_admitted_v119(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,
        const std::function<bool(std::uint32_t,std::string&)>& admit,std::string& error) const{
        return cache_.read_admitted(uri,found,out,admit,error);
    }
private:
    AAssetManager* manager_{};
    OriginalCacheAssetsV1 cache_;
};
}
