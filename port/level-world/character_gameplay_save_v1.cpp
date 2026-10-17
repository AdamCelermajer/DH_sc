#include "character_gameplay_save_v1.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::character_gameplay_save_v1 {namespace {
struct Range {std::uintptr_t begin,end;};
template<class T>bool range(const T* p,Range& out){const auto a=reinterpret_cast<std::uintptr_t>(p);if(!p||a%alignof(T)||a>UINTPTR_MAX-sizeof(T))return false;out={a,a+sizeof(T)};return true;}
bool overlap(Range a,Range b){return a.begin<b.end&&b.begin<a.end;}
bool valid(const Character& c){Range s;return c.identity&&range(c.current_save_14e8,s);}
bool separate_save(const SaveRef& s,Range out){Range r,v;if(!range(&s,r)||!range(s.save,v)||overlap(out,r)||overlap(out,v))return false;
 for(const auto* q:{s.quest_character_174,s.quest_character_114}){
  if(q){Range a;if(!range(q,a)||overlap(out,a))return false;}
 }
 return true;
}
bool valid_save(const SaveRef* s){Range r,v;return range(s,r)&&s->identity&&range(s->save,v)&&!overlap(r,v);}
bool valid_player_stores(const SaveRef& s){
 Range r,v,a,b;
 return range(&s,r)&&range(s.save,v)&&range(s.quest_character_174,a)&&range(s.quest_character_114,b)&&
  !overlap(r,v)&&!overlap(r,a)&&!overlap(r,b)&&!overlap(a,b)&&
  s.quest_character_174==&s.save->source_quest_log_118().character_5c&&
  s.quest_character_114==&s.save->source_quest_log_b8().character_5c;
}
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
}
Runtime::Runtime(Character c,Services s):character_(c),services_(s){if(!valid(c))throw std::invalid_argument("Actual Character and canonical Save slot required");}
Status Runtime::initialize_player_savegame(Result* r,std::string& e){return execute(0,0,r,e);}
Status Runtime::set_player(std::uintptr_t p,Result* r,std::string& e){return execute(1,p,r,e);}
Status Runtime::set_slot(std::uint32_t s,Result* r,std::string& e){return execute(2,s,r,e);}
Status Runtime::load(std::int32_t m,Result* r,std::string& e){return execute(3,std::uint32_t(m),r,e);}
Status Runtime::init_all(Result* r,std::string& e){return execute(4,0,r,e);}
Status Runtime::execute(unsigned operation,std::uintptr_t argument,Result* out,std::string& error){
 if(busy_)return Status::busy;
 Range r,e,t,slot;if(!valid(character_)||!range(out,r)||!range(&error,e)||!range(this,t)||!range(character_.current_save_14e8,slot)||overlap(r,e)||overlap(r,t)||overlap(e,t)||overlap(r,slot)||overlap(e,slot))return Status::invalid_argument;
 const auto* prior=operation>=1&&operation<=3?*character_.current_save_14e8:nullptr;
 if(prior&&(!valid_save(prior)||!separate_save(*prior,r)||!separate_save(*prior,e)))return Status::invalid_argument;
 *out={};out->captured_character=character_.identity;out->mask=signed_word(std::uint32_t(argument));error.clear();busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
 const auto fail=[&](const char* message){if(error.empty())error=message;return Status::failed;};
 const auto provider=[&](const Request& q,SaveRef*& response){++out->provider_calls;if(!services_.invoke){error="Reached unavailable Character initialization provider";return false;}return services_.invoke(services_.context,q,response,error)==0;};
 const auto player_stores=[&](SaveRef* save,std::uintptr_t player){
  out->captured_save=save?save->identity:0;if(!save)return true;
  if(!valid_save(save)||!valid_player_stores(*save)||!separate_save(*save,r)||!separate_save(*save,e)){error="Actual Save and both quest Character fields required";return false;}
  out->stage=Stage::quest_174;*save->quest_character_174=player;++out->stores;
  out->stage=Stage::save_10;save->save->set_character_only(player);++out->stores;
  out->stage=Stage::quest_114;*save->quest_character_114=player;++out->stores;return true;
 };
 try{
  if(operation==0){
   out->stage=Stage::allocate;SaveRef* allocated=nullptr;Request request{Operation::allocate_save,character_.identity,nullptr,0x198,0,0};
   if(!provider(request,allocated))return fail("Save allocation provider failed");
   if(!valid_save(allocated)||!separate_save(*allocated,r)||!separate_save(*allocated,e))return fail("Save allocation did not return actual stable backing");
   out->captured_save=allocated->identity;out->stage=Stage::construct;SaveRef* response=allocated;request={Operation::construct_blank_save,character_.identity,allocated,0,0,0};
   if(!provider(request,response))return fail("Blank Save constructor provider failed");
   if(!valid_save(allocated)||!valid_player_stores(*allocated)||!separate_save(*allocated,r)||!separate_save(*allocated,e))return fail("Save constructor changed its captured backing");
   out->stage=Stage::publish;*character_.current_save_14e8=allocated;++out->stores;
   if(!player_stores(*character_.current_save_14e8,character_.identity))return fail("SG_SetPlayer provider boundary failed");
  }else if(operation==1){if(!player_stores(*character_.current_save_14e8,argument))return fail("SG_SetPlayer provider boundary failed");
  }else if(operation==2){
   auto* save=*character_.current_save_14e8;out->captured_save=save?save->identity:0;
   if(save){out->stage=Stage::slot_4;save->save->set_slot(signed_word(std::uint32_t(argument)));++out->stores;}
  }else if(operation==3){
   auto* save=*character_.current_save_14e8;out->captured_save=save?save->identity:0;
   if(save){out->stage=Stage::load;if(!save->loader||&save->loader->save()!=save->save)return fail("Actual matching gameplay Save LoadOwner required");++out->load_calls;if(!save->loader->load(out->mask,error))return fail("Gameplay SG_Load failed");}
  }else{
   SaveRef* unused=nullptr;out->stage=Stage::init_post;
   if(!provider({Operation::virtual_init_post,character_.identity,nullptr,0,0,0x1c},unused))return fail("Character InitPost virtual failed");
   out->stage=Stage::init_final;
   if(!provider({Operation::virtual_init_final,character_.identity,nullptr,0,0,0x58},unused))return fail("Character InitFinal virtual failed");
  }
  out->stage=Stage::complete;return Status::complete;
 }catch(const std::exception& x){if(error.empty())error=x.what();return Status::failed;}catch(...){return fail("Character initialization provider threw");}
}
}
