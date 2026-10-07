#include "source_campaign_script_audio_v117.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include <script_command_receivers_v59.hpp>
#include <character_design_services.hpp>
#include <cstring>
namespace model_renderer {namespace {
bool scalar(const dh2::loader::CheckedCommandBorrowV59& c,unsigned offset,unsigned width,std::uint32_t& value,std::string& e){
 const auto* field=c.actual_data?c.actual_data->scalar(offset):nullptr;
 if(!field||field->width!=width){e="Required original script audio field "+std::to_string(offset)+" width "+std::to_string(width);return false;}
 value=field->bits;return true;
}
bool word(const dh2::loader::CheckedCommandBorrowV59& c,unsigned offset,std::int32_t& value,std::string& e){
 std::uint32_t bits{};if(!scalar(c,offset,4,bits,e))return false;std::memcpy(&value,&bits,4);return true;
}
bool trace(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world||!world->debug||!world->debug_files){if(e.empty())e="Required SAME script audio Debug owner";return false;}
 if(dh2_character_debug_load(world->debug.get(),world->debug_files)<0){e="Script audio Debug.load failed";return false;}
 std::uint32_t ignored{};if(dh2_character_debug_get(&ignored,world->debug.get(),"isTracingScriptCmd",world->debug_files)<0){e="Script audio Debug.GetSwitch failed";return false;}return true;
}
//Original Execute returns void and discards Play's invalid-file result.
//Only a genuine retained unavailable datasource qualifies: its diagnostic
//remains on the SAME UID slot, readiness stays false, and no voice is made.
bool play_result(bool delivered,const dh2::audio::AudioApplicationBorrowV42& manager,int ordinal,std::string& e){
 if(delivered){e.clear();return true;}
 auto* runtime=manager.manager?manager.manager->runtime_on_producer():nullptr;
 const auto* row=runtime?runtime->bindings().row(ordinal):nullptr;
 if(row&&runtime->source_slot_unavailable_v100(row->uid)&&!runtime->source_slot_ready_v94(row->uid)){e.clear();return true;}
 return false;
}
}
bool execute_source_campaign_script_audio_v117(const SourceCampaignCandidateBorrowV55& candidate,
 const dh2::loader::CheckedCommandBorrowV59& command,bool skip,std::int32_t module,bool& handled,std::string& e){
 (void)module;handled=false;
 if(!command.descriptor){e="Required actual script command descriptor";return false;}
 const int kind=command.descriptor->kind;if(kind<13||kind>15){e.clear();return true;}handled=true;
 dh2::loader::CheckedCommandBorrowV59 actual;
 if(!command.actual_receiver||!command.actual_receiver->checked_data_borrow(actual,e))return false;
 if(actual.identity!=command.identity||actual.actual_data!=command.actual_data||actual.descriptor!=command.descriptor||
    actual.skip4!=command.skip4||actual.kind8!=command.kind8||actual.data_c!=command.data_c||!command.kind8||*command.kind8!=kind){e="Foreign/copied script audio command loan";return false;}
 std::uint32_t music{};
 if(kind!=15&&!scalar(command,12,1,music,e))return false;
 if(kind==13&&skip&&!music){e.clear();return true;} //45fefc skips only nonmusic.
 if(!trace(candidate,e))return false;
 if(kind==15){ //45fd20 ignores skip and guards CurrentLevel before Vox.
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!borrow_current_native_level_v27(current,e))return false;if(!current){e.clear();return true;}
  if(current.level()!=candidate.level){e="Script PlayLevelMusic requires SAME current Level";return false;}
  const auto fields=current.level()->config_fields();if(!fields.music11c){e="Required actual Level music11c";return false;}
  std::int32_t fade{};if(!word(command,8,fade,e))return false;
  dh2::audio::AudioApplicationBorrowV42 manager;if(!borrow_actual_application_audio_v42(manager,e)||!manager.manager){if(e.empty())e="Required actual script music SoundManager";return false;}
  const int id=*fields.music11c;
  if(!play_result(play_campaign_music_v101(candidate.actual_world,id,true,false,fade,e),manager,id,e))return false;
  return set_campaign_music_state_v101(candidate.actual_world,manager.manager->music_fields_on_producer().ambient_31?"ambient":"combat",e);
 }
 std::int32_t fade{};if(!word(command,8,fade,e))return false;
 if(kind==14&&music)return stop_campaign_music_v117(candidate.actual_world,fade,e); //45fe2c ignores skip.
 std::int32_t id{};if(!word(command,16,id,e))return false;
 if(kind==14)return stop_campaign_sound_v106(candidate.actual_world,id,fade,e);
 std::uint32_t loop{};if(!scalar(command,13,1,loop,e))return false;
 dh2::audio::AudioApplicationBorrowV42 manager;if(!borrow_actual_application_audio_v42(manager,e)||!manager.manager){if(e.empty())e="Required actual script Play SoundManager";return false;}
 const bool delivered=music?play_campaign_music_v101(candidate.actual_world,id,loop!=0,fade!=0,2000,e):
  play_campaign_plain_sound_v115(candidate.actual_world,id,loop!=0,fade,0,false,e);
 return play_result(delivered,manager,id,e);
}
}
