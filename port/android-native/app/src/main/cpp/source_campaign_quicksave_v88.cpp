#include "source_campaign_quicksave_v88.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_services_owner_v5.hpp"
#include "native_gslevel_runtime_v27.hpp"
#include "level_constructor_bindings_v4.hpp"
namespace model_renderer {namespace {
bool needed(std::string& e){if(e.empty())e="Required SAME current campaign QuickSave/Level_ec/PM backing";return false;}
struct QuickSaveTransportV88 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> players;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e)const{
  out=world.lock();auto expected=level.lock();SourceCampaignCandidateBorrowV55 c;
  return out&&expected&&borrow_source_campaign_candidate_runtime_v61(c,e)&&c.actual_world==out->owner&&c.level==expected?true:needed(e);
 }
 bool save(const std::shared_ptr<void>& receiver,std::shared_ptr<dh2::level::LevelSavegameRuntimeV1>& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> w;if(!current(w,e)||!receiver||!w->native_level_c1_v25||!*w->native_level_c1_v25)return needed(e);
  auto connection=*w->native_level_c1_v25;auto bindings=connection->bindings();out=bindings?bindings->save():nullptr;
  auto expected=level.lock();const auto& field=expected->constructor_fields_v3().save_ec;
  if(!out||connection->candidate()!=expected||field.get()!=out.get()||receiver.get()!=out.get()||field.owner_before(out)||out.owner_before(field)||receiver.owner_before(out)||out.owner_before(receiver))return needed(e);
  e.clear();return true;
 }
};
}
bool borrow_source_campaign_quicksave_v88(const SourceCampaignCandidateBorrowV55& c,
 std::shared_ptr<dh2::player::PlayerLevelQuickSaveV29>& out,std::string& e,
 std::function<bool(bool&,std::string&)> hosting){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!c.level||!borrow_source_campaign_condition_world_v70(c,world,e)||!world->player_manager||!world->player_manager->manager())return needed(e);
 auto* fields=world->player_manager->manager()->source_frame_fields_v68();if(!fields)return needed(e);
 auto transport=std::make_shared<QuickSaveTransportV88>();transport->world=world;transport->level=c.level;transport->players=world->player_manager;
 dh2::player::PlayerQuickSaveServicesV29 s;s.owner=transport;s.players=world->player_manager->manager();s.player_manager719=&fields->byte719;
 s.local_hosting=std::move(hosting); //original offline branch never reaches it
 s.online_byte5=[transport](std::uint8_t& value,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;if(!transport->current(world,e))return false;
  auto online=world->application->get_online_loading_v55();if(!online)return needed(e);value=online->byte5();e.clear();return true;};
 s.character=[transport](std::uintptr_t id,dh2::player::PlayerQuickSaveCharacterV29& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;if(!transport->current(world,e))return false;
  SourceCampaignCharacterBorrowV62 borrowed;if(!borrow_source_campaign_character_v62(world->owner,id,borrowed,e)||!borrowed.character||!borrowed.character->actor)return false;
  auto r=borrowed.character;auto* checkpoint=r->actor->checkpoint1468_v83();if(!r->life||!checkpoint)return needed(e);
  out={r,id,&r->life->dead,r->actor->source_position160_v7(),checkpoint};e.clear();return true;};
 s.save_flag39=[transport](const auto& receiver,std::uint8_t*& flag,std::string& e){std::shared_ptr<dh2::level::LevelSavegameRuntimeV1> save;
  if(!transport->save(receiver,save,e))return false;flag=&save->owner().fields().inhibit_save39;e.clear();return true;};
 s.save_ec=[transport](const auto& receiver,std::string& e){std::shared_ptr<dh2::level::LevelSavegameRuntimeV1> save;
  return transport->save(receiver,save,e)&&save->owner().save(e);};
 out=std::make_shared<dh2::player::PlayerLevelQuickSaveV29>(std::move(s));e.clear();return true;
}
}
