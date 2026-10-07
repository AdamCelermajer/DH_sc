#include "checkpoint_save_runtime_v83.hpp"
#include "campaign_save_filename_v45.hpp"
namespace dh2::level {namespace {
template<class F,class... A>bool invoke(const F& f,const char* name,std::string& e,A&&... a){if(!f){e=std::string("Required actual checkpoint ")+name;return false;}return f(std::forward<A>(a)...,e);}
}
bool player_save_checkpoint_v83(data::PlayerSaveLoadOwnerV1& authority,
 CampaignSaveProfileV45* profile,const PlayerCheckpointServicesV83& s,std::string& e){
 // Actual source guard466190/1b4 occurs before filename/online queries.
 if(!authority.profile().identity||authority.save_disabled()){e.clear();return true;}
 if(!profile||authority.profile().identity!=reinterpret_cast<std::uintptr_t>(profile)||authority.profile().owner.get()!=profile){e="Checkpoint profile does not match SAME source Save8";return false;}
 bool online{};if(!invoke(s.online,"GetOnline byte5",e,online))return false;
 bool multi=false;if(online){bool hosting{};if(!invoke(s.local_hosting,"PM.IsLocalPlayerHosting",e,hosting))return false;
  if(!hosting)multi=true;else{std::uint8_t flag{};if(!invoke(s.manager719,"SAME PM719",e,flag))return false;multi=flag!=0;}}
 const auto slot=static_cast<std::uint32_t>(authority.save().slot());
 if(!profile->source_filename_store_v83(campaign_save_filename_v45(slot,true,multi),e))return false;
 // Source deliberately rereads online after changing the actual profile path.
 if(!invoke(s.online,"GetOnline byte5 reread",e,online))return false;
 if(online){authority.source_save_mode_store_v83(2);
  if(!invoke(s.setup_stream,"_SetupStream468630(true,false)",e,true,false)||!invoke(s.ensure_stream,"actual empty Stream1c constructor",e))return false;
  if(!profile->save_all(e)||!invoke(s.setup_stream,"_SetupStream468630(false,false)",e,false,false))return false;
 }else{authority.source_save_mode_store_v83(1);if(!profile->save_all(e))return false;}
 // No scope-exit rollback: failures above retain filename/mode/source jobs.
 return profile->source_filename_store_v83(campaign_save_filename_v45(slot,false,false),e);
}
bool LevelSavegameRuntimeV1::source_save_checkpoint_v83(std::uint32_t seed,
 std::int32_t difficulty,std::int32_t row,std::string& e){
 auto cache=cache_.lock();if(!cache){e.clear();return true;} //463168 actual NULL Savegame4 branch
 if(!same_cache(cache.get(),e))return false;
 bool online=false;if(!application_.network_online||!application_.network_online(application_.context,online,e))return false;
 bool multi=false;if(online){bool hosting{};if(!application_.is_hosting||!application_.is_hosting(application_.context,hosting,e))return false;
  if(!hosting)multi=true;else{std::uint8_t flag{};if(!application_.manager_flag719||!application_.manager_flag719(application_.context,flag,e))return false;multi=flag!=0;}}
 const auto mode=owner_.fields().mode0c;
 if(!cache->source_filename_store_v83(LevelSavegameOwnerV1::checkpoint_filename(seed,mode,multi),e))return false;
 if(!save(this,cache.get(),e))return false;
 return cache->source_filename_store_v83(LevelSavegameOwnerV1::filename(seed,difficulty,row,mode),e);
}
}
