#include "source_campaign_character_cache_v81.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "character_candidate_cache_v62.hpp"
#include "character_oid_cache_v81.hpp"
#include "character_script_session.hpp"
#include "scene_preload_owner_v81.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include "native_resource_budget_v38.hpp"
namespace model_renderer {
bool prepare_source_campaign_character_preload_v81(const SourceCampaignCandidateBorrowV55& candidate,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!candidate.roots||!world->read_admitted_v81||!world->files_owner){if(e.empty())e="Required SAME SceneManager/Files preloaded-root producers";return false;}
 if(world->character_preload_v81){e="Actual campaign Scene preload services already prepared";return false;}
 dh2::world::ScenePreloadAssetsV81 assets;assets.provider=world->files_owner;assets.budget=dh2::android_resources::budget_lease_v39();
 assets.read=[weak=std::weak_ptr<SourceWorldBorrowV61>(world)](const auto& file,bool& found,auto& bytes,const auto& before,auto& e){
  auto actual=weak.lock();SourceCampaignCandidateBorrowV55 current;
  if(!actual||!actual->read_admitted_v81||!borrow_source_campaign_candidate_runtime_v61(current,e)||current.actual_world!=actual->owner){if(e.empty())e="Released/replaced actual Scene preload filesystem owner";return false;}
  return actual->read_admitted_v81(file,found,bytes,before,e);
 };
 auto collection=std::make_shared<dh2::world::ScenePreloadCollectionV81>(std::move(assets));
 if(!candidate.roots->source_preload_collection_v81(collection,e))return false;
 dh2::character::CharacterOidPreloadServicesV81 services;services.provider=collection;
 services.preload_scene=[weak=std::weak_ptr<dh2::world::ScenePreloadCollectionV81>(collection)](const auto& file,auto& e){
  auto actual=weak.lock();if(!actual){e="Actual SceneManager preloaded-root collection expired";return false;}return actual->preload(file,e);
 };
 return bind_source_campaign_character_preload_v81(candidate,std::move(services),e);
}
bool bind_source_campaign_character_preload_v81(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::character::CharacterOidPreloadServicesV81 services,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 if(!services.provider||!services.preload_scene||world->character_preload_v81){e="Require one genuine SAME SceneManager preloaded-root provider";return false;}
 world->character_preload_v81=std::make_shared<dh2::character::CharacterOidPreloadServicesV81>(std::move(services));e.clear();return true;
}
bool bind_source_campaign_register_summon_v81(const std::shared_ptr<void>& identity,
 dh2::character::CharacterScriptSessionInput& input,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!identity||!borrow_source_campaign_candidate_runtime_v61(candidate,e)||candidate.actual_world!=identity||
    !borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->design)return false;
 std::shared_ptr<const dh2::character::CharacterCandidateCacheV62> tables;
 if(!borrow_source_campaign_character_cache_v81(identity,tables,e))return false;
 auto counts=dh2::character::character_oid_cache_process_v81();
 input.summon_cache_v81=counts;
 input.register_summon_v81=[weak=std::weak_ptr<SourceWorldBorrowV61>(world),tables,counts](std::int32_t id,std::uint32_t count,std::string& e){
  auto world=weak.lock();SourceCampaignCandidateBorrowV55 current;
  if(!world||!world->design||!borrow_source_campaign_candidate_runtime_v61(current,e)||current.actual_world!=world->owner||current.application!=world->application){if(e.empty())e="Released/replaced actual RegisterSummon World/Application";return false;}
  auto design=world->design->borrow();const auto* characters=design.characters();
  if(!characters){e="Required actual CharacterProperties table for source AddCharOIDToCache";return false;}
  const dh2::character::CharacterOidPreloadServicesV81 unbound;
  return counts->add(id,count,*characters,tables->models(),world->character_preload_v81?*world->character_preload_v81:unbound,e);
 };
 e.clear();return true;
}
void clear_source_character_oid_cache_at_level_d1_v81()noexcept{
 //Storage has already been constructed by the process global/native borrower.
 //No scene/resource teardown or cache re-creation is hidden in this operation.
 dh2::character::character_oid_cache_process_v81()->clear_at_level_d1();
}
}
