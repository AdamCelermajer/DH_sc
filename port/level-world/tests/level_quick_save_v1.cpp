#include "../level_quick_save_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace qs = dh2::level_quick_save_v1;

namespace {
void check(bool ok, const char* text) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", text); std::exit(1); }
}

struct Fixture {
    std::vector<std::string> calls;
    std::uint8_t gate{0x31};
    bool save_saw_open_gate{};
    bool fail_save{};
    qs::Services services{this, copy_position, write_gate, save};

    static std::int32_t copy_position(void* raw, std::uintptr_t character,
        std::uint32_t word_168, std::uint32_t word_160, std::uint32_t word_164,
        std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(character == 0x1234 && word_168 == 0x11 && word_160 == 0x22 &&
              word_164 == 0x33,
              "QuickSave position provider arguments changed");
        self.calls.emplace_back("position");
        return 0;
    }
    static std::int32_t write_gate(void* raw, std::uintptr_t savegame,
        std::uint8_t value, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(savegame == 0x5678, "QuickSave left borrowed LevelSavegame identity");
        self.gate = value;
        self.calls.emplace_back(value == 0x31 ? "restore" : value == 0 ? "open" : "block");
        return 0;
    }
    static std::int32_t save(void* raw, std::uintptr_t savegame, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(savegame == 0x5678, "Save used another LevelSavegame");
        self.save_saw_open_gate = self.gate == 0;
        self.calls.emplace_back("save");
        return self.fail_save ? 1 : 0;
    }
};

qs::State valid_state() {
    qs::State state{};
    state.level = 0x10;
    state.level_savegame = 0x5678;
    state.local_character = 0x1234;
    state.level_state = 38;
    state.online = 0;
    state.character_word_168 = 0x11;
    state.character_word_160 = 0x22;
    state.character_word_164 = 0x33;
    state.level_savegame_gate_39 = 0x31;
    return state;
}
}

int main() {
    std::string error;
    qs::Result result{};

    auto skipped = valid_state();
    skipped.level_state = 37;
    Fixture untouched;
    check(qs::run(&skipped, 1, &untouched.services, &result, error) == qs::Status::skipped,
          "state != 38 did not skip QuickSave");
    check(untouched.calls.empty(), "QuickSave called a provider before admission");
    auto missing_savegame = valid_state();
    missing_savegame.level_savegame = 0;
    check(qs::run(&missing_savegame, 1, &untouched.services, &result, error) == qs::Status::skipped,
          "null LevelSavegame did not skip QuickSave");
    auto missing_character = valid_state();
    missing_character.local_character = 0;
    check(qs::run(&missing_character, 1, &untouched.services, &result, error) == qs::Status::skipped,
          "null local Character did not skip QuickSave");
    auto character_gate = valid_state();
    character_gate.character_quicksave_predicate = 1;
    check(qs::run(&character_gate, 1, &untouched.services, &result, error) == qs::Status::skipped,
          "Character virtual+0x34 gate did not skip QuickSave");
    auto network_gate = valid_state();
    network_gate.online = 1;
    check(qs::run(&network_gate, 1, &untouched.services, &result, error) == qs::Status::skipped,
          "online non-host did not skip QuickSave");
    network_gate.local_player_is_host = 1;
    network_gate.player_manager_gate_719 = 1;
    check(qs::run(&network_gate, 1, &untouched.services, &result, error) == qs::Status::skipped,
          "PlayerManager+0x719 gate did not skip QuickSave");
    check(untouched.calls.empty(), "a skipped QuickSave mutated source state");

    auto forced = valid_state();
    Fixture force_fixture;
    check(qs::run(&forced, 1, &force_fixture.services, &result, error) == qs::Status::saved,
          "forced QuickSave failed");
    check(force_fixture.save_saw_open_gate && force_fixture.gate == 0x31,
          "forced Save did not temporarily open and restore its gate");
    check(force_fixture.calls == std::vector<std::string>{"position", "open", "save", "restore"},
          "forced QuickSave source order changed");
    check(result.position_copied && result.save_called && result.original_gate_restored,
          "forced QuickSave receipt incomplete");

    Fixture ordinary;
    check(qs::run(&forced, 0, &ordinary.services, &result, error) == qs::Status::saved,
          "ordinary QuickSave call failed");
    check(!ordinary.save_saw_open_gate && ordinary.gate == 0x31,
          "ordinary QuickSave changed LevelSavegame +0x39");
    check(ordinary.calls == std::vector<std::string>{"position", "save"},
          "ordinary QuickSave touched +0x39 instead of calling Save directly");
    check(result.original_gate_restored,
          "ordinary QuickSave did not report its unchanged gate");

    Fixture failed;
    failed.fail_save = true;
    check(qs::run(&forced, 1, &failed.services, &result, error) == qs::Status::service_failed,
          "Save provider failure was hidden");
    check(failed.gate == 0x31 && result.original_gate_restored,
          "provider failure stranded the temporary LevelSavegame gate");
    std::puts("PASS level_quick_save_v1 checks=9");
}
