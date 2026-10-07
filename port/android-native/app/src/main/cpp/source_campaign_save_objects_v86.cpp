#include "source_campaign_save_objects_v86.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_script_actor_v96.hpp"
#include "source_campaign_anchor_v75.hpp"
#include "source_campaign_fx_v77.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "canonical_character_save_v86.hpp"
#include "canonical_gameobject_base_owner_v1.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_save_files_owner_v61.hpp"
#include "level_savegame_writer_v2.hpp"
#include "level_validate_checkpoint_v115.hpp"
#include <cstring>
namespace model_renderer {
namespace {bool needed(const char* leaf,std::string& e){if(e.empty())e=std::string("Required actual OBJS ")+leaf;return false;}}
class SourceCampaignSaveObjectsV86 final:public std::enable_shared_from_this<SourceCampaignSaveObjectsV86> {
 std::weak_ptr<SourceWorldBorrowV61> world_;
 dh2::level::LevelSavegameApplicationV1 prior_;
 std::shared_ptr<void> noncharacter_owner_;
 SourceNonCharacterSaveBorrowV86 noncharacter_;
 bool current(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world_.lock();if(!w||!w->owner||!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=w->owner||c.application!=w->application||!c.objects)return needed("SAME current canonical World/App/ObjectManager",e);
  return true;
 }
 static std::int32_t key(std::uintptr_t value){const auto bits=static_cast<std::uint32_t>(value);std::int32_t result;std::memcpy(&result,&bits,4);return result;}
 bool object(const dh2::world::CanonicalObjectBorrowV1* object,dh2::level::LevelSaveObjectBorrowV2& out,std::string& e){
  out={};if(!object){e.clear();return true;} //actual null source map node
  if(!object->identity||!object->lease)return needed("published canonical object lease",e);
  std::uintptr_t character{};
  if(!object->as_character||!object->as_character(object->context,character,e))return false;
  if(character)return character_object(character,out,e);
  if(!noncharacter_owner_||!noncharacter_){
   //Eligibility is inspected BEFORE the source invokes Save/Load. Lend the
   //real base fields now; require the derived serializer only at its reached
   //virtual call, so a C1 checkpoint28=0 object does not demand an unused body.
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!current(c,w,e)||!borrow_source_campaign_object_base_v77(c,object->identity,pin,base,e)||!base)return false;
   struct Dispatch {std::weak_ptr<SourceCampaignSaveObjectsV86> owner;std::shared_ptr<void> receiver;std::uintptr_t identity{};};
   auto dispatch=std::make_shared<Dispatch>(Dispatch{shared_from_this(),std::move(pin),object->identity});
   out.identity=reinterpret_cast<const void*>(object->identity);out.context=dispatch.get();
   out.checkpoint28=base->byte(0x28);out.gametype48=base->string(0x48);out.map_name=base->string(0x30);
   out.room64=object->room64;out.disabled81=base->byte(0x81);out.receiver_lease_v86=dispatch;
   out.is_character=[](void*,bool& value,std::string& e){value=false;e.clear();return true;}; //actual AsCharacter above returnedNULL
   out.is_player=[](void*,bool&,std::string& e){return needed("source nonCharacter IsPlayer unsafe call",e);};
   out.save=[](void* raw,dh2::level::SavegameStreamV2& stream,std::string& e){auto& d=*static_cast<Dispatch*>(raw);auto owner=d.owner.lock();dh2::level::LevelSaveObjectBorrowV2 actual;
    if(!owner||!owner->noncharacter_owner_||!owner->noncharacter_||!owner->noncharacter_(d.identity,actual,e)||actual.identity!=reinterpret_cast<const void*>(d.identity)||!actual.save)return needed("reached actual nonCharacter Save virtual10",e);
    return actual.save(actual.context,stream,e);};
   out.load=[](void* raw,dh2::level::SavegameStreamV2& stream,std::string& e){auto& d=*static_cast<Dispatch*>(raw);auto owner=d.owner.lock();dh2::level::LevelSaveObjectBorrowV2 actual;
    if(!owner||!owner->noncharacter_owner_||!owner->noncharacter_||!owner->noncharacter_(d.identity,actual,e)||actual.identity!=reinterpret_cast<const void*>(d.identity)||!actual.load)return needed("reached actual nonCharacter Load virtual14",e);
    return actual.load(actual.context,stream,e);};
   e.clear();return true;
  }
  if(!noncharacter_(object->identity,out,e))return false;
  if(out.identity!=reinterpret_cast<const void*>(object->identity)||!out.save||!out.load)return needed("same nonCharacter serializer receiver",e);
  if(!out.receiver_lease_v86)out.receiver_lease_v86=object->lease;e.clear();return true;
 }
 dh2::level::LevelSaveObjectsServicesV2 services(){
  dh2::level::LevelSaveObjectsServicesV2 s;s.context=this;
  s.first=[](void* p,std::uintptr_t& it,auto& out,bool& found,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   if(!self.current(c,w,e))return false;std::int32_t k{};const dh2::world::CanonicalObjectBorrowV1* value{};
   found=c.objects->source_ordered_begin_v38(k,value);if(!found){out={};e.clear();return true;}it=static_cast<std::uint32_t>(k);return self.object(value,out,e);};
  s.next=[](void* p,std::uintptr_t& it,auto& out,bool& found,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   if(!self.current(c,w,e))return false;std::int32_t k{};const dh2::world::CanonicalObjectBorrowV1* value{};
   found=c.objects->source_ordered_next_v38(key(it),k,value);if(!found){out={};e.clear();return true;}it=static_cast<std::uint32_t>(k);return self.object(value,out,e);};
  s.network_online=[](void* p,bool& online,std::string& e){return static_cast<SourceCampaignSaveObjectsV86*>(p)->online(online,e);};
  s.locally_controlled=[](void* p,const void* object,bool& value,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);auto w=self.world_.lock();auto pm=w&&w->application?w->application->source_player_manager_v59():nullptr;
   return pm?pm->source_is_local_player_v61(reinterpret_cast<std::uintptr_t>(object),value,e):needed("same PM.IsLocalPlayer",e);};
  s.by_name=[](void* p,const char* name,std::int32_t room,bool all,const char* filter,auto& out,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   if(!self.current(c,w,e))return false;dh2::target_providers::Handle16 handle{};const dh2::world::CanonicalObjectBorrowV1* found{};
   if(!c.objects->by_name(name,room,all,filter,handle,e)||!c.objects->resolve_handle_v4(handle,false,found,{},e))return false;
   return self.object(found,out,e);};
  s.player=[](void* p,std::int32_t index,bool active,auto& out,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);auto w=self.world_.lock();auto pm=w&&w->application?w->application->source_player_manager_v59():nullptr;
   dh2::player::PlayerInfoFieldsV1* info{};if(!pm||!pm->manager()||!pm->manager()->get_player(index,active,info,e)||!info)return needed("PM.GetPlayer actual saved player",e);
   if(!info->character660){out={};e.clear();return true;}return self.character_object(info->character660,out,e);};
  return s;
 }
 bool online(bool& value,std::string& e){auto w=world_.lock();auto source=w&&w->application?w->application->get_online_loading_v55():nullptr;
  if(!source)return needed("same GetOnline byte5",e);value=source->byte5()!=0;e.clear();return true;}
