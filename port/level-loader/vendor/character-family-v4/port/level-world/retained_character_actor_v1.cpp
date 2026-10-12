#include "retained_character_actor_v1.hpp"
namespace dh2::character {
RetainedCharacterActorV1::RetainedCharacterActorV1(std::uintptr_t id,std::shared_ptr<void> pin,const std::string& catalog):world_pin_(std::move(pin)),identity_(id),class_name_(catalog){
 // Whole ObjectBase C1 33f15c: key0, cacheSELF, frameUINTMAX, room-1.
 handle_={0,UINT32_MAX,id};
 class_name20_=class_name_.c_str();
 source_bools_[0x81]=0; // Whole ObjectBase C1 33f15c oracle: disabled81=0.
 source_bools_[0x84]=0; // Whole ObjectBase ctor verified before InitProperties.
 source_bools_[0x118]=0; // ObjectBaseC1 33f2c8 remote-update byte constructor0.
 // CharacterC1 3a9594 stores r6=-1 (3a93d8) into signed source byte14a8.
 animation_ai.owner_byte14a8=-1;
}
RetainedCharacterActorV1::~RetainedCharacterActorV1(){close();}
void RetainedCharacterActorV1::close()noexcept{
 session.reset();injury.reset();state_changed.reset();machine.reset();
 animation.reset();controller.reset();diagnostics.reset();object.reset();
}
bool RetainedCharacterActorV1::set_name(void* p,const char* name,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(!name||a.script_attempted_){e="SetName requires pre-session canonical receiver";return false;}a.source_name_=name;return true;}
bool RetainedCharacterActorV1::set_archetype(void* p,const char* name,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(!name||a.script_attempted_){e="archetype requires pre-session canonical receiver";return false;}a.source_archetype_=name;return true;}
bool RetainedCharacterActorV1::as_character(void* p,std::uintptr_t& out,std::string&){out=static_cast<RetainedCharacterActorV1*>(p)->identity_;return true;}
world::CanonicalObjectBorrowV1 RetainedCharacterActorV1::canonical(std::shared_ptr<void> lease){
 return {identity_,std::move(lease),&handle_,&type_f4_,across_rooms_written_?&across_rooms87_:nullptr,&room64_,this,set_name,set_archetype,as_character,&class_name20_,read_across_rooms};
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
bool RetainedCharacterActorV1::read_bool(void* p,std::uint32_t offset,std::uint8_t& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset==0x87&&a.across_rooms_written_){value=a.across_rooms87_;return true;}auto i=a.source_bools_.find(offset);if(i==a.source_bools_.end()){e="required actual Character property bool producer";return false;}value=i->second;return true;}
bool RetainedCharacterActorV1::write_bool(void* p,std::uint32_t offset,std::uint8_t value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);switch(offset){case 0x87:a.publish_across_rooms(value);return true;case 0x84:case 0x80:case 0x83:case 0xf0:case 0xf1:case 0x2ed:case 0x15c:case 0x60:case 0x13e4:case 0x1430:a.source_bools_[offset]=value;return true;default:e="unrecovered Character bool property offset";return false;}}
bool RetainedCharacterActorV1::write_int(void* p,std::uint32_t offset,std::int32_t value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset!=0x274){e="unrecovered Character int property offset";return false;}a.spawn_probability_=value;a.spawn_probability_written_=true;return true;}
bool RetainedCharacterActorV1::write_float(void* p,std::uint32_t offset,float value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset!=0x143c){e="unrecovered Character float property offset";return false;}a.spawn_view_radius_=value;a.spawn_view_radius_written_=true;return true;}
bool RetainedCharacterActorV1::write_string(void* p,std::uint32_t offset,const std::string& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);switch(offset){case 0x30:return set_name(p,value.c_str(),e);case 0x48:return set_archetype(p,value.c_str(),e);case 0x68:case 0x90:case 0xb4:case 0xd4:case 0x290:case 0x2a8:case 0x2c0:case 0x278:case 0x358:case 0x13b0:case 0x1398:case 0x13cc:case 0x13e8:case 0x1400:case 0x1418:a.source_strings_[offset]=value;return true;default:e="unrecovered Character CString property offset";return false;}}
bool RetainedCharacterActorV1::write_vector3(void* p,std::uint32_t offset,const std::array<float,3>& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);switch(offset){case 0x160:if(a.object)a.object->position=value;else a.source_position_=value;a.source_position_written_=true;return true;case 0x16c:for(unsigned n=0;n<3;++n)a.runtime.rotation.rotation[n]=value[n];a.source_rotation_written_=true;return true;case 0x120:a.source_scale_=value;a.source_scale_written_=true;return true;default:e="unrecovered Character vector property offset";return false;}}
bool RetainedCharacterActorV1::write_point2(void* p,std::uint32_t offset,const std::array<std::int32_t,2>& value,std::string& e){auto& a=*static_cast<RetainedCharacterActorV1*>(p);if(offset!=0x1434){e="unrecovered Character Point2Di property offset";return false;}a.spawn_delay_=value;a.spawn_delay_written_=true;return true;}
bool RetainedCharacterActorV1::source_position(std::array<float,3>& value,std::string& e)const{if(object){value=object->position;return true;}if(!source_position_written_){e="required source position default/override";return false;}value=source_position_;return true;}
bool RetainedCharacterActorV1::source_scale(std::array<float,3>& value,std::string& e)const{if(!source_scale_written_){e="required source scale default/override";return false;}value=source_scale_;return true;}
const std::string* RetainedCharacterActorV1::source_string(std::uint32_t offset)const noexcept{if(offset==0x30)return &source_name_;if(offset==0x48)return &source_archetype_;auto i=source_strings_.find(offset);return i==source_strings_.end()?nullptr:&i->second;}
bool RetainedCharacterActorV1::init_post_fields(NpcInitPostBorrowV1& out,std::string& e){
 auto waypoint=source_strings_.find(0x13e8),model=source_strings_.find(0x290);
 if(!object||(!session&&!initialization_properties_)||!spawn_probability_written_||!source_scale_written_||!source_rotation_written_||waypoint==source_strings_.end()||model==source_strings_.end()){
  e="required completed canonical properties and same allocated Character graph before InitPost";return false;
 }
 out={object->identity,&init_post_called1394_,&spawn_probability_,&waypoint->second,&room64_,&char_properties_id13c8_,initialization_properties_?initialization_properties_:&session->property_view(),&model->second,source_scale_.data(),nullptr,&self_fx_offset1488_,&self_fx1484_,&visual2d8_,&fade1440_,&fade1444_,object->position.data(),initial_position1450_.data(),runtime.rotation.rotation,initial_rotation145c_.data(),this,live_delayed};return true;
}
bool RetainedCharacterActorV1::construct_fields(std::shared_ptr<ScriptCharacterObject> same,WorldNpcStateServicesV1 services){
 if(graph_attempted_||!identity_||!world_pin_||!same||same->identity!=identity_||!same->properties||!same->life){error_="required same registered Character graph/World/resource lifetime";return false;}
 if(source_position_written_&&same->position!=source_position_){error_="registered Character position differs from canonical property owner";return false;}
 graph_attempted_=true;object=std::move(same);
 controller=std::make_unique<CharacterWorldNpcControllerV1>(identity_);
 // Facts/body/predicate pointers are the retained fields, never temporaries.
 services.facts=&facts;services.bodies=&bodies;services.predicates=&predicates;
 machine=std::make_unique<CharacterWorldNpcStateOwnerV1>(identity_,services);
 ai_owner={identity_,controller->identity(),reinterpret_cast<std::uintptr_t>(&machine->native_fsm()),reinterpret_cast<std::uintptr_t>(object->properties.get()),0,0,0,0};
 ai_events={object->target.identity,&ai_owner,character_idle_ai_keys(),0,character_idle_external_keys(),0,0,0,0,0,0};
 state_changed=std::make_unique<CharacterWorldNpcStateChangedV1>(*machine,ai_events);
 return true;
}
std::uint32_t* RetainedCharacterActorV1::live_delayed(void* p){auto& a=*static_cast<RetainedCharacterActorV1*>(p);return a.session?&a.session->owner().lifecycle().delayed:&a.initial_delayed3ec_;}
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
 return true;
}
bool RetainedCharacterActorV1::load_script(){
 if(!session||script_load_attempted_){error_="required fresh retained script load attempt";return false;}
 script_load_attempted_=true;
 if(session->start()!=0||!session->error().empty()||session->owner().last_source_load_status()!=0){error_=session->error();return false;}
 ScriptSessionView active{};if(!session->owner().active(active)){error_="required actual started MonsterSession active owner";return false;}
 ai_events.active=active.identity;return true;
}
}
