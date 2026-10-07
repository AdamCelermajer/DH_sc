#include "character_design_services.hpp"
#include <map>
#include <string>
#include <cstring>
#include <new>
namespace dh2::character {
struct DebugSwitches {std::map<std::string,std::uint8_t> switches;bool loaded=false;int failure=0;};
}
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool files(const DebugFileServices24* f){return aligned(f,alignof(DebugFileServices24))&&f->open_read&&f->close_read;}
int load(DebugSwitches*,const DebugFileServices24*);
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
int load(DebugSwitches* s,const DebugFileServices24* f){
 if(s->failure)return s->failure;
 if(s->loaded)return 1;
 s->loaded=true;
 std::uintptr_t handle=0;
 if(f->open_read(f->context,"DebugSwitches.savegame",&handle))return s->failure=-2;
 if(handle){const auto closed=f->close_read(f->context,handle);return s->failure=closed?-2:-3;}
 // Original recursive load calls see loaded1, so the forced false writes and
 // missing queries execute without another file attempt.
 for(auto name:{"IsDeactivatingFlashMenus","IsDeactivatingFlashMenusUpdate","IsDeactivatingFlashMenusRender"}){
  auto status=load(s,f);if(status<0)return status;
  auto found=s->switches.find(name);
  if(found==s->switches.end()){
   s->switches[name]=0;std::uint32_t ignored=0;
   status=get(s,"isTracingDebugSwitches",f,ignored);if(status<0)return status;
  }
  // Only source empty/missing-file values exist in this bounded owner domain;
  // changing a true value would require source save and is not fabricated.
  if(s->switches[name])return s->failure=-3;
  s->switches[name]=0;
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
