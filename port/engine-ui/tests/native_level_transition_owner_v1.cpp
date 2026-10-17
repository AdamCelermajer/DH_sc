#include "../native_level_transition_owner_v1.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {
using namespace dh2::ui;
struct Fixture {
    std::vector<std::string> calls;
    std::uintptr_t state_machine{};
    bool ok{true};
};
bool add(Fixture& f, const char* name, std::string& error) {
    f.calls.emplace_back(name);
    if (!f.ok) error = std::string("failed ") + name;
    return f.ok;
}
bool app_loading(void* p, std::uintptr_t l, std::string& e) { return l==11 && add(*static_cast<Fixture*>(p),"app.loading",e); }
bool app_145(void* p, std::uintptr_t l, std::uint8_t v, std::string& e) { return l==11&&v==1&&add(*static_cast<Fixture*>(p),"app.flag145",e); }
bool app_score(void* p, std::uintptr_t l, std::string& e) { return l==11&&add(*static_cast<Fixture*>(p),"app.score",e); }
bool app_f0(void* p, std::uintptr_t l, std::uint8_t v, std::string& e) { return l==11&&v==1&&add(*static_cast<Fixture*>(p),"app.flagf0",e); }
bool app_player(void* p, std::uintptr_t l, std::uintptr_t s, bool f, std::string& e) { return l==11&&s==12&&f&&add(*static_cast<Fixture*>(p),"app.player",e); }
bool app_save(void* p, std::uintptr_t l, std::uintptr_t s, bool f, std::string& e) { return l==11&&s==13&&f&&add(*static_cast<Fixture*>(p),"app.save",e); }
bool app_reset(void* p, std::uintptr_t l, std::string& e) { return l==11&&add(*static_cast<Fixture*>(p),"app.reset",e); }
bool app_continue(void* p, const LevelTransitionLoadV1& r, std::string& e) { return r.file=="next.mlx"&&r.entry_point==7&&add(*static_cast<Fixture*>(p),"app.continue",e); }

bool qsave(void* p,std::uintptr_t l,std::uintptr_t s,std::int32_t st,bool force,std::string& e){return l==11&&s==13&&st==0&&force&&add(*static_cast<Fixture*>(p),"gs.quick_save",e);}
bool push(void* p,std::uintptr_t l,std::string& e){return l==11&&add(*static_cast<Fixture*>(p),"gs.push",e);}
bool c74(void* p,std::uintptr_t l,std::string& e){return l==11&&add(*static_cast<Fixture*>(p),"gs.cleanup74",e);}
bool c75(void* p,std::uintptr_t l,std::string& e){return l==11&&add(*static_cast<Fixture*>(p),"gs.cleanup75",e);}
bool all_players(void* p,std::uintptr_t l,std::uintptr_t m,bool force,std::string& e){return l==11&&m==15&&force&&add(*static_cast<Fixture*>(p),"gs.players",e);}
bool sounds(void* p,std::uint32_t ms,std::string& e){return ms==500&&add(*static_cast<Fixture*>(p),"gs.sounds",e);}
bool visible(void* p,bool& value,std::string& e){value=true;return add(*static_cast<Fixture*>(p),"gs.visible",e);}
bool pop(void* p,std::string& e){return add(*static_cast<Fixture*>(p),"gs.pop",e);}
bool clear_state(void* p,std::uintptr_t l,std::int32_t s,std::string& e){return l==11&&s==0&&add(*static_cast<Fixture*>(p),"gs.clear_state",e);}
bool net(void* p,std::uintptr_t m,std::string& e){return m==14&&add(*static_cast<Fixture*>(p),"gs.network",e);}
bool pm_update(void* p,std::uintptr_t m,std::string& e){return m==15&&add(*static_cast<Fixture*>(p),"gs.pm_update",e);}
bool seeds(void* p,std::uint32_t seed,std::uint32_t sync,std::string& e){return seed==123&&sync==0&&add(*static_cast<Fixture*>(p),"gs.seeds",e);}
bool hud(void* p,std::string& e){return add(*static_cast<Fixture*>(p),"gs.hud",e);}
bool del(void* p,std::uintptr_t l,std::string& e){return l==11&&add(*static_cast<Fixture*>(p),"gs.delete",e);}
bool clear_gs(void* p,std::uintptr_t g,std::string& e){return g==10&&add(*static_cast<Fixture*>(p),"gs.clear_gs",e);}
bool construct(void* p,const LevelTransitionLoadV1& r,std::uintptr_t sm,std::string& e){auto& f=*static_cast<Fixture*>(p);f.state_machine=sm;return r.file=="next.mlx"&&sm!=0&&add(f,"gs.construct",e);}
bool guard(void* p,std::uintptr_t sm,bool on,std::string& e){return sm==static_cast<Fixture*>(p)->state_machine&&add(*static_cast<Fixture*>(p),on?"gs.guard_on":"gs.guard_off",e);}
bool update(void* p,std::uintptr_t sm,std::string& e){return sm==static_cast<Fixture*>(p)->state_machine&&add(*static_cast<Fixture*>(p),"gs.update",e);}

