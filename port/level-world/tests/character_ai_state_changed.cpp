#include "../character_ai_state_changed.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
unsigned checks=0,cases=0,calls=0,guards=0,nested=0;
void test(bool v){++checks;if(!v)throw std::runtime_error("State relay audit check "+std::to_string(checks));}
std::uint32_t word(std::istream& f){std::uint32_t v=0;f.read(reinterpret_cast<char*>(&v),4);test(bool(f));return v;}
struct Fixture {
 AIEventState64 state{};std::array<std::array<std::uintptr_t,51>,2> vtables{};
 AIEventServices24 services{this,invoke,32,0};dh2::object_identity::TargetServices16 target{this,end};
 unsigned mode=0,op=0,depth=0;int fail=0;
 std::vector<std::array<std::uint32_t,6>> trace;
 static int invoke(void* p,AIEventState64* s,const AIEventRequest40* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);test(r->service==5&&r->operation==(f.op?0x98:0x20));test(r->event==(f.op?0:0x1d));test(r->payload<=UINT32_MAX);
  if(r->operation==0x98&&r->callee==0x3dccd0){dh2::object_identity::TargetScript16 script{r->subject,0,0};return dh2_character_ais_external_end_anim(&script,&f.target);}
  f.trace.push_back({0,static_cast<unsigned>(r->subject),r->operation,static_cast<unsigned>(r->callee),static_cast<unsigned>(r->payload),r->argument});++calls;*out=0xffffffff;
  if(f.mode==1)s->active=0;
  else if(f.mode>=2){s->active=2;s->ais_virtuals=f.vtables[1].data();}
  if(f.mode>=3&&!f.depth){f.depth=1;++nested;test((f.op?dh2_character_ai_end_anim(s,&f.services):dh2_character_ai_state_changed(s,INT32_MIN,-1,&f.services))==0);}
  return f.fail;
 }
 static int end(void* p,const dh2::object_identity::TargetCall32* r){auto& f=*static_cast<Fixture*>(p);test(r->receiver==1&&!r->argument&&!r->argument_count&&!r->value_type);test(std::string(r->callback)=="OnEndOfAnim");f.trace.push_back({1,1,0,0,0,0});++calls;return f.fail;}
 void setup(unsigned operation,unsigned active,unsigned key,unsigned mutation){op=operation;mode=mutation;depth=0;fail=0;trace.clear();auto slot=op?0x98:0x20;auto empty=op?0x3dbeec:0x3dbe8c;vtables={};vtables[0][slot/4]=key;vtables[1][slot/4]=mode==4?key:empty;state={1,nullptr,nullptr,active,vtables[0].data(),255,255,255,0,0,0};services={this,invoke,32,0};}
};
int main(int argc,char** argv){try{
 test(argc==2);std::ifstream gold(argv[1],std::ios::binary);test(word(gold)==0x31435341);auto count=word(gold);test(count==1578);Fixture f;
 for(unsigned i=0;i<count;++i){std::array<unsigned,7> args{};for(auto& x:args)x=word(gold);auto active=word(gold),table=word(gold),n=word(gold);std::vector<std::array<unsigned,6>> expected(n);for(auto& row:expected)for(auto& x:row)x=word(gold);f.setup(args[0],args[1],args[2],args[5]);
  int status=0;if(args[0]==2){dh2::object_identity::TargetScript16 script{1,args[6],0};status=dh2_character_ais_external_end_anim(&script,&f.target);}else if(args[0])status=dh2_character_ai_end_anim(&f.state,&f.services);else status=dh2_character_ai_state_changed(&f.state,static_cast<std::int32_t>(args[3]),static_cast<std::int32_t>(args[4]),&f.services);
  test(!status&&f.trace==expected);test(f.state.active==active&&f.state.ais_virtuals==f.vtables[table-1].data());++cases;
 }test(gold.peek()==EOF);
 auto valid=[&](){f.setup(0,1,0x70000001,0);};valid();test(dh2_character_ai_state_changed(nullptr,0,0,&f.services)==1);++guards;
 alignas(AIEventState64) unsigned char bad[65]{};test(dh2_character_ai_state_changed(reinterpret_cast<AIEventState64*>(bad+1),0,0,&f.services)==1);++guards;
 valid();f.state.active=0;f.state.ais_virtuals=reinterpret_cast<const std::uintptr_t*>(1);test(!dh2_character_ai_state_changed(&f.state,0,0,nullptr));++guards;
 valid();f.vtables[0][8]=0x3dbe8c;test(!dh2_character_ai_state_changed(&f.state,INT32_MIN,-1,reinterpret_cast<const AIEventServices24*>(1)));++guards;
 valid();f.state.ais_virtuals=nullptr;test(dh2_character_ai_state_changed(&f.state,0,0,&f.services)==1);++guards;
 valid();test(dh2_character_ai_state_changed(&f.state,0,0,nullptr)==2);++guards;
 valid();f.services.available=0;test(dh2_character_ai_state_changed(&f.state,0,0,&f.services)==2);++guards;
 valid();f.services.reserved=1;test(dh2_character_ai_state_changed(&f.state,0,0,&f.services)==1);++guards;
 valid();f.services.invoke=nullptr;test(dh2_character_ai_state_changed(&f.state,0,0,&f.services)==2);++guards;
 valid();f.vtables[0][8]=0;test(dh2_character_ai_state_changed(&f.state,0,0,&f.services)==2);++guards;
 valid();f.mode=1;f.fail=7;test(dh2_character_ai_state_changed(&f.state,INT32_MIN,-1,&f.services)==3&&f.state.active==0&&f.trace.size()==1);++guards;
 dh2::object_identity::TargetScript16 script{1,0,0};test(dh2_character_ais_external_end_anim(&script,nullptr)==1);++guards;f.fail=7;test(dh2_character_ais_external_end_anim(&script,&f.target)==2);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"cases\":"<<cases<<",\"ordered_callbacks\":"<<calls<<",\"nested\":"<<nested<<",\"guards\":"<<guards<<"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
