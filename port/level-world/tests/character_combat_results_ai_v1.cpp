#define main prior_owner_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_combat_results_ai_v1.hpp"
namespace {
struct CombatFixture:Fixture {
 explicit CombatFixture(const std::string& common):Fixture(common){}
 static int type(void*,uintptr_t,const char** name){*name="Character";return 0;}
 static int methods(void*,uintptr_t,const dh2_script_object_method** methods,uint32_t* count){*methods=nullptr;*count=0;return 0;}
 static int method(void*,const dh2_script_callback_scope*,uintptr_t,uint32_t,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int nested(void* context,const dh2_script_callback_scope* scope,const dh2_script_value*,uint32_t,char*,size_t){
  auto& f=*static_cast<CombatFixture*>(context);std::string error;return character_combat_results_ai_v1({f.owner.get(),scope,0x3dc9a8},101,202,0,error);
 }
};
}
int main(int argc,char** argv){try{
 check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 CombatFixture f(common);check(!f.owner->advance({0,0,nullptr,"Other"},{&f,Fixture::service}));ScriptSessionView view{};check(f.owner->active(view));
 dh2_script_object_services objects{&f,CombatFixture::type,CombatFixture::methods,CombatFixture::method};check(!dh2_script_vm_set_source_objects(view.vm,&objects));
 load(view.vm,"hits=0;misses=0;depth=0;function Hit(a,b) assert(a._this~=nil and b._this~=nil);hits=hits+1;if depth==0 then depth=1;Nested();depth=0 end end;function Miss(a,b) misses=misses+1 end");
 check(!dh2_script_alias_add(view.aliases,"OnTargetHit","Hit"));check(!dh2_script_alias_add(view.aliases,"OnTargetMissed","Miss"));check(!f.owner->init_vcb(view.identity,{&f,Fixture::service}));
 check(!dh2_script_vm_bind_source_scoped(view.vm,"Nested",CombatFixture::nested,&f));std::string error;const auto top=dh2_script_vm_stack_size(view.vm);
 CombatResultsAiBorrowV1 borrow{f.owner.get(),nullptr,0x3dc9a8};check(!character_combat_results_ai_v1(borrow,101,202,0,error));check(get(view.vm,"hits").number==2);
 for(unsigned flags=1;flags<=3;++flags)check(!character_combat_results_ai_v1(borrow,101,202,flags,error));check(get(view.vm,"misses").number==3);
 check(dh2_script_vm_stack_size(view.vm)==top);dh2_script_callback_scope expired{view.vm,0};borrow.scope=&expired;check(character_combat_results_ai_v1(borrow,101,202,0,error)==-1);borrow.scope=nullptr;
 borrow.source_character_callback=0x3dbf04;check(character_combat_results_ai_v1(borrow,101,202,0,error)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS);
 f.owner->lifecycle().active=0;check(!character_combat_results_ai_v1(borrow,101,202,0,error));
 std::cout<<"PASS actual private ScriptOwner VCB, two source objects, nested scoped combat result, outcomes1/2/3, stale scope and wrong overload rejection\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
