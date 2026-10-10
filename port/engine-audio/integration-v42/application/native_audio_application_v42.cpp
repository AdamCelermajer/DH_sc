#include "native_audio_application_v42.hpp"
#include "../../../android-native/app/src/main/cpp/original_cache_assets_v1.hpp"
#include "disabled_bootstrap_v42.hpp"
#include "application_audio_serial_v42.hpp"
#include <android/asset_manager_jni.h>
#include <mutex>
#include <stdexcept>
namespace dh2::android_audio {
namespace {
struct AssetProvider {
    JavaVM* vm{};jobject java_assets{};
    dh2::android_ui::OriginalCacheAssetsV1 cache;
    dh2::audio::AudioGameplaySourcesV40 playback;
    std::shared_ptr<void> world_lease;
    explicit AssetProvider(AAssetManager* assets):cache(assets){}
    ~AssetProvider() {
        if(!vm||!java_assets)return;
        JNIEnv* env=nullptr;bool attached=false;
        if(vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_6)!=JNI_OK) {
            if(vm->AttachCurrentThread(&env,nullptr)!=JNI_OK)return;
            attached=true;
        }
        env->DeleteGlobalRef(java_assets);
        if(attached)vm->DetachCurrentThread();
    }
    static bool read(void* raw,const char* uri,std::shared_ptr<const std::vector<std::uint8_t>>& out,std::string& error) {
        auto& self=*static_cast<AssetProvider*>(raw);
        if(!uri){error="Required exact original audio asset URI";return false;}
        bool found=false;std::vector<std::uint8_t> bytes;
        if(!self.cache.read(uri,found,bytes,error))return false;
        if(!found){error=std::string("Required exact original audio asset ")+uri;return false;}
        out=std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes));return true;
    }
    static bool read_optional(void* raw,const char* uri,bool& found,std::shared_ptr<const std::vector<std::uint8_t>>& out,std::string& error) {
        auto& self=*static_cast<AssetProvider*>(raw);found=false;out.reset();
        if(!uri){error="Required exact original audio asset URI";return false;}
        std::vector<std::uint8_t> bytes;
        if(!self.cache.read(uri,found,bytes,error))return false;
        if(found)out=std::make_shared<const std::vector<std::uint8_t>>(std::move(bytes));
        return true;
    }
    static int gates(void* raw,const dh2::sound::VoxPlay3DRequestV2& query,dh2::sound::VoxPlay3DResponseV2& out) {
        auto& self=*static_cast<AssetProvider*>(raw);
        if(!self.world_lease||!self.playback.gates.invoke)return -1;
        return self.playback.gates.invoke(self.playback.gates.context,query,out);
    }
    static bool random(void* raw,int& value) {
        auto& self=*static_cast<AssetProvider*>(raw);
        return self.world_lease&&self.playback.random.next&&self.playback.random.next(self.playback.random.context,value);
    }
    static bool command(void* raw,const dh2::character::CombatSoundPlayV1& play,const dh2::audio::AudioSoundV34& sound,
                        const dh2::audio::AudioGroupV34& group,dh2::audio::AudioCommandV34& out,std::string& error) {
        auto& self=*static_cast<AssetProvider*>(raw);
        if(!self.world_lease||!self.playback.source_command){error="Required actual active GS/listener/emitter/settings publication";return false;}
        return self.playback.source_command(self.playback.context,play,sound,group,out,error);
    }
};
struct Registry {
    std::mutex mutex;
    std::shared_ptr<dh2::audio::AudioApplicationManagerV42> manager;
    std::shared_ptr<AssetProvider> provider;
    bool attempted{},building{},close_requested{};
    ApplicationAudioSerialV42 serial;
    std::string construction_error;
};
// Process lifetime quarantine/owner: no static destruction can release callback
// storage without the explicit producer close/join/drain barrier below.
Registry& registry(){static auto* value=new Registry;return *value;}
}
bool ensure_application_audio_v42(JNIEnv* env,jobject assets,std::string& error) {
    auto& state=registry();std::shared_ptr<dh2::audio::AudioApplicationManagerV42> existing;
    {
        std::lock_guard<std::mutex> lock(state.mutex);existing=state.manager;
        if(!state.construction_error.empty()){error=state.construction_error;return false;}
        if(existing&&state.close_requested){error="Previous Application audio owner is closing; producer shutdown barrier required before startup";return false;}
        if(!existing) {
            if(state.close_requested){state.attempted=true;state.construction_error=error="Reserved Application audio construction cancelled";return false;}
            if(state.attempted){error="Application audio construction is already pending";return false;}
            state.attempted=true;state.building=true;
            if(!state.serial.reserve()){state.building=false;error="Application audio owner serial exhausted";state.construction_error=error;return false;}
        }
    }
    if(existing) {
        if(!existing->runtime_on_producer()){error="Application audio producer changed; old producer must complete shutdown before replacement";return false;}
        error.clear();return true;
    }
    std::shared_ptr<AssetProvider> provider;
    std::shared_ptr<dh2::audio::AudioApplicationManagerV42> manager;
    try {
        if(!env||!assets)throw std::runtime_error("Required actual Java AssetManager for Application audio construction");
        auto* actual=AAssetManager_fromJava(env,assets);
        if(!actual)throw std::runtime_error("Required actual native AssetManager");
        provider=std::make_shared<AssetProvider>(actual);
        if(env->GetJavaVM(&provider->vm)!=JNI_OK)throw std::runtime_error("Required actual audio JavaVM lease");
        provider->java_assets=env->NewGlobalRef(assets);
        if(!provider->java_assets||env->ExceptionCheck()){env->ExceptionClear();throw std::runtime_error("Audio AssetManager global-reference construction failed");}
        dh2::audio::AudioGameplaySourcesV40 sources;
        sources.context=provider.get();sources.exact_assets={provider.get(),AssetProvider::read,AssetProvider::read_optional};
        sources.gates={provider.get(),AssetProvider::gates};sources.random={provider.get(),AssetProvider::random};sources.source_command=AssetProvider::command;
        manager=dh2::audio::AudioApplicationManagerV42::create(sources,provider,source_disabled_bootstrap_v42,error);
    } catch(const std::exception& failure){error=failure.what();}
    bool cancelled=false;
    {
        std::lock_guard<std::mutex> lock(state.mutex);cancelled=state.close_requested;
        if(manager&&!cancelled){state.provider=provider;state.manager=manager;state.building=false;error.clear();return true;}
    }
    if(manager&&cancelled) {
        manager->request_output_close();std::string close_error;
        if(!manager->shutdown(close_error)) {
            error="Application audio construction cancelled; owner retained: "+close_error;
            std::lock_guard<std::mutex> lock(state.mutex);state.manager=manager;state.provider=provider;
        } else {
            //No publication occurred and the real producer close/join/drain
            //completed. Retire this cancelled incarnation; a later surface
            //may reserve a fresh token. Never latch cancellation as a permanent
            //construction failure or reuse the closed manager.
            std::lock_guard<std::mutex> lock(state.mutex);
            state.building=false;state.attempted=false;state.close_requested=false;
            state.serial.clear();state.construction_error.clear();
            error="Application audio construction cancelled before publication; shutdown complete";
            return false;
        }
    }
    if(error.empty())error="Required actual Application audio construction";
    std::lock_guard<std::mutex> lock(state.mutex);state.building=false;state.construction_error=error;return false;
}
bool borrow_application_audio_v42(dh2::audio::AudioApplicationBorrowV42& out,std::string& error) {
    auto& state=registry();std::lock_guard<std::mutex> lock(state.mutex);
    if(state.close_requested){error="Application audio owner shutdown pending; complete its producer barrier before borrowing";return false;}
    if(state.building||!state.construction_error.empty()){error=state.construction_error.empty()?"Application audio construction pending":state.construction_error;return false;}
    out.manager=state.manager;error.clear();return true; // actual nullable global
}
bool publish_actual_playback_v42(const dh2::audio::AudioGameplaySourcesV40& sources,std::shared_ptr<void> lease,std::string& error) {
    auto& state=registry();std::lock_guard<std::mutex> lock(state.mutex);
    if(!state.manager||!state.manager->runtime_on_producer()){error="Required same constructed Application audio producer";return false;}
    if(!lease||!sources.gates.invoke||!sources.random.next||!sources.source_command){error="Required complete actual GS/Vox RNG/listener/command provider lease";return false;}
    state.provider->playback=sources;state.provider->world_lease=std::move(lease);error.clear();return true;
}
bool unpublish_actual_playback_v101(const std::shared_ptr<void>&expected,std::string&error){
 auto&state=registry();std::lock_guard<std::mutex>lock(state.mutex);
 if(!state.manager||!state.manager->runtime_on_producer()||!state.provider||!expected||state.provider->world_lease!=expected){
  error="Required SAME actual campaign audio provider for release";return false;
 }
 state.provider->playback={};state.provider->world_lease.reset();error.clear();return true;
}
bool detach_expected_playback_v102(const dh2::audio::AudioApplicationBorrowV42&captured,
 const std::shared_ptr<void>&expected,bool&detached,std::string&error){
 detached=false;
 if(!captured.manager||!expected){error="Required captured expected audio manager/provider";return false;}
 auto&state=registry();std::lock_guard<std::mutex>lock(state.mutex);
 // Absence or replacement is legal during retirement. Do not clear another
 // Application or a newly published campaign, even if its raw address aliases.
 if(state.manager!=captured.manager||!state.provider||!state.provider->world_lease||
    state.provider->world_lease.get()!=expected.get()||
    state.provider->world_lease.owner_before(expected)||expected.owner_before(state.provider->world_lease)){
  error.clear();return true;
 }
 if(!state.manager->runtime_on_producer()){error="Required captured audio producer for final provider detach";return false;}
 state.provider->playback={};state.provider->world_lease.reset();detached=true;error.clear();return true;
}
std::uint64_t application_audio_owner_v42() {
    auto& state=registry();std::lock_guard<std::mutex> lock(state.mutex);return state.serial.current();
}
std::uint64_t reserve_application_audio_v42() {
    auto& state=registry();std::lock_guard<std::mutex> lock(state.mutex);
    //Each new surface claims its own empty reservation. Reusing the old token
    //lets an old Activity.onDestroy cancel the new startup before construction.
    //No manager/callback storage exists in this branch, so no barrier is owed.
    //Attempted failures and constructing/live owners retain their authority.
    if(!state.manager&&!state.building&&!state.attempted){
        state.serial.clear();state.close_requested=false;
    }
    return state.serial.reserve();
}
void request_application_audio_close_v42(std::uint64_t expected) {
    auto& state=registry();std::shared_ptr<dh2::audio::AudioApplicationManagerV42> manager;
    {std::lock_guard<std::mutex> lock(state.mutex);if(!state.serial.matches(expected))return;state.close_requested=true;manager=state.manager;}
    if(manager)manager->request_output_close();
}
bool shutdown_application_audio_v42(std::uint64_t expected,std::string& error) {
    auto& state=registry();std::shared_ptr<dh2::audio::AudioApplicationManagerV42> manager;
    {
        std::lock_guard<std::mutex> lock(state.mutex);
        if(!state.serial.matches(expected)){error.clear();return true;}
        state.close_requested=true;
        if(state.building){error="Application audio construction cancellation pending; owner retained";return false;}
        manager=state.manager;
    }
    if(manager&&!manager->shutdown(error))return false;
    std::shared_ptr<AssetProvider> released;
    {
        std::lock_guard<std::mutex> lock(state.mutex);
        if(state.manager!=manager){error="Application audio owner changed during shutdown";return false;}
        state.manager.reset();released=std::move(state.provider);state.attempted=false;state.close_requested=false;state.serial.clear();state.construction_error.clear();
    }
    released.reset();error.clear();return true;
}
}
