#include "../character_target_events.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
using Entry=std::array<std::uint32_t,16>;
static std::uint32_t bits(float x){std::uint32_t result;std::memcpy(&result,&x,4);return result;}
struct Context {
 std::array<std::uint32_t,13> row{};
 std::vector<Entry> trace;
 TargetOwner16* alternate=nullptr;
 TargetEventServices40* services=nullptr;
 const std::int32_t* alternate_sounds=nullptr;
 bool triggered=false;
 int fail=-1;
};
static int invoke(void* opaque,TargetEventState32* state,const TargetEventRequest64* req,TargetEventResponse24* response){
 auto& c=*static_cast<Context*>(opaque);const auto op=req->service;assert(op<11);
 if(op>=1&&op<=3)assert(req->text&&std::strcmp(req->text,"isTracingCharAIEvents")==0);
 c.trace.push_back({op,req->event,static_cast<std::uint32_t>(req->subject),static_cast<std::uint32_t>(req->other),static_cast<std::uint32_t>(req->sound),req->flag,bits(req->position[0]),bits(req->position[1]),bits(req->position[2]),bits(req->parameters[0]),bits(req->parameters[1]),static_cast<std::uint32_t>(req->integer),static_cast<std::uint32_t>(state->target->owner->identity),static_cast<std::uint32_t>(state->active),state->continued,state->target->changed});
 if(c.row[8]==op&&!c.triggered){
  c.triggered=true;
  switch(c.row[9]){
   case 1:state->active=0;break;
   case 2:state->active=32;break;
   case 3:state->target->owner=c.alternate;break;
   case 4:state->continued=255;state->target->changed=254;break;
   case 5:{Entry entry{};entry[0]=11;c.trace.push_back(entry);assert(dh2_character_target_event(state,c.row[0],c.services)==0);entry[0]=12;c.trace.push_back(entry);break;}
   case 6:c.services->ai_sounds=c.alternate_sounds;break;
   case 7:c.services->sound_manager=42;break;
   default:assert(false);
  }
 }
 response->word=op==2?c.row[7]:op==4?c.row[4]:op==7?c.row[6]:0;
 if(op==5)response->identity=c.row[5];
 if(op==8)std::memcpy(response->position,&c.row[10],12);
 return static_cast<int>(op)==c.fail?1:0;
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);char magic[4];file.read(magic,4);assert(std::memcmp(magic,"CTE1",4)==0);
 auto word=[&](){std::uint32_t result=0;file.read(reinterpret_cast<char*>(&result),4);assert(file);return result;};
 const auto cases=word(),ai_count=word();assert(ai_count==76);std::vector<std::int32_t> sounds(ai_count);for(auto& x:sounds){auto value=word();std::memcpy(&x,&value,4);}auto reversed=sounds;std::reverse(reversed.begin(),reversed.end());
 TargetOwner16 owners[2]{{1,0,0,0},{2,0,0,0}};TargetState48 target{};TargetEventState32 state{};Context context;TargetEventServices40 services{&context,invoke,sounds.data(),ai_count,0,41};context.alternate=&owners[1];context.services=&services;context.alternate_sounds=reversed.data();
 auto reset=[&](){target={11,&owners[0],0,0,0,1,1,static_cast<std::uint8_t>(context.row[3]),0,0};state={&target,context.row[1],static_cast<std::uint8_t>(context.row[2]),{},0};services.ai_sounds=sounds.data();services.ai_count=ai_count;services.sound_manager=41;context.trace.clear();context.triggered=false;context.fail=-1;};
 std::uint32_t calls=0,checks=0;
 for(std::uint32_t i=0;i<cases;++i){
  for(auto& x:context.row){x=word();}std::array<std::uint32_t,6> expected{};for(auto& x:expected){x=word();}const auto count=word();assert(count<1000);std::vector<Entry> trace(count);for(auto& entry:trace)for(auto& x:entry){x=word();}reset();
  assert(dh2_character_target_event(&state,context.row[0],&services)==0);
  const std::array<std::uint32_t,6> actual{static_cast<std::uint32_t>(target.owner->identity),static_cast<std::uint32_t>(state.active),state.continued,target.changed,services.ai_sounds==sounds.data()?0u:1u,static_cast<std::uint32_t>(services.sound_manager)};
  assert(actual==expected&&context.trace==trace);calls+=count;checks+=6+16*count;
 }
 assert(file.peek()==EOF);std::uint32_t guards=0;
 context.row={13,31,1,77,1,301,44,0,UINT32_MAX,0,0x3f800000,0x80000000,0x7fc12345};reset();const auto original=state;const auto old_target=target;
 auto atomic=[&](TargetEventState32* s,std::uint32_t event,const TargetEventServices40* svc){context.trace.clear();assert(dh2_character_target_event(s,event,svc)==1&&context.trace.empty()&&std::memcmp(&state,&original,sizeof state)==0&&std::memcmp(&target,&old_target,sizeof target)==0);++guards;};
 atomic(nullptr,13,&services);atomic(&state,13,nullptr);atomic(&state,9,&services);atomic(&state,18,&services);TargetEventServices40 empty=services;empty.invoke=nullptr;atomic(&state,13,&empty);
 alignas(TargetEventState32) std::array<unsigned char,64> unaligned{};atomic(reinterpret_cast<TargetEventState32*>(unaligned.data()+1),13,&services);atomic(&state,13,reinterpret_cast<TargetEventServices40*>(unaligned.data()+1));
 state.target=reinterpret_cast<TargetState48*>(unaligned.data()+1);assert(dh2_character_target_event(&state,13,&services)==1&&context.trace.empty());state=original;++guards;
 target.owner=reinterpret_cast<TargetOwner16*>(unaligned.data()+1);assert(dh2_character_target_event(&state,13,&services)==1&&context.trace.empty());target=old_target;++guards;
 state.reserved_bytes[6]=1;assert(dh2_character_target_event(&state,13,&services)==1&&context.trace.empty());state=original;++guards;
 std::uint32_t failures=0;
 for(unsigned op=0;op<11;++op){context.row[0]=op>=4&&op<=6?12:13;reset();context.fail=op;assert(dh2_character_target_event(&state,context.row[0],&services)==2&&!context.trace.empty()&&context.trace.back()[0]==op);++failures;}
 context.row[0]=13;reset();services.ai_sounds=nullptr;assert(dh2_character_target_event(&state,13,&services)==2&&context.trace.back()[0]==7);++failures;
 reset();context.row[6]=UINT32_MAX;assert(dh2_character_target_event(&state,13,&services)==2&&context.trace.back()[0]==7);++failures;
 // Captured active delivery failure retains the Died prefix byte write.
 context.row[0]=10;reset();context.fail=10;assert(dh2_character_target_event(&state,10,&services)==2&&state.continued==0);++failures;
 // Sound storage is only required by InSight; other handlers still perform
 // their full debug prefix and state effects with a null sound table.
 reset();services.ai_sounds=nullptr;assert(dh2_character_target_event(&state,10,&services)==0&&state.continued==0);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_services\":"<<calls<<",\"word_checks\":"<<checks<<",\"genuine_ai_rows\":"<<ai_count<<",\"native_guards\":"<<guards<<",\"provider_failure_checks\":"<<failures<<",\"mismatches\":0}\n";
}
