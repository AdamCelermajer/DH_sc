#include "canonical_character_candidate_v60.hpp"
#include "character_ai_groups_v87.hpp"
#include "canonical_character_spawn_select_v87.hpp"
#include "canonical_character_save_v86.hpp"
#include "character_props_id_owner_v1.hpp"
#include "game_object_spawn_probability_v1.hpp"
#include "character_properties_temp_global_v62.hpp"
#include "character_world_npc_init_final_v1.hpp"
#include "player_initial_grants_v2.hpp"
#include "character_revive_owner_v1.hpp"
#include "character_script_init_vitals.hpp"
#include "character_script_source_virtuals_v101.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::world {namespace {
class CandidateActorV60 final:public character::RetainedCharacterActorV1 {
public:
 CandidateActorV60(std::shared_ptr<void> world,const char* catalog):RetainedCharacterActorV1(
  reinterpret_cast<std::uintptr_t>(this),std::move(world),catalog,character::RetainedCharacterConstructionV7::fresh_canonical){}
};
const data::AiProps* actual_ai(CanonicalCharacterCandidateRecordV60& record){
 return record.design.ai()?data::ai_props(*record.design.ai(),record.properties->resolved[1]):nullptr;
}
}
bool CanonicalCharacterCandidateFactoryV60::construct(const CanonicalFactoryEntryV1& entry,
 const CanonicalSourceObjectRequestV1& source,CanonicalClassReceiverV1& out,std::string& e){
 if(!entry.name||(std::strcmp(entry.name,"Character")&&std::strcmp(entry.name,"Player"))||entry.original_address!=0x340800){
  e="Required exact Character/Player catalog alias factory340800";return false;
 }
 if(!services_.world||!services_.design||!services_.design->ready()||!services_.loot_tables||
    !services_.loot_tables->borrow()||!services_.random||!source.source_lease||(!source.attribute&&!source.native_spawn_name_v68)){
  e="Required actual source Character C1/immutable tables/Random/XML lifetime";return false;
 }
 const auto* name=source.native_spawn_name_v68?source.native_spawn_name_v68:source.attribute(source.source_context,source.element,"name");
 if(!name){e="Required actual Character source placement/spawn name";return false;}
 auto record=std::make_shared<CanonicalCharacterCandidateRecordV60>();record->services=services_;record->source=source;
 record->design=services_.design->borrow();record->properties=std::make_shared<data::PropertyState>();record->life=std::make_shared<data::CombatActorState>();
 data::reset_properties(*record->design.rules(),*record->properties);record->view=data::property_view(*record->design.rules(),*record->properties);
 record->actor=std::make_shared<CandidateActorV60>(services_.world,entry.name);
 const auto identity=record->actor->canonical(record->actor).identity;
 records_.emplace(identity,record); // Retain reached fresh C1 prefix before services.
 record->constructor_inventory=std::make_unique<data::FreshInventoryOwnedV4>(identity,services_.loot_tables->borrow(),
  *services_.random,std::int8_t(-1),record->properties);
 record->inventory37c=record->constructor_inventory.get();
 record->save_fields=std::make_shared<character::LootPlayerFieldAssociationV47>(identity); // Character C1 NULL14e8.
 record->faery_association_v68=std::make_unique<character::PlayerFaeryAssociationV8>(identity); // SAME nested CharAI C2 3cec78 storesNULL420.
 auto object=std::make_shared<character::ScriptCharacterObject>(identity,name,record->properties,record->life,std::array<float,3>{});
 character::WorldNpcStateServicesV1 state;
 if(services_.state&&!services_.state(*record,state,e)){record->failed=true;record->error=e;return false;}
 if(!record->actor->construct_fields(object,state)||!record->actor->bind_initialization_properties(record->view,e)){
  if(e.empty())e=record->actor->error();record->failed=true;record->error=e;return false;
 }
 //Character C1 already owns embedded CharAnimator. Construct its SAME
 //resource capsule now; LoadVisual/SetAnimationSet attach later without
 //replacing its dictionary registrations, clock or constructor fields.
 if(!record->bind_visual(e)){record->failed=true;record->error=e;return false;}
 record->controllable374_v70.controller378=record->actor->controller?record->actor->controller->identity():0; // SAME source C1 default controller publication
 record->target_character_v62.identity=identity;record->target_character_v62.resolved=record->properties->resolved.data();record->target_character_v62.name=object->name.c_str();
 record->target_character_v62.interactive415=1; //Actual nested CharAI C2 strb3cec6c.
 if(services_.publish_script_object&&!services_.publish_script_object(object,e)){record->failed=true;record->error=e;return false;}
 character::RetainedCharacterPositionBackendsV7 position;position.world=services_.world;
 if(services_.position&&!services_.position(*record,position,e)){record->failed=true;record->error=e;return false;}
 if(!position.visual_sync_position){std::weak_ptr<CanonicalCharacterCandidateRecordV60> weak=record;
  position.visual_sync_position=[weak](auto id,auto& error){auto r=weak.lock();
   if(!r||!r->visual){error="Required SAME initialized Character visual for SetPosition";return false;}
   return r->visual->sync_visual_position_v7(id,error);
  };
 }
 record->position=std::make_unique<character::RetainedCharacterPositionOwnerV7>(*record->actor,std::move(position));
 CanonicalClassReceiverV1 result;result.object=record->actor->canonical(record);result.source_lease=source.source_lease;
 result.source_loading_fields_v95=[record](CanonicalObjectLoadingFieldsV95& out,std::string& e){
  if(!record->actor){e="Released SAME Character loading receiver";return false;}
  GameObjectInitializationFieldsV62 fields;if(!record->actor->inherited_initialization_fields_v62(record,fields,e))return false;
  CanonicalObjectLoadingFieldsV95 value;value.receiver=record;value.archetype48=&record->actor->source_archetype();
  value.enabled8a=fields.byte(0x8a);value.minimum_ec=fields.integer(0xec);value.disabled_f1=fields.byte(0xf1);
  value.condition_a8=fields.pointer(0xa8);value.tested_ac=fields.byte(0xac);value.condition_cc=fields.pointer(0xcc);value.tested_d0=fields.byte(0xd0);
  if(!value.enabled8a||!value.minimum_ec||!value.disabled_f1||!value.condition_a8||!value.tested_ac||!value.condition_cc||!value.tested_d0){e="Unproduced SAME Character ObjectBase loading fields";return false;}
  out=std::move(value);return true;
 };
 result.source_is_updatable_v95=[record](bool& value,std::string& e){if(!record->actor){e="Released Character virtual38 receiver";return false;}value=true;return true;};
 result.source_init_final_v95=[record](std::string& e){return record->initialize_final_v70(e);};
 result.properties=[record]{return record->actor->properties();};
 result.init_post=[record](auto& error){return record->initialize(error);};
 result.is_game_object=[](bool& value,auto& error){value=true;error.clear();return true;}; // Source GameObject virtual classification.
 result.position=[record](auto& p,auto& error){return record->actor->source_position(p,error);};
 result.set_position=[record](const auto& p,bool destination,auto& error){return record->set_position(p,destination,error);};
 out=std::move(result);e.clear();return true;
}
std::shared_ptr<CanonicalCharacterCandidateRecordV60> CanonicalCharacterCandidateFactoryV60::find(std::uintptr_t id)const{
 auto at=records_.find(id);return at==records_.end()?nullptr:at->second;
}
std::shared_ptr<CanonicalCharacterCandidateRecordV60> CanonicalCharacterCandidateFactoryV60::find_hud_actor_v62(std::uintptr_t pointer)const{
 for(const auto& entry:records_)if(reinterpret_cast<std::uintptr_t>(&entry.second->hud_actor_v62)==pointer)return entry.second;return nullptr;
}
bool CanonicalCharacterCandidateFactoryV60::physical_peer_v68(void* context,std::uintptr_t& identity,std::string& e)const{
 identity=0;if(!context){e.clear();return true;} // Actual physical owner NULL.
 for(const auto& entry:records_){const auto& r=*entry.second;if(r.physical_owner_v62&&r.physical_owner_v62.get()==context){identity=r.actor->object->identity;e.clear();return true;}}
 e="Required actual non-Character PhysicalObject owner resolver";return false;
}
bool CanonicalCharacterCandidateFactoryV60::erase_after_unpublication(std::uintptr_t id,std::string& e){
 auto at=records_.find(id);if(at==records_.end()){e="Unknown Character candidate during source removal";return false;}
 if(!at->second->close_after_unpublication(e))return false;records_.erase(at);return true;
}
bool CanonicalCharacterCandidateRecordV60::is_player(bool& out,std::string& e)const{
 if(!actor||!design.ai()){e="Required SAME Character GetCharAI/name receiver";return false;}
 return character::loot_character_is_player_v47(*design.ai(),view,actor->source_name(),out,e);
}
bool CanonicalCharacterCandidateRecordV60::initialize_skill_slots_v70(std::string& e){
 if(!actor||!save||!player_script_owner_v62||!prepared_equipment_v60||prepared_equipment_v60->inventory()!=inventory37c){e="Required SAME Character Save/Gear/loaded player script for initial skill slots";return false;}
 struct Delivery {
  CanonicalCharacterCandidateRecordV60& record;std::string& error;
  static int invoke(void* context,const player::InitialGrantRequest32V2* q,player::InitialGrantResponse8V2* out){
   auto& d=*static_cast<Delivery*>(context);auto& r=d.record;auto& e=d.error;
   if(!q||!out||q->owner!=r.actor->object->identity)return -1;*out={};using namespace player;
   const auto row=static_cast<std::uint32_t>(q->arguments[0]);
   auto skill=[&]()->const data::SkillRecord*{if(row>=r.save->skills().size()||!r.services.skills)return nullptr;const auto id=r.save->skill_id(row);const auto& rows=r.services.skills.skills();return id>=0&&std::size_t(id)<rows.size()?&rows[std::size_t(id)]:nullptr;};
   switch(q->operation){
    case has_skill_slots:out->value=r.save->has_skill_slots();return 0;
    case set_skill_slot:{
     data::SavedSkillUpdateServicesV1 update{&d,[](void* raw,std::uintptr_t id,std::string& error){auto& d=*static_cast<Delivery*>(raw);
      if(id!=d.record.actor->object->identity){error="Required SAME initial slot CharAI receiver";return false;}
      if(d.record.player_script_owner_v62->native_update_mapped_skills_v70()<0){error=d.record.player_script_owner_v62->error();return false;}return true;}};
     return r.save->set_skill_in_slot(q->arguments[0],static_cast<std::uint32_t>(q->arguments[1]),update,e)?0:-1;
    }
    case swap_equipment:return r.prepared_equipment_v60->swap_inventory_for_initial_slots(e)?0:-1;
    case skill_level:case saved_level_read:if(row>=r.save->skills().size())return -1;out->value=r.save->skill_level(row);return 0;
    case increment_skill:{const InitialGrantServices16V2 services{context,invoke};return dh2_player_increment_skill_v2(&out->value,q->owner,q->arguments[0],static_cast<std::uint32_t>(q->arguments[1]),&services);}
    case has_savegame:out->value=1;return 0;
    case has_saved_rows:out->value=r.save->skills_initialized()&&!r.save->skills().empty();return 0;
    case property_integer:if(row>=r.properties->resolved.size())return -1;out->value=r.properties->resolved[row]>>8;return 0;
    case skill_available:{const auto* actual=skill();if(!actual)return -1;out->value=(r.properties->resolved[19]>>8)>=static_cast<std::int32_t>(actual->scalar.words[8]);return 0;}
    case skill_limit:{constexpr const char* keys[]{"MaxSkillLevelBNormal","MaxSkillLevelCHard","MaxSkillLevelDVeryHard"};if(row>2)return -1;return r.player_script_owner_v62->session().constant("CharacterDesign",keys[row],out->value);}
    case difficulty_unlocked:out->value=r.save->unlocked_difficulty();return 0;
    case can_increment:{const auto* actual=skill();if(!actual)return -1;const auto raw=std::uint32_t(r.properties->resolved[19]>>8)-actual->scalar.words[8];std::int32_t difference{};std::memcpy(&difference,&raw,4);out->value=r.save->skill_level(row)<=difference;return 0;}
    case property_add:{const auto raw=std::uint32_t(q->arguments[1])<<8;std::int32_t delta{};std::memcpy(&delta,&raw,4);return dh2_property_add(&r.view,q->arguments[0],delta)?-1:0;}
    case saved_level_increment:if(row>=r.save->skills().size())return -1;return r.save->set_skill_level(row,r.save->skill_level(row)+1,e)?0:-1;
    case update_all_skills:if(r.player_script_owner_v62->native_source_update_all_skills_v70()<0){e=r.player_script_owner_v62->error();return -1;}return 0;
    case properties_recalculate:return data::recalc_properties_with_class(*r.design.classes(),*r.design.rules(),*r.properties,e)?0:-1;
    case potion_capacity_store:r.prepared_equipment_v60->project_potion_capacity(static_cast<std::int8_t>(static_cast<std::uint8_t>(row)));return 0;
    case debug_load:if(!r.services.debug||!r.services.debug_files)return -1;return dh2_character_debug_load(r.services.debug,r.services.debug_files)==1?0:-1;
    case debug_query:{if(!r.services.debug||!r.services.debug_files)return -1;std::uint32_t value{};const auto code=dh2_character_debug_get(&value,r.services.debug,"isTracingChar_Stats",r.services.debug_files);out->value=static_cast<std::int32_t>(value);return code==1?0:-1;}
    default:e="Required initial skill slot source operation "+std::to_string(q->operation);return -1;
   }
  }
 }delivery{*this,e};
 const player::InitialGrantServices16V2 services{&delivery,Delivery::invoke};
 const int code=dh2_player_initial_skill_slots_v2(actor->object->identity,&services);
 if(code&&e.empty())e="Required source initial skill slot continuation";return code==0;
}
bool CanonicalCharacterCandidateRecordV60::revive_v70(std::uintptr_t target,std::uint32_t physical,std::string& e){
 if(!actor||!actor->machine||!life){e="Required SAME Character Revive graph";return false;}
 GameObjectInitializationFieldsV62 fields;if(!actor->inherited_initialization_fields_v62(shared_from_this(),fields,e))return false;
 character::CharacterReviveBorrowV1 source{actor->object->identity,life.get(),fields.byte(0x118),&actor->machine->combat_fields().network_id,actor->source_network114_v70(),actor->save_position1474_v83()};
 struct Delivery {CanonicalCharacterCandidateRecordV60& record;std::string& error;
  static int invoke(void* raw,const character::CharacterReviveRequestV1* q,std::uint32_t* word){
   auto& d=*static_cast<Delivery*>(raw);auto& r=d.record;if(!q||!word||q->subject!=r.actor->object->identity)return -1;*word=0;
   if(q->entry==0x7fd794){bool online{};if(!r.services.online_byte5||!r.services.online_byte5(online,d.error))return -1;*word=online;return 0;}
   if(q->entry==0x3a49f0){bool player{};if(!r.is_player(player,d.error))return -1;*word=player;return 0;}
   if(q->entry==0x3b4088)return r.initialize_physical(d.error)?0:-1;
   if(q->entry==0x525508){if(!q->payload||!r.services.initial_height)return -1;auto* position=reinterpret_cast<float*>(q->payload);bool found{};float height{};
    if(!r.services.initial_height(position,found,height,d.error))return -1;if(found)position[2]=height;*word=found;return 0;}
   if(q->entry==0x3d8894){
    if(r.player_script_owner_v62){if(r.player_script_owner_v62->native_source_update_all_skills_v70()<0){d.error=r.player_script_owner_v62->error();return -1;}return 0;}
    // NPC's whole source selected script skill-vector update is a different
    // existing receiver and must be delivered by its genuine session owner.
   }
   character::NpcInitPostResponseV1 out;
   if(!CanonicalCharacterCandidateRecordV60::init_service(&r,{q->entry,q->argument0,q->argument1,q->subject,q->payload,nullptr},out,d.error))return -1;
   *word=static_cast<std::uint32_t>(out.value);return 0;
  }
 }delivery{*this,e};
 const character::CharacterReviveServicesV1 callbacks{&delivery,Delivery::invoke};character::CharacterReviveResultV1 result{};
 const int status=dh2_character_revive_v1(&result,&source,target,physical,&callbacks);
 if(status!=1&&e.empty())e="Required source Character Revive continuation at "+std::to_string(result.last_entry);return status==1;
}
bool CanonicalCharacterCandidateRecordV60::initialize_final_v70(std::string& e){
 if(failed||!init_complete||!actor||!inherited_init){e="Required completed SAME InitPost before Character InitFinal";return false;}
 GameObjectInitializationFieldsV62 fields;if(!actor->inherited_initialization_fields_v62(shared_from_this(),fields,e))return false;
 auto* once=fields.byte?fields.byte(0x1395):nullptr;auto* threshold=fields.integer?fields.integer(0x274):nullptr;
 if(!once||!threshold){e="Required source Character once1395/probability274 cells";return false;}if(*once)return true;*once=1;
 auto fail=[&](){failed=true;error=e;return false;};std::int32_t spawn{};
 if(!inherited_init->check_spawn_probability(spawn,e))return fail();if(spawn>=*threshold)return true;
 bool eligible{};if(!inherited_init->init_final(eligible,e))return fail();
 const auto* ai=actual_ai(*this);if(!ai){e="Required actual Character.GetCharType InitFinal";return fail();}
 if(ai->type==3||ai->type==2){
  character::NpcInitPostRequestV1 q{0x3b49f8,0,0,actor->object->identity};character::NpcInitPostResponseV1 out;
  if(!services.remaining||!services.remaining(*this,q,out,e)){if(e.empty())e="Required source follower/faery final placement";return fail();}
 }
 if(actor->source_visual()){
  auto retained=visual?visual->visual():nullptr;if(!retained||reinterpret_cast<std::uintptr_t>(retained.get())!=actor->source_visual()){e="Required SAME retained visual for Character final default light";return fail();}
  bool player{};if(!is_player(player,e))return fail();
  retained->store_light_set(std::int32_t(character::character_light_set_id_v1(player?"SceneLight":"MonsterLight")));
 }
 // CharAI.OnInitFinal3d0ba4 reloads actual selected ScriptOwner.active.
 bool player{};if(!is_player(player,e))return fail();
 if(player){if(!player_script_owner_v62||player_script_owner_v62->native_ai_init_final_v70()<0){e=player_script_owner_v62?player_script_owner_v62->error():"Required SAME PlayerIPhone final owner";return fail();}}
 else {character::ScriptSessionView active{};if(actor->session&&actor->session->owner().active(active)&&!character::character_npc_external_init_final_v1(*actor->session,active.identity,e))return fail();}
 if(!is_player(player,e))return fail();if(!player)return true;
 bool local{};if(!services.is_local_player||!services.is_local_player(actor->object->identity,local,e)){if(e.empty())e="Required real PM.IsLocalPlayer for Character final tail";return fail();}if(!local)return true;
 if(player_script_owner_v62->update()!=1){e=player_script_owner_v62->error();return fail();}
 if(!data::recalc_properties_with_class(*design.classes(),*design.rules(),*properties,e))return fail();
 const auto capacity=std::max(0,properties->resolved[194]);const auto bits=std::uint8_t(capacity);std::int8_t signed_bits;std::memcpy(&signed_bits,&bits,1);
 if(!equipment||!equipment->ready()){e="Required SAME Gear for source Character3a8 final capacity";return fail();}equipment->project_potion_capacity(signed_bits);
 if(services.player_save_final_v122)return services.player_save_final_v122(*this,e);
 const auto writer=profile_bootstrap?profile_bootstrap->campaign_writer():nullptr;
 if(!writer||!writer->ready()||!load){e="Required SAME fully registered campaign SG_Save final tail";return fail();}
 data::PlayerSaveWriteOwnerV1 save_writer(load,writer->write_services());if(!save_writer.save(e))return fail();return true;
}
bool CanonicalCharacterCandidateRecordV60::player_add_borrow_v70(player::PlayerAddCharacterBorrowV5& out,std::string& e){
 if(!actor||!actor->object||!services.world){e="Required SAME source Character C1 before player Add borrow";return false;}
 const auto canonical=actor->canonical(shared_from_this());
 if(!canonical.class_name20||!*canonical.class_name20||!canonical.shared_handle||canonical.shared_handle->cached!=actor->object->identity){e="Required actual source Spawn publication before player facet";return false;}
 if(!player_facet_v70){CanonicalPlayerFieldsV3 f;f.object=actor->object;f.runtime=&actor->runtime;f.handle=canonical.shared_handle;f.type_f4=canonical.type_f4;f.room64=canonical.room64;
  f.across_rooms87=canonical.across_rooms87;f.class_name20=canonical.class_name20;
  // Original receiver is mutable; these are the SAME CString cells. Facet
  // name/archetype methods only accept their existing value, never defaults.
  f.name=const_cast<std::string*>(&actor->source_name());f.archetype=const_cast<std::string*>(&actor->source_archetype());f.world_lease=services.world;
  player_facet_v70=std::make_unique<CanonicalPlayerFacetV3>(std::move(f));}
 out={};out.identity=actor->object->identity;out.receiver=shared_from_this();out.same_facet=player_facet_v70.get();
 out.controller1f88=&player_controller1f88_v70;out.internal1f8c=&player_internal1f8c_v70;
 out.position160=actor->source_position160_v7();out.rotation16c=actor->runtime.rotation.rotation;
 // Online initial-position/zone branches require their separate source
 // producer borrow; offline Add never reads these optional fields.
 return true;
}
bool CanonicalCharacterCandidateRecordV60::initialize_player_save(std::string& e){
 if(save_attempted||!actor||!save_fields||*save_fields->save_slot14e8()){
  e="Source InitializePlayerSavegame cannot replay or replace Character14e8";return false;
 }
 save_attempted=true;save=std::make_shared<data::PlayerSavegameV1>();
 // Source465ae0 ->3b36d8 store->tail SG_SetCharacter3bb754, same slot/Save.
 if(!save_fields->source_fresh_save_store_then_character_v60(save,e))return false;
 load=std::make_shared<data::PlayerSaveLoadOwnerV1>(save);
 quest_sync_owner=std::make_unique<data::PlayerSaveQuestSyncOwnerV3>(*save);e.clear();return true;
}
bool CanonicalCharacterCandidateRecordV60::transfer_inventory(player::PlayerEquipmentRenderInputsV1& in,std::string& e){
 if(inventory_transferred||!constructor_inventory||inventory37c!=constructor_inventory.get()||
    in.constructed_inventory_v60||in.character!=constructor_inventory->character()||
    in.properties!=properties||in.random!=services.random){
  e="Required once-only SAME Character C1 inventory37c transfer into Gear";return false;
 }
 in.constructed_inventory_v60=std::move(constructor_inventory);inventory_transferred=true;e.clear();return true;
}
bool CanonicalCharacterCandidateRecordV60::set_position(const std::array<float,3>& p,bool destination,std::string& e){
 if(!position){e="Required SAME Character position owner";return false;}return position->set_position(p.data(),destination,e);
}
bool CanonicalCharacterCandidateRecordV60::bind_inherited_initialization(std::string& e){
 if(inherited_init)return true;
 if(!services.inherited){e="Required concrete inherited Character condition/device/visual/PF services";return false;}
 GameObjectInitializationFieldsV62 fields;
 if(!actor->inherited_initialization_fields_v62(actor,fields,e))return false;
 GameObjectInitializationServicesV1 actual;
 if(!services.inherited(*this,actual,e))return false;
 if(!actual.owner)actual.owner=services.world;
 if(!actual.condition_init){const auto condition_fields=fields;const auto condition_services=services.conditions;
  actual.condition_init=[condition_fields,condition_services](std::uint32_t offset,std::string& error){return condition_data_init_borrow_v62(condition_fields,offset,condition_services,error);};
 }
 std::weak_ptr<CanonicalCharacterCandidateRecordV60> weak=shared_from_this();
 if(!actual.check_spawn_probability){const auto spawn_fields=fields;
  actual.check_spawn_probability=[weak,spawn_fields](std::int32_t& roll,std::string& error){auto r=weak.lock();if(!r){error="Expired source Character spawn receiver";return false;}
   GameObjectSpawnProbabilityBorrowV1 b;b.owner=r->actor;b.cached_roll270=spawn_fields.integer(0x270);b.probability274=spawn_fields.integer(0x274);b.network_id108=spawn_fields.integer(0x108);b.byte82=r->actor->failed_spawn_byte82_v62();b.random0=r->services.random;b.random1=r->services.alternate_random;
   b.handle_as_player_character=[weak](bool& value,std::string& e){auto same=weak.lock();if(!same){e="Expired canonical Handle-AsChar-IsPlayer receiver";return false;}return same->is_player(value,e);};
   b.online_byte5=r->services.online_byte5;
   std::int32_t online_owner{};auto* slot=spawn_fields.pointer(0xfc);
   if(slot){if(*slot>UINT32_MAX){error="Source online ownerfc exceeds wire-word domain";return false;}const auto word=std::uint32_t(*slot);std::memcpy(&online_owner,&word,4);b.online_owner_fc=&online_owner;}
   b.set_visible_false=[weak](std::string& e){auto same=weak.lock();if(!same||!same->services.failed_spawn_visible){e="Required actual failed-spawn SetVisible(false) continuation";return false;}return same->services.failed_spawn_visible(*same,e);};
   b.object_base_delete=[weak](std::string& e){auto same=weak.lock();if(!same){e="Expired source ObjectBase.Delete receiver";return false;}same->actor->source_object_base_delete_v62();return true;};
   b.mark_for_deletion=[weak](std::string& e){auto same=weak.lock();if(!same||!same->services.failed_spawn_mark){e="Required actual ObjectManager mark-for-deletion continuation";return false;}return same->services.failed_spawn_mark(*same,e);};
   std::int32_t probability{};return game_object_check_spawn_probability_v1(b,roll,probability,error);
  };
 }
 actual.set_position=[weak](const float* p,bool destination,std::string& error){auto r=weak.lock();if(!r||!p){error="Expired source Character SetPosition receiver";return false;}std::array<float,3> value;std::copy_n(p,3,value.data());return r->set_position(value,destination,error);};
 actual.load_visual=[weak](std::string& error){auto r=weak.lock();if(!r){error="Expired source Character LoadVisual receiver";return false;}return r->load_visual_v77(error);};
 actual.visual_sync=[weak](std::uintptr_t id,std::string& error){auto r=weak.lock();if(!r||!r->visual){error="Required same Character visual for inherited Sync";return false;}return r->visual->sync_visual(id,error);};
 inherited_init=std::make_unique<GameObjectInitializationBorrowV62>(std::move(fields),std::move(actual));return true;
}
bool CanonicalCharacterCandidateRecordV60::initialize_physical(std::string& e){
 if(physical_owner_v62){e="Source Character InitPhysicalObject cannot replay a retained body prefix";return false;}
 if(!services.physical_world||!services.physical||!actor||!actor->object||!visual||!visual->visual()){e="Required SAME source World/Character/visual/physical dispatcher";return false;}
 if(actor->position_fields_v7().physical2dc){e="Required original replacement-body destruction before InitPhysicalObject";return false;}
 const auto* stat=actor->source_bool_field(0x84);if(!stat){e="Required source static84 for Character InitPhysicalObject";return false;}
 character::CharacterPhysicalCollisionBorrowV62 collisions;character::WorldNpcPhysicalServicesV1 actual;
 if(!services.physical(*this,collisions,actual,e))return false;
 std::weak_ptr<character::RetainedCharacterActorV1> weak_actor=actor;
 auto assignment=[weak_actor](std::uintptr_t identity,const physical::NativeBody* body,bool assigned,std::string& error){auto same=weak_actor.lock();
  if(!same||!same->position_fields_v7().constructed||!identity||!body){error="Expired actual Character SetPhysicalObject field authority";return false;}
  auto& slot=same->position_fields_v7().physical2dc;
  if(assigned){if(slot||!body->body){error="Character physical2dc assignment requires newly created same body";return false;}slot=identity;}
  else {if(slot!=identity){error="Character physical2dc changed before source detachment";return false;}slot=0;}
  return true;
 };
 physical_owner_v62=std::make_unique<character::CharacterWorldPhysicalV62>(*services.physical_world,*actor->object,view,std::move(collisions),actual,std::move(assignment));
 physical::NpcBodyRequest request;request.properties=&view;request.ai=design.ai();request.static_owner=*stat;
 auto projection=[this](const auto& q,auto& output,auto& error){return visual->body_projection_v62(q,output,error);};
 if(!physical_owner_v62->initialize_source(request,projection)){e=physical_owner_v62->error();return false;}return true;
}
bool CanonicalCharacterCandidateRecordV60::load_script(std::string& e){
 if(script_load_attempted_v62){e="Source Character LoadScriptProcess cannot replay failed/loaded prefix";return false;}script_load_attempted_v62=true;
 bool player{};if(!is_player(player,e))return false;
 if(player){
  if(!services.player_script||!actor||!actor->machine||!save||!save_fields||*save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(save.get())||!services.effect_services){e="Required actual source player Save/ScriptOwner/skill initialization inputs";return false;}
  CanonicalPlayerScriptInputsV62 inputs;if(!services.player_script(*this,inputs,e))return false;
  auto& in=inputs.session;
  if((in.savegame&&in.savegame!=save)||(in.properties&&in.properties!=properties)||(in.combat&&in.combat!=life)||(in.identity&&in.identity!=actor->object->identity)){e="Player script provider attempted foreign Save/property/life/Character";return false;}
  in.identity=actor->object->identity;in.name=actor->source_name();in.properties=properties;in.combat=life;in.savegame=save;in.position=actor->object->position;in.source_is_character=1;in.state_machine=&actor->machine->native_fsm();
  const auto& temporary=character::character_properties_temp_global_v62();
  if(in.temporary&&in.temporary!=temporary){e="Player script provider attempted a second CharProperties::s_temp owner";return false;}in.temporary=temporary;
  player_script_owner_v62=character::skills::CharacterPlayerSkillsV6::create(services.design->borrow(),in,services.skills,inputs.faeries,*services.effect_services,actor->machine->native_fsm(),inputs.initialization,e);
  if(!player_script_owner_v62)return false;
  auto& session=player_script_owner_v62->session();
  if(!actor->bind_player_script_lifecycle_v62(&session.owner().lifecycle(),e))return false;
  if(session.advance()!=0){e=session.error();return false;}
  character::ScriptSessionView active{};if(session.owner().active(active)){actor->ai_events.active=active.identity;actor->ai_events.ais_virtuals=character::character_script_source_virtuals_v101(active.kind);if(!actor->ai_events.ais_virtuals){e="Required actual selected player AIS constructor table";return false;}}
  return true;
 }
 if(!services.npc_script){e="Required actual NPC script cache/target/controller/timer inputs";return false;}
 character::CharacterScriptSessionInput input;if(!services.npc_script(*this,input,e))return false;
 if(!actor->construct_script(services.design->borrow(),std::move(input))){e=actor->error();return false;}
 if(!services.skills||!services.faeries_v70||!services.effect_services){e="Required actual NPC Skill/Faery/Debug constructor resources";return false;}
 try{npc_skills_v84=std::make_unique<character::CanonicalNpcSkillsV84>(*actor,services.skills,services.faeries_v70,*services.effect_services);actor->source_retire_ctor_skill_vectors_v84();}
 catch(const std::exception& exception){e=exception.what();return false;}
 if(!actor->load_script()){e=actor->error();return false;}return true;
}
bool CanonicalCharacterCandidateRecordV60::initialize_loaded_script(std::uint32_t final,std::string& e){
 if(script_init_attempted_v62){e="Source Character InitProcess cannot replay failed/delivered prefix";return false;}script_init_attempted_v62=true;
 if(player_script_owner_v62){const auto status=player_script_owner_v62->native_init_loaded_script_v62(final);if(status<0){e=player_script_owner_v62->error();return false;}return true;}
 if(!actor||!actor->session){e="Required SAME loaded NPC ScriptOwner for InitProcess";return false;}
 if(!npc_skills_v84||!services.effect_services){e="Required SAME NPC source skill vectors before InitProcess";return false;}
 npc_loaded_init_v70=std::make_unique<character::CharacterDeferredScript>(*actor->session);
 const character::CharacterInitServices16 callbacks{this,[](void* raw,character::CharacterScriptSession& session,const character::ScriptLifecycleRequest32& q){
  auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(raw);
  if(!r.actor||r.actor->session.get()!=&session||q.subject!=r.actor->object->identity||!r.npc_skills_v84)return -1;
  if(q.service==character::script_refresh_vitals){character::ScriptInitVitals24 result{};const character::ScriptInitVitals32 request{q.subject,&session.property_view(),*r.services.effect_services};return dh2_character_script_init_vitals(&result,&request)==1?0:-1;}
  const int code=q.service==character::script_configure_skills?r.npc_skills_v84->configure():q.service==character::script_update_skills?r.npc_skills_v84->update():-1;
  if(code<0){r.error=r.npc_skills_v84->error();return -1;}return 0;
 }};
 const int result=npc_loaded_init_v70->initialize_loaded_v70(final,callbacks);
 if(result<0){e=npc_loaded_init_v70->error();return false;}return true;
}
int CanonicalCharacterCandidateRecordV60::source_load_n_init_v106(std::uint32_t final,std::string& e){
 if(final>1||!actor||!actor->object)return -1;
 if(player_script_owner_v62){const int result=player_script_owner_v62->initialize(final);if(result<0)e=player_script_owner_v62->error();return result;}
 if(!actor->session){if(!load_script(e))return -2;}
 if(!npc_skills_v84||!services.effect_services){e="Required SAME NPC skill vectors/InitProcess service storage";return -2;}
 if(!npc_loaded_init_v70)npc_loaded_init_v70=std::make_unique<character::CharacterDeferredScript>(*actor->session);
 const character::CharacterInitServices16 callbacks{this,[](void* raw,character::CharacterScriptSession& session,const character::ScriptLifecycleRequest32& q){
  auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(raw);
  if(r.actor->session.get()!=&session||q.subject!=r.actor->object->identity||!r.npc_skills_v84)return -1;
  if(q.service==character::script_refresh_vitals){character::ScriptInitVitals24 result{};
   const character::ScriptInitVitals32 request{q.subject,&session.property_view(),*r.services.effect_services};return dh2_character_script_init_vitals(&result,&request)==1?0:-1;}
  const int result=q.service==character::script_configure_skills?r.npc_skills_v84->configure():q.service==character::script_update_skills?r.npc_skills_v84->update():-1;
  if(result<0)r.error=r.npc_skills_v84->error();return result<0?-1:0;
 }};
 const int result=npc_loaded_init_v70->load_and_init(final,callbacks);
 if(result<0){e=npc_loaded_init_v70->error();return result;}
 character::ScriptSessionView active{};
 if(actor->session->owner().active(active)){actor->ai_events.active=active.identity;actor->ai_events.ais_virtuals=character::character_script_source_virtuals_v101(active.kind);
  if(!actor->ai_events.ais_virtuals){e="Required actual selected AIS constructor table after frame loading";return -2;}}
 return result;
}
bool CanonicalCharacterCandidateFactoryV60::retire_closed_v111(std::uintptr_t id,std::string& e){
 auto at=records_.find(id);if(at==records_.end()){e="Unknown actual closed Character receiver";return false;}
 const auto& record=at->second;
 if(!record->actor||record->actor->object||record->physical_owner_v62||record->visual||record->player_script_owner_v62||record->actor->session){e="Character native D0 prefix has not completed";return false;}
 records_.erase(at);return true;
}
bool CanonicalCharacterCandidateRecordV60::refresh_hud_actor_v62(std::string& e){
 if(!actor||!actor->object||actor->object->properties!=properties||!actor->machine){e="Required SAME canonical actor for persistent HUD projection";return false;}
 hud_actor_v62.resolved=properties->resolved.data();hud_actor_v62.resolved_count=properties->resolved.size();hud_actor_v62.identity=actor->object->identity;
 hud_actor_v62.name_symbol=std::uint32_t(properties->resolved[18]);hud_actor_v62.debug_name=actor->source_name().c_str();hud_actor_v62.network_id=actor->machine->combat_fields().network_id;
 std::copy(actor->object->position.begin(),actor->object->position.end(),hud_actor_v62.position);
 if(player_script_owner_v62){const auto& state=player_script_owner_v62->state();
  auto project=[&](const character::skills::Slots16& source,std::vector<std::uintptr_t>& output)->bool{
   if(source.count>65536||(source.count&&!source.items)){e="Invalid actual owned skill-vector borrow for HUD";return false;}
   output.resize(source.count);for(std::uint32_t i=0;i<source.count;++i)output[i]=reinterpret_cast<std::uintptr_t>(source.items[i]);return true;
  };
  if(!project(state.skills,hud_skills_v62)||!project(state.spells,hud_spells_v62))return false;
  hud_actor_v62.skills=hud_skills_v62.data();hud_actor_v62.skill_count=hud_skills_v62.size();hud_actor_v62.spells=hud_spells_v62.data();hud_actor_v62.spell_count=hud_spells_v62.size();
 }
 return true;
}
int CanonicalCharacterCandidateRecordV60::target_borrow_v62(void* raw,character::skills::WorldTargetActorBorrowV1* out){
 if(!raw||!out)return -1;auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(raw);
 if(!r.actor||!r.actor->object||!r.actor->machine)return -1;
 auto& actor=*r.actor;const auto* visible=actor.source_bool_field(0x80);const auto* disabled=actor.source_bool_field(0x81);const auto* enabled=actor.source_bool_field(0x8a);const auto* zoned=actor.source_bool_field(0x2ee);const auto* inzone=actor.source_bool_field(0x2f0);
 if(!visible||!disabled||!enabled||!zoned||!inzone)return -1;
 r.target_character_v62.resolved=r.properties->resolved.data();r.target_character_v62.name=actor.object->name.c_str();r.target_character_v62.flags520=actor.machine->state().flags;r.target_character_v62.dead1449=r.life->dead;r.target_character_v62.disabled81=*disabled;r.target_character_v62.visible8a=*enabled;
 r.target_search_v62.identity=actor.object->identity;r.target_search_v62.visible=*visible;r.target_search_v62.zoned=*zoned;r.target_search_v62.in_zone=*inzone;r.target_search_v62.character_word1310=r.properties->resolved[198];r.target_search_v62.character_word1314=r.properties->resolved[199];
 std::copy(actor.object->position.begin(),actor.object->position.end(),r.target_search_v62.position);r.target_search_v62.rotation=actor.runtime.rotation.rotation[2];
 GameObjectInitializationFieldsV62 fields;if(!actor.inherited_initialization_fields_v62(r.actor,fields,r.error))return -1;
 character::skills::WorldTargetActorBorrowV1 b;b.identity=actor.object->identity;b.search=&r.target_search_v62;b.character=&r.target_character_v62;b.life=r.life.get();b.scene=r.visual&&r.visual->visual()?&r.visual->visual()->scene():nullptr;
 b.target_node=fields.pointer(0x180);b.target_enabled=visible;b.cached_target_position=actor.runtime.target_position;b.position=actor.object->position.data();b.heading_angle=&actor.runtime.rotation.rotation[2];b.controller_heading_angle=&actor.runtime.rotation.heading_angle;b.base_byte2ed=actor.source_bool_field(0x2ed);b.aabb6=actor.runtime.subobjects.absolute_bounds;
 b.talk_required2fa_v108=actor.source_bool_field(0x2fa);
 *out=b;return 0;
}
bool CanonicalCharacterCandidateRecordV60::register_world_target_v62(std::string& e){
 if(target_registered_v62)return true;
 if(!services.world_targets||!actor||!actor->machine||!actor->object||!actor->shared_handle().key){e="Required actually published canonical Character/Handle before target registration";return false;}
 if(!services.canonical_objects){e="Required SAME canonical ObjectManager frame78 before Character target registration";return false;}
 services.world_targets->begin_frame(services.canonical_objects->source_frame78_v4());
 character::skills::WorldActorRegistrationV1 registration;registration.identity=actor->object->identity;registration.handle_key=actor->shared_handle().key;registration.context=this;registration.refresh=target_borrow_v62;registration.machine=&actor->machine->state();registration.shared_handle=&actor->shared_handle();registration.current_target=&actor->object->target.target;
 registration.controller=[](void* raw,character::ControllerCommandState32* out){auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(raw);if(!out||!r.services.controller)return -1;return r.services.controller(*out,r,r.error)?0:-1;};
 registration.machine_state_v108=[](void* raw,std::uint32_t* out){auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(raw);if(!out||!r.actor||!r.actor->machine)return -1;std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&r.actor->machine->native_fsm(),0)!=1)return -1;*out=static_cast<std::uint32_t>(state);return 0;};
 if(services.world_targets->add(registration)!=0){e=services.world_targets->error();return false;}target_registered_v62=true;return true;
}
bool CanonicalCharacterCandidateRecordV60::prepare_equipment(std::string& e){
 if(equipment||prepared_equipment_v60||inventory_transferred||!profile_bootstrap||!constructor_inventory||!services.equipment_inputs){
  e="Required once-only actual visual/equipment inputs and selected Save bootstrap at SG_Load4";return false;
 }
 player::PlayerEquipmentRenderInputsV1 input;
 if(!services.equipment_inputs(*this,input,e))return false;
 // Transport fixes authority from this exact C1 receiver. Art/visual/debug/
 // world/text/required effect services must come from genuine source owners.
 if((input.character&&input.character!=actor->shared_handle().cached)||
    (input.properties&&input.properties!=properties)||(input.random&&input.random!=services.random)){
  e="Player equipment input attempts another canonical authority";return false;
 }
 input.design=services.design->borrow();input.character=actor->shared_handle().cached;input.properties=properties;input.random=services.random;
 input.potion_capacity=constructor_inventory->potion_capacity_v4();
 input.source_visual2d8_v62=&actor->source_visual();
 if(!profile_bootstrap->configure_equipment(input,e)||!transfer_inventory(input,e))return false;
 equipment=std::make_unique<player::PlayerEquipmentRenderOwnerV1>(std::move(input));prepared_equipment_v60=equipment.get();
 if(!equipment->prepare_restore_v60(e))return false;
 if(equipment->inventory()!=inventory37c||equipment->properties()!=properties){e="Prepared Gear lost source inventory37c/sheets";return false;}
 return true;
}
bool CanonicalCharacterCandidateRecordV60::take_equipment(std::unique_ptr<player::PlayerEquipmentRenderOwnerV1>& out,std::string& e){
 if(out||failed||!init_complete||!equipment||!equipment->ready()||equipment.get()!=prepared_equipment_v60){e="Required completed SAME candidate InitPost/Gear before runtime ownership transfer";return false;}
 out=std::move(equipment);e.clear();return true;
}
bool CanonicalCharacterCandidateRecordV60::load_visual_v77(std::string& e){
 if(!bind_visual(e)||!visual->load_visual(e))return false;
 return !services.visual_ready_v77||services.visual_ready_v77(*this,e);
}
bool CanonicalCharacterCandidateRecordV60::bind_visual(std::string& e){
 if(visual)return true;
 if(!services.visual){e="Required actual Character SceneManager/cache/CharAnimator providers";return false;}
 character::CharacterFamilyVisualServicesV6 source;
 if(!services.visual(*this,source,e))return false;
 if(source.world!=services.world){e="Character visual belongs to another World lifetime";return false;}
 if(!source.animation_selection){std::weak_ptr<CanonicalCharacterCandidateRecordV60> weak=shared_from_this();
  source.animation_selection=[weak](auto&,auto& selected,auto& error){auto r=weak.lock();
   if(!r||!r->services.animation_tables||!r->services.skills){error="Required actual CharAnim/SkillList tables";return false;}
   const auto& tables=*r->services.animation_tables;
   auto table=r->properties->resolved[2];if(table<0||std::size_t(table)>=tables.characters.size())table=17;
   if(table<0||std::size_t(table)>=tables.characters.size()){error="Required original CharAnim fallback17 row";return false;}
   auto list=r->properties->resolved[28];const auto& lists=r->services.skills.lists();if(list<0||std::size_t(list)>=lists.size())list=3;
   if(list<0||std::size_t(list)>=lists.size()){error="Required original SkillList fallback3 row";return false;}
   selected.table=table;selected.skill_list=list;selected.skill_animations.clear();
   for(auto id:lists[list]){if(id<0){selected.skill_animations.push_back(-1);continue;}
    if(std::size_t(id)>=r->services.skills.skills().size()){error="Required actual SkillGetData out-of-range branch";return false;}
    selected.skill_animations.push_back(static_cast<std::int32_t>(r->services.skills.skills()[id].scalar.words[1]));
   }return true;
  };
 }
 if(!source.register_set_v62){std::weak_ptr<CanonicalCharacterCandidateRecordV60> weak=shared_from_this();
  source.register_set_v62=[weak](const auto& tables,auto table,auto set,const auto& skills,auto callbacks,auto& error){auto r=weak.lock();
   if(!r){error="Released actual Character animation registration";return false;}bool player{};if(!r->is_player(player,error))return false;
   const auto* design=r->design.design();std::int32_t count=1,mask{};
   if(!design||!design->lookup||(player&&design->lookup(design->context,0,"AnimStances","COUNT_IPHONE",&count))||
      design->lookup(design->context,0,"AnimStancedAnim","SL__LIST_IPHONE",&mask)){
    error="Required actual PyDataConstants animation registration producer";return false;
   }
   return character::character_generic_register_animation_set_v62(tables,table,set,count,static_cast<std::uint32_t>(mask),skills,std::move(callbacks),error);
  };
 }
 visual=std::make_unique<character::RetainedCharacterFamilyVisualV6>(actor,std::move(source));return true;
}
bool CanonicalCharacterCandidateRecordV60::initialize(std::string& e){
 if(failed){e=error.empty()?"Failed Character candidate InitPost prefix cannot retry":error;return false;}
 if(init_attempted){if(init_complete)return true;e="Character InitPost candidate cannot replay incomplete continuation";return false;}
 init_attempted=true;
 if(!actor->init_post_fields(init_fields,e)){failed=true;error=e;return false;}
 if(services.world_targets&&!register_world_target_v62(e)){failed=true;error=e;return false;}
 character::CharacterGenericInitPostFieldsV61 fields;fields.common=init_fields;
 fields.save14e8=save_fields->save_slot14e8();fields.inventory_context=this;
 fields.store_gold3a4=[](void* p,std::int32_t value,auto& error){auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(p);
  if(!r.prepared_equipment_v60){error="Required SAME prepared Gear before source gold store";return false;}
  return r.prepared_equipment_v60->source_initpost_gold_store_v61(value,error);
 };
 fields.store_capacity3a8=[](void* p,std::uint8_t value,auto& error){auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(p);
  if(!r.prepared_equipment_v60||!r.prepared_equipment_v60->prepared_v60()){
   error="Required SAME pre-grant Gear before source capacity store";return false;
  }
  r.prepared_equipment_v60->project_potion_capacity(static_cast<std::int8_t>(value));error.clear();return true;
 };
 generic_init=std::make_unique<character::CharacterGenericInitPostOwnerV61>(fields,character::NpcInitPostServicesV1{this,init_service});
 if(!generic_init->initialize()){e=generic_init->error();failed=true;error=e;return false;}
 bool player{};if(!is_player(player,e)){failed=true;error=e;return false;}
 if(player&&(!services.finish_player_profile_v67||!services.finish_player_profile_v67(*this,e))){if(e.empty())e="Required SAME completed player/profile writer attachment";failed=true;error=e;return false;}
 init_complete=true;return true;
}
bool CanonicalCharacterCandidateRecordV60::init_service(void* raw,const character::NpcInitPostRequestV1& q,
 character::NpcInitPostResponseV1& out,std::string& e){
 auto& r=*static_cast<CanonicalCharacterCandidateRecordV60*>(raw);
 if(!r.actor||q.subject!=r.actor->shared_handle().cached){e="Required SAME source Character InitPost identity";return false;}
 if(q.source_entry==0x337888||q.source_entry==0x337a88){
  if(!r.services.debug||!r.services.debug_files){e="Required SAME Application Character Debug services";return false;}
  if(q.source_entry==0x337888){if(dh2_character_debug_load(r.services.debug,r.services.debug_files)!=1){e="Original Debug load failed";return false;}return true;}
  std::uint32_t value{};if(!q.text||dh2_character_debug_get(&value,r.services.debug,q.text,r.services.debug_files)!=1){e="Original Debug query failed";return false;}out.value=static_cast<std::int32_t>(value);return true;
 }
 if(q.source_entry==0x3a49f0){bool player;if(!r.is_player(player,e))return false;out.value=player;return true;}
 if(q.source_entry==0x3a2fec){out.ai=actual_ai(r);if(!out.ai){e="Required actual Character AI row/fallback8";return false;}return true;}
 if(q.source_entry==0x3b4738){
  if(r.services.register_character_fx)return r.services.register_character_fx(r,e);
  if(!r.services.character_effects){e="Required SAME actual CharacterEffects table";return false;}
  const auto& rows=*r.services.character_effects;
  character::InitFxRows16 input{rows.data(),static_cast<std::uint32_t>(rows.size()),r.properties->resolved[7]};
  const auto status=dh2_character_init_fx_register(&input,r.services.effect_table,r.services.effect_queue,r.services.effect_services);
  if(status!=1){e="Original Character FX registration failed: "+std::to_string(status);return false;}return true;
 }
 if(q.source_entry==0x495430){
  const auto id=static_cast<std::int32_t>(q.argument0);
  if(r.services.grab_fx)return r.services.grab_fx(r,id,out.identity,e);
  if(!r.services.effect_table){e="Required actual FX manager table";return false;}
  const auto status=dh2_character_init_fx_negative_grab(&out.identity,id,r.services.effect_table->set_count,r.services.effect_services);
  if(status!=1){e="Required original positive GrabAnimFX factory: "+std::to_string(id);return false;}return true;
 }
 if(q.source_entry==0x36effc){
  if(!r.services.is_local_player){e="Required actual PM IsLocalPlayer/PlayerInfo virtual50";return false;}
  bool local{};if(!r.services.is_local_player(q.subject,local,e))return false;out.value=local;return true;
 }
 if(q.source_entry==0x4679e8){
  if(!r.save||!r.quest_sync_owner||q.payload!=reinterpret_cast<std::uintptr_t>(r.save.get())){
   e="Required SAME Save for SG_TryQuestSync";return false;
  }
  return r.quest_sync_owner->try_sync(r.services.quest_sync,e);
 }
 if(q.source_entry==0x31f594){
  if(!r.services.current_level){e="Required actual Application.GetCurrentLevel for player InitPost";return false;}
  CharacterCurrentLevelBorrowV61 current;
  if(!r.services.current_level(current,e))return false;
  if(current.identity&&(!current.receiver||!current.mode118)){e="Required SAME actual current Level+118 receiver";return false;}
  out.identity=current.identity;r.captured_level_v61=std::move(current);return true;
 }
 if(q.source_entry==0x3bb950){
  if(!r.captured_level_v61.identity||r.captured_level_v61.identity!=q.payload||!r.captured_level_v61.mode118){
   e="Required captured current Level for SG_SetGameDifficulty";return false;
  }
  const auto requested=*r.captured_level_v61.mode118;
  r.captured_level_v61={};
  if(!r.save_fields){e="Required source Character14e8 field";return false;}
  if(!*r.save_fields->save_slot14e8())return true; // Exact source NULL-save guard.
  if(!r.save||*r.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r.save.get())){e="Required SAME Save for SG_SetGameDifficulty";return false;}
  if(requested>r.save->unlocked_difficulty())return true; // Signed source bxgt.
  if(!r.services.difficulty_global){e="Required actual PlayerSavegame::m_difficultyLevel9a6060 owner";return false;}
  r.services.difficulty_global->store_from_pdfl(requested);return true;
 }
 if(q.source_entry==0x3b3d38){
  const auto* array=r.actor->source_string(0x1398);const auto* name=r.actor->source_string(0x13b0);
  if(!array||!name){e="Required actual Character property CString producers";return false;}
  bool selected_player{};if(!r.is_player(selected_player,e))return false;
 if(selected_player&&r.save&&r.save->slot()!=-1&&(!r.load||!r.load->profile().identity||r.save->class_id()<0||
     (r.services.selected_profile_class&&r.save->class_id()!=*r.services.selected_profile_class))){
   e="Required actual selected PCLS before player SafeGetCharPropsId; no saved-class fallback";return false;
  }
  character::CharacterPropsIdServicesV1 services=r.services.presets;services.context=&r;
  services.is_player=[](void* p,bool& value,auto& error){return static_cast<CanonicalCharacterCandidateRecordV60*>(p)->is_player(value,error);};
  services.load_save=[](void* p,auto& save,std::int32_t mask,auto& error){auto& self=*static_cast<CanonicalCharacterCandidateRecordV60*>(p);
   if(!self.load||&self.load->save()!=&save){error="Required actual SAME selected SaveLoad for SafeGetCharPropsId";return false;}return self.load->load(mask,error);
  };
  // Preset context must remain its genuine provider, not the record context.
  services.preset=[](void* p,const auto& name,const std::int16_t*& rows,std::uint32_t& count,auto& error){auto& self=*static_cast<CanonicalCharacterCandidateRecordV60*>(p);
   if(self.services.character_templates_v78){
    auto& fields=self.actor->kill_fields_v42();std::int32_t id{};
    if(!fields.produced){error="Required SAME CharacterC1 template13ca before preset query";return false;}
    if(!self.services.character_templates_v78->safe_template_id(name,fields.template13ca,id,error))return false;
   }
   const auto& source=self.services.presets;if(!source.preset){error="Required actual CharacterPropertiesTemplate/Random selector";return false;}
   return source.preset(source.context,name,rows,count,error);
  };
  return character::character_safe_props_id_v1(*r.init_fields.properties_id13c8,*array,*name,*r.design.characters(),
   r.save.get(),*r.services.random,services,out.value,e);
 }
 if(q.source_entry==0x3df2a4){const auto index=static_cast<std::int32_t>(q.argument0);const auto& rows=r.design.characters()->rows;
  if(index>=0&&std::size_t(index)<rows.size())r.properties->base=rows[index];return true;
 }
 if(q.source_entry==0x3e0810)return data::recalc_properties_with_class(*r.design.classes(),*r.design.rules(),*r.properties,e);
 if(q.source_entry==0x3df6e0){
  // Exact PROPS_GetInt194(false): cached resolved sheet, arithmetic >>8.
  if(q.argument1||q.argument0>=r.properties->resolved.size()){e="Required source default-sheet GetInt branch or valid property ID";return false;}
  const auto raw=r.properties->resolved[q.argument0];
  out.value=raw>=0?raw/256:static_cast<std::int32_t>(-((-(static_cast<std::int64_t>(raw))+255)/256));return true;
 }
 if(q.source_entry==0x3defac||q.source_entry==0x3df480){
  if(!r.prepared_equipment_v60){e="Required prepared SAME inventory for source gear properties";return false;}
  return q.source_entry==0x3defac?r.prepared_equipment_v60->source_reset_gear_properties_v61(e):r.prepared_equipment_v60->source_load_gear_properties_v61(e);
 }
 if(q.source_entry==0x3a54d4){
  if(!r.services.models){e="Required actual Character model dictionary";return false;}
  auto actual=r.services.model_name;actual.receiver=r.actor;
  actual.is_faery=[&r](bool& value,std::string& error){const auto* ai=actual_ai(r);if(!ai){error="Required actual Character IsFaerie AiProps";return false;}value=ai->type==3;return true;};
  actual.is_player=[&r](bool& value,std::string& error){return r.is_player(value,error);};
  actual.is_local_player=[&r](bool& value,std::string& error){if(!r.services.is_local_player){error="Required actual PM local model predicate";return false;}return r.services.is_local_player(r.actor->object->identity,value,error);};
  return character::character_model_name_v62(r.properties->resolved[3],*r.init_fields.properties_id13c8,*r.services.models,actual,out.text,e);
 }
 if(q.source_entry==0x38bd64){if(!r.bind_inherited_initialization(e))return false;return r.inherited_init->check_spawn_probability(out.value,e);}
 if(q.source_entry==0x33ec0c){if(!r.bind_inherited_initialization(e))return false;return r.inherited_init->object_base_init_post(e);}
 if(q.source_entry==0x38be5c){
  if(!r.bind_inherited_initialization(e))return false;bool eligible{};
  if(!r.inherited_init->init_post(eligible,e))return false;out.value=eligible;return true;
 }
 if(q.source_entry==0x38ab60){bool meet{};if(!game_object_meet_condition_v1(meet,e))return false;out.value=meet;return true;}
 if(q.source_entry==0x3b4088)return r.initialize_physical(e);
 if(q.source_entry==0x3cf1f0)return r.load_script(e);
 if(q.source_entry==0x3ce7c0)return r.initialize_loaded_script(q.argument0,e);
 if(q.source_entry==0x394eb0){ // GameObject::LoadVisualObject, called by inherited InitPost.
  return r.load_visual_v77(e);
 }
 if(q.source_entry==0x3c9f4c){ // CharAnimator::SetAnimationSet, separate later call.
  if(!r.visual||!r.visual->visual()){e="Required inherited source visual before SetAnimationSet";return false;}
  if(!r.services.animation_tables){e="Required actual CharAnimator AnimationTables";return false;}
  return r.visual->set_animation_set(*r.services.animation_tables,e);
 }
 if(q.source_entry==0x393db4){if(!q.payload){e="Required actual Character initial position pointer";return false;}
  std::array<float,3> p;std::copy_n(reinterpret_cast<const float*>(q.payload),3,p.data());return r.set_position(p,q.argument0!=0,e);
 }
 if(q.source_entry==0x3a58f4){
  if(!q.payload||!r.init_fields.initial_position1450){e="Required SAME Character SetInitialPosition source cells";return false;}
  const auto* source=reinterpret_cast<const float*>(q.payload);auto* initial=r.init_fields.initial_position1450;
  // Original stores X/Y/Z before querying PFWorld. Failure retains these
  // stores; miss preserves their copied Z instead of snapping to an up plane.
  for(unsigned i=0;i<3;++i)initial[i]=source[i];
  if(!r.services.initial_height){e="Required actual PFWorld GetFloorBelow525508";return false;}
  bool hit{};float height{};if(!r.services.initial_height(initial,hit,height,e))return false;if(hit)initial[2]=height;return true;
 }
 if(q.source_entry==0x3bc4d0){
  if(q.argument0==4){
   if(r.services.prepare_player_equipment)return r.services.prepare_player_equipment(r,e);
   return r.prepare_equipment(e); // Actual V59 hook delivers mask4 exactly once.
  }
  if(!r.save_fields){e="Required actual Character14e8 slot producer";return false;}
  const auto* slot=r.save_fields->save_slot14e8();
  if(!*slot)return true; // Genuine fresh NPC constructor NULL14e8 branch.
  if(!r.save||!r.load||*slot!=reinterpret_cast<std::uintptr_t>(r.save.get())||&r.load->save()!=r.save.get()){
   e="Required actual same Character SG_Load receiver";return false;
  }
  return r.load->load(static_cast<std::int32_t>(q.argument0),e);
 }
 if(q.source_entry==0x3b395c){
  if(!r.prepared_equipment_v60||r.prepared_equipment_v60->inventory()!=r.inventory37c){
   e="Required SAME prepared Character inventory37c at _InitEquipment";return false;
  }
  return r.prepared_equipment_v60->finish_initial_grants_v60(e);
 }
 if(q.source_entry==0x3b3a90)return r.initialize_skill_slots_v70(e);
 if(q.source_entry==0x3d8cfc){if(!r.npc_skills_v84){e="Required SAME NPC source configure receiver";return false;}if(r.npc_skills_v84->configure()<0){e=r.npc_skills_v84->error();return false;}return true;}
 if(q.source_entry==0x3d8894){
  if(r.player_script_owner_v62){if(r.player_script_owner_v62->native_source_update_all_skills_v70()<0){e=r.player_script_owner_v62->error();return false;}return true;}
  if(r.npc_skills_v84){if(r.npc_skills_v84->update()<0){e=r.npc_skills_v84->error();return false;}return true;}
  // Delayed NPCs legitimately have no VM yet. Execute the source FSM gates
  // before borrowing the genuine fresh CharAI C1 empty-vector projection.
  std::int32_t state{};if(!r.actor->machine||dh2_character_native_fsm_get_integer(&state,&r.actor->machine->native_fsm(),0)!=1){e="Required SAME NPC UpdateAllSkills FSM predicate";return false;}
  if(state==6)return true;
  if(dh2_character_native_fsm_get_integer(&state,&r.actor->machine->native_fsm(),0)!=1){e="Required SAME NPC UpdateAllSkills casting predicate";return false;}if(state==7)return true;
  const auto* empty=r.actor->source_ctor_empty_skill_vectors_v84();
  if(!empty||(*empty)[0]||(*empty)[1]){e="Required actual NPC skill-vector owner after source C1";return false;}return true;
 }
 if(q.source_entry==0x3a59ac)return r.revive_v70(q.payload,q.argument1,e);
 if(q.source_entry==0x3b3a70){
  if(!r.services.effect_services){e="Required actual shared Debug for InitHpMp";return false;}
  character::ScriptInitVitals24 result{};const character::ScriptInitVitals32 request{r.actor->object->identity,&r.view,*r.services.effect_services};
  if(dh2_character_script_init_vitals(&result,&request)!=1){e="Required original Character InitHpMp";return false;}return true;
 }
 if(q.source_entry==0x470a54){
  auto actual=r.visual?r.visual->visual():nullptr;if(!actual||q.payload!=reinterpret_cast<std::uintptr_t>(actual.get())){e="Required SAME VisualObject ApplyMeshBox receiver";return false;}
  return actual->apply_mesh_box(e);
 }
 if(q.source_entry==0x59719c){
  auto actual=r.visual?r.visual->visual():nullptr;if(!actual||q.payload!=reinterpret_cast<std::uintptr_t>(actual.get())){e="Required SAME visual root AutomaticCulling receiver";return false;}
  actual->source_set_automatic_culling_v70(q.argument0);return true;
 }
 if(q.source_entry==0x3d37d0){
  return character::source_character_add_to_group_v87(r.shared_from_this(),e);
 }
 if(q.source_entry==0x3b3b00){if(!r.services.initialize_sounds_v70){e="Required Character GetCharSounds/Vox LoadSound initialization";return false;}return r.services.initialize_sounds_v70(r,e);}
 if(q.source_entry==0x3b5214){
  if(!r.services.initialize_target_marker_v70){e="Required SAME Character target marker initialization";return false;}
  bool present{};if(!r.services.initialize_target_marker_v70(r,present,e))return false;out.value=present;return true;
 }
 if(q.source_entry==0x3a41a0){if(!r.services.initialize_highlight_v70){e="Required SAME Character multiplayer-highlight receiver";return false;}return r.services.initialize_highlight_v70(r,e);}
 if(r.services.remaining)return r.services.remaining(r,q,out,e);
 e="Required source Character candidate InitPost helper "+std::to_string(q.source_entry);return false;
}
bool CanonicalCharacterCandidateRecordV60::close_after_unpublication(std::string& e){
 spawn_select_v87.reset();
 if(save_connection_v86&&!save_connection_v86->close(e))return false;
 save_connection_v86.reset();
 // The death adapter borrows and temporarily wraps the SAME actor callbacks.
 // Restore them while the FSM, physical body and selected VM still exist.
 death_v84.reset();
 if(actor){auto* cross=actor->source_target_cross149c_v70();if(cross&&*cross){
  if(!target_fx_pin_v70||!target_fx_manager_v70){e="Required SAME FX manager before target-cross release";return false;}if(!target_fx_manager_v70->drop(*cross,e))return false;
 }}
 if(actor&&actor->source_highlight14a0()){
  if(!target_fx_pin_v70||!target_fx_manager_v70){e="Required SAME retained FX manager before Character highlight release";return false;}
  if(!target_fx_manager_v70->drop(actor->source_highlight14a0(),e))return false;
 }
 if(target_marker_v70&&!target_marker_v70->release(e))return false;target_marker_v70.reset();target_fx_manager_v70=nullptr;target_fx_pin_v70.reset();
 if(physical_owner_v62&&!physical_owner_v62->release()){e=physical_owner_v62->error();return false;}physical_owner_v62.reset();collision_owner_v68.reset();
 if(actor&&actor->position_fields_v7().physical2dc){e="Required actual physical release before Character candidate destruction";return false;}
 // Lua close/finalizers borrow actual Gear, properties, target/FSM and visual.
 // Keep those alive until this sole player VM and its instances are closed.
 if(player_script_owner_v62){player_script_owner_v62.reset();actor->detach_player_script_lifecycle_v62();}
 npc_loaded_init_v70.reset();npc_skills_v84.reset();
 if(actor&&actor->session)actor->session.reset();
 projectile_bindings_v112.reset();combat_bindings_v115.reset();fsm_context_v101.reset();if(actor)actor->bodies={};
 if(inventory_transferred&&!equipment_released){
  if(!services.release_player_equipment){e="Required SAME player Skill/Gear teardown before borrowed Character visual destruction";return false;}
  if(!services.release_player_equipment(*this,e))return false;equipment_released=true;prepared_equipment_v60=nullptr;
 }
 if(visual&&!visual->close(e))return false;visual.reset();profile_bootstrap.reset();quest_sync_owner.reset();load.reset();preview_profile_v122.reset();save.reset();
 // Embedded conditions clear while the actual Character storage is retained.
 // On failure the same record and remaining compiled slot survive for teardown.
 if(actor){GameObjectInitializationFieldsV62 same;
  if(!actor->inherited_initialization_fields_v62(actor,same,e))return false;
  if(!condition_data_clear_borrow_v62(same,0x8c,services.conditions,e)||
     !condition_data_clear_borrow_v62(same,0xb0,services.conditions,e))return false;
 }
 generic_init.reset();npc_init.reset();inherited_init.reset();
 if(actor){if(!character::source_character_remove_from_group_v87(*this,e))return false;actor->close();}
 position.reset();merchant_inventory_context_v114.reset();constructor_inventory.reset();inventory37c=nullptr;e.clear();return true;
}
}





