#pragma once
#include "integration-v42/audio_application_manager_v42.hpp"
#include "../game-data/level_tables.hpp"
#include <array>
#include <functional>
namespace dh2::audio {
struct AudioLevelRoutingSourcesV94 {
 std::shared_ptr<void> owner; // actual immutable Arrays owner, independent of World
 const data::LevelTables* levels{};
 // SAME current RES_PATH authority; native strcpy reads this before row+c.
 std::function<bool(std::string&,std::string&)> borrow_res_path;
 // Host source-scope guard after borrowing prefix, before inherited driver.
 // This adds no original audio/resource operation or ready-state substitute.
 std::function<bool(std::string&)> validate_delivery;
};
// VoxSoundManager.SetLevelRouting3695f4. delivered is API completion; routed
// is the source boolean result, ignored by Level's state15 caller.
inline bool set_level_routing_source_v94(const AudioApplicationBorrowV42& captured,
 std::int32_t row,const AudioLevelRoutingSourcesV94& source,bool& routed,std::string& e){
 routed=false;if(row==-1){e.clear();return true;}
 if(!captured.manager){e="Required actual process SoundManager on SetLevelRouting";return false;}
 if(!captured.manager->source_producer_ready_v94(e))return false;
 if(captured.manager->disabled()){e.clear();return true;} // actual JAVA_SOUNDS gate
 if(!source.owner||!source.levels||row<0||std::size_t(row)>=source.levels->levels.size()||!source.borrow_res_path){e="Required SAME actual Arrays.LevelList row/RES_PATH";return false;}
 // Native computes the SAME row pointer before strcpy/strlen/memcpy prefix.
 const auto& actual=source.levels->levels[std::size_t(row)];std::string prefix;
 if(!source.borrow_res_path(prefix,e))return false;
 if(source.validate_delivery&&!source.validate_delivery(e))return false;
 // LevelRecord's historical description name is the first CString at row+c:
 // actual schema DynamicBusRouting. LevelDescription is scalar word6.
 std::string path(prefix.c_str());path+="data/routing/";path+=actual.dynamic_bus_routing_v94().c_str();
 if(path.size()>=512){e="Original SetLevelRouting512-byte buffer overflow rejected";return false;}
 auto* runtime=captured.manager->runtime_on_producer();
 if(!runtime||!runtime->source_dynamic_bus_routing_v94(path.c_str(),e)){if(e.empty())e="Required SAME constructed Vox driver dynamic routing slot";return false;}
 routed=true;e.clear();return true;
}
// Genuine current EmitterHandle domain returned by Main's existing Vox owner.
// Token must refer to that real voice; owner pins its native handle/value storage.
struct AudioStopEmitterBorrowV94 {std::shared_ptr<void> owner;std::uint64_t token{};};
struct AudioStopPrefixV94 {
 std::array<AudioStopEmitterBorrowV94,10> handles;
 bool constructor_started{},constructor_completed{},enumeration_completed{};
 unsigned count{},stopped{},destroyed{};
};
struct AudioStopPrimitivesV94 {
 std::shared_ptr<void> owner; // independent actual process audio authority
 std::function<bool(int,std::string&)> platform_stop; // exact nativeStopSoundBig
 // Existing native cleanup authority retains the SAME call-prefix resources
 // before actual value C1. It retires only after all genuine value D1 leaves.
 std::function<bool(const std::shared_ptr<AudioStopPrefixV94>&,std::string&)> retain_prefix;
 std::function<bool(const std::shared_ptr<AudioStopPrefixV94>&,std::string&)> retire_completed_prefix;
 // Native GetEmitterHandles8689bc: physical map78 ascending key, then virtual
 // map98 ascending key, matching SAME DataObj118; stops at10 TOTAL handles.
 std::function<bool(AudioGameplayRuntimeV42&,std::uintptr_t actual_slot,int actual_uid,
   std::array<AudioStopEmitterBorrowV94,10>&,unsigned&,std::string&)> emitter_handles;
 std::function<bool(AudioGameplayRuntimeV42&,const AudioStopEmitterBorrowV94&,float,std::string&)> stop_emitter;
 // Ten EmitterHandle value slots are constructed before enumeration. Their
 // native D1 runs reverse array order, including unused slots; Main lends the
 // real value-resource prefix, never ten invented actor/driver receivers.
 std::function<bool(std::array<AudioStopEmitterBorrowV94,10>&,std::string&)> construct_handles;
 std::function<bool(AudioStopEmitterBorrowV94&,std::string&)> destroy_handle;
};
// VoxSoundManager.Stop369fec source body over SAME manager/voices. No clock,
// JNI fallback, sound-file load, implicit LoadSound, or replacement voice map.
inline bool stop_sound_source_v94(const AudioApplicationBorrowV42& captured,int ordinal,
 int fade_ms,const AudioStopPrimitivesV94& leaves,std::string& e){
 if(ordinal<0){e.clear();return true;} // BEFORE process platform-byte read
 if(!captured.manager||!captured.manager->source_producer_ready_v94(e)){if(e.empty())e="Required actual process Stop receiver";return false;}
 if(captured.manager->disabled()){
  if(!leaves.owner||!leaves.platform_stop){e="Required reached nativeStopSoundBig JNI leaf";return false;}
  return leaves.platform_stop(ordinal,e); // native branch ignores fade_ms
 }
 auto* runtime=captured.manager->runtime_on_producer();
 const auto* row=runtime?runtime->bindings().row(ordinal):nullptr;
 if(!runtime||!runtime->source_data_initialized()||!row){e="Required SAME initialized generated Stop ordinal";return false;}
 const auto slot=runtime->source_slot_identity_v101(row->uid);
 if(!slot){e.clear();return true;} // native SoundManager8[uid] NULL branch
 // Loaded source slot owns a completed decoded sample. No load/read is issued.
 if(!runtime->source_slot_ready_v94(row->uid)){e.clear();return true;}
 // Native i2f, then div by float1000, preserving signed fade input.
 const float seconds=float(fade_ms)/1000.f;
 if(!leaves.owner||!leaves.retain_prefix||!leaves.retire_completed_prefix||!leaves.construct_handles||!leaves.emitter_handles||!leaves.stop_emitter||!leaves.destroy_handle){e="Required actual Vox handle construction/enumeration/Stop/D1 leaves";return false;}
 auto prefix=std::make_shared<AudioStopPrefixV94>();
 if(!leaves.retain_prefix(prefix,e))return false;
 prefix->constructor_started=true;
 if(!leaves.construct_handles(prefix->handles,e))return false;
 prefix->constructor_completed=true;
 if(!leaves.emitter_handles(*runtime,slot,row->uid,prefix->handles,prefix->count,e))return false;
 prefix->enumeration_completed=true;
 if(prefix->count>prefix->handles.size()){e="Native GetEmitterHandles exceeded actual ten-slot capacity";return false;}
 for(unsigned i=0;i<prefix->count;++i){if(!prefix->handles[i].owner||!prefix->handles[i].token){e="Required actual returned Vox emitter handle";return false;}if(!leaves.stop_emitter(*runtime,prefix->handles[i],seconds,e))return false;++prefix->stopped;}
 for(unsigned i=prefix->handles.size();i;--i){if(!leaves.destroy_handle(prefix->handles[i-1],e))return false;++prefix->destroyed;}
 if(!leaves.retire_completed_prefix(prefix,e))return false;
 e.clear();return true;
}
}
