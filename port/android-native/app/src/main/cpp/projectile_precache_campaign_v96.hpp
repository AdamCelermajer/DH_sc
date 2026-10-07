#pragma once
#include "projectile_precache_process_table_v96.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_spawn_groups_v108.hpp"
#include "model_renderer.hpp"
#include <canonical_level_context_v1.hpp>
#include <application_services_owner_v5.hpp>
namespace model_renderer {
//Called only by existing Stage29 projectile_sources, AFTER Item145 PreCache.
//The World slot contains Main's independent pool/Create/DeSpawn authority.
//All table fields and FX delivery are supplied here from existing process/App owners.
inline bool lend_source_campaign_projectile_precache_v96(
 std::weak_ptr<SourceWorldBorrowV61> weak_world,
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> weak_level,
 dh2::loader::ProjectilePrecacheSourcesV96& out,std::string& e){
 const auto world=weak_world.lock();const auto level=weak_level.lock();
 SourceCampaignCandidateBorrowV55 current;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!world||!level||!borrow_source_campaign_candidate_runtime_v61(current,e)||
    !same(current.actual_world,world->owner)||!same(current.level,level)||
    !same(current.application,world->application)||level->constructor_fields_v3().field130!=29){
  if(e.empty())e="Required SAME current Stage29 projectile source scope";return false;
 }
 const auto primitive=world->projectile_precache_v96;
 if(!primitive||!primitive->owner||!primitive->validate_current||!primitive->borrow_manager||
    primitive->owner.get()==world.get()||primitive->owner.get()==level.get()||
    primitive->owner.get()==world->application.get()||primitive->owner.get()==world->owner.get()||
    (!primitive->owner.owner_before(world)&&!world.owner_before(primitive->owner))||
    (!primitive->owner.owner_before(level)&&!level.owner_before(primitive->owner))||
    (!primitive->owner.owner_before(world->application)&&!world->application.owner_before(primitive->owner))||
    (!primitive->owner.owner_before(world->owner)&&!world->owner.owner_before(primitive->owner))){
  e="Required independent SAME Main projectile primitive authority";return false;
 }
 if(primitive->table_owner||primitive->table_count||primitive->table_fx_set14||primitive->register_fx_set){
  e="Main projectile input must contain only actual pool/Create/DeSpawn leaves";return false;
 }
 auto fx=world->application->source_fx_libraries_v63();
 if(!fx||!fx->belongs_to(world->application)){e="Required SAME Application projectile FX queue";return false;}
 std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;
 //This public borrower calls OriginalUiSession.process_arrays_borrow_v101.
 //It loads/parses nothing and retains the SAME immutable process arrays.
 if(!borrow_process_spawn_arrays_v108(arrays,e))return false;
 auto delivery=*primitive;
 if(!bind_actual_projectile_fx_rows_v96(arrays,fx,delivery,e))return false;
 const auto weak_primitive=std::weak_ptr<dh2::loader::ProjectilePrecacheSourcesV96>(primitive);
 const auto weak_app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(world->application);
 const auto weak_fx=std::weak_ptr<dh2::fx::VisualFxManagerLibrariesV63>(fx);
 auto scope=[weak_world,weak_level,weak_primitive,weak_app,weak_fx](std::string& error){
  const auto w=weak_world.lock();const auto l=weak_level.lock();const auto p=weak_primitive.lock();
  const auto app=weak_app.lock();const auto libraries=weak_fx.lock();SourceCampaignCandidateBorrowV55 actual;
  const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
  if(!w||!l||!p||!app||!libraries||!borrow_source_campaign_candidate_runtime_v61(actual,error)||
     !same(actual.actual_world,w->owner)||!same(actual.level,l)||!same(actual.application,app)||
     !same(w->application,app)||!same(w->projectile_precache_v96,p)||
     !same(app->source_fx_libraries_v63(),libraries)||!libraries->belongs_to(app)||
     l->constructor_fields_v3().field130!=29){
   if(error.empty())error="Projectile precache current Level/App/FX authority changed or cancelled";return false;
  }
  return true;
 };
 auto native=delivery.validate_current;
 delivery.validate_current=[scope,native=std::move(native)](std::string& error){
  return scope(error)&&native(error)&&scope(error);
 };
 //Recheck after the process loan; publish no half-filled packet on failure.
 if(!scope(e))return false;
 out=std::move(delivery);e.clear();return true;
}
}
