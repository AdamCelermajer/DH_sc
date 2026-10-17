#include "../character_init_final_v1.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character_init_final_v1;
namespace {
unsigned checks=0;
void check(bool ok,const char* message){++checks;if(!ok)throw std::runtime_error(message);}
struct Fixture {
 Visual first{11,77},second{22,88};State state{101,0,100,&first,99};
 int roll=17,faerie=0,follower=0,local_character=0,visual=1,player_first=1,player_second=1,local=1,property=257,mutation=0;
 int fail=-1,throw_at=-1;unsigned player_queries=0;Runtime* runtime=nullptr;
 float look[3]{-2.5f,4.125f,-0.0f},target[3]{10.0f,-8.5f,3.125f},position[3]{};
 std::vector<unsigned> trace;
 Services services(){return {this,[](void* raw,State& s,const Request& q,Reply& reply,std::string& error)->int {
  auto& f=*static_cast<Fixture*>(raw);check(&s==&f.state,"same borrowed Character fields");const unsigned op=unsigned(q.operation);
  check(q.subject==((q.operation==Operation::look_at_vector||q.operation==Operation::target_position)?202:101),"captured original Character identity");
  f.trace.push_back(op);
  if(f.fail==int(op)){error="selected provider failed";return 1;}
  if(f.throw_at==int(op))throw std::runtime_error("selected provider threw");
  if(f.mutation&16){Result nested;std::string e;check(f.runtime->initialize(&nested,e)==Status::busy,"reentry rejected");}
  switch(q.operation){
   case Operation::spawn_probability:reply.word=f.roll;if(f.mutation&1)s.spawn_probability_274=31;break;
   case Operation::base_init_final:if(f.mutation&8)s.initialized_1395=0;break;
   case Operation::is_faerie:reply.word=f.faerie;break;
   case Operation::is_follower:reply.word=f.follower;break;
   case Operation::local_player:check(q.argument0==0&&q.argument1==1,"exact GetLocalPlayer(0,true)");reply.character_660=f.local_character?202:0;break;
   case Operation::look_at_vector:std::copy(f.look,f.look+3,reply.vector);break;
   case Operation::target_position:reply.target=f.target;break;
   case Operation::set_position:check(q.argument0==1&&q.vector,"source SetPosition true");std::copy(q.vector,q.vector+3,f.position);break;
   case Operation::is_player:reply.word=f.player_queries++?f.player_second:f.player_first;break;
   case Operation::light_set:check(q.name,"source light name");check(!std::strcmp(q.name,f.player_first?"SceneLight":"MonsterLight"),"pinned actual light name");reply.word=42;if(f.mutation&4)s.visual_2d8=&f.second;if(f.mutation&2)s.visual_2d8=nullptr;break;
   case Operation::char_ai_init_final:break;
   case Operation::is_local_player:reply.word=f.local;break;
   case Operation::update_all_skills:break;
   case Operation::recalculate:check(q.argument0==1,"source full recalculation true");break;
   case Operation::property_int:check(q.argument0==0xc2&&q.argument1==0,"source property194 cached false");reply.word=f.property;break;
   case Operation::save:break;
  }
  return 0;
 }};}
};
void print(const Fixture& f,const Result& r,Status status){unsigned bits[3];std::memcpy(bits,f.position,sizeof(bits));std::cout<<"{\"status\":"<<unsigned(status)<<",\"decision\":"<<unsigned(r.decision)<<",\"latch\":"<<unsigned(f.state.initialized_1395)<<",\"threshold\":"<<f.state.spawn_probability_274<<",\"visual\":"<<(f.state.visual_2d8?f.state.visual_2d8->identity:0)<<",\"light\":["<<f.first.light_set_40<<','<<f.second.light_set_40<<"],\"byte\":"<<unsigned(f.state.property_194_byte_3a8)<<",\"position\":["<<bits[0]<<','<<bits[1]<<','<<bits[2]<<"],\"calls\":"<<r.entered_calls<<",\"stores\":"<<r.stores<<",\"trace\":[";for(unsigned i=0;i<f.trace.size();++i)std::cout<<(i?",":"")<<f.trace[i];std::cout<<"]}\n";}
}
int main(int argc,char** argv){try{
 if(argc==13){Fixture f;const auto arg=[&](unsigned n){return std::strtol(argv[n],nullptr,0);};f.state.initialized_1395=std::uint8_t(arg(1));f.state.spawn_probability_274=int(arg(2));f.roll=int(arg(3));f.faerie=int(arg(4));f.follower=int(arg(5));f.local_character=int(arg(6));f.visual=int(arg(7));f.player_first=int(arg(8));f.player_second=int(arg(9));f.local=int(arg(10));f.property=int(arg(11));f.mutation=int(arg(12));if(!f.visual)f.state.visual_2d8=nullptr;Runtime runtime(&f.state,f.services());f.runtime=&runtime;Result result;std::string error;const auto status=runtime.initialize(&result,error);check(status==Status::complete,"ARM comparison reached successful caller");print(f,result,status);return 0;}
 unsigned cases=0;
 for(int failure=0;failure<16;++failure)for(bool throws:{false,true}){
  Fixture f;f.faerie=0;f.follower=1;f.local_character=1;f.fail=throws?-1:failure;f.throw_at=throws?failure:-1;Runtime runtime(&f.state,f.services());f.runtime=&runtime;Result r;std::string e;
  check(runtime.initialize(&r,e)==Status::failed,"each reached mandatory callee failure");check(r.last_operation==unsigned(failure)&&f.trace.back()==unsigned(failure)&&f.state.initialized_1395==1,"failure retains exact reached prefix/latch");
  const auto before=f.trace.size();check(runtime.initialize(&r,e)==Status::complete&&r.decision==Decision::already_initialized&&f.trace.size()==before,"explicit second source call observes retained latch");++cases;
 }
 {Fixture f;f.mutation=16;f.follower=1;f.local_character=1;Runtime runtime(&f.state,f.services());f.runtime=&runtime;Result r;std::string e;check(runtime.initialize(&r,e)==Status::complete&&f.state.property_194_byte_3a8==1,"reentry preserves entire reached flow");++cases;}
 {Fixture f;f.mutation=2;Runtime runtime(&f.state,f.services());f.runtime=&runtime;Result r;std::string e;check(runtime.initialize(&r,e)==Status::failed&&r.stores==1&&f.trace.back()==unsigned(Operation::light_set),"unsafe fresh-null visual publication fails after lookup prefix");++cases;}
 {Fixture f;Runtime runtime(&f.state);Result r;std::string e;check(runtime.initialize(&r,e)==Status::failed&&f.state.initialized_1395==1&&r.entered_calls==1,"missing reached probability provider retains latch");++cases;}
 {Fixture f;Runtime runtime(&f.state,f.services());std::string e;check(runtime.initialize(nullptr,e)==Status::invalid_argument&&f.state.initialized_1395==0,"invalid output performs no effects");++cases;}
 std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
