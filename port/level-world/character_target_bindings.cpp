#include "character_target_bindings.hpp"
#include <cstdio>
#include <cstring>
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool owner_valid(const TargetOwner16* o){return aligned(o)&&!o->reserved16&&!o->reserved;}
bool valid(const TargetState48* s){return aligned(s)&&!s->reserved8&&!s->reserved&&(!s->owner||owner_valid(s->owner));}
bool bindings_valid(const TargetBindings48* b){return aligned(b)&&valid(b->state)&&!b->reserved[0]&&!b->reserved[1];}
int call(TargetState48* s,const TargetServices16* c,std::uint32_t service,std::uintptr_t subject,std::uint32_t& out,const char* name=nullptr){
 if(!c||!c->invoke)return 2;
 const TargetRequest24 request{service,0,subject,name};out=0;
 return c->invoke(c->context,s,&request,&out)?2:0;
}
int fail(char* e,std::size_t n,const char* text){if(e&&n)std::snprintf(e,n,"%s",text);return 1;}
int scoped(void* p,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t ec,bool clear){
 auto* b=static_cast<TargetBindings48*>(p);
 if(!bindings_valid(b)||!dh2_script_callback_scope_valid(scope))return fail(error,ec,"Malformed target callback capability");
 struct Restore {TargetBindings48* b;const dh2_script_callback_scope* old;~Restore(){b->scope=old;}} restore{b,b->scope};b->scope=scope;
 const int result=clear?dh2_character_clear_target(b->state,&b->services):dh2_character_target_set_values(b,args,count);
 if(result){if(error&&ec)std::snprintf(error,ec,"Native target provider failure %d",result);return 1;}return 0;
}
int set_callback(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t ec){return scoped(p,s,a,n,e,ec,false);}
int clear_callback(void* p,const dh2_script_callback_scope* s,const dh2_script_value* a,std::uint32_t n,char* e,std::size_t ec){return scoped(p,s,a,n,e,ec,true);}
}
extern "C" int dh2_character_ai_set_target(TargetState48* s,std::uintptr_t incoming,std::uint32_t mode,const TargetServices16* c){
 if(!valid(s)||(c&&!aligned(c)))return 1;
 s->candidate=incoming;
 if(mode){s->target=incoming;return 0;}
 if(s->target!=incoming){if(!owner_valid(s->owner))return 2;s->owner->word14d0=0;}
 std::uint32_t tracing=0,unused=0;
 if(call(s,c,target_debug_load,0,unused)||call(s,c,target_debug_query,0,tracing,"IsTracingCharAITarget"))return 2;
 if(tracing&&s->target!=incoming){
  if(call(s,c,target_debug_load,0,unused)||call(s,c,target_debug_query,0,unused,"isTracingCharAITarget"))return 2;
 }
 s->target=incoming;
 if(!incoming)return 0;
 if(!owner_valid(s->owner))return 2;
 if(call(s,c,target_owner_ai_id,s->owner->identity,unused))return 2; // Discarded GetCharAIId, source still executes it.
 const auto current=s->target;
 if(current!=s->last_target){s->changed=0;s->last_target=current;}
 std::uint32_t dead=0;
 if(call(s,c,target_virtual_dead,current,dead))return 2;
 s->alive=static_cast<std::uint8_t>(dead^1u);
 std::uint32_t sight=0;
 if(call(s,c,target_in_sight,s->target,sight))return 2;
 s->sight=static_cast<std::uint8_t>(sight);
 return 0;
}
extern "C" int dh2_character_ai_sync_last_target(TargetState48* s){if(!valid(s))return 1;s->last_target=s->target;return 0;}
extern "C" int dh2_character_clear_target(TargetState48* s,const TargetServices16* c){const int r=dh2_character_ai_set_target(s,0,0,c);return r?r:dh2_character_ai_sync_last_target(s);}
extern "C" int dh2_character_has_target(std::uint32_t* out,const TargetState48* s){if(!aligned(out)||!valid(s))return 1;*out=s->target!=0;return 0;}
extern "C" int dh2_character_target_identity(std::uintptr_t* out,const TargetState48* s){if(!aligned(out)||!valid(s))return 1;*out=s->target;return 0;}
extern "C" int dh2_character_target_ai_id(std::int32_t* out,const std::int32_t* resolved,std::uint32_t count){if(!aligned(out)||!aligned(resolved)||count>65536)return 1;const auto id=resolved[1];*out=id>=0&&std::uint32_t(id)<count?id:8;return 0;}
extern "C" int dh2_character_target_sight(std::uint32_t* out,const float* first,const float* second,float radius){
 if(!aligned(out)||!aligned(first)||!aligned(second))return 1;
 const float x=first[0]-second[0],y=first[1]-second[1],z=first[2]-second[2];
 const float xx=x*x,yy=y*y,zz=z*z;*out=radius*radius>((xx+yy)+zz);return 0;
}
extern "C" int dh2_character_target_has_lua(void* p,const dh2_script_value* a,std::uint32_t count,dh2_script_value* out,std::uint32_t capacity,std::uint32_t* returned,char* e,std::size_t ec){
 if(!aligned(returned))return fail(e,ec,"Malformed target return count");
 *returned=0;
 const auto* b=static_cast<TargetBindings48*>(p);
 if((count&&!a)||!bindings_valid(b)||!aligned(out)||!capacity)return fail(e,ec,"Malformed HasTarget receiver/output");
 std::uint32_t result=0;if(dh2_character_has_target(&result,b->state))return fail(e,ec,"Malformed HasTarget state");
 std::memset(out,0,sizeof(*out));out->type=DH2_SCRIPT_BOOLEAN;out->boolean=result;*returned=1;return 0;
}
extern "C" int dh2_character_target_set_values(TargetBindings48* b,const dh2_script_value* a,std::uint32_t count){
 if((count&&!aligned(a))||!bindings_valid(b))return 1;
 if(!count||(a[0].type!=2&&a[0].type!=7))return 0;
 if(a[0].reserved)return 1;
 return dh2_character_ai_set_target(b->state,a[0].identity,0,&b->services);
}
extern "C" int dh2_character_target_bind(dh2_script_vm* vm,TargetBindings48* b){
 if(!vm||!bindings_valid(b)||b->scope||!b->services.invoke)return -1;
 int r=dh2_script_vm_bind_source_values(vm,"HasTarget",dh2_character_target_has_lua,b);if(r)return r;
 r=dh2_script_vm_bind_source_scoped(vm,"SetTarget",set_callback,b);if(r)return r;
 return dh2_script_vm_bind_source_scoped(vm,"ClearTarget",clear_callback,b);
}
