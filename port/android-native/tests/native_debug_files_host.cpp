#include "../app/src/main/cpp/native_debug_files.hpp"
#include "../../persistence/binary.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>

namespace fs=std::filesystem;
namespace ds=dh2::debug_switches;
namespace nf=dh2::native::debug_files;
using Bytes=std::vector<std::uint8_t>;
void require(bool value,const char* why){if(!value)throw std::runtime_error(why);}
Bytes read(const fs::path& name){std::ifstream file(name,std::ios::binary);require(bool(file),"real file open failed");return Bytes(std::istreambuf_iterator<char>(file),{});}
void store(const fs::path& name,const Bytes& bytes){std::ofstream file(name,std::ios::binary|std::ios::trunc);require(bool(file),"fixture output open failed");file.write(reinterpret_cast<const char*>(bytes.data()),static_cast<std::streamsize>(bytes.size()));file.close();require(bool(file),"fixture output write failed");}
Bytes words(std::initializer_list<std::uint32_t> values){Bytes out(values.size()*4);std::size_t at=0;for(auto value:values){dh2_save_write32(out.data()+at,value);at+=4;}return out;}
std::map<std::string,std::uint8_t> decode(const Bytes& bytes){
    std::size_t at=0;auto word=[&](){require(at<=bytes.size()&&bytes.size()-at>=4,"short persisted word");const auto value=dh2_save_read32(bytes.data()+at);at+=4;return value;};
    require(word()==0x44425357&&word()==0x20000&&word()==0,"persisted header differs");const auto count=word();std::map<std::string,std::uint8_t> result;
    for(std::uint32_t i=0;i<count;++i){const auto n=word();require(at<=bytes.size()&&n<=bytes.size()-at&&bytes.size()-at-n>=1,"short persisted row");const std::string key(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;require(result.emplace(key,bytes[at++]).second,"duplicate persisted row");}
    require(at==bytes.size(),"persisted trailing rows differ");return result;
}
void initialize(nf::Backend& backend,const fs::path& folder,const Bytes* seed=nullptr){std::string error;require(backend.initialize(folder,seed?seed->data():nullptr,seed?seed->size():0,error),error.c_str());}
ds::Status load(nf::Backend& backend){return backend.runtime().load(backend.globals(),backend.services());}
std::uintptr_t filesystem(nf::Backend& backend){return backend.globals().application->engine->files->identity;}
int operation(nf::Backend& backend,const ds::Request& request,ds::File& reply){return backend.services().invoke(backend.services().context,&request,&reply);}

int main(int argc,char** argv){try{
    require(argc==3,"pass original configuration and new host test directory");
    const auto original=read(argv[1]);require(original.size()==665&&decode(original).size()==23,"actual cache fixture differs");
    const auto parent=fs::absolute(argv[2]);fs::create_directories(parent);unsigned cases=0;
    auto folder=[&](){const auto value=parent/std::to_string(cases);require(!fs::exists(value),"host case directory already exists; use a fresh output directory");fs::create_directory(value);return value;};

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);
     require(read(backend.filename())==original&&!backend.globals().loaded&&backend.counters().save_attempts==0&&backend.counters().read_attempts==0,"asset installation fabricated source operations");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);require(load(backend)==ds::Status::complete,"actual source load failed");
     const auto& count=backend.counters();require(backend.globals().loaded==1&&backend.runtime().switches().size()==23&&backend.runtime().modules().empty(),"actual owned source maps differ");
     require(count.read_opens==1&&count.read_closes==1&&count.save_attempts==5&&count.save_completions==5&&count.write_opens==5&&count.write_closes==5&&count.word_writes==20&&count.string_writes>0&&count.byte_writes==count.string_writes&&count.bytes_written>0&&!count.io_errors&&!count.rejected_requests,"real load/save/close effects differ");
     require(read(backend.filename())==original&&backend.retained_reads()==1&&backend.retained_writes()==5&&!backend.active_reads()&&!backend.active_writes(),"real source file or lifetime differs");
     const auto attempts=count.read_attempts;require(load(backend)==ds::Status::complete&&count.read_attempts==attempts,"shared source loaded guard ignored");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);require(load(backend)==ds::Status::complete,"initial source load failed");
     require(backend.runtime().set_switch("native_test_persisted",1,backend.globals(),backend.services())==ds::Status::complete,"real source set/save failed");
     const auto persisted=read(backend.filename());require(persisted!=original&&decode(persisted).at("native_test_persisted")==1,"source setter did not persist typed row");
     const Bytes different{1,2,3};initialize(backend,path,&different);require(read(backend.filename())==persisted&&backend.globals().loaded==1,"same owner installation overwrote save or source guard");
     nf::Backend recreated;initialize(recreated,path,&original);require(read(recreated.filename())==persisted&&!recreated.globals().loaded,"backend recreation overwrote persistent file");
     require(load(recreated)==ds::Status::complete&&recreated.runtime().switches().at("native_test_persisted")==1&&read(recreated.filename())==persisted,"recreated runtime failed real persistent file");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path);require(!fs::exists(backend.filename())&&load(backend)==ds::Status::complete,"ordinary missing read was not supported");
     require(backend.globals().loaded==1&&backend.counters().read_misses==1&&!backend.counters().read_opens&&!backend.counters().save_attempts&&backend.runtime().switches().size()==6&&!fs::exists(backend.filename()),"missing read fabricated save/empty maps");
     std::uint8_t ignored=0;require(backend.runtime().get_switch("isTracingDebugSwitchesFile",backend.globals(),backend.services(),ignored)==ds::Status::complete,"source tracing query failed");
     require(backend.runtime().set_switch("created_from_missing",1,backend.globals(),backend.services())==ds::Status::complete&&decode(read(backend.filename())).at("created_from_missing")==1&&backend.counters().write_opens==1&&backend.counters().write_closes==1,"source save did not create missing real file");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);ds::File opened{};
     require(!operation(backend,{ds::Operation::open_read,&backend.runtime(),filesystem(backend),nullptr,"DebugSwitches.savegame"},opened)&&opened.identity&&opened.size==original.size(),"retained stream open failed");
     require(load(backend)==ds::Status::complete&&Bytes(opened.bytes,opened.bytes+opened.size)==original&&read(backend.filename())==original,"nested saves invalidated previously opened read bytes");
     ds::File ignored{};require(!operation(backend,{ds::Operation::close,&backend.runtime(),filesystem(backend),&opened,nullptr},ignored)&&Bytes(opened.bytes,opened.bytes+opened.size)==original&&!backend.active_reads(),"source close invalidated retained backing");
     require(operation(backend,{ds::Operation::close,&backend.runtime(),filesystem(backend),&opened,nullptr},ignored)!=0&&backend.counters().read_closes==2&&backend.counters().rejected_requests==1,"duplicate close accepted");++cases;}

    {const auto path=folder();nf::Backend backend;require(load(backend)==ds::Status::service_failed&&backend.globals().loaded==1&&backend.counters().rejected_requests==1,"uninitialized dependency was a successful no-op");initialize(backend,path,&original);require(load(backend)==ds::Status::complete&&!backend.counters().read_attempts&&read(backend.filename())==original,"failure repaired original loaded guard");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);ds::File ignored{};
     require(operation(backend,{ds::Operation::open_read,&backend.runtime(),filesystem(backend),nullptr,"../outside.save"},ignored)!=0&&!ignored.identity&&!backend.counters().read_attempts&&!fs::exists(parent/"outside.save"),"arbitrary source path accepted");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);ds::Runtime other{123};ds::File ignored{};
     require(operation(backend,{ds::Operation::save,&other,0,nullptr,nullptr},ignored)!=0&&!backend.counters().save_attempts&&read(backend.filename())==original,"foreign runtime owner accepted");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);ds::File ignored{};
     require(operation(backend,{ds::Operation::open_read,&backend.runtime(),filesystem(backend)+1,nullptr,"DebugSwitches.savegame"},ignored)!=0&&!backend.counters().read_attempts&&read(backend.filename())==original,"foreign filesystem identity accepted");++cases;}

    {const auto path=folder();nf::Backend backend;const auto malformed=words({0xdeadbeef,0x20000,0,0});initialize(backend,path,&malformed);
     require(load(backend)==ds::Status::unsupported_configuration&&backend.globals().loaded==1&&backend.counters().read_opens==1&&!backend.counters().read_closes&&backend.active_reads()==1&&backend.retained_reads()==1&&read(backend.filename())==malformed,"unsupported configuration fabricated source close/rollback");
     require(load(backend)==ds::Status::complete&&backend.counters().read_attempts==1&&backend.active_reads()==1,"retry repaired original guard or leaked resource");++cases;}

    {const auto path=folder();nf::Backend backend;const auto short_name=words({0x44425357,0x20000,0,1,256});initialize(backend,path,&short_name);
     require(load(backend)==ds::Status::invalid_argument&&backend.globals().loaded==1&&backend.active_reads()==1&&!backend.counters().read_closes&&!backend.counters().save_attempts&&backend.runtime().switches().empty()&&read(backend.filename())==short_name,"short read added cleanup or map repair");++cases;}

    {const auto path=folder();nf::Backend backend;const auto modules=words({0x44425357,0x20000,1});initialize(backend,path,&modules);
     require(load(backend)==ds::Status::unsupported_configuration&&backend.active_reads()==1&&!backend.counters().read_closes&&backend.runtime().modules().empty(),"backend fabricated unsupported source module decoding");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path);std::ofstream large(backend.filename(),std::ios::binary);large.seekp(16*1024*1024);large.put('\0');large.close();
     require(load(backend)==ds::Status::service_failed&&backend.globals().loaded==1&&backend.counters().io_errors==1&&backend.active_reads()==1&&!backend.counters().read_closes&&!backend.counters().save_attempts&&fs::file_size(backend.filename())==16u*1024u*1024u+1,"oversize IO error lost retained handle/source guard");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path);fs::create_directory(backend.filename());
     require(load(backend)==ds::Status::service_failed&&backend.globals().loaded==1&&backend.counters().io_errors==1&&!backend.counters().read_closes&&backend.runtime().switches().empty(),"real read OS error added cleanup/defaults");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path);fs::create_directory(backend.filename());backend.globals().loaded=1; // Isolate save with an explicit source-guard fixture.
     require(backend.runtime().set_switch("retained_on_io_error",1,backend.globals(),backend.services())==ds::Status::service_failed&&backend.runtime().switches().at("retained_on_io_error")==1&&backend.counters().save_attempts==1&&!backend.counters().save_completions&&backend.counters().write_attempts==1&&!backend.counters().write_opens&&!backend.counters().write_closes&&backend.counters().io_errors==1,"real writer open error rolled back source map");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path);backend.globals().loaded=1;const auto moved=path.string()+"-moved";fs::rename(path,moved);
     require(backend.runtime().set_switch("ordinary_open_miss",1,backend.globals(),backend.services())==ds::Status::complete&&backend.runtime().switches().at("ordinary_open_miss")==1&&backend.counters().save_attempts==1&&backend.counters().save_completions==1&&backend.counters().write_misses==1&&!backend.counters().write_opens&&!backend.counters().write_closes&&!backend.counters().io_errors,"ordinary source write open miss differs");++cases;}

    {const auto path=folder();nf::Backend backend;initialize(backend,path,&original);const auto different=parent/"different";fs::create_directory(different);std::string error;
     require(!backend.initialize(different,original.data(),original.size(),error)&&!error.empty()&&backend.filename()==path/"DebugSwitches.savegame"&&!fs::exists(different/"DebugSwitches.savegame")&&read(backend.filename())==original,"backend identity backing changed during reinitialize");++cases;}

    {const auto path=folder();nf::Backend backend;std::string error;require(!backend.initialize(path,nullptr,1,error)&&!fs::exists(path/"DebugSwitches.savegame"),"invalid asset bytes accepted");require(!backend.initialize(fs::path("relative"),original.data(),original.size(),error),"relative private directory accepted");require(!backend.initialize(path/"missing",original.data(),original.size(),error),"backend fabricated private directory");++cases;}

    {const auto path=folder();Bytes tiny(11,0xa5);nf::Backend backend;initialize(backend,path,&tiny);require(load(backend)==ds::Status::complete&&backend.counters().read_closes==1&&backend.counters().save_attempts==0&&!backend.active_reads()&&read(backend.filename())==tiny,"short source file should close normally without decode");++cases;}

    {const auto path=folder();nf::Backend backend;auto partial=words({0x44425357,0x20000,0,2,10});
     const std::string key="early_true";partial.insert(partial.end(),key.begin(),key.end());partial.push_back(1);const auto invalid_length=words({256});partial.insert(partial.end(),invalid_length.begin(),invalid_length.end());initialize(backend,path,&partial);
     require(load(backend)==ds::Status::invalid_argument&&backend.globals().loaded==1&&backend.runtime().switches().at(key)==1&&backend.counters().save_attempts==1&&backend.counters().save_completions==1&&backend.counters().write_closes==1&&!backend.counters().read_closes&&backend.active_reads()==1&&!backend.active_writes(),"later decode failure repaired completed save/source state");
     const auto persisted=read(backend.filename());require(persisted!=partial&&decode(persisted).at(key)==1,"later decode failure rolled back actual persistent effects");++cases;}

    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"real_source_load_and_live_map_save\":true,\"real_private_file_persistence\":true,\"retained_reads_across_nested_saves\":true,\"existing_file_preserved_on_recreation\":true,\"actual_os_error_effects\":true,\"new_original_body_claims\":0,\"android_wired\":false}\n";return 0;
}catch(const std::exception& error){std::cerr<<"native debug files: "<<error.what()<<'\n';return 1;}}
