#include "../character_gameplay_save_v1.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character_gameplay_save_v1;
namespace {
unsigned checks=0;
void check(bool ok,const char* label){++checks;if(!ok)throw std::runtime_error(label);}
struct Backing {
 dh2::data::PlayerSavegameV1 save;
 dh2::data::PlayerSaveProfileV1 profile;
 std::uintptr_t& q174;
 std::uintptr_t& q114;
 std::vector<unsigned>* trace=nullptr;
 int fail_load=0;
 dh2::data::PlayerSaveLoadOwnerV1 loader;
 SaveRef ref;
 explicit Backing(unsigned identity,std::vector<unsigned>& events):q174(save.source_quest_log_118().character_5c),q114(save.source_quest_log_b8().character_5c),trace(&events),loader(save,profile,{std::make_shared<int>(0),[this](const auto& q,auto&,auto& error){
  trace->push_back(200+unsigned(q.operation));if(fail_load){error="load failure";return false;}return true;
 }}),ref(borrow_save(identity,save,&loader)){q174=77;q114=88;}
};
struct Fixture {
 std::vector<unsigned> trace;
 Backing a{11,trace},b{22,trace};SaveRef* slot=nullptr;
 Character character{101,&slot};Runtime* runtime=nullptr;int mutation=0,fail=0;unsigned virtual_epoch=0;
 Fixture(){a.save.set_slot(-1);b.save.set_slot(-1);}
 Services services(){return {this,[](void* raw,const Request& q,SaveRef*& response,std::string& error)->int {
  auto& f=*static_cast<Fixture*>(raw);const auto operation=unsigned(q.operation);
  f.trace.push_back(operation*1000+unsigned(q.character));
  if(q.operation==Operation::allocate_save){check(q.allocation_bytes==0x198&&q.allocator_tag==0&&q.save==nullptr,"exact source allocator request");response=&f.a.ref;}
  if(q.operation==Operation::construct_blank_save){check(q.save==&f.a.ref&&q.allocation_bytes==0,"captured Save constructor");
   if(f.mutation&1)f.slot=&f.b.ref;
   if(f.mutation&2)response=&f.b.ref; // C1 r0 is discarded; allocator identity survives.
   check(q.save->save->slot()==-1&&q.save->save->level()==0&&q.save->save->class_id()==-1,"existing blank constructor projection");
  }
  if(q.operation==Operation::virtual_init_post){check(q.virtual_slot==0x1c,"source InitPost virtual slot");if(f.mutation&1)f.virtual_epoch=1;if(f.mutation&2)f.character.identity=202;}
  if(q.operation==Operation::virtual_init_final){check(q.virtual_slot==0x58,"source fresh InitFinal slot");f.trace.push_back(4000+f.virtual_epoch);}
  if(f.mutation&4){Result nested;std::string e;check(f.runtime->load(0,&nested,e)==Status::busy,"same runtime rejects reentry");}
  if(f.fail==int(operation)+1){error="fixture provider failure";return 1;}
  if(f.fail==10+int(operation))throw std::runtime_error("fixture provider exception");
  return 0;
 }};}
};
Status execute(Runtime& r,unsigned op,unsigned arg,Result& result,std::string& e){switch(op){case 0:return r.initialize_player_savegame(&result,e);case 1:return r.set_player(arg,&result,e);case 2:return r.set_slot(arg,&result,e);case 3:return r.load(std::int32_t(arg),&result,e);default:return r.init_all(&result,e);}}
void print(const Fixture& f,const Result& r,Status status){std::cout<<"{\"status\":"<<unsigned(status)<<",\"character\":"<<r.captured_character<<",\"save\":"<<r.captured_save<<",\"slot_identity\":"<<(f.slot?f.slot->identity:0)<<",\"fields\":["<<f.a.q174<<','<<f.a.save.character()<<','<<f.a.q114<<','<<std::uint32_t(f.a.save.slot())<<','<<f.b.q174<<','<<f.b.save.character()<<','<<f.b.q114<<','<<std::uint32_t(f.b.save.slot())<<"],\"providers\":"<<r.provider_calls<<",\"stores\":"<<r.stores<<",\"loads\":"<<r.load_calls<<",\"trace\":[";bool comma=false;for(const auto value:f.trace)if(value<200||value>=300){std::cout<<(comma?",":"")<<value;comma=true;}std::cout<<"]}\n";}
}
int main(int argc,char** argv){try{
 if(argc==5){const unsigned op=std::strtoul(argv[1],nullptr,0),argument=std::strtoul(argv[2],nullptr,0),present=std::strtoul(argv[3],nullptr,0);Fixture f;f.mutation=std::atoi(argv[4]);f.slot=present?&f.b.ref:nullptr;Runtime runtime(f.character,f.services());f.runtime=&runtime;Result result;std::string error;const auto status=execute(runtime,op,argument,result,error);check(status==Status::complete,"original comparison complete");print(f,result,status);return 0;}
 unsigned cases=0;
 for(unsigned op=0;op<5;++op)for(unsigned present=0;present<2;++present){Fixture f;f.slot=present?&f.b.ref:nullptr;Runtime runtime(f.character,f.services());f.runtime=&runtime;Result result;std::string error;check(execute(runtime,op,7,result,error)==Status::complete,"whole wrapper delivered");++cases;}
 for(unsigned op:{0u,4u})for(int failure:{1,2,3,4,10,11,12,13}){Fixture f;f.slot=&f.b.ref;f.fail=failure;Runtime runtime(f.character,f.services());f.runtime=&runtime;Result r;std::string e;const auto status=execute(runtime,op,0,r,e);const bool reached=op==0?(failure==1||failure==2||failure==10||failure==11):(failure==3||failure==4||failure==12||failure==13);check(status==(reached?Status::failed:Status::complete),"provider failure prefix");if(op==0&&reached)check(f.slot==&f.b.ref&&f.a.save.character()==0,"no publication before successful constructor");if(op==4&&reached)check(r.provider_calls==unsigned((failure==3||failure==12)?1:2),"no extra virtual after failure");++cases;}
 for(unsigned op:{0u,4u}){Fixture f;f.mutation=7;Runtime runtime(f.character,f.services());f.runtime=&runtime;Result r;std::string e;check(execute(runtime,op,0,r,e)==Status::complete,"reentry guard preserves dispatch");++cases;}
 {Fixture f;f.slot=&f.b.ref;f.b.ref.loader=&f.a.loader;Runtime runtime(f.character,f.services());Result r;std::string e;check(runtime.load(0,&r,e)==Status::failed&&r.load_calls==0,"different Save LoadOwner rejects");++cases;}
 {Fixture f;f.slot=&f.b.ref;f.b.ref.quest_character_114=nullptr;Runtime runtime(f.character,f.services());Result r;std::string e;check(runtime.set_player(99,&r,e)==Status::failed&&r.stores==0&&f.b.q174==77,"missing actual quest field rejects before stores");++cases;}
 {Fixture f;f.slot=&f.b.ref;f.b.fail_load=1;Runtime runtime(f.character,f.services());Result r;std::string e;check(runtime.load(2,&r,e)==Status::failed&&r.load_calls==1&&f.trace.size()==1,"actual selected LoadOwner failure prefix");++cases;}
 {Fixture f;Runtime runtime(f.character);Result r;std::string e;check(runtime.init_all(&r,e)==Status::failed&&r.provider_calls==1,"reached missing InitPost provider");check(runtime.initialize_player_savegame(&r,e)==Status::failed&&r.provider_calls==1,"reached missing allocator");++cases;}
 {Fixture f;Runtime runtime(f.character,f.services());std::string e;check(runtime.load(0,nullptr,e)==Status::invalid_argument,"missing output");++cases;}
 {Fixture f;f.slot=&f.b.ref;Runtime runtime(f.character,f.services());Result r;std::string e;
  check(f.b.ref.quest_character_174==&f.b.save.source_quest_log_118().character_5c&&f.b.ref.quest_character_114==&f.b.save.source_quest_log_b8().character_5c,"exact canonical embedded aliases");
  check(reinterpret_cast<std::uintptr_t>(f.b.ref.quest_character_174)>=reinterpret_cast<std::uintptr_t>(&f.b.save)&&reinterpret_cast<std::uintptr_t>(f.b.ref.quest_character_174)<reinterpret_cast<std::uintptr_t>(&f.b.save)+sizeof(f.b.save),"quest174 is inside actual Save");
  check(reinterpret_cast<std::uintptr_t>(f.b.ref.quest_character_114)>=reinterpret_cast<std::uintptr_t>(&f.b.save)&&reinterpret_cast<std::uintptr_t>(f.b.ref.quest_character_114)<reinterpret_cast<std::uintptr_t>(&f.b.save)+sizeof(f.b.save),"quest114 is inside actual Save");
  check(runtime.set_player(99,&r,e)==Status::complete&&r.stores==3&&f.b.q174==99&&f.b.save.character()==99&&f.b.q114==99,"embedded owner stores accepted");
  check(runtime.load(0,&r,e)==Status::complete&&r.load_calls==1,"same actual Save loader accepted");++cases;
 }
 {Fixture f;f.b.save.set_character_only(99);check(f.b.save.character()==99&&f.b.q174==77&&f.b.q114==88,"Save-only setter does not fan out");f.b.save.set_character(101);check(f.b.q174==101&&f.b.save.character()==101&&f.b.q114==101,"convenience setter composes canonical stores");++cases;}
 for(unsigned invalid=0;invalid<7;++invalid){Fixture f;f.slot=&f.b.ref;std::uintptr_t detached=66;
  switch(invalid){
   case 0:std::swap(f.b.ref.quest_character_174,f.b.ref.quest_character_114);break;
   case 1:f.b.ref.quest_character_174=&f.a.q174;break;
   case 2:f.b.ref.quest_character_114=&f.a.q114;break;
   case 3:f.b.ref.quest_character_174=&f.b.q114;break;
   case 4:f.b.ref.quest_character_114=&f.b.q174;break;
   case 5:f.b.ref.quest_character_174=&detached;break;
   case 6:f.b.ref.quest_character_114=&detached;break;
  }
  Runtime runtime(f.character,f.services());Result r;std::string e;check(runtime.set_player(99,&r,e)==Status::failed&&r.stores==0&&f.slot==&f.b.ref&&f.b.q174==77&&f.b.q114==88&&f.b.save.character()==0&&f.a.q174==77&&f.a.q114==88&&f.a.save.character()==0&&detached==66,"swapped foreign duplicate detached fields reject before mutation");++cases;
 }
 {Fixture f;f.slot=&f.b.ref;std::swap(f.a.ref.quest_character_174,f.a.ref.quest_character_114);Runtime runtime(f.character,f.services());f.runtime=&runtime;Result r;std::string e;check(runtime.initialize_player_savegame(&r,e)==Status::failed&&r.stores==0&&f.slot==&f.b.ref&&f.a.q174==77&&f.a.q114==88&&f.a.save.character()==0,"invalid constructed aliases reject before publication");++cases;}
 {Fixture f;const auto accepted=borrow_save(33,f.b.save,&f.b.loader),rejected=borrow_save(44,f.b.save,&f.a.loader);check(accepted.save==&f.b.save&&accepted.loader==&f.b.loader&&accepted.quest_character_174==&f.b.q174&&accepted.quest_character_114==&f.b.q114,"borrow accepts actual same loader and fields");check(rejected.identity==0&&!rejected.save&&!rejected.loader&&!rejected.quest_character_174&&!rejected.quest_character_114&&f.a.save.character()==0&&f.b.save.character()==0,"borrow rejects foreign loader without mutation");++cases;}
 std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"checks\":"<<checks<<"}\n";return 0;
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
