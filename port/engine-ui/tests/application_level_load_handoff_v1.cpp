#include "../application_level_load_handoff_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

namespace {
using namespace dh2::ui;
void check(bool ok, const char* message) {
    if (!ok) { std::cerr << "FAIL " << message << '\n'; std::exit(1); }
}
struct Fixture {
    std::uintptr_t level{0x100}, profile{0x200}, level_save{0x300};
    std::vector<std::string> calls;
    bool fail_quick_save{};
    LevelTransitionLoadV1 continued;

    static Fixture& self(void* raw) { return *static_cast<Fixture*>(raw); }
    static bool show(void* raw, std::uintptr_t level, std::string&) {
        auto& f=self(raw); if (level!=f.level) return false;
        f.calls.emplace_back("loading-menu"); return true;
    }
    static bool flag145(void* raw, std::uintptr_t level, std::uint8_t value,
                        std::string&) {
        auto& f=self(raw); if (level!=f.level || value!=1) return false;
        f.calls.emplace_back("flag-145"); return true;
    }
    static bool score(void* raw, std::uintptr_t level, std::string&) {
        auto& f=self(raw); if (level!=f.level) return false;
        f.calls.emplace_back("score"); return true;
    }
    static bool flagf0(void* raw, std::uintptr_t level, std::uint8_t value,
                       std::string&) {
        auto& f=self(raw); if (level!=f.level || value!=1) return false;
        f.calls.emplace_back("flag-f0"); return true;
    }
    static bool save_player(void* raw, std::uintptr_t level,
                            std::uintptr_t profile, bool force, std::string&) {
        auto& f=self(raw); if (level!=f.level || profile!=f.profile || !force) return false;
        f.calls.emplace_back("player-save!"); return true;
    }
    static bool quick_save(void* raw, std::uintptr_t level,
                           std::uintptr_t level_save, bool force,
                           std::string& error) {
        auto& f=self(raw);
        if (level!=f.level || level_save!=f.level_save || !force) return false;
        f.calls.emplace_back("level-save!");
        if (f.fail_quick_save) { error="LevelSavegame provider failed"; return false; }
        return true;
    }
    static bool reset(void* raw, std::uintptr_t level, std::string&) {
        auto& f=self(raw); if (level!=f.level) return false;
        f.calls.emplace_back("reset-loaded"); return true;
    }
    static bool continuation(void* raw, const LevelTransitionLoadV1& request,
                             std::string&) {
        auto& f=self(raw); f.calls.emplace_back("load-continuation");
        f.continued=request; return true;
    }
    ApplicationLevelLoadServicesV1 services() {
        return {this, show, flag145, score, flagf0, save_player, quick_save,
                reset, continuation};
    }
};
}

int main() {
    Fixture f;
    ApplicationLevelLoadHandoffV1 owner(f.services());
    ApplicationLevelLoadStateV1 state{f.level,f.profile,f.level_save,38,1};
    LevelTransitionLoadV1 request{};
    request.file="007_crypt_01.rule.xml";
    request.entry_point=0; request.player_slot=4; request.difficulty=2;
    std::string error;
    check(owner.run(state,request,error)==ApplicationLevelLoadStatusV1::complete,
          "loaded-level Application::LoadLevel handoff failed");
    check(error.empty() && f.calls==std::vector<std::string>{
        "loading-menu","flag-145","score","flag-f0","player-save!",
        "level-save!","reset-loaded","load-continuation"},
        "Application::LoadLevel source call ordering or force flags differ");
    check(f.continued.file==request.file && f.continued.entry_point==0 &&
          f.continued.player_slot==4 && f.continued.difficulty==2,
          "loader continuation did not preserve the existing transition request");

    Fixture skipped;
    ApplicationLevelLoadHandoffV1 skipped_owner(skipped.services());
    state={skipped.level,skipped.profile,skipped.level_save,37,1};
    check(skipped_owner.run(state,request,error)==
              ApplicationLevelLoadStatusV1::skipped_not_loaded && skipped.calls.empty(),
          "non-38 source Level must return before save or teardown providers");

    Fixture failure; failure.fail_quick_save=true;
    ApplicationLevelLoadHandoffV1 failure_owner(failure.services());
    state={failure.level,failure.profile,failure.level_save,38,0};
    check(failure_owner.run(state,request,error)==
              ApplicationLevelLoadStatusV1::provider_failed &&
          error=="LevelSavegame provider failed" &&
          failure.calls==std::vector<std::string>{"flag-145","score","flag-f0",
              "player-save!","level-save!"} &&
          failure_owner.reached_phase()==ApplicationLevelLoadPhaseV1::quick_save,
          "failed Level QuickSave must stop before ResetIsLoaded or destination load");
    std::cout << "PASS application_level_load_handoff_v1 checks=5\n";
}
