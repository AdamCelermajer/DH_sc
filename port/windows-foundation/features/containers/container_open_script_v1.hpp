#pragma once

// P16 CONTAINERS2 (T6): OnOpen script contract for authored containers.
//
// The original OnOpen bodies (data/scripts/objects/*_spawn_container.luac) are
// small decision trees: GetRand(0,100) draws, `slot < percent` tests and
// Summon(CharacterTable row, spawn=true, 0,0,0, relative) calls. They are kept as
// DATA here (one contract per authored script name), and one generic interpreter
// runs any contract with injected services. Nothing is keyed by map or level.
//
// Unknown script names are not an error: the caller logs them once. A Summon
// that the spawn owner refuses is a normal script outcome (the Lua call returns
// and the script continues), reported in the run report.

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::containers {

struct OpenScriptStepV1 {
    enum class Kind : std::uint8_t { draw, if_less, summon };
    Kind kind = Kind::draw;
    // draw: GetRand(0,100) written to `slot`. if_less: tests slots[slot] < percent.
    std::int32_t slot = 0;
    std::int32_t percent = 0;
    // summon: CharacterTable row name (GetPyOID("CharacterTable", name)).
    std::string character;
    std::vector<OpenScriptStepV1> then_steps;
    std::vector<OpenScriptStepV1> else_steps;
};

struct OpenScriptContractV1 {
    std::string script;                 // authored row script name (no .luac)
    std::vector<OpenScriptStepV1> steps;
};

// Built-in contracts mirrored from the original scripts (see the .cpp).
const std::vector<OpenScriptContractV1>& open_script_contracts_v1();
const OpenScriptContractV1* find_open_script_contract_v1(const std::string& script);

struct OpenScriptSummonRequestV1 {
    std::string character;    // CharacterTable row
    bool spawn = true;        // Summon(..., spawn=true)
};

// GetRand(lo, hi): inclusive bounds, as in the original scripts.
using OpenScriptRandomFn = std::function<bool(std::int32_t lo, std::int32_t hi,
                                              std::int32_t& value, std::string& error)>;
// One Summon call. Returns true when the owner spawned the character. A false
// return with `reason` set is a refused summon (the script continues).
using OpenScriptSummonFn = std::function<bool(const OpenScriptSummonRequestV1& request,
                                              std::string& reason)>;

struct OpenScriptRunReportV1 {
    std::size_t draws = 0;
    std::size_t summon_calls = 0;
    std::size_t summons_spawned = 0;
    // One line per Summon call, for the caller's single log line each.
    std::vector<std::string> summon_lines;
};

// Runs one contract. Draws happen in source order; a random failure aborts.
bool run_open_script_v1(const OpenScriptContractV1& contract,
                        const OpenScriptRandomFn& random,
                        const OpenScriptSummonFn& summon,
                        OpenScriptRunReportV1& report, std::string& error);

// Every CharacterTable row one contract can Summon (the pool reservation input for a level).
std::vector<std::string> open_contract_summon_characters_v1(const OpenScriptContractV1& contract);

// Every CharacterTable row that any built-in contract can Summon.
std::vector<std::string> open_script_summon_characters_v1();

} // namespace dh::foundation::containers
