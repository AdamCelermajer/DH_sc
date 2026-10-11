#include "../gslevel_queued_state_handoff_v1.hpp"
#include "../../level-world/level_quick_save_v1.hpp"

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>

namespace {
using namespace dh2::ui;
void check(bool ok, const char* message) {
    if (!ok) { std::cerr << "FAIL " << message << '\n'; std::exit(1); }
}
struct Fixture {
    std::uintptr_t sm{0x10}, gs{0x20}, level{0x30}, save{0x40}, objects{0x50}, players{0x60};
    std::vector<std::string> calls;
    bool fail_save_all{}, fail_update{}, update_guard{};
    bool loading_visible{}, hide_when_sounds_stop{}, fail_visibility{};
    LevelTransitionLoadV1 destination;
    static Fixture& self(void* p) { return *static_cast<Fixture*>(p); }
    static bool quick(void* p, std::uintptr_t l, std::uintptr_t s, std::int32_t state,
                      bool force, std::string&) {
        auto& f=self(p);
        if (l!=f.level || s!=f.save || state!=0 || !force) return false;
        dh2::level_quick_save_v1::State source{};
        source.level=l; source.level_savegame=s; source.level_state=state;
        dh2::level_quick_save_v1::Services services{};
        dh2::level_quick_save_v1::Result result{}; std::string error;
        if (dh2::level_quick_save_v1::run(&source, 1, &services, &result, error) !=
            dh2::level_quick_save_v1::Status::skipped || result.service_calls) return false;
        f.calls.emplace_back("quick-save-skipped"); return true;
    }
    static bool push(void* p, std::uintptr_t l, std::string&) {
        auto& f=self(p); if(l!=f.level)return false; f.loading_visible=true;
        f.calls.emplace_back("loading-push");return true;
    }
    static bool c74(void* p, std::uintptr_t l, std::string&) {
        auto& f=self(p); if(l!=f.level)return false; f.calls.emplace_back("cleanup-74");return true;
    }
    static bool c75(void* p, std::uintptr_t l, std::string&) {
        auto& f=self(p); if(l!=f.level)return false; f.calls.emplace_back("cleanup-75");return true;
    }
    static bool save_players(void* p, std::uintptr_t l, std::uintptr_t pm,
                            bool force, std::string& e) {
        auto& f=self(p); if(l!=f.level||pm!=f.players||!force)return false;
        f.calls.emplace_back("save-all-players-force");
        if(f.fail_save_all){e="SaveAllPlayer failed";return false;} return true;
    }
    static bool sounds(void* p, std::uint32_t fade, std::string&) {
        auto& f=self(p); if(fade!=500)return false; f.calls.emplace_back("sounds-500");
        if(f.hide_when_sounds_stop)f.loading_visible=false;
        return true;
    }
    static bool visible(void* p, bool& value, std::string& error) {
        auto& f=self(p); f.calls.emplace_back("loading-visible");
        if(f.fail_visibility){error="visibility unavailable";return false;}
        value=f.loading_visible;return true;
    }
    static bool pop(void* p, std::string&) { self(p).calls.emplace_back("loading-pop"); return true; }
    static bool clear(void* p, std::uintptr_t l, std::int32_t v, std::string&) {
        auto& f=self(p); if(l!=f.level||v!=0)return false; f.calls.emplace_back("clear-level-state");return true;
    }
    static bool network(void* p, std::uintptr_t om, std::string&) {
        auto& f=self(p); if(om!=f.objects)return false; f.calls.emplace_back("network-uninit");return true;
    }
    static bool player_update(void* p, std::uintptr_t pm, std::string&) {
        auto& f=self(p); if(pm!=f.players)return false; f.calls.emplace_back("player-update");return true;
    }
    static bool seeds(void* p, std::uint32_t seed, std::uint32_t sync, std::string&) {
        auto& f=self(p); if(seed!=1234||sync!=0)return false; f.calls.emplace_back("seed");return true;
    }
    static bool hud(void* p, std::string&) { self(p).calls.emplace_back("close-hud"); return true; }
    static bool delete_level(void* p, std::uintptr_t l, std::string&) {
        auto& f=self(p); if(l!=f.level)return false; f.calls.emplace_back("delete-level");return true;
    }
    static bool clear_gs(void* p, std::uintptr_t gs, std::string&) {
        auto& f=self(p); if(gs!=f.gs)return false; f.calls.emplace_back("clear-current-gs");return true;
    }
    static bool construct(void* p, const LevelTransitionLoadV1& req, std::uintptr_t sm,
                          std::string&) {
        auto& f=self(p); if(sm!=f.sm)return false; f.destination=req; f.calls.emplace_back("construct-destination");return true;
    }
    static bool guard(void* p, std::uintptr_t sm, bool on, std::string&) {
        auto& f=self(p); if(sm!=f.sm)return false; f.update_guard=on;
        f.calls.emplace_back(on?"guard-on":"guard-off");return true;
    }
    static bool update(void* p, std::uintptr_t sm, std::string& error) {
        auto& f=self(p); if(sm!=f.sm||!f.update_guard)return false;
        f.calls.emplace_back("destination-update");
        if(f.fail_update){error="destination update failed";return false;} return true;
    }
    GsLevelQueuedSwitchServicesV1 services() {
        return {this,quick,push,c74,c75,save_players,sounds,pop,clear,network,
                player_update,seeds,hud,delete_level,clear_gs,construct,guard,update,visible};
    }
    GsLevelQueuedSwitchStateV1 state() const { return {sm,gs,level,save,objects,players,0,1,1,1}; }
};
}
int main() {
    Fixture f;
    GsLevelQueuedStateHandoffV1 owner(f.services());
    LevelTransitionLoadV1 request{};
    request.file="007_crypt_01.rule.xml"; request.entry_point=3;
    request.player_slot=2; request.difficulty=1;
    std::string error;
    check(owner.dispatch(f.state(),request,1234,error)==GsLevelQueuedSwitchStatusV1::complete,
          "queued state handoff failed");
    check(error.empty() && f.calls==std::vector<std::string>{"quick-save-skipped","loading-push",
        "cleanup-74","cleanup-75","save-all-players-force","sounds-500","loading-visible","loading-pop",
        "clear-level-state","network-uninit","player-update","seed","close-hud",
        "delete-level","clear-current-gs","construct-destination","guard-on",
        "destination-update","guard-off"}, "source order or force values differ");
    check(f.destination.file==request.file && f.destination.entry_point==3 &&
          f.destination.player_slot==2 && f.destination.difficulty==1 && !f.update_guard,
          "destination request or updating guard was not preserved");
    check(owner.dispatch(f.state(),request,1234,error)==GsLevelQueuedSwitchStatusV1::invalid_state,
          "destructive queued handoff must not be replayable");
    Fixture failed; failed.fail_save_all=true;
    GsLevelQueuedStateHandoffV1 failed_owner(failed.services());
    check(failed_owner.dispatch(failed.state(),request,1234,error)==
              GsLevelQueuedSwitchStatusV1::provider_failed &&
          failed_owner.reached_phase()==GsLevelQueuedSwitchPhaseV1::save_all_players &&
          failed.calls==std::vector<std::string>{"quick-save-skipped","loading-push",
              "cleanup-74","cleanup-75","save-all-players-force"},
          "SaveAllPlayer failure must stop before constructing destination");
    Fixture update_failure; update_failure.fail_update=true;
    GsLevelQueuedStateHandoffV1 update_owner(update_failure.services());
    check(update_owner.dispatch(update_failure.state(),request,1234,error)==
              GsLevelQueuedSwitchStatusV1::provider_failed &&
          !update_failure.update_guard &&
          update_failure.calls.size()>=2 &&
          update_failure.calls[update_failure.calls.size()-2]=="destination-update" &&
          update_failure.calls.back()=="guard-off",
          "failed destination update must clear the StateMachine updating guard");
    Fixture hidden; hidden.hide_when_sounds_stop=true;
    GsLevelQueuedStateHandoffV1 hidden_owner(hidden.services());
    check(hidden_owner.dispatch(hidden.state(),request,1234,error)==GsLevelQueuedSwitchStatusV1::complete &&
          std::find(hidden.calls.begin(),hidden.calls.end(),"loading-pop")==hidden.calls.end(),
          "visibility changes during unload must suppress loading pop");
    Fixture invisible_error; invisible_error.fail_visibility=true;
    GsLevelQueuedStateHandoffV1 visibility_owner(invisible_error.services());
    check(visibility_owner.dispatch(invisible_error.state(),request,1234,error)==GsLevelQueuedSwitchStatusV1::provider_failed &&
          visibility_owner.reached_phase()==GsLevelQueuedSwitchPhaseV1::loading_menu_visibility &&
          invisible_error.calls.back()=="loading-visible",
          "visibility failure must stop before pop and level destruction");
    std::cout << "PASS gslevel_queued_state_handoff_v1 checks=8\n";
}
