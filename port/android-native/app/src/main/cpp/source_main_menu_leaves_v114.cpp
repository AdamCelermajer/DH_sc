#include "source_main_menu_leaves_v114.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_quicksave_v88.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "application_services_owner_v5.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "player_save_write_owner_v1.hpp"
#include <ctime>
namespace model_renderer {namespace {
bool required(std::string& e){if(e.empty())e="Required SAME native main-menu Level/Save/PM backing";return false;}
bool same(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
bool current(const dh2::loader::MainMenuLevelBorrowV114& actual,SourceCampaignCandidateBorrowV55& c,
 std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e){
 dh2::loader::CanonicalCurrentLevelBorrowV1 level;
 if(!borrow_current_native_level_v27(level,e)||!level||level.identity()!=actual.identity||!same(actual.owner,level.level())||
    !borrow_source_campaign_candidate_runtime_v61(c,e)||!same(c.level,level.level())||!borrow_source_campaign_condition_world_v70(c,w,e))return required(e);
 auto native=level.level()->constructor_borrow_v3();if(!native.owner||!native.fields||native.identity!=actual.identity||
    actual.state130!=&native.fields->field130||actual.dungeon198!=&native.fields->byte198||actual.saving145!=&native.fields->byte145)return required(e);
 return true;
}
}
bool source_main_menu_level_v114(dh2::loader::MainMenuLevelBorrowV114& out,std::string& e){
 dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
 dh2::loader::MainMenuLevelBorrowV114 actual;
 if(current){auto native=current.level()->constructor_borrow_v3();if(!native.owner||!native.fields||native.identity!=current.identity())return required(e);
  actual={current.level(),current.identity(),&native.fields->field130,&native.fields->byte198,&native.fields->byte145};}
 out=std::move(actual);e.clear();return true;
}
bool source_main_menu_quick_save_v114(const dh2::loader::MainMenuLevelBorrowV114& actual,bool disable,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(actual,c,w,e))return false;
 std::shared_ptr<dh2::player::PlayerLevelQuickSaveV29> quick;
 //The existing whole QuickSave owns actual GetLocalPlayer/checkpoint/Save_ec.
 //Positive online hosting stays a required genuine provider, never offline by default.
 if(!borrow_source_campaign_quicksave_v88(c,quick,e)||!quick)return required(e);
 const auto& fields=c.level->constructor_fields_v3();
 return quick->execute({c.level,actual.identity,&fields.save_ec,&fields.field130},disable,e);
}
bool source_main_menu_save_local_v114(const dh2::loader::MainMenuLevelBorrowV114& actual,bool disable,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(actual,c,w,e))return false;
 auto pm=w->application?w->application->source_player_manager_v59():nullptr;
 if(!pm||!pm->belongs_to_application(w->application)||!pm->manager())return required(e);
 //Original SG_SaveLocalPlayer3f07b0 is precisely GetLocalPlayer(0,true),
 //then tail-dispatch SG_SavePlayer3efa54 on its current Character660.
 dh2::player::PlayerInfoFieldsV1* info{};if(!pm->manager()->get_local_player(0,true,info,e)||!info)return required(e);
 const auto id=info->character660;if(!id){e.clear();return true;}
 SourceCampaignCharacterBorrowV62 loan;if(!borrow_source_campaign_character_v62(w->owner,id,loan,e)||!loan.character)return false;
 auto r=loan.character;bool player{};if(!r->is_player(player,e))return false;if(!player){e.clear();return true;}
 if(!r->save_fields||!r->save_fields->save_slot14e8())return required(e);
 if(!*r->save_fields->save_slot14e8()){e.clear();return true;} //Real SG methods' NULL14e8 guard.
 if(!r->save||!r->load||*r->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r->save.get())||
    &r->load->save()!=r->save.get()||!r->properties)return required(e);
 const bool previous=r->load->save_disabled();if(disable)r->load->source_block_store_v88(false);
 //Same proven SG_SavePlayer ordering/resources used by existing V88 SaveAll.
 r->save->set_player_level(r->properties->resolved[19]>>8);
 r->save->source_save_date_store_v88(static_cast<std::uint32_t>(std::time(nullptr)));
 if(!source_campaign_character_save_entrypoint_v68(w->owner,id,c.level->constructor_fields_v3().level110,-1,e))return false;
 dh2::data::PlayerSaveWriteServicesV1 write;
 if(r->load->profile().identity){auto actual_writer=r->profile_bootstrap?r->profile_bootstrap->campaign_writer():nullptr;
  if(!actual_writer||!actual_writer->ready())return required(e);write=actual_writer->write_services();}
 dh2::data::PlayerSaveWriteOwnerV1 writer(r->load,std::move(write));
 if(!writer.save(e))return false; //Preserve actual block/date/entry/file-job prefix.
 r->load->source_block_store_v88(previous);e.clear();return true;
}
bool source_main_menu_remove_players_v114(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 auto pm=app?app->source_player_manager_v59():nullptr;
 if(!pm||!pm->belongs_to_application(app)||!pm->manager())return required(e);
 return pm->manager()->remove_all_players_v114(e);
}
}
