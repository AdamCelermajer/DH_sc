// Include inside model_renderer AFTER renderer_campaign_noncharacter_v69.inc.
// Global headers: campaign_zone_class_binding_v83.hpp, source_campaign_zones_v83.hpp,
// source_campaign_runtime_v61.hpp, renderer_campaign_declaration_arrays_v81.hpp,
// world_loot_gameplay_v23.hpp and <tuple>. No canonical receiver is constructed here.
struct CampaignNativeLendersV89 {
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::weak_ptr<dh2::world::CanonicalObjectManagerV1> objects;
 dh2::world::CanonicalPropertyMapV1* properties{};
 std::weak_ptr<SourceWorldBorrowV61> source;
 dh2::loader::GameObjectArraysOwnerV81::Borrow arrays;
 dh2::world::ZoneCollisionServicesV83 zones;
 std::shared_ptr<const std::vector<std::string>> sound_names;
 // Each positive external-script allocation is one private LuaScript VM;
 // these leases use the candidate's SAME Application LuaManager cache.
 std::map<std::uintptr_t,std::shared_ptr<dh2::scripts::GenericLuaScriptOwnerV13>> object_scripts;
 std::vector<std::weak_ptr<dh2::loader::AuxiliaryReleaseRecordV67>> records;
 bool current(SourceCampaignCandidateBorrowV55& out,std::string& e)const{
  auto w=world.lock();auto l=level.lock();auto m=objects.lock();
  if(!w||!l||!m||!borrow_source_campaign_candidate_v55(out,e)||out.actual_world.get()!=w.get()||
     out.level.get()!=l.get()||out.objects.get()!=m.get()||out.properties!=properties||
     w.owner_before(out.actual_world)||out.actual_world.owner_before(w)||l.owner_before(out.level)||out.level.owner_before(l)||
     m.owner_before(out.objects)||out.objects.owner_before(m)){if(e.empty())e="Changed SAME native non-Character lender scope";return false;}return true;
 }
 bool delivered(const dh2::loader::AuxiliaryDeliveryV67& d,std::string& e)const{
  SourceCampaignCandidateBorrowV55 actual;if(!current(actual,e))return false;
 if(d.actual_world.get()!=actual.actual_world.get()||d.level!=actual.level||d.objects!=actual.objects||d.properties!=actual.properties||
     d.actual_world.owner_before(actual.actual_world)||actual.actual_world.owner_before(d.actual_world)){e="Foreign auxiliary source delivery";return false;}return true;
 }
 bool destroy_object_lua_script(dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::uintptr_t identity,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;auto source_world=source.lock();auto* cell=base.pointer(0x300);
  if(!current(actual,e)||!source_world||!source_world->gameobject_graph_v68||!cell||*cell!=identity){if(e.empty())e="Changed SAME GameObject LuaScript300 D1 receiver/identity";return false;}
  std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* same{};
  if(!source_world->gameobject_graph_v68->borrow_base_v77(base.identity(),receiver,same,e)||same!=&base){if(e.empty())e="Foreign GameObject LuaScript300 D1 base";return false;}
  auto script=object_scripts.find(identity);if(script==object_scripts.end()||!script->second){e="Missing SAME retained object LuaScript C1 owner at GameObjectD1";return false;}
  // game_object_source_destroy_v72 stores +300=0 only after this D1 succeeds.
  if(!script->second->close_source_v88(e))return false;object_scripts.erase(script);e.clear();return true;
 }
 template<class Record>bool record(const char* name,std::uintptr_t id,std::shared_ptr<Record>& out,std::string& e)const{
  for(const auto& weak:records){auto journal=weak.lock();
   if(journal&&journal->class_name&&!std::strcmp(name,journal->class_name)&&journal->identity&&journal->identity()==id){
    auto actual=std::static_pointer_cast<Record>(journal->record);if(!actual||!actual->owner||actual->owner->base().identity()!=id){e="Changed actual native class journal receiver";return false;}
    out=std::move(actual);return true;}}
  e="Required SAME journal-admitted class record before typed native service preparation";return false;
 }
 template<class Getter>auto visual_asset(Getter get){
  auto dictionary=arrays.dictionary;
  return [get,dictionary](std::int32_t id,std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!dictionary||!get(pin,base,e)||!base)return false;const std::string* name{};
   if(!dictionary->file(id,name,e)||!name){if(e.empty())e="Required actual GameObjectDict visual CString";return false;}
   auto model=base->string(0x290);if(!model){e="Required SAME actual model290";return false;}*model=*name;return true;};
 }
 template<class Getter>auto load_object_script(Getter get){
  auto self=this;
  return [get,self](const char* name,const char* prefix,std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!get(pin,base,e)||!base)return false;auto lua=base->pointer(0x300);if(!lua){e="Required actual GameObject LuaScript300";return false;}
   // Original38ef60 destroys the previous LuaScript through vtable+4 and
   // clears+300 before even inspecting the new filename.
   if(*lua){auto old=self->object_scripts.find(*lua);if(old==self->object_scripts.end()||!old->second){e="Changed actual old object LuaScript300 owner";return false;}
    if(!old->second->close_source_v88(e))return false;self->object_scripts.erase(old);*lua=0;}
   if(!name||!*name){e.clear();return true;}
   SourceCampaignCandidateBorrowV55 actual;auto source=self->source.lock();
   if(!self->current(actual,e)||!source||!source->design||!source->level_application||!source->level_application->script_cache()){
    if(e.empty())e="Required SAME Application LuaManager/cache and design for object LoadExternalScript";return false;}
   auto design=std::make_shared<dh2::character::CharacterGameDesign::Borrow>(source->design->borrow());
   if(!design->design()){e="Required SAME actual design callbacks for object LuaScript";return false;}
   dh2::scripts::LuaScriptServicesV13 services;services.owner=design;services.design=*design->design();
   auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
   if(!world||!world->objects){e="Required SAME object-script UserData service owner";return false;}
   services.objects=&world->objects->services();
   std::string error;auto unique=dh2::scripts::GenericLuaScriptOwnerV13::create(false,source->level_application->script_cache(),std::move(services),16u*1024u*1024u,error);
   if(!unique){e="Actual object LuaScript C1: "+error;return false;}
   auto script=std::shared_ptr<dh2::scripts::GenericLuaScriptOwnerV13>(std::move(unique));
   const auto identity=script->identity();if(!identity||self->object_scripts.count(identity)){e="Duplicate/invalid actual object LuaScript allocation identity";return false;}
   // Source stores the C1 receiver at+300 before assigning its path or calling
   // Load. The lender map is its strong allocation lease, not another cache.
   self->object_scripts.emplace(identity,script);*lua=identity;
   auto retire=[&](std::string& failure){const bool closed=script->close_source_v88(failure);self->object_scripts.erase(identity);*lua=0;return closed;};
   if(!script->assign_path(prefix?prefix:"",error)){retire(error);e="Actual object LuaScript path: "+error;return false;}
   bool loaded{};if(!script->load(name,loaded,error)){retire(error);e="Actual object LuaScript Load: "+error;return false;}
   // Original38ef60 keeps+300 only for a true LuaScript::Load result. On
   // false,38f02c calls vptr+4 (LuaScript D0,37c148), which returns its
   // nonnull receiver in R0;38f030 clears+300 without changing that return.
   if(!loaded){if(!retire(error)){e="Actual object LuaScript D0 after false Load: "+error;return false;}
    e.clear();return true;}
   e.clear();return true;};
 }
 template<class Getter>auto object_script_call(Getter get){
  auto self=this;
  return [get,self](const char* name,std::uintptr_t actor,const char* event,std::string& e){
   SourceCampaignCandidateBorrowV55 actual;std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!self->current(actual,e)||!get(pin,base,e)||!base)return false;
   const auto* lua=base->pointer(0x300);auto script=lua?self->object_scripts.find(*lua):self->object_scripts.end();
   if(!lua||!*lua||script==self->object_scripts.end()||!script->second||script->second->identity()!=*lua){
    e="Required SAME retained Openable LuaScript300 Call receiver";return false;
   }
   //3a0afc: OnOpen pushes UserData only when opener398 is nonNULL.
   //3a0cb4: OnAnimEvent pushes the one actual event string.
   if(name&&!std::strcmp(name,"OnOpen")&&actor){auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
    std::uintptr_t character{};
    if(!world||!world->objects||!source_campaign_object_as_character_v114(actual.actual_world,actor,character,e)||
       character!=actor||!world->objects->find(actor)){if(e.empty())e="Required SAME published opener UserData receiver";return false;}
   }
   auto selected=script->second;
   return dh2::world::call_container_script_source_v126(*selected,name,actor,event,e);
  };
 }
};
static bool prepare_campaign_native_lenders_v89(const SourceCampaignCandidateBorrowV55& actual,
 RendererCampaignNonCharacterInputsV69& inputs,std::string& e){
 if(!actual.actual_world||!actual.level||!actual.objects||!actual.properties||!actual.roots||!actual.physical_world||
    !inputs.source.auxiliary.difficulty_names||!inputs.source.auxiliary.sound_names||
    !inputs.source.auxiliary.retain_release_record||!inputs.source.auxiliary.release_completed){
  e="Required actual candidate, immutable names and real native release journal before whole lender preparation";return false;}
 auto provider=std::make_shared<CampaignNativeLendersV89>();provider->world=actual.actual_world;
 provider->level=actual.level;provider->objects=actual.objects;provider->properties=actual.properties;
 std::shared_ptr<SourceWorldBorrowV61> source;
 if(!borrow_source_campaign_condition_world_v70(actual,source,e)||!source||!source->read||!source->files_owner)return false;
 provider->source=source;provider->sound_names=inputs.source.auxiliary.sound_names;
 if(!source->noncharacter_release_provider_v92){e="Required same source-qualified GameObjectD1 owner for LuaScript300";return false;}
 {
  auto release=source->noncharacter_release_provider_v92;auto prior=release->native.gameobject.destroy_lua_script300;
  std::weak_ptr<CampaignNativeLendersV89> weak=provider;
  release->native.gameobject.destroy_lua_script300=[weak,prior](auto& base,std::uintptr_t identity,std::string& e){
   auto self=weak.lock();if(!self){e="Retired SAME object LuaScript300 lender";return false;}
   if(self->object_scripts.count(identity))return self->destroy_object_lua_script(base,identity,e);
   if(prior)return prior(base,identity,e);
   e="Required actual LuaScript300 D1 owner";return false;
  };
 }
 if(!borrow_source_campaign_game_object_arrays_v81(actual,provider->arrays,e))return false;
 auto prepared=inputs;prepared.declaration_arrays=provider->arrays;
 provider->zones=prepared.zone_collision;
 if(!provider->zones.peer)provider->zones.peer=[provider](std::uintptr_t id,dh2::world::ZonePeerBorrowV83& out,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;auto source=provider->source.lock();
  if(!provider->current(actual,e)||!source||!source->gameobject_graph_v68){if(e.empty())e="Required actual native GameObject graph for Zone peer";return false;}
  auto graph=source->gameobject_graph_v68;std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!graph->borrow_base_v77(id,receiver,base,e)||!base)return false;
  out={};out.receiver=std::move(receiver);out.identity=id;out.position160=base->vector3(0x160);out.absolute12c=base->absolute_aabb12c();out.physical2dc=base->pointer(0x2dc);
  out.physical_radius=[weak=std::weak_ptr<dh2::world::CanonicalGameObjectGraphV68>(graph),id](float& radius,std::string& e){
   auto graph=weak.lock();std::shared_ptr<void> pin;dh2::physical::NativeBody* body{};
   if(!graph||!graph->borrow_native_body_v90(id,pin,body,e)||!body)return false;radius=body->radius;return true;};
  e.clear();return true;};
 if(!bind_source_campaign_zone_services_v83(actual,provider->zones,e))return false;
 prepared.zone_collision=provider->zones;
 if(!prepared.zone_physical.owner)prepared.zone_physical.owner=provider;
 if(!prepared.zone_physical.collision_test_notification)prepared.zone_physical.collision_test_notification=dh2::world::zone_collision_notification_v83;
 if(!prepared.zone_physical.contact)prepared.zone_physical.contact=[provider](auto& base,dh2::physical::ContactEvent event,std::uintptr_t peer,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;if(!provider->current(actual,e))return false;
  if(!base.class_name20()){e="Unproduced actual Zone class20";return false;}
  if(event==dh2::physical::ContactEvent::persist||event==dh2::physical::ContactEvent::result){e.clear();return true;} //395944/39649c literal void paths
#define NATIVE_ZONE_CASE(name,Record,...) if(!std::strcmp(base.class_name20(),name)){std::shared_ptr<dh2::loader::Record> r;if(!provider->record(name,base.identity(),r,e)||&r->owner->base()!=&base)return false;__VA_ARGS__;}
  NATIVE_ZONE_CASE("CheckpointZone",CanonicalCheckpointRecordV26,if(event==dh2::physical::ContactEvent::add)return dh2::world::checkpoint_collision_begin_v83(*r->owner,peer,provider->zones,e);e.clear();return true)
  NATIVE_ZONE_CASE("QuestMoveInZone",CanonicalQuestMoveRecordV31,if(event==dh2::physical::ContactEvent::add)return dh2::world::quest_move_collision_begin_v83(*r->owner,peer,provider->zones,e);e.clear();return true)
  NATIVE_ZONE_CASE("TriggerZone",CanonicalTriggerZoneRecordV22,return event==dh2::physical::ContactEvent::add?r->owner->collision_begin(peer,e):r->owner->collision_end(peer,e))
  NATIVE_ZONE_CASE("TriggerZoneExitLevel",CanonicalExitZoneRecordV29,return event==dh2::physical::ContactEvent::add?r->owner->collision_begin(peer,e):r->owner->collision_end(peer,e))
  NATIVE_ZONE_CASE("TriggerObject",CanonicalTriggerObjectRecordV28,
   auto* characters=r->owner->source_integer(0x3a0);auto* players=r->owner->source_integer(0x3a4);
   if(!characters||!players){e="Required SAME ZoneEx counters3a0/3a4";return false;}
   auto inside=[r,provider](auto id,auto& result,std::string& e){return dh2::world::zone_is_inside_v83(r->owner->base(),r->owner->source_colzone384(),id,provider->zones,result,e);};
   return dh2::world::zone_ex_contact_v90(r->owner->source_contacts(),*characters,*players,peer,event==dh2::physical::ContactEvent::add,inside,provider->zones.is_character,provider->zones.is_player,e))
  NATIVE_ZONE_CASE("Door",CanonicalDoorRecordV27,e.clear();return true) //397ca0 parent is Zone, literal begin/end
#undef NATIVE_ZONE_CASE
  e="Unrecovered actual Zone contact class";return false;
 };
 auto& auxiliary=prepared.source.auxiliary;auxiliary.owner=provider;
 auto admission=auxiliary.retain_release_record;
 auxiliary.retain_release_record=[provider,admission](const auto& d,const auto& record,const auto& request,std::string& e){
  if(!provider->delivered(d,e)||!admission(d,record,request,e))return false;
  // Observer of Main's existing admission only. Main owns the strong prefix;
  // this directory is weak and never creates another registry or C1.
  provider->records.push_back(record);return true;};
 auto make_lender=[provider](auto original,const char* name,auto record_tag){
  using Record=typename std::decay_t<decltype(record_tag)>::element_type;
  return [provider,original,name](const auto& d,auto& receiver,auto& out,std::string& e){
   if(!provider->delivered(d,e))return false;
   std::shared_ptr<Record> record;if(!provider->record(name,receiver.base().identity(),record,e)||record->owner.get()!=&receiver)return false;
   if(original&&!original(d,receiver,out,e))return false;out.owner=provider;
   auto source=provider->source.lock();if(!source||!source->noncharacter_release_provider_v92){e="Required same source-qualified class release provider";return false;}
   bind_auxiliary_gameobject_destroy_v92(std::weak_ptr<CampaignNonCharacterReleaseV92>(source->noncharacter_release_provider_v92),out);
   using Services=std::decay_t<decltype(out)>;std::weak_ptr<Record> weak=record;
   if constexpr(std::is_same_v<Services,dh2::world::AnimatedDecorServicesV23>){out.decor.owner=provider;out.source_init.owner=provider;}
   if constexpr(std::is_same_v<Services,dh2::world::CheckpointZoneServicesV26>){
    out.startup.owner=provider;if(!out.whole_collision_begin&&!bind_campaign_checkpoint_record_v83(weak,provider->zones,out,e))return false;
   }
   if constexpr(std::is_same_v<Services,dh2::world::QuestMoveZoneServicesV31>){
    out.startup.owner=provider;if(!out.whole_collision_begin&&!bind_campaign_quest_move_record_v83(weak,provider->zones,out,e))return false;
   }
   if constexpr(std::is_same_v<Services,dh2::world::TriggerZoneServicesV22>){
    SourceCampaignCandidateBorrowV55 actual;if(!provider->current(actual,e)||!bind_campaign_trigger_record_v83(actual,weak,provider->zones,out,e))return false;
   }
   if constexpr(std::is_same_v<Services,dh2::world::ExitZoneServicesV29>){
    out.trigger.owner=provider;SourceCampaignCandidateBorrowV55 actual;
    if(!provider->current(actual,e)||!bind_campaign_trigger_record_v83(actual,weak,provider->zones,out.trigger,e))return false;
    if(!out.level_name_id)out.level_name_id=[provider](const char* name,std::int32_t& id,std::string& e){
     auto source=provider->source.lock();SourceCampaignCandidateBorrowV55 actual;if(!provider->current(actual,e)||!source||!source->design||!name)return false;
     const auto tables=source->design->borrow();const auto* levels=tables.levels();if(!levels){e="Required SAME actual Arrays.LevelList";return false;}
     id=-1;for(std::size_t i=0;i<levels->level_names.size();++i)if(levels->level_names[i]==name){id=static_cast<std::int32_t>(i);break;}e.clear();return true;};
    if(!out.whole_update){dh2::world::ExitZoneRuntimeServicesV83 runtime;
     if(!bind_source_campaign_exit_services_v83(actual,provider->zones,out.trigger,runtime,e)||!bind_campaign_exit_record_v83(weak,runtime,out,e))return false;}
   }
   if constexpr(std::is_same_v<Services,dh2::world::SpawnPointServicesV15>){
    if(!out.start_script)out.start_script=[provider](auto id,auto room,auto flag,std::string& e){
     SourceCampaignCandidateBorrowV55 actual;if(!provider->current(actual,e))return false;
     auto scripts=actual.application?actual.application->source_script_manager_v52():nullptr;
     if(!scripts){e="Required SAME App ScriptManager for SpawnPoint";return false;}return scripts->start_script_v96(id,room,flag,e);};
   }
   if constexpr(std::is_same_v<Services,dh2::world::SoundEmitterServicesV32>)out.sound_names=provider->sound_names;
   if constexpr(std::is_same_v<Services,dh2::world::DoorServicesV27>||std::is_same_v<Services,dh2::world::TriggerObjectServicesV28>){
    out.startup.owner=provider;out.startup.zone.owner=provider;
   }
    if constexpr(std::is_same_v<Services,dh2::world::DoorServicesV27>){
     if(!out.startup.apply_mesh_box)out.startup.apply_mesh_box=[provider](auto& door,auto visual_id,std::string& e){
     SourceCampaignCandidateBorrowV55 actual;auto source=provider->source.lock();if(!provider->current(actual,e)||!source||!source->gameobject_graph_v68)return false;
     std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
     if(!source->gameobject_graph_v68->borrow_base_v77(door.base().identity(),pin,base,e)||base!=&door.base()||*base->pointer(0x2d8)!=visual_id)return false;
     std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
     return source->gameobject_graph_v68->borrow_visual(visual_id,visual,e)&&visual&&visual->apply_mesh_box(e);};
   }
   if constexpr(std::is_same_v<Services,dh2::world::TriggerObjectServicesV28>){
    if(!out.startup.load_external_script)out.startup.load_external_script=[provider,weak](auto& trigger,const char* name,const char* prefix,std::string& e){
     auto actual=weak.lock();if(!actual||!actual->owner||actual->owner.get()!=&trigger){e="Released SAME TriggerObject external script receiver";return false;}
     auto get=[weak](auto& pin,auto*& base,std::string& e){auto r=weak.lock();if(!r||!r->owner){e="Released SAME TriggerObject C1";return false;}pin=r;base=&r->owner->base();return true;};
     return provider->load_object_script(get)(name,prefix,e);};
   }
   e.clear();return true; // typed service preparation only; missing engine methods stay absent
  };
 };
