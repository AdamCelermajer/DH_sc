#include "character_script_states.hpp"
#include <array>
#include <map>
#include <string>
#include <cstdio>
namespace {
using namespace dh2::character;
bool valid(ScriptStatesRuntime16* s,const ScriptStatesServices16* c){return s&&c&&c->invoke&&!s->reserved;}
int deliver(ScriptStatesRuntime16* s,const ScriptStatesServices16* c,ScriptStatesService kind,const char* text=nullptr,ScriptStateRecord32* record=nullptr,std::uint32_t index=0,ScriptStateRecord32** response=nullptr){ScriptStateRecord32* ignored=nullptr;const ScriptStatesRequest32 request{kind,index,text,record,0};return c->invoke(c->context,s,&request,response?response:&ignored)?-2:1;}
const char* member(const ScriptStateRecord32& r,std::uint32_t i){switch(i){case 0:return r.update;case 1:return r.conditions;case 2:return r.init;default:return r.post;}}
}
extern "C" int dh2_character_script_states_register(ScriptStatesRuntime16* s,const dh2_script_value* arguments,std::uint32_t count,const ScriptStatesServices16* c){
 if(!valid(s,c)||(!arguments&&count))return -1;
 if(count<2||count>5)return 1;
 for(std::uint32_t i=0;i<count;++i){if(arguments[i].reserved)return -1;if(arguments[i].type!=DH2_SCRIPT_STRING)return 1;if(!arguments[i].text)return -1;}
 ScriptStateRecord32* record=nullptr;const auto result=deliver(s,c,states_lookup_insert,arguments[0].text,nullptr,0,&record);if(result<0)return result;if(!record)return -2;
 for(std::uint32_t i=1;i<count;++i){const auto r=deliver(s,c,states_assign,arguments[i].text,record,i-1);if(r<0)return r;}
 return 1;
}
extern "C" int dh2_character_script_states_call(ScriptStatesRuntime16* s,std::uint32_t index,const ScriptStatesServices16* c){
 if(!valid(s,c)||index>3)return -1;
 const auto record=s->current;if(!record)return 1;const auto name=member(*record,index);if(!name)return -1;
 return deliver(s,c,states_lua_call,name);
}
extern "C" int dh2_character_script_states_change(ScriptStatesRuntime16* s,ScriptStateRecord32* target,const ScriptStatesServices16* c){
 if(!valid(s,c))return -1;
 if(!target)return 1;
 if(!target->update||!target->conditions||!target->init||!target->post)return -1;
 const auto result=dh2_character_script_states_call(s,3,c);if(result<0)return result;
 s->current=target;return dh2_character_script_states_call(s,2,c);
}
extern "C" int dh2_character_script_states_update(ScriptStatesRuntime16* s,const ScriptStatesServices16* c){
 if(!valid(s,c))return -1;
 auto result=deliver(s,c,states_default_update);if(result<0)return result;
 if(s->flags&1){result=deliver(s,c,states_lua_call,"OnUpdate");if(result<0)return result;}
 result=dh2_character_script_states_call(s,0,c);if(result<0)return result;
 return dh2_character_script_states_call(s,1,c);
}
namespace dh2::character {
struct ScriptStates::Impl {
 struct Record {ScriptStateRecord32 view{};std::array<std::string,4> names;Record(){refresh();}void refresh(){view={names[0].c_str(),names[1].c_str(),names[2].c_str(),names[3].c_str()};}};
 std::map<std::string,Record,std::less<>> records;ScriptStatesRuntime16 runtime{};const ScriptStatesCalls24* calls=nullptr;
 ScriptStates* self=nullptr;dh2_script_vm* vm=nullptr;const dh2_script_aliases* aliases=nullptr;const dh2_script_callback_scope* scope=nullptr;int vm_status=0;
 static int scoped_call(void* pointer,const char* name){auto& t=*static_cast<Impl*>(pointer);const auto resolved=dh2_script_alias_resolve(t.aliases,name);if(!resolved)return 1;
  // Resolve before callback reentry can modify the owning alias value.
  const std::string captured=resolved;t.vm_status=dh2_script_callback_call_discard_source(t.scope,captured.c_str(),nullptr,0);return t.vm_status<0?1:0;
 }
 static int registration(void* pointer,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t capacity){auto& t=*static_cast<Impl*>(pointer);if(scope->vm!=t.vm||!dh2_script_callback_scope_valid(scope))return 1;const auto result=t.self->register_values(args,count);if(result<0&&capacity)std::snprintf(error,capacity,"native RegisterAIState failure %d",result);return result<0;}
 static int change(void* pointer,const dh2_script_callback_scope* scope,const dh2_script_value* args,std::uint32_t count,char* error,std::size_t capacity){auto& t=*static_cast<Impl*>(pointer);if(scope->vm!=t.vm||!dh2_script_callback_scope_valid(scope))return 1;if(count!=1)return 0;
  const char* name=nullptr;
  if(args&&args[0].type==DH2_SCRIPT_STRING)name=args[0].text;
  else if(args&&args[0].type==DH2_SCRIPT_NIL)name="nil";
  else if(args&&args[0].type==DH2_SCRIPT_BOOLEAN)name=args[0].boolean?"true":"false";
  if(!name){if(capacity)std::snprintf(error,capacity,"native ChangeAIState numeric/identity getString coercion unsupported");return 1;}
  struct Restore {Impl& t;const dh2_script_callback_scope* previous;~Restore(){t.scope=previous;}} restore{t,t.scope};t.scope=scope;
  try{const ScriptStatesCalls24 calls{&t,&scoped_call,nullptr};const auto result=t.self->change_name(name,calls);if(result<0&&capacity)std::snprintf(error,capacity,"native ChangeAIState failure %d",result);return result<0;}catch(const std::bad_alloc&){if(capacity)std::snprintf(error,capacity,"native ChangeAIState allocation failure");return 1;}
 }
 static int dispatch(void* pointer,ScriptStatesRuntime16*,const ScriptStatesRequest32* r,ScriptStateRecord32** out){
  auto& t=*static_cast<Impl*>(pointer);
  switch(r->service){
   case states_lookup_insert:*out=&t.records[r->text].view;return 0;
   case states_assign:for(auto& entry:t.records)if(&entry.second.view==r->record){entry.second.names[r->index]=r->text;entry.second.refresh();return 0;}return 1;
   case states_lua_call:return t.calls&&t.calls->lua_call?t.calls->lua_call(t.calls->context,r->text):1;
   case states_default_update:return t.calls&&t.calls->default_update?t.calls->default_update(t.calls->context):1;
   default:return 1;
  }
 }
 ScriptStatesServices16 services(){return {this,&dispatch};}
};
ScriptStates::ScriptStates():impl_(new Impl){impl_->self=this;}ScriptStates::~ScriptStates()=default;
int ScriptStates::register_values(const dh2_script_value* args,std::uint32_t count){try{const auto c=impl_->services();return dh2_character_script_states_register(&impl_->runtime,args,count,&c);}catch(const std::bad_alloc&){return -2;}}
int ScriptStates::change_name(const char* name,const ScriptStatesCalls24& calls){if(!name||!calls.lua_call)return -1;const auto i=impl_->records.find(name);if(i==impl_->records.end())return 1;
 struct Restore {Impl& t;const ScriptStatesCalls24* old;~Restore(){t.calls=old;}} restore{*impl_,impl_->calls};impl_->calls=&calls;const auto c=impl_->services();return dh2_character_script_states_change(&impl_->runtime,&i->second.view,&c);
}
int ScriptStates::call_current(std::uint32_t index,const ScriptStatesCalls24& calls){if(!calls.lua_call)return -1;struct Restore {Impl& t;const ScriptStatesCalls24* old;~Restore(){t.calls=old;}} restore{*impl_,impl_->calls};impl_->calls=&calls;const auto c=impl_->services();return dh2_character_script_states_call(&impl_->runtime,index,&c);}
int ScriptStates::external_update(const ScriptStatesCalls24& calls){if(!calls.lua_call||!calls.default_update)return -1;struct Restore {Impl& t;const ScriptStatesCalls24* old;~Restore(){t.calls=old;}} restore{*impl_,impl_->calls};impl_->calls=&calls;const auto c=impl_->services();return dh2_character_script_states_update(&impl_->runtime,&c);}
ScriptStatesRuntime16& ScriptStates::runtime() noexcept{return impl_->runtime;}
const char* ScriptStates::current_name() const noexcept{for(const auto& entry:impl_->records)if(&entry.second.view==impl_->runtime.current)return entry.first.c_str();return nullptr;}
bool ScriptStates::find(const char* name,ScriptStateRecord32& out) const noexcept{if(!name)return false;const auto i=impl_->records.find(name);if(i==impl_->records.end())return false;out=i->second.view;return true;}
std::size_t ScriptStates::size() const noexcept{return impl_->records.size();}
int ScriptStates::bind_source(dh2_script_vm* vm,const dh2_script_aliases* aliases){if(!vm||!aliases)return -1;impl_->vm=vm;impl_->aliases=aliases;auto result=dh2_script_vm_bind_source_scoped(vm,"RegisterAIState",&Impl::registration,impl_.get());if(result)return result;return dh2_script_vm_bind_source_scoped(vm,"ChangeAIState",&Impl::change,impl_.get());}
int ScriptStates::last_vm_status() const noexcept{return impl_->vm_status;}
}
