#include "character_init_final_v1.hpp"
#include <stdexcept>
namespace dh2::character_init_final_v1 {namespace {
struct Range{std::uintptr_t b,e;};
template<class T>bool range(const T* p,Range& r){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;r={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.b<b.e&&b.b<a.e;}
}
Runtime::Runtime(State* s,Services services):state_(s),services_(services){Range r;if(!range(s,r)||!s->character)throw std::invalid_argument("Actual Character InitFinal fields required");}
Status Runtime::initialize(Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range s,r,e,t,v;if(!range(state_,s)||!state_->character||!range(out,r)||!range(&error,e)||!range(this,t)||overlap(s,r)||overlap(s,e)||overlap(s,t)||overlap(r,e)||overlap(r,t)||overlap(e,t))return Status::invalid_argument;
 if(state_->visual_2d8&&(!range(state_->visual_2d8,v)||overlap(v,s)||overlap(v,r)||overlap(v,e)||overlap(v,t)))return Status::invalid_argument;
 *out={};out->captured_character=state_->character;error.clear();busy_=true;struct Scope{bool& b;~Scope(){b=false;}}scope{busy_};
 const auto fail=[&](const char* message){if(error.empty())error=message;return Status::failed;};
 const auto call=[&](Operation op,Reply& reply,std::uintptr_t subject=0,std::uint32_t a=0,std::uint32_t b=0,const char* name=nullptr,const float* vector=nullptr){
  out->last_operation=std::uint32_t(op);++out->entered_calls;reply={};if(!services_.invoke){error="Reached missing Character InitFinal provider";return false;}
  return services_.invoke(services_.context,*state_,{op,subject?subject:out->captured_character,a,b,name,vector},reply,error)==0;
 };
 try{
  if(state_->initialized_1395){out->decision=Decision::already_initialized;return Status::complete;}
  state_->initialized_1395=1;++out->stores;Reply reply;
  if(!call(Operation::spawn_probability,reply))return fail("Spawn probability provider failed");
  if(reply.word>=state_->spawn_probability_274){out->decision=Decision::probability_rejected;return Status::complete;}
  if(!call(Operation::base_init_final,reply)||!call(Operation::is_faerie,reply))return fail("Base final/Faerie provider failed");
  bool follow=reply.word!=0;
  if(!follow){if(!call(Operation::is_follower,reply))return fail("Follower provider failed");follow=reply.word!=0;}
  if(follow){
   if(!call(Operation::local_player,reply,0,0,1))return fail("Local Player provider failed");
   out->local_character=reply.character_660;
   if(out->local_character){
    if(!call(Operation::look_at_vector,reply,out->local_character))return fail("Look-at vector provider failed");
    float position[3]{reply.vector[0],reply.vector[1],reply.vector[2]};
    if(!call(Operation::target_position,reply,out->local_character))return fail("Target position provider failed");
    const auto target=reinterpret_cast<std::uintptr_t>(reply.target);
    if(!reply.target||target%alignof(float)||target>UINTPTR_MAX-3*sizeof(float))return fail("Actual local-player target position required");
    // Each original soft-float addition stores binary32 before the next axis.
    position[0]=position[0]+reply.target[0];position[1]=position[1]+reply.target[1];position[2]=position[2]+reply.target[2];
    if(!call(Operation::set_position,reply,0,1,0,nullptr,position))return fail("Source SetPosition provider failed");
   }
  }
  if(state_->visual_2d8){
   if(!call(Operation::is_player,reply))return fail("Visual Player classification failed");
   const auto* name=reply.word?"SceneLight":"MonsterLight";
   if(!call(Operation::light_set,reply,0,0,0,name))return fail("LightSetManager lookup failed");
   auto* visual=state_->visual_2d8;
   if(!range(visual,v)||!visual->identity||overlap(v,s)||overlap(v,r)||overlap(v,e)||overlap(v,t))return fail("Source light publication reached unavailable visual");
   visual->light_set_40=reply.word;++out->stores;
  }
  if(!call(Operation::char_ai_init_final,reply)||!call(Operation::is_player,reply))return fail("CharAI final/Player classification failed");
  if(reply.word){
   if(!call(Operation::is_local_player,reply))return fail("Locality provider failed");
   if(reply.word){
    if(!call(Operation::update_all_skills,reply)||!call(Operation::recalculate,reply,0,1)||!call(Operation::property_int,reply,0,0xc2,0))return fail("Skill/recalc/property provider failed");
    state_->property_194_byte_3a8=std::uint8_t(reply.word<0?0:std::uint32_t(reply.word));++out->stores;
    if(!call(Operation::save,reply))return fail("Source SG_Save writer required");
   }
  }
  out->decision=Decision::initialized;return Status::complete;
 }catch(const std::exception& x){if(error.empty())error=x.what();return Status::failed;}catch(...){return fail("Character InitFinal provider threw");}
}
}
