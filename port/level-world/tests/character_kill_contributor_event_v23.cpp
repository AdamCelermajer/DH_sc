#define main prior_owner_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_owner_v23_fixture.inc"
#pragma GCC diagnostic pop
#undef main
#include "../character_kill_contributor_event_v23.hpp"
#include "../character_script_player_vcb_v2.hpp"
#include <cstdio>
namespace {
struct KillFixture:Fixture {
 AIEventOwner48 event_owner{};AIEventState64 ai{};std::array<std::uintptr_t,51> keys{};
 std::unique_ptr<CharacterKillContributorEventV23> contributor;unsigned states{},attacks{};std::string error;
 explicit KillFixture(const std::string& common):Fixture(common){}
 static int type(void*,uintptr_t,const char** n){*n="Character";return 0;}
 static int methods(void*,uintptr_t,const dh2_script_object_method** m,uint32_t* n){*m=nullptr;*n=0;return 0;}
 static int method(void*,const dh2_script_callback_scope*,uintptr_t,uint32_t,const dh2_script_value*,uint32_t,dh2_script_value*,uint32_t,uint32_t*,char*,size_t){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 static int attack(void* p,const dh2_script_callback_scope* scope,const dh2_script_value*,uint32_t,char* error,size_t size){auto& f=*static_cast<KillFixture*>(p);++f.attacks;if(f.contributor->raise(202,scope,f.error))return 0;std::snprintf(error,size,"%s",f.error.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
};
}
int main(int argc,char** argv){try{check(argc==2);std::ifstream input(argv[1],std::ios::binary);std::string common((std::istreambuf_iterator<char>(input)),{});check(common.size()==13535);
 KillFixture f(common);check(!f.advance());ScriptSessionView v{};check(f.owner->active(v)&&v.kind==script_player_iphone);std::uintptr_t source{};check(character_ais_kill_method_v23(v,source)&&source==0x3ddb10);
 dh2_script_object_services objects{&f,KillFixture::type,KillFixture::methods,KillFixture::method};check(!dh2_script_vm_set_source_objects(v.vm,&objects));
 load(v.vm,"kill_calls=0;function VictimKill(victim) assert(victim._this~=nil);kill_calls=kill_calls+1 end;AddToVFTable('OnKill','VictimKill')");check(!f.owner->init_vcb(v.identity,f.services()));check(f.owner->active(v));std::uint32_t actual_flags{};check(!dh2_character_script_player_vcb_v2(&actual_flags,v.aliases)&&actual_flags==v.callback_flags&&(actual_flags&0x400));
 auto lease=std::make_shared<int>(1);f.event_owner={f.store.owner,2,3,4,0,0,0,0};f.ai={5,&f.event_owner,f.keys.data(),v.identity,nullptr,0,0,0,0,0,0};KillContributorServicesV23 services;services.actual_receiver=lease;services.state_event=[&](std::uint32_t event,std::uintptr_t victim,const dh2_script_callback_scope*,std::string&){check(event==4&&victim==202);++f.states;return true;};f.contributor=std::make_unique<CharacterKillContributorEventV23>(f.ai,*f.owner,services);
 check(!dh2_script_vm_bind_source_scoped(v.vm,"ActualAttack",KillFixture::attack,&f));auto top=dh2_script_vm_stack_size(v.vm);load(v.vm,"ActualAttack()");check(get(v.vm,"kill_calls").number==1&&f.attacks==1&&dh2_script_vm_stack_size(v.vm)==top);
 f.event_owner.locked=1;load(v.vm,"ActualAttack()");check(get(v.vm,"kill_calls").number==1&&f.states==1);f.event_owner.forced=1;load(v.vm,"ActualAttack()");check(get(v.vm,"kill_calls").number==2&&f.states==1);f.event_owner.locked=f.event_owner.forced=0;
 load(v.vm,"function VictimKill(victim)kill_calls=kill_calls+10;error('source-kill-prefix')end");load(v.vm,"ActualAttack()");check(get(v.vm,"kill_calls").number==12&&f.error.find("source-kill-prefix")!=std::string::npos);
 check(character_ais_kill_vm_v23(*f.owner,v.identity,0x3dbf00,202,nullptr,f.error)==DH2_SCRIPT_REQUIRED_FAILURE_STATUS);
 dh2_script_callback_scope expired{v.vm,0};check(character_ais_kill_vm_v23(*f.owner,v.identity,source,202,&expired,f.error)==-1);
 // Genuine InitVCB alias membership removal makes the original gate false.
 check(!dh2_script_alias_clear_contents(v.aliases));check(!f.owner->init_vcb(v.identity,f.services()));check(f.owner->active(v)&&!(v.callback_flags&0x400));check(character_ais_kill_vm_v23(*f.owner,v.identity,source,202,&expired,f.error)==0);
 f.contributor.reset();check(f.ai.ai_virtuals==f.keys.data());
 std::cout<<"PASS actual commons/private ScriptOwnerV2/OnKill alias/source VCB400 + busy sameVM scoped contributor event4 + forced/locked FSM + Lua error prefix/expiry + no-second-AI checks="<<checks<<"; external namespace/FSM fixtures declared\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
