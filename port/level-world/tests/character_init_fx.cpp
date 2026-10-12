#include "../character_init_fx.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
using namespace dh2;
static unsigned checks=0;
static void check(bool v){++checks;if(!v)throw std::runtime_error("InitFX check "+std::to_string(checks));}
static std::uint32_t read(std::istream& s){std::uint32_t v;s.read(reinterpret_cast<char*>(&v),4);check(bool(s));return v;}
struct Event{std::uint32_t op;std::string name;bool operator==(const Event& x)const{return op==x.op&&name==x.name;}};
struct Fixture{std::vector<Event> trace;data::CharacterEffects* rows=nullptr;unsigned module=1;bool mutate=false,reentry=false,entered=false;int fail=-1;};
static int service(void* p,std::uint32_t op,const char* name,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({op,name?name:""});
 if(f.mutate&&f.trace.size()==1)for(unsigned i=0;i<3;++i)f.rows[i]={15,15,15,15,0};
 if(f.reentry&&!f.entered){f.entered=true;const fx::PreloadServices16 services{&f,service};std::uintptr_t v=9;check(dh2_character_init_fx_negative_grab(&v,-1,3,&services)==1&&v==0);}
 *out=op==fx::debug_module?f.module:0;return f.fail==static_cast<int>(f.trace.size()-1)?1:0;
}
static int missing(void*,const char*,std::uintptr_t* handle){*handle=0;return 0;}
static int close(void*,std::uintptr_t){throw std::runtime_error("missing file cannot close");}
int main(int argc,char** argv){try{check(argc==2);std::ifstream in(argv[1],std::ios::binary);check(bool(in));check(read(in)==0x31584649);auto count=read(in);check(count==480);unsigned events=0,guards=0;
 fx::PreloadStep8 s0[]={{7,0}},s1[]={{1,0},{2,0}};fx::PreloadSet16 sets[]={{s0,1,0},{s1,2,0},{nullptr,0,0}};fx::PreloadTable16 table{sets,3,16};
 for(unsigned i=0;i<count;++i){auto kind=read(in);auto id=static_cast<std::int32_t>(read(in));auto index=static_cast<std::int32_t>(read(in));auto module=read(in),mutation=read(in),n=read(in),qn=read(in);data::CharacterEffects rows[3];
  for(auto& row:rows){row.blood_death=static_cast<int>(read(in));row.blood=static_cast<int>(read(in));row.footprint=static_cast<int>(read(in));row.swoosh=static_cast<int>(read(in));}
  std::vector<Event> expected;for(unsigned k=0;k<n;++k){auto op=read(in),len=read(in);std::string name(len,'\0');in.read(name.data(),len);check(bool(in));expected.push_back({op,name});}
  std::vector<int> qexpected;for(unsigned k=0;k<qn;++k)qexpected.push_back(static_cast<int>(read(in)));
  int storage[64]{};fx::PreloadQueue16 queue{storage,0,64};Fixture fixture;fixture.rows=rows;fixture.module=module;fixture.mutate=mutation;fx::PreloadServices16 services{&fixture,service};character::InitFxRows16 source{rows,3,index};std::uintptr_t out=123;
  check((kind?dh2_character_init_fx_negative_grab(&out,id,3,&services):dh2_character_init_fx_register(&source,&table,&queue,&services))==1);check(fixture.trace==expected);check(queue.count==qn&&(!qn||!std::memcmp(storage,qexpected.data(),4*qn)));if(kind)check(out==0);events+=fixture.trace.size();
 }
 check(in.peek()==std::char_traits<char>::eof());
 data::CharacterEffects rows[3]{};character::InitFxRows16 source{rows,3,1};int storage[64]{};fx::PreloadQueue16 queue{storage,0,64};Fixture fixture;fixture.rows=rows;fx::PreloadServices16 services{&fixture,service};
 for(unsigned i=0;i<8;++i){auto view=source;auto t=table;auto q=queue;auto s=services;fixture.trace.clear();
  if(i==1)view.rows=nullptr;if(i==2)view.count=0;if(i==3)view.count=4097;if(i==4)t.set_count=4097;if(i==5)q.count=65;if(i==6)s.call=nullptr;if(i==7)q.ids=nullptr;
  check(dh2_character_init_fx_register(i?&view:nullptr,&t,&q,&s)==-1&&fixture.trace.empty());++guards;
 }
 for(int i=0;i<2;++i){fixture.trace.clear();fixture.fail=i;check(dh2_character_init_fx_register(&source,&table,&queue,&services)==-2&&fixture.trace.size()==static_cast<unsigned>(i+1));}fixture.fail=-1;
 std::uintptr_t out=123;fixture.trace.clear();check(dh2_character_init_fx_negative_grab(&out,1,3,&services)==-3&&out==123&&fixture.trace.size()==2);
 fixture.trace.clear();fixture.reentry=true;check(dh2_character_init_fx_register(&source,&table,&queue,&services)==1&&fixture.entered);fixture.reentry=false;
 // Genuine shared DebugSwitches/module ownership with an explicit missing-file
 // provider fixture. Registration queues actual IDs; no FX factory is invoked.
 auto* debug=dh2_character_debug_create();character::DebugFileServices24 files{nullptr,missing,close};auto* modules=dh2_fx_debug_modules_create(debug,&files);check(debug&&modules);fx::PreloadServices16 genuine{modules,dh2_fx_debug_preload_service};
 fx::PreloadStep8 step{77,0};std::vector<fx::PreloadSet16> backing(80);backing[79]={&step,1,0};fx::PreloadTable16 actual{backing.data(),80,284};data::CharacterEffects input[]={{-1,-1,-1,-1,0},{-1,79,-1,-1,0}};character::InitFxRows16 supplied{input,2,1};queue.count=0;
 check(dh2_character_init_fx_negative_grab(&out,-1,276,&genuine)==1&&out==0);check(dh2_character_init_fx_register(&supplied,&actual,&queue,&genuine)==1&&queue.count==1&&queue.ids[0]==77);dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);
 std::cout<<"{\"validation\":\"PASS\",\"gold_cases\":"<<count<<",\"ordered_services\":"<<events<<",\"atomic_guards\":"<<guards<<",\"failed_prefix_checks\":2,\"unsupported_factory_checks\":1,\"host_reentry_checks\":1,\"genuine_Debug_missing_file_fixture_compositions\":2,\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