bool check(bool ok,const char* label){if(!ok)std::cerr<<"FAIL "<<label<<"\n";return ok;}
}

int main(){
    Fixture f;
    NativeLevelTransitionServicesV1 services{};
    services.application={&f,app_loading,app_145,app_score,app_f0,app_player,app_save,app_reset,app_continue};
    services.queued_switch={&f,qsave,push,c74,c75,all_players,sounds,pop,clear_state,net,pm_update,seeds,hud,del,clear_gs,construct,guard,update,visible};
    NativeLevelTransitionOwnerV1 owner(services);
    const ApplicationLevelLoadStateV1 app{11,12,13,38,1};
    const NativeLevelTransitionReferencesV1 refs{10,14,15};
    LevelTransitionLoadV1 request{}; request.file="next.mlx"; request.entry_point=7; request.seed=123;
    std::string error;
    bool ok=true;
    ok &= check(owner.begin_application_load(app,refs,request,error)==NativeLevelTransitionStatusV1::complete,"application load");
    const std::vector<std::string> expected_app={"app.loading","app.flag145","app.score","app.flagf0","app.player","app.save","app.reset","app.continue"};
    ok &= check(f.calls==expected_app,"application callback order");
    GsLevelQueuedSwitchStateV1 gs{};
    gs.old_gslevel=10; gs.old_level=11; gs.old_level_savegame=13; gs.object_manager=14; gs.player_manager=15;
    gs.level_unload_force=1; gs.has_level_cleanup_74=1; gs.has_level_cleanup_75=1;
    ok &= check(owner.dispatch_queued_switch(gs,123,error)==NativeLevelTransitionStatusV1::complete,"queued switch");
    const std::vector<std::string> expected_all={"app.loading","app.flag145","app.score","app.flagf0","app.player","app.save","app.reset","app.continue",
      "gs.quick_save","gs.push","gs.cleanup74","gs.cleanup75","gs.players","gs.sounds","gs.visible","gs.pop","gs.clear_state","gs.network","gs.pm_update","gs.seeds","gs.hud","gs.delete","gs.clear_gs","gs.construct","gs.guard_on","gs.update","gs.guard_off"};
    ok &= check(f.calls==expected_all,"source-ordered two-boundary callbacks");
    ok &= check(owner.native_state_machine_identity()!=0&&owner.native_state_machine_identity()!=10,"native semantic state-machine identity");
    ok &= check(owner.dispatch_queued_switch(gs,123,error)==NativeLevelTransitionStatusV1::invalid_state,"one-shot dispatch");
    if(!ok)return 1;
    std::cout<<"PASS native_level_transition_owner_v1 boundaries=2 callbacks=27\n";
    return 0;
}
