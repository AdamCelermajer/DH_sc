#include "source_campaign_zones_v83.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_conditions_v70.hpp"
#include "source_campaign_fx_v77.hpp"
#include "source_campaign_frame_services_v106.hpp"
#include <game_object_source_frame_v74.hpp>
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <script_manager_owner_v52.hpp>
#include <source_assertion_process_v76.hpp>
#include <effects_tables.hpp>
#include <checkpoint_save_runtime_v83.hpp>
#include <native_level_application_v25.hpp>
#include <algorithm>
#include <cstring>
namespace model_renderer {namespace {
struct CampaignZoneTransportV83 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 bool actual(std::shared_ptr<SourceWorldBorrowV61>& out,std::string& e)const{
  out=world.lock();if(!out||!out->owner||!out->application||!out->canonical_world||!out->player_manager){e="Required SAME campaign Zone World/App/PM";return false;}return true;
 }
 bool character(std::uintptr_t id,SourceCampaignCharacterBorrowV62& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!this->actual(actual,e))return false;
  return borrow_source_campaign_character_v62(actual->owner,id,out,e)&&bool(out.character);
 }
 bool candidate(SourceCampaignCandidateBorrowV55& out,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!this->actual(actual,e)||!borrow_source_campaign_candidate_v55(out,e))return false;
  auto expected=level.lock();if(!expected||out.actual_world.get()!=actual->owner.get()||out.level!=expected||out.objects!=actual->canonical_world->manager_lease){e="Campaign Zone candidate identity changed";return false;}return true;
 }
};
}
bool bind_source_campaign_zone_services_v83(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::world::ZoneCollisionServicesV83& s,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto transport=std::make_shared<CampaignZoneTransportV83>();transport->world=world;transport->level=candidate.level;
 // Keep incoming native provider alive independently; it must itself use weak
 // World scope. The callback fallback only handles actual non-Character peers.
 auto original_peer=s.peer;auto native_provider=s.provider;
 s.provider=transport;
 s.is_character=[transport](auto id,auto& result,auto& e){
  SourceCampaignCandidateBorrowV55 current;if(!transport->candidate(current,e))return false;
  const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool found=current.objects->source_ordered_begin_v38(key,object);
  while(found){if(object&&object->identity==id){if(!object->as_character){e="Required actual virtual24/Character conversion";return false;}std::uintptr_t character{};if(!object->as_character(object->context,character,e))return false;result=character!=0;e.clear();return true;}found=current.objects->source_ordered_next_v38(key,key,object);}
  e="Zone peer is not a published SAME ObjectManager receiver";return false;
 };
 s.handle_character=[transport](auto id,auto& character,auto& e){
  SourceCampaignCandidateBorrowV55 current;if(!transport->candidate(current,e))return false;
  SourceCampaignCharacterBorrowV62 c;if(!transport->character(id,c,e)||!c.character->actor)return false;
  auto actual=c.character->actor->canonical(c.character);if(!actual.shared_handle){e="Required SAME Character GetHandle";return false;}
  auto handle=*actual.shared_handle;const dh2::world::CanonicalObjectBorrowV1* object{};
  if(!current.objects->resolve_handle_v4(handle,false,object,{},e))return false;
  if(!object){character=0;e.clear();return true;}if(!object->as_character){e="Required actual ObjectHandle Character conversion";return false;}
  return object->as_character(object->context,character,e);
 };
 s.is_player=[transport](auto id,auto& result,auto& e){SourceCampaignCharacterBorrowV62 c;return transport->character(id,c,e)&&c.character->is_player(result,e);};
 s.local_player=[transport](auto index,auto flag,auto& id,auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  dh2::player::PlayerInfoFieldsV1* local{};if(!actual->player_manager->get_local_player(index,flag,local,e))return false;
  if(!local){e="Original Zone PM.GetLocalPlayer record NULL dereference";return false;}id=local->character660;e.clear();return true;
 };
 s.current_level=[transport](auto& out,auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
  out={};if(!current){e.clear();return true;}auto expected=transport->level.lock();
  if(!expected||current.level()!=expected){e="Zone received a different current Level";return false;}
  const auto& fields=expected->constructor_fields_v3();out={expected,expected->identity(),fields.events.get(),&fields.save_ec,&fields.field130,&fields.byte144,&fields.seed114,&fields.difficulty40,&fields.row3c};e.clear();return true;
 };
 s.player=[transport](auto id,auto& out,auto& e){
  SourceCampaignCharacterBorrowV62 c;if(!transport->character(id,c,e)||!c.character->actor||!c.character->life)return false;
  auto* a=c.character->actor.get();out={c.character,id,a->runtime.subobjects.position,a->checkpoint1468_v83(),a->save_position1474_v83(),{}};
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=c.character;
  out.dead=[weak](auto& dead,auto& e){auto same=weak.lock();if(!same||!same->life){e="Released actual Zone Character virtual34";return false;}dead=same->life->dead!=0;e.clear();return true;};e.clear();return true;
 };
 s.peer=[transport,original_peer,native_provider](auto id,auto& out,auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  const auto& characters=actual->canonical_world->manager.characters();
  if(std::find(characters.begin(),characters.end(),id)==characters.end()){
   if(!original_peer||!native_provider){e="Required same non-Character Zone physical/radius borrower";return false;}return original_peer(id,out,e);
  }
  SourceCampaignCharacterBorrowV62 c;if(!transport->character(id,c,e)||!c.character->actor)return false;
  auto* a=c.character->actor.get();out={};out.receiver=c.character;out.identity=id;out.position160=a->runtime.subobjects.position;
  out.absolute12c=a->runtime.subobjects.absolute_bounds;out.physical2dc=&a->position_fields_v7().physical2dc;
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=c.character;
  out.physical_radius=[weak](float& radius,auto& e){auto same=weak.lock();if(!same||!same->physical_owner_v62){e="Required SAME actual Character PhysicalObject.GetRadius";return false;}radius=same->physical_owner_v62->projection().body.radius;e.clear();return true;};e.clear();return true;
 };
 s.constant=[transport](auto group,auto key,auto& value,auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e)||!actual->design)return false;
  auto borrow=actual->design->borrow();auto design=borrow.design();
  if(!design||!design->lookup||design->lookup(design->context,0,group,key,&value)){e="Required actual Zone PyDataConstants";return false;}e.clear();return true;
 };
 s.assertion=[](auto file,auto line,auto expression,auto& e){auto actual=dh2::world::SourceAssertionProcessV76::borrow();const auto mode=*actual->source_level();
  if(mode==2){e="Original Zone mode2 assertion NULL write refused";return false;}if(mode==1)return actual->report(file,line,expression,e);e.clear();return true;};
 s.save_player_checkpoint=[transport](auto id,auto& e){
  SourceCampaignCharacterBorrowV62 c;if(!transport->character(id,c,e))return false;
  if(!c.character->save){e.clear();return true;} //Character3bc49c genuineNULL14e8
  if(!c.character->load||&c.character->load->save()!=c.character->save.get()){e="Checkpoint Character SaveLoad authority mismatch";return false;}
  auto bootstrap=c.character->profile_bootstrap;
  dh2::level::PlayerCheckpointServicesV83 source;source.provider=transport;
  source.online=[transport](auto& online,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto owner=actual->application->get_online_loading_v55();if(!owner){e="Required checkpoint actual GetOnline";return false;}online=owner->byte5()!=0;e.clear();return true;};
  source.local_hosting=[transport](auto& hosting,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
   auto connection=actual->native_level_c1_v25?*actual->native_level_c1_v25:nullptr;
   if(!connection||!connection->bindings()){e="Required SAME Level constructor hosting provider";return false;}
   auto services=connection->bindings()->services();if(!services.local_player_hosting){e="Required actual checkpoint hosting selector";return false;}return services.local_player_hosting(hosting,e);};
  source.manager719=[transport](auto& flag,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto manager=actual->player_manager->manager();auto fields=manager?manager->source_frame_fields_v68():nullptr;if(!fields){e="Required actual PM719 C1 field";return false;}flag=fields->byte719;e.clear();return true;};
  //Online stream setup is a separate actual profile transport and fails at
  //its reached leaf. Offline passes never allocate or substitute that stream.
  return dh2::level::player_save_checkpoint_v83(*c.character->load,bootstrap?bootstrap->profile().get():nullptr,source,e);
 };
 s.save_level_checkpoint=[transport](const auto& receiver,auto seed,auto difficulty,auto row,auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  auto connection=actual->native_level_c1_v25?*actual->native_level_c1_v25:nullptr;
  auto runtime=connection&&connection->bindings()?connection->bindings()->save():nullptr;
  if(!runtime||runtime.get()!=receiver.get()||runtime.owner_before(receiver)||receiver.owner_before(runtime)){e="Checkpoint Save_ec does not match SAME LevelSavegame C1";return false;}
  return runtime->source_save_checkpoint_v83(seed,difficulty,row,e);
 };
 e.clear();return true;
}
bool bind_source_campaign_trigger_services_v83(const SourceCampaignCandidateBorrowV55& candidate,
 std::uintptr_t zone,const dh2::world::ZoneCollisionServicesV83& common,dh2::world::TriggerZoneServicesV22& s,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!zone||!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto transport=std::make_shared<CampaignZoneTransportV83>();transport->world=world;transport->level=candidate.level;
 s.is_character=common.is_character;s.is_player=common.is_player;
 s.local_player=[common](auto index,auto flag,auto& out,auto& e){std::uintptr_t id{};if(!common.local_player||!common.local_player(index,flag,id,e))return false;
  out={id,0};if(id){dh2::world::ZonePlayerBorrowV83 player;if(!common.player||!common.player(id,player,e)||!player.dead)return false;bool dead{};if(!player.dead(dead,e))return false;out.dead1480=dead;}e.clear();return true;};
 s.player_count=[transport](auto& count,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto value=actual->player_manager->count_field();if(!value){e="Required SAME source PM6c4";return false;}count=*value;e.clear();return true;};
 s.player_character=[transport](auto index,auto flag,auto& id,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  auto manager=actual->player_manager->manager();dh2::player::PlayerInfoFieldsV1* player{};if(!manager||!manager->get_player(index,flag,player,e))return false;
  if(!player){e="Original Trigger GetPlayer record NULL dereference";return false;}id=player->character660;e.clear();return true;};
 s.online5=[transport](auto& value,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto online=actual->application->get_online_loading_v55();if(!online){e="Required actual Trigger GetOnline";return false;}value=online->byte5()!=0;e.clear();return true;};
 s.app_delta_ms=[](auto& dt,auto& e){std::uint32_t value{};if(!borrow_application_dt_v93(value,e))return false;std::memcpy(&dt,&value,4);return true;};
 s.script_id=[transport](auto name,auto flag,auto& id,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto scripts=actual->application->source_script_manager_v52();if(!scripts||scripts->diagnostics().failed){e="Required actual Trigger ScriptManager";return false;}id=scripts->id_from_name(name,flag);e.clear();return true;};
 s.script_flags=[transport](auto id,auto& flags,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto scripts=actual->application->source_script_manager_v52();if(!scripts||id<0||std::size_t(id)>=scripts->commands().size()){e="Original Trigger script row index invalid";return false;}flags=scripts->commands()[id].skip4;e.clear();return true;};
 s.script_running=[transport](auto id,auto& running,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto scripts=actual->application->source_script_manager_v52();if(!scripts){e="Required actual Trigger IsScriptRunning";return false;}return scripts->is_script_running_v96(id,running,e);};
 s.start_script=[transport](auto id,auto room,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;auto scripts=actual->application->source_script_manager_v52();if(!scripts){e="Required actual Trigger StartScript";return false;}return scripts->start_script_v96(id,room,false,e);};
 s.effect_id=[transport](auto name,auto& id,auto& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e)||!actual->effects){e="Required actual FxNames";return false;}
  auto table=actual->effects->borrow();if(!table){e="Unloaded actual FxNames table";return false;}const auto& names=table.set_names();id=-1;for(std::size_t i=0;i<names.size();++i)if(names[i]==name){id=static_cast<std::int32_t>(i);break;}e.clear();return true;};
 s.find_named_object=[transport](auto name,auto module,auto create,auto& identity,auto& type,auto& e){
  SourceCampaignCandidateBorrowV55 current;if(!transport->candidate(current,e))return false;
  dh2::target_providers::Handle16 handle{};if(!current.objects->by_name(name,module,create,nullptr,handle,e))return false;
  const dh2::world::CanonicalObjectBorrowV1* object{};if(!current.objects->resolve_handle_v4(handle,false,object,{},e))return false;
  identity=0;type=0;if(object){if(!object->type_f4){e="Required SAME actual named GameObject.typeF4";return false;}identity=object->identity;type=*object->type_f4;}e.clear();return true;
 };
 auto fx=[transport](std::shared_ptr<void>& pin,dh2::fx::CharacterMeshFxOwnerV4*& manager,std::string& e){std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;return borrow_source_campaign_fx_v77(actual->owner,pin,manager,e);};
 s.create_marker=[fx](auto id,bool anchor,auto& marker,auto& e){if(anchor){e="Trigger GrabAnimFX expected genuine NULL anchor";return false;}std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};if(!fx(pin,manager,e)||!manager)return false;
  std::uintptr_t identity{};if(!manager->grab_marker_v28(id,0,identity,e))return false;marker={identity,identity?pin:std::shared_ptr<void>{}};e.clear();return true;};
 s.marker_owner=[fx](auto& marker,auto owner,auto& e){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};return marker.lease&&fx(pin,manager,e)&&manager&&pin==marker.lease&&manager->marker_store_anchor_v83(marker.identity,owner,e);};
 s.marker_restart=[fx](auto& marker,auto reset,auto& e){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};return marker.lease&&fx(pin,manager,e)&&manager&&pin==marker.lease&&manager->marker_sync_v83(marker.identity,reset,e);};
 s.marker_visible=[fx](auto& marker,auto visible,auto& e){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};return marker.lease&&fx(pin,manager,e)&&manager&&pin==marker.lease&&manager->marker_visible_v28(marker.identity,visible,e);};
 s.marker_loop=[fx](auto& marker,auto looping,auto& e){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};return marker.lease&&fx(pin,manager,e)&&manager&&pin==marker.lease&&manager->marker_loop_v76(marker.identity,looping,e);};
 s.marker_release=[fx](auto& marker,auto& e){std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};return marker.lease&&fx(pin,manager,e)&&manager&&pin==marker.lease&&manager->drop(marker.identity,e);};
 auto base=[transport,zone](std::shared_ptr<void>& lease,dh2::world::CanonicalGameObjectBaseOwnerV1*& receiver,std::string& e){SourceCampaignCandidateBorrowV55 current;if(!transport->candidate(current,e))return false;return borrow_source_campaign_object_base_v77(current,zone,lease,receiver,e)&&receiver&&receiver->identity()==zone;};
 s.touching=[base,common](auto peer,auto& result,auto& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* receiver{};return base(pin,receiver,e)&&dh2::world::zone_is_touching_v83(*receiver,peer,common,result,e);};
 auto positive_online_update=s.require_online_update;
 s.require_online_update=[transport,positive_online_update](auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  auto online=actual->application->get_online_loading_v55();if(!online){e="Required Trigger RequireOnlineUpdate.GetOnline";return false;}
  if(!online->byte5()){e.clear();return true;} //38b8cc..8f4 literal offline return
  if(!positive_online_update){e="Required positive RequireOnlineUpdate source100/119/hosting/virtual54";return false;}return positive_online_update(e);
 };
 // colzone384 belongs to the typed Trigger owner and is supplied by the
 // per-record bridge. Do not substitute a guessed base pointer-map cell.
 e.clear();return true;
}
bool bind_source_campaign_exit_services_v83(const SourceCampaignCandidateBorrowV55& candidate,
 const dh2::world::ZoneCollisionServicesV83& common,const dh2::world::TriggerZoneServicesV22& trigger,
 dh2::world::ExitZoneRuntimeServicesV83& s,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e))return false;
 auto transport=std::make_shared<CampaignZoneTransportV83>();transport->world=world;transport->level=candidate.level;
 s.collision=common;s.play_idle_sound=trigger.play_idle_sound;s.require_online_update=trigger.require_online_update;
 s.current_v114=[transport](dh2::world::CanonicalExitZoneV29& zone,std::string& error){
  SourceCampaignCandidateBorrowV55 current;std::shared_ptr<void> pin;
  dh2::world::CanonicalGameObjectBaseOwnerV1* actual{};
  if(!transport->candidate(current,error)||
     !borrow_source_campaign_object_base_v77(current,zone.base().identity(),pin,actual,error))return false;
  if(!pin||actual!=&zone.base()){error="Exit update receiver was replaced or retired";return false;}
  error.clear();return true;
 };
 s.update_idle_sound_v114=[transport](dh2::world::CanonicalExitZoneV29& zone,std::string& error){
  SourceCampaignCandidateBorrowV55 current;std::shared_ptr<void> pin;
  dh2::loader::GameObjectSourceFrameServicesV74 frame;
  if(!transport->candidate(current,error)||
     !borrow_source_campaign_frame_services_v106(current,zone.base().identity(),pin,frame,error))return false;
  auto* value=zone.base().integer(0x370);
  if(!pin||!value||!frame.idle_sound){error="Required actual Exit UpdateIdleSound frame provider";return false;}
  const auto bits=static_cast<std::uint16_t>(*value);std::int16_t sound{};
  std::memcpy(&sound,&bits,sizeof(sound));
  return frame.idle_sound(zone.base(),sound,error);
 };
 s.online=trigger.online5;s.virtual54=trigger.virtual54;
 auto positive_hosting=s.local_hosting;
 s.local_hosting=[transport,positive_hosting](auto& hosting,auto& e){
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e))return false;
  auto online=actual->application->get_online_loading_v55();if(!online){e="Required actual Exit hosting GetOnline";return false;}
  if(online->byte5()){if(!positive_hosting){e="Required positive COnline/CMatching/CNet hosting selector";return false;}return positive_hosting(hosting,e);}
  online=actual->application->get_online_loading_v55();if(!online||online->byte5()){e="Exit hosting changed online branch during repeated source query";return false;}
  hosting=true;e.clear();return true; //36f09c literal offline1
 };
 s.unlock_fasttravel=[transport](auto id,auto name,auto unlocked,auto difficulty,auto& e){
  SourceCampaignCharacterBorrowV62 c;if(!transport->character(id,c,e))return false;
  if(!c.character->save){e.clear();return true;} //3bbac0 real NULL-save return
  if(!c.character->save_fields||!c.character->save_fields->save_slot14e8()||*c.character->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(c.character->save.get())){e="Exit FastTravel Save14e8 authority mismatch";return false;}
  if(difficulty==-1){auto global=c.character->services.difficulty_global;if(!global){e="Required actual process CurrentDifficulty";return false;}difficulty=global->value();}
  std::shared_ptr<SourceWorldBorrowV61> actual;if(!transport->actual(actual,e)||!actual->design)return false;
  auto tables=actual->design->borrow();auto levels=tables.levels();if(!levels){e="Required SAME actual Arrays.FastTravel names";return false;}
  return c.character->save->source_set_fast_travel_v83(name,unlocked,difficulty,levels->travel_names,e);
 };
 // Borrow the actual process HUD generation/localization transport. The
 // provider resolves primary3/root140 afresh at each source invocation.
 return bind_native_zone_presentation_v83(candidate.actual_world,s,e);
}
}