#define REAL_NATIVE_LENDER(field,name,record) auxiliary.field=make_lender(auxiliary.field,name,std::shared_ptr<dh2::loader::record>{})
 REAL_NATIVE_LENDER(dummy,"Dummy",CanonicalDummyRecordV16);
 REAL_NATIVE_LENDER(spawn_point,"SpawnPoint",CanonicalSpawnPointRecordV16);
 REAL_NATIVE_LENDER(decor,"Decor",CanonicalDecorRecordV16);
 REAL_NATIVE_LENDER(animated_decor,"AnimatedDecor",CanonicalAnimatedDecorRecordV23);
 REAL_NATIVE_LENDER(trigger_zone,"TriggerZone",CanonicalTriggerZoneRecordV22);
 REAL_NATIVE_LENDER(checkpoint,"CheckpointZone",CanonicalCheckpointRecordV26);
 REAL_NATIVE_LENDER(door,"Door",CanonicalDoorRecordV27);
 REAL_NATIVE_LENDER(trigger_object,"TriggerObject",CanonicalTriggerObjectRecordV28);
 REAL_NATIVE_LENDER(exit_zone,"TriggerZoneExitLevel",CanonicalExitZoneRecordV29);
 REAL_NATIVE_LENDER(quest_move,"QuestMoveInZone",CanonicalQuestMoveRecordV31);
 REAL_NATIVE_LENDER(sound,"SoundEmitter",CanonicalSoundEmitterRecordV32);
