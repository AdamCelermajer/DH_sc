#include "source_script_ui_world_v97.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "application_services_owner_v5.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "script_manager_owner_v52.hpp"
#include "renderer_native_menu_prefix_v62.hpp"
#include "model_renderer.hpp"
namespace dh2::android_ui {namespace {
const char* running_name9a5fd8{}; //original source BSS pointer
std::weak_ptr<loader::ScriptManagerOwnerV52> running_name_owner;
bool required(std::string& e,const char* op){if(e.empty())e=std::string("Required actual script UI provider: ")+op;return false;}
}
bool SourceScriptUiWorldV97::create(const model_renderer::SourceCampaignCandidateBorrowV55& candidate,
 std::shared_ptr<SourceScriptUiWorldV97>& out,std::string& e){
 std::shared_ptr<model_renderer::SourceWorldBorrowV61> world;
 if(!model_renderer::borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto transport=std::shared_ptr<SourceScriptUiWorldV97>(new SourceScriptUiWorldV97());
 transport->world_=world;transport->application_=candidate.application;out=std::move(transport);e.clear();return true;
}
bool SourceScriptUiWorldV97::player_character(std::uintptr_t& character,std::string& e)const{
 const auto application=application_.lock();const auto pm=application?application->source_player_manager_v59():nullptr;
 if(!pm)return required(e,"SAME Application.PlayerManager");
 player::PlayerInfoFieldsV1* info{};if(!pm->get_local_player(0,true,info,e)||!info)return required(e,"actual GetLocalPlayer(0,true)");
 character=info->character660;e.clear();return true;
}
bool SourceScriptUiWorldV97::player_name(std::uintptr_t character,std::string& out,std::string& e)const{
 const auto world=world_.lock();if(!world||!world->owner)return required(e,"live source Character dialogue-name scope");
 model_renderer::SourceCampaignCharacterBorrowV62 borrowed;
 if(!model_renderer::borrow_source_campaign_character_v62(world->owner,character,borrowed,e)||!borrowed.character)return false;
 const auto& record=borrowed.character;
 if(!record->save_fields||!record->save_fields->save_slot14e8())return required(e,"Character.Save14e8 cell");
 if(!*record->save_fields->save_slot14e8()){out.clear();e.clear();return true;}
 if(!record->save||record->save->character()!=character||*record->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record->save.get()))return required(e,"SAME selected Save/PNAM owner");
 out=record->save->name();e.clear();return true;
}
bool SourceScriptUiWorldV97::style_name(std::int32_t style,std::string& out,std::string& e)const{
 const auto world=world_.lock();if(!world||!world->design)return required(e,"SAME process game-design constant table");
 auto design=world->design->borrow();const auto* name=design.constant_name_v97("DialogStyles",style);
 if(!name)return required(e,"actual PyDataConstants.getConstantName");
 out=name;e.clear();return true;
}
bool SourceScriptUiWorldV97::before_stop_message(std::string& e)const{
 const auto application=application_.lock();const auto pm=application?application->source_player_manager_v59():nullptr;
 if(!pm)return required(e,"NativeStopMessage actual Application PM");
 player::PlayerInfoFieldsV1* info{};if(!pm->get_local_player(0,false,info,e))return false;
 e.clear();return true; //original result is unused, including real NULL
}
bool SourceScriptUiWorldV97::before_stop_dialog(std::string& e)const{
 const auto application=application_.lock();const auto online=application?application->get_online_loading_v55():nullptr;
 if(!online)return required(e,"NativeStopMessage actual COnline");
 if(!online->byte5()){e.clear();return true;}
 return required(e,"online hosting selector/CMsgScriptCmd(kind3,-1,-1) SendMessage");
}
bool SourceScriptUiWorldV97::publish_current_name(const char* name,std::string& e)const{
 const auto application=application_.lock();const auto manager=application?application->source_script_manager_v52():nullptr;
 if(!manager||!name)return required(e,"actual ScriptManager owned name pointer");
 bool owned=false;for(const auto& entry:manager->names())if(entry.get()==name){owned=true;break;}
 if(!owned)return required(e,"ExecuteScript name pointer belongs to another owner");
 running_name9a5fd8=name;running_name_owner=manager;e.clear();return true;
}
const char* source_running_script_name_v97()noexcept{
 auto owner=running_name_owner.lock();if(!owner)return nullptr;
 //Names may have been unloaded while the App owner stays alive.
 for(const auto& name:owner->names())if(name.get()==running_name9a5fd8)return running_name9a5fd8;
 return nullptr;
}
bool SourceScriptUiWorldV97::send_script_message(bool,std::int32_t,std::int32_t,std::string& e)const{
 return required(e,"positive CMsgScriptCmd constructor/network SendMessage transport");
}
bool SourceScriptUiWorldV97::store_controller_global(std::uint8_t value,std::string& e)const{
 return model_renderer::source_store_menu_cut_screen_v62(value,e);
}
}
