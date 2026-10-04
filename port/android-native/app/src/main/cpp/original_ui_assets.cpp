#include "original_ui_assets.hpp"
#include "../scene-materials/swf_texture.hpp"
#include "../asset-payloads/sha256.hpp"
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <memory>
#include <stdexcept>

namespace dh2::android_ui {
namespace {
struct Entry {const char* key;const char* uri;const char* asset;const char* digest;std::uint32_t bytes;};
const Entry catalog[]={
#include "original_ui_asset_catalog.inc"
};
struct Close {void operator()(AAsset* p)const{if(p)AAsset_close(p);}};
}
bool OriginalUiAssets::read(std::string_view uri,std::vector<std::uint8_t>& out,std::string& error) const{
    try{
        if(!manager_)throw std::runtime_error("Original UI asset manager unavailable");
        std::string key;
        if(!scene::swf_texture_archive_key(uri,true,false,key,error))return false;
        const auto* found=std::lower_bound(std::begin(catalog),std::end(catalog),key,
            [](const Entry& entry,const std::string& name){return std::strcmp(entry.key,name.c_str())<0;});
        if(found==std::end(catalog)||key!=found->key){
            bool present=false;
            if(!cache_.read(std::string(uri),present,out,error))return false;
            if(present)return true;
            throw std::runtime_error("Original UI resource unavailable: "+std::string(uri));
        }
        std::unique_ptr<AAsset,Close> asset(AAssetManager_open(manager_,found->asset,AASSET_MODE_STREAMING));
        if(!asset)throw std::runtime_error(std::string("Bundled original UI asset missing: ")+found->uri);
        if(AAsset_getLength64(asset.get())!=found->bytes||found->bytes>32u*1024u*1024u)
            throw std::runtime_error("Bundled original UI asset length mismatch");
        std::vector<std::uint8_t> candidate(found->bytes);
        std::size_t done=0;
        while(done<candidate.size()){
            const int bytes=AAsset_read(asset.get(),candidate.data()+done,candidate.size()-done);
            if(bytes<=0)throw std::runtime_error("Short original UI asset read");
            done+=static_cast<std::size_t>(bytes);
        }
        assets::Sha256Digest digest{};
        if(!assets::sha256(candidate.data(),candidate.size(),digest))throw std::runtime_error("Original UI asset digest failed");
        std::string hex;for(auto byte:digest){char text[3];std::snprintf(text,sizeof(text),"%02x",byte);hex+=text;}
        if(hex!=found->digest)throw std::runtime_error("Original UI asset bytes changed: "+std::string(uri));
        out=std::move(candidate);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
