#include "../object_identity.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace dh2::object_identity;
struct Call {std::uintptr_t receiver,argument;std::uint32_t count,type;std::string name;};
struct Context {std::vector<Call> calls;TargetScript16* state;const TargetServices16* services;bool reenter=false,entered=false;int fail=0;};
static int invoke(void* opaque,const TargetCall32* call){
 auto& c=*static_cast<Context*>(opaque);c.calls.push_back({call->receiver,call->argument,call->argument_count,call->value_type,call->callback});
 if(c.reenter&&!c.entered){c.entered=true;c.state->available_callbacks=0;assert(dh2_object_target_event(c.state,target_died,0,c.services)==0);}
 return c.fail;
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);char magic[4];file.read(magic,4);assert(std::memcmp(magic,"OIE1",4)==0);
 auto word=[&](){std::uint32_t x=0;file.read(reinterpret_cast<char*>(&x),4);assert(file);return x;};const auto cases=word();std::uint32_t checks=0,callbacks=0;
 TargetScript16 state{UINT64_C(0x2025000),0,0};Context context{{},&state,nullptr,false,false,0};TargetServices16 services{&context,invoke};context.services=&services;
 for(std::uint32_t i=0;i<cases;++i){
  const auto event=word(),mask=word(),enemy=word(),has_call=word(),receiver=word(),argument=word(),count=word(),type=word(),len=word();assert(len<=128);std::string name(len,'\0');file.read(name.data(),len);assert(file);context.calls.clear();state.available_callbacks=mask;
  state.identity=receiver;
  assert(dh2_object_target_event(&state,event,enemy,&services)==0);assert(context.calls.size()==has_call);
  if(has_call){const auto& call=context.calls[0];assert(call.receiver==receiver&&call.argument==argument&&call.count==count&&call.type==type&&call.name==name);++callbacks;checks+=5;}
  ++checks;
 }
 assert(file.peek()==EOF);std::uint32_t guards=0;
 context.calls.clear();state.available_callbacks=0;assert(dh2_object_target_event(&state,target_in_melee_range,0,nullptr)==0&&context.calls.empty());++guards;
 assert(dh2_object_target_event(&state,target_died,0,nullptr)==1&&context.calls.empty());++guards;
 assert(dh2_object_target_event(&state,0,0,&services)==1&&context.calls.empty());++guards;
 assert(dh2_object_target_event(nullptr,target_died,0,&services)==1);++guards;
 state.reserved=1;assert(dh2_object_target_event(&state,target_died,0,&services)==1);state.reserved=0;++guards;
 context.fail=1;assert(dh2_object_target_event(&state,enemy_spotted,0,&services)==2&&context.calls.size()==1&&context.calls[0].type==7&&context.calls[0].argument==0);context.fail=0;++guards;
 // The caller's scoped method/VM bridge is a separate provider. No work queue
 // or post-callback availability reload is introduced by this coordinator.
 context.calls.clear();context.reenter=true;state.available_callbacks=32;assert(dh2_object_target_event(&state,target_in_melee_range,0,&services)==0);assert(context.calls.size()==2&&context.calls[0].name=="OnTargetInMeleeRange"&&context.calls[1].name=="OnTargetDied"&&state.available_callbacks==0);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"callback_checks\":"<<checks<<",\"callbacks\":"<<callbacks<<",\"native_guards\":"<<guards<<",\"native_reentry_checks\":1,\"mismatches\":0}\n";
}
