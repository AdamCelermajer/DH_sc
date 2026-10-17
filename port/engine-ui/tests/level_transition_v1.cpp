#include "../level_transition_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

namespace {
using namespace dh2::ui;
void check(bool value, const char* message) {
    if (!value) { std::cerr << "FAIL " << message << '\n'; std::exit(1); }
}
struct Fixture {
    LevelTransitionCurrentV1 current{true, 1, 38};
    std::vector<std::string> calls;
    bool fail_display{};
    LevelTransitionLoadV1 loaded;
    static Fixture& self(void* p) { return *static_cast<Fixture*>(p); }
    static bool convert(void* p, double value, std::int32_t& out, std::string&) {
        auto& f=self(p); f.calls.push_back("convert"); out = value == -1.0 ? -1 : static_cast<int>(value); return true;
    }
    static bool current_level(void* p, LevelTransitionCurrentV1& out, std::string&) {
        auto& f=self(p); f.calls.push_back("current"); out=f.current; return true;
    }
    static bool quick_save(void* p, bool force, std::string&) {
        self(p).calls.push_back(force ? "quick!" : "quick"); return true;
    }
    static bool save_players(void* p, bool force, std::string&) {
        self(p).calls.push_back(force ? "save-all!" : "save-all"); return true;
    }
    static bool slot(void* p, std::int32_t& out, std::string&) {
        self(p).calls.push_back("slot"); out=4; return true;
    }
    static bool difficulty(void* p, std::int32_t& out, std::string&) {
        self(p).calls.push_back("difficulty"); out=2; return true;
    }
    static bool display(void* p, bool flag, const std::string& name,
                        const std::string& repeated, std::int32_t entry, std::string& error) {
        auto& f=self(p); f.calls.push_back("display:"+name+":"+std::to_string(entry));
        if(flag || repeated!=name) { error="DisplayFastTravel argument mismatch"; return false; }
        if(f.fail_display) { error="display failed"; return false; }
        return true;
    }
    static bool load(void* p, const LevelTransitionLoadV1& request, std::string&) {
        auto& f=self(p); f.calls.push_back("load"); f.loaded=request; return true;
    }
    LevelTransitionServicesV1 services() {
        return {this, convert, current_level, quick_save, save_players, slot,
                difficulty, display, load};
    }
};
}

int main() {
    using namespace dh2::data;
    LevelTables tables;
    LevelDeclaration crypt{};
    crypt.name = "CRYPT";
    crypt.level_file = "007_crypt_01.rule.xml";
    tables.levels.push_back(crypt);
    Fixture f;
    LevelTransitionOwnerV1 owner(tables, f.services());
    std::string error="stale";
    check(owner.transition("CRYPT", -1.0, error), "source transition succeeds");
    check(error.empty() && f.calls == std::vector<std::string>{
        "current","convert","quick","save-all","slot","difficulty",
        "display:CRYPT:0","load"}, "ordered QuickSave/SaveAll/resolve/Display/LoadLevel transaction");
    check(f.loaded.file=="007_crypt_01.rule.xml" && f.loaded.entry_point==0 &&
          f.loaded.player_slot==4 && f.loaded.difficulty==2 && f.loaded.resume &&
          f.loaded.pending && !f.loaded.remote_trigger && !f.loaded.seed &&
          !f.loaded.synchronized_seed, "source LoadLevel argument projection");

    Fixture blocked; blocked.current.transition_flag=0;
    LevelTransitionOwnerV1 blocked_owner(tables, blocked.services());
    check(blocked_owner.transition("CRYPT", 5.0, error) &&
          blocked.calls==std::vector<std::string>{"current"},
          "source current-Level transition gate short circuits before conversion");

    Fixture no_level; no_level.current.present=false;
    LevelTransitionOwnerV1 no_level_owner(tables, no_level.services());
    check(no_level_owner.transition("CRYPT", 3.0, error), "null-Level transition succeeds");
    check(no_level.calls==std::vector<std::string>{"current","convert","difficulty",
          "display:CRYPT:3","load"} && no_level.loaded.player_slot==0,
          "null Level skips saves and source initializes slot to zero");

    Fixture empty; empty.current.state=0;
    LevelTransitionOwnerV1 empty_owner(tables, empty.services());
    check(empty_owner.transition("", 8.0, error) &&
          empty.calls==std::vector<std::string>{"current","convert","save-all","slot"},
          "empty source string retains save prefix then skips destination continuation");

    Fixture partial; partial.fail_display=true;
    LevelTransitionOwnerV1 partial_owner(tables, partial.services());
    check(!partial_owner.transition("CRYPT", 1.0, error) && error=="display failed" &&
          partial.calls.back()=="display:CRYPT:1" &&
          partial_owner.reached_phase()==LevelTransitionPhaseV1::display_fast_travel,
          "later service failure preserves source side-effect prefix and phase");
    partial.fail_display=false;
    check(!partial_owner.transition("MISSING", 1.0, error) &&
          error.find("absent from the current LevelList")!=std::string::npos,
          "unknown LevelList destination fails closed before menu or loader");
    std::cout << "PASS level_transition_v1 checks=9\n";
}
