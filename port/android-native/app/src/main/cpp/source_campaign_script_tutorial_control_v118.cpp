#include "source_campaign_script_tutorial_control_v118.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_settings_update_job_v102.hpp"
#include <script_command_receivers_v59.hpp>
#include <canonical_character_candidate_v60.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <player_save_difficulty_global_v29.hpp>
#include <owned_hud_settings_v1.hpp>
#include <cstring>
namespace model_renderer {namespace {
bool required(const char* leaf,std::string& e){if(e.empty())e=std::string("Required actual tutorial control ")+leaf;return false;}
bool word(const dh2::loader::CheckedCommandBorrowV59& command,std::uint32_t offset,std::int32_t& out,std::string& e){
 const auto* field=command.actual_data?command.actual_data->scalar(offset):nullptr;
 if(!field||field->width!=4)return required("generated Data signed word",e);
 std::memcpy(&out,&field->bits,4);return true;
}
bool tutorial(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::int32_t index,std::uint32_t& offset,std::shared_ptr<dh2::ui::OwnedHudSettingsV1>& settings,std::string& e){
 if(!app||(settings=app->source_settings4c_v67())==nullptr)return required("SAME App4c SavegameManager",e);
 //The native owner has the real14-byte constructor/load domain. A source
 //out-of-bounds index is rejected, never clamped to a disabled tutorial.
 if(index<0||static_cast<std::uint32_t>(index)>=settings->tutorials().size())return required("tutorial index within actual settings storage",e);
 offset=0x29u+static_cast<std::uint32_t>(index);
 if(!settings->source_tutorial_cell_v116(offset))return required("actual tutorial byte producer",e);
 return true;
}
bool persist(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,bool job1,std::string& e){
 std::int32_t last{};if(!borrow_native_last_open_menu_id_v118(last,e))return false;
 if(last==17)return save_process_settings_v102(app,e);
 return job1?start_process_settings_job1_v118(app,e):start_process_settings_job_v102(app,e);
}
}
bool execute_source_campaign_script_tutorial_control_v118(const SourceCampaignCandidateBorrowV55& candidate,
 const dh2::loader::CheckedCommandBorrowV59& command,bool skip,std::int32_t module,bool& handled,std::string& e){
 (void)skip;(void)module;handled=false;
 if(!command.descriptor)return required("canonical command descriptor",e);
 const auto kind=command.descriptor->kind;
 if(kind!=77&&kind!=78){e.clear();return true;}handled=true;
 dh2::loader::CheckedCommandBorrowV59 actual;
 if(!command.actual_receiver||!command.actual_receiver->checked_data_borrow(actual,e))return false;
 if(actual.identity!=command.identity||actual.descriptor!=command.descriptor||actual.actual_data!=command.actual_data||
    actual.skip4!=command.skip4||actual.kind8!=command.kind8||actual.data_c!=command.data_c||!command.kind8||*command.kind8!=kind)
  return required("SAME original tutorial command/Data assignment",e);
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world||!world->application)return false;
 const auto app=world->application;
 if(kind==77){ //LockTutorial45925c: ignores skip/module; clear BEFORE save.
  std::int32_t index{};std::uint32_t offset{};std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;
  if(!word(command,8,index,e)||!tutorial(app,index,offset,settings,e)||!settings->source_store_tutorial_v88(offset,0,e))return false;
  return persist(app,true,e);
 }
 //DoTutorial460950 reads local PlayerInfo660 before difficulty/tutorial gates.
 auto pm=app->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* info{};
 if(!pm||!pm->get_local_player(0,false,info,e)||!info)return required("actual PM.GetLocalPlayer(0,false) receiver",e);
 const auto character=info->character660;std::int32_t index{};
 if(!word(command,16,index,e))return false;
 if(!character){e.clear();return true;}
 SourceCampaignCharacterBorrowV62 actor;
 if(!borrow_source_campaign_character_v62(candidate.actual_world,character,actor,e)||!actor.character||!actor.character->save_fields)return false;
 auto& record=*actor.character;std::int32_t difficulty{};
 const dh2::player::CharacterSaveDifficultyBorrowV29 source{record.actor,character,record.save_fields->save_slot14e8(),record.services.difficulty_global};
 if(!dh2::player::character_game_difficulty_v29(source,difficulty,e))return false;
 if(difficulty!=0){e.clear();return true;}
 std::uint32_t offset{};std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;
 if(!tutorial(app,index,offset,settings,e))return false;
 if(!*settings->source_tutorial_cell_v116(offset)){e.clear();return true;}
 auto online=app->get_online_loading_v55();if(!online)return required("actual GetOnline receiver",e);
 if(online->byte5()){e.clear();return true;}
 const char* name=command.actual_data->cstring(12);
 auto scripts=app->source_script_manager_v52();if(!scripts||!name)return required("actual ScriptManager/tutorial script CString",e);
 const auto id=scripts->id_from_name(name,true);
 if(id!=-1&&!scripts->start_script_v96(id,-1,true,e))return false;
 //Original reloads App4c after the synchronous StartScript callbacks.
 if(!tutorial(app,index,offset,settings,e)||!settings->source_store_tutorial_v88(offset,0,e))return false;
 return persist(app,false,e);
}
}
