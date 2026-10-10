#include "../character_debug_stdio_v136.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks{};
void check(bool value,const char* why){++checks;if(!value)throw std::runtime_error(why);}
using Bytes=std::vector<std::uint8_t>;
void word(Bytes& out,std::uint32_t value){for(unsigned shift:{0u,8u,16u,24u})out.push_back(std::uint8_t(value>>shift));}
void row(Bytes& out,const std::string& name,std::uint8_t value){word(out,static_cast<std::uint32_t>(name.size()));out.insert(out.end(),name.begin(),name.end());out.push_back(value);}
Bytes fixture(std::uint32_t version){
 Bytes out;word(out,0x44425357);word(out,version);
 if(version>=0x20000){word(out,3);row(out,"AnimatedFX",0);row(out,"Muted",1);row(out,"Muted",0);}
 word(out,6);row(out,"First",255);row(out,"False",0);row(out,"Duplicate",1);row(out,"Duplicate",0);
 row(out,"IsDisplayLoadingStepName",1);row(out,"IsDeactivatingFlashMenus",1);return out;
}
using Owner=std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)>;
Owner owner(){return {dh2_character_debug_create(),dh2_character_debug_destroy};}
std::map<std::string,std::uint32_t> snapshot(DebugSwitches* s){std::uint32_t loaded{},count{};check(dh2_character_debug_snapshot(s,&loaded,&count)==1&&loaded==1,"loaded prefix retained");std::map<std::string,std::uint32_t> out;
 for(std::uint32_t index=0;index<count;++index){const char* name{};std::uint32_t value{};check(dh2_character_debug_entry(s,index,&name,&value)==1,"actual map diagnostic");out[name]=value;}return out;
}
struct File {
 Bytes input;std::size_t at{};bool absent{},fail_open{},fail_read{},fail_read_close{},fail_write_close{},fail_write{};
 unsigned opens{},read_closes{},writes{},write_closes{};std::vector<Bytes> saves;std::vector<std::string> events;
 DebugFileServices24 files{this,open,close};
 DebugExistingFileServicesV136 streams{remaining,read,seek,open_write,write};
 static int open(void* raw,const char* name,std::uintptr_t* handle){auto& f=*static_cast<File*>(raw);check(!std::strcmp(name,"DebugSwitches.savegame"),"exact source filename");++f.opens;f.events.push_back("open-read");*handle=f.absent?0:1;return f.fail_open?1:0;}
 static int close(void* raw,std::uintptr_t handle){auto& f=*static_cast<File*>(raw);if(handle==1){++f.read_closes;f.events.push_back("close-read");return f.fail_read_close?1:0;}check(handle==2,"actual distinct save receiver");++f.write_closes;f.events.push_back("close-write");return f.fail_write_close?1:0;}
 static int remaining(void* raw,std::uintptr_t handle,std::uint64_t* out){auto& f=*static_cast<File*>(raw);check(handle==1&&f.at<=f.input.size(),"same read receiver remaining");*out=f.input.size()-f.at;return 0;}
 static int read(void* raw,std::uintptr_t handle,void* out,std::uint32_t count){auto& f=*static_cast<File*>(raw);check(handle==1&&count<=f.input.size()-f.at,"same exact read bounds");if(f.fail_read)return 1;if(count)std::memcpy(out,f.input.data()+f.at,count);f.at+=count;return 0;}
 static int seek(void* raw,std::uintptr_t handle,std::int64_t delta){auto& f=*static_cast<File*>(raw);check(handle==1&&delta==-4&&f.at>=4,"source invalid magic rewind");f.at-=4;f.events.push_back("rewind");return 0;}
 static int open_write(void* raw,const char* name,std::uintptr_t* handle){auto& f=*static_cast<File*>(raw);check(!std::strcmp(name,"DebugSwitches.savegame"),"exact save filename");f.events.push_back("open-write");f.saves.emplace_back();*handle=2;return 0;}
 static int write(void* raw,std::uintptr_t handle,const void* data,std::uint32_t count){auto& f=*static_cast<File*>(raw);check(handle==2&&!f.saves.empty(),"same save receiver");++f.writes;if(f.fail_write)return 1;const auto* bytes=static_cast<const std::uint8_t*>(data);if(count)f.saves.back().insert(f.saves.back().end(),bytes,bytes+count);return 0;}
 int load(DebugSwitches* s){return dh2_character_debug_load_stream_v136(s,&files,&streams);}
};
void failed_once(File& f,DebugSwitches* s,int expected){
 check(f.load(s)==expected,"expected source failure");const auto events=f.events;std::uint32_t value=99;
 check(f.load(s)==expected&&dh2_character_debug_get(&value,s,"First",&f.files)==expected&&value==99,"failure latches and preserves query output");
 check(f.events==events&&f.opens==1&&f.read_closes==unsigned(!f.absent&&!f.fail_open),"failure does not replay stream prefix");
}
std::uint32_t take_word(const Bytes& bytes,std::size_t& at){check(bytes.size()-at>=4,"saved typed word");auto value=std::uint32_t(bytes[at])|(std::uint32_t(bytes[at+1])<<8)|(std::uint32_t(bytes[at+2])<<16)|(std::uint32_t(bytes[at+3])<<24);at+=4;return value;}
std::map<std::string,std::uint32_t> take_rows(const Bytes& bytes,std::size_t& at){std::map<std::string,std::uint32_t> result;const auto count=take_word(bytes,at);for(std::uint32_t index=0;index<count;++index){const auto length=take_word(bytes,at);check(bytes.size()-at>length,"saved row bytes");std::string name(reinterpret_cast<const char*>(bytes.data()+at),length);at+=length;result[name]=bytes[at++];}return result;}
}
int main(int argc,char** argv){try{
 unsigned golden_operations{};
 if(argc>=2){
  std::ifstream gold(argv[1],std::ios::binary);
  auto source_word=[&](){std::uint32_t value{};gold.read(reinterpret_cast<char*>(&value),4);check(bool(gold),"original absent-file gold word");return value;};
  auto source_text=[&](std::uint32_t count){std::string text(count,'\0');gold.read(text.data(),count);check(bool(gold),"original absent-file gold text");return text;};
  check(source_word()==0x31565344,"original absent-file gold magic");golden_operations=source_word();Owner s{nullptr,dh2_character_debug_destroy};File f;f.absent=true;
  for(std::uint32_t index=0;index<golden_operations;++index){
   const auto reset=source_word(),load=source_word(),length=source_word(),expected_value=source_word(),expected_loaded=source_word(),count=source_word();const auto name=source_text(length);
   std::map<std::string,std::uint32_t> expected;for(std::uint32_t row_index=0;row_index<count;++row_index){const auto size=source_word(),value=source_word();expected[source_text(size)]=value;}const auto calls=source_word();
   if(reset){s=owner();check(bool(s),"fresh golden Debug owner");f.opens=f.read_closes=0;f.events.clear();}
   const auto before=f.opens;std::uint32_t value=0x12345678;
   check(load?f.load(s.get())==1:dh2_character_debug_get(&value,s.get(),name.c_str(),&f.files)==1&&value==expected_value,"original absent-file operation");
   std::uint32_t loaded{},actual_count{};check(dh2_character_debug_snapshot(s.get(),&loaded,&actual_count)==1&&loaded==expected_loaded&&actual_count==count,"original loaded/count gold");
   std::map<std::string,std::uint32_t> actual;for(std::uint32_t row_index=0;row_index<actual_count;++row_index){const char* key{};std::uint32_t result{};check(dh2_character_debug_entry(s.get(),row_index,&key,&result)==1,"gold entry");actual[key]=result;}
   check(actual==expected&&f.opens-before==calls&&!f.read_closes&&!f.writes,"original absent-file map/file-call gold");
  }
  check(gold.peek()==EOF,"complete original absent-file gold");
 }
 for(auto version:{0x10000u,0x20000u}){
  File f;f.input=fixture(version);auto s=owner();check(bool(s)&&f.load(s.get())==1,"supported existing source version");
  const auto map=snapshot(s.get());check(map.at("First")==1&&map.at("False")==0&&map.at("Duplicate")==0&&map.at("IsDisplayLoadingStepName")==1,"parsed switches and normalized bool");
  for(auto name:{"IsDeactivatingFlashMenus","IsDeactivatingFlashMenusUpdate","IsDeactivatingFlashMenusRender"})check(map.at(name)==0,"forced Flash suffix");
  check(map.at("ConnectToAlphaServer")==0&&map.at("ConnectToBetaServer")==0,"source connection queries");
  check(f.opens==1&&f.read_closes==1&&f.saves.size()==f.write_closes&&f.saves.size()>=5,"source changed-value saves close distinct streams");
  auto closed=std::find(f.events.begin(),f.events.end(),"close-read");check(closed!=f.events.end()&&std::find(closed,f.events.end(),"open-write")!=f.events.end(),"forced switch save follows input close");
  std::uint32_t value{};check(dh2_character_debug_get(&value,s.get(),"IsDisplayLoadingStepName",&f.files)==1&&value==1,"active load/get route observes file value");
  if(version>=0x20000){check(dh2_character_debug_module_get_v136(&value,s.get(),"AnimatedFX",&f.files)==1&&value==1,"new source module inserts true");check(dh2_character_debug_module_get_v136(&value,s.get(),"Muted",&f.files)==1&&value==0,"duplicate source module stores false");}
  std::size_t at{};const auto& saved=f.saves.back();check(take_word(saved,at)==0x44425357&&take_word(saved,at)==0x20000,"source save format");const auto modules=take_rows(saved,at);const auto switches=take_rows(saved,at);check(at==saved.size()&&switches.at("IsDisplayLoadingStepName")==1&&switches.at("IsDeactivatingFlashMenus")==0,"saved final prefix switches");if(version>=0x20000)check(modules.at("AnimatedFX")==1&&modules.at("Muted")==0,"same owner module map saved");
  const auto events=f.events;check(f.load(s.get())==1&&f.events==events,"loaded singleton never reopens");
 }
 {File f;f.absent=true;auto s=owner();check(f.load(s.get())==1&&f.opens==1&&!f.read_closes&&!f.writes,"absent-file source branch unchanged");check(snapshot(s.get()).size()==6,"absent source map suffix unchanged");}
 {File f;f.input=fixture(0x10000);f.input[0]=0;auto s=owner();failed_once(f,s.get(),-3);check(f.at==0&&snapshot(s.get()).count("isTracingDebugSwitchesFile"),"invalid magic retains debug and rewind prefix");}
 {File f;f.input=fixture(0x10000);f.input[6]=0;auto s=owner();failed_once(f,s.get(),-3);check(snapshot(s.get()).count("isTracingDebugSwitchesFile"),"invalid version debug prefix");}
 const auto valid=fixture(0x20000);
 for(std::size_t cut=0;cut<valid.size();++cut){File f;f.input.assign(valid.begin(),valid.begin()+cut);auto s=owner();failed_once(f,s.get(),-3);if(cut>70)check(!snapshot(s.get()).empty(),"truncated file retains completed map prefix");}
 {File f;f.input=fixture(0x10000);f.input.resize(12);word(f.input,256);auto s=owner();failed_once(f,s.get(),-3);}
 {File f;f.input=fixture(0x10000);f.fail_read=true;auto s=owner();failed_once(f,s.get(),-2);}
 {File f;f.input=fixture(0x10000);f.fail_read_close=true;auto s=owner();failed_once(f,s.get(),-2);check(snapshot(s.get()).at("First")==1,"close failure retains parsed values");}
 {File f;f.input=fixture(0x10000);f.fail_write_close=true;auto s=owner();failed_once(f,s.get(),-2);check(f.write_closes==1&&snapshot(s.get()).at("First")==1,"save close failure retains changed source store");}
 {File f;f.input=fixture(0x10000);f.fail_write=true;auto s=owner();failed_once(f,s.get(),-2);check(f.write_closes==1,"partial save still closes");}
 {File f;f.input=fixture(0x10000);f.streams.open_write=nullptr;auto s=owner();failed_once(f,s.get(),-3);check(snapshot(s.get()).at("First")==1,"missing reached save leaf retains changed store");}
 {File f;word(f.input,0x44425357);word(f.input,0x20000);word(f.input,0xffffffff);word(f.input,0xffffffff);auto s=owner();check(f.load(s.get())==1&&!f.writes,"source signed nonpositive counts skip rows");}
 {File f;f.input=fixture(0x10000);f.streams.read=[](void*,std::uintptr_t,void*,std::uint32_t)->int{throw std::runtime_error("read delivery");};auto s=owner();failed_once(f,s.get(),-2);}
 // Production stdio read/remaining/rewind/write helper on an actual file.
 {struct Stdio {std::FILE* file=std::tmpfile();unsigned closes{};static int open(void* raw,const char*,std::uintptr_t* out){auto& f=*static_cast<Stdio*>(raw);*out=reinterpret_cast<std::uintptr_t>(f.file);return 0;}static int close(void* raw,std::uintptr_t id){auto& f=*static_cast<Stdio*>(raw);++f.closes;check(id==reinterpret_cast<std::uintptr_t>(f.file),"stdio same receiver");const auto result=std::fclose(f.file);f.file=nullptr;return result;}~Stdio(){if(file)std::fclose(file);}} f;
  check(bool(f.file),"actual scratch stdio stream");Bytes input;word(input,0x44425357);word(input,0x10000);word(input,1);row(input,"False",0);check(std::fwrite(input.data(),1,input.size(),f.file)==input.size()&&!std::fseek(f.file,0,SEEK_SET),"write actual read fixture");DebugFileServices24 files{&f,Stdio::open,Stdio::close};const auto streams=debug_stdio_services_v136(nullptr);auto s=owner();check(dh2_character_debug_load_stream_v136(s.get(),&files,&streams)==1&&f.closes==1,"actual stdio existing-file route");}
 {struct StdioSave {
   std::FILE* input=std::tmpfile();std::vector<std::FILE*> outputs;unsigned closes{};
   static int open(void* raw,const char*,std::uintptr_t* out){auto& f=*static_cast<StdioSave*>(raw);*out=reinterpret_cast<std::uintptr_t>(f.input);return 0;}
   static int open_write(void* raw,const char*,std::uintptr_t* out){auto& f=*static_cast<StdioSave*>(raw);auto* file=std::tmpfile();if(!file)return 1;f.outputs.push_back(file);*out=reinterpret_cast<std::uintptr_t>(file);return 0;}
   static int close(void* raw,std::uintptr_t id){auto& f=*static_cast<StdioSave*>(raw);auto* file=reinterpret_cast<std::FILE*>(id);++f.closes;if(file==f.input)f.input=nullptr;else{const auto at=std::find(f.outputs.begin(),f.outputs.end(),file);check(at!=f.outputs.end(),"same stdio save receiver");f.outputs.erase(at);}return std::fclose(file);}
   ~StdioSave(){if(input)std::fclose(input);for(auto* file:outputs)std::fclose(file);}
  } f;
  check(bool(f.input),"actual positive stdio stream");const auto input=fixture(0x20000);check(std::fwrite(input.data(),1,input.size(),f.input)==input.size()&&!std::fseek(f.input,0,SEEK_SET),"positive stdio bytes");DebugFileServices24 files{&f,StdioSave::open,StdioSave::close};const auto streams=debug_stdio_services_v136(StdioSave::open_write);auto s=owner();check(dh2_character_debug_load_stream_v136(s.get(),&files,&streams)==1&&f.closes>=7&&f.outputs.empty(),"actual stdio parser/save/close route");
 }
 if(argc>=3){
  struct ActualFiles {
   std::string directory;unsigned opens{},closes{};
   static int open(void* raw,const char* name,std::uintptr_t* out){auto& self=*static_cast<ActualFiles*>(raw);++self.opens;*out=reinterpret_cast<std::uintptr_t>(std::fopen((self.directory+"/"+name).c_str(),"rb"));return *out?0:1;}
   static int close(void* raw,std::uintptr_t handle){++static_cast<ActualFiles*>(raw)->closes;return std::fclose(reinterpret_cast<std::FILE*>(handle));}
  } actual{argv[2]};
  const auto path=actual.directory+"/DebugSwitches.savegame";
  for(auto version:{0x10000u,0x20000u}){
   auto* file=std::fopen(path.c_str(),"wb");check(bool(file),"owned real directory fixture");const auto input=fixture(version);check(std::fwrite(input.data(),1,input.size(),file)==input.size()&&!std::fclose(file),"real source fixture bytes");
   const DebugFileServices24 files{&actual,ActualFiles::open,ActualFiles::close};auto s=owner();const auto before=actual.closes;
   check(load_debug_stdio_v136(s.get(),&files,actual.directory.c_str())==1&&actual.closes-before>=7,"production convenience read/save/close route");
   check(snapshot(s.get()).at("IsDisplayLoadingStepName")==1,"production convenience query sees parsed value");
   check(!std::remove(path.c_str()),"owned fixture cleanup");
  }
 }
 std::cout<<"PASS "<<checks<<" existing DebugSwitches source-version/parser/prefix/save/close checks; "<<golden_operations<<" original absent-file operations\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
