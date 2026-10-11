#pragma once

// P16 CONTAINERS2 (T6) / v20 GENERAL: OnOpen of an authored container.
//
// The OnOpen body is the authored file data/scripts/objects/<script>.luac. Despite its extension it is plain
// Lua source (for example moth_spawn_container: local prob_spawn = 25; calc_spawn = GetRand(0,100); if
// calc_spawn < prob_spawn then Summon(GetPyOID("CharacterTable", "Swamp_Moth_Minions"), true, 0.0, 0.0, 0.0,
// true) end). It runs in the original Lua runtime (port/script-runtime, dh2_script_vm), with the game
// callbacks bound below. No script name, map, or container is special-cased here.
//
// Bound callbacks: GetRand(lo, hi), GetPyOID("CharacterTable", row), Summon(oid, spawn, x, y, z, relative),
// Trace(text), GetName(). Any other game call raises a Lua error, so the open fails closed and is logged.
//
// The OnOpen actor argument is passed as nil. The original passes the opener; the damage branch of
// container.luac (DealDamages / actor:GetName / actor:PlayFX) is therefore not executed. That gap is listed in
// the v20 GENERAL report and is not hidden by a fallback.

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::containers {

struct OpenScriptSummonRequestV1 {
    std::string character;     // CharacterTable row (GetPyOID("CharacterTable", row))
    bool spawn = true;         // Summon(..., spawn)
    float offset[3] = {0, 0, 0};
    bool relative = true;      // Summon(..., relative): offset from the opened object
};

// GetRand(lo, hi): inclusive bounds, as in the original scripts.
using OpenScriptRandomFn = std::function<bool(std::int32_t lo, std::int32_t hi,
                                              std::int32_t& value, std::string& error)>;
// One Summon call. Returns true when the owner spawned the character. A false return with `reason` set
// is a refused summon: a normal script outcome, the Lua call returns false and the script continues.
using OpenScriptSummonFn = std::function<bool(const OpenScriptSummonRequestV1& request,
                                              std::string& reason)>;

struct OpenScriptServicesV1 {
    OpenScriptRandomFn random;
    OpenScriptSummonFn summon;
    // GetPyOID("CharacterTable", row) returns the row index in this table (null: the call fails).
    const std::vector<std::string>* character_rows = nullptr;
    std::string object_name;   // GetName()
};

struct OpenScriptRunReportV1 {
    std::size_t draws = 0;
    std::size_t summon_calls = 0;
    std::size_t summons_spawned = 0;
    std::vector<std::string> summon_lines; // one line per Summon call
    std::vector<std::string> trace_lines;  // Trace() text, in order
};

// Runs OnOpen(nil) of one authored source. `chunk_name` only labels diagnostics.
bool run_authored_open_script_v1(const std::string& source, const std::string& chunk_name,
                                 const OpenScriptServicesV1& services,
                                 OpenScriptRunReportV1& report, std::string& error);

// CharacterTable rows named by literal GetPyOID("CharacterTable", "row") arguments of Summon calls in the
// authored source. Used to reserve spawn-pool capacity before the level runs.
std::vector<std::string> authored_open_summon_rows_v1(const std::string& source);

} // namespace dh::foundation::containers
