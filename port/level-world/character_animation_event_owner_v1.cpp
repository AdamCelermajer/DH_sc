#include "character_animation_event_owner_v1.hpp"
#include "character_mesh_fx_owner_v4.hpp"
#include <cstring>
#include <limits>
namespace dh2::character {
int animation_floor_type_v1(void* p,const char** out){
 if(!p||!out)return -1;
 const auto& b=*static_cast<AnimationFloorBorrowV1*>(p);
 if(!b.cached_floor)return -1;
 if(*b.cached_floor==std::numeric_limits<std::uint32_t>::max()){*out=nullptr;return 0;}
 if(!b.world||*b.cached_floor>=b.world->records.size()||!b.world->records[*b.cached_floor])return -1;
 *out="";return 0;
}
int animation_specific_node_v1(const scene::Scene& s,std::uint32_t root,const char* name,std::int32_t& result){
 result=-1;if(!name||s.graph.size()>65536)return -1;
 std::vector<std::uint32_t> pending;
 if(root==std::numeric_limits<std::uint32_t>::max()){
  for(std::size_t i=s.graph.size();i--;)if(s.graph[i].parent==-1)pending.push_back(static_cast<std::uint32_t>(i));
 }else{
  if(root>=s.graph.size())return -1;
  pending.push_back(root);
 }
 std::uint32_t visits{};
 while(!pending.empty()){
  auto id=pending.back();pending.pop_back();if(++visits>s.graph.size())return -1;
  if(!std::strcmp(s.graph[id].name.c_str(),name)){result=static_cast<std::int32_t>(id);return 0;}
  // Reverse push retains the original child list order during DFS.
  for(std::size_t i=s.graph.size();i--;)if(s.graph[i].parent==static_cast<std::int32_t>(id))pending.push_back(static_cast<std::uint32_t>(i));
 }
 return 0;
}
CharacterAnimationEventOwnerV1::CharacterAnimationEventOwnerV1(AIEventState64& ai,
 AnimationEventBorrowV1 actor,data::EffectsTables::Borrow tables,fx::CharacterMeshFxOwnerV1* mesh):
 ai_(ai),actor_(actor),tables_(std::move(tables)),mesh_(mesh){}
bool CharacterAnimationEventOwnerV1::foot(bool left,float out[3]){
 skills::WorldTargetActorBorrowV1 b{};
 if(!actor_.world||actor_.world->actor(actor_.character,&b)||!b.position||!b.target_node){
  error_="Required source foot GetTargetPosition borrow unavailable";return false;
 }
 if(*b.target_node&&(!b.target_enabled||(*b.target_enabled&&!b.cached_target_position))){
  error_="Required source foot target-node producer unavailable";return false;
 }
 const float* position=skills::dh2_world_target_position_v1(b.position,b.cached_target_position,*b.target_node,b.target_enabled?*b.target_enabled:0);
 if(!position){error_="Required source foot target position unavailable";return false;}
 std::memcpy(out,position,3*sizeof(float));
 if(!actor_.visual){error_="Required source VisualObject borrow unavailable";return false;}
 if(!*actor_.visual)return true;
 if(!actor_.scene||!actor_.visual_root||b.scene!=actor_.scene){
  error_="Required same-scene visual foot root unavailable";return false;
 }
 std::int32_t node{};
 if(animation_specific_node_v1(*actor_.scene,*actor_.visual_root,left?"Bip01_L_Foot":"Bip01_R_Foot",node)){
  error_="Required source visual foot node lookup failed";return false;
 }
 if(node>=0){const auto& world=actor_.scene->graph[static_cast<std::uint32_t>(node)].world;out[0]=world[12];out[1]=world[13];out[2]=world[14];}
 return true;
}
bool CharacterAnimationEventOwnerV1::play(std::int32_t set,const float position[3]){
 // Whole source PlayAnimFXSet's invalid-index early exit, before manager data.
 if(set<0||static_cast<std::size_t>(set)>=tables_.sets().size())return true;
 if(mesh_v4_){std::uintptr_t created{};return mesh_v4_->play_set(set,position,nullptr,0,&created,error_);}
 if(!mesh_){error_="Required source VisualFXManager PlayAnimFXSet owner unavailable";return false;}
 std::uintptr_t created{};return mesh_->play_set(set,position,nullptr,0,&created,error_);
}
bool CharacterAnimationEventOwnerV1::default_event(const char* event,const dh2_script_callback_scope* scope){
 if(!actor_.script_owner||!actor_.script_identity||!actor_.animator_lag){
  error_="Required source OnAnimEvent Script/animator-lag borrow unavailable";return false;
 }
 dh2_script_value args[2]{};args[0].type=DH2_SCRIPT_STRING;args[0].text=event;args[0].text_bytes=std::strlen(event);
 args[1].type=DH2_SCRIPT_NUMBER;args[1].number=static_cast<float>(*actor_.animator_lag);
 if(scope){
  ScriptSessionView selected{};
  if(!actor_.script_owner->find(*actor_.script_identity,selected)||scope->vm!=selected.vm||!dh2_script_callback_scope_valid(scope)){
   error_="Required SAME selected player OnAnimEvent callback scope";return false;
  }
  const auto* name=dh2_script_alias_resolve(selected.aliases,"OnAnimEvent");
  const int status=dh2_script_callback_call_discard_source_objects(scope,name,args,2);
  if(status<0){error_="Required scoped player OnAnimEvent delivery failed";return false;}lua_error_=static_cast<std::uint32_t>(status);
 }else if(actor_.script_owner->call_discard(*actor_.script_identity,"OnAnimEvent",args,2,lua_error_)){
  error_=actor_.script_owner->error();if(error_.empty())error_="Required source OnAnimEvent Lua delivery failed";return false;
 }
 if(std::strcmp(event,"step_left")&&std::strcmp(event,"step_right"))return true;
 float position[3]{};if(!foot(std::strcmp(event,"step_right")!=0,position))return false;
 if(!tables_||tables_.characters().empty()||!actor_.properties||!actor_.properties->resolved){
  error_="Required same CharacterFX cache/table borrow unavailable";return false;
 }
 const auto select=[&]()->const data::CharacterEffects&{
  const auto id=actor_.properties->resolved[7];
  return tables_.characters()[id>=0&&static_cast<std::size_t>(id)<tables_.characters().size()?static_cast<std::size_t>(id):0];
 };
 if(!play(select().footprint,position))return false;
 const auto& row=select(); // source reloads cached CharacterFX after Play.
 const char* floor{};
 if(!actor_.floor_type||actor_.floor_type(actor_.floor_context,&floor)){
  error_="Required source cached PFFloor type producer unavailable";return false;
 }
 if(!row.trigger_floor_fx||!floor)return true;
 for(const auto& step:tables_.footsteps())if(!std::strcmp(step.floor_type.c_str(),floor))return play(step.effect,position);
 return true;
}
bool CharacterAnimationEventOwnerV1::relay(const char* event,const dh2_script_callback_scope* scope){
 error_.clear();lua_error_=0;if(!event){error_="Malformed source animation event";return false;}
 if(!ai_.active)return true;
 if(!ai_.owner||ai_.owner->owner!=actor_.character||!ai_.ais_virtuals||
  !actor_.script_identity||*actor_.script_identity!=ai_.active){
  error_="Required same Character/AIS animation relay borrow unavailable";return false;
 }
 if(ai_.ais_virtuals[0x94/4]!=0x3dca50){error_="Required selected AIS OnAnimEvent implementation unavailable";return false;}
 return default_event(event,scope);
}
}
