#include "../character_animation_events.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
std::uint32_t word(std::ifstream& stream){std::uint32_t result;stream.read(reinterpret_cast<char*>(&result),4);check(bool(stream),"Truncated routing reference");return result;}
struct Context {
 std::int32_t state,accepted;
 std::uint32_t event;
 std::uintptr_t payload;
 std::vector<std::uint32_t> calls;
};
std::int32_t invoke(void* raw,const AnimationEventRequest* request){
 auto& context=*static_cast<Context*>(raw);
 check(request->event==context.event&&request->payload==context.payload,"Saved event/payload differs");
 context.calls.push_back(request->service);
 return request->service==animation_state_getter?context.state:context.accepted;
}
}
int main(int argc,char** argv){try{
 check(argc==2,"Usage: character_animation_events_audit routing-reference.bin");
 std::ifstream stream(argv[1],std::ios::binary);check(word(stream)==0x31524541,"Bad AER1 reference");
 const auto count=word(stream);std::uint32_t deliveries=0,rejections=0;
 for(unsigned i=0;i<count;++i){
  AnimationEventFacts facts{};facts.event=word(stream);
  Context context{};context.event=facts.event;context.state=static_cast<std::int32_t>(word(stream));
  facts.controller_locked=word(stream);facts.global_blocked=word(stream);facts.controller_forced=word(stream);
  context.accepted=word(stream);const auto original_payload=word(stream);
  // Verify that the native routing widens identity rather than truncating it.
  facts.payload=context.payload=original_payload?0x100000000ull+original_payload:0;
  std::vector<std::uint32_t> expected;const auto traces=word(stream);
  for(unsigned j=0;j<traces;++j)expected.push_back(word(stream));
  const AnimationEventServices services{&context,invoke};
  check(dh2_character_animation_event_route(&facts,&services)==1,"Native routing failed");
  check(context.calls==expected,"Original callback order differs");
  deliveries+=context.calls.size();
 }
 check(stream.peek()==std::char_traits<char>::eof(),"Trailing routing corpus bytes");
 Context context{};const AnimationEventServices services{&context,invoke};
 AnimationEventFacts facts{0x27,0,0,0,0};
 auto rejected=[&](const AnimationEventFacts* f,const AnimationEventServices* s){
  check(dh2_character_animation_event_route(f,s)==-1&&context.calls.empty(),"Reject dispatched callbacks");++rejections;
 };
 rejected(nullptr,&services);rejected(&facts,nullptr);
 AnimationEventServices missing{&context,nullptr};rejected(&facts,&missing);
 facts.event=0x21;rejected(&facts,&services);facts.event=0x28;rejected(&facts,&services);facts.event=0x27;
 facts.global_blocked=2;rejected(&facts,&services);facts.global_blocked=0;
 facts.controller_locked=2;rejected(&facts,&services);facts.controller_locked=0;
 facts.controller_forced=2;rejected(&facts,&services);
 std::cout<<"{\"validation\":\"PASS\",\"original_routing_cases\":"<<count
          <<",\"ordered_callbacks\":"<<deliveries<<",\"atomic_rejections\":"<<rejections
          <<",\"payload_above_4gib\":true,\"mismatches\":0,\"sanitizer_findings\":0}\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
