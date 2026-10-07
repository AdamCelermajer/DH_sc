#include "renderer_private_save_read_v39.hpp"
#include "../level-world/level_savegame_cache_v1.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>
using dh2::android_ui::read_private_level_save_v39;
namespace {unsigned checks{};std::string mutation_path;enum Mutation{none,grow,shrink,close_failure};Mutation mutation{};
void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
void write(const std::string& path,const std::vector<unsigned char>& bytes){std::ofstream file(path,std::ios::binary|std::ios::trunc);file.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());check(bool(file),"test file write");}
std::vector<unsigned char> frame(){return {2,0,0,0,4,0,0,0,'I','N','F','O',7,0,0,0,4,0,0,0,'O','B','J','S',0,0,0,0};}
}
extern "C" size_t __real_fread(void*,size_t,size_t,FILE*);
extern "C" int __real_fclose(FILE*);
extern "C" size_t __wrap_fread(void* out,size_t size,size_t count,FILE* file){
 if(mutation==grow){mutation=none;std::ofstream append(mutation_path,std::ios::binary|std::ios::app);append.put('x');append.close();}
 else if(mutation==shrink){mutation=none;std::filesystem::resize_file(mutation_path,0);}
 return __real_fread(out,size,count,file);
}
extern "C" int __wrap_fclose(FILE* file){const auto result=__real_fclose(file);if(mutation==close_failure){mutation=none;return EOF;}return result;}
int main(int argc,char** argv){try{
 check(argc==2,"isolated fixture directory required");std::filesystem::create_directories(argv[1]);const std::string directory=argv[1],name="dh2_000_1_000_000_level.savegame",path=directory+"/"+name;bool found=true;std::vector<unsigned char> bytes{9};std::string error;
 check(read_private_level_save_v39(directory,name,64,found,bytes,error)&&!found&&bytes.empty(),"genuine missing file");
 for(const auto& invalid:std::vector<std::string>{"","../x","a/b","a\\b",std::string("a\0b",3)})check(!read_private_level_save_v39(directory,invalid,64,found,bytes,error)&&!found,"invalid private filename");
 check(!read_private_level_save_v39(directory,name,0,found,bytes,error),"zero byte policy");check(!read_private_level_save_v39(directory,name,4294967296ull,found,bytes,error),"native uint32 domain");
 write(path,{});check(read_private_level_save_v39(directory,name,64,found,bytes,error)&&found&&bytes.empty(),"actual empty transport file");
 auto expected=frame();write(path,expected);check(read_private_level_save_v39(directory,name,expected.size(),found,bytes,error)&&found&&bytes==expected,"exact limit/original frame unchanged");
 auto oversized=expected;oversized.push_back(1);write(path,oversized);check(!read_private_level_save_v39(directory,name,expected.size(),found,bytes,error)&&found&&bytes.empty()&&error.find("exceeds")!=std::string::npos,"oversize before output allocation");
 write(path,expected);mutation_path=path;mutation=grow;check(!read_private_level_save_v39(directory,name,64,found,bytes,error)&&error.find("grew")!=std::string::npos&&bytes.empty(),"growth was truncated/accepted");
 write(path,expected);mutation=shrink;check(!read_private_level_save_v39(directory,name,64,found,bytes,error)&&error.find("short read")!=std::string::npos,"shrink accepted");
 write(path,expected);mutation=close_failure;check(!read_private_level_save_v39(directory,name,64,found,bytes,error)&&error.find("close failed")!=std::string::npos,"close failure accepted");
 std::filesystem::create_directories(directory+"/not-regular");check(!read_private_level_save_v39(directory,"not-regular",64,found,bytes,error)&&error.find("regular")!=std::string::npos,"directory accepted");
#if !defined(_WIN32)
 const auto fifo=directory+"/not-regular-fifo";check(!::mkfifo(fifo.c_str(),0600),"fixture FIFO create");check(!read_private_level_save_v39(directory,"not-regular-fifo",64,found,bytes,error)&&error.find("regular")!=std::string::npos,"FIFO blocked/accepted");
#endif
 struct Files{std::string directory;std::uint64_t budget=64;std::map<std::string,unsigned> calls;};Files io{directory,64,{}};
 dh2::level::LevelSavegameCacheServicesV1 services;services.context=&io;services.read_file=[](void* p,const std::string& filename,bool& hit,std::vector<std::uint8_t>& out,std::string& e){auto& f=*static_cast<Files*>(p);++f.calls[filename];return read_private_level_save_v39(f.directory,filename,f.budget,hit,out,e);};
 write(path,expected);dh2::level::LevelSavegameCacheV1 primary(services);check(primary.construct(name,false,error)&&primary.has_cached_file(),error.c_str());dh2::level::LevelSavegameFieldsV1 fields;check(primary.register_section("INFO",dh2::level::LevelSavegameSectionV1::info,fields,error)&&primary.load_section("INFO",dh2::level::LevelSavegameSectionV1::info,fields,error)&&fields.loaded_row2c==7,"genuine source INFO decode");
 write(path,{255,255,255,255});write(path+".bak",expected);dh2::level::LevelSavegameCacheV1 backup(services);check(backup.construct(name,false,error)&&backup.has_cached_file()&&io.calls[name+".bak"]==1,"source invalid-primary backup branch");
 write(path,std::vector<unsigned char>(65,0));io.calls.clear();dh2::level::LevelSavegameCacheV1 capped(services);check(!capped.construct(name,false,error)&&io.calls[name]==1&&!io.calls.count(name+".bak")&&!capped.ready(),"cap failure became fake miss/backup success");
 for(unsigned i=0;i<100;++i)check(!read_private_level_save_v39(directory,name,64,found,bytes,error)&&bytes.empty(),"repeated required cap failure");
 std::cout<<"PASS bounded private save actual files+source cache: missing/exact/oversize, nonregular/FIFO, injected growth/shrink/close errors, real INFO/backup and cap-required failure; checks="<<checks<<"\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
