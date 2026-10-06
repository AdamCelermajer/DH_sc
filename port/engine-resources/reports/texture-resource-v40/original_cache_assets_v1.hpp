#pragma once
#include <android/asset_manager.h>
#include <cstdint>
#include <memory>
#include <functional>
#include <string>
#include <vector>
namespace dh2::android_ui {
class OriginalCacheAssetsV1 {
    struct State;
    AAssetManager* manager_{};
    mutable std::shared_ptr<State> state_;
public:
    explicit OriginalCacheAssetsV1(AAssetManager* manager=nullptr):manager_(manager){}
    void manager(AAssetManager* manager){manager_=manager;state_.reset();}
    bool mount(std::size_t& files,std::string& error) const;
    bool verify_samples(std::string& error) const;
    bool read(const std::string&,bool& found,std::vector<std::uint8_t>&,std::string&) const;
    bool read_admitted(const std::string&,bool& found,std::vector<std::uint8_t>&,
        const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string&) const;
    bool directory(std::vector<std::string>&,std::string&) const;
};
}
