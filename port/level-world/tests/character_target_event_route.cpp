#include "../character_ai_events.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
using Entry=std::array<std::uint32_t,4>;
struct Context {
 std::vector<Entry> trace;
 AIEventOwner48* alternate;
 AIEventPayload24* payload;
 const AIEventServices24* services;
 std::uint32_t returned,event,old_payload;
 bool replace=false,change_payload=false,reenter=false,entered=false;
 int fail=-1;
};
static int invoke(void* opaque,AIEventState64* state,const AIEventRequest40* req,std::uint32_t* result){
 auto& c=*static_cast<Context*>(opaque);assert(req->event==c.event&&req->argument==0);
 const auto captured=c.old_payload?UINT64_C(0x100000000)+c.old_payload:0;
 if(req->service==ai_event_virtual){
  const unsigned slot=c.event==9?0x34:0x40+4*(c.event-10);
  assert(req->operation==slot&&req->callee==UINT64_C(0x300000000)+slot&&req->subject==UINT64_C(0x20000000b)&&req->payload==(c.event==9?captured:0));
  c.trace.push_back({0,c.event,11,c.old_payload});
  if(c.replace)state->owner=c.alternate;
  if(c.change_payload)c.payload->value=0;
  if(c.reenter&&!c.entered){c.entered=true;AIEventResult16 nested{};assert(dh2_character_ai_event(&nested,state,c.event,c.payload,c.services)==0&&nested.phase==7);}
 }else{
  assert(req->service==ai_event_state_event&&req->operation==0&&req->callee==0&&req->payload==captured);
  const auto owner=static_cast<std::uint32_t>(req->subject-UINT64_C(0x100000100));assert(owner==1||owner==2);c.trace.push_back({1,c.event,owner,c.old_payload});
 }
 *result=c.returned;return static_cast<int>(req->service)==c.fail?1:0;
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);char magic[4];file.read(magic,4);assert(std::memcmp(magic,"CTR1",4)==0);
 auto word=[&](){std::uint32_t x=0;file.read(reinterpret_cast<char*>(&x),4);assert(file);return x;};const auto cases=word();
 AIEventOwner48 owners[2]{{UINT64_C(0x100000001),UINT64_C(0x100000201),UINT64_C(0x100000101),UINT64_C(0x100000301),0,0,0,0},{UINT64_C(0x100000002),UINT64_C(0x100000202),UINT64_C(0x100000102),UINT64_C(0x100000302),0,0,0,0}};
 std::array<std::uintptr_t,51> table{};for(unsigned i=0;i<table.size();++i)table[i]=UINT64_C(0x300000000)+4*i;
 AIEventState64 state{};AIEventPayload24 payload{};AIEventServices24 services{};AIEventResult16 output{};Context context{{},&owners[1],&payload,&services,0,9,0};services={&context,invoke,3,0};
 auto reset=[&](){state={UINT64_C(0x20000000b),&owners[0],table.data(),0,nullptr,0,0,0,0,0,0};context.trace.clear();context.replace=context.change_payload=context.reenter=context.entered=false;context.fail=-1;};
 std::uint32_t calls=0,checks=0;
 for(std::uint32_t i=0;i<cases;++i){
  const auto event=word(),blocked=word(),locked=word(),forced=word(),returned=word(),old_payload=word(),replace=word(),count=word();assert(count<=2);std::vector<Entry> expected(count);for(auto& entry:expected)for(auto& x:entry){x=word();}
  reset();state.global_blocked=blocked;for(auto& owner:owners){owner.locked=locked;owner.forced=forced;}payload={old_payload?UINT64_C(0x100000000)+old_payload:0,0,0,0};context.returned=returned;context.event=event;context.old_payload=old_payload;context.replace=replace;
  assert(dh2_character_ai_event(&output,&state,event,&payload,&services)==0&&output.phase==7&&context.trace==expected);calls+=count;checks+=4*count+1;
 }
 assert(file.peek()==EOF);reset();for(auto& owner:owners){owner.locked=owner.forced=0;}context.event=9;context.old_payload=0xf1234567;payload={UINT64_C(0x1f1234567),0,0,0};output={91,92,93,94};const auto old_output=output;std::uint32_t guards=0;
 auto atomic=[&](AIEventResult16* out,AIEventState64* s,const AIEventPayload24* p,const AIEventServices24* svc){context.trace.clear();assert(dh2_character_ai_event(out,s,9,p,svc)==1&&context.trace.empty()&&std::memcmp(&output,&old_output,sizeof output)==0);++guards;};
 atomic(nullptr,&state,&payload,&services);atomic(&output,nullptr,&payload,&services);atomic(&output,&state,nullptr,&services);atomic(&output,&state,&payload,nullptr);AIEventServices24 empty=services;empty.invoke=nullptr;atomic(&output,&state,&payload,&empty);
 payload.reserved1=1;atomic(&output,&state,&payload,&services);payload.reserved1=0;state.global_blocked=256;atomic(&output,&state,&payload,&services);state.global_blocked=0;
 owners[0].forced=256;atomic(&output,&state,&payload,&services);owners[0].forced=0;state.owner=nullptr;atomic(&output,&state,&payload,&services);state.owner=&owners[0];
 reset();context.fail=ai_event_virtual;assert(dh2_character_ai_event(&output,&state,9,&payload,&services)==3&&context.trace.size()==1);
 reset();context.replace=true;context.fail=ai_event_state_event;assert(dh2_character_ai_event(&output,&state,9,&payload,&services)==3&&context.trace.size()==2&&state.owner==&owners[1]);
 reset();context.replace=true;context.change_payload=true;assert(dh2_character_ai_event(&output,&state,9,&payload,&services)==0);assert(context.trace.size()==2&&context.trace[1]==(Entry{1,9,2,0xf1234567}));
 reset();payload.value=UINT64_C(0x1f1234567);context.reenter=true;assert(dh2_character_ai_event(&output,&state,9,&payload,&services)==0);assert(context.trace.size()==4&&context.trace[0][0]==0&&context.trace[1][0]==0&&context.trace[2][0]==1&&context.trace[3][0]==1);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_services\":"<<calls<<",\"word_checks\":"<<checks<<",\"native_guards\":"<<guards<<",\"provider_failure_checks\":2,\"native_only_reentry_checks\":1,\"native_only_captured_argument_checks\":1,\"mismatches\":0}\n";
}
