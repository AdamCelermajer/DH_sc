// Include after RendererCharacterCandidateV60 and RendererPlayerSpawnV70.
// One delivery owns the original AddCharacter attempt and its failed prefix;
// it never derives Character660 or PM6c4 from a renderer-ready flag.
class RendererPlayerAddCharacterV70 final {
 std::weak_ptr<RendererCharacterCandidateV60> transport_;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 std::weak_ptr<void> world_;
 std::map<std::int32_t,std::unique_ptr<RendererPlayerSpawnV70>> spawning_;
 std::unique_ptr<dh2::player::PlayerManagerCharacterDeliveryV70> delivery_;
 std::function<bool(const dh2::world::CanonicalObjectBorrowV1&,std::string&)> light_final_;
 std::function<bool(const dh2::player::PlayerAddRequestV5&,dh2::player::PlayerAddResponseV5&,std::string&)> source_tail_;
 bool invoke(const dh2::player::PlayerAddRequestV5& q,dh2::player::PlayerAddResponseV5& out,std::string& e){
  using O=dh2::player::PlayerAddOperationV5;
  auto transport=transport_.lock();auto app=application_.lock();auto pm=app?app->source_player_manager_v59():nullptr;
  if(!transport||!app||!pm){e="Released SAME canonical AddCharacter transport";return false;}
  const auto online=app->get_online_loading_v55();
  if(q.operation==O::online){if(!online){e="Required original AddCharacter GetOnline";return false;}out.value=online->byte5()!=0;return true;}
  if(q.operation==O::spawn){
   model_renderer::SourceCampaignCandidateBorrowV55 candidate;
   auto actual_world=world_.lock();
   if(!actual_world||!model_renderer::borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world.get()!=actual_world.get()){
    if(e.empty())e="Required SAME current candidate for source player Spawn";return false;
   }
   auto& retained=spawning_[q.argument];if(retained){e="Source player Spawn prefix cannot replay";return false;}
   retained=std::make_unique<RendererPlayerSpawnV70>(transport,candidate);
   return retained->spawn(q.name,out.identity,e);
  }
  if(q.operation==O::is_local||q.operation==O::is_active||q.operation==O::get_slots||q.operation==O::get_levels){
   // The original request carries the SAME PlayerInfo pointer. Resolve it
   // through the network owner before using it; never accept another record.
   auto* info=reinterpret_cast<dh2::player::PlayerInfoFieldsV1*>(q.subject);
   if(!info||!pm->network()->borrow(*info)){e="Required SAME AddCharacter PlayerInfo receiver";return false;}
   bool value{};
   if(q.operation==O::is_local){if(!pm->network()->is_local(*info,value,e))return false;out.value=value;return true;}
   if(q.operation==O::is_active){if(!online||!pm->network()->is_active_selected_v67(*info,online->byte5(),value,e))return false;out.value=value;return true;}
   std::shared_ptr<dh2::player::PlayerInfoSkillBuffersV26> buffers;
   if(!pm->skill_buffers(*info,buffers,e)||!buffers)return false;
   return q.operation==O::get_slots?buffers->get_slots(*info,q.buffer,q.size,out.value,e):buffers->get_levels(*info,q.buffer,q.size,out.value,e);
  }
  if(q.operation==O::current_level){dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!model_renderer::borrow_current_native_level_v27(current,e))return false;
   out.identity=current?current.identity():0;return true;
  }
  if(q.operation==O::quick_save){
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!model_renderer::borrow_current_native_level_v27(current,e)||!current||current.identity()!=q.subject){if(e.empty())e="Required SAME current Level for AddCharacter QuickSave";return false;}
   // Source QuickSave always performs the actual local0,true query BEFORE
   // inspecting Save_ec/phase130. Startup phase6 is a genuine early return.
   dh2::player::PlayerInfoFieldsV1* local{};
   if(!pm->manager()->get_local_player(0,true,local,e)||!local){if(e.empty())e="Required QuickSave source local PlayerInfo";return false;}
   const auto& fields=current.level()->constructor_fields_v3();
   if(!fields.save_ec||fields.field130!=38||!local->character660)return true;
   if(!source_tail_){e="Required positive Level QuickSave checkpoint/save receiver";return false;}return source_tail_(q,out,e);
  }
  if(q.operation==O::attach_light){
   // AttachLight371050 ignores the internal-id argument and captures the
   // actual ordered LightPoint list before invoking any InitFinal callback.
   model_renderer::SourceCampaignCandidateBorrowV55 candidate;auto world=world_.lock();
   if(!world||!model_renderer::borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world.get()!=world.get()||!candidate.objects){if(e.empty())e="Required SAME source ObjectManager light list";return false;}
   std::vector<dh2::world::CanonicalObjectBorrowV1> lights;
   std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* entry{};
   for(bool have=candidate.objects->source_ordered_begin_v38(key,entry);have;have=candidate.objects->source_ordered_next_v38(key,key,entry)){
    if(!entry)continue;
    if(!entry->class_name20||!*entry->class_name20||!entry->lease){e="Required actual source receiver class20 for LightBaseList";return false;}
    if(!std::strcmp(*entry->class_name20,"LightPoint"))lights.push_back(*entry);
   }
   for(const auto& light:lights){if(!light_final_){e="Required actual LightPoint virtual58 InitFinal";return false;}if(!light_final_(light,e))return false;}
   return true;
  }
  auto record=transport->actor(q.subject);
  switch(q.operation){
   case O::initialize_save:return transport->initialize_player_save(q.subject,e);
   case O::init_all:return transport->initialize_all_v70(q.subject,e);
   case O::init_camera:{auto world=world_.lock();if(!world){e="Released SAME World before InitCam";return false;}return model_renderer::source_campaign_character_init_camera_v75(world,q.subject,e);}
   case O::set_slot:
    if(!record||!record->save){e="Required actual Character14e8 before SetSlot";return false;}record->save->set_slot(q.argument);return true;
   case O::set_class:
    if(!record||!record->save){e="Required actual Character14e8 before SetPlayerClass";return false;}record->save->set_player_class(q.argument);return true;
   case O::skill_count:
    if(!record||!record->save){e="Required actual Character14e8 skill count";return false;}out.value=static_cast<std::int32_t>(record->save->skills().size());return true;
   case O::set_skill_level:
    if(!record||!record->save){e="Required actual Character14e8 before SetSkillLevel";return false;}
    return record->save->set_skill_level(static_cast<std::uint32_t>(q.argument),q.secondary,e);
   case O::set_skill_slot:{
    if(!record||!record->save){e="Required actual Character14e8 before SetSkillInSlot";return false;}
    const dh2::data::SavedSkillUpdateServicesV1 update{record.get(),[](void* context,std::uintptr_t character,std::string& error){
     auto& same=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(context);
     if(!same.actor||same.actor->object->identity!=character||!same.player_script_owner_v62){error="Required SAME CharAI mapped-skill update receiver";return false;}
     if(same.player_script_owner_v62->native_update_mapped_skills_v70()<0){error=same.player_script_owner_v62->error();return false;}return true;
    }};
    return record->save->set_skill_in_slot(q.argument,static_cast<std::uint32_t>(q.secondary),update,e);
   }
   case O::set_visible:{
    if(!record||!record->services.inherited){e="Required SAME GameObject SetVisible receiver";return false;}
    dh2::world::GameObjectInitializationServicesV1 inherited;
    if(!record->services.inherited(*record,inherited,e))return false;
    if(!inherited.set_visible){e="Required actual GameObject SetVisible body";return false;}
    return inherited.set_visible(q.argument!=0,e);
   }
   case O::set_idle:
    if(!record||!record->actor||!record->actor->machine){e="Required SAME Character FSM before SetIdle";return false;}
    // 3c1a00 stores the caller's byte3c before _SetState(3,-1,NULL).
    record->actor->machine->state().idle_suppressed=static_cast<std::uint8_t>(q.argument);
    if(record->actor->machine->transition(3,-1,0)<0){e=record->actor->machine->error();return false;}return true;
   case O::attach_controller:{
    dh2::player::PlayerInfoFieldsV1* info{};
    if(!pm->get_by_internal(q.argument,false,info,e)||!info||info->character660!=q.subject||!record||!record->actor||!record->actor->controller){if(e.empty())e="Required SAME source Character for local controller attachment";return false;}
    if(!info->local66c){e="Required original remote NetController constructor";return false;}
    if(record->player_controllers_v70){e="Character controller attachment prefix cannot replay";return false;}
    record->player_controllers_v70=std::make_unique<dh2::player::PlayerControllerAttachmentV70>();
    dh2::player::PlayerControllerAttachmentServicesV70 services;services.events=app->events14();
    services.online=[online](bool& value,std::string& error){if(!online){error="Required original controller attachment GetOnline";return false;}value=online->byte5()!=0;return true;};
    // Source Emu event bodies are separate required endpoints. Construction
    // attaches the actual listeners; it never consumes invented input events.
    const auto controllable=reinterpret_cast<std::uintptr_t>(&record->controllable374_v70);
    if(!record->player_controllers_v70->construct(q.subject,controllable,info->controller668,std::move(services),
      [record](auto& command,std::uintptr_t actual_controllable,bool,std::string& error){
       if(actual_controllable!=reinterpret_cast<std::uintptr_t>(&record->controllable374_v70)||!record->actor->controller){error="Required SAME Controllable374 SetController receiver";return false;}
       record->controllable374_v70.controller378=command.controller;
       record->actor->controller->source_set_controller_v70(&command);return true;
      },e))return false;
    return true;
   }
   default:break;
  }
  // Remaining source bodies are not optional: Visible must alter the real visual/PF owner;
  // QuickSave, controller and LightPoint.InitFinal preserve their own tails.
  if(!source_tail_){e="Required canonical AddCharacter source body "+std::to_string(std::uint32_t(q.operation));return false;}
  return source_tail_(q,out,e);
 }
