#include "../visual_fx_preload.hpp"
#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::fx;
static std::uint64_t checks=0;
static void require(bool b){++checks;if(!b)throw std::runtime_error("FX audit mismatch");}
static std::uint32_t u32(std::istream& s){std::uint32_t v=0;s.read(reinterpret_cast<char*>(&v),4);require(bool(s));return v;}
struct Event {std::uint32_t op;std::string name;bool operator==(const Event& x)const{return op==x.op&&name==x.name;}};
struct Fixture {
 std::array<std::array<PreloadStep8,8>,3> steps{};std::array<PreloadSet16,3> sets{};std::array<std::int32_t,64> ids{};PreloadTable16 table{sets.data(),3,16};PreloadQueue16 queue{ids.data(),0,64};PreloadServices16 services{this,call};std::vector<Event> trace;std::uint32_t enabled=1;std::int32_t mutate=-1,reentry=-1,fail=-1;bool reentered=false;
 static int call(void* p,std::uint32_t op,const char* name,std::uint32_t* out){
  auto& f=*static_cast<Fixture*>(p);auto index=static_cast<std::int32_t>(f.trace.size());f.trace.push_back({op,name?name:""});
  if(index==f.mutate){f.sets[0].count=1;f.steps[0][0].effect_id=11;}
  if(index==f.reentry&&!f.reentered){f.reentered=true;require(dh2_fx_register_set(&f.table,&f.queue,1,&f.services)==1);}
  if(index==f.fail)return -1;*out=op==debug_module?f.enabled:0;return 0;
 }
};
struct Files {std::string directory;unsigned opens=0,closes=0;static int open(void* p,const char* name,std::uintptr_t* out){auto& f=*static_cast<Files*>(p);++f.opens;require(std::strcmp(name,"DebugSwitches.savegame")==0);FILE* handle=std::fopen((f.directory+"/"+name).c_str(),"rb");if(!handle&&errno!=ENOENT)return -1;*out=reinterpret_cast<std::uintptr_t>(handle);return 0;}static int close(void* p,std::uintptr_t handle){auto& f=*static_cast<Files*>(p);++f.closes;return std::fclose(reinterpret_cast<FILE*>(handle));}};
int main(int argc,char** argv){try{
 require(argc==3);std::ifstream gold(argv[1],std::ios::binary);char magic[4]{};gold.read(magic,4);require(std::memcmp(magic,"FXP1",4)==0);auto count=u32(gold);std::uint64_t events=0,reentries=0;
 for(std::uint32_t n=0;n<count;++n){
  Fixture f;auto id=static_cast<std::int32_t>(u32(gold));auto effect=u32(gold);f.enabled=u32(gold);f.mutate=static_cast<std::int32_t>(u32(gold));f.reentry=static_cast<std::int32_t>(u32(gold));f.queue.count=u32(gold);auto trace_count=u32(gold),out_count=u32(gold);require(f.queue.count<=64&&out_count<=64);
  for(std::uint32_t i=0;i<f.queue.count;++i)f.ids[i]=static_cast<std::int32_t>(u32(gold));
  for(unsigned i=0;i<3;++i){auto k=u32(gold);require(k<=8);f.sets[i]={f.steps[i].data(),static_cast<std::int32_t>(k),0};for(std::uint32_t j=0;j<k;++j){f.steps[i][j]={static_cast<std::int32_t>(u32(gold)),static_cast<std::int32_t>(u32(gold))};}}
  std::vector<Event> expected;for(std::uint32_t i=0;i<trace_count;++i){auto op=u32(gold),bytes=u32(gold);require(bytes<4096);std::string name(bytes,'\0');gold.read(name.data(),bytes);require(bool(gold));expected.push_back({op,name});}
  std::vector<std::int32_t> out;for(std::uint32_t i=0;i<out_count;++i)out.push_back(static_cast<std::int32_t>(u32(gold)));
  require((effect?dh2_fx_register_effect(&f.table,&f.queue,id,&f.services):dh2_fx_register_set(&f.table,&f.queue,id,&f.services))==1);require(f.trace==expected);require(f.queue.count==out.size());require(std::equal(out.begin(),out.end(),f.ids.begin()));events+=f.trace.size();reentries+=f.reentered;
 }
 require(gold.peek()==EOF);unsigned prefixes=0,guards=0;
 for(int i=0;i<4;++i){Fixture f;f.fail=i;require(dh2_fx_register_effect(&f.table,&f.queue,11,&f.services)==-2);require(f.trace.size()==static_cast<unsigned>(i+1)&&f.queue.count==0);++prefixes;}
 for(int i=0;i<8;++i){Fixture f;const PreloadTable16* t=&f.table;PreloadQueue16* q=&f.queue;const PreloadServices16* s=&f.services;if(i==0)t=nullptr;else if(i==1)q=nullptr;else if(i==2)s=nullptr;else if(i==3)f.queue.count=65;else if(i==4)f.table.sets=nullptr;else if(i==5)f.services.call=nullptr;else if(i==6)f.sets[0].reserved=1;else f.sets[0].count=4097;require(dh2_fx_register_set(t,q,0,s)==-1);require(f.trace.empty());require(std::all_of(f.ids.begin(),f.ids.end(),[](int x){return x==0;}));++guards;}
 {Fixture f;f.queue.capacity=0;require(dh2_fx_register_effect(&f.table,&f.queue,11,&f.services)==-3&&f.queue.count==0&&f.trace.size()==4);}
 {Fixture f;f.steps[0][0]={0,1};f.sets[0]={f.steps[0].data(),1,0};require(dh2_fx_register_set(&f.table,&f.queue,0,&f.services)==-3&&f.trace.size()==256&&f.queue.count==0);}
 Files files{argv[2]};dh2::character::DebugFileServices24 file_services{&files,Files::open,Files::close};auto* debug=dh2_character_debug_create();require(debug!=nullptr);auto* modules=dh2_fx_debug_modules_create(debug,&file_services);require(modules!=nullptr);
 PreloadServices16 services{modules,dh2_fx_debug_preload_service};std::array<PreloadSet16,78> source_sets{};PreloadStep8 step{283,0};source_sets[77]={&step,1,0};PreloadTable16 table{source_sets.data(),78,284};std::int32_t ids[4]{};PreloadQueue16 queue{ids,0,4};
 for(int i=0;i<200;++i)require(dh2_fx_register_set(&table,&queue,77,&services)==1);
 require(queue.count==1&&queue.ids[0]==283&&files.opens==1&&files.closes==0);std::uint32_t result=0;require(dh2_fx_debug_module_get(&result,modules,"AnimatedFX")==1&&result==1);std::uint32_t loaded=0,entries=0;require(dh2_character_debug_snapshot(debug,&loaded,&entries)==1&&loaded==1);
 bool trace_found=false,precache_found=false;for(std::uint32_t i=0;i<entries;++i){const char* name=nullptr;std::uint32_t value=99;require(dh2_character_debug_entry(debug,i,&name,&value)==1);if(std::strcmp(name,"isTracingDebugSwitches")==0){trace_found=true;require(value==0);}if(std::strcmp(name,"isTracingPreCached_FX")==0){precache_found=true;require(value==0);}}
 require(trace_found&&precache_found);dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);
 std::string file=files.directory+"/DebugSwitches.savegame";{std::ofstream existing(file,std::ios::binary);existing<<"unsupported source version";require(bool(existing));}
 debug=dh2_character_debug_create();modules=dh2_fx_debug_modules_create(debug,&file_services);services.context=modules;queue.count=0;require(dh2_fx_register_set(&table,&queue,77,&services)==-2&&queue.count==0);require(files.opens==2&&files.closes==1);result=99;require(dh2_fx_debug_module_get(&result,modules,"AnimatedFX")==-3&&result==99);dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);require(std::remove(file.c_str())==0);
 Dl_info module{},world{};require(dladdr(reinterpret_cast<void*>(&dh2_fx_register_set),&module)&&dladdr(reinterpret_cast<void*>(&dh2_character_debug_load),&world));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"original_cases\":"<<count<<",\"ordered_services\":"<<events<<",\"reentries\":"<<reentries<<",\"failure_prefixes\":"<<prefixes<<",\"atomic_guards\":"<<guards<<",\"real_fx77_repeats\":200,\"actual_missing_file_opens\":1,\"existing_file_rejected\":true,\"module_library\":\""<<module.dli_fname<<"\",\"world_library\":\""<<world.dli_fname<<"\"}\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
