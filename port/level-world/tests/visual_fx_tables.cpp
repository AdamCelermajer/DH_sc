#include "../visual_fx_tables.hpp"
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
std::uint64_t checks=0;
void check_impl(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("Owned effects table audit mismatch at line "+std::to_string(line));}
#define check(value) check_impl(bool(value),__LINE__)
std::vector<std::uint8_t> file(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
std::vector<std::uint8_t> project(const data::EffectsTables::Borrow& b){
 std::vector<std::uint8_t> out{'E','F','X','1'};
 auto word=[&](std::uint32_t v){for(unsigned i=0;i<4;++i)out.push_back(std::uint8_t(v>>(8*i)));};
 auto text=[&](const std::string& s){word(s.size());out.insert(out.end(),s.begin(),s.end());};
 auto integers=[&](const std::vector<std::int32_t>& values){word(values.size());for(auto v:values)word(v);};
 auto names=[&](const std::vector<std::string>& values){word(values.size());for(const auto& v:values)text(v);};
 word(b.sets().size());for(const auto& row:b.sets()){
  word(row.force_cache);word(row.loop);word(row.type);word(row.steps.size());
  for(const auto& s:row.steps){word(s.file);word(s.force_cancel);word(s.loop);word(s.orient_once);word(s.orient_with_anchor);word(s.play_time);word(s.pool_size);word(s.redir);word(s.scale_with_anchor);word(s.self_illum);word(s.speed_bits);text(s.subobject);}
 }
 word(b.characters().size());for(const auto& row:b.characters()){word(row.blood_death);word(row.blood);word(row.footprint);word(row.swoosh);word(row.trigger_floor_fx);}
 word(b.footsteps().size());for(const auto& row:b.footsteps()){word(row.effect);text(row.floor_type);integers(row.run_sounds);integers(row.walk_sounds);}
 names(b.set_names());names(b.character_names());names(b.footstep_names());names(b.dictionary().names);names(b.dictionary().values);
 word(b.set_end());word(b.character_end());word(b.data_consumed());
 unsigned dictionary_bytes=4;for(const auto& s:b.dictionary().values)dictionary_bytes+=4+s.size();word(dictionary_bytes);
 return out;
}
struct Files {
 std::filesystem::path directory;unsigned opens=0;
 static int open(void* opaque,const char* name,std::uintptr_t* out){auto& self=*static_cast<Files*>(opaque);check(!std::strcmp(name,"DebugSwitches.savegame"));++self.opens;errno=0;auto* f=std::fopen((self.directory/name).string().c_str(),"rb");if(!f&&errno!=ENOENT)return -1;*out=reinterpret_cast<std::uintptr_t>(f);return 0;}
 static int close(void*,std::uintptr_t handle){return handle?std::fclose(reinterpret_cast<FILE*>(handle)):-1;}
};
}
int main(int argc,char** argv){try{
 check(argc==3);const std::filesystem::path root=argv[1];
 const char* filenames[]={"effects_pyarray.bin","effects_pyarraynames.bin","effects_pystructnames.bin","effects_dictionary_pyarraynames.bin","effects_dictionary_pyarray.bin"};
 std::array<std::vector<std::uint8_t>,5> raw;for(unsigned i=0;i<5;++i)raw[i]=file(root/filenames[i]);const auto gold=file(root/"original-reader-projection.bin");
 auto load=[&](data::EffectsTables& owner,const std::array<std::vector<std::uint8_t>,5>& input,std::string& error){return owner.load(bytes(input[0]),bytes(input[1]),bytes(input[2]),bytes(input[3]),bytes(input[4]),error);};
 std::string error;auto owner=std::make_unique<data::EffectsTables>();if(!load(*owner,raw,error))throw std::runtime_error(error);check(error.empty());auto b=owner->borrow();check(bool(b)&&project(b)==gold);
 check(b.sets().size()==276&&b.characters().size()==3&&b.footsteps().size()==7&&b.dictionary().values.size()==284);
 check(b.set_end()==12700&&b.character_end()==12755&&b.data_consumed()==12953&&b.names_consumed()==8906&&b.schema_consumed()==346);
 check(b.set_names()[77]=="Zombie_spawn"&&b.sets()[77].steps.size()==1&&b.sets()[77].steps[0].file==283);
 check(b.dictionary().names[283]=="zombie_spawn_fx"&&b.dictionary().values[283]=="data/3D/interface/zombie_spawn_fx.bdae");
 check(b.characters()[1].blood==79&&b.characters()[1].blood_death==-1&&b.characters()[1].footprint==-1);
 auto backing=fx::PreloadBacking::create(b,error);check(bool(backing)&&error.empty());
 const auto& table=backing->table();check(table.set_count==276&&table.effect_count==284);
 for(unsigned i=0;i<table.set_count;++i){check(table.sets[i].count==static_cast<int>(b.sets()[i].steps.size()));for(int j=0;j<table.sets[i].count;++j){check(table.sets[i].steps[j].effect_id==b.sets()[i].steps[j].file);check(table.sets[i].steps[j].type==b.sets()[i].steps[j].redir);}}
 check(!load(*owner,raw,error)&&project(owner->borrow())==gold); // pinned reload
 owner.reset();b={};check(project(backing->source())==gold);
 for(auto& v:raw)std::fill(v.begin(),v.end(),0);check(project(backing->source())==gold);
 // The registration composition now reads genuine decoded cache rows, not
 // synthetic row77. Debug/file ownership and ordered unique queue are real.
 Files files{argv[2]};check(!std::filesystem::exists(files.directory/"DebugSwitches.savegame"));
 character::DebugFileServices24 providers{&files,Files::open,Files::close};
 auto* debug=dh2_character_debug_create();check(debug);auto* modules=dh2_fx_debug_modules_create(debug,&providers);check(modules);
 fx::PreloadServices16 services{modules,dh2_fx_debug_preload_service};std::array<std::int32_t,284> ids{};fx::PreloadQueue16 queue{ids.data(),0,ids.size()};
 for(unsigned i=0;i<200;++i)check(dh2_fx_register_set(&table,&queue,77,&services)==1);
 check(queue.count==1&&ids[0]==283&&files.opens==1);
 dh2_fx_debug_modules_destroy(modules);dh2_character_debug_destroy(debug);
 // Multiple truncation points in every stream must reject without publishing.
 for(unsigned i=0;i<5;++i)raw[i]=file(root/filenames[i]);
 unsigned rejections=0;
 for(unsigned stream=0;stream<5;++stream){
  const auto original=raw[stream];
  for(unsigned i=0;i<128;++i){auto input=raw;input[stream].resize((original.size()*i)/128);data::EffectsTables candidate;check(!load(candidate,input,error)&&!candidate.borrow()&&!error.empty());++rejections;}
  auto input=raw;input[stream].push_back(0);data::EffectsTables candidate;check(!load(candidate,input,error)&&!candidate.borrow());++rejections;
  input=raw;std::fill_n(input[stream].begin(),4,255);check(!load(candidate,input,error)&&!candidate.borrow());++rejections;
 }
 // A failed reload leaves a previously published unpinned snapshot intact.
 data::EffectsTables stable;check(load(stable,raw,error));auto invalid=raw;invalid[0].resize(3);check(!load(stable,invalid,error)&&project(stable.borrow())==gold);
 auto empty=fx::PreloadBacking::create({},error);check(!empty&&!error.empty());
 Dl_info world{},data{};check(dladdr(reinterpret_cast<void*>(&fx::PreloadBacking::create),&world));check(dladdr(reinterpret_cast<void*>(&data::load_dictionary),&data));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"sets\":276,\"steps\":276,\"character_rows\":3,\"footstep_rows\":7,\"dictionary_rows\":284,\"atomic_rejections\":"<<rejections<<",\"actual_fx77_registrations\":200,\"native_snapshot_pinned_after_inputs_and_loader_destroyed\":true,\"FX_factory_or_playback\":false,\"world_library\":\""<<world.dli_fname<<"\",\"data_library\":\""<<data.dli_fname<<"\"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
