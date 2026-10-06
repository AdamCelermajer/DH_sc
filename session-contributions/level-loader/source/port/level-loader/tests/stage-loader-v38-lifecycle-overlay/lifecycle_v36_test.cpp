#include "lifecycle_v36.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <vector>
using namespace dh2::loader;
static void check(bool x,const char* message){if(!x)throw std::runtime_error(message);}
static std::uint32_t word(std::istream& in){std::uint32_t n{};in.read(reinterpret_cast<char*>(&n),4);check(bool(in),"truncated original gold");return n;}
struct Fields {std::uint32_t progress{},state{},counter{},current{};LifecycleFieldsV36 borrow(){return {&progress,&state,&counter,&current};}};
static LifecycleServicesV36 fixture_services(std::vector<int>& calls){
 LifecycleServicesV36 s;
 for(int i=0;i<38;++i)s.stage_body[i]=[&,i](std::string&){calls.push_back(i);return LifecycleStepV36::complete;};
 s.publish_progress=[](auto,auto,std::string&){return true;};
 s.after_source_increment[7]=[](std::string&){return true;}; /* Explicit old-fixture counter boundary. */
 return s;
}
int main(int argc,char** argv){try{
 check(argc==2,"usage: lifecycle_v36_test ORIGINAL_GOLD");std::ifstream in(argv[1],std::ios::binary);check(bool(in),"original gold missing");
 char magic[4];in.read(magic,4);check(std::memcmp(magic,"L36G",4)==0,"bad gold");auto stages=word(in),rows=word(in);check(stages==38,"stage count");
 for(unsigned i=0;i<stages;++i)check(lifecycle_stages_v36()[i].original_entry==word(in),"original dispatch entry mismatch");
 for(unsigned i=0;i<rows;++i){Fields f;f.state=word(in);f.counter=word(in);f.current=word(in);const auto state=word(in),progress=word(in),counter=word(in),current=word(in);
  lifecycle_progress_tail_v36(f.borrow());check(std::uint32_t(f.state)==state&&std::uint32_t(f.progress)==progress&&std::uint32_t(f.counter)==counter&&std::uint32_t(f.current)==current,"original scalar tail mismatch");}
 check(in.peek()==std::char_traits<char>::eof(),"gold trailing bytes");
 unsigned safety=0;
 // Fixture stage bodies are explicit observers. This verifies orchestration,
 // not real gameplay providers or whole original _LoadProcess execution.
 {Fields f;f.current=500;std::vector<int> calls;auto s=fixture_services(calls);auto level=std::make_shared<int>(9);LifecycleV36 load(f.borrow(),level,{},s);
  while(f.state<36){check(load.tick()==LifecycleStatusV36::loading||f.state==36,"source walk failure");check(!load.diagnostics().gameplay_ready&&!load.diagnostics().whole_level_init_verified,"fake ready");}
  const auto reached=calls.size();for(int i=0;i<3;++i)check(load.tick()==LifecycleStatusV36::awaiting_end_loading&&f.state==36&&f.progress==100,"real state36 wait");
  check(calls.size()==reached+3,"state36 must execute its real polling body once per tick");
  f.state=37;check(load.tick()==LifecycleStatusV36::source_finished&&f.state==38,"real menu EndLoading transition");const auto count=calls.size();load.tick();check(calls.size()==count,"completed stage replay");++safety;}
 {Fields f;f.state=7;std::vector<int> calls;auto s=fixture_services(calls);int attempts=0;s.stage_body[7]=[&](std::string&){++attempts;return attempts<4?LifecycleStepV36::pending:LifecycleStepV36::complete;};
  LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);for(int i=0;i<3;++i){load.tick();check(f.state==7&&attempts==i+1,"map spin was not bounded");}load.tick();check(f.state==8&&attempts==4,"resumable map completion");++safety;}
 {Fields f;f.state=10;LifecycleServicesV36 s;s.publish_progress=[](auto,auto,std::string&){return true;};
 s.after_source_increment[7]=[](std::string&){return true;}; /* Explicit old-fixture counter boundary. */LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);
  check(load.tick()==LifecycleStatusV36::failed&&f.state==10,"missing InitPost advanced");check(load.diagnostics().required_service=="ObjectManager.InitPost","missing provider diagnostics");load.tick();check(f.state==10,"failed stage replay");++safety;}
 {Fields f;std::vector<int> calls;auto s=fixture_services(calls);auto resource=std::make_shared<int>(8);std::weak_ptr<int> weak=resource;auto level=std::make_shared<int>();std::weak_ptr<int> levelweak=level;int cleanup=0;
  s.cancel_and_unload=[&](std::string&){++cleanup;check(!weak.expired()&&!levelweak.expired(),"resource lease dropped before Unload");return cleanup==1?LifecycleStepV36::pending:LifecycleStepV36::complete;};
  LifecycleV36 load(f.borrow(),level,{resource},s);resource.reset();level.reset();load.request_cancel();check(load.tick()==LifecycleStatusV36::cancelling&&!weak.expired(),"pending cleanup lost lease");
  check(load.tick()==LifecycleStatusV36::cancelled&&weak.expired()&&levelweak.expired(),"cleanup did not release own leases");load.tick();check(cleanup==2&&f.state==0&&calls.empty(),"cancel replay/source-state mutation");++safety;}
 {Fields f;std::vector<int> calls;auto s=fixture_services(calls);LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);load.tick();f.state=0;check(load.tick()==LifecycleStatusV36::failed&&calls.size()==1,"external rewind replayed constructors");++safety;}
 {Fields f;std::vector<int> calls;auto s=fixture_services(calls);s.stage_body[0]=[&](std::string&){f.state=38;return LifecycleStepV36::complete;};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);check(load.tick()==LifecycleStatusV36::failed&&!load.diagnostics().gameplay_ready,"provider counterfeit completion accepted");++safety;}
 {Fields f;std::vector<int> calls;auto s=fixture_services(calls);LifecycleV36* running{};s.stage_body[0]=[&](std::string&){running->tick();return LifecycleStepV36::complete;};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);running=&load;check(load.tick()==LifecycleStatusV36::failed&&f.state==0,"reentrancy advanced state");++safety;}
 {Fields f;LifecycleV36 load(f.borrow(),{}, {},{});check(load.tick()==LifecycleStatusV36::failed,"unpinned source receiver accepted");++safety;}
 {Fields f;std::vector<int> calls;auto s=fixture_services(calls);s.publish_progress={};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);check(load.tick()==LifecycleStatusV36::failed&&f.state==1,"callback failure lost completed source prefix");load.tick();check(calls.size()==1,"publish failure replayed stage");++safety;}
 {Fields f;std::vector<int> calls;auto s=fixture_services(calls);int attempts=0;s.stage_body[0]=[&](std::string&)->LifecycleStepV36{++attempts;throw std::runtime_error("explicit provider exception");};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);check(load.tick()==LifecycleStatusV36::failed&&f.state==0,"provider exception escaped");load.tick();check(attempts==1,"thrown stage replay");++safety;}
 {Fields f;f.state=38;f.progress=7;f.counter=500;f.current=2;std::vector<int> calls;auto s=fixture_services(calls);int published=0;s.publish_progress=[&](auto state,auto progress,std::string&){++published;return state==38&&progress==100;};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);check(load.tick()==LifecycleStatusV36::source_finished&&f.progress==100&&f.counter==500&&f.current==2&&published==1&&calls.empty(),"completed source tail must set100 and skip counter min");++safety;}
 {Fields f;f.state=7;f.progress=44;std::vector<int> calls;auto s=fixture_services(calls);unsigned file13c=5;s.after_source_increment[7]=[&](std::string&){check(f.state==8&&f.progress==44,"post hook wrong source order");++file13c;return true;};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);check(load.tick()==LifecycleStatusV36::loading&&f.state==8&&file13c==6&&f.progress==21,"original130/13c/progress source order");++safety;}
 {Fields f;f.state=7;std::vector<int> calls;auto s=fixture_services(calls);s.after_source_increment[7]={};LifecycleV36 load(f.borrow(),std::make_shared<int>(),{},s);check(load.tick()==LifecycleStatusV36::failed&&f.state==8&&load.diagnostics().completed_stages==0,"missing13c service fabricated completed stage");load.tick();check(calls.size()==1,"failed post service replayed root factories");++safety;}
 std::cout<<"PASS original_dispatch_cases="<<stages<<" original_progress_cases="<<rows<<" adapter_safety_cases="<<safety<<" whole_level_init_verified=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
