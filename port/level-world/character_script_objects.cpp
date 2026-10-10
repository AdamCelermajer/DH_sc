#include "character_script_objects.hpp"
#include "character_target_providers.hpp"
#include "gameobject_lua_representation.hpp"
#include "character_spatial_bindings.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
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
bool CharacterScriptObjects::retire_native_receiver_v123(std::uintptr_t id,const std::shared_ptr<ScriptCharacterObject>& object,std::string& e){
 const auto at=records_.find(id);
 if(!object||object->identity!=id||at==records_.end()||at->second!=object||object->binding.services.context!=this||object->binding.services.invoke!=target){e="Required SAME native Character Lua facade at retirement";return false;}
 object->binding.services={};records_.erase(at);e.clear();return true;
}
bool CharacterScriptObjects::bind_source_random_channel0_v125(
 std::shared_ptr<data::LootRandom8V2> random,std::function<bool(bool&,std::string&)> online,
 std::string& e){
 if(!random||!online){e="Required same-Application Random channel 0 and GetOnline byte5";return false;}
 if(source_random_channel0_v125_){
  if(source_random_channel0_v125_.get()!=random.get()){
   e="Character Lua objects already borrow a different Application Random channel 0";return false;
  }
  if(!online_byte5_v125_){e="Bound source Random owner has no GetOnline byte5 query";return false;}
  e.clear();return true;
 }
 source_random_channel0_v125_=std::move(random);online_byte5_v125_=std::move(online);e.clear();return true;
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
 auto* self=static_cast<CharacterScriptObjects*>(p);auto object=self->find(id);if(!object)return 1;
 if(source==0x38ebe4)return gameobject_lua::dh2_gameobject_lua_get_id(out,returned,id);
 if(source==0x3b6c7c)return gameobject_lua::dh2_gameobject_lua_get_target(out,returned,&object->target);
 // Lua Character:IsDead / :IsPlayer are the GameObject wrapper callbacks,
 // not ObjectBase::IsDead/IsPlayer and not the native Character entrypoints
 // used as their virtual targets. IDA: GameObject::_IsDead (0x38ea48)
 // dispatches Character::IsDead (0x3a2ed4); _IsPlayer (0x38e9f0)
 // dispatches Character::IsPlayer (0x3a49f0). Both read this same retained
 // Character's live state. Returning BOOLEAN models pushBoolean in the
 // wrapper, including nonzero source IsDead bytes.
 if(source==0x38ea48){
  *out={};out->type=DH2_SCRIPT_BOOLEAN;out->boolean=object->life->dead!=0;*returned=1;return 0;
 }
 if(source==0x38e9f0){
  if(self->ai_types_.size()<=8){*returned=0;if(error&&size)std::snprintf(error,size,"Required source Character IsPlayer AI fallback row");return 1;}
  auto ai_id=object->properties->resolved[1];
  if(ai_id<0||static_cast<std::size_t>(ai_id)>=self->ai_types_.size())ai_id=8;
  const auto type=self->ai_types_[static_cast<std::size_t>(ai_id)];
  // Exact Character::IsPlayer order: any nonzero AI type decides the result;
  // only type zero checks whether the name contains "PlayerCharacter".
  const bool is_player=type?type==1:std::strstr(object->name.c_str(),"PlayerCharacter")!=nullptr;
  *out={};out->type=DH2_SCRIPT_BOOLEAN;out->boolean=is_player;*returned=1;return 0;
 }
 if(source==0x393310){
  *returned=0;if(count&&!arguments)return 1;
  std::uint32_t minimum=0,bound=100;
  auto to_uinteger=[&](const dh2_script_value& value,std::uint32_t& result){
   if(value.type!=DH2_SCRIPT_NUMBER||!std::isfinite(value.number))return false;
   const double truncated=std::trunc(static_cast<double>(value.number));
   if(truncated<static_cast<double>(std::numeric_limits<std::int32_t>::min())||
      truncated>static_cast<double>(std::numeric_limits<std::uint32_t>::max()))return false;
   if(truncated<0){const auto signed_value=static_cast<std::int32_t>(truncated);result=static_cast<std::uint32_t>(signed_value);}
   else result=static_cast<std::uint32_t>(truncated);
   return true;
  };
  if(count==1){if(!to_uinteger(arguments[0],bound))return 0;}
  else if(count==2){std::uint32_t maximum{};if(!to_uinteger(arguments[0],minimum)||!to_uinteger(arguments[1],maximum))return 0;bound=maximum-minimum;}
  // IDA GameObject::_Rand (0x393310): one integer means [0,n), two mean
  // [minimum,maximum), all other arities default to [0,100). Offline it calls
  // Random::GetRandom(maximum-minimum,false), adds minimum, and increments the
  // same process debug counter even when the bound is zero.
  bool online=false;
  std::string online_error;
  if(!self->online_byte5_v125_||!self->online_byte5_v125_(online,online_error)){
   *returned=0;if(error&&size)std::snprintf(error,size,"%s",online_error.empty()?"Required actual same-Application GetOnline byte5":online_error.c_str());return 1;
  }
  if(online){*returned=0;if(error&&size)std::snprintf(error,size,"Online GameObject::_Rand requires original ReturnValues+0xfc seed owner");return 1;}
  if(!self->source_random_channel0_v125_){*returned=0;if(error&&size)std::snprintf(error,size,"Required same-Application Random channel 0");return 1;}
  std::int32_t signed_bound{};std::memcpy(&signed_bound,&bound,sizeof(bound));std::int32_t draw{};
  if(dh2_loot_v2_random(self->source_random_channel0_v125_.get(),signed_bound,&draw)){
   *returned=0;if(error&&size)std::snprintf(error,size,"Original offline GameObject::_Rand provider failed");return 1;
  }
  const std::uint32_t result_bits=minimum+static_cast<std::uint32_t>(draw);std::int32_t result{};std::memcpy(&result,&result_bits,sizeof(result));
  out[0]={};out[0].type=DH2_SCRIPT_NUMBER;out[0].number=static_cast<float>(result);*returned=1;return 0;
 }
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
