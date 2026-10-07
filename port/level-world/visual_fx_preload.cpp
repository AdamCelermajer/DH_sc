#include "visual_fx_preload.hpp"
#include <map>
#include <new>
#include <string>
namespace dh2::fx {
struct DebugModules {character::DebugSwitches* owner;const character::DebugFileServices24* files;std::map<std::string,std::uint32_t> values;};
namespace {
bool valid(const PreloadTable16* t,const PreloadQueue16* q,const PreloadServices16* s){
 if(!t||!q||!s||!s->call||t->set_count>4096||t->effect_count>4096||(t->set_count&&!t->sets)||q->count>q->capacity||(q->capacity&&!q->ids))return false;
 for(std::uint32_t i=0;i<t->set_count;++i)if(t->sets[i].reserved||t->sets[i].count>4096||(t->sets[i].count>0&&!t->sets[i].steps))return false;
 return true;
}
int gate(const PreloadServices16* s){std::uint32_t value=0;if(s->call(s->context,debug_load,nullptr,&value))return -2;if(s->call(s->context,debug_module,"AnimatedFX",&value))return -2;return value?1:0;}
int effect(const PreloadTable16* t,PreloadQueue16* q,std::int32_t id,const PreloadServices16* s){
 int status=gate(s);if(status<0)return status;if(!status||id<0||static_cast<std::uint32_t>(id)>=t->effect_count)return 1;
 std::uint32_t value=0;if(s->call(s->context,debug_load,nullptr,&value)||s->call(s->context,debug_switch,"isTracingPreCached_FX",&value))return -2;
 for(std::uint32_t i=0;i<q->count;++i)if(q->ids[i]==id)return 1;
 if(q->count>=q->capacity)return -3;q->ids[q->count++]=id;return 1;
}
int set(const PreloadTable16* t,PreloadQueue16* q,std::int32_t id,const PreloadServices16* s,std::uint32_t depth){
 if(depth>=128)return -3;
 int status=gate(s);if(status<0)return status;if(!status||id<0||static_cast<std::uint32_t>(id)>=t->set_count)return 1;
 const PreloadSet16* row=t->sets+id;
 for(std::int32_t i=0;i<row->count;++i){const auto step=row->steps[i];status=step.type==1?set(t,q,step.effect_id,s,depth+1):effect(t,q,step.effect_id,s);if(status!=1)return status;}
 return 1;
}
bool name_ok(const char* name){if(!name)return false;for(std::size_t i=0;i<4096;++i)if(!name[i])return true;return false;}
}
}
extern "C" int dh2_fx_register_set(const dh2::fx::PreloadTable16* t,dh2::fx::PreloadQueue16* q,std::int32_t id,const dh2::fx::PreloadServices16* s){return dh2::fx::valid(t,q,s)?dh2::fx::set(t,q,id,s,0):-1;}
extern "C" int dh2_fx_register_effect(const dh2::fx::PreloadTable16* t,dh2::fx::PreloadQueue16* q,std::int32_t id,const dh2::fx::PreloadServices16* s){return dh2::fx::valid(t,q,s)?dh2::fx::effect(t,q,id,s):-1;}
extern "C" dh2::fx::DebugModules* dh2_fx_debug_modules_create(dh2::character::DebugSwitches* o,const dh2::character::DebugFileServices24* f){if(!o||!f||!f->open_read||!f->close_read)return nullptr;try{return new dh2::fx::DebugModules{o,f,{}};}catch(...){return nullptr;}}
extern "C" void dh2_fx_debug_modules_destroy(dh2::fx::DebugModules* o){delete o;}
extern "C" int dh2_fx_debug_module_get(std::uint32_t* result,dh2::fx::DebugModules* o,const char* name){
 if(!result||!o||!dh2::fx::name_ok(name))return -1;
 try{
  auto found=o->values.find(name);if(found!=o->values.end()){*result=found->second;return 1;}
  int status=dh2_character_debug_load(o->owner,o->files);if(status!=1)return status;
  std::uint32_t ignored=0;status=dh2_character_debug_get(&ignored,o->owner,"isTracingDebugSwitches",o->files);if(status!=1)return status;
  o->values[name]=1;*result=1;return 1;
 }catch(...){return -2;}
}
extern "C" int dh2_fx_debug_preload_service(void* p,std::uint32_t op,const char* name,std::uint32_t* result){
 auto* o=static_cast<dh2::fx::DebugModules*>(p);if(!o||!result)return -1;int status=-1;
 if(op==dh2::fx::debug_load){if(name)return -1;status=dh2_character_debug_load(o->owner,o->files);}
 else if(op==dh2::fx::debug_module)status=dh2_fx_debug_module_get(result,o,name);
 else if(op==dh2::fx::debug_switch)status=dh2_character_debug_get(result,o->owner,name,o->files);
 return status==1?0:status;
}