public:
 SourceCampaignSaveObjectsV86(const std::shared_ptr<SourceWorldBorrowV61>& w,dh2::level::LevelSavegameApplicationV1 prior):world_(w),prior_(std::move(prior)){}
 bool bind_noncharacter(std::shared_ptr<void> owner,SourceNonCharacterSaveBorrowV86 borrow,std::string& e){
  if(!owner||!borrow||noncharacter_owner_||noncharacter_)return needed("once-only genuine nonCharacter save journal",e);
  noncharacter_owner_=std::move(owner);noncharacter_=std::move(borrow);e.clear();return true;
 }
 bool character_object(std::uintptr_t id,dh2::level::LevelSaveObjectBorrowV2& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(c,w,e))return false;
  SourceCampaignCharacterBorrowV62 borrowed;if(!borrow_source_campaign_character_v62(c.actual_world,id,borrowed,e)||!borrowed.character)return false;
  auto r=borrowed.character;dh2::world::GameObjectInitializationFieldsV62 f;
  if(!r->actor->inherited_initialization_fields_v62(r,f,e))return false;
  auto canonical=r->actor->canonical(r);
  out={};out.identity=reinterpret_cast<const void*>(id);out.context=r.get();
  out.checkpoint28=f.byte(0x28);out.gametype48=r->actor->source_string(0x48);
  out.map_name=&r->actor->source_name();out.room64=canonical.room64;out.disabled81=f.byte(0x81);out.receiver_lease_v86=r;
  out.is_character=[](void*,bool& yes,std::string& error){yes=true;error.clear();return true;};
  out.is_player=[](void* p,bool& yes,std::string& error){return static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(p)->is_player(yes,error);};
  if(!r->save_connection_v86){
   dh2::character::CanonicalCharacterSaveServicesV86 s;s.provider=shared_from_this();auto weak=std::weak_ptr<SourceCampaignSaveObjectsV86>(shared_from_this());
   s.current_level_byte=[weak](std::uint32_t offset,bool& value,std::string& e){auto self=weak.lock();if(!self)return needed("restore native provider lifetime",e);
    dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,e))return false;
    if(!level){value=false;e.clear();return true;}SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
    if(!self->current(c,w,e)||level.level()!=c.level)return needed("restore current Level ownership",e);
    const auto& fields=level.level()->constructor_fields_v3();if(offset==0xf2)value=fields.byte_f2!=0;else if(offset==0xf3)value=fields.byte_f3!=0;else return needed("source Level flag offset",e);return true;};
   s.sync_visibility=[weak](std::uintptr_t id,std::string& e){auto self=weak.lock();auto w=self?self->world_.lock():nullptr;SourceCampaignCharacterBorrowV62 actor;
    if(!w||!borrow_source_campaign_character_v62(w->owner,id,actor,e))return false;auto* visible=actor.character->actor->source_bool_field(0x80);
    if(!visible)return needed("restored source visible80",e);return source_campaign_character_sync_visibility_v86(w->owner,id,e);};
   s.set_visible=[weak](std::uintptr_t id,bool visible,std::string& e){auto self=weak.lock();auto w=self?self->world_.lock():nullptr;return w&&source_campaign_character_set_visible_v96(w->owner,id,visible,e);};
   s.update_anchor=[weak](std::uintptr_t anchor,std::string& e){auto self=weak.lock();auto w=self?self->world_.lock():nullptr;return w&&source_campaign_anchor_update_v75(w->owner,anchor,e);};
   s.effects={this,[](void* p,dh2::data::PropertyView*,const dh2::character::BuffRequest32* q,std::uintptr_t* out){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);auto w=self.world_.lock();
    if(!w||!q||!out)return 0;if(q->service==dh2::character::buff_fx_release&&!q->subject)return 1;
    std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};std::string e;
    if(!borrow_source_campaign_fx_v77(w->owner,pin,manager,e)||!manager)return 0;
    switch(q->service){
     case dh2::character::buff_fx_load:return manager->grab_marker_v28(q->id,q->character,*out,e)?1:0;
     case dh2::character::buff_fx_release:{auto actual=q->subject;return manager->drop(actual,e)?1:0;}
     case dh2::character::buff_fx_object:return manager->buff_anim_controller_v87(q->subject,*out,e)?1:0;
     case dh2::character::buff_fx_enable:return manager->buff_play_clip_v87(q->subject,q->index,e)?1:0;
     default:return 0;
    }
   }};
   r->save_connection_v86=std::make_unique<dh2::character::CanonicalCharacterSaveV86>(r,std::move(s));
  }
  return r->save_connection_v86->bind(out,e);
 }
 bool load(dh2::data::Bytes bytes,std::uint64_t offset,dh2::level::LevelSavegameFieldsV1& fields,std::string& e){
  if(fields.initializing38){e.clear();return true;} //original __LoadObjects first guard
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!current(c,w,e))return false;
  if(fields.level8!=reinterpret_cast<const void*>(c.level->identity())||offset>bytes.size||(bytes.size&&!bytes.data))return needed("same Level/cache OBJS stream offset",e);
  dh2::level::SavegameStreamV2 stream(bytes);stream.seek(offset);
  return dh2::level::level_savegame_load_objects_v2(stream,fields,services(),e);
 }
 bool validate_checkpoint(dh2::level::LevelSavegameCacheV1& cache,
  dh2::level::LevelSavegameFieldsV1& fields,std::uint32_t seed,std::int32_t difficulty,
  std::int32_t row,bool& valid,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!current(c,w,e)||!c.level||fields.level8!=reinterpret_cast<const void*>(c.level->identity()))return needed("same Level checkpoint validation receiver",e);
  const auto actual=c.level->constructor_fields_v3().save_ec;
  auto runtime=actual?std::static_pointer_cast<dh2::level::LevelSavegameRuntimeV1>(actual):nullptr;
  if(!runtime||runtime->cache().get()!=&cache||&runtime->owner().fields()!=&fields)return needed("same LevelEC cache/INFO checkpoint receiver",e);
  auto files=w->application->source_save_files_v61();
  if(!files||!files->belongs_to_application(w->application)||!files->files())return needed("same Application FileManager checkpoint existence",e);
  dh2::level::LevelCheckpointValidationServicesV115 services;
  services.online=[this](bool& value,std::string& error){return online(value,error);};
  services.hosting=[this](bool& value,std::string& error){return prior_.is_hosting?prior_.is_hosting(prior_.context,value,error):needed("positive original PM.IsHosting",error);};
  services.manager719=[this](std::uint8_t& value,std::string& error){auto current_world=world_.lock();auto pm=current_world&&current_world->application?current_world->application->source_player_manager_v59():nullptr;
   auto* manager=pm?pm->manager():nullptr;auto* current_fields=manager?manager->source_frame_fields_v68():nullptr;
   if(!current_fields)return needed("same produced PM719 checkpoint validation",error);value=current_fields->byte719;return true;};
  services.exists=[files](const std::string& filename,bool& found,std::string& error){return files->files()->source_exists_v115(filename,found,error);};
  return dh2::level::level_validate_checkpoint_v115(cache,fields,seed,difficulty,row,services,valid,e);
 }
 void bind_application(dh2::level::LevelSavegameApplicationV1& app){
  app.context=this;app.files.context=this;app.files.storage_lease=shared_from_this();
  app.validate_checkpoint=[](void* p,auto& cache,auto& fields,std::uint32_t seed,std::int32_t difficulty,std::int32_t row,bool& valid,std::string& e){return static_cast<SourceCampaignSaveObjectsV86*>(p)->validate_checkpoint(cache,fields,seed,difficulty,row,valid,e);};
  app.files.load_objects_stream_v2=[](void* p,auto bytes,std::uint64_t offset,auto& fields,std::string& e){return static_cast<SourceCampaignSaveObjectsV86*>(p)->load(bytes,offset,fields,e);};
  app.network_online=[](void* p,bool& value,std::string& e){return static_cast<SourceCampaignSaveObjectsV86*>(p)->online(value,e);};
  app.is_hosting=[](void* p,bool& value,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);return self.prior_.is_hosting?self.prior_.is_hosting(self.prior_.context,value,e):needed("positive original PM.IsHosting",e);};
  app.manager_flag719=[](void* p,std::uint8_t& value,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);auto w=self.world_.lock();auto pm=w&&w->application?w->application->source_player_manager_v59():nullptr;auto* manager=pm?pm->manager():nullptr;auto* fields=manager?manager->source_frame_fields_v68():nullptr;
   if(!fields)return needed("same produced PM719",e);value=fields->byte719;return true;};
  app.save_all=[](void* p,auto& cache,auto& fields,std::string& e){auto& self=*static_cast<SourceCampaignSaveObjectsV86*>(p);SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
   if(!self.current(c,w,e)||fields.level8!=reinterpret_cast<const void*>(c.level->identity()))return needed("same Level saveAll receiver",e);
   auto files=w->application->source_save_files_v61();if(!files||!files->belongs_to_application(w->application)||!files->jobs())return needed("same Application FileManager/jobs queue",e);
   return dh2::level::level_savegame_save_all_v2(cache,fields,self.services(),*files->jobs(),e);};
 }
};
bool bind_source_campaign_save_application_v86(const std::shared_ptr<SourceWorldBorrowV61>& world,dh2::level::LevelSavegameApplicationV1& app,std::string& e){
 if(!world||!world->owner||!world->application||!world->canonical_world||world->save_objects_v86)return needed("fresh SAME World save-directory publication",e);
 auto owner=std::make_shared<SourceCampaignSaveObjectsV86>(world,app);world->save_objects_v86=owner;owner->bind_application(app);e.clear();return true;
}
bool borrow_source_campaign_character_saved_object_v86(const std::shared_ptr<void>& world,std::uintptr_t id,dh2::level::LevelSaveObjectBorrowV2& out,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,actual,e)||!actual->save_objects_v86)return needed("current canonical save directory",e);
 return actual->save_objects_v86->character_object(id,out,e);
}
bool bind_source_campaign_noncharacter_saved_objects_v86(const std::shared_ptr<void>& world,std::shared_ptr<void> owner,SourceNonCharacterSaveBorrowV86 borrow,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> actual;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,actual,e)||!actual->save_objects_v86)return needed("same save directory before class serializer enrollment",e);
 return actual->save_objects_v86->bind_noncharacter(std::move(owner),std::move(borrow),e);
}
bool borrow_source_campaign_character_buffs_v86(const std::shared_ptr<void>& world,std::uintptr_t id,dh2::character::BuffOwner*& out,std::string& e){
 dh2::level::LevelSaveObjectBorrowV2 object;if(!borrow_source_campaign_character_saved_object_v86(world,id,object,e))return false;
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,id,actual,e)||!actual.character->save_connection_v86)return needed("SAME canonical buff/serializer owner",e);
 return actual.character->save_connection_v86->borrow_buffs(out,e);
}
}
