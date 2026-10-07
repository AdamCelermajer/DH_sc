#include "application_services_owner_v5.hpp"
namespace dh2::application {
namespace {
// One real native heap allocation. The identity names this actual receiver,
// unlike Level's separate EventManager base identity at Level+0.
struct HeapEventManagerV5 {
 events::EventManagerOwnerV12 receiver;
 HeapEventManagerV5():receiver(reinterpret_cast<std::uintptr_t>(&receiver)){}
};
}
bool ApplicationServicesOwnerV5::post_init_events_v5(std::string& error){
 if(event_publication_attempted_){error="Application EventManager PostInit publication already attempted";return false;}
 event_publication_attempted_=true;
 auto allocation=std::make_shared<HeapEventManagerV5>();
 // Source32f818 allocate ->32f820 actual EventManager C1 ->32f828 publish14.
 events14_=std::shared_ptr<events::EventManagerOwnerV12>(allocation,&allocation->receiver);
 error.clear();return true;
}
}
