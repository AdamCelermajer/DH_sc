#include "quest_objective_factory_v1.hpp"
namespace dh2::data::quest_objective_factory_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(x&&y&&an&&bn&&x<y+bn&&y<x+an);
}
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool output(const Runtime* self,const Result* out,const Record* q=nullptr){return aligned(out)&&!overlap(out,sizeof(*out),self,sizeof(*self))&&(!q||!overlap(out,sizeof(*out),q,sizeof(*q)));}
bool coherent(const Record& q){return q.ref.action.identity&&q.ref.action.identity<=UINTPTR_MAX-48&&q.ref.fields==&q.fields&&q.ref.action.character_10==&q.fields.character_10;}
constexpr std::array<std::uint32_t,13> allocation_bytes{{48,48,40,40,36,44,24,40,40,40,48,48,36}};
Dispatch derived(std::int32_t type){return Dispatch(std::uint32_t(type)+std::uint32_t(Dispatch::kill_enemies));}
struct Call {
 const Services& services;Result result{};
 bool fail(Status s,Operation op){result.status=s;result.last_operation=op;return false;}
 template<class F>bool send(Operation op,F f){result.last_operation=op;++result.service_calls;try{if(f())return fail(Status::service_failed,op);}catch(...){return fail(Status::service_failed,op);}return true;}
 bool base(Record& q){
  q.dispatch_0=Dispatch::base;++result.field_stores;result.last_operation=Operation::base_construct;
  if(!services.get_constant)return fail(Status::service_unavailable,Operation::get_constant);
  std::int32_t value=0;
  if(!send(Operation::get_constant,[&]{return services.get_constant(services.context,q,"v2QuestObjectiveType","Invalid",&value);}))return false;
  if(!coherent(q))return fail(Status::projection_changed,Operation::get_constant);
  q.fields.type_4=value;q.done_14=0;q.compiled_8=0;q.fields.py_data_c={};q.fields.character_10=0;result.field_stores+=5;
  return true;
 }
 bool create(std::int32_t type,const Runtime* self,Result* out){
  if(!services.allocate)return fail(Status::service_unavailable,Operation::allocate);
  result.logical_bytes=allocation_bytes[std::uint32_t(type)];Record* q=nullptr;
  const bool allocated=send(Operation::allocate,[&]{return services.allocate(services.context,result.logical_bytes,0,&q);});
  result.record=q;
  if(!output(self,out,q))return fail(Status::invalid_argument,Operation::allocate);
  if(!allocated)return false;
  if(!aligned(q))return fail(Status::source_fault,Operation::allocate);
  if(!coherent(*q))return fail(Status::projection_changed,Operation::allocate);
  if(type==6){
   q->done_14=0;q->dispatch_0=Dispatch::null_vtable;q->fields.type_4=0;q->compiled_8=0;q->fields.py_data_c={};q->fields.character_10=0;result.field_stores+=6;
   result.last_operation=Operation::default_fields;
  }
  if(!base(*q))return false;
  if(type==6){q->dispatch_0=derived(type);++result.field_stores;}
  else if(type==0||type==1||type==10||type==11){
   q->derived_2c=0;q->quantity_20=0;q->dispatch_0=derived(type);q->secondary_18=derived(type);q->derived_24=quest_objective_list_v1::Definition{};q->derived_28=0;result.field_stores+=6;
  }else if(type==5){
   q->derived_24=-1;q->dispatch_0=derived(type);q->secondary_18=derived(type);q->derived_28=0;q->quantity_20=0;result.field_stores+=5;
  }else {
   q->quantity_20=0;q->dispatch_0=derived(type);q->secondary_18=derived(type);result.field_stores+=3;
   if(type!=4&&type!=12){q->derived_24=-1;++result.field_stores;}
  }
  result.last_operation=Operation::construct;return true;
 }
 bool destroy(Record& q,bool deleting){
  result.record=&q;result.last_operation=Operation::destruct;
  if(q.dispatch_0!=Dispatch::base&&(q.dispatch_0<Dispatch::kill_enemies||q.dispatch_0>Dispatch::gather_loot))return fail(Status::source_fault,Operation::destruct);
  if(q.dispatch_0==Dispatch::automatic){q.dispatch_0=Dispatch::automatic;++result.field_stores;}
  else if(q.dispatch_0!=Dispatch::base){q.dispatch_0=Dispatch::event_receiver;q.secondary_18=Dispatch::event_receiver;result.field_stores+=2;}
  // The actual ObjectiveD1/D2 callee is bx lr. No predicates/events are reached.
  if(!deleting)return true;
  if(!services.deallocate)return fail(Status::service_unavailable,Operation::deallocate);
  return send(Operation::deallocate,[&]{return services.deallocate(services.context,&q);});
 }
};
struct Guard{bool& busy;~Guard(){busy=false;}};
}
Status Runtime::create(std::int32_t type,Result* out){
 if(type<0||type>12||!output(this,out))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{services_};
 if(call.create(type,this,out))call.result.last_operation=Operation::complete;
 if(!output(this,out,call.result.record))return Status::invalid_argument;
 *out=call.result;return out->status;
}
Status Runtime::construct_base(Record& q,Result* out){
 if(!output(this,out,&q))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{services_};call.result.record=&q;
 if(!coherent(q))call.fail(Status::projection_changed,Operation::base_construct);
 else if(call.base(q))call.result.last_operation=Operation::complete;
 *out=call.result;return out->status;
}
Status Runtime::destroy(Record& q,bool deleting,Result* out){
 if(!output(this,out,&q))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};Call call{services_};
 if(!coherent(q))call.fail(Status::projection_changed,Operation::destruct);
 else if(call.destroy(q,deleting))call.result.last_operation=Operation::complete;
 *out=call.result;return out->status;
}
Status Runtime::save_data(const Record& q,
 const player_save_section_writers_v1::WriteServicesV1& stream,
 Result* out,std::string& error){
 if(!output(this,out,&q)||overlap(out,sizeof(*out),&stream,sizeof(stream))||
    overlap(&error,sizeof(error),this,sizeof(*this))||
    overlap(&error,sizeof(error),&q,sizeof(q))||
    overlap(&error,sizeof(error),out,sizeof(*out))||
    overlap(&error,sizeof(error),&stream,sizeof(stream)))return Status::invalid_argument;
 if(busy_)return Status::reentrant;
 busy_=true;Guard guard{busy_};
 Result result{};result.record=const_cast<Record*>(&q);result.last_operation=Operation::save_data;
 error.clear();
 if(!coherent(q))result.status=Status::projection_changed;
 else if(!stream.write)result.status=Status::service_unavailable;
 else {
  bool saved_quantity=false;
  switch(q.dispatch_0){
   case Dispatch::kill_enemies:case Dispatch::clear_enemies:
   case Dispatch::trigger_plate:case Dispatch::destroy_game_object:
   case Dispatch::talk_to_npc:case Dispatch::open_game_object:
   case Dispatch::trigger_on:case Dispatch::picked_up_liftable:
   case Dispatch::kill_enemy_template:case Dispatch::clear_enemy_template:
    saved_quantity=true;break;
   case Dispatch::move_in_zone:case Dispatch::automatic:case Dispatch::gather_loot:
    saved_quantity=false;break;
   default:result.status=Status::source_fault;break;
  }
  if(result.status==Status::complete){
   // _saveData loads this stored bool with LDRB and passes that byte through
   // writeAs<bool>; the ARM body does not canonicalize nonzero values.
   const std::uint8_t done=q.done_14;
   bool failed=false;
   try{
    ++result.service_calls;
    if(!stream.write(stream.context,{&done,1},error)){
     if(error.empty())error="source Objective completion write failed";
     failed=true;
    }
    if(saved_quantity){
     // SavedQty ignores the base writer's return and always performs this
     // second virtual write. Read quantity only after the first callback.
     const auto value=q.quantity_20;
     const std::uint8_t bytes[]{std::uint8_t(value),std::uint8_t(value>>8),
       std::uint8_t(value>>16),std::uint8_t(value>>24)};
     ++result.service_calls;
     if(!stream.write(stream.context,{bytes,sizeof(bytes)},error)){
      if(error.empty())error="source Objective saved quantity write failed";
      failed=true;
     }
    }
    if(failed)result.status=Status::service_failed;
   }catch(...){
    if(error.empty())error="source Objective stream writer threw";
    result.status=Status::service_failed;
   }
  }
 }
 if(result.status==Status::complete)result.last_operation=Operation::complete;
 *out=result;return result.status;
}
}
