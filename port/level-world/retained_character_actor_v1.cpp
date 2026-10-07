#include "retained_character_actor_v1.hpp"
#include "character_script_source_virtuals_v101.hpp"
#include "character_scene.hpp"
#include "character_world_ai_queue_v1.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::character {
RetainedCharacterActorV1::RetainedCharacterActorV1(std::uintptr_t id,std::shared_ptr<void> pin,const std::string& catalog,RetainedCharacterConstructionV7 construction):world_pin_(std::move(pin)),identity_(id),class_name_(catalog){
 // Whole ObjectBase C1 33f15c: key0, cacheSELF, frameUINTMAX, room-1.
 handle_={0,UINT32_MAX,id};
 class_name20_=class_name_.c_str();
 source_bools_[0x81]=0; // Whole ObjectBase C1 33f15c oracle: disabled81=0.
 source_bools_[0x84]=0; // Whole ObjectBase ctor verified before InitProperties.
 source_bools_[0x118]=0; // ObjectBaseC1 33f2c8 remote-update byte constructor0.
 // CharacterC1 3a9594 stores r6=-1 (3a93d8) into signed source byte14a8.
 animation_ai.owner_byte14a8=-1;
 if(construction==RetainedCharacterConstructionV7::fresh_canonical){
  source_pointers_v62_[0x2f4]=0;source_bools_[0x2f8]=0;
  //Inherited GameObject C1 38c2c4/38c2f8: source talk marker starts
  //primary1/required0 before real GameEvent registration changes it.
  source_bools_[0x2fa]=0;source_bools_[0x2fb]=1;
  source_bools_[0x1448]=1; //actual Character C2 3a95b0/3a95b4, low-health sound rearm
  source_aggro_v84_=std::make_unique<SourceCharacterAggroV84>(true);
  //CharAI C2 3cec60/64: distinct seeking412 and click-target413 are1.
  source_bools_[0x412]=1;
  source_click_v101_=std::make_unique<world::WorldClickFieldsOwnerV1>();
  source_ai_pointers_v105_=std::make_unique<CharacterAiPointerFieldsV105>();
  source_frame_fields_v106_=std::make_unique<CharacterFrameFieldsV106>();
  source_target_list304_v111_=std::make_unique<world::GameObjectTargetListOwnerV107>(id);
  source_ctor_empty_timers_v111_=true;
  source_bools_[0x2fc]=0; // GameObject C2 38c564, CanUpdate force-update.
  source_bools_[0x1480]=0; // Character C2 3a9628/2c, interaction flag.
  source_bools_[0x548]=0;source_pointers_v62_[0x54c]=0; //embedded FSM C1 3c1b38/3c1b3c: repeat4c/object50
  source_bools_[0x530]=0; //embedded FSM C1 3c1ac4 word34 zero, Limbus mode
  source_bools_[0x14ac]=0;source_bools_[0x14ad]=0; //3a968c,3a96b0.
  source_pointers_v62_[0x14ec]=0; //fresh Character C1 optional native owner.
  animation_ai.seeking=1;animation_ai.target_sticky=1;
  //CharAI C1 3cee44..58 explicitly zeroes b4/b8/bc and c0/c4/c8.
  source_ctor_skill_vector_counts_v84_.fill(0);source_ctor_skill_vectors_v84_=true;
  network114_v70_=0;network114_produced_v70_=true; // ObjectBase C1 explicit33f2a8.
  target_cross149c_v70_=0;target_cross149c_produced_v70_=true; // Character C1 NULL interaction FX cells.
  // Character C2 3a9584 sets r3=0; 3a95f8..3a9624 explicitly stores
  // all six checkpoint/save-position words. C1 repeats at3aa46c.
  checkpoint1468_v83_.fill(0);save_position1474_v83_.fill(0);checkpoint_c1_v83_=true;
  // Inherited ObjectBase/GameObject C1, same constructor stores as the
  // canonical generic base. Do not replay these on live Crypt adoption.
  for(auto offset:{0xecu,0x108u,0x270u,0x370u})source_integers_v62_[offset]=-1;
  source_integers_v62_[0x194]=0; //GameObject C1 38c130 stores word194=0.
  for(auto offset:{0xfcu,0x180u,0xa8u,0xccu,0x2e4u})source_pointers_v62_[offset]=0; //2e4 GameObjectC2 38c53c
  source_bools_[0xac]=0;source_bools_[0xd0]=0; // ConditionData C1 tested bytes.
  //Whole ObjectBase C1 stores r5=0 at33f1dc/e0,33f260..26c.
  for(auto offset:{0x28u,0x29u,0x60u,0x85u,0x86u,0x88u,0x89u,0x10cu})source_bools_[offset]=0;
  for(auto offset:{0x2efu,0x2f0u,0x372u,0x373u})source_bools_[offset]=0;
  source_bools_[0x2fa]=0;source_bools_[0x2fb]=1; //GameObjectC2 38c560/38c52c
  source_bools_[0x1395]=0; // CharacterC1 3aa2ec/f0 once-only InitFinal gate.
  source_bools_[0x1481]=0; //C1 3a93cc r8=0 ->3a9630/34 byte1481, CanRespawn gate.
  source_bools_[0x8a]=1;source_bools_[0x2ee]=1; //ObjectBase/GameObject C1.
  for(auto offset:{0x90u,0xb4u,0xd4u,0x254u,0x278u,0x290u,0x2a8u,0x2c0u,0x358u})source_strings_[offset]={};
  if(!source_kill_fields_v42_.construct_fresh(error_))throw std::logic_error(error_);
  if(!source_position_fields_v7_.construct(runtime,error_))throw std::logic_error(error_);
  source_position_written_=true; // GameObjectC2 stores source Position160=0.
 }
}
bool RetainedCharacterActorV1::construct_kill_fields_source_c1_v42(std::string& error){
 return source_kill_fields_v42_.construct_fresh(error);
}
bool RetainedCharacterActorV1::adopt_kill_fields_observed_v42(std::uintptr_t killer,
 std::uintptr_t master,std::int16_t template_id,std::uint8_t suppress,std::string& error){
 if(suppress>1){error="Required actual boolean Character suppress quest14e4";return false;}
 return source_kill_fields_v42_.adopt_observed(killer,master,template_id,suppress,error);
}
RetainedCharacterActorV1::~RetainedCharacterActorV1(){close();}
void RetainedCharacterActorV1::close()noexcept{
 //Native weak-target lifetime follows logical source receiver destruction,
 //even when diagnostic/shared facades still retain its host allocation.
 native_lifetime_v107_.reset();
 if(source_ai_queue_registered_v105_){character_ai_queue_v105()->remove(ai_events.ai);source_ai_queue_registered_v105_=false;}
 player_lifecycle_v62_=nullptr;
 session.reset();injury.reset();state_changed.reset();machine.reset();
 animation.reset();controller.reset();diagnostics.reset();object.reset();
}
bool RetainedCharacterActorV1::set_name(void* p,const char* name,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(!name||a.script_attempted_){e="SetName requires pre-session canonical receiver";return false;}a.source_name_=name;return true;}
bool RetainedCharacterActorV1::set_archetype(void* p,const char* name,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(!name||a.script_attempted_){e="archetype requires pre-session canonical receiver";return false;}a.source_archetype_=name;return true;}
bool RetainedCharacterActorV1::as_character(void* p,std::uintptr_t& out,std::string&){out=static_cast<RetainedCharacterActorV1*>(p)->identity_;return true;}
world::CanonicalObjectBorrowV1 RetainedCharacterActorV1::canonical(std::shared_ptr<void> lease){
 const auto room=source_pointers_v62_.find(0x2f4);
 const auto no_room=source_bools_.find(0x2f8);
 return {identity_,std::move(lease),&handle_,&type_f4_,across_rooms_written_?&across_rooms87_:nullptr,&room64_,this,set_name,set_archetype,as_character,&class_name20_,read_across_rooms,
  room==source_pointers_v62_.end()?nullptr:&room->second,
  no_room==source_bools_.end()?nullptr:&no_room->second};
}
bool RetainedCharacterActorV1::adoption_name_v2(void* p,const char* name,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(!name||a.source_name_!=name||!a.object||a.object->name!=name){e="Canonical adoption must preserve actual retained Character name";return false;}a.source_name_=name;return true;}
bool RetainedCharacterActorV1::adoption_archetype_v2(void* p,const char* name,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(!name||a.source_archetype_!=name){e="Canonical adoption must preserve actual Character archetype";return false;}a.source_archetype_=name;return true;}
bool RetainedCharacterActorV1::canonical_adoption_v2(std::shared_ptr<void> lease,const std::string& name,const std::string& archetype,std::int32_t room,world::CanonicalObjectBorrowV1& out,std::string& e){
 if(!lease||!object||object->identity!=identity_||object->name!=source_name_||name!=source_name_||archetype!=source_archetype_||room!=room64_){e="Required unchanged SAME retained Character adoption inputs before source Add";return false;}
 out=canonical(std::move(lease));out.set_name=adoption_name_v2;out.set_archetype=adoption_archetype_v2;return true;
}
world::CanonicalPropertyActorV1 RetainedCharacterActorV1::properties()noexcept{
 return {class_name20_,&template_name_,{this,read_bool,write_bool,write_int,write_float,write_string,write_vector3,write_point2}};
}
bool RetainedCharacterActorV1::read_bool(void* p,std::uint32_t offset,std::uint8_t& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset==0x15c){value=a.source_bounds_flat;return true;}if(offset==0x87&&a.across_rooms_written_){value=a.across_rooms87_;return true;}auto i=a.source_bools_.find(offset);if(i==a.source_bools_.end()){e="required actual Character property bool producer";return false;}value=i->second;return true;}
bool RetainedCharacterActorV1::write_bool(void* p,std::uint32_t offset,std::uint8_t value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);switch(offset){case 0x15c:a.source_bounds_flat=value;return true;case 0x87:a.publish_across_rooms(value);return true;case 0x84:case 0x80:case 0x83:case 0xf0:case 0xf1:case 0x2ed:case 0x60:case 0x13e4:case 0x1430:a.source_bools_[offset]=value;return true;default:e="unrecovered Character bool property offset";return false;}}
bool RetainedCharacterActorV1::write_int(void* p,std::uint32_t offset,std::int32_t value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset!=0x274){e="unrecovered Character int property offset";return false;}a.spawn_probability_=value;a.spawn_probability_written_=true;return true;}
bool RetainedCharacterActorV1::write_float(void* p,std::uint32_t offset,float value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset!=0x143c){e="unrecovered Character float property offset";return false;}a.spawn_view_radius_=value;a.spawn_view_radius_written_=true;return true;}
bool RetainedCharacterActorV1::write_string(void* p,std::uint32_t offset,const std::string& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);switch(offset){case 0x30:return set_name(p,value.c_str(),e);case 0x48:return set_archetype(p,value.c_str(),e);case 0x68:case 0x90:case 0xb4:case 0xd4:case 0x290:case 0x2a8:case 0x2c0:case 0x278:case 0x358:case 0x13b0:case 0x1398:case 0x13cc:case 0x13e8:case 0x1400:case 0x1418:a.source_strings_[offset]=value;return true;default:e="unrecovered Character CString property offset";return false;}}
bool RetainedCharacterActorV1::write_vector3(void* p,std::uint32_t offset,const std::array<float,3>& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);switch(offset){case 0x160:if(a.object)a.object->position=value;else a.source_position_=value;a.source_position_written_=true;return true;case 0x16c:for(unsigned n=0;n<3;++n)a.runtime.rotation.rotation[n]=value[n];a.source_rotation_written_=true;return true;case 0x120:a.source_scale_=value;a.source_scale_written_=true;return true;default:e="unrecovered Character vector property offset";return false;}}
bool RetainedCharacterActorV1::write_point2(void* p,std::uint32_t offset,const std::array<std::int32_t,2>& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset!=0x1434){e="unrecovered Character Point2Di property offset";return false;}a.spawn_delay_=value;a.spawn_delay_written_=true;return true;}
bool RetainedCharacterActorV1::source_position(std::array<float,3>& value,std::string& e)const{if(object){value=object->position;return true;}if(!source_position_written_){e="required source position default/override";return false;}value=source_position_;return true;}
bool RetainedCharacterActorV1::source_scale(std::array<float,3>& value,std::string& e)const{if(!source_scale_written_){e="required source scale default/override";return false;}value=source_scale_;return true;}
const std::string* RetainedCharacterActorV1::source_string(std::uint32_t offset)const noexcept{if(offset==0x30)return &source_name_;if(offset==0x48)return &source_archetype_;auto i=source_strings_.find(offset);return i==source_strings_.end()?nullptr:&i->second;}
bool RetainedCharacterActorV1::visual_strings_v6(std::string*& model,std::string*& xref,std::string& error){
 model=nullptr;xref=nullptr;auto m=source_strings_.find(0x290),x=source_strings_.find(0x2a8);
 if(m==source_strings_.end()||x==source_strings_.end()){error="Required source Character model/xref CString fields";return false;}
 model=&m->second;xref=&x->second;return true;
}
bool RetainedCharacterActorV1::init_post_fields(NpcInitPostBorrowV1& out,std::string& e){
 auto waypoint=source_strings_.find(0x13e8),model=source_strings_.find(0x290);
 if(!object||(!session&&!initialization_properties_)||!spawn_probability_written_||!source_scale_written_||!source_rotation_written_||waypoint==source_strings_.end()||model==source_strings_.end()){
  e="required completed canonical properties and same allocated Character graph before InitPost";return false;
 }
 out={object->identity,&init_post_called1394_,&spawn_probability_,&waypoint->second,&room64_,&char_properties_id13c8_,initialization_properties_?initialization_properties_:&session->property_view(),&model->second,source_scale_.data(),nullptr,&self_fx_offset1488_,&self_fx1484_,&visual2d8_,&fade1440_,&fade1444_,object->position.data(),initial_position1450_.data(),runtime.rotation.rotation,initial_rotation145c_.data(),this,live_delayed};return true;
}
bool RetainedCharacterActorV1::inherited_initialization_fields_v62(std::shared_ptr<void> lease,world::GameObjectInitializationFieldsV62& out,std::string& e){
 if(!lease||!object||!source_scale_written_||!source_rotation_written_||!source_position_fields_v7_.constructed){e="Required actual retained inherited Character initialization fields";return false;}
 world::GameObjectInitializationFieldsV62 f;f.receiver=std::move(lease);f.source_identity=identity_;
 f.byte=[this](std::uint32_t o)->std::uint8_t*{if(o==0x15c)return &source_bounds_flat;auto at=source_bools_.find(o);return at==source_bools_.end()?nullptr:&at->second;};
 f.integer=[this](std::uint32_t o)->std::int32_t*{if(o==0x110)return machine?&machine->combat_fields().network_id:nullptr;if(o==0x274)return spawn_probability_written_?&spawn_probability_:nullptr;if(source_frame_fields_v106_){if(o==0x544)return &predicates.interaction; //SAME source FSM48 C1=0 (3c1b34), not mask528
 if(o==0x1490)return &source_frame_fields_v106_->state_fx_kind1490;if(o==0x1500)return &source_frame_fields_v106_->displayed_gold1500;if(o==0x1504)return &source_frame_fields_v106_->displayed_damage1504;}auto at=source_integers_v62_.find(o);return at==source_integers_v62_.end()?nullptr:&at->second;};
 f.pointer=[this](std::uint32_t o)->std::uintptr_t*{switch(o){case 0x2dc:return &source_position_fields_v7_.physical2dc;case 0x2e0:return &source_position_fields_v7_.attached2e0;case 0x2d8:return &visual2d8_;case 0x14e0:return source_frame_fields_v106_?&source_frame_fields_v106_->timer14e0:nullptr;default:{auto at=source_pointers_v62_.find(o);return at==source_pointers_v62_.end()?nullptr:&at->second;}}};
 f.string=[this](std::uint32_t o)->std::string*{if(o==0x48)return &source_archetype_;auto at=source_strings_.find(o);return at==source_strings_.end()?nullptr:&at->second;};
 f.vector3=[this](std::uint32_t o)->float*{switch(o){case 0x120:return source_scale_.data();case 0x160:return object->position.data();case 0x16c:return runtime.rotation.rotation;
  //SAME cached184 field already written at actual actor-runtime target phase
  //from its real target_absolute_position. No root/node resample or pose copy.
  case 0x184:return runtime.target_position;
  case 0x1450:return source_ai_pointers_v105_||init_post_called1394_?initial_position1450_.data():nullptr;
  case 0x145c:return source_ai_pointers_v105_||init_post_called1394_?initial_rotation145c_.data():nullptr;
  default:return nullptr;}};
 f.scalar=[this](std::uint32_t o)->float*{if(o==0x178)return &runtime.rotation.heading_angle;if(o==0x143c)return spawn_view_radius_written_?&spawn_view_radius_:nullptr;if(o==0x14fc&&source_frame_fields_v106_)return &source_frame_fields_v106_->state_fx_delay14fc;return nullptr;};
 f.relative_aabb144=[this]{return runtime.subobjects.local_bounds;};
 f.update_absolute_aabb=[this]{for(unsigned i=0;i<6;++i)runtime.subobjects.absolute_bounds[i]=runtime.subobjects.local_bounds[i]+object->position[i%3];};
 out=std::move(f);e.clear();return true;
}
bool RetainedCharacterActorV1::construct_fields(std::shared_ptr<ScriptCharacterObject> same,WorldNpcStateServicesV1 services){
 if(graph_attempted_||!identity_||!world_pin_||!same||same->identity!=identity_||!same->properties||!same->life){error_="required same registered Character graph/World/resource lifetime";return false;}
 if(source_position_written_&&same->position!=source_position_){error_="registered Character position differs from canonical property owner";return false;}
 graph_attempted_=true;object=std::move(same);
 if(source_ai_pointers_v105_){
  // Character C2 overwrites inherited defaults at3a9770/3a977c before
  // allocating its actual controller. These are not ObjectBase's zeroes.
  source_bools_[0x28]=1;source_bools_[0x85]=1;
  runtime.controller.validate_boundary=1; //same source1c4 store3a9778.
 }
 controller=std::make_unique<CharacterWorldNpcControllerV1>(identity_);
 // Facts/body/predicate pointers are the retained fields, never temporaries.
 services.facts=&facts;services.bodies=&bodies;services.predicates=&predicates;
 machine=std::make_unique<CharacterWorldNpcStateOwnerV1>(identity_,services);
 ai_owner={identity_,controller->identity(),reinterpret_cast<std::uintptr_t>(&machine->native_fsm()),reinterpret_cast<std::uintptr_t>(object->properties.get()),0,0,0,0};
 ai_events={object->target.identity,&ai_owner,character_idle_ai_keys(),0,character_idle_external_keys(),0,0,0,0,0,0};
 if(source_ai_pointers_v105_){
  if(!character_ai_queue_v105()->add(ai_events.ai)){error_="Duplicate actual CharAI construction in process update queue";return false;}
  source_ai_queue_registered_v105_=true;
 }
 state_changed=std::make_unique<CharacterWorldNpcStateChangedV1>(*machine,ai_events);
 return true;
}
std::uint32_t* RetainedCharacterActorV1::live_delayed(void* p){auto& a=*static_cast<RetainedCharacterActorV1*>(p);return a.player_lifecycle_v62_?&a.player_lifecycle_v62_->delayed:a.session?&a.session->owner().lifecycle().delayed:&a.initial_delayed3ec_;}
bool RetainedCharacterActorV1::bind_player_script_lifecycle_v62(ScriptLifecycleState64* same,std::string& e){
 if(!same||!object||session||script_attempted_||player_lifecycle_v62_){e="Required once-only SAME player ScriptOwner lifecycle binding";return false;}
 same->delayed=initial_delayed3ec_;player_lifecycle_v62_=same;script_attempted_=true;return true;
}
bool RetainedCharacterActorV1::bind_initialization_properties(data::PropertyView& v,std::string& e){
 if(!object||script_attempted_||initialization_properties_||v.base!=object->properties->base.data()||v.saved!=object->properties->saved.data()||v.gear!=object->properties->gear.data()||v.resolved!=object->properties->resolved.data()){e="required sole canonical property projection before script allocation";return false;}initialization_properties_=&v;return true;
}
bool RetainedCharacterActorV1::bind_animation(std::shared_ptr<const CharacterAnimationResources> resources){
 if(!object||animation_attempted_||!resources){error_="required fresh actual Character visual animation resources";return false;}animation_attempted_=true;animation=CharacterAnimationInstance::create(std::move(resources),error_);return bool(animation);
}
bool RetainedCharacterActorV1::construct_graph(std::shared_ptr<ScriptCharacterObject> same,std::shared_ptr<const CharacterAnimationResources> resources,WorldNpcStateServicesV1 services){
 if(!resources){error_="required actual Character animation resources";return false;}return construct_fields(std::move(same),services)&&bind_animation(std::move(resources));
}
bool RetainedCharacterActorV1::construct_script(CharacterGameDesign::Borrow&& design,CharacterScriptSessionInput input){
 if(script_attempted_||!object||!machine||!controller||source_name_.empty()||object->name!=source_name_){error_="required canonical Add name and retained graph before MonsterSession";return false;}
 script_attempted_=true;input.identity=identity_;input.name=source_name_;input.properties=object->properties;input.combat=object->life;input.position=object->position;input.source_is_character=1;input.state_machine=&machine->native_fsm();
 auto candidate=CharacterScriptSession::create(std::move(design),input,error_);if(!candidate)return false;
 if(initialization_properties_)candidate->owner().lifecycle().delayed=initial_delayed3ec_;
 session=std::move(candidate);
 source_ctor_empty_timers_v111_=false; //SAME timer view is now lent by the Session
 return true;
}
bool RetainedCharacterActorV1::load_script(){
 if(!session||script_load_attempted_){error_="required fresh retained script load attempt";return false;}
 script_load_attempted_=true;
 if(session->start()!=0||!session->error().empty()||session->owner().last_source_load_status()!=0){error_=session->error();return false;}
 ScriptSessionView active{};if(!session->owner().active(active)){error_="required actual started MonsterSession active owner";return false;}
 ai_events.active=active.identity;ai_events.ais_virtuals=character_script_source_virtuals_v101(active.kind);return ai_events.ais_virtuals!=nullptr;
}
bool RetainedCharacterActorV1::visual_fields_v5(std::shared_ptr<void> lease,const std::function<bool(std::string&)>& update_pf,world::GameObjectVisualFieldBorrowV5& out,std::string& e){
 if(!lease||!object||!object->properties||!source_scale_written_||!source_rotation_written_){e="Required SAME initialized Character visual fields";return false;}
 world::GameObjectVisualFieldBorrowV5 result;result.receiver_lease=std::move(lease);result.identity=identity_;
 result.static84=source_bool_field(0x84);result.position160=object->position.data();result.rotation16c=runtime.rotation.rotation;result.scale120=source_scale_.data();
 auto apply=[this,update_pf](const float* box,bool marker,std::string& error){
  if(!box){error="Required actual Character mesh bounds";return false;}
  physical::CharacterOwnerBoundsInput input{};std::copy_n(box,6,input.mesh_box);std::copy_n(object->position.data(),3,input.position);
  input.collision_scale=object->properties->resolved[16];input.already_scaled=marker?1u:0u;input.previous_flat=source_bounds_flat;
  physical::CharacterOwnerBounds bounds{};if(dh2_character_owner_bounds(&bounds,&input)){error="Source Character SetRelativeAABB failed";return false;}
  source_bounds_flat=static_cast<std::uint8_t>(bounds.flat);std::copy_n(bounds.relative_box,6,runtime.subobjects.local_bounds);std::copy_n(bounds.absolute_box,6,runtime.subobjects.absolute_bounds);source_bounds_ready=true;
  if(!update_pf){error="Required SAME Character UpdatePFObject after bounds prefix";return false;}return update_pf(error);
 };
 result.apply_mesh_box_v6=apply;
 result.apply_mesh_box=[apply](const float* box,std::string& error){return apply(box,false,error);};
 out=std::move(result);return true;
}
}

