#include "npc_animation_event_owner_v1.hpp"
#include "../script-runtime/script_function_alias.h"
#include <cstring>
namespace dh2::character {
bool NpcAnimationEventOwnerV1::foot(bool left,float* out){
 skills::WorldTargetActorBorrowV1 actor{};
 if(!b_.world||b_.world->actor(b_.character,&actor)||!actor.position||!actor.target_node||
  (*actor.target_node&&!actor.target_enabled)){error_="Required actual NPC foot target-position fields";return false;}
 const auto* point=skills::dh2_world_target_position_v1(actor.position,actor.cached_target_position,*actor.target_node,actor.target_enabled?*actor.target_enabled:0);
 if(!point||!b_.visual){error_="Required actual NPC foot visual/GetTargetPosition";return false;}
 std::memcpy(out,point,12);if(!*b_.visual)return true;
 if(!b_.scene||!b_.visual_root||actor.scene!=b_.scene){error_="Required SAME NPC foot Scene/visual root";return false;}
 int node{};if(animation_specific_node_v1(*b_.scene,*b_.visual_root,left?"Bip01_L_Foot":"Bip01_R_Foot",node)){error_="NPC original specific-foot node lookup failed";return false;}
 if(node>=0){const auto& world=b_.scene->graph[std::size_t(node)].world;out[0]=world[12];out[1]=world[13];out[2]=world[14];}return true;
}
bool NpcAnimationEventOwnerV1::play(int set,const float* point){
 if(!effects_){error_="Required actual NPC Effects table lease";return false;}
 if(set<0||std::size_t(set)>=effects_.sets().size())return true;
 if(!mesh_){error_="Required actual NPC foot VisualFXManager PlayAnimFXSet";return false;}
 std::uintptr_t created{};return mesh_->play_set(set,point,nullptr,0,&created,error_);
}
bool NpcAnimationEventOwnerV1::relay(const char* event,const dh2_script_callback_scope* scope){
 error_.clear();lua_error_=0;
 if(!event||!b_.ai){error_="Malformed NPC source animation event";return false;}
 if(!b_.ai->active)return true;
 ScriptSessionView selected{};
 if(!b_.script||!b_.script->find(b_.ai->active,selected)||!selected.vm||!selected.aliases||
  !b_.ai->owner||b_.ai->owner->owner!=b_.character||!b_.ai->ais_virtuals||
  b_.ai->ais_virtuals[0x94/4]!=0x3dca50||!b_.animator_lag){error_="Required actual selected NPC AIS OnAnimEvent identity/source owner";return false;}
 if(scope&&(scope->vm!=selected.vm||!dh2_script_callback_scope_valid(scope))){error_="NPC OnAnimEvent private scope mismatch";return false;}
 dh2_script_value args[2]{};args[0].type=DH2_SCRIPT_STRING;args[0].text=event;args[0].text_bytes=std::strlen(event);
 args[1].type=DH2_SCRIPT_NUMBER;args[1].number=float(*b_.animator_lag);
 if(scope){
  const auto* name=dh2_script_alias_resolve(selected.aliases,"OnAnimEvent");
  const int status=dh2_script_callback_call_discard_source_objects(scope,name,args,2);
  if(status<0){error_="Required scoped NPC OnAnimEvent delivery failed";return false;}lua_error_=std::uint32_t(status);
 }else if(b_.script->call_discard(selected.identity,"OnAnimEvent",args,2,lua_error_)){
  error_=b_.script->error();if(error_.empty())error_="Required NPC OnAnimEvent delivery failed";return false;
 }
 if(std::strcmp(event,"step_left")&&std::strcmp(event,"step_right"))return true;
 float point[3];if(!foot(std::strcmp(event,"step_right")!=0,point))return false;
 if(!effects_||effects_.characters().empty()||!b_.properties||!b_.properties->resolved){error_="Required SAME NPC CharacterFX property/table";return false;}
 auto row=[&]()->const data::CharacterEffects&{const int id=b_.properties->resolved[7];return effects_.characters()[id>=0&&std::size_t(id)<effects_.characters().size()?std::size_t(id):0];};
 if(!play(row().footprint,point))return false;
 const auto& current=row();const char* floor{};
 if(!b_.floor_type||b_.floor_type(b_.floor_context,&floor)){error_="Required actual NPC cached PFFloor type";return false;}
 if(!current.trigger_floor_fx||!floor)return true;
 for(const auto& step:effects_.footsteps())if(!std::strcmp(step.floor_type.c_str(),floor))return play(step.effect,point);
 return true;
}
}
