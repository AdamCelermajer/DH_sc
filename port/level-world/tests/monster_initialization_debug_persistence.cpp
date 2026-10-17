// Reuse the unchanged root-owned Lua/class/property initialization driver.
// This test only supplies the previously unresolved real debug/persistence side.
#define main original_monster_initialization_baseline_main
#include "monster_initialization_session.cpp"
#undef main
#include "../debug_switches_runtime.hpp"
#include "../debug_switches_persistence.hpp"
#include "../../persistence/binary.h"
#include <filesystem>
#include <memory>

namespace ds=dh2::debug_switches;
namespace dp=dh2::debug_switches_persistence;
struct PositiveFixture:Fixture {
 struct ReadResource {std::vector<unsigned char> bytes;bool closed=false;};
 struct WriteResource {std::ofstream file;bool closed=false;};
 ds::Runtime debug_owner{0x500000001ull};ds::FileSystem files{0x600000001ull};ds::Engine engine{0x700000001ull,&files};ds::Application debug_application{0x800000001ull,&engine};
 ds::Globals debug_globals{0,&debug_owner,&debug_application};
 ds::Services debug_services{this,debug_operation};dp::Services write_services{this,write_operation};
 std::map<std::uintptr_t,std::unique_ptr<ReadResource>> reads;
 std::map<std::uintptr_t,std::unique_ptr<WriteResource>> writes;
 std::map<std::uintptr_t,std::unique_ptr<std::string>> strings;
 std::vector<unsigned> regen_order;std::vector<rg::Result> regenerations;
 std::filesystem::path filename;
 unsigned debug_loads=0,debug_queries=0,constructed=0,destroyed=0,read_opens=0,read_closes=0,write_opens=0,write_closes=0,save_calls=0,write_calls=0;
 unsigned fail_write_at=0,fail_query_at=0;std::uint8_t last_debug_value=77;
 PositiveFixture(CharacterTable& c,ClassTables& cl,PropertyRules& r,dh2_pycst_view& d,const char* name,
                 const std::filesystem::path& folder,const std::vector<unsigned char>& configuration):Fixture(c,cl,r,d,name) {
  std::filesystem::create_directories(folder);filename=folder/"DebugSwitches.savegame";
  std::ofstream initial(filename,std::ios::binary|std::ios::trunc);
  require(bool(initial),"configuration file creation failed");initial.write(reinterpret_cast<const char*>(configuration.data()),std::streamsize(configuration.size()));initial.close();require(bool(initial),"configuration initial persistence failed");
 }
 static PositiveFixture& fixture(void* p){return *static_cast<PositiveFixture*>(static_cast<Fixture*>(p));}
 dp::Owner debug_view(){return {debug_owner.identity(),&debug_owner.switches(),&debug_owner.modules()};}
 static int debug_operation(void* p,const ds::Request* request,ds::File* reply){
  auto& s=*static_cast<PositiveFixture*>(p);require(request->owner==&s.debug_owner,"wrong debug owner");
  if(request->operation==ds::Operation::open_read){
   require(request->filesystem==s.files.identity&&std::string(request->path)=="DebugSwitches.savegame","wrong read resource");
   auto resource=std::make_unique<ReadResource>();resource->bytes=read(s.filename.string());
   const auto id=reinterpret_cast<std::uintptr_t>(resource.get());*reply={id,resource->bytes.data(),resource->bytes.size()};
   s.reads.emplace(id,std::move(resource));++s.read_opens;return 0;
  }
  if(request->operation==ds::Operation::close){
   require(request->filesystem==s.files.identity&&request->file,"wrong read close");auto found=s.reads.find(request->file->identity);require(found!=s.reads.end()&&!found->second->closed,"dead read backing");found->second->closed=true;++s.read_closes;return 0;
  }
  ++s.save_calls;dp::Result result{};
  return dp::save(s.debug_view(),s.debug_globals,s.debug_services,s.write_services,result)==dp::Status::complete?0:1;
 }
 static int write_operation(void* p,const dp::Request* request,dp::Stream* reply){
  auto& s=*static_cast<PositiveFixture*>(p);require(request->owner==s.debug_owner.identity(),"wrong writer owner");++s.write_calls;
  if(request->operation==dp::Operation::open_write){
   require(request->filesystem==s.files.identity&&request->word==1&&std::string(request->bytes)=="DebugSwitches.savegame","wrong write open");
   auto resource=std::make_unique<WriteResource>();resource->file.open(s.filename,std::ios::binary|std::ios::trunc);require(bool(resource->file),"real write open failed");
   const auto id=reinterpret_cast<std::uintptr_t>(resource.get());*reply={id};s.writes.emplace(id,std::move(resource));++s.write_opens;
  }else{
   auto found=s.writes.find(request->stream);require(found!=s.writes.end()&&!found->second->closed,"dead writable stream");auto& file=found->second->file;
   if(request->operation==dp::Operation::close){require(request->filesystem==s.files.identity,"wrong write close filesystem");file.close();require(bool(file),"real write close failed");found->second->closed=true;++s.write_closes;}
   else{
    unsigned char data[4]{};
    if(request->operation==dp::Operation::write_word){dh2_save_write32(data,request->word);file.write(reinterpret_cast<const char*>(data),4);}
    else if(request->operation==dp::Operation::write_string){dh2_save_write32(data,std::uint32_t(request->size));file.write(reinterpret_cast<const char*>(data),4);file.write(request->bytes,std::streamsize(request->size));}
    else {require(request->operation==dp::Operation::write_byte,"unknown write operation");const auto value=std::uint8_t(request->word);file.write(reinterpret_cast<const char*>(&value),1);}
    file.flush();require(bool(file),"real stream write failed");
   }
  }
  return s.fail_write_at==s.write_calls?1:0;
 }
 static int regen_operation(void* p,const rg::Request* request,rg::Reply* reply){
  auto& s=fixture(p);s.regen_order.push_back(unsigned(request->operation));
  if(request->operation==rg::Operation::read_property||request->operation==rg::Operation::add_property){
   if(request->operation==rg::Operation::add_property)require(s.strings.empty(),"stat add preceded native string destruction");
   return Fixture::regen(static_cast<Fixture*>(&s),request,reply);
  }
  if(request->operation==rg::Operation::debug_load){require(request->subject==s.debug_owner.identity(),"wrong captured debug owner");++s.debug_loads;return s.debug_owner.load(s.debug_globals,s.debug_services)==ds::Status::complete?0:1;}
  if(request->operation==rg::Operation::string_construct){require(request->text&&std::string(request->text)=="isTracingChar_Stats","wrong stat tracing key");auto text=std::make_unique<std::string>(request->text);const auto id=reinterpret_cast<std::uintptr_t>(text.get());reply->identity=id;s.strings.emplace(id,std::move(text));++s.constructed;return 0;}
  if(request->operation==rg::Operation::debug_query){require(request->subject==s.debug_owner.identity(),"query changed captured debug owner");auto found=s.strings.find(request->sheet);require(found!=s.strings.end(),"query string token not retained");++s.debug_queries;if(s.fail_query_at==s.debug_queries)return 1;const auto status=s.debug_owner.get_switch(*found->second,s.debug_globals,s.debug_services,s.last_debug_value);reply->word=s.last_debug_value;return status==ds::Status::complete?0:1;}
  require(request->operation==rg::Operation::string_destroy,"unknown regeneration operation");require(s.strings.erase(request->subject)==1,"native string destroyed twice");++s.destroyed;return 0;
 }
 static int level_operation(void* p,sl::Character* character,const sl::Request* request,std::uint32_t* out){
  auto& run=*static_cast<Fixture::LevelRun*>(p);auto& s=*static_cast<PositiveFixture*>(run.fixture);
  if(request->operation!=sl::Operation::regen_hp&&request->operation!=sl::Operation::regen_mp)return Fixture::set_operation(p,character,request,out);
  s.trace.emplace_back(request->operation==sl::Operation::regen_hp?"RegenHP":"RegenMP");
  rg::State state{s.owner,reinterpret_cast<std::uintptr_t>(&s.properties),reinterpret_cast<std::uintptr_t>(s.properties.resolved.data())};rg::Globals globals{s.debug_owner.identity()};rg::Services services{static_cast<Fixture*>(&s),regen_operation};rg::Result result{};
  const auto status=request->operation==sl::Operation::regen_hp?rg::regen_hp(&state,&globals,request->argument,&services,&result):rg::regen_mp(&state,&globals,request->argument,&services,&result);
  s.regenerations.push_back(result);return status==rg::Status::complete?0:1;
 }
 static int set_level_with_debug(void* p,std::uintptr_t id,float raw){
  auto& s=fixture(p);s.subject(id);s.trace.emplace_back("SetLevel");++s.set_level_calls;
  Fixture::LevelRun run{static_cast<Fixture*>(&s),raw};sl::Character character{s.owner,reinterpret_cast<std::uintptr_t>(&s.properties),std::uint32_t(s.properties.base[19])};
  sl::Arguments args{reinterpret_cast<std::uintptr_t>(&run.argument),1,3};sl::Globals globals{&s.application};sl::Services services{&run,level_operation};
  return sl::set_level(&character,&args,&globals,&services,&s.last_level)==sl::Status::complete?0:1;
 }
 void load_with_debug(ms::Source common,ms::Source monster){std::string error;auto s=Fixture::services();s.set_level=set_level_with_debug;
  require(vm.create(s,error)==ms::Status::complete&&vm.bind_functions(error)==ms::Status::complete&&vm.load_common(common,error)==ms::Status::complete&&vm.load_external(monster,error)==ms::Status::complete,error.c_str());trace.clear();}
 void check_positive(const std::vector<unsigned char>& original_configuration){
  require(regenerations.size()==2&&regenerations[0].positive_amount>0&&regenerations[1].positive_amount>0&&regenerations[0].added&&regenerations[1].added,"positive source regeneration not reached");
  require(properties.resolved[36]==properties.resolved[38]&&properties.resolved[41]==properties.resolved[43],"live properties not restored by source adds");
  require(debug_globals.loaded==1&&debug_loads==2&&debug_queries==2&&constructed==2&&destroyed==2&&strings.empty()&&read_opens==1&&read_closes==1,"debug/native string lifetime differs");
  require(save_calls==5&&write_opens==5&&write_closes==5&&debug_owner.modules().empty()&&debug_owner.switches().size()==24&&debug_owner.switches().at("isTracingChar_Stats")==0,"actual debug configuration effects differ");
  require(read(filename.string())==original_configuration,"real final save wire image differs");
  const std::vector<unsigned> expected{0,0,1,2,3,4,5,0,0,1,2,3,4,5};require(regen_order==expected,"positive debug/query/destroy/add ordering differs");
  for(const auto& resource:reads)require(resource.second->closed&&resource.second->bytes==original_configuration,"original opened read backing lost during saves");
 }
};
int main(int argc,char** argv){try{
 require(argc==5,"pass cache/common/monster/temp-folder");const std::string root=std::string(argv[1])+"/data/pydata/";std::string error;
 auto cp=read(root+"character_properties_pyarray.bin"),cn=read(root+"character_properties_pyarraynames.bin"),cs=read(root+"character_properties_pystructnames.bin");CharacterTable characters;PropertyRules rules;require(load_characters(bytes(cp),bytes(cn),bytes(cs),characters,error)&&load_property_rules(characters,rules,error),error.c_str());
 auto ca=read(root+"character_classes_pyarray.bin"),can=read(root+"character_classes_pyarraynames.bin"),cas=read(root+"character_classes_pystructnames.bin"),design_bytes=read(root+"design_pycst.bin");ClassTables classes;require(load_classes(bytes(ca),bytes(can),bytes(cas),classes,error),error.c_str());dh2_pycst_view design{};require(!dh2_pycst_open(&design,design_bytes.data(),design_bytes.size()),"design cache rejected");
 auto la=read(root+"levels_pyarray.bin"),lan=read(root+"levels_pyarraynames.bin"),las=read(root+"levels_pystructnames.bin");LevelTables levels;require(load_levels(bytes(la),bytes(lan),bytes(las),levels,error),error.c_str());
 const auto configuration=read(std::string(argv[1])+"/DebugSwitches.savegame");auto common_bytes=read(argv[2]),monster_bytes=read(argv[3]);ms::Source common{common_bytes.data(),common_bytes.size()},monster{monster_bytes.data(),monster_bytes.size()};const std::filesystem::path folder(argv[4]);unsigned cases=0,positive_cases=0;
 for(auto name:{"Crypt_Ghost","Crypt_Ghost_RE"})for(auto level_name:{"GOTHICUS_CRYPT_01","SWAMP","INFECTED_VILLAGE_01"})for(int mode=0;mode<3;++mode){
  PositiveFixture f(characters,classes,rules,design,name,folder/std::to_string(cases),configuration);f.levels=&levels;f.level_oid=find_level(levels,level_name);f.difficulty=mode;f.properties.saved[36]-=256;f.properties.saved[41]-=256;f.load_with_debug(common,monster);
  require(f.vm.dispatch(ms::Event::init,0,error)==ms::Status::complete,error.c_str());f.check_positive(configuration);
  std::int32_t minimum=0;require(read_level_range_word(levels,f.level_oid*72,0x3c+mode*4,minimum)&&f.properties.base[19]==minimum*256,"source actual catalogue level differs");
  require(f.vm.dispatch(ms::Event::enemy_spotted,0x1234567800000091ull,error)==ms::Status::complete&&f.target==0x1234567800000091ull&&f.vm.statistics().completed_callbacks==2,"same initialized VM failed source enemy callback");++cases;++positive_cases;
 }
 for(auto name:{"Crypt_Ghost","Crypt_Ghost_RE"}){
  PositiveFixture f(characters,classes,rules,design,name,folder/std::to_string(cases),configuration);require(f.debug_owner.load(f.debug_globals,f.debug_services)==ds::Status::complete&&f.debug_owner.set_switch("isTracingChar_Stats",1,f.debug_globals,f.debug_services)==ds::Status::complete,"real tracing switch update failed");
  const auto persisted=read(f.filename.string());require(persisted!=configuration&&f.save_calls==6,"true tracing switch did not save");f.properties.saved[36]-=256;f.properties.saved[41]-=256;f.load_with_debug(common,monster);require(f.vm.dispatch(ms::Event::init,0,error)==ms::Status::complete&&f.last_debug_value==1&&f.regenerations.size()==2&&f.regenerations[0].added&&f.regenerations[1].added&&f.properties.resolved[36]==f.properties.resolved[38]&&f.properties.resolved[41]==f.properties.resolved[43]&&read(f.filename.string())==persisted,"discarded tracing result changed actual regeneration");++cases;
 }
 {PositiveFixture f(characters,classes,rules,design,"Crypt_Ghost",folder/std::to_string(cases),configuration);f.load_with_debug(common,monster);require(f.vm.dispatch(ms::Event::init,0,error)==ms::Status::complete&&f.regenerations.size()==2&&!f.regenerations[0].positive_amount&&!f.regenerations[1].positive_amount&&f.debug_globals.loaded==0&&f.debug_loads==0&&f.read_opens==0&&f.save_calls==0&&f.constructed==0&&read(f.filename.string())==configuration,"zero delta fabricated debug calls");++cases;}
 {PositiveFixture f(characters,classes,rules,design,"Crypt_Ghost",folder/std::to_string(cases),configuration);f.levels=&levels;f.level_oid=find_level(levels,"SWAMP");f.difficulty=2;f.properties.saved[36]-=256;const auto saved=f.properties.saved;f.fail_write_at=2;f.load_with_debug(common,monster);require(f.vm.dispatch(ms::Event::init,0,error)==ms::Status::script_error&&f.debug_globals.loaded==1&&f.save_calls==1&&f.write_opens==1&&f.write_closes==0&&f.read_closes==0&&f.constructed==0&&f.properties.saved==saved&&!f.vm.ready()&&read(f.filename.string()).size()==4,"file error repaired prior source effects");++cases;}
 for(unsigned phase:{1u,2u}){PositiveFixture f(characters,classes,rules,design,"Crypt_Ghost",folder/std::to_string(cases),configuration);f.properties.saved[36]-=256;f.properties.saved[41]-=256;const auto old_mana=f.properties.saved[41];f.fail_query_at=phase;f.load_with_debug(common,monster);require(f.vm.dispatch(ms::Event::init,0,error)==ms::Status::script_error&&f.constructed==phase&&f.destroyed==phase-1&&f.strings.size()==1&&f.save_calls==5&&f.write_closes==5&&f.properties.saved[41]==old_mana&&!f.vm.ready(),"failed query did not retain native string/source effects");if(phase==2)require(f.properties.resolved[36]==f.properties.resolved[38],"MP failure rolled back completed HP add");++cases;}
 std::printf("{\"validation\":\"PASS\",\"host_cases\":%u,\"actual_catalogue_positive_cases\":%u,\"unchanged_original_monster_on_init\":true,\"positive_hp_and_mp_actual_source_debug_runtime\":true,\"real_temp_file_source_save_writer\":true,\"retained_read_views_across_nested_saves\":true,\"genuine_native_string_storage\":true,\"host_playerinfo_and_level_producers_are_fixtures\":true,\"native_wired\":false}\n",cases,positive_cases);return 0;
}catch(const std::exception& error){std::fprintf(stderr,"positive monster init: %s\n",error.what());return 1;}}
