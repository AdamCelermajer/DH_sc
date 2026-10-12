#include "../character_target_update.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
using Entry=std::array<std::uint32_t,4>;
struct Context {
 std::array<std::uint32_t,16> row{};
 std::array<unsigned,11> counts{};
 std::vector<Entry> trace;
 TargetOwner16* alternate=nullptr;
 const TargetUpdateServices16* services=nullptr;
 bool triggered=false;
 int fail=-1;
};
static int invoke(void* opaque,TargetState48* state,const TargetUpdateRequest24* request,std::uint32_t* result){
 auto& c=*static_cast<Context*>(opaque);const auto op=request->service;assert(op<11);
 c.trace.push_back({op,request->event,static_cast<std::uint32_t>(request->subject),static_cast<std::uint32_t>(request->other)});
 const auto occurrence=++c.counts[op];
 const std::array<unsigned,11> slots{0,1,occurrence==1?3u:4u,5,6,7,8,9,10,11,0};
 *result=op==10?0:c.row[slots[op]];
 if(c.row[14]==op&&!c.triggered){
  c.triggered=true;
  switch(c.row[15]){
   case 1:state->target=0;break;
   case 2:state->target=4;break;
   case 3:state->owner=c.alternate;break;
   case 4:state->alive=255;state->sight=255;break;
   case 5:c.trace.push_back({11,0,0,0});assert(dh2_character_target_update(state,c.services)==0);c.trace.push_back({12,0,0,0});break;
   default:assert(false);
  }
 }
 return static_cast<int>(op)==c.fail?1:0;
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);char magic[4];file.read(magic,4);assert(std::memcmp(magic,"CTU1",4)==0);
 auto word=[&](){std::uint32_t x=0;file.read(reinterpret_cast<char*>(&x),4);assert(file);return x;};
 const auto cases=word();std::uint32_t services_count=0,events=0,checks=0;
 TargetOwner16 owners[2]{{1,55,0,0},{2,55,0,0}};TargetState48 state{};Context context;context.alternate=&owners[1];TargetUpdateServices16 services{&context,invoke};context.services=&services;
 auto reset=[&](){state={11,&owners[0],4,context.row[2]?3u:0u,4,static_cast<std::uint8_t>(context.row[12]),static_cast<std::uint8_t>(context.row[13]),77,0,0};context.counts.fill(0);context.trace.clear();context.triggered=false;context.fail=-1;};
 for(std::uint32_t i=0;i<cases;++i){
  for(auto& x:context.row){x=word();}
  std::array<std::uint32_t,7> expected{};for(auto& x:expected){x=word();}
  const auto count=word();assert(count<1000);std::vector<Entry> trace(count);for(auto& entry:trace)for(auto& x:entry){x=word();}reset();
  assert(dh2_character_target_update(&state,&services)==0);
  const std::array<std::uint32_t,7> actual{static_cast<std::uint32_t>(state.owner->identity),static_cast<std::uint32_t>(state.candidate),static_cast<std::uint32_t>(state.target),static_cast<std::uint32_t>(state.last_target),state.alive,state.sight,state.changed};
  assert(actual==expected&&context.trace==trace);checks+=7+4*count;services_count+=count;for(const auto& entry:trace)events+=entry[0]==10;
 }
 assert(file.peek()==EOF);std::uint32_t guards=0;
 context.row={0,0,1,1,1,44,0,1,1,0,1,1,0,0,UINT32_MAX,0};reset();const auto original=state;
 auto atomic=[&](TargetState48* s,const TargetUpdateServices16* svc){context.trace.clear();assert(dh2_character_target_update(s,svc)==1&&context.trace.empty()&&std::memcmp(&state,&original,sizeof state)==0);++guards;};
 atomic(nullptr,&services);atomic(&state,nullptr);TargetUpdateServices16 empty{&context,nullptr};atomic(&state,&empty);
 alignas(TargetState48) std::array<unsigned char,sizeof(TargetState48)+8> unaligned{};atomic(reinterpret_cast<TargetState48*>(unaligned.data()+1),&services);
 alignas(TargetUpdateServices16) std::array<unsigned char,sizeof(TargetUpdateServices16)+8> unaligned_services{};atomic(&state,reinterpret_cast<TargetUpdateServices16*>(unaligned_services.data()+1));
 state.owner=reinterpret_cast<TargetOwner16*>(unaligned.data()+1);context.trace.clear();assert(dh2_character_target_update(&state,&services)==1&&context.trace.empty());++guards;state=original;
 state.reserved=1;assert(dh2_character_target_update(&state,&services)==1&&context.trace.empty());++guards;state=original;
 owners[0].reserved16=1;assert(dh2_character_target_update(&state,&services)==1&&context.trace.empty());++guards;owners[0].reserved16=0;
 // Failure is delivered separately from raw source query words. Previously
 // executed mutations are retained; no service is silently treated as true.
 std::uint32_t failure_checks=0;
 for(const auto op:{0,1,2,3,4,5,6,7,8,10}){reset();context.fail=op;assert(dh2_character_target_update(&state,&services)==2);assert(!context.trace.empty()&&context.trace.back()[0]==static_cast<unsigned>(op));++failure_checks;}
 context.row[8]=0;reset();context.fail=9;assert(dh2_character_target_update(&state,&services)==2&&context.trace.back()[0]==9);++failure_checks;
 context.row[14]=3;context.row[15]=1;reset();assert(dh2_character_target_update(&state,&services)==2&&state.target==0&&context.trace.back()[0]==3);++failure_checks;
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_services\":"<<services_count<<",\"raise_events\":"<<events<<",\"word_checks\":"<<checks<<",\"native_guards\":"<<guards<<",\"provider_failure_checks\":"<<failure_checks<<",\"mismatches\":0}\n";
}
