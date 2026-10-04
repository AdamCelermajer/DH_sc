#define main historical_owner_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
namespace {
struct SourceFixture:Fixture{
 std::map<std::string,std::string> resources;
 std::vector<std::string> requests;
 bool fail_cache=false;
 explicit SourceFixture(const std::string& common):Fixture(common){}
 static int service(void* opaque,ScriptOwner& owner,const ScriptOwnerRequest& request,ScriptOwnerResponse& response){
  auto& self=*static_cast<SourceFixture*>(opaque);
  if(request.service!=owner_cached_file)return Fixture::service(&self,owner,request,response);
  self.requests.emplace_back(request.filename);if(self.fail_cache)return 1;
  auto found=self.resources.find(request.filename);response.word=found!=self.resources.end();
  if(response.word){response.bytes=found->second.data();response.size=found->second.size();}return 0;
 }
 ScriptOwnerServices source(){return {this,&service};}
};
int required(void*,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned* n,char*,std::size_t){*n=0;return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
int ordinary(void*,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned* n,char*,std::size_t){*n=0;return 1;}
int scoped_required(void*,const dh2_script_callback_scope*,const dh2_script_value*,unsigned,char*,std::size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
int include_required(void*,const dh2_script_include_scope*,const char*,char*,std::size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 SourceFixture first(common),second(common);
 check(first.owner->advance({0,0,nullptr,"Other"},first.source())==0);
 check(second.owner->advance({0,0,nullptr,"Other"},second.source())==0);
 ScriptSessionView a{},b{};check(first.owner->active(a)&&second.owner->active(b));check(a.vm!=b.vm);
 std::string path="unchanged";check(first.owner->copy_path(a.identity,path)==0&&path=="data/scripts/ai/");
 const std::string embedded("a\0b/",4);check(!first.owner->assign_path(a.identity,embedded.data(),embedded.size()));
 check(!first.owner->copy_path(a.identity,path)&&path==embedded);
 check(!second.owner->copy_path(b.identity,path)&&path=="data/scripts/ai/");
 const std::string skills="data/scripts/skills/";check(!first.owner->assign_path(a.identity,skills.data(),skills.size()));
 first.resources[skills+"_commons.luac"]="loaded_count=(loaded_count or 0)+1;function DeclareSkill(...) argc=select('#',...);name,index=... end";
 first.resources[skills+"typed.luac"]="typed_count=(typed_count or 0)+1";
 first.resources[skills+"broken.luac"]="before_error=9;error('authored failure')";
 first.resources[skills+"native.luac"]="before_required=7;pcall(NativeRequired);after_required=8";
 bool delivered=false;
 check(!first.owner->load_file(a.identity,"_commons",first.source(),delivered)&&delivered);
 check(first.requests.size()==1&&first.requests.back()==skills+"_commons.luac");
 check(!first.owner->load_file(a.identity,"_commons",first.source(),delivered)&&delivered&&first.requests.size()==1);
 check(!first.owner->load_file(a.identity,"typed.lua",first.source(),delivered)&&delivered);
 check(!first.owner->load_file(a.identity,"absent",first.source(),delivered)&&!delivered);
 check(!first.owner->load_file(a.identity,"broken",first.source(),delivered)&&!delivered);
 check(first.owner->last_source_load_status()>0&&get(a.vm,"before_error").number==9);
 const auto before=first.requests.size();first.fail_cache=true;delivered=true;
 check(first.owner->load_file(a.identity,"not_loaded",first.source(),delivered)==-2&&delivered&&first.requests.size()==before+1);first.fail_cache=false;
 check(dh2_script_vm_bind_source_values(a.vm,"NativeRequired",required,nullptr)==0);
 check(first.owner->load_file(a.identity,"native",first.source(),delivered)==-2&&delivered);
 check(get(a.vm,"before_required").number==7&&get(a.vm,"after_required").number==8);
 const auto initial=dh2_script_vm_required_failure_epoch(a.vm);check(initial==1);
 dh2_script_value values[18]{};for(unsigned i=0;i<18;++i){values[i].type=DH2_SCRIPT_NUMBER;values[i].number=float(i);}
 std::uint32_t source_error=999;
 check(!first.owner->call_discard(a.identity,"DeclareSkill",values,18,source_error)&&source_error==0&&get(a.vm,"argc").number==18);
 load(a.vm,"projection_count=0;function Returns() return 1,'x',setmetatable({},{__index=function(_,k) if k=='_this' then projection_count=projection_count+1 end end}) end;function AuthoredError() error(123) end;function NonStringError() error({}) end;function UncaughtRequired() NativeRequired() end;function CaughtRequired() pcall(NativeRequired);side_effect=42 end;function Clean() clean_calls=(clean_calls or 0)+1 end");
 check(!first.owner->call_discard(a.identity,"Returns",nullptr,0,source_error)&&source_error==0&&get(a.vm,"projection_count").number==1);
 check(!first.owner->call_discard(a.identity,"AuthoredError",nullptr,0,source_error)&&source_error>0);
 check(first.owner->call_discard(a.identity,"NonStringError",nullptr,0,source_error)==-2);
 check(first.owner->call_discard(a.identity,"UncaughtRequired",nullptr,0,source_error)==-2);
 check(first.owner->call_discard(a.identity,"CaughtRequired",nullptr,0,source_error)==-2&&get(a.vm,"side_effect").number==42);
 check(!first.owner->call_discard(a.identity,"Clean",nullptr,0,source_error)&&!source_error&&get(a.vm,"clean_calls").number==1);
 check(dh2_script_vm_required_failure_epoch(a.vm)==initial+2&&dh2_script_vm_required_failure_epoch(b.vm)==0);
 check(!dh2_script_vm_bind_source_values(a.vm,"Ordinary",ordinary,nullptr));
 load(a.vm,"function OrdinaryError() Ordinary() end");
 check(!first.owner->call_discard(a.identity,"OrdinaryError",nullptr,0,source_error)&&source_error>0&&dh2_script_vm_required_failure_epoch(a.vm)==initial+2);
 check(!dh2_script_vm_bind_source_scoped(a.vm,"ScopedRequired",scoped_required,nullptr));
 check(!dh2_script_vm_bind_source_include(a.vm,include_required,nullptr));
 load(a.vm,"function ScopedMissing() pcall(ScopedRequired) end;function IncludeMissing() pcall(Include,'test') end");
 check(first.owner->call_discard(a.identity,"ScopedMissing",nullptr,0,source_error)==-2);
 check(first.owner->call_discard(a.identity,"IncludeMissing",nullptr,0,source_error)==-2);
 check(dh2_script_vm_required_failure_epoch(a.vm)==initial+4);
 check(!dh2_script_alias_add(a.aliases,"Alias","Clean"));
 check(!first.owner->call_discard(a.identity,"Alias",nullptr,0,source_error)&&get(a.vm,"clean_calls").number==2);
 check(!dh2_script_alias_add(a.aliases,"OnTargetHit","Clean"));check(!first.owner->init_vcb(a.identity,first.source()));
 check(first.owner->active(a)&&a.callback_flags==0x800);
 path="guard";source_error=777;delivered=true;
 check(first.owner->copy_path(0,path)==-1&&path=="guard");
 check(first.owner->assign_path(a.identity,nullptr,1)==-1);
 check(first.owner->call_discard(0,"Clean",nullptr,0,source_error)==-1&&source_error==777);
 check(first.owner->call_discard(a.identity,"Clean",nullptr,1,source_error)==-1&&source_error==777);
 check(first.owner->load_file(0,"x",first.source(),delivered)==-1&&delivered);
 check(first.owner->init_vcb(0,first.source())==-1);
 check(dh2_script_vm_stack_size(a.vm)==5&&dh2_script_vm_stack_size(b.vm)==5);
 Dl_info world{},runtime{};check(dladdr(reinterpret_cast<void*>(&dh2_character_script_binding),&world));check(dladdr(reinterpret_cast<void*>(&dh2_script_vm_call_source_status_objects),&runtime));
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"private_path_and_cache_owners\":2,\"required_failure_kinds\":4,\"source_Lua_errors_separate\":true,\"caught_required_failures_rejected\":true,\"returned_values_projected\":true,\"arguments_without_fixed_arity_cap\":18,\"world_library\":\""<<world.dli_fname<<"\",\"runtime_library\":\""<<runtime.dli_fname<<"\",\"mismatches\":0}\n";
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
