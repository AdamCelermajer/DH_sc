#include "original_cache_assets_v1.hpp"
#include "../asset-payloads/zip_asset_pack_v1.hpp"
#include "../asset-payloads/sha256.hpp"
#include <android/log.h>
#include <cerrno>
#include <cstring>
#include <limits>
#include <mutex>
#include <unistd.h>
namespace dh2::android_ui {
struct OriginalCacheAssetsV1::State {
    struct Descriptor {
        int fd{-1};off64_t offset{};std::uint64_t bytes{};
        ~Descriptor(){if(fd>=0)close(fd);}
        bool read(std::uint64_t at,void* out,std::size_t n,std::string& e) const{
            if(at>bytes||n>bytes-at||at>std::uint64_t(std::numeric_limits<off64_t>::max()-offset)){
                e="Original cache APK descriptor range invalid";return false;
            }
            auto* cursor=static_cast<unsigned char*>(out);
            std::size_t done=0;
            while(done<n){
                const auto count=pread64(fd,cursor+done,n-done,offset+static_cast<off64_t>(at+done));
                if(count<0&&errno==EINTR)continue;
                if(count<=0){e="Short original cache APK descriptor read";return false;}
                done+=static_cast<std::size_t>(count);
            }
            e.clear();return true;
        }
    };
    assets::ZipAssetPackV1 pack;
    std::size_t files{};
};
namespace {std::mutex mount_mutex;}
bool OriginalCacheAssetsV1::mount(std::size_t& files,std::string& e) const{
    std::lock_guard<std::mutex> lock(mount_mutex);
    if(!manager_){e="Original cache APK asset manager missing";return false;}
    if(!state_){
        auto* asset=AAssetManager_open(manager_,"dh2-original-cache.zip",AASSET_MODE_RANDOM);
        if(!asset){e="Complete original cache missing from APK";return false;}
        auto backing=std::make_shared<State::Descriptor>();off64_t length{};
        backing->fd=AAsset_openFileDescriptor64(asset,&backing->offset,&length);
        AAsset_close(asset);
        if(backing->fd<0||backing->offset<0||length<=0){e="Original cache ZIP must be stored uncompressed inside APK";return false;}
        backing->bytes=static_cast<std::uint64_t>(length);
        assets::ZipBackingV1 source;source.owner=backing;source.bytes=backing->bytes;
        source.read=[backing](std::uint64_t at,void* out,std::size_t n,std::string& error){return backing->read(at,out,n,error);};
        auto next=std::make_shared<State>();
        if(!next->pack.mount(std::move(source),"com.gameloft.android.GAND.GloftD2SS/files/",e))return false;
        next->files=next->pack.entries().size();
        if(next->files!=6833){e="Bundled original cache directory count differs from canonical input";return false;}
        state_=std::move(next);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original cache mounted | files %zu | archive bytes %llu | external storage 0",state_->files,static_cast<unsigned long long>(backing->bytes));
    }
    files=state_->files;e.clear();return true;
}
bool OriginalCacheAssetsV1::read(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,std::string& e) const{
    std::size_t files;if(!mount(files,e))return false;
    std::shared_ptr<State> state;
    {std::lock_guard<std::mutex> lock(mount_mutex);state=state_;}
    if(!state){e="Original cache APK owner changed during read";return false;}
    return state->pack.read(uri,found,out,e);
}
bool OriginalCacheAssetsV1::read_admitted(const std::string& uri,bool& found,std::vector<std::uint8_t>& out,
    const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string& e) const{
    std::size_t files;if(!mount(files,e))return false;
    std::shared_ptr<State> state;
    {std::lock_guard<std::mutex> lock(mount_mutex);state=state_;}
    if(!state){e="Original cache APK owner changed during read";return false;}
    return state->pack.read_admitted(uri,found,out,before_payload,e);
}
bool OriginalCacheAssetsV1::directory(std::vector<std::string>& out,std::string& e) const{
    std::size_t files;if(!mount(files,e))return false;
    std::shared_ptr<State> state;
    {std::lock_guard<std::mutex> lock(mount_mutex);state=state_;}
    if(!state){e="Original cache APK owner changed during directory read";return false;}
    std::vector<std::string> next;next.reserve(files);
    for(const auto& entry:state->pack.entries())next.push_back(entry.uri);
    out=std::move(next);e.clear();return true;
}
bool OriginalCacheAssetsV1::verify_samples(std::string& e) const{
    struct Sample {const char* uri;std::uint32_t bytes;const char* sha;};
    static const Sample samples[]={
#include "original_cache_samples_v1.inc"
    };
    for(const auto& sample:samples){
        bool found=false;std::vector<std::uint8_t> bytes;
        if(!read(sample.uri,found,bytes,e))return false;
        if(!found||bytes.size()!=sample.bytes){e="Required original cache probe missing or wrong length";return false;}
        assets::Sha256Digest digest{};
        if(!assets::sha256(bytes.data(),bytes.size(),digest)){e="Original cache probe digest failed";return false;}
        const char* digits="0123456789abcdef";std::string hex;
        for(auto byte:digest){hex+=digits[byte>>4];hex+=digits[byte&15];}
        if(hex!=sample.sha){e="Original cache probe bytes differ from supplied ZIP";return false;}
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original cache verified | uri %s | bytes %zu | sha256 %s",sample.uri,bytes.size(),hex.c_str());
    }
    e.clear();return true;
}
}
