#include "player_kill_continuation_v1.hpp"
#include <stdexcept>

namespace dh2::player_kill_continuation_v1 {
namespace {
struct Range {std::uintptr_t first,end;};
bool range(const void* p,std::size_t n,std::size_t alignment,Range& r){
 const auto a=reinterpret_cast<std::uintptr_t>(p);
 if(!p||a%alignment||a>UINTPTR_MAX-n)return false;
 r={a,a+n};return true;
}
bool overlap(Range a,Range b){return a.first<b.end&&b.first<a.end;}
bool separate(Range a,const void* p,std::size_t n,std::size_t alignment=1){
 Range b{};return range(p,n,alignment,b)&&!overlap(a,b);
}
bool outputs(Result* out,std::string& error,const void* runtime,std::size_t size,const Bindings* b=nullptr){
 Range r{},e{};
 if(!range(out,sizeof(*out),alignof(Result),r)||!range(&error,sizeof(error),alignof(std::string),e)||overlap(r,e)||
    !separate(r,runtime,size)||!separate(e,runtime,size))return false;
 if(!b)return true;
 if(!b->dead||!b->properties||dh2_property_validate(b->properties))return false;
 const auto& v=*b->properties;
 for(auto x:{r,e}){
  if(!separate(x,b->dead,sizeof(*b->dead),alignof(std::uint32_t))||!separate(x,b->properties,sizeof(v),alignof(data::PropertyView)))return false;
  for(auto sheet:{v.defaults,v.types,v.base,static_cast<const std::int32_t*>(v.saved),v.gear,static_cast<const std::int32_t*>(v.resolved)})
   if(!separate(x,sheet,224*sizeof(*sheet),alignof(std::int32_t)))return false;
  if(v.group_count&&!separate(x,v.groups,v.group_count*sizeof(*v.groups),alignof(data::PropertyBuffGroup)))return false;
  for(std::uint32_t i=0;i<v.group_count;++i){const auto& g=v.groups[i];
   if(g.count&&!separate(x,g.sheets,g.count*sizeof(*g.sheets),alignof(const std::int32_t*)))return false;
   for(std::uint32_t j=0;j<g.count;++j)if(!separate(x,g.sheets[j],224*sizeof(std::int32_t),alignof(std::int32_t)))return false;
  }
 }
 return true;
}
struct Busy {bool& value;explicit Busy(bool& v):value(v){value=true;}~Busy(){value=false;}};
struct Run {
 std::uintptr_t character,killer;std::uint32_t force;data::PropertyView* properties;
 Backend backend;Result& result;std::string& error;
 bool call(Operation op,Reply& reply,std::uintptr_t subject=0,std::int32_t argument=0,std::int32_t index=0,const char* name=nullptr){
  result.last_operation=std::uint32_t(op);++result.calls;reply={};
  Request q{op,character,killer,subject,properties,argument,index,force,name};
  if(!backend.invoke||backend.invoke(backend.context,&q,&reply,error)){
   if(error.empty())error="required source Kill service failed";
   return false;
  }
  return true;
 }
 bool manager(Reply& r){
  if(!call(Operation::application_player_manager,r))return false;
  if(r.identity&&r.player_manager)return true;
  error="actual Application/PlayerManager missing";return false;
 }
 Status continuation(){
  Reply r{};
  if(!call(Operation::is_player,r,character))return Status::failed;
  if(!r.word||force){
   if(!call(Operation::general_continuation,r,character))return Status::failed;
   result.general_completed=1;return Status::complete;
  }
  if(!call(Operation::property_add_int,r,character,1,25))return Status::failed;
  result.death_count_added=1;
  if(!manager(r))return Status::failed;
  const auto player_manager=r.player_manager;
  if(!call(Operation::is_local_player,r,player_manager))return Status::failed;
  result.local_player=r.word!=0;
  if(r.word){
   if(!call(Operation::trophy_manager,r))return Status::failed;
   const auto trophy=r.identity;
   if(!trophy){error="actual TrophyManager missing";return Status::failed;}
   const std::int32_t thresholds[]={10,50,100};
   const char* names[]={"quest_died_10_times","quest_died_50_times","quest_died_100_times"};
   for(unsigned i=0;i<3;++i){
    if(!call(Operation::property_get_int,r,character,0,25))return Status::failed;
    ++result.property_reads;result.last_death_count=r.word;
    if(r.word!=thresholds[i])continue;
    if(!call(Operation::trophy_id,r,0,0,0,names[i]))return Status::failed;
    result.trophy_index=r.word;
    if(!call(Operation::trophy_unlock,r,trophy,result.trophy_index))return Status::failed;
    ++result.trophy_calls;break;
   }
  }
  if(!call(Operation::online,r))return Status::failed;
  result.online=r.word!=0;
  if(r.word){
   if(!manager(r))return Status::failed;
   const auto fresh_manager=r.player_manager;
   if(!call(Operation::get_local_player,r,fresh_manager,1,0))return Status::failed;
   result.local_player_queried=1;
  }
  return Status::complete;
 }
};
template<class F>Status guarded(std::string& error,F f){
 try{return f();}catch(const std::exception& e){error=e.what();return Status::failed;}
 catch(...){error="source Kill provider exception";return Status::failed;}
}
}
Runtime::Runtime(Bindings b):bindings_(b){
 Range d{},v{};
 if(!b.character||!range(b.dead,sizeof(*b.dead),alignof(std::uint32_t),d)||
    !range(b.properties,sizeof(*b.properties),alignof(data::PropertyView),v)||overlap(d,v)||
    !b.backend.invoke||dh2_property_validate(b.properties))throw std::invalid_argument("invalid borrowed source Kill owners");
}
Status Runtime::continue_after_hp0(std::uintptr_t killer,std::uint32_t force,Result* out,std::string& error){
 if(busy_)return Status::busy;
 if(consumed_)return Status::consumed;
 if(force>1||!outputs(out,error,this,sizeof(*this),&bindings_)||*bindings_.dead!=1||bindings_.properties->resolved[36]!=0)return Status::invalid_argument;
 Busy busy(busy_);consumed_=true;*out={};error.clear();
 Run run{bindings_.character,killer,force,bindings_.properties,bindings_.backend,*out,error};
 return guarded(error,[&]{return run.continuation();});
}
CtrlCaller::CtrlCaller(std::uintptr_t c,Backend b):character_(c),backend_(b){
 if(!c||!b.invoke)throw std::invalid_argument("invalid borrowed Ctrl_Kill services");
}
Status CtrlCaller::kill(std::uintptr_t killer,std::uint32_t force,Result* out,std::string& error){
 if(busy_)return Status::busy;
 if(failed_)return Status::consumed;
 if(force>1||!outputs(out,error,this,sizeof(*this)))return Status::invalid_argument;
 Busy busy(busy_);*out={};error.clear();Run run{character_,killer,force,nullptr,backend_,*out,error};
 const auto status=guarded(error,[&]{Reply r{};
  if(!run.call(Operation::is_dead,r,character_))return Status::failed;
  if(r.word){out->skipped=1;return Status::complete;}
  if(!run.call(Operation::kill,r,character_))return Status::failed;
  out->kill_completed=1;
  if(!run.call(Operation::event2,r,character_,2))return Status::failed;
  out->event2_completed=1;return Status::complete;
 });
 if(status==Status::failed)failed_=true;
 return status;
}
}
