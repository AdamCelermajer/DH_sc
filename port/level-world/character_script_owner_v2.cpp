#include "character_script_owner_v2.hpp"
#include "character_script_virtual.hpp"
#include "character_script_player_vcb_v2.hpp"
#include <cstring>
#include <map>
#include <set>
#include <utility>
#include <stdexcept>
extern "C" int dh2_script_alias_clear_contents(dh2_script_aliases*);
namespace {struct Failure {int status;};}
namespace dh2::character {
struct ScriptOwnerV2::Impl {
 struct Session {
  std::uint32_t kind;
  std::uint32_t callback_flags=0;
  std::uint32_t executing=0;
  ScriptConstructorFields72 fields{};
  dh2_script_vm* vm=nullptr;
  dh2_script_aliases* aliases=nullptr;
  dh2_script_int_map* integers=nullptr;
  dh2_script_int_bindings integer_receiver{};
  std::string path;
  std::set<std::string> loaded;
  explicit Session(std::uint32_t k,std::size_t limit,
   const ScriptNativeIntegerConfiguration32* native_integers):kind(k){
   if(dh2_character_script_constructor_fields(&fields,k))throw Failure{-1};
   vm=dh2_script_vm_create_empty(limit);if(!vm)throw Failure{-2};
   aliases=dh2_script_alias_create();if(!aliases){dh2_script_vm_destroy(vm);vm=nullptr;throw Failure{-2};}
   if(native_integers){
    integers=dh2_script_int_create();
    if(!integers){
     dh2_script_alias_clear_contents(aliases);dh2_script_vm_destroy(vm);
     dh2_script_alias_destroy(aliases);aliases=nullptr;vm=nullptr;throw Failure{-2};
    }
    integer_receiver={integers,native_integers->context,native_integers->identity,native_integers->format_fraction,0};
   }
  }
  ~Session(){
   loaded.clear();path.clear();
   // Keep the wrapper/context alive through VM finalizers, but clear the two
   // source maps before close, matching LuaScript's container teardown order.
   if(aliases)dh2_script_alias_clear_contents(aliases);
   if(integers)dh2_script_int_clear_contents(integers);
   if(vm)dh2_script_vm_destroy(vm);
   if(aliases)dh2_script_alias_destroy(aliases);
   if(integers)dh2_script_int_destroy(integers);
  }
  ScriptSessionView view() const {
   return {reinterpret_cast<std::uintptr_t>(this),vm,aliases,&fields,path.c_str(),kind,callback_flags,loaded.size()};
  }
 };
 ScriptOwnerV2* self;
 ScriptLifecycleState64 state{};
 std::size_t limit;
 bool native_integer_mode=false;
 ScriptNativeIntegerConfiguration32 native_integer_configuration{};
 std::map<std::uintptr_t,std::shared_ptr<Session>> sessions;
 const ScriptCreationFacts24* facts=nullptr;
 const ScriptOwnerServicesV2* services=nullptr;
 std::uint32_t constructing=script_player_iphone;
 int vm_status=0;
 int source_load_status=0;
 std::string message;
 Impl(ScriptOwnerV2* o,std::uintptr_t character,std::size_t budget,
  const ScriptNativeIntegerConfiguration32* native_integers=nullptr):self(o),limit(budget){
  if(native_integers){
   if(native_integers->reserved)throw std::invalid_argument("Malformed native integer configuration");
   native_integer_mode=true;native_integer_configuration=*native_integers;
  }
  state.owner=character;state.timer33=state.timer34=-1;
 }
 std::shared_ptr<Session> session(std::uintptr_t identity){
  const auto i=sessions.find(identity);if(i==sessions.end())throw Failure{-1};return i->second;
 }
 ScriptOwnerResponse call(std::uint32_t service,std::uint32_t a=0,std::uint32_t b=0,
  std::uintptr_t subject=0,const ScriptSessionView* view=nullptr,const ScriptBinding24* binding=nullptr,const char* file=nullptr){
  ScriptOwnerResponse out{};const ScriptOwnerRequest req{service,a,b,0,subject,view,binding,file};
  if(services->invoke(services->context,*self,req,out))throw Failure{-2};
  if(out.reserved)throw Failure{-1};
  return out;
 }
 void vm_result(int status){
  vm_status=status;if(status){message="VM operation failed";throw Failure{-2};}
 }
 void initial_virtual(std::uintptr_t identity,std::uint32_t slot){
  const auto hold=session(identity);const ScriptTimerCall16 pair{hold->vm,hold->aliases};
  const auto epoch=dh2_script_vm_required_failure_epoch(hold->vm);
  struct Execution {Session& s;Execution(Session& value):s(value){++s.executing;}~Execution(){--s.executing;}} executing{*hold};
  const auto result=dh2_character_script_initial_virtual(&pair,hold->kind,slot);
  if(dh2_script_vm_required_failure_epoch(hold->vm)!=epoch){message="Required native service failed during initial virtual";throw Failure{-2};}
  if(hold->kind!=script_external){if(result)throw Failure{-1};return;}
  vm_status=result;
  if(result==-2){message=dh2_script_vm_error(hold->vm);return;} // Source Call result is ignored.
  if(result)throw Failure{result==-1?-1:-2};
 }
 void bindings(std::uint32_t phase,std::uintptr_t identity,std::uintptr_t character=0){
  const auto hold=session(identity);auto& s=*hold;
  if(phase==1){for(std::uint32_t library=0;library<4;++library)vm_result(dh2_script_vm_open_source_library(s.vm,library));}
  if(phase==2)s.fields.character=character; // Before the original captured Character callback.
  for(std::uint32_t i=0;i<dh2_character_script_binding_count(phase);++i){
   ScriptBinding24 descriptor{};dh2_character_script_binding(&descriptor,phase,i);const auto view=s.view();
   // Original LuaScript Arguments/Binder+8 (script+18) is null. bindMethod
   // returns before installing a Lua closure; preserve this initial-source gate.
   if(descriptor.method)continue;
   if(phase==1&&s.integers&&(i==2||i==3)){
    const bool set=i==2;
    if(descriptor.original_callback!=(set?0x37de5cu:0x37ec14u)||
      std::strcmp(descriptor.name,set?"SetInt":"GetInt"))throw Failure{-1};
    // Preserve exact source prefix and allow genuine provider replacement at
    // this delivery. Never use the bind-both convenience API at index2.
    vm_result(dh2_script_vm_bind_source_values(s.vm,descriptor.name,
     set?dh2_script_int_set_callback:dh2_script_int_get_callback,&s.integer_receiver));
   }
   call(owner_register_binding,phase,i,phase==2?character:identity,&view,&descriptor);
  }
  if(phase==2)s.path="data/scripts/ai/";
 }
 bool load(std::uintptr_t identity,const char* name,const dh2_script_include_scope* scope=nullptr){
  const auto hold=session(identity);auto& s=*hold;
  if(!name)return false;
  std::string requested=s.path+name;
  const char* suffix=std::strstr(name,".lua");
  if(!suffix)requested+=".luac";
  else if(std::strncmp(suffix,".luac",5))requested+='c';
  if(s.loaded.find(requested)!=s.loaded.end())return true;
  const auto view=s.view();const auto file=call(owner_cached_file,0,0,identity,&view,nullptr,requested.c_str());
  if(!file.word)return false; // Source manager missing/false remains a delivered result.
  if(!file.bytes&&file.size)throw Failure{-1};
  struct Execution {Session& s;Execution(Session& value):s(value){++s.executing;}~Execution(){--s.executing;}} executing{s};
  const auto epoch=dh2_script_vm_required_failure_epoch(s.vm);
  source_load_status=scope?dh2_script_include_load(scope,file.bytes,file.size):dh2_script_vm_load_source_file(s.vm,file.bytes,file.size);
  vm_status=source_load_status>0?-2:source_load_status;
  if(dh2_script_vm_required_failure_epoch(s.vm)!=epoch){message="Required native service failed during file loading";throw Failure{-2};}
  if(source_load_status>0){message=scope?dh2_script_include_error(scope):dh2_script_vm_error(s.vm);return false;} // Source AddFile false.
  if(source_load_status<0)throw Failure{-2}; // Negative native diagnostics are not source false.
  s.loaded.insert(requested);
  return true;
 }
 static void selected_construct(void* p,ScriptSelectionState16* selected,std::uint32_t kind){
  auto& t=*static_cast<Impl*>(p);
  if(kind!=script_default&&kind!=script_player&&kind!=script_player_iphone&&kind!=script_external)throw Failure{-2};
  t.state.external_name=selected->external_name;t.state.scripted=selected->scripted;
  struct Restore {std::uint32_t& slot;std::uint32_t old;~Restore(){slot=old;}} restore{t.constructing,t.constructing};
  t.constructing=kind;
  ScriptLifecycleServices16 c{&t,&dispatch};
  if(dh2_character_script_lifecycle(&t.state,script_replace_iphone,0,&c)<0)throw Failure{-1};
  selected->external_name=t.state.external_name;selected->scripted=t.state.scripted;
 }
 static void dispatch(void* p,ScriptLifecycleState64*,const ScriptLifecycleRequest32* r,ScriptLifecycleResponse16* out){
  auto& t=*static_cast<Impl*>(p);
  switch(r->service){
   case script_create_step:{
    ScriptSelectionState16 selected{t.state.external_name,t.state.scripted,0};
    const ScriptSelectionServices16 c{&t,&selected_construct};
    if(dh2_character_script_create_step(&selected,t.facts,&c)<0)throw Failure{-1};
    t.state.external_name=selected.external_name;t.state.scripted=selected.scripted;break;
   }
   case script_ai_terminate:t.call(owner_ai_terminate,0,0,t.state.owner);break;
   case script_destroy:{t.session(r->subject);t.sessions.erase(r->subject);break;}
   case script_construct_iphone:{
    auto s=std::make_shared<Session>(t.constructing,t.limit,t.native_integer_mode?&t.native_integer_configuration:nullptr);const auto identity=reinterpret_cast<std::uintptr_t>(s.get());
    t.sessions.emplace(identity,std::move(s));out->identity=identity;break;
   }
   case script_bind_functions:t.bindings(1,r->subject);break;
   case script_set_character:t.bindings(2,r->subject,r->payload);break;
   case script_load_common:t.load(r->subject,"_commons");break;
   case script_load_external:t.load(r->subject,reinterpret_cast<const char*>(r->payload));break;
   case script_owner_is_character:out->word=t.call(owner_is_character,0,0,r->subject).word;break;
   case script_query_budget:out->word=t.call(owner_budget,r->argument0).word;break;
   case script_owner_is_dead:out->word=t.call(owner_is_dead,0,0,r->subject).word;break;
   case script_design_tick:out->word=t.call(owner_design_tick,r->argument0).word;break;
   case script_timer_stop:t.call(owner_timer_stop,r->argument0,0,r->subject);break;
   case script_timer_start:out->word=t.call(owner_timer_start,r->argument0,r->argument1,r->subject).word;break;
   case script_ai_init:{
    ScriptLifecycleServices16 c{&t,&dispatch};
    if(dh2_character_script_lifecycle(&t.state,script_on_init,0,&c)<0)throw Failure{-1};
    break;
   }
   case script_ais_init:t.initial_virtual(r->subject,8);break;
   case script_ais_init_post:t.initial_virtual(r->subject,12);break;
   case script_ais_init_final:t.initial_virtual(r->subject,16);break;
   case script_pending_init_vcb:{
    const auto s=t.session(r->subject);
    if(s->kind==script_external||s->kind==script_default){
     if(dh2_character_script_init_vcb(&s->callback_flags,s->aliases,s->kind==script_external))throw Failure{-1};
    }else if(s->kind==script_player||s->kind==script_player_iphone){
     if(dh2_character_script_player_vcb_v2(&s->callback_flags,s->aliases))throw Failure{-1};
    }else{const auto view=s->view();t.call(owner_init_vcb,0,0,r->subject,&view);}
    break;
   }
   default:throw Failure{-2};
  }
 }
};
ScriptOwnerV2::ScriptOwnerV2(std::uintptr_t character,std::size_t limit):impl_(new Impl(this,character,limit)){}
ScriptOwnerV2::ScriptOwnerV2(std::uintptr_t character,std::size_t limit,
 const ScriptNativeIntegerConfiguration32& native_integers):impl_(new Impl(this,character,limit,&native_integers)){}
ScriptOwnerV2::~ScriptOwnerV2()=default;
ScriptLifecycleState64& ScriptOwnerV2::lifecycle() noexcept{return impl_->state;}
const ScriptLifecycleState64& ScriptOwnerV2::lifecycle() const noexcept{return impl_->state;}
bool ScriptOwnerV2::find(std::uintptr_t identity,ScriptSessionView& out) const noexcept{
 const auto i=impl_->sessions.find(identity);if(i==impl_->sessions.end())return false;out=i->second->view();return true;
}
bool ScriptOwnerV2::pending(ScriptSessionView& out) const noexcept{return find(impl_->state.pending,out);}
bool ScriptOwnerV2::active(ScriptSessionView& out) const noexcept{return find(impl_->state.active,out);}
bool ScriptOwnerV2::integer_bindings(std::uintptr_t identity,const dh2_script_int_bindings*& out) const noexcept{
 out=nullptr;const auto i=impl_->sessions.find(identity);
 if(i==impl_->sessions.end()||!i->second->integers)return false;
 out=&i->second->integer_receiver;return true;
}
int ScriptOwnerV2::last_vm_status() const noexcept{return impl_->vm_status;}
int ScriptOwnerV2::last_source_load_status() const noexcept{return impl_->source_load_status;}
const std::string& ScriptOwnerV2::error() const noexcept{return impl_->message;}
int ScriptOwnerV2::copy_path(std::uintptr_t identity,std::string& out)const noexcept{
 const auto found=impl_->sessions.find(identity);if(found==impl_->sessions.end())return -1;
 try{std::string value=found->second->path;out.swap(value);return 0;}catch(...){return -2;}
}
int ScriptOwnerV2::assign_path(std::uintptr_t identity,const char* bytes,std::size_t size)noexcept{
 const auto found=impl_->sessions.find(identity);if(found==impl_->sessions.end()||(!bytes&&size))return -1;
 try{std::string value(bytes?bytes:"",size);found->second->path.swap(value);return 0;}catch(...){return -2;}
}
int ScriptOwnerV2::load_file(std::uintptr_t identity,const char* name,
 const ScriptOwnerServicesV2& services,bool& loaded){
 auto& t=*impl_;const auto found=t.sessions.find(identity);
 if(!services.invoke||found==t.sessions.end()||found->second->executing||dh2_script_vm_stack_size(found->second->vm)<0)return -1;
 const auto hold=found->second;const auto epoch=dh2_script_vm_required_failure_epoch(hold->vm);
 struct Restore{Impl& impl;const ScriptOwnerServicesV2* old;~Restore(){impl.services=old;}} restore{t,t.services};
 t.services=&services;t.vm_status=0;t.source_load_status=0;t.message.clear();
 try{
  const bool value=t.load(identity,name);
  if(dh2_script_vm_required_failure_epoch(hold->vm)!=epoch){t.message="Required native service failed during source file load";return -2;}
  loaded=value;return 0;
 }catch(const Failure& failure){return failure.status;}
 catch(const std::exception& exception){t.message=exception.what();return -2;}
}
int ScriptOwnerV2::call_discard(std::uintptr_t identity,const char* name,
 const dh2_script_value* arguments,std::uint32_t count,std::uint32_t& source_error){
 auto& t=*impl_;const auto found=t.sessions.find(identity);
 if(!name||found==t.sessions.end()||found->second->executing||dh2_script_vm_stack_size(found->second->vm)<0)return -1;
 const auto hold=found->second;const auto resolved=dh2_script_alias_resolve(hold->aliases,name);
 struct Execution{Impl::Session& session;Execution(Impl::Session& value):session(value){++session.executing;}~Execution(){--session.executing;}} executing{*hold};
 const int status=dh2_script_vm_call_source_status_objects(hold->vm,resolved,arguments,count);
 t.vm_status=status>0?-2:status;t.message=dh2_script_vm_error(hold->vm);
 if(status<0)return status==-1?-1:-2;
 source_error=static_cast<std::uint32_t>(status);return 0;
}
int ScriptOwnerV2::init_vcb(std::uintptr_t identity,const ScriptOwnerServicesV2& services){
 auto& t=*impl_;const auto found=t.sessions.find(identity);if(found==t.sessions.end()||!services.invoke)return -1;
 const auto hold=found->second;
 try{
  if(hold->kind==script_external||hold->kind==script_default)
   return dh2_character_script_init_vcb(&hold->callback_flags,hold->aliases,hold->kind==script_external)?-1:0;
  if(hold->kind==script_player||hold->kind==script_player_iphone)
   return dh2_character_script_player_vcb_v2(&hold->callback_flags,hold->aliases);
  struct Restore{Impl& impl;const ScriptOwnerServicesV2* old;~Restore(){impl.services=old;}} restore{t,t.services};
  t.services=&services;const auto view=hold->view();t.call(owner_init_vcb,0,0,identity,&view);return 0;
 }catch(const Failure& failure){return failure.status;}
 catch(const std::exception& exception){t.message=exception.what();return -2;}
}
int ScriptOwnerV2::advance(const ScriptCreationFacts24& facts,const ScriptOwnerServicesV2& services){
 auto& t=*impl_;
 if(!services.invoke||!t.state.owner||facts.reserved||t.state.load_step<0||t.state.delayed>255||t.state.scripted>255||
  t.state.reserved0||t.state.reserved1||t.state.reserved2||t.limit<65536||t.limit>1073741824)return -1;
 if(t.state.load_step<=6)for(const auto& entry:t.sessions)
  if(entry.second->executing||dh2_script_vm_stack_size(entry.second->vm)<0)return -2;
 struct Restore {
  Impl& impl;const ScriptCreationFacts24* facts;const ScriptOwnerServicesV2* services;
  ~Restore(){impl.facts=facts;impl.services=services;}
 } restore{t,t.facts,t.services};
 t.facts=&facts;t.services=&services;t.vm_status=0;t.message.clear();
 try{
  const ScriptLifecycleServices16 c{&t,&Impl::dispatch};
  if(dh2_character_script_lifecycle(&t.state,script_load_process,0,&c)<0)throw Failure{-1};
  return 0;
 }catch(const Failure& f){return f.status;}
 catch(const std::bad_alloc&){t.message="Native ownership allocation failed";return -2;}
}
int ScriptOwnerV2::include(std::uintptr_t identity,const char* name,
 const ScriptOwnerServicesV2& services,const dh2_script_include_scope* scope){
 auto& t=*impl_;
 const auto found=t.sessions.find(identity);
 if(!name||!services.invoke||found==t.sessions.end()||!scope||scope->vm!=found->second->vm||!dh2_script_include_scope_valid(scope))return -1;
 const auto hold=found->second;
 struct Restore {Impl& impl;const ScriptOwnerServicesV2* old;~Restore(){impl.services=old;}} restore{t,t.services};
 struct Execution {Impl::Session& s;Execution(Impl::Session& value):s(value){++s.executing;}~Execution(){--s.executing;}} executing{*hold};
 t.services=&services;
 try{t.load(identity,name,scope);return 0;}
 catch(const Failure& f){return f.status;}
 catch(const std::bad_alloc&){t.message="Native Include ownership allocation failed";return -2;}
}
}
