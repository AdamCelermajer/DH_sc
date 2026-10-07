#include "../application_services_owner_v5.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;
struct Receiver {unsigned calls{};
 static bool event(void* p,const events::EventBorrowV12&,events::EventManagerOwnerV12& manager,std::int32_t& result,std::string&){auto& self=*static_cast<Receiver*>(p);assert(manager.identity()!=0);++self.calls;result=0;return true;}
};
int main(){
 unsigned checks=0;std::string error;
 auto app=std::make_shared<application::ApplicationServicesOwnerV5>();
 assert(app->identity()==reinterpret_cast<std::uintptr_t>(app.get())&&!app->events14()&&app->active_camera().source_active()==0);++checks;
 assert(app->post_init_events_v5(error));++checks;
 const auto same=app->events14();assert(same&&same->identity()==reinterpret_cast<std::uintptr_t>(same.get())&&same->identity()!=app->identity());++checks;
 assert(same->receiver_records_v12().empty()&&same->pending_count()==0&&same->delayed_count()==0);++checks;
 assert(!app->post_init_events_v5(error)&&app->events14()==same);++checks;
 auto other_borrow=app;assert(other_borrow->events14()==same&&&other_borrow->active_camera()==&app->active_camera());++checks;
 // Explicit two Level-base identities; they must not alias Application14.
 events::EventManagerOwnerV12 level_first(0x1234),level_second(0x2345);
 assert(level_first.identity()!=same->identity()&&level_second.identity()!=same->identity());++checks;
 auto receiver=std::make_shared<Receiver>();bool inserted{};
 events::EventReceiverV12 handler{reinterpret_cast<std::uintptr_t>(receiver.get()),receiver.get(),Receiver::event,receiver};
 assert(same->attach(4,handler,0,inserted,error)&&inserted);++checks;
 std::int32_t type=4;events::EventBorrowV12 event{reinterpret_cast<std::uintptr_t>(&type),&type,[](void* p,std::int32_t& out,std::string&){out=*static_cast<std::int32_t*>(p);return true;},{}};
 assert(same->raise_async(event,error)&&receiver->calls==1);++checks;
 assert(level_first.raise(event,error)&&level_second.raise(event,error)&&receiver->calls==1);++checks;
 std::weak_ptr<events::EventManagerOwnerV12> heap=same;other_borrow.reset();app.reset();assert(!heap.expired()&&same->receiver_count(4)==1);++checks;
 assert(same->flush(error)&&same->receiver_count(4)==0);++checks;
 std::cout<<"PASS Application C1-NULL/PostInit SAME heap events14, shared process camera authority, distinct Level bases, immediate delivery and lifetimes "<<checks<<'\n';
}
