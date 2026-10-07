#include "canonical_character_save_v86.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "character_ai_groups_v87.hpp"
#include "source_assertion_process_v76.hpp"
#include "../game-data/class_tables.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::character {
bool destroy_canonical_character_save_v108(world::CanonicalCharacterCandidateRecordV60& r,std::uintptr_t captured,std::string& e){
 if(!captured||!r.save_fields||!r.save||!r.load||captured!=*r.save_fields->save_slot14e8()||
    captured!=reinterpret_cast<std::uintptr_t>(r.save.get())||&r.load->save()!=r.save.get()){
  e="Character Save D0 requires SAME actual14e8/Save/load authority";return false;
 }
 const auto bootstrap=r.profile_bootstrap;
 const auto profile=bootstrap?bootstrap->profile():nullptr;
 const auto quests=bootstrap?bootstrap->quest_owner_v70():nullptr;
 if(bootstrap&&(bootstrap->save().get()!=r.save.get()||bootstrap->load_owner().get()!=r.load.get())){
  e="Character Save D0 selected profile belongs to another Save";return false;
 }
 if(!r.load->destroy_source_v108([profile](const data::PlayerSaveProfileV1& actual,std::string& e){
  if(!profile||actual.identity!=reinterpret_cast<std::uintptr_t>(profile.get())||actual.owner.get()!=profile.get()||
     actual.owner.owner_before(profile)||profile.owner_before(actual.owner)){
   e="Positive Save+8 D0 requires actual selected CampaignSaveProfile";return false;
  }
  return profile->destroy_source_v108(e);
 },[quests,save=r.save](std::uint32_t selector,std::string& e){
  if(quests){if(quests->save().get()!=save.get()){e="Quest D1 belongs to another Save";return false;}return quests->destroy_collection_v108(selector,e);}
  auto& collection=selector?save->volatile_quests_v45():save->regular_quests_v45();
  return collection.source_destroy_empty_v108(e);
 },e))return false;
 if(!r.save_fields->source_save_destroyed_null_store_v108(captured,e))return false;
 //These host loans may pin logically destroyed receivers; they do not stand
 //in for source D0. Retire them only after the actual ordered body and NULL.
 r.profile_bootstrap.reset();r.quest_sync_owner.reset();r.load.reset();r.save.reset();e.clear();return true;
}
namespace {
bool missing(const char* leaf,std::string& e){if(e.empty())e=std::string("Required SAME canonical Character restore ")+leaf;return false;}
bool ai(world::CanonicalCharacterCandidateRecordV60& r,const data::AiProps*& out,std::string& e){
 NpcInitPostResponseV1 response;
 if(!world::CanonicalCharacterCandidateRecordV60::init_service(&r,{0x3a2fec,0,0,r.actor->object->identity,0,nullptr},response,e))return false;
 out=response.ai;return out?true:missing("GetCharType/AiProps",e);
}
}
CanonicalCharacterSaveV86::CanonicalCharacterSaveV86(std::weak_ptr<world::CanonicalCharacterCandidateRecordV60> r,CanonicalCharacterSaveServicesV86 s):record_(std::move(r)),services_(std::move(s)),buff_services_{this,buff}{}
CanonicalCharacterSaveV86::~CanonicalCharacterSaveV86(){std::string ignored;close(ignored);}
BuffOwner* CanonicalCharacterSaveV86::buffs()noexcept{
 auto r=record_.lock();return r&&r->player_script_owner_v62?r->player_script_owner_v62->native_buffs():npc_buffs_;
}
bool CanonicalCharacterSaveV86::borrow_buffs(BuffOwner*& out,std::string& e){
 RetainedCharacterSaveExtraV3 fields;if(!extra(fields,e))return false;
 out=fields.same_buffs;return out?true:missing("actual CharProperties buff owner before Lua/timer publication",e);
}
int CanonicalCharacterSaveV86::buff(void* raw,data::PropertyView* view,const BuffRequest32* request,std::uintptr_t* out){
 auto& self=*static_cast<CanonicalCharacterSaveV86*>(raw);auto r=self.record_.lock();
 if(!r||!request||!out||!r->actor||!r->actor->session||view!=&r->actor->session->property_view()||request->character!=r->actor->object->identity)return 0;
 if(request->service==buff_recalculate){
  const auto* classes=r->design.class_rows();if(!classes)return 0;
  //Whole class-to-base/all-property resolver over the live buff groups. Do
  //not recalc a copied PropertyState with its own empty group projection.
  const auto status=dh2_class_recalc_base(classes->data(),static_cast<std::uint32_t>(classes->size()),r->properties->base.data(),view);
  r->view.groups=view->groups;r->view.group_count=view->group_count;
  return status?0:1;
 }
 if(request->service==buff_fx_release&&!request->subject)return 1; //494978 NULL early return
 return self.services_.effects.invoke?self.services_.effects.invoke(self.services_.effects.context,view,request,out):0;
}
bool CanonicalCharacterSaveV86::extra(RetainedCharacterSaveExtraV3& out,std::string& e){
 auto r=record_.lock();if(!r||!r->actor||!r->actor->object)return missing("receiver lifetime",e);
 world::GameObjectInitializationFieldsV62 f;
 if(!r->actor->inherited_initialization_fields_v62(r,f,e))return false;
 out={};out.inherited={f.source_identity,f.byte(0x80),f.byte(0x8a),f.byte(0xac),f.byte(0xd0),f.integer(0x270),f.pointer(0x2d8)};
 if(!out.inherited.visible80||!out.inherited.enabled8a||!out.inherited.tested_ac||!out.inherited.tested_d0||!out.inherited.archetype270||!out.inherited.visual2d8)return missing("inherited real serialization cells",e);
 out.metadata=&metadata_;out.same_buffs=buffs();
 if(!out.same_buffs&&r->actor->session){
  auto& session=*r->actor->session;
  if(session.properties()!=r->properties||session.combat_state()!=r->life)return missing("NPC property/timer authority",e);
  buff_bindings_={&session.property_view(),&session.timers(),&session.native_timer_services(),&buff_services_};
  npc_buffs_=dh2_character_buffs_create(&buff_bindings_);
  if(!npc_buffs_)return missing("sole NPC CharProperties buff constructor",e);
  out.same_buffs=npc_buffs_;
 }
 if(r->actor->controller){
  ControllerCommandState32 current;
  if(!r->services.controller||!r->services.controller(current,*r,e))return missing("controller source global",e);
  out.same_controller=r->actor->controller->command_state(current.global_blocked);
 }
 out.initial_position1450=r->init_fields.initial_position1450;
 out.initial_rotation145c=r->init_fields.initial_rotation145c;
 e.clear();return true;
}
bool CanonicalCharacterSaveV86::initialize(std::string& e){
 if(attempted_){if(failed_)e=error_;return !failed_;}attempted_=true;
 auto r=record_.lock();if(!r||!r->actor||!r->actor->object||!services_.provider){failed_=true;return missing("published actor/native providers",e);}
 //Actual CharProperties C2 legacy tag; checkpoint/pose stay the existing
 //actual1468/1474 cells, never these metadata fallback arrays.
 if(!metadata_.tags.construct_fresh(e)){failed_=true;error_=e;return false;}
 connection_=std::make_unique<RetainedCharacterSaveConnectionV3>(*r->actor,r,
  [this](auto& out,auto& error){return extra(out,error);},CharacterSaveRestoreServicesV3{this,invoke});
 e.clear();return true;
}
bool CanonicalCharacterSaveV86::bind(level::LevelSaveObjectBorrowV2& out,std::string& e){return initialize(e)&&connection_->bind_methods(out,e);}
bool CanonicalCharacterSaveV86::close(std::string& e){
 connection_.reset();
 if(npc_buffs_){if(dh2_character_buffs_destroy(npc_buffs_)!=1)return missing("NPC buff FX/D1 release",e);npc_buffs_=nullptr;}
 e.clear();return true;
}
bool CanonicalCharacterSaveV86::script_lifecycle(bool initialize,std::string& e){
 auto r=record_.lock();if(!r||!r->actor||!r->actor->object)return missing("script receiver",e);
 ScriptLifecycleState64* state{};TimerStore32* timers{};const TimerServices32* timer_services{};
 if(r->player_script_owner_v62){auto& session=r->player_script_owner_v62->session();state=&session.owner().lifecycle();timers=&session.timers();timer_services=&session.timer_services();}
 else if(r->actor->session){auto& session=*r->actor->session;state=&session.owner().lifecycle();timers=&session.timers();timer_services=&session.native_timer_services();}
 if(!state||!timers||!timer_services||state->owner!=r->actor->object->identity||timers->owner!=state->owner)return missing("retained script lifecycle/timers",e);
 struct Delivery {world::CanonicalCharacterCandidateRecordV60& r;TimerStore32& timers;const TimerServices32& timer_services;
  static void invoke(void* raw,ScriptLifecycleState64*,const ScriptLifecycleRequest32* q,ScriptLifecycleResponse16* out){
   auto& d=*static_cast<Delivery*>(raw);auto& r=d.r;std::string error;bool okay=false;
   switch(q->service){
   case script_owner_is_dead:out->word=r.life->dead;okay=true;break;
   case script_timer_stop:okay=dh2_character_timer_stop(&d.timers,q->argument0)>=0;break;
   case script_design_tick:{auto design=r.design.design();std::int32_t value{};
    const auto* name=q->argument0==0x33?"AI_Tick":q->argument0==0x34?"DoT_Tick":nullptr;
    okay=name&&design&&design->lookup&&design->lookup(design->context,0,"CharacterDesign",name,&value)==0;
    if(okay)std::memcpy(&out->word,&value,4);break;}
   case script_timer_start:{auto value=dh2_character_timer_start(&d.timers,q->argument0,-1,static_cast<std::int32_t>(q->argument1),0,&d.timer_services);
    //Source timer allocation -1 is a real returned ID, not readiness.
    okay=value>=-1;if(okay)out->word=static_cast<std::uint32_t>(value);break;}
   case script_skill_cleanup:case script_spell_cleanup:
    if(r.player_script_owner_v62){okay=(q->service==script_skill_cleanup?r.player_script_owner_v62->cleanup_skills():r.player_script_owner_v62->cleanup_spells())>=0;error=r.player_script_owner_v62->error();}
    else if(r.npc_skills_v84){okay=(q->service==script_skill_cleanup?r.npc_skills_v84->cleanup_skills():r.npc_skills_v84->cleanup_spells())>=0;error=r.npc_skills_v84->error();}
    else{auto empty=r.actor->source_ctor_empty_skill_vectors_v84();okay=empty&&(*empty)[0]==0&&(*empty)[1]==0;}break;
   case script_ais_init:case script_ais_init_post:case script_ais_init_final:case script_ais_terminate:{
    const auto slot=q->service==script_ais_init?8u:q->service==script_ais_init_post?12u:q->service==script_ais_init_final?16u:20u;
    if(r.player_script_owner_v62){auto& owner=r.player_script_owner_v62->session().owner();okay=owner.source_initial_virtual_v86(q->subject,slot)==0;error=owner.error();}
    else if(r.actor->session){auto& owner=r.actor->session->owner();okay=owner.source_initial_virtual_v86(q->subject,slot)==0;error=owner.error();}break;}
   default:break;
   }
   if(!okay)throw std::runtime_error(error.empty()?"Required actual AI restore lifecycle service "+std::to_string(q->service):error);
  }
 } delivery{*r,*timers,*timer_services};
 try{const ScriptLifecycleServices16 services{&delivery,Delivery::invoke};
  const auto result=dh2_character_script_lifecycle(state,initialize?script_ai_script_init:script_cleanup,0,&services);
  if(result!=1)return missing("whole AI_ScriptInit/CleanUp source protocol",e);e.clear();return true;
 }catch(const std::exception& failure){e=failure.what();return false;}
}
bool CanonicalCharacterSaveV86::invoke(void* raw,const CharacterSaveRestoreRequestV3& q,CharacterSaveRestoreResponseV3& out,std::string& e){return static_cast<CanonicalCharacterSaveV86*>(raw)->callback(q,out,e);}
bool CanonicalCharacterSaveV86::callback(const CharacterSaveRestoreRequestV3& q,CharacterSaveRestoreResponseV3& out,std::string& e){
 auto r=record_.lock();if(!r||!r->actor||!r->actor->object)return missing("callback receiver",e);out={};
 const auto id=r->actor->object->identity;auto visual=r->visual?r->visual->visual():nullptr;
 switch(q.entry){
 case 0x3a49f0:{bool value{};if(!r->is_player(value,e))return false;out.word=value;return true;}
 case 0x3a3064:{const data::AiProps* row{};if(!ai(*r,row,e))return false;out.word=row->type==4;return true;}
 case 0x31f594:{bool value{};if(!services_.current_level_byte||!services_.current_level_byte(q.argument0,value,e))return missing("current Level f2/f3",e);out.word=value;return true;}
 case 0x38b954:{auto assertion=world::SourceAssertionProcessV76::borrow();if(!assertion)return missing("source assertion process owner",e);
  //38b960/970: mode2 reaches deliberate NULL store, mode1 __assert. All
  //other source modes continue silently. Never fabricate a trap/log return.
  const auto mode=*assertion->source_level();if(mode==1||mode==2)return missing("positive saved-archetype source assertion trap",e);return true;}
 case 0x38b0f0:return services_.set_visible&&services_.set_visible(id,q.argument0!=0,e);
 case 0x4713d0:
  return services_.sync_visibility&&services_.sync_visibility(id,e);
 case 0x3a59ac:return r->revive_v70(q.payload,q.argument1,e);
 case 0x3e07a0:if(dh2_property_set(&r->view,static_cast<std::int32_t>(q.argument0),static_cast<std::int32_t>(q.argument1)))return missing("PROPS_Set real type/sheets",e);return true;
 case 0x3cfd7c:return script_lifecycle(false,e);
 case 0x3cfde4:return script_lifecycle(true,e);
 case 0x3c01c0:case 0x3c01d4:{std::int32_t state{};
  if(!r->actor->machine||dh2_character_native_fsm_get_integer(&state,&r->actor->machine->native_fsm(),0)!=1)return missing("FSM nullable state",e);
  out.word=state==(q.entry==0x3c01c0?0:17);return true;}
 case 0x3c1a00:case 0x3c1a64:case 0x3c1a74:{auto& machine=*r->actor->machine;
  auto& native=machine.native_fsm();auto& fields=machine.owner().machine();
  const auto next=q.entry==0x3c1a00?3:q.entry==0x3c1a64?17:0;
  if(q.entry==0x3c1a00)machine.state().idle_suppressed=q.argument0;
  if(q.entry==0x3c1a74){machine.state().dead_alternate=q.argument0&255u;
   if(native.current_present&&(machine.state().current==0||machine.state().current==17||machine.state().current==16)){
    native.current_present=0;fields.current_index=-1;machine.state().current=-1;
   }}
  if(machine.transition(next,-1,0)<0){e=machine.error();return false;}return true;}
 case 0x3a5248:{bool result{};if(!source_character_can_respawn_v87(*r,result,e))return false;out.word=result;return true;}
 case 0x3d6cdc:if(!r->death_v84)return missing("AI_SetDead owner",e);return r->death_v84->set_dead(e);
 case 0x3b4088:return r->initialize_physical(e);
 case 0x3935dc:{world::GameObjectInitializationFieldsV62 fields;if(!r->actor->inherited_initialization_fields_v62(r,fields,e))return false;
  auto* cached=fields.pointer(0x180);auto* visible=fields.byte(0x80);if(!cached||!visible)return missing("GetTargetPosition180/80",e);
  out.point=*cached&&*visible?fields.vector3(0x184):r->actor->source_position160_v7();return out.point?true:missing("actual cached target184",e);}
 case 0x393db4:if(!r->position)return missing("Position owner",e);return r->position->set_position(reinterpret_cast<const float*>(q.payload),q.argument0!=0,e);
 case 0x3938a0:{const auto* p=reinterpret_cast<const float*>(q.payload);if(!p)return missing("SetRotation argument",e);
  auto* rotation=r->actor->runtime.rotation.rotation;rotation[0]=p[0];rotation[1]=p[1];rotation[2]=p[2];r->actor->runtime.rotation.heading_angle=p[2];
  if(!r->actor->source_visual()||(r->actor->machine->state().flags&16u))return true;
  return visual?visual->sync_rotation_v86(e):missing("sole visual rotation receiver",e);}
 case 0x38ba74:if(!visual||q.subject!=reinterpret_cast<std::uintptr_t>(visual.get()))return missing("sole visual Sync receiver",e);return visual->sync(e);
 case 0x2e0:return services_.update_anchor&&services_.update_anchor(q.subject,e);
 case 0x3fc:
  if(!r->ai_group_v87||!r->ai_group_v87->source_present()||q.subject!=reinterpret_cast<std::uintptr_t>(r->ai_group_v87.get())||r->actor->source_ai_group34!=q.subject)return missing("same produced CharGroup3fc",e);
  out.group=r->ai_group_v87->saved_fields();return true;
 default:return missing("unrecognized source serialization entry",e);
 }
}
}
