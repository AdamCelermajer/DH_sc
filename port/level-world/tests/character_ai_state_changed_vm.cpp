// Reuse existing isolated owner registration/timer fixture without editing it.
#define main previous_owner_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_ai_state_changed_vm.hpp"
#include "../character_ai_state_changed.hpp"
namespace {
void vmcheck(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("End VM audit line "+std::to_string(line)+" check "+std::to_string(checks));}
}
#define check(value) vmcheck((value),__LINE__)
namespace {
struct External:Fixture {
 std::string script;unsigned recursive=0;std::uintptr_t identity=0;
 explicit External(const std::string& common):Fixture(common){}
 static int provider(void* p,ScriptOwner& owner,const ScriptOwnerRequest& r,ScriptOwnerResponse& out){
  auto& f=*static_cast<External*>(p);
  if(r.service==owner_cached_file&&std::strcmp(r.filename,"data/scripts/ai/_commons.luac")){out.word=1;out.bytes=f.script.data();out.size=f.script.size();return 0;}
  return Fixture::service(&f,owner,r,out);
 }
 static int recurse(void* p,const dh2_script_callback_scope* scope,const dh2_script_value*,uint32_t,char*,size_t){auto& f=*static_cast<External*>(p);++f.recursive;const auto old=f.owner->lifecycle().active;f.owner->lifecycle().active=0;int result=character_ais_external_end_anim(*f.owner,f.identity,scope);f.owner->lifecycle().active=old;return result;}
 int start(){return owner->advance({7,0,"fixture","Player"},{this,provider});}
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);External f(common);check(!f.start());ScriptSessionView v{};check(f.owner->active(v)&&v.kind==script_external);f.identity=v.identity;
 // The real commons endpoint is an empty Lua body, executed in its private VM.
 auto top=dh2_script_vm_stack_size(v.vm);check(!character_ais_external_end_anim(*f.owner,v.identity));check(dh2_script_vm_stack_size(v.vm)==top);
 load(v.vm,"calls=0;projections=0;function EndA()calls=calls+1;return 1,'x',setmetatable({}, {__index=function()projections=projections+1;return nil end})end;function EndB()calls=calls+10 end;AddToVFTable('OnEndOfAnim','EndA')");
 check(!character_ais_external_end_anim(*f.owner,v.identity)&&get(v.vm,"calls").number==1);check(get(v.vm,"projections").number==1);check(!dh2_script_alias_add(v.aliases,"OnEndOfAnim","EndB"));check(!character_ais_external_end_anim(*f.owner,v.identity)&&get(v.vm,"calls").number==11);
 // Fresh global lookup does not freeze the function object at alias creation.
 load(v.vm,"function EndB()calls=calls+100 end");check(!character_ais_external_end_anim(*f.owner,v.identity)&&get(v.vm,"calls").number==111);
 // Explicitly no InitVCB availability bit gates this endpoint.
 check(v.callback_flags==0);check(!dh2_script_alias_add(v.aliases,"OnEndOfAnim","Absent"));check(character_ais_external_end_anim(*f.owner,v.identity)!=0);check(dh2_script_vm_stack_size(v.vm)==top);
 check(!dh2_script_alias_add(v.aliases,"OnEndOfAnim","Broken"));load(v.vm,"function Broken()calls=calls+1;error('end-prefix')end");
 const auto broken=character_ais_external_end_anim(*f.owner,v.identity);const std::string diagnostic=dh2_script_vm_error(v.vm);
 check(broken!=0&&get(v.vm,"calls").number==112);check(diagnostic.find("end-prefix")!=std::string::npos);check(dh2_script_vm_stack_size(v.vm)==top);
 check(!dh2_script_vm_bind_source_scoped(v.vm,"NestedEnd",External::recurse,&f));check(!dh2_script_alias_add(v.aliases,"OnEndOfAnim","Recursive"));load(v.vm,"depth=0;function Recursive()calls=calls+1;if depth==0 then depth=1;NestedEnd();depth=0 end end");check(!character_ais_external_end_anim(*f.owner,v.identity));check(f.recursive==1&&get(v.vm,"calls").number==114&&f.owner->lifecycle().active==v.identity&&dh2_script_vm_stack_size(v.vm)==top);
 dh2_script_callback_scope expired{v.vm,0};check(character_ais_external_end_anim(*f.owner,v.identity,&expired)==-1);check(character_ais_external_end_anim(*f.owner,UINT64_MAX)==-1);
 // Direct captured identity remains usable after active is replaced/cleared.
 f.owner->lifecycle().active=0;check(!character_ais_external_end_anim(*f.owner,v.identity));check(get(v.vm,"calls").number==116);f.owner->lifecycle().active=v.identity;
 Fixture player(common);check(!player.advance());ScriptSessionView pv{};check(player.owner->active(pv)&&pv.kind!=script_external);check(character_ais_external_end_anim(*player.owner,pv.identity)==-1);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_commons_end_calls\":1,\"scoped_nested_calls\":"<<f.recursive<<",\"fresh_alias_and_global\":true,\"availability_gate\":false,\"full_gameplay_namespace\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
