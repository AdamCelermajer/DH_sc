#include "../source_level_lifecycle_gate_v1.hpp"

#include <cstdio>
#include <cstdlib>

namespace gate = dh2::source_level_lifecycle_gate_v1;
namespace {
void check(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}
gate::Snapshot active() {
    gate::Snapshot s{};
    s.gslevel_owner = 0x100;
    s.gslevel_level_34 = 0x200;
    s.application_current_level = 0x200;
    s.level_savegame_ec = 0x300;
    s.savegame_parent_level_08 = 0x200;
    s.gslevel_phase = gate::GsLevelPhase::active;
    s.level_state_130 = 38;
    s.transition_flag_144 = 1;
    return s;
}
}

int main() {
    auto s = active();
    auto d = gate::inspect(s);
    check(d.status == gate::Status::ready && d.level == 0x200 &&
          d.gslevel_owner == 0x100 && d.level_savegame == 0x300 &&
          d.transition_flag == 1 && d.quick_save_available,
          "matching published source owner was rejected");

    s.gslevel_phase = gate::GsLevelPhase::updating;
    check(gate::inspect(s).status == gate::Status::loading,
          "Level was exposed while GSLevel was still updating");
    s = active(); s.level_state_130 = 37;
    check(gate::inspect(s).status == gate::Status::not_loaded,
          "source Level state 38 completion gate was skipped");
    s = active(); s.application_current_level = 0x201;
    check(gate::inspect(s).status == gate::Status::owner_mismatch,
          "different static current Level identity was accepted");
    s = active(); s.gslevel_level_34 = 0;
    check(gate::inspect(s).status == gate::Status::unpublished_level,
          "candidate Level without GSLevel+0x34 owner was accepted");
    s = active(); s.teardown_started = 1;
    check(gate::inspect(s).status == gate::Status::tearing_down,
          "tearing-down Level was accepted");
    s = active(); s.savegame_parent_level_08 = 0x201;
    check(gate::inspect(s).status == gate::Status::savegame_parent_mismatch,
          "LevelSavegame from a different Level was accepted");
    s = active(); s.level_savegame_ec = 0;
    const auto no_save = gate::inspect(s);
    check(no_save.status == gate::Status::ready && !no_save.quick_save_available,
          "nullable source LevelSavegame incorrectly blocked a live Level");
    s.gslevel_owner = 0;
    check(gate::inspect(s).status == gate::Status::missing_owner,
          "missing GSLevel owner was accepted");
    s = active(); s.application_current_level = 0;
    check(gate::inspect(s).status == gate::Status::missing_level,
          "missing Application current-Level identity was accepted");
    std::puts("PASS source_level_lifecycle_gate_v1 checks=10");
}
