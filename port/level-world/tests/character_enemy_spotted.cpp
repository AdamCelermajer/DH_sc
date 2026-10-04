#include "../character_enemy_spotted.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::character;
using Entry=std::array<std::uint32_t,12>;
struct Context {std::array<std::uint32_t,12> row{};std::vector<Entry> trace;TargetOwner16* alternate;EnemySpottedServices24* services;const std::uint32_t* first_threat;const std::uint32_t* second_threat;bool triggered=false;int fail=-1,invalidate_op=-1;};
static int invoke(void* opaque,EnemySpottedState16* state,const EnemySpottedRequest48* req,std::uint32_t* output){
 auto& c=*static_cast<Context*>(opaque);const auto op=req->service;assert(op<12&&!req->reserved&&!req->reserved2);unsigned name=0;
 if(req->text){if(std::strcmp(req->text,"isTracingCharAIEvents")==0)name=1;else {assert(std::strcmp(req->text,"isTracingThreatChange")==0);name=2;}}
 c.trace.push_back({op,static_cast<std::uint32_t>(req->subject),static_cast<std::uint32_t>(req->other),static_cast<std::uint32_t>(req->enemy),req->word,name,static_cast<std::uint32_t>(state->ai->target->owner->identity),static_cast<std::uint32_t>(state->ai->active),static_cast<std::uint32_t>(state->group),state->ai->continued,state->ai->target->changed,c.services->initial_threat==c.first_threat?0u:1u});
 if(c.row[10]==op&&!c.triggered){
  c.triggered=true;
  switch(c.row[11]){
   case 1:state->ai->active=0;break;
   case 2:state->ai->active=32;break;
   case 3:state->ai->target->owner=c.alternate;break;
   case 4:{Entry entry{};entry[0]=12;c.trace.push_back(entry);assert(dh2_character_enemy_spotted(state,3,c.services)==0);entry[0]=13;c.trace.push_back(entry);break;}
   case 5:state->group=0;break;
   case 6:c.services->initial_threat=c.second_threat;break;
   default:assert(false);
  }
 }
 *output=op==5?c.row[req->subject==3?1:2]:op==6?c.row[req->subject==3?3:4]:op==7?c.row[5]:op==8?c.row[6]:op==9?c.row[7]:op==10?c.row[8]:0;
 if(static_cast<int>(op)==c.invalidate_op&&(op!=6||req->subject!=3))state->ai=reinterpret_cast<TargetEventState32*>(std::uintptr_t{1});
 return static_cast<int>(op)==c.fail?1:0;
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1],std::ios::binary);assert(file);char magic[4];file.read(magic,4);assert(std::memcmp(magic,"CES1",4)==0);auto word=[&](){std::uint32_t x=0;file.read(reinterpret_cast<char*>(&x),4);assert(file);return x;};const auto cases=word(),threat=word();assert(threat==0x41200000);const auto alternate_threat=threat^0x80000000;
 TargetOwner16 owners[2]{{1,0,0,0},{2,0,0,0}};TargetState48 target{};TargetEventState32 ai{};EnemySpottedState16 state{};EnemySpottedServices24 services{};Context context{{},{},&owners[1],&services,&threat,&alternate_threat};services={&context,invoke,&threat};
 auto reset=[&](){target={11,&owners[0],0,0,0,1,1,77,0,0};ai={&target,context.row[9],1,{},0};state={&ai,context.row[0]?51u:0u};services.initial_threat=&threat;context.trace.clear();context.triggered=false;context.fail=context.invalidate_op=-1;};
 std::uint32_t calls=0,checks=0;
 for(std::uint32_t i=0;i<cases;++i){
  for(auto& x:context.row){x=word();}std::array<std::uint32_t,6> expected{};for(auto& x:expected){x=word();}const auto count=word();assert(count<1000);std::vector<Entry> trace(count);for(auto& entry:trace)for(auto& x:entry){x=word();}reset();assert(dh2_character_enemy_spotted(&state,3,&services)==0);
  const std::array<std::uint32_t,6> actual{static_cast<std::uint32_t>(target.owner->identity),static_cast<std::uint32_t>(ai.active),static_cast<std::uint32_t>(state.group),ai.continued,target.changed,services.initial_threat==&threat?0u:1u};assert(actual==expected&&context.trace==trace);calls+=count;checks+=6+12*count;
 }
 assert(file.peek()==EOF);context.row={1,0,0,0,0,1,1,0,0x3f800000,31,UINT32_MAX,0};reset();const auto original=state;const auto old_ai=ai;const auto old_target=target;std::uint32_t guards=0;
 auto atomic=[&](EnemySpottedState16* s,std::uintptr_t enemy,const EnemySpottedServices24* svc){context.trace.clear();assert(dh2_character_enemy_spotted(s,enemy,svc)==1&&context.trace.empty()&&std::memcmp(&state,&original,sizeof state)==0&&std::memcmp(&ai,&old_ai,sizeof ai)==0&&std::memcmp(&target,&old_target,sizeof target)==0);++guards;};
 atomic(nullptr,3,&services);atomic(&state,0,&services);atomic(&state,3,nullptr);EnemySpottedServices24 empty{&context,nullptr,&threat};atomic(&state,3,&empty);
 alignas(TargetEventState32) std::array<unsigned char,64> bad{};atomic(reinterpret_cast<EnemySpottedState16*>(bad.data()+1),3,&services);atomic(&state,3,reinterpret_cast<EnemySpottedServices24*>(bad.data()+1));
 state.ai=reinterpret_cast<TargetEventState32*>(bad.data()+1);assert(dh2_character_enemy_spotted(&state,3,&services)==1&&context.trace.empty());state=original;++guards;
 ai.target=reinterpret_cast<TargetState48*>(bad.data()+1);assert(dh2_character_enemy_spotted(&state,3,&services)==1&&context.trace.empty());ai=old_ai;++guards;
 target.owner=reinterpret_cast<TargetOwner16*>(bad.data()+1);assert(dh2_character_enemy_spotted(&state,3,&services)==1&&context.trace.empty());target=old_target;++guards;
 ai.reserved_bytes[6]=1;assert(dh2_character_enemy_spotted(&state,3,&services)==1&&context.trace.empty());ai=old_ai;++guards;
 unsigned failures=0;
 for(unsigned op=0;op<12;++op){reset();context.fail=op;assert(dh2_character_enemy_spotted(&state,3,&services)==2&&context.trace.back()[0]==op);++failures;}
 reset();services.initial_threat=nullptr;assert(dh2_character_enemy_spotted(&state,3,&services)==2&&context.trace.back()[0]==9);++failures;
 reset();services.initial_threat=reinterpret_cast<const std::uint32_t*>(bad.data()+1);assert(dh2_character_enemy_spotted(&state,3,&services)==2&&context.trace.back()[0]==9);++failures;
 // A malformed provider projection fails before constructing a request that
 // would read through it. These are native-only lifetime guard cases.
 for(const auto op:{6,7}){context.row[5]=0;reset();context.invalidate_op=op;assert(dh2_character_enemy_spotted(&state,3,&services)==2&&context.trace.back()[0]==static_cast<unsigned>(op));++failures;}
 // The authored field is required only after the exact zero-aggro branch.
 reset();context.row[7]=0xbf800000;services.initial_threat=nullptr;assert(dh2_character_enemy_spotted(&state,3,&services)==0&&context.trace.back()[0]==11);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_services\":"<<calls<<",\"word_checks\":"<<checks<<",\"authored_threat_bits\":"<<threat<<",\"native_guards\":"<<guards<<",\"provider_failure_checks\":"<<failures<<",\"mismatches\":0}\n";
}