#undef REAL_NATIVE_LENDER
 auto& containers=prepared.source.containers;
 const auto same=[](const auto& a,const auto& b){return a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if((containers.openable_table&&!same(containers.openable_table,provider->arrays.openable))||
    (containers.destructible_table&&!same(containers.destructible_table,provider->arrays.destructible))){e="Competing container Arrays snapshot";return false;}
 containers.openable_table=provider->arrays.openable;containers.destructible_table=provider->arrays.destructible;
 auto openable=containers.openable_services;
 containers.openable_services=[provider,openable](const auto& scope,const auto& request,const auto& slot,auto& out,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;if(!provider->current(actual,e))return false;
  if(openable&&!openable(scope,request,slot,out,e))return false;
  out.world=provider;out.roots=actual.roots;out.physics=actual.physical_world.get();
  out.visual.owner=provider;out.initialization.owner=provider;out.position.owner=provider;out.decor.owner=provider;out.container.owner=provider;
  if(!out.visual.read_asset)out.visual.read_asset=[provider](const auto& path,auto& bytes,bool& found,std::string& e){
   SourceCampaignCandidateBorrowV55 actual;auto source=provider->source.lock();return provider->current(actual,e)&&source&&source->read&&source->read(path,found,bytes,e);};
  auto get=[slot](auto& pin,auto*& base,std::string& e){auto actual=slot?slot->lock():nullptr;if(!actual){e="Unproduced SAME Openable C1";return false;}pin=actual;base=&actual->base();return true;};
  if(!out.container.visual_asset)out.container.visual_asset=provider->visual_asset(get);
  if(!out.container.meet_condition)out.container.meet_condition=dh2::world::game_object_meet_condition_v1;
  if(!out.container.load_object_script)out.container.load_object_script=provider->load_object_script(get);
  if(!out.container.has_script)out.container.has_script=[get](bool& present,std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!get(pin,base,e))return false;auto lua=base->pointer(0x300);if(!lua){e="Required SAME LuaScript300";return false;}present=*lua!=0;return true;};
  if(!out.container.script_call)out.container.script_call=provider->object_script_call(get);
  e.clear();return true;
 };
 auto destructible=containers.destructible_services;
 containers.destructible_services=[provider,destructible](const auto& scope,const auto& request,const auto& slot,auto& init,auto& out,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;if(!provider->current(actual,e))return false;
  if(destructible&&!destructible(scope,request,slot,init,out,e))return false;
  init.owner=provider;out.owner=provider;out.common.owner=provider;out.table=provider->arrays.destructible;
  auto get=[slot](auto& pin,auto*& base,std::string& e){auto actual=slot?slot->lock():nullptr;if(!actual){e="Unproduced SAME Destructible C1";return false;}pin=actual;base=&actual->base();return true;};
  if(!out.common.visual_asset)out.common.visual_asset=provider->visual_asset(get);
  if(!out.common.load_object_script)out.common.load_object_script=provider->load_object_script(get);
  if(!out.common.has_script)out.common.has_script=[get](bool& present,std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!get(pin,base,e))return false;auto lua=base->pointer(0x300);if(!lua){e="Required SAME LuaScript300";return false;}present=*lua!=0;return true;};
  e.clear();return true;
 };
 // Main's ONE pre-XML prepared owner is optional until an Item is reached.
 // Publish only its alias; constructing/spawning145 remains actual stage29.
 if(auto prepared_items=source->prepared_items_v88;prepared_items&&prepared_items->factory_prepared_v88()&&prepared_items->items()){
  const auto& factory=prepared_items->items()->factory();
  if(&factory.source_manager_v57()!=actual.objects.get()||&factory.source_property_map_v57()!=actual.properties){e="Prepared Item owner belongs to a different source manager/property map";return false;}
  auto alias=std::shared_ptr<dh2::character::WorldItemLiveOwnerV5>(prepared_items,prepared_items->items());
  auto prior=containers.actual_items.lock();if(prior&&!same(prior,alias)){e="Competing source Item pool owner";return false;}
  containers.actual_items=alias;
 }
 // No prepared owner means no advertised Item entry; reached Item remains an
 // explicit factory failure. Never create a pool or run PreCache in this binder.
 inputs=std::move(prepared);e.clear();return true;
}
