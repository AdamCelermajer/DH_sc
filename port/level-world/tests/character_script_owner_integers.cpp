// New native-mode audit. Historical provider-only owner proof stays intact.
#define main historical_owner_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include <cstdio>
namespace {
void require(bool ok,unsigned line){++checks;if(!ok)throw std::runtime_error("integer owner line "+std::to_string(line)+" check "+std::to_string(checks));}
}
#define check(value) require((value),__LINE__)
namespace {
struct NativeFixture:Fixture {
 struct Callback {NativeFixture* fixture;uintptr_t identity;const dh2_script_int_bindings* integers;dh2_script_vm* vm;dh2_script_aliases* aliases;};
 std::map<uintptr_t,Callback> contexts;
 std::map<uintptr_t,unsigned> set_deliveries;
 std::map<std::string,std::string> resources;
 ScriptOwnerServices persistent{this,&service};
 unsigned prefix_observations=0,identity_calls=0,format_calls=0,scope_calls=0,include_calls=0,busy_guards=0,close_observations=0,after_close_observations=0,override_calls=0;
 bool override_set=false;bool no_services=false;
 explicit NativeFixture(const std::string& common,bool missing=false):Fixture(common),no_services(missing){
  const ScriptNativeIntegerConfiguration32 source{this,missing?nullptr:&identity,missing?nullptr:&fraction,0};
  owner.reset(new ScriptOwner(store.owner,8*1024*1024,source));
 }
 ~NativeFixture(){owner.reset();} // ALL contexts/providers remain alive throughclose.
 static int identity(void* p,uintptr_t value,uint32_t* out){auto& t=*static_cast<NativeFixture*>(p);check(value==UINT64_C(0xfedcba9876543210));++t.identity_calls;*out=0xfedcba98;return 0;}
 static int fraction(void* p,float n,char* out,size_t cap,size_t* size){auto& t=*static_cast<NativeFixture*>(p);++t.format_calls;int bytes=std::snprintf(out,cap,"%f",static_cast<double>(n));if(bytes<0||size_t(bytes)>=cap)return 1;*size=size_t(bytes);return 0;}
 static int fixture_identity(void*,const dh2_script_value*,uint32_t,dh2_script_value* out,uint32_t capacity,uint32_t* count,char*,size_t){if(!capacity)return 1;*out={};out->type=2;out->identity=UINT64_C(0xfedcba9876543210);*count=1;return 0;}
 static int override_callback(void* pointer,const dh2_script_value* a,uint32_t n,dh2_script_value* out,uint32_t capacity,uint32_t* returned,char* error,size_t ec){
  auto& c=*static_cast<Callback*>(pointer);++c.fixture->override_calls;
  return dh2_script_int_set_callback(const_cast<dh2_script_int_bindings*>(c.integers),a,n,out,capacity,returned,error,ec);
 }
 static int include(void* pointer,const dh2_script_include_scope* scope,const char* name,char* error,size_t ec){
  auto& c=*static_cast<Callback*>(pointer);auto& t=*c.fixture;++t.include_calls;
  check(dh2_script_include_scope_valid(scope)&&scope->vm==c.vm);
  const dh2_script_int_bindings* now=nullptr;check(t.owner->integer_bindings(c.identity,now)&&now==c.integers);
  const int status=t.owner->include(c.identity,name,t.persistent,scope);
  check(dh2_script_include_scope_valid(scope));if(status&&ec)std::snprintf(error,ec,"native owner Include failed %d",status);return status;
 }
 static int scope(void* pointer,const dh2_script_callback_scope* scope,const dh2_script_value*,uint32_t,char*,size_t){
  auto& c=*static_cast<Callback*>(pointer);auto& t=*c.fixture;++t.scope_calls;check(dh2_script_callback_scope_valid(scope));
  dh2_script_value v{};check(dh2_script_vm_get_global(c.vm,"forbidden",&v)==-1);
  check(dh2_script_vm_load_source_file(c.vm,"x=1",3)==-1);
  const auto step=t.owner->lifecycle().load_step;t.owner->lifecycle().load_step=1;
  check(t.owner->advance({10,0,"__player__","Player"},t.persistent)==-2);t.owner->lifecycle().load_step=step;t.busy_guards+=3;
  check(dh2_script_callback_call_discard_source(scope,"InsideIntegerScope",nullptr,0)==0&&dh2_script_callback_scope_valid(scope));return 0;
 }
 static int observe_close(void* pointer,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t* count,char*,size_t){
  auto& c=*static_cast<Callback*>(pointer);auto& t=*c.fixture;*count=0;
  if(dh2_script_int_size(c.integers->map)||dh2_script_alias_contains(c.aliases,"BeforeClose"))return 1;
  // No generic VM calls during close: those remain outside callback contract.
  int32_t n=-1;if(dh2_script_int_get(c.integers->map,"cleared",&n)||n)return 1;
  ++t.close_observations;return 0;
 }
 static int observe_after(void* pointer,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t* count,char*,size_t){
  auto& c=*static_cast<Callback*>(pointer);*count=0;int32_t n=0;
  if(dh2_script_int_get(c.integers->map,"fromgc",&n)||n!=44||!dh2_script_alias_contains(c.aliases,"AfterClose"))return 1;
  ++c.fixture->after_close_observations;return 0;
 }
 static int service(void* p,ScriptOwner& selected,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
  auto& t=*static_cast<NativeFixture*>(p);
  if(r.service==owner_ai_terminate)t.set_deliveries.erase(selected.lifecycle().pending);
  if(r.service==owner_cached_file&&std::strcmp(r.filename,"data/scripts/ai/_commons.luac")){
   auto i=t.resources.find(r.filename);out.word=i!=t.resources.end();if(out.word){out.bytes=i->second.data();out.size=i->second.size();}return 0;
  }
  if(r.service==owner_register_binding){
   const dh2_script_int_bindings* receiver=nullptr;check(selected.integer_bindings(r.session->identity,receiver));
   auto& c=t.contexts[r.session->identity];c={&t,r.session->identity,receiver,r.session->vm,r.session->aliases};
   if(r.argument0==1&&r.argument1==2){
    check(r.binding->original_callback==0x37de5c&&get(c.vm,"SetInt").type==DH2_SCRIPT_FUNCTION);
    const bool first=t.set_deliveries[c.identity]++==0;
    check(get(c.vm,"GetInt").type==(first?DH2_SCRIPT_NIL:DH2_SCRIPT_FUNCTION));
    const char* code=first?"SetInt('prefix',41)":"SetInt('rebound',42)";check(dh2_script_vm_load_source_file(c.vm,code,std::strlen(code))==0);++t.prefix_observations;
   }
   if(r.argument0==1&&r.argument1==3){
    check(r.binding->original_callback==0x37ec14&&get(c.vm,"GetInt").type==DH2_SCRIPT_FUNCTION);
    check(dh2_script_vm_load_source_file(c.vm,"prefix_seen=GetInt('prefix')",28)==0);check(get(c.vm,"prefix_seen").number==41);++t.prefix_observations;
   }
   // Exact delivered provider failure remains AFTER the installed prefix.
   const auto status=Fixture::service(&t,selected,r,out);if(status)return status;
   if(r.argument0==1&&r.argument1==0)check(!dh2_script_vm_bind_source_include(c.vm,&include,&c));
   if(r.argument0==1&&r.argument1==2&&t.override_set)check(!dh2_script_vm_bind_source_values(c.vm,"SetInt",&override_callback,&c));
   if(r.argument0==1&&r.argument1==3){
    check(!dh2_script_vm_bind(c.vm,"FixtureIdentity",&fixture_identity,nullptr));
    check(!dh2_script_vm_bind_source_scoped(c.vm,"IntegerScope",&scope,&c));
    check(!dh2_script_vm_bind_source_values(c.vm,"ObserveIntegerClose",&observe_close,&c));
    check(!dh2_script_vm_bind_source_values(c.vm,"ObserveIntegerAfter",&observe_after,&c));
   }
   return 0;
  }
  return Fixture::service(&t,selected,r,out);
 }
 int start(const char* name="__player__"){return owner->advance({uint32_t(std::strlen(name)),0,name,"Other"},persistent);}
 void execute(const char* code){ScriptSessionView v{};check(owner->active(v));check(dh2_script_vm_load_source_file(v.vm,code,std::strlen(code))==0);check(dh2_script_vm_stack_size(v.vm)>=5);}
 const dh2_script_int_bindings* receiver(){ScriptSessionView v{};check(owner->active(v));const dh2_script_int_bindings* p=nullptr;check(owner->integer_bindings(v.identity,p));return p;}
 float value(const char* name){ScriptSessionView v{};check(owner->active(v));return get(v.vm,name).number;}
 bool boolean(const char* name){ScriptSessionView v{};check(owner->active(v));auto value=get(v.vm,name);check(value.type==DH2_SCRIPT_BOOLEAN);return value.boolean;}
 void prepare_close(){execute("AddToVFTable('BeforeClose','Old');PushVFTable();AddToVFTable('BeforeClose','During');SetInt('cleared',99);proxy=newproxy(true);getmetatable(proxy).__gc=function() ObserveIntegerClose();SetInt('fromgc',44);assert(GetInt('fromgc')==44);AddToVFTable('AfterClose','Fresh');ObserveIntegerAfter() end");}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 NativeFixture first(common),second(common);check(!first.start()&&!second.start());const auto* a=first.receiver();const auto* b=second.receiver();check(a!=b&&a->map!=b->map&&first.delivered.size()==170&&second.delivered.size()==170);
 first.execute("SetInt('private',71); own=GetInt('private'); missing=GetInt('absent'); SetInt('Alias16038',72); collided=GetInt('Alias16275'); SetInt(1.5,73); fraction=GetInt('1.500000'); SetInt({_this=FixtureIdentity()},74); identity=GetInt('0xfedcba98')");
 second.execute("other=GetInt('private');SetInt('private',81);own=GetInt('private')");check(first.value("own")==71&&first.value("missing")==0&&first.value("collided")==72&&first.value("fraction")==73&&first.value("identity")==74&&second.value("other")==0&&second.value("own")==81&&first.identity_calls==1&&first.format_calls==1);
 first.owner->lifecycle().load_step=1;check(!first.start()&&first.receiver()==a);first.execute("retained=GetInt('private');rebound=GetInt('rebound')");check(first.value("retained")==71&&first.value("rebound")==42&&first.delivered.size()==340);
 first.execute("function InsideIntegerScope()SetInt('scoped',91);assert(GetInt('scoped')==91)end;IntegerScope();scope_seen=GetInt('scoped')");check(first.scope_calls==1&&first.busy_guards==3&&first.value("scope_seen")==91);
 first.resources["data/scripts/ai/ints.luac"]="include_seen=GetInt('private');SetInt('included',92)";first.execute("Include('ints');included=GetInt('included')");check(first.include_calls==1&&first.value("include_seen")==71&&first.value("included")==92&&first.receiver()==a);
 for(int at:{1,2,3,4}){NativeFixture failed(common);failed.fail_at=at;check(failed.start()==-2);ScriptSessionView v{};check(failed.owner->pending(v)&&!failed.owner->lifecycle().active&&failed.delivered.size()==unsigned(at));check(get(v.vm,"SetInt").type==(at>=2?DH2_SCRIPT_FUNCTION:DH2_SCRIPT_NIL)&&get(v.vm,"GetInt").type==(at>=3?DH2_SCRIPT_FUNCTION:DH2_SCRIPT_NIL));}
 NativeFixture override(common);override.override_set=true;check(!override.start());override.execute("SetInt('override',101);override_seen=GetInt('override')");check(override.override_calls==1&&override.value("override_seen")==101);
 NativeFixture missing(common,true);check(!missing.start());const auto size=dh2_script_int_size(missing.receiver()->map);missing.execute("ok_fraction=pcall(function()SetInt(1.5,1)end);ok_identity=pcall(function()GetInt({_this=FixtureIdentity()})end)");check(!missing.boolean("ok_fraction")&&!missing.boolean("ok_identity")&&dh2_script_int_size(missing.receiver()->map)==size);
 NativeFixture replace(common);check(!replace.start());replace.execute("SetInt('old_epoch',111)");replace.prepare_close();replace.owner->lifecycle().load_step=0;
 {int result=replace.start();if(result||replace.close_observations!=1||replace.after_close_observations!=1)std::fprintf(stderr,"replacement result%d close%u after%u message %s\n",result,replace.close_observations,replace.after_close_observations,replace.owner->error().c_str());check(!result&&replace.close_observations==1&&replace.after_close_observations==1);}
 replace.execute("new_epoch=GetInt('old_epoch')");check(replace.value("new_epoch")==0&&replace.owner->lifecycle().load_step==7);
 NativeFixture close(common);check(!close.start());close.prepare_close();close.owner.reset();check(close.close_observations==1&&close.after_close_observations==1);
 for(const char* name:{"Player","Other"}){NativeFixture plain(common);check(!plain.owner->advance({0,0,nullptr,name},plain.persistent));check(plain.receiver()->map&&plain.delivered.size()==170);}
 NativeFixture external(common);external.resources["data/scripts/ai/fixture.luac"]="SetInt('loaded',121);function OnInit()init_seen=GetInt('loaded')end";check(!external.start("fixture")&&external.value("init_seen")==121);ScriptSessionView v{};check(external.owner->active(v)&&v.loaded_files==2&&v.kind==script_external);
 NativeFixture error(common);error.resources["data/scripts/ai/fixture.luac"]="SetInt('error_prefix',131);error('real-source-load-failure')";check(!error.start("fixture"));error.execute("error_seen=GetInt('error_prefix')");check(error.value("error_seen")==131&&error.owner->error().find("real-source-load-failure")!=std::string::npos);
 const dh2_script_int_bindings* unknown=reinterpret_cast<const dh2_script_int_bindings*>(uintptr_t(1));check(!first.owner->integer_bindings(UINT64_C(0xbadbadbadbad),unknown)&&!unknown);
 Fixture legacy(common);check(!legacy.advance());check(legacy.owner->active(v)&&get(v.vm,"SetInt").type==DH2_SCRIPT_NIL);check(!legacy.owner->integer_bindings(v.identity,unknown)&&!unknown);
 bool invalid=false;try{ScriptNativeIntegerConfiguration32 bad{};bad.reserved=1;ScriptOwner bad_owner(1,8*1024*1024,bad);}catch(const std::invalid_argument&){invalid=true;}check(invalid);
 Dl_info owner_info{},integer{},runtime{},lifecycle{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_binding),&owner_info));check(dladdr(reinterpret_cast<void*>(&dh2_script_int_get),&integer));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_load_source_file),&runtime));check(dladdr(reinterpret_cast<void*>(&dh2_character_script_lifecycle),&lifecycle));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"private_owner_pairs\":1,\"exact_integer_registration_indices\":[2,3],\"source_global_deliveries\":170,\"prefix_failure_cases\":4,\"source_replacement_cases\":1,\"actual_close_finalizers\":2,\"source_scope_calls\":1,\"generic_busy_guards\":3,\"actual_nested_Include\":1,\"all_supported_private_session_kinds\":true,\"legacy_provider_only_preserved\":true,\"full_namespace_claim\":false,\"owner_library\":\""<<owner_info.dli_fname<<"\",\"integer_library\":\""<<integer.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"lifecycle_library\":\""<<lifecycle.dli_fname<<"\"}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