public:
 bool initialize(const std::shared_ptr<RendererCharacterCandidateV60>& transport,
  const model_renderer::SourceCampaignCandidateBorrowV55& candidate,
  std::function<bool(const dh2::player::PlayerAddRequestV5&,dh2::player::PlayerAddResponseV5&,std::string&)> source_tail,
  std::string& e,std::function<bool(const dh2::world::CanonicalObjectBorrowV1&,std::string&)> light_final={}){
  auto pm=candidate.application?candidate.application->source_player_manager_v59():nullptr;
  if(delivery_||!transport||!transport->belongs_to_v62(candidate.actual_world)||!pm||!pm->manager()){e="Required once-only SAME PM AddCharacter binding";return false;}
  transport_=transport;application_=candidate.application;world_=candidate.actual_world;source_tail_=std::move(source_tail);
  light_final_=std::move(light_final);
  dh2::player::PlayerAddServicesV5 services;
  // Pin the real callback transport descriptor, without a World/App backedge.
  services.provider_lease=std::make_shared<std::weak_ptr<RendererCharacterCandidateV60>>(transport);
  services.invoke=[this](const auto& q,auto& out,auto& error){return invoke(q,out,error);};
  std::weak_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> weak_pm=pm;
  services.network_fields=[weak_pm](auto& info,auto& out,auto& error){auto same=weak_pm.lock();auto fields=same?same->network()->borrow(info):nullptr;
   if(!fields||!fields->character_class_produced_v68||!fields->managed_fields_produced_v70){error="Required SAME produced AddCharacter NetStruct fields";return false;}
   out.player_class380=&fields->character_class380_v68;out.visible4e5=&fields->character_visible4e5_v70;return true;};
  services.character=[weak=transport_](std::uintptr_t id,auto& out,auto& error){auto t=weak.lock();auto r=t?t->actor(id):nullptr;
   if(!r){error="Required source-spawned canonical AddCharacter receiver";return false;}return r->player_add_borrow_v70(out,error);};
  delivery_=std::make_unique<dh2::player::PlayerManagerCharacterDeliveryV70>(*pm->manager(),std::move(services));return true;
 }
 bool deliver(const dh2::player::PlayerManagerRequestV1& q,dh2::player::PlayerManagerResponseV1& out,std::string& e){
  if(!delivery_){e="Required constructed canonical AddCharacter delivery";return false;}return delivery_->deliver(q,out,e);
 }
};
