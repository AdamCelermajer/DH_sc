#include "event_manager_owner_v12.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
using namespace dh2::events;
unsigned checks{};void check(bool value){++checks;if(!value)throw std::runtime_error("check "+std::to_string(checks));}
std::uint32_t read(std::ifstream& f){std::uint32_t v{};f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
struct Payload{std::int32_t type{};unsigned touches{};};
struct Handler{unsigned id{};std::vector<unsigned>* calls{};std::function<void(EventManagerOwnerV12&)> mutation;bool reject{};};
bool get_type(void* p,std::int32_t& type,std::string&){type=static_cast<Payload*>(p)->type;return true;}
bool on_event(void* p,const EventBorrowV12& e,EventManagerOwnerV12& manager,std::int32_t& result,std::string& error){auto& h=*static_cast<Handler*>(p);h.calls->push_back(h.id);++static_cast<Payload*>(e.context)->touches;if(h.mutation)h.mutation(manager);if(h.reject){error="Required actual handler unavailable fixture";return false;}result=static_cast<std::int32_t>(h.id%4);return true;}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream f(argv[1],std::ios::binary);char tag[4]{};f.read(tag,4);check(std::string(tag,4)=="EM12");auto count=read(f);
 EventManagerOwnerV12 manager(123);Payload payload;std::vector<unsigned> calls;Handler handlers[8];EventReceiverV12 receivers[8];
 for(unsigned i=0;i<8;++i){handlers[i].id=i;handlers[i].calls=&calls;receivers[i]={i+1,&handlers[i],on_event,{}};}
 EventBorrowV12 event{456,&payload,get_type,{}};std::string error;
 for(unsigned index=0;index<count;++index){auto op=read(f);auto type=static_cast<std::int32_t>(read(f));auto id=read(f);auto priority=static_cast<std::int32_t>(read(f));auto result=read(f),delayed=read(f),touches=read(f),rows=read(f),dispatched=read(f);payload={type,0};calls.clear();bool actual{};
  if(op==0)check(manager.attach(type,receivers[id],priority,actual,error));
  else if(op==1)check(manager.detach(type,id+1,actual,error));
  else if(op==2)check(manager.delayed_detach(type,id+1,actual,error));
  else if(op==3)check(manager.drop_delayed_detach(error));
  else if(op==4)check(manager.raise(event,error));
  else if(op==5)check(manager.raise_async(event,error));
  else if(op==6)check(manager.flush(error));
  else check(manager.update(1.25,error));
  check(unsigned(actual)==result&&manager.delayed_count()==delayed&&payload.touches==touches);
  const auto observed=manager.receiver_records_v12();check(observed.size()==rows);
  for(unsigned j=0;j<rows;++j){auto t=static_cast<std::int32_t>(read(f));auto identity=read(f)+1;auto p=static_cast<std::int32_t>(read(f));auto byte=read(f);check(observed[j].type==t&&observed[j].identity==identity&&observed[j].priority==p&&observed[j].byte10==byte);}
  check(calls.size()==dispatched);for(unsigned j=0;j<dispatched;++j)check(calls[j]==read(f));
 }
 // Snapshot membership is source-owned even when a handler detaches or Flushes.
 check(manager.flush(error));bool out{};payload={7,0};calls.clear();
 check(manager.attach(7,receivers[0],100,out,error)&&out);check(manager.attach(7,receivers[2],-100,out,error)&&out);
 handlers[0].mutation=[&](auto& same){bool removed{};check(same.identity()==123&&same.detach(7,3,removed,error)&&removed);};
 check(manager.raise_async(event,error)&&calls==std::vector<unsigned>({0,2})&&manager.receiver_count(7)==1);
 check(manager.attach(7,receivers[2],-100,out,error)&&out);handlers[0].mutation=[&](auto& same){check(same.flush(error));};calls.clear();
 check(manager.raise(event,error)&&calls==std::vector<unsigned>({0,2})&&manager.receiver_count(7)==0);handlers[0].mutation={};
 // Modern safety repair for native double-free/dangling delayed-node cases.
 check(manager.attach(7,receivers[0],0,out,error));check(manager.delayed_detach(7,1,out,error)&&out);check(manager.delayed_detach(7,1,out,error)&&out&&manager.delayed_count()==1);
 check(manager.detach(7,1,out,error)&&out);check(manager.drop_delayed_detach(error)&&manager.delayed_count()==0);
 // Native Update owns Raise -> deleting virtual4 -> list-node removal. The
 // producer below is explicitly fixture storage, NOT RaiseAsync/quest cloning.
 unsigned deleted{};PendingEventV12 pending{event,&deleted,[](void* p,std::uintptr_t id,auto&){check(id==456);++*static_cast<unsigned*>(p);return true;}};
 EventQueueProducerV12 producer{&pending,[](void* p,auto& out,auto&){out=*static_cast<PendingEventV12*>(p);return true;}};
 check(!manager.import_pending_from_required_producer_v12({},error)&&manager.pending_count()==0);
 check(manager.attach(7,receivers[2],0,out,error));check(manager.import_pending_from_required_producer_v12(producer,error));check(manager.import_pending_from_required_producer_v12(producer,error));calls.clear();
 check(manager.update(99,error)&&deleted==2&&manager.pending_count()==0&&calls==std::vector<unsigned>({2,2}));
 check(manager.import_pending_from_required_producer_v12(producer,error));check(manager.flush(error)&&deleted==2); // source clear frees nodes, not payloads
 {EventManagerOwnerV12 disposable(999);check(disposable.import_pending_from_required_producer_v12(producer,error));}check(deleted==2);
 pending.deleting_destructor=[](void* p,std::uintptr_t,auto& e){++*static_cast<unsigned*>(p);e="Required deleting destructor rejected fixture";return false;};
 check(manager.import_pending_from_required_producer_v12(producer,error));check(!manager.update(1,error)&&manager.failed()&&manager.pending_count()==1&&deleted==3);check(!manager.update(1,error)&&deleted==3);check(manager.flush(error));
 check(manager.attach(7,receivers[2],0,out,error));handlers[2].reject=true;calls.clear();check(!manager.raise(event,error)&&manager.failed());const auto delivered=calls.size();check(!manager.raise_async(event,error)&&calls.size()==delivered);check(manager.flush(error));handlers[2].reject=false;
 std::cout<<"PASS EventManager whole source1610 sequences, receiver snapshot/order/result1, synchronous RaiseAsync, pending Raise-delete-pop, Flush nondelete, required failures and modern delayed-node safety; checks="<<checks<<'\n';
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
