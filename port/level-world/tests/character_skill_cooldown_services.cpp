#include "../character_skill_cooldown_services.hpp"
#include "../character_coordinator.hpp"
#include "../../adam-script-runtime/script_game_bindings.h"
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_skill_cooldown_services;
constexpr std::uintptr_t OWNER=0x100000001ull;
float floating(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
struct Fixture {
    k::Services services{this,OWNER,list,slot,number};
    dh2_script_value args[2]{};std::int32_t fields[5],spells[5],alternate[5],alternate_spells[5];
    unsigned count=3,mask=31,mutation=0,number_calls=0,slot_calls=0,list_calls=0,fail=0,call_count=0,throw_at=0;
    bool slot_fail=false,slot_throw=false;
    bool use_alternate=false;std::vector<unsigned> lists;
    std::uint32_t returned=99;char error[128]{};
    Fixture(){for(auto* values:{fields,spells})for(unsigned i=0;i<5;++i)values[i]=0x11223344;for(auto* values:{alternate,alternate_spells})for(unsigned i=0;i<5;++i)values[i]=0x55667788;args[0].type=3;args[1].type=3;args[1].number=100;}
    bool provider(){++call_count;if(throw_at==call_count)throw std::runtime_error("injected");return fail!=call_count;}
    static int list(void* p,std::uintptr_t owner,std::uint32_t kind,std::uint32_t* out){auto& f=*static_cast<Fixture*>(p);assert(owner==OWNER);++f.list_calls;f.lists.push_back(kind);if(!f.provider())return 1;*out=f.count;if(f.mutation==1)f.args[0].number=1;if(f.mutation==3)f.args[0].number=7;return 0;}
    static int slot(void* p,std::uintptr_t owner,std::uint32_t kind,std::uint32_t index,k::Slot* out){auto& f=*static_cast<Fixture*>(p);assert(owner==OWNER&&kind<2);++f.slot_calls;if(f.slot_throw)throw std::runtime_error("slot failure");if(f.slot_fail)return 1;if(!f.provider())return 1;if(index>=5)return 1;if(!(f.mask&(1u<<index))){*out={};return 0;}auto* field=(kind?(f.use_alternate?f.alternate_spells:f.spells):(f.use_alternate?f.alternate:f.fields))+index;*out={0x200000001ull+kind*256+index*32,field};if(f.mutation==2)f.args[1].type=0;if(f.mutation==4&&f.slot_calls==1)f.use_alternate=true;if(f.mutation==5)f.args[1].type=4;return 0;}
    static int number(void* p,const dh2_script_value* value,float* out){auto& f=*static_cast<Fixture*>(p);++f.number_calls;if(!f.provider())return 1;*out=value->number;if(f.mutation==5&&value==f.args+1)f.use_alternate=true;return 0;}
    int run(bool spell,unsigned n=2){return (spell?k::spell:k::skill)(&services,args,n,nullptr,0,&returned,error,sizeof(error));}
};
std::string read(const char* path){std::ifstream f(path,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};}
struct LuaFixture:Fixture {
    dh2::character::Coordinator actor{OWNER,8};dh2_script_vm* vm=nullptr;
    dh2_script_game_bindings timer_bindings{this,OWNER,start,stop,0};unsigned expired=0,starts=0,stops=0;
    LuaFixture(const char* ai,const char* skills){
        actor.state.current=3;actor.state.flags=0x2380;
        actor.bind({this,facts,{this,state_service},before,nullptr});
        vm=dh2_script_vm_create(4*1024*1024);assert(vm);
        assert(dh2_script_game_bind(vm,&timer_bindings)==0);
        assert(dh2_script_vm_bind_source_values(vm,"SetSkillCooldownTimerId__",k::skill,&services)==0);
        assert(dh2_script_vm_bind_source_values(vm,"SetSpellCooldownTimerId__",k::spell,&services)==0);
        load(read(ai));load(read(skills));
        // Registration fixture has a genuine cooldown-only check. Unsupported
        // update/pre/use callbacks fail if invoked; no combat provider is faked.
        load("DeclareSkill('fixture_skill',0); RegisterSkillEx({update=function() error('unbound update') end,check=function() return not HasSkillCooldown(),false end,pre=function() error('unbound pre') end,skill=function() error('unbound skill') end}); SetSkill('fixture_skill')");
    }
    ~LuaFixture(){dh2_script_vm_destroy(vm);} // close before borrowed owners retire
    static dh2::character::Facts facts(void*){dh2::character::Facts f{};f.is_player=1;return f;}
    static void state_service(void*,dh2::character::State*,const dh2::character::Request*){throw std::runtime_error("unbound state effect");}
    static std::int32_t start(void* p,std::uintptr_t owner,std::uint32_t ms,std::int32_t repeat,std::int32_t event,std::uintptr_t ref){auto& f=*static_cast<LuaFixture*>(p);assert(owner==OWNER&&event==0x35&&!ref);++f.starts;return f.actor.start_timer(ms,repeat,event,ref);}
    static void stop(void* p,std::uintptr_t owner,std::uint32_t id){auto& f=*static_cast<LuaFixture*>(p);assert(owner==OWNER);++f.stops;assert(f.actor.stop_timer(id)>=0);}
    static void before(void* p,dh2::character::Coordinator& owner,std::int32_t event,dh2::character::Timer32& timer,std::uint32_t){auto& f=*static_cast<LuaFixture*>(p);assert(&owner==&f.actor&&event==0x35);++f.expired;assert(dh2_script_game_on_timer(f.vm,timer.id)==0);}
    void load(const std::string& source){assert(dh2_script_vm_load(vm,source.data(),source.size(),"@cooldown-fixture")==0);}
    void call(const char* name,float n){dh2_script_value value{};value.type=3;value.number=n;assert(dh2_script_vm_call_discard_source(vm,name,&value,1)==0);}
    void check(bool ready){dh2_script_value result[2]{};std::uint32_t count=0;assert(dh2_script_vm_call(vm,"OnSkillCheck",nullptr,0,result,2,&count)==0&&count==2&&result[0].type==1&&bool(result[0].boolean)==ready);}
};
int main(int argc,char** argv){
    if(argc>1&&!std::strcmp(argv[1],"oracle")){
        assert(argc==11);Fixture f;bool spell=std::strtoul(argv[2],nullptr,0);unsigned n=std::strtoul(argv[3],nullptr,0);f.args[0].type=std::strtoul(argv[4],nullptr,0);f.args[0].number=floating(std::strtoul(argv[5],nullptr,0));f.args[0].boolean=f.args[0].number!=0;f.args[1].type=std::strtoul(argv[6],nullptr,0);f.args[1].number=floating(std::strtoul(argv[7],nullptr,0));f.count=std::strtoul(argv[8],nullptr,0);f.mask=std::strtoul(argv[9],nullptr,0);f.mutation=std::strtoul(argv[10],nullptr,0);int status=f.run(spell,n);assert(status==0);
        std::cout<<"{\"lists\":[";bool first=true;for(auto k:f.lists){if(!first)std::cout<<',';first=false;std::cout<<k;}std::cout<<"],\"fields\":[";first=true;for(auto* values:{spell?f.spells:f.fields,spell?f.alternate_spells:f.alternate})for(unsigned i=0;i<5;++i){if(!first)std::cout<<',';first=false;std::cout<<std::uint32_t(values[i]);}std::cout<<"]}\n";return 0;
    }
    assert(argc==3);unsigned host=0,guards=0,failures=0,lua=0,slot_contract=0;
    {Fixture f;f.args[0].number=2;assert(f.run(false)==0&&f.fields[2]==100&&f.list_calls==0);++host;}
    {Fixture f;f.args[0].type=1;f.args[0].number=1;assert(f.run(false)==0&&f.fields[1]==100&&f.list_calls==1);++host;}
    {Fixture f;f.args[0].type=4;f.mutation=1;assert(f.run(false)==0&&f.fields[1]==100&&f.fields[0]==0x11223344&&f.number_calls==2);++host;}
    {Fixture f;f.mutation=2;assert(f.run(false)==0&&f.fields[0]==-1);++host;}
    {Fixture f;f.mutation=3;f.args[0].number=100;assert(f.run(true,1)==0&&f.spells[0]==100&&f.spells[2]==100);++host;}
    {Fixture f;f.mutation=4;f.args[0].number=100;assert(f.run(true,1)==0&&f.spells[0]==100&&f.spells[1]==0x11223344&&f.alternate_spells[1]==100&&f.alternate_spells[2]==100);++host;}
    {Fixture f;f.mutation=5;assert(f.run(false)==0&&f.fields[0]==100&&f.alternate[0]==0x55667788);++host;}
    {Fixture f;f.mask=0;assert(f.run(false)==0&&f.returned==0&&f.run(true,1)==0&&f.returned==0&&f.fields[0]==0x11223344&&f.spells[0]==0x11223344);++host;++slot_contract;}
    {Fixture f;f.args[0].type=0;assert(f.run(true,1)==0&&f.spells[0]==-1&&f.spells[2]==-1);++host;}
    for(unsigned n:{0u,1u}){Fixture f;assert(f.run(false,n)==0&&f.call_count==0);++host;}
    {Fixture f;f.args[1].type=1;assert(f.run(false)==0&&f.call_count==0);++host;}
    {Fixture f;f.args[1].type=1;assert(k::skill(nullptr,f.args,2,nullptr,0,&f.returned,nullptr,0)==0&&f.returned==0);++host;}
    {Fixture f;f.args[0].type=1;assert(k::spell(nullptr,f.args,1,nullptr,0,&f.returned,nullptr,0)==0&&f.returned==0);++host;}
    {Fixture f;f.args[0].type=0;f.services.list_count=[](void* p,std::uintptr_t owner,std::uint32_t kind,std::uint32_t* count)->int{Fixture nested;nested.args[1].type=0;assert(nested.run(false)==0&&nested.fields[0]==-1);return Fixture::list(p,owner,kind,count);};assert(f.run(false)==0&&f.fields[0]==100);++host;}
    for(bool throwing:{false,true})for(unsigned at=1;at<=3;++at){Fixture f;f.args[0].type=4;if(throwing)f.throw_at=at;else f.fail=at;assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.returned==0&&f.fields[0]==0x11223344&&f.call_count==at);++failures;}
    for(bool throwing:{false,true}){Fixture f;f.slot_throw=throwing;f.slot_fail=!throwing;assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.returned==0&&f.fields[0]==0x11223344&&f.slot_calls==1&&f.error[0]);++failures;++slot_contract;}
    for(bool throwing:{false,true}){Fixture f;if(throwing)f.throw_at=3;else f.fail=3;assert(f.run(true,1)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.spells[0]==0&&f.spells[1]==0x11223344);++failures;}
    {Fixture f;f.services.number=nullptr;f.args[0].type=4;assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.call_count==0);++failures;}
    {Fixture f;f.services.slot=nullptr;assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE);++failures;}
    {Fixture f;f.args[0].number=-1;assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.slot_calls==0);++failures;}
    {Fixture f;f.args[0].number=6;assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE);++failures;}
    {Fixture f;assert(k::skill(&f.services,f.args,2,nullptr,0,nullptr,nullptr,0)==-1);++guards;assert(k::skill(&f.services,nullptr,2,nullptr,0,&f.returned,nullptr,0)==-1);++guards;}
    {Fixture f;auto* result=reinterpret_cast<std::uint32_t*>(&f.services);auto old=f.services.context;assert(k::skill(&f.services,f.args,2,nullptr,0,result,nullptr,0)==-1&&f.services.context==old);++guards;}
    {Fixture f;assert(k::skill(&f.services,f.args,2,nullptr,0,&f.args[0].type,nullptr,0)==-1&&f.args[0].type==3);++guards;}
    {Fixture f;alignas(k::Services) unsigned char bytes[sizeof(k::Services)+1]{};assert(k::skill(bytes+1,f.args,2,nullptr,0,&f.returned,nullptr,0)==-1);++guards;}
    {Fixture f;f.services.slot=[](void* p,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot* slot)->int{auto& f=*static_cast<Fixture*>(p);*slot={OWNER,reinterpret_cast<std::int32_t*>(&f.args[0].type)};return 0;};assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.args[0].type==3);++guards;}
    {Fixture f;f.services.slot=[](void*,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot* slot)->int{*slot={OWNER,nullptr};return 0;};assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE);++guards;}
    {Fixture f;f.services.slot=[](void* p,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot* slot)->int{auto& f=*static_cast<Fixture*>(p);*slot={0,f.fields};return 0;};assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE);++guards;}
    {Fixture f;std::memset(f.error,'A',sizeof(f.error));f.services.slot=[](void* p,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot* slot)->int{auto& f=*static_cast<Fixture*>(p);*slot={OWNER,reinterpret_cast<std::int32_t*>(f.error)};return 0;};assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE);for(char c:f.error)assert(c=='A');++guards;}
    {Fixture f;f.services.slot=[](void* p,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot* slot)->int{auto& f=*static_cast<Fixture*>(p);*slot={OWNER,reinterpret_cast<std::int32_t*>(reinterpret_cast<unsigned char*>(f.fields)+1)};return 0;};assert(f.run(false)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&f.fields[0]==0x11223344);++guards;}
    {Fixture f;alignas(std::uint32_t) unsigned char bytes[sizeof(std::uint32_t)+1]{};assert(k::skill(&f.services,f.args,2,nullptr,0,reinterpret_cast<std::uint32_t*>(bytes+1),nullptr,0)==-1);++guards;}
    {LuaFixture f(argv[1],argv[2]);f.check(true);f.call("SetSkillCooldown",25);assert(f.fields[0]==0&&f.actor.timers().slots[0].event==0x35);f.check(false);assert(f.actor.update_timers(24,0)==1&&f.fields[0]==0);assert(f.actor.update_timers(1,0)==1&&f.fields[0]==-1&&f.expired==1);f.check(true);++lua;
      f.call("SetSpellCooldown",10);assert(f.spells[0]==0&&f.spells[2]==0&&f.fields[0]==-1);assert(f.actor.update_timers(10,0)==1&&f.spells[0]==-1&&f.spells[2]==-1);++lua;
      f.call("SetSkillCooldown",30);f.call("SetSkillCooldown",50);assert(f.fields[0]==1&&f.actor.timers().slots[0].active&&f.actor.timers().slots[1].active);assert(f.actor.update_timers(30,0)==1&&f.fields[0]==-1&&f.actor.timers().slots[1].active);assert(f.actor.update_timers(20,0)==1&&!f.actor.timers().slots[1].active);++lua;
      f.call("SetSkillCooldown",15);f.call("StopTimerCB",0);assert(!f.actor.timers().slots[0].active&&f.fields[0]==0&&f.stops==1);++lua;
    }
    {for(unsigned mode=0;mode<3;++mode){LuaFixture f(argv[1],argv[2]);if(mode==0)f.services.slot=nullptr;else if(mode==1)f.services.slot=[](void* p,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot*)->int{++static_cast<Fixture*>(p)->slot_calls;return 1;};else f.services.slot=[](void* p,std::uintptr_t,std::uint32_t,std::uint32_t,k::Slot*)->int{++static_cast<Fixture*>(p)->slot_calls;throw std::runtime_error("slot failure");};const auto epoch=dh2_script_vm_required_failure_epoch(f.vm);f.load("caught = pcall(function() SetSkillCooldown(10) end)");assert(dh2_script_vm_required_failure_epoch(f.vm)==epoch+1&&f.actor.timers().slots[0].active&&f.fields[0]==0x11223344&&f.slot_calls==unsigned(mode!=0));dh2_script_value caught{};assert(dh2_script_vm_get_global(f.vm,"caught",&caught)==0&&caught.type==1&&!caught.boolean);++slot_contract;}++lua;}
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<host<<",\"guards\":"<<guards<<",\"failure_cases\":"<<failures<<",\"real_lua_cases\":"<<lua<<",\"slot_contract_cases\":"<<slot_contract<<",\"one_coordinator_timer_owner\":true,\"native_player_active\":false}\n";
}
