#include "character_design_services.hpp"
#include <map>
#include <string>
#include <cstring>
#include <new>
#include <array>
#include <limits>
namespace dh2::character {
struct DebugSwitches {std::map<std::string,std::uint8_t> switches,modules;bool loaded=false;int failure=0;};
}
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool files(const DebugFileServices24* f){return aligned(f,alignof(DebugFileServices24))&&f->open_read&&f->close_read;}
int load(DebugSwitches*,const DebugFileServices24*,const DebugExistingFileServicesV136* = nullptr);
int get(DebugSwitches* s,const char* name,const DebugFileServices24* f,std::uint32_t& value){
 if(s->failure)return s->failure;
 auto found=s->switches.find(name);
 if(found==s->switches.end()){
  s->switches[name]=0;
  auto status=load(s,f);if(status<0)return status;
  std::uint32_t ignored=0;status=get(s,"isTracingDebugSwitches",f,ignored);if(status<0)return status;
 }
 // Source rereads after the recursive load/query. No boolean normalization.
 value=s->switches[name];return 1;
}
// 337b4c writes the current live maps after every changed SetSwitch/SetModule.
// Do not stage all input rows and publish a replacement map after parsing.
int save(DebugSwitches* s,const DebugFileServices24* f,const DebugExistingFileServicesV136* io){
 if(!io||!io->open_write||!io->write)return -3;
 std::uintptr_t handle{};
 if(io->open_write(f->context,"DebugSwitches.savegame",&handle))return -2;
 if(!handle)return 1; // Genuine source FileManager open-write NULL branch.
 int status=1;
 auto bytes=[&](const void* data,std::uint32_t count){if(status==1&&io->write(f->context,handle,data,count))status=-2;};
 auto word=[&](std::uint32_t value){const std::uint8_t raw[]{std::uint8_t(value),std::uint8_t(value>>8),std::uint8_t(value>>16),std::uint8_t(value>>24)};bytes(raw,4);};
 auto text=[&](const std::string& name){if(name.size()>std::numeric_limits<std::uint32_t>::max()){status=-3;return;}word(static_cast<std::uint32_t>(name.size()));bytes(name.data(),static_cast<std::uint32_t>(name.size()));};
 try{
  word(0x44425357);word(0x20000);word(static_cast<std::uint32_t>(s->modules.size()));
  for(const auto& row:s->modules){text(row.first);bytes(&row.second,1);if(status!=1)break;}
  if(status==1){word(static_cast<std::uint32_t>(s->switches.size()));
   for(auto at=s->switches.begin();at!=s->switches.end()&&status==1;++at){
    std::uint32_t ignored{};status=get(s,"isTracingDebugSwitchesFile",f,ignored);
    if(status==1){text(at->first);bytes(&at->second,1);}
   }
  }
 }catch(...){status=-2;}
 // Close is reached even after a partial write/provider exception.
 try{if(f->close_read(f->context,handle))status=-2;}catch(...){status=-2;}
 return status;
}
int set_switch(DebugSwitches* s,const char* name,std::uint8_t value,const DebugFileServices24* f,const DebugExistingFileServicesV136* io){
 auto found=s->switches.find(name);
 if(found==s->switches.end()){
  const int status=load(s,f);if(status<0)return status;
  std::uint32_t ignored{};const int query=get(s,"isTracingDebugSwitches",f,ignored);if(query<0)return query;
  s->switches[name]=0;
 }
 auto& actual=s->switches[name];if(actual==value)return 1;
 actual=value;return save(s,f,io);
}
int set_module(DebugSwitches* s,const char* name,std::uint8_t value,const DebugFileServices24* f,const DebugExistingFileServicesV136* io){
 auto found=s->modules.find(name);
 if(found==s->modules.end()){
  const int status=load(s,f);if(status<0)return status;
  std::uint32_t ignored{};const int query=get(s,"isTracingDebugSwitches",f,ignored);if(query<0)return query;
  s->modules[name]=1;return 1; // Exact SetModule337404 miss ignores input bool.
 }
 if(found->second==value)return 1;
 found->second=value;return save(s,f,io);
}
int parse(DebugSwitches* s,const DebugFileServices24* f,const DebugExistingFileServicesV136* io,std::uintptr_t handle){
 if(!io||!io->remaining||!io->read||!io->seek_relative)return -3;
 std::uint64_t left{};if(io->remaining(f->context,handle,&left))return -2;
 // Source337548..5f0 skips <=11 bytes. Native rejects that unusable prefix,
 // and rejects short typed reads instead of executing source assertions.
 if(left<=11)return -3;
 int status=1;
 auto bytes=[&](void* data,std::uint32_t count){if(status!=1)return;if(left<count){status=-3;return;}if(io->read(f->context,handle,data,count)){status=-2;return;}left-=count;};
 auto word=[&](){std::uint8_t raw[4]{};bytes(raw,4);return std::uint32_t(raw[0])|(std::uint32_t(raw[1])<<8)|(std::uint32_t(raw[2])<<16)|(std::uint32_t(raw[3])<<24);};
 auto signed_word=[&](){const auto raw=word();std::int32_t value{};std::memcpy(&value,&raw,4);return value;};
 const auto magic=word();if(status!=1)return status;
 if(magic!=0x44425357){
  std::uint32_t ignored{};const int query=get(s,"isTracingDebugSwitchesFile",f,ignored);if(query<0)return query;
  if(io->seek_relative(f->context,handle,-4))return -2;
  return -3; // Preserve source rewind/debug prefix; reject invalid file.
 }
 const auto version=signed_word();if(status!=1)return status;
 if(version<0x10000){std::uint32_t ignored{};const int query=get(s,"isTracingDebugSwitchesFile",f,ignored);return query<0?query:-3;}
 auto rows=[&](std::int32_t count,bool modules){
  for(std::int32_t index=0;index<count&&status==1;++index){
   const auto length=word();if(status!=1)break;
   // readString317734 supplies a256-byte CString. Reject the oversized case
   // rather than consume a truncated name and treat its next byte as bool.
   if(length>255){status=-3;break;}
   std::array<char,256> name{};bytes(name.data(),length);std::uint8_t value{};bytes(&value,1);if(status!=1)break;
   value=value?1:0;
   if(modules)status=set_module(s,name.data(),value,f,io);
   else{std::uint32_t ignored{};status=get(s,"isTracingDebugSwitchesFile",f,ignored);if(status==1)status=set_switch(s,name.data(),value,f,io);}
  }
 };
 if(version>=0x20000){const auto count=signed_word();if(status==1)rows(count,true);}
 if(status==1){const auto count=signed_word();if(status==1)rows(count,false);}
 return status;
}
int load(DebugSwitches* s,const DebugFileServices24* f,const DebugExistingFileServicesV136* io){
 if(s->failure)return s->failure;
 if(s->loaded)return 1;
 s->loaded=true;
 std::uintptr_t handle=0;
 if(f->open_read(f->context,"DebugSwitches.savegame",&handle))return s->failure=-2;
 if(handle){
  int status{};try{status=parse(s,f,io,handle);}catch(...){status=-2;}
  try{if(f->close_read(f->context,handle))status=-2;}catch(...){status=-2;}
  if(status!=1)return s->failure=status;
 }
 // Original recursive load calls see loaded1, so the forced false writes and
 // missing queries execute without another file attempt.
 for(auto name:{"IsDeactivatingFlashMenus","IsDeactivatingFlashMenusUpdate","IsDeactivatingFlashMenusRender"}){
  auto status=load(s,f);if(status<0)return status;
  auto found=s->switches.find(name);
  if(found==s->switches.end()){
   s->switches[name]=0;std::uint32_t ignored=0;
   status=get(s,"isTracingDebugSwitches",f,ignored);if(status<0)return status;
  }
  const auto set=set_switch(s,name,0,f,io);if(set<0)return s->failure=set;
 }
 for(auto name:{"ConnectToAlphaServer","ConnectToBetaServer"}){
  auto status=load(s,f);if(status<0)return status;
  std::uint32_t ignored=0;status=get(s,name,f,ignored);if(status<0)return status;
 }
 return 1;
}
}
extern "C" DebugSwitches* dh2_character_debug_create(){try{return new DebugSwitches;}catch(...){return nullptr;}}
extern "C" void dh2_character_debug_destroy(DebugSwitches* s){delete s;}
extern "C" int dh2_character_debug_load(DebugSwitches* s,const DebugFileServices24* f){if(!s||!files(f))return -1;try{return load(s,f);}catch(...){return s->failure=-2;}}
extern "C" int dh2_character_debug_load_stream_v136(DebugSwitches* s,const DebugFileServices24* f,const DebugExistingFileServicesV136* io){if(!s||!files(f)||!io)return -1;try{return load(s,f,io);}catch(...){return s->failure=-2;}}
extern "C" int dh2_character_debug_get(std::uint32_t* value,DebugSwitches* s,const char* name,const DebugFileServices24* f){
 if(!aligned(value,alignof(std::uint32_t))||!s||!name||!files(f))return -1;
 try{std::uint32_t result=0;auto status=get(s,name,f,result);if(status>0)*value=result;return status;}catch(...){return s->failure=-2;}
}
extern "C" int dh2_character_debug_set_v102(DebugSwitches* s,const char* name,std::uint8_t value,const DebugFileServices24* f){
 if(!s||!name||!files(f)||value>1)return -1;
 try{if(s->failure)return s->failure;auto found=s->switches.find(name);
  if(found==s->switches.end()){const int status=load(s,f);if(status<0)return status;std::uint32_t trace{};const int query=get(s,"isTracingDebugSwitches",f,trace);if(query<0)return query;s->switches[name]=0;}
  auto& actual=s->switches[name];if(actual==value)return 1;actual=value;return s->failure=-3;
 }catch(...){return s->failure=-2;}
}
extern "C" int dh2_character_debug_snapshot(const DebugSwitches* s,std::uint32_t* loaded,std::uint32_t* count){if(!s||!aligned(loaded,4)||!aligned(count,4)||loaded==count)return -1;*loaded=s->loaded;*count=static_cast<std::uint32_t>(s->switches.size());return 1;}
extern "C" int dh2_character_debug_entry(const DebugSwitches* s,std::uint32_t index,const char** name,std::uint32_t* value){
 if(!s||!aligned(name,alignof(const char*))||!aligned(value,4)||index>=s->switches.size())return -1;
 auto at=s->switches.begin();while(index--)++at;*name=at->first.c_str();*value=at->second;return 1;
}
extern "C" int dh2_character_debug_module_get_v136(std::uint32_t* value,DebugSwitches* s,const char* name,const DebugFileServices24* f){
 if(!aligned(value,4)||!s||!name||!files(f))return -1;
 try{if(s->failure)return s->failure;auto found=s->modules.find(name);if(found!=s->modules.end()){*value=found->second;return 1;}
  const auto status=load(s,f);if(status<0)return status;std::uint32_t ignored{};const auto query=get(s,"isTracingDebugSwitches",f,ignored);if(query<0)return query;
  s->modules[name]=1;*value=1;return 1;
 }catch(...){return s->failure=-2;}
}
extern "C" int dh2_character_debug_level_service(void* context,LevelModel32* model,const LevelRequest24* request){
 auto* b=static_cast<DebugLevelBinding16*>(context);
 if(!aligned(b,alignof(DebugLevelBinding16))||!b->owner||!files(b->files)||!model||!request||request->reserved||(request->property!=36&&request->property!=41)||request->retained_delta<=0)return 1;
 if(request->service==level_debug_load)return request->name||dh2_character_debug_load(b->owner,b->files)<0?1:0;
 if(request->service==level_debug_query){std::uint32_t ignored=0;return !request->name||std::strcmp(request->name,"isTracingChar_Stats")||dh2_character_debug_get(&ignored,b->owner,request->name,b->files)<0?1:0;}
 return 1;
}
extern "C" int dh2_character_design_tick(std::uint32_t* out,const dh2_script_design_bindings* design,std::uint32_t event){
 if(!aligned(out,4)||!aligned(design,alignof(dh2_script_design_bindings))||!design->lookup||design->reserved||(event!=0x33&&event!=0x34))return -1;
 std::int32_t value=0;if(design->lookup(design->context,0,"CharacterDesign",event==0x33?"AI_Tick":"DoT_Tick",&value))return -2;
 std::memcpy(out,&value,4);return 1;
}
