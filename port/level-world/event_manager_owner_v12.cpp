#include "event_manager_owner_v12.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::events {
EventManagerOwnerV12::EventManagerOwnerV12(std::uintptr_t identity):identity_(identity){
 if(!identity)throw std::invalid_argument("Required actual EventManager base identity");
 // C1/C2: receiver map empty, pending20 and delayed28 self-linked empty.
}
EventManagerOwnerV12::~EventManagerOwnerV12(){
 // D1/D2 clear pending nodes (NOT event deleting dtors), delayed nodes, then
 // pending nodes again and receiver map. Leases retain real native owners.
 queue20_.clear();delayed28_.clear();queue20_.clear();receivers_.clear();
}
bool EventManagerOwnerV12::ready(std::string& e)const{
 if(failed_){e="EventManager cannot replay a failed delivery prefix";
  if(!failed_reason_.empty())e+="; first failure: "+failed_reason_;return false;}
 return true;
}
void EventManagerOwnerV12::latch_failure(const std::string& error,const char* fallback){
 if(failed_)return;
 failed_=true;failed_reason_=error.empty()?fallback:error;
}
bool EventManagerOwnerV12::attach(std::int32_t type,const EventReceiverV12& receiver,std::int32_t priority,bool& out,std::string& e){
 out=false;if(!ready(e))return false;
 if(!receiver.identity||!receiver.on_event){e="Required actual IEventReceiver virtual8";return false;}
 auto it=receivers_.find(type);
 if(it!=receivers_.end())for(const auto& r:it->second)if(r.receiver.identity==receiver.identity)return true;
 auto& list=receivers_[type];list.push_back({receiver,priority,0,next_token_++});out=true;return true;
}
bool EventManagerOwnerV12::detach(std::int32_t type,std::uintptr_t receiver,bool& out,std::string& e){
 out=false;if(!ready(e))return false;auto m=receivers_.find(type);if(m==receivers_.end())return true;
 for(auto it=m->second.begin();it!=m->second.end();++it)if(it->receiver.identity==receiver){m->second.erase(it);out=true;break;}return true;
}
bool EventManagerOwnerV12::delayed_detach(std::int32_t type,std::uintptr_t receiver,bool& out,std::string& e){
 out=false;if(!ready(e))return false;auto m=receivers_.find(type);if(m==receivers_.end())return true;
 for(const auto& r:m->second)if(r.receiver.identity==receiver){
  // Modern safety correction: duplicate native delayed raw-node references
  // would double-free. Treat the already scheduled SAME node as scheduled.
  if(std::none_of(delayed28_.begin(),delayed28_.end(),[&](const auto& d){return d.token==r.token;}))delayed28_.push_back({type,r.token});
  out=true;return true;
 }return true;
}
bool EventManagerOwnerV12::retire_receiver_alias_v88(std::uintptr_t receiver,std::string& e){
 if(!receiver||dispatch_depth_||updating_){e="Actual event observer retirement requires quiescent delivery";return false;}
 for(auto m=receivers_.begin();m!=receivers_.end();){
  for(auto it=m->second.begin();it!=m->second.end();){
   if(it->receiver.identity!=receiver){++it;continue;}
   const auto token=it->token;
   delayed28_.remove_if([token](const auto& d){return d.token==token;});
   it=m->second.erase(it);
  }
  if(m->second.empty())m=receivers_.erase(m);else ++m;
 }
 e.clear();return true;
}
bool EventManagerOwnerV12::drop_delayed_detach(std::string& e){
 if(!ready(e))return false;
 for(const auto& d:delayed28_){auto m=receivers_.find(d.type);if(m==receivers_.end())continue;
  for(auto it=m->second.begin();it!=m->second.end();++it)if(it->token==d.token){m->second.erase(it);break;}
 }delayed28_.clear();return true;
}
bool EventManagerOwnerV12::raise(const EventBorrowV12& event,std::string& e){
 if(!ready(e))return false;if(!event.identity||!event.get_type){e="Required actual IEvent virtual8 GetType";return false;}
 std::int32_t type{};if(!event.get_type(event.context,type,e)){latch_failure(e,"Required actual IEvent.GetType failed");return false;}
 auto m=receivers_.find(type);if(m==receivers_.end())return true;
 // Whole source snapshot: receiver, priority and byte10; never priority sort.
 const auto snapshot=m->second;++dispatch_depth_;struct Scope{unsigned& d;~Scope(){--d;}}scope{dispatch_depth_};
 for(const auto& r:snapshot){std::int32_t result{};
  if(!r.receiver.on_event(r.receiver.context,event,*this,result,e)){latch_failure(e,"Actual EventManager handler rejected required delivery");return false;}
  if(result==1)break;
 }return true;
}
bool EventManagerOwnerV12::import_pending_from_required_producer_v12(const EventQueueProducerV12& producer,std::string& e){
 if(!ready(e))return false;if(!producer.produce){e="Required genuine EventManager pending-list producer; RaiseAsync is synchronous";return false;}
 PendingEventV12 pending;if(!producer.produce(producer.context,pending,e))return false;
 if(!pending.event.identity||!pending.event.get_type||!pending.deleting_destructor){e="Required actual queued event type and deleting destructor";return false;}
 queue20_.push_back(std::move(pending));return true;
}
bool EventManagerOwnerV12::update(double source_dt,std::string& e){
 (void)source_dt; // Whole original33900c never reads double argument.
 if(!ready(e))return false;if(updating_){e="Unsupported destructive EventManager Update reentry";return false;}
 updating_=true;struct Scope{bool& b;~Scope(){b=false;}}scope{updating_};
 while(!queue20_.empty()){
  auto& pending=queue20_.front();if(!raise(pending.event,e))return false;
  if(!pending.deleting_destructor(pending.destroy_context,pending.event.identity,e)){latch_failure(e,"Required queued IEvent deleting destructor");return false;}
  queue20_.pop_front();
 }return drop_delayed_detach(e);
}
bool EventManagerOwnerV12::flush(std::string& e){
 if(updating_){e="Unsupported destructive EventManager Flush during queued Update";return false;}
 // Source Flush3384ac clears pending nodes, receiver map, delayed nodes.
 queue20_.clear();receivers_.clear();delayed28_.clear();failed_=false;failed_reason_.clear();return true;
}
std::size_t EventManagerOwnerV12::receiver_count(std::int32_t type)const noexcept{auto m=receivers_.find(type);return m==receivers_.end()?0:m->second.size();}
std::vector<ReceiverObservationV12> EventManagerOwnerV12::receiver_records_v12()const{std::vector<ReceiverObservationV12> out;for(const auto& m:receivers_)for(const auto& r:m.second)out.push_back({m.first,r.receiver.identity,r.priority,r.byte10});return out;}
}
