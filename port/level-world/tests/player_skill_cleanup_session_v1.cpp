#include "../player_skill_cleanup_session_v1.hpp"
#define main retained_cleanup_fixture_main
#include "player_skill_session_v1.cpp"
#undef main
namespace c=dh2::player_skill_cleanup_session_v1;
void cleanup_prepare(Fixture& f){f.common();p::source::Result out{};check(f.owner->prepare(&out)==p::source::Status::complete,"cleanup source preparation failed");}
void cleanup_load(Fixture& f,const std::string& source){static unsigned n=0;auto path="fixture/cleanup/"+std::to_string(n++);f.overlay(path.c_str(),source);s::LoadResult r{};check(!f.load(path,r)&&r.source_success,"cleanup fixture load failed");}
float cleanup_global(Fixture& f,const char* name){dh2_script_value v{};check(!dh2_script_vm_get_global(f.session->vm(),name,&v)&&v.type==DH2_SCRIPT_NUMBER,"cleanup fixture global missing");return v.number;}
void oracle_vectors(Fixture& f){
    auto skills=f.owner->slots(c::List::skill),faeries=f.owner->slots(c::List::faery);
    check(skills[0]&&skills[1]&&faeries[0]&&faeries[1],"oracle real instances unavailable");
    const_cast<std::vector<std::uintptr_t>&>(f.owner->slots(c::List::skill))={skills[0],0,skills[1]};
    const_cast<std::vector<std::uintptr_t>&>(f.owner->slots(c::List::faery))={faeries[0],faeries[1]};
}
int cleanup_oracle(int argc,char** argv){
    check(argc==10,"oracle requires cache temp mode null-script set-error set-count callback-error callback-count");
    const auto mode=std::stoi(argv[4]),null_script=std::stoi(argv[5]),set_error=std::stoi(argv[6]),set_count=std::stoi(argv[7]),callback_error=std::stoi(argv[8]),callback_count=std::stoi(argv[9]);
    Catalogue cat(argv[2]);Fixture f(argv[2],std::filesystem::path(argv[3])/"oracle",cat,"KnightPlayerBase");cleanup_prepare(f);oracle_vectors(f);
    if(mode>=4){const_cast<std::vector<std::uintptr_t>&>(f.owner->slots(c::List::skill)).clear();const_cast<std::vector<std::uintptr_t>&>(f.owner->slots(c::List::faery)).clear();}
    cleanup_load(f,"function SetSkill() "+std::string(set_error?"error('set');":"")+(set_count?"return 'old',false":"")+" end; function OnSkillCleanUp() "+std::string(callback_error?"error('cleanup');":"")+(callback_count?"return 'retained',nil,false":"")+" end;");
    auto* script=null_script?nullptr:f.session.get();c::Runtime runtime(&script,*f.owner,CHAR);c::Result r{};std::string error;
    const int status=mode==0?runtime.cleanup(c::List::skill,0,r,error):(mode==1||mode==4)?runtime.cleanup(c::List::skill,r,error):(mode==2||mode==5)?runtime.cleanup(c::List::faery,r,error):runtime.cleanup_all(r,error);
    check(!status,"cleanup oracle failed");
    std::cout<<"{\"constructed\":"<<r.constructed<<",\"set_calls\":"<<r.set_calls<<",\"cleanup_calls\":"<<r.cleanup_calls<<",\"erased\":"<<r.erased<<",\"destroyed\":"<<r.destroyed<<",\"completed\":"<<r.completed<<",\"retained\":"<<runtime.retained_failed_returns()<<"}\n";
    f.session.reset();return 0;
}
struct CleanupProbe {c::Runtime* runtime;unsigned calls=0;};
int cleanup_probe(void* raw,const dh2_script_value*,std::uint32_t n,dh2_script_value*,std::uint32_t,std::uint32_t* returned,char*,std::size_t) noexcept {try{
    auto& p=*static_cast<CleanupProbe*>(raw);check(n==0,"source cleanup callback received arguments");
    c::Result out{};out.index=99;const auto before=out;std::string error="sentinel";
    check(p.runtime->cleanup(c::List::skill,0,out,error)==-1&&!std::memcmp(&out,&before,sizeof(out))&&error=="sentinel","single cleanup reentry changed output");
    check(p.runtime->cleanup(c::List::skill,out,error)==-1&&!std::memcmp(&out,&before,sizeof(out))&&error=="sentinel","list cleanup reentry changed output");
    check(p.runtime->cleanup_all(out,error)==-1&&!std::memcmp(&out,&before,sizeof(out))&&error=="sentinel","all cleanup reentry changed output");
    ++p.calls;*returned=0;return 0;
}catch(...){return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}}
int main(int argc,char** argv){try{
    if(argc>1&&std::string(argv[1])=="--oracle")return cleanup_oracle(argc,argv);
    check(argc==3,"cleanup requires cache and Debug directory");Catalogue cat(argv[1]);unsigned actual=0,protocol=0,failures=0,guards=0;
    for(const char* name:{"KnightPlayerBase","MagePlayerBase","RoguePlayerBase"}){
        Fixture f(argv[1],std::filesystem::path(argv[2])/name,cat,name);cleanup_prepare(f);
        auto* slot=f.session.get();const auto vm=f.session->vm();const auto flags=f.ais.flags_b8;
        const auto skills=f.owner->slots(c::List::skill),faeries=f.owner->slots(c::List::faery);const auto loads=f.session->statistics().load_calls;
        const auto before=f.properties;c::Runtime runtime(&slot,*f.owner,CHAR);c::Result r{};std::string error;
        if(runtime.cleanup_all(r,error))throw std::runtime_error(std::string(name)+" actual cleanup "+error);
        check(r.examined==21&&r.null_slots==8&&r.completed==13&&r.constructed==13&&r.destroyed==13&&r.set_calls==13&&r.cleanup_calls==13,"actual full cleanup counts differ");
        check(f.session->vm()==vm&&f.ais.flags_b8==flags&&f.owner->slots(c::List::skill)==skills&&f.owner->slots(c::List::faery)==faeries&&f.session->statistics().load_calls==loads&&!std::memcmp(&before,&f.properties,sizeof(before)),"cleanup created/reloaded/mutated owner authority");
        ++actual;slot=nullptr;check(!runtime.cleanup_all(r,error)&&r.completed==13&&!r.set_calls&&!r.cleanup_calls&&r.constructed==13&&r.destroyed==13,"null-script source path differs");++protocol;
    }
    Fixture f(argv[1],std::filesystem::path(argv[2])/"protocol",cat,"KnightPlayerBase");cleanup_prepare(f);auto* slot=f.session.get();c::Runtime runtime(&slot,*f.owner,CHAR);c::Result r{};std::string error;
    auto lease=f.owner->lease_timer_fields(CHAR);check(bool(lease),"cleanup timer field lease missing");p::Owner::TimerFieldSlot timer{};check(lease->slot(CHAR,c::List::skill,0,timer),"cleanup timer slot missing");*timer.field18=77;
    cleanup_load(f,"set_count=0; cleanup_count=0; function SetSkill(name,index) set_count=set_count+1; return 'old',false end; function OnSkillCleanUp(...) assert(select('#',...)==0); cleanup_count=cleanup_count+1; return 'new',nil,false end;");
    check(!runtime.cleanup_all(r,error)&&r.erased==13&&r.completed==13&&cleanup_global(f,"set_count")==13&&cleanup_global(f,"cleanup_count")==13&&*timer.field18==77,"cleanup replayed or blanket-stopped timer18");++protocol;
    cleanup_load(f,"function SetSkill() set_effect=23; error('ordinary') end; function OnSkillCleanUp() error('unreached') end;");
    check(!runtime.cleanup(c::List::skill,0,r,error)&&r.set_calls==1&&!r.cleanup_calls&&r.destroyed==1&&r.lua_errors==1&&r.decision==c::Decision::set_skill_error&&cleanup_global(f,"set_effect")==23,"ordinary SetSkill error did not skip/destroy");++protocol;
    cleanup_load(f,"function SetSkill() return 1,2 end; function OnSkillCleanUp() callback_effect=31; error('ordinary callback') end;");
    check(!runtime.cleanup(c::List::faery,0,r,error)&&r.set_calls==1&&r.cleanup_calls==1&&r.erased==1&&r.destroyed==1&&r.last_lua_status>0&&cleanup_global(f,"callback_effect")==31,"ordinary cleanup error lost normal destroy");++protocol;
    cleanup_load(f,"function SetSkill() set_effect=37; error({}) end;");
    auto retained=runtime.retained_failed_returns();check(runtime.cleanup(c::List::skill,0,r,error)==-1&&r.phase==c::Phase::set_skill&&r.last_lua_status==-4&&!r.destroyed&&!r.cleanup_calls&&runtime.retained_failed_returns()==retained+1&&cleanup_global(f,"set_effect")==37,"unsupported SetSkill error invented cleanup");++failures;
    cleanup_load(f,"set_count=0; cleanup_count=0; function SetSkill() set_count=set_count+1; return 'old',false end; function OnSkillCleanUp() cleanup_count=cleanup_count+1; if cleanup_count==2 then pcall(function() GetHostPlayer() end) end; return 'retained' end;");
    retained=runtime.retained_failed_returns();check(runtime.cleanup_all(r,error)==-1&&r.phase==c::Phase::callback&&r.set_calls==2&&r.cleanup_calls==2&&r.completed==1&&r.destroyed==1&&r.erased==2&&r.last_lua_status==-5&&r.faery_slots==0&&runtime.retained_failed_returns()==retained+1&&cleanup_global(f,"cleanup_count")==2,"caught required failure continued cleanup loop");++failures;
    cleanup_load(f,"function SetSkill() end; function OnSkillCleanUp() callback_effect=41; error({}) end;");
    retained=runtime.retained_failed_returns();check(runtime.cleanup(c::List::skill,0,r,error)==-1&&r.last_lua_status==-4&&!r.destroyed&&runtime.retained_failed_returns()==retained+1&&cleanup_global(f,"callback_effect")==41,"unsupported callback destroyed failed returns");++failures;
    CleanupProbe probe{&runtime};check(!dh2_script_vm_bind(f.session->vm(),"cleanup_probe",cleanup_probe,&probe),"cleanup probe binding failed");
    cleanup_load(f,"function SetSkill() return 'old' end; function OnSkillCleanUp() cleanup_probe() end;");check(!runtime.cleanup(c::List::skill,0,r,error)&&probe.calls==1,"cleanup reentry guard failed");guards+=3;
    r.index=99;const auto saved=r;error="sentinel";
    check(runtime.cleanup(static_cast<c::List>(99),r,error)==-1&&!std::memcmp(&r,&saved,sizeof(r))&&error=="sentinel","invalid cleanup list changed outputs");++guards;
    check(runtime.cleanup(c::List::skill,15,r,error)==-1&&!std::memcmp(&r,&saved,sizeof(r))&&error=="sentinel","null source slot manufactured callback");++guards;
    for(auto* control:{static_cast<void*>(&runtime),static_cast<void*>(f.session.get()),static_cast<void*>(f.owner.get()),static_cast<void*>(&slot),static_cast<void*>(&f.owner->state())}){
        check(runtime.cleanup_all(*reinterpret_cast<c::Result*>(control),error)==-1&&error=="sentinel"&&slot==f.session.get(),"cleanup Result aliased live owner");++guards;
    }
    auto* args=const_cast<p::Arguments*>(f.owner->instance_arguments(f.owner->slots(c::List::skill)[0]));const auto text_before=args->values[0].text;
    check(runtime.cleanup_all(r,args->values[0].text)==-1&&args->values[0].text==text_before&&!std::memcmp(&r,&saved,sizeof(r)),"cleanup error erased authored Arguments");++guards;
    check(*timer.field18==77&&f.session->vm(),"cleanup changed timer/VM ownership");
    f.session.reset();std::cout<<"{\"validation\":\"PASS\",\"actual_class_cleanups\":"<<actual<<",\"protocol_cases\":"<<protocol<<",\"failure_prefix_cases\":"<<failures<<",\"guard_cases\":"<<guards<<",\"same_vm\":true,\"native_death\":false}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
