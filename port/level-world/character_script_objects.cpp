#include "character_script_objects.hpp"
#include "character_target_providers.hpp"
#include "gameobject_lua_representation.hpp"
#include "character_spatial_bindings.hpp"
#include <cstdio>
#include <cstring>
#include <stdexcept>
namespace dh2::character {
ScriptCharacterObject::ScriptCharacterObject(std::uintptr_t id,std::string text,
 std::shared_ptr<data::PropertyState> p,std::shared_ptr<data::CombatActorState> c,const std::array<float,3>& point):
 identity(id),name(std::move(text)),properties(std::move(p)),life(std::move(c)),position(point){
 owner.identity=identity;target.owner=&owner;
 // This is the retained native CharAI record identity, distinct from Character.
 target.identity=reinterpret_cast<std::uintptr_t>(&target);
 binding.state=&target;
}
CharacterScriptObjects::CharacterScriptObjects(CharacterGameDesign::Borrow&& d,
 DebugSwitches* debug,const DebugFileServices24* files):design_(std::move(d)),debug_(debug),files_(files){
 if(!design_||!design_.ai()||!debug_||!files_||!files_->open_read||!files_->close_read)
  throw std::invalid_argument("Missing scene script object dependencies");
 for(const auto& row:design_.ai()->rows)ai_types_.push_back(row.type);
}
std::shared_ptr<ScriptCharacterObject> CharacterScriptObjects::add(std::uintptr_t id,
 const std::string& name,std::shared_ptr<data::PropertyState> p,
 std::shared_ptr<data::CombatActorState> c,const std::array<float,3>& point){
 if(!id||!p||!c||c->dead>255||records_.count(id))throw std::invalid_argument("Invalid/duplicate scene script identity");
 auto object=std::make_shared<ScriptCharacterObject>(id,name,std::move(p),std::move(c),point);
 object->binding.services={this,target};records_.emplace(id,object);return object;
}
std::shared_ptr<ScriptCharacterObject> CharacterScriptObjects::find(std::uintptr_t id) const noexcept{
 const auto found=records_.find(id);return found==records_.end()?nullptr:found->second;
}
bool CharacterScriptObjects::publish_existing_v62(const std::shared_ptr<ScriptCharacterObject>& object,std::string& e){
 if(!object||!object->identity||!object->properties||!object->life||object->target.owner!=&object->owner||
    object->owner.identity!=object->identity||object->binding.state!=&object->target){e="Required SAME canonical Character Lua facade";return false;}
 auto found=records_.find(object->identity);
 if(found!=records_.end()){if(found->second==object){e.clear();return true;}e="Character Lua identity already refers to another receiver";return false;}
 if(object->binding.services.context||object->binding.services.invoke){e="Canonical Character target facade already bound elsewhere";return false;}
 object->binding.services={this,target};records_.emplace(object->identity,object);e.clear();return true;
}
int CharacterScriptObjects::type(void* p,std::uintptr_t id,const char** out){
 return p&&static_cast<CharacterScriptObjects*>(p)->find(id)?
  gameobject_lua::dh2_gameobject_lua_type(out,gameobject_lua::character):1;
}
int CharacterScriptObjects::methods(void* p,std::uintptr_t id,const dh2_script_object_method** out,std::uint32_t* n){
 return p&&static_cast<CharacterScriptObjects*>(p)->find(id)?
  gameobject_lua::dh2_gameobject_lua_methods(out,n,gameobject_lua::character):1;
}
int CharacterScriptObjects::invoke(void* p,const dh2_script_callback_scope* scope,
 std::uintptr_t id,std::uint32_t source,const dh2_script_value* arguments,std::uint32_t count,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!p||!dh2_script_callback_scope_valid(scope)||!out||!capacity||!returned)return 1;
 auto object=static_cast<CharacterScriptObjects*>(p)->find(id);if(!object)return 1;
 if(source==0x38ebe4)return gameobject_lua::dh2_gameobject_lua_get_id(out,returned,id);
 if(source==0x3b6c7c)return gameobject_lua::dh2_gameobject_lua_get_target(out,returned,&object->target);
 if(source==0x38e700){
  // ObjectMethod captures self._this before projecting its other arguments.
  // Resolve that retained Character and read its live raw point now, rather
  // than the session owner's point or the opaque identity as a pointer.
  SpatialBindings40 spatial{};spatial.owner_position=object->position.data();
  return dh2_character_get_position(&spatial,arguments,count,out,capacity,returned,error,size);
 }
 *returned=0;if(error&&size)std::snprintf(error,size,"Unreconstructed scene Character method 0x%08x",source);
 return 1;
}
int CharacterScriptObjects::target(void* p,TargetState48* state,const TargetRequest24* r,std::uint32_t* out){
 if(!p||!state||!r||!out||r->reserved)return 1;
 auto& self=*static_cast<CharacterScriptObjects*>(p);
 if(!state->owner)return 1;
 auto owner=self.find(state->owner->identity);
 if(!owner||state!=&owner->target)return 1;
 if(r->service==target_debug_load)return dh2_character_debug_load(self.debug_,self.files_)<0;
 if(r->service==target_debug_query)return !r->text||dh2_character_debug_get(out,self.debug_,r->text,self.files_)<0;
 if(r->service==target_owner_ai_id){
  auto object=self.find(r->subject);if(!object)return 1;std::int32_t id=0;
  const auto status=dh2_character_target_ai_id(&id,object->properties->resolved.data(),self.design_.ai()->rows.size());
  std::memcpy(out,&id,4);return status;
 }
 auto object=self.find(r->subject);if(!object)return 1;
 if(r->service==target_virtual_dead){
  if(object->life->dead>255)return 1;
  target_providers::Character32 character{};character.identity=object->identity;
  CombatProperties896 properties{};std::memcpy(properties.words,object->properties->resolved.data(),sizeof properties);
  character.properties=&properties;
  character.dead1449=static_cast<std::uint8_t>(object->life->dead);
  target_providers::Types16 types{self.ai_types_.data(),static_cast<std::uint32_t>(self.ai_types_.size()),0};
  std::int32_t result=0;
  const auto status=target_providers::dh2_character_target_query(&result,target_providers::is_dead,&character,nullptr,&types,nullptr);
  std::memcpy(out,&result,4);return status;
 }
 if(r->service==target_in_sight){
  // The original null argument reloads current target. The setter requests its
  // live target after the dead virtual, so both retained coordinates are read now.
  std::int32_t id=0;if(dh2_character_target_ai_id(&id,owner->properties->resolved.data(),self.design_.ai()->rows.size()))return 1;
  const auto* row=data::ai_props(*self.design_.ai(),id);if(!row)return 1;
  return dh2_character_target_sight(out,owner->position.data(),object->position.data(),row->view_radius);
 }
 return 1;
}
}
