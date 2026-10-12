// Reuse the prior real timer/alias registration fixture without changing its
// historical test or reports. Its main is retained but never called here.
#define main previous_owner_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include "character_script_virtual.hpp"
namespace {
struct ExternalFixture:Fixture {
 std::map<std::string,std::string> resources;
 std::vector<std::string> requested;
 unsigned observed=0;bool single_step=false,cache_failure=false;
 uint32_t expected_flags=0;uintptr_t expected_active=0;
 ExternalFixture(const std::string& common):Fixture(common){}
 static int observe(void* p,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t* count,char*,size_t){
  auto& t=*static_cast<ExternalFixture*>(p);*count=0;ScriptSessionView v{};
  if(!t.owner->pending(v)||t.owner->lifecycle().load_step!=5||t.owner->lifecycle().active!=t.expected_active||v.callback_flags!=t.expected_flags||t.store.count!=2)return 1;
  ++t.observed;return 0;
 }
 static int invoke(void* p,ScriptOwner& owner,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
  auto& t=*static_cast<ExternalFixture*>(p);
  if(r.service==owner_is_character&&t.single_step){out.word=0;return 0;}
  if(r.service==owner_cached_file){
   t.requested.emplace_back(r.filename);if(t.cache_failure&&std::strcmp(r.filename,"data/scripts/ai/_commons.luac"))return 1;
   if(std::strcmp(r.filename,"data/scripts/ai/_commons.luac")){
    const auto i=t.resources.find(r.filename);out.word=i!=t.resources.end();if(out.word){out.bytes=i->second.data();out.size=i->second.size();}return 0;
   }
  }
  const auto status=Fixture::service(&t,owner,r,out);
  if(!status&&r.service==owner_register_binding&&!std::strcmp(r.binding->name,"Trace")){
   if(dh2_script_vm_bind_source_values(r.session->vm,"Observe",&observe,&t))return 1;
  }
  return status;
 }
 ScriptOwnerServices extended_services(){return {this,&invoke};}
 int run(const char* name="fixture"){return owner->advance({uint32_t(std::strlen(name)),0,name,"Other"},extended_services());}
};
std::string read_file(const char* name){std::ifstream f(name,std::ios::binary);check(bool(f));return {(std::istreambuf_iterator<char>(f)),{}};}
void require(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("external owner line "+std::to_string(line)+" check "+std::to_string(checks));}
}
#define check(value) require((value),__LINE__)
int main(int argc,char** argv){try{
 check(argc==5);const auto common=read_file(argv[1]);check(common.size()==13535);
 ExternalFixture staged(common);staged.single_step=true;staged.owner->lifecycle().delayed=1;
 staged.resources["data/scripts/ai/fixture.luac"]="AddToVFTable('OnInit','CustomInit');AddToVFTable('OnUpdate','Absent');function CustomInit()Observe();AddToVFTable('OnTargetHit','AbsentHit');return setmetatable({}, {__index=function(t,k)Observe();return nil end})end";
 ScriptSessionView v{};
 for(int step=0;step<7;++step){check(staged.run()==0&&staged.owner->lifecycle().load_step==step+1);check(staged.owner->pending(v));check(v.kind==script_external&&!v.constructor_fields->derived_present&&v.constructor_fields->flags_b8==0);
  if(step<6)check(!staged.owner->lifecycle().active);
  if(step==0)check(dh2_script_vm_stack_size(v.vm)==0&&staged.delivered.empty());
  if(step==1)check(dh2_script_vm_stack_size(v.vm)==5&&staged.delivered.size()==35);
  if(step==2)check(staged.delivered.size()==170&&v.constructor_fields->character==staged.store.owner);
  if(step==3)check(v.loaded_files==1);
  if(step==4)check(v.loaded_files==2&&v.callback_flags==0&&staged.observed==0);
  if(step>=5)check(staged.observed==2&&v.callback_flags==0x801&&v.constructor_fields->flags_b8==0);
 }
 check(staged.owner->active(v)&&staged.requested==std::vector<std::string>({"data/scripts/ai/_commons.luac","data/scripts/ai/fixture.luac"}));
 staged.expected_flags=0x801;staged.expected_active=v.identity;staged.owner->lifecycle().load_step=5;check(!staged.run()&&staged.observed==4); // no once-only Init guard
 ExternalFixture lua_error(common);lua_error.resources["data/scripts/ai/fixture.luac"]="AddToVFTable('OnUpdate','Absent');function OnInit()error('source-init-error')end";
 check(!lua_error.run()&&lua_error.owner->active(v)&&lua_error.owner->last_vm_status()==-2&&v.callback_flags==1&&v.loaded_files==2);
 check(lua_error.owner->error().find("source-init-error")!=std::string::npos&&lua_error.owner->lifecycle().load_step==7);
 ExternalFixture top_error(common);top_error.resources["data/scripts/ai/fixture.luac"]="AddToVFTable('OnTargetOutOfRange','Absent');error('source-top-error')";
 check(!top_error.run()&&top_error.owner->active(v)&&v.callback_flags==4&&v.loaded_files==1&&top_error.owner->last_vm_status()==0);
 check(top_error.owner->error().find("source-top-error")!=std::string::npos); // Later common OnInit succeeds; error prefix remains diagnostic.
 ExternalFixture missing(common);check(!missing.run()&&missing.owner->active(v)&&v.loaded_files==1&&v.callback_flags==0&&missing.owner->last_vm_status()==0);
 ExternalFixture rejected(common);rejected.cache_failure=true;check(rejected.run()==-2&&rejected.owner->lifecycle().load_step==4&&!rejected.owner->lifecycle().active&&rejected.owner->pending(v));
 for(const char* builtin:{"__monster__","__faery__"}){ExternalFixture unsupported(common);check(unsupported.run(builtin)==-2&&unsupported.delivered.empty()&&!unsupported.owner->lifecycle().pending);}
 // Real Crypt scripts: do not manufacture their absent game globals. Actual
 // original top-level/Init errors remain diagnostics, with source publication
 // and any alias prefix verified through genuine VM execution.
 unsigned authored=0;
 for(unsigned i=0;i<3;++i){const char* name=i==0?"follower":i==1?"monster":"rene";ExternalFixture real(common);real.resources[std::string("data/scripts/ai/")+name+".luac"]=read_file(argv[i+2]);check(!real.run(name)&&real.owner->active(v)&&v.kind==script_external&&real.owner->lifecycle().scripted==1&&real.owner->lifecycle().load_step==7);
  check(real.owner->last_vm_status()==0); // The actual common OnInit is an empty source function.
  if(i==0)check(v.loaded_files==2&&v.callback_flags==0x3c2&&real.owner->error().empty());
  else check(v.loaded_files==1&&!real.owner->error().empty());
  check(real.requested.size()==2);check(get(v.vm,"GetPyInt").type==DH2_SCRIPT_NIL&&get(v.vm,"GetProp").type==DH2_SCRIPT_NIL);
  uint32_t expected=0;check(!dh2_character_script_init_vcb(&expected,v.aliases,1)&&expected==v.callback_flags);++authored;
 }
 Dl_info owner{},runtime{},virtuals{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_constructor_fields),&owner));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_create_empty),&runtime));check(dladdr(reinterpret_cast<void*>(&dh2_character_script_init_vcb),&virtuals));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"ordered_loader_steps\":7,\"fixture_initial_calls\":2,\"returned_table_observations\":4,\"source_delivery_failure_prefixes\":3,\"native_cache_failure_prefixes\":1,\"unsupported_kinds_rejected\":2,\"actual_crypt_scripts\":"<<authored<<",\"actual_follower_initialization\":true,\"actual_follower_callback_flags\":962,\"authored_script_provider_failures\":2,\"fake_game_globals\":false,\"callback_flags_separate_from_initial_fields\":true,\"owner_library\":\""<<owner.dli_fname<<"\",\"virtual_library\":\""<<virtuals.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
