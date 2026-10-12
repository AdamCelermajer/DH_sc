#include "character_clear_aggro.hpp"
#include <cstdio>
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool table(const dh2::data::AggroTable* t){if(!aligned(t)||t->count>t->capacity||t->capacity>1048576||(!aligned(t->entries)&&t->capacity))return false;std::uint64_t previous=0;for(unsigned i=0;i<t->count;++i){const auto& e=t->entries[i];if(!e.character||e.character<=previous||e.reserved)return false;previous=e.character;}return true;}
bool receiver(const ClearAggroState16* s){if(!aligned(s)||!aligned(s->receiver)||s->receiver->reserved||s->receiver->reserved8||!aligned(s->receiver->owner))return false;return !s->receiver->owner->reserved&&!s->receiver->owner->reserved16&&s->receiver->owner->identity;}
bool target(const ClearAggroCharacter32* t){if(!aligned(t)||!t->identity||!aligned(t->target)||t->target->reserved[0]||t->target->reserved[1]||!t->target->services.invoke||!aligned(t->target->state))return false;const auto* s=t->target->state;return !s->reserved&&!s->reserved8&&aligned(s->owner)&&s->owner->identity==t->identity&&!s->owner->reserved&&!s->owner->reserved16;}
}
extern "C" int dh2_character_clear_aggro(ClearAggroState16* s,ClearAggroCharacter32* t,const ClearAggroServices24* svc,const dh2_script_callback_scope* scope){
 if(!aligned(s))return 1;if(!t)return 0;
 if(!receiver(s)||!table(s->outgoing)||!target(t)||!aligned(svc)||!svc->invoke)return 1;
 bool exists=false;for(unsigned i=0;i<s->outgoing->count;++i)if(s->outgoing->entries[i].character==t->identity){exists=true;break;}
 if(exists){
  if(!table(t->incoming))return 1;
  dh2::data::AggroChange change{};dh2::data::AggroRequest request{s->outgoing,t->incoming,s->receiver->owner->identity,t->identity,0,0};
  if(dh2_aggro_apply(&change,&request,dh2::data::aggro_clear))return 1;
  // No source calls occur between erases and this captured owner argument.
  ClearAggroRequest32 notify{clear_aggro_notify,0,t->target->state->identity,s->receiver->owner->identity,scope};
  if(svc->invoke(svc->context,s,t,&notify))return 2;
 }
 if(!receiver(s)||!target(t))return 2;
 if(s->receiver->owner->identity==t->target->state->target){
  // Carry only the supplied private same-VM capability to real setter services.
  auto* binding=t->target;const auto* old=binding->scope;
  struct Restore {TargetBindings48* binding;const dh2_script_callback_scope* old;~Restore(){binding->scope=old;}} restore{binding,old};binding->scope=scope;
  if(dh2_character_ai_set_target(binding->state,0,0,&binding->services))return 2;
  if(!receiver(s)||!target(t))return 2;
  ClearAggroRequest32 stop{clear_aggro_stop,0,t->controller,0,scope};
  if(!stop.receiver||svc->invoke(svc->context,s,t,&stop))return 2;
 }
 return 0;
}
extern "C" int dh2_character_clear_aggro_values(ClearAggroBindings16* b,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count){
 if(!aligned(b)||!aligned(b->state)||!aligned(b->services)||(count&&!aligned(args)))return 1;
 if(!count)return 0;
 if(args[0].reserved)return 1;
 if(args[0].type!=DH2_SCRIPT_IDENTITY&&args[0].type!=DH2_SCRIPT_SOURCE_OBJECT)return 0;
 if(!args[0].identity)return dh2_character_clear_aggro(b->state,nullptr,b->services,scope);
 if(!b->services->resolve)return 2;
 ClearAggroCharacter32* character=nullptr;
 if(b->services->resolve(b->services->context,args[0].identity,&character)||!character||!aligned(character)||character->identity!=args[0].identity)return 2;
 return dh2_character_clear_aggro(b->state,character,b->services,scope);
}
extern "C" int dh2_character_clear_aggro_scoped(void* p,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t size){
 if(!dh2_script_callback_scope_valid(scope)){if(error&&size)std::snprintf(error,size,"Malformed ClearAggro callback capability");return 1;}
 auto status=dh2_character_clear_aggro_values(static_cast<ClearAggroBindings16*>(p),scope,args,count);
 if(status&&error&&size)std::snprintf(error,size,"Native ClearAggro delivery failed (%d)",status);return status?1:0;
}
extern "C" int dh2_character_clear_aggro_bind(dh2_script_vm* vm,ClearAggroBindings16* b){if(!aligned(b)||!aligned(b->state)||!aligned(b->services))return -1;return dh2_script_vm_bind_source_scoped(vm,"ClearAggro",dh2_character_clear_aggro_scoped,b);}
