// P16 CONTAINERS2 (T6): focused tests for the OnOpen script contract interpreter.
// Pure logic: scripted GetRand values, a fake Summon owner. No assets, no EXE.

#include "container_open_script_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <deque>
#include <functional>
#include <string>
#include <vector>

using namespace dh::foundation::containers;

namespace {

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++failures;
    }
}

// Returns scripted values in order; every call is one GetRand(0,100).
struct ScriptedRandom {
    std::deque<std::int32_t> values;
    std::size_t calls = 0;
    bool operator()(std::int32_t lo, std::int32_t hi, std::int32_t& value, std::string& error) {
        error.clear();
        ++calls;
        if (lo != 0 || hi != 100 || values.empty()) {
            error = "scripted random exhausted or wrong bounds";
            return false;
        }
        value = values.front();
        values.pop_front();
        return true;
    }
};

struct RecordingSummon {
    std::vector<std::string> characters;
    bool refuse = false;
    bool operator()(const OpenScriptSummonRequestV1& request, std::string& reason) {
        characters.push_back(request.character);
        if (refuse) {
            reason = "profile not admitted";
            return false;
        }
        return true;
    }
};

const OpenScriptContractV1& contract(const char* name) {
    const auto* found = find_open_script_contract_v1(name);
    if (!found) {
        std::fprintf(stderr, "missing contract %s\n", name);
        std::exit(2);
    }
    return *found;
}

} // namespace

int main() {
    // Contracts for the four original spawn scripts and the example script exist; unknown names do not.
    check(find_open_script_contract_v1("moth_spawn_container") != nullptr, "moth contract");
    check(find_open_script_contract_v1("plant_spawn_container") != nullptr, "plant contract");
    check(find_open_script_contract_v1("plant_spawn_container2") != nullptr, "plant2 contract");
    check(find_open_script_contract_v1("zombie_spawn_container") != nullptr, "zombie contract");
    check(find_open_script_contract_v1("no_such_script") == nullptr, "unknown script is not a contract");

    // moth: one GetRand(0,100); value < 25 summons Swamp_Moth_Minions; 25 does not.
    {
        ScriptedRandom random{{24}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("moth_spawn_container"), std::ref(random), std::ref(summon), report, error),
              "moth run");
        check(random.calls == 1 && report.draws == 1, "moth draws once");
        check(summon.characters.size() == 1 && summon.characters[0] == "Swamp_Moth_Minions", "moth summons minions at 24");
        check(report.summons_spawned == 1 && report.summon_lines.size() == 1, "moth summon line");
    }
    {
        ScriptedRandom random{{25}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("moth_spawn_container"), std::ref(random), std::ref(summon), report, error),
              "moth boundary run");
        check(summon.characters.empty() && report.summon_calls == 0, "moth does not summon at 25 (strict <)");
    }
    // zombie uses the same single 25% test for InfectedBurned.
    {
        ScriptedRandom random{{0}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("zombie_spawn_container"), std::ref(random), std::ref(summon), report, error),
              "zombie run");
        check(summon.characters.size() == 1 && summon.characters[0] == "InfectedBurned", "zombie summons InfectedBurned");
    }
    // plant: three draws, then always one moth summon (outer test is commented out in the source).
    {
        ScriptedRandom random{{99, 99, 99}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("plant_spawn_container"), std::ref(random), std::ref(summon), report, error),
              "plant run");
        check(random.calls == 3 && summon.characters.size() == 1 && summon.characters[0] == "Swamp_Moth_Minions",
              "plant always summons one moth after three draws");
    }
    // plant2: the second draw only happens behind the 25% gate. Failing the gate: 2 draws, no summon.
    {
        ScriptedRandom random{{30, 5}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("plant_spawn_container2"), std::ref(random), std::ref(summon), report, error),
              "plant2 gate-fail run");
        check(random.calls == 2 && summon.characters.empty(), "plant2 gate fail: 2 draws, no summon");
    }
    // plant2 gate passes: value 10 (< 25), spider roll 60 (>= 50), slot1 = 15 (< 20) -> EarthTemple_BigSpider.
    {
        ScriptedRandom random{{10, 15, 60}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("plant_spawn_container2"), std::ref(random), std::ref(summon), report, error),
              "plant2 pass run");
        check(random.calls == 3 && summon.characters.size() == 1 && summon.characters[0] == "EarthTemple_BigSpider",
              "plant2 spider branch picks the big spider at slot1 < 20");
    }
    // plant2: spider roll 10 (< 50), slot1 = 40 (>= 20) -> EarthTemple_SmallPlant.
    {
        ScriptedRandom random{{10, 40, 10}};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("plant_spawn_container2"), std::ref(random), std::ref(summon), report, error),
              "plant2 small run");
        check(summon.characters.size() == 1 && summon.characters[0] == "EarthTemple_SmallPlant",
              "plant2 plant branch picks the small plant at slot1 >= 20");
    }
    // A refused summon is a normal outcome: the script continues and reports the reason.
    {
        ScriptedRandom random{{1}};
        RecordingSummon summon;
        summon.refuse = true;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("moth_spawn_container"), std::ref(random), std::ref(summon), report, error),
              "refused summon does not abort");
        check(report.summon_calls == 1 && report.summons_spawned == 0 && report.summon_lines.size() == 1 &&
              report.summon_lines[0].find("outcome=refused reason=profile not admitted") != std::string::npos,
              "refused summon reported with reason");
    }
    // A random failure aborts the script with the owner's error.
    {
        ScriptedRandom random{};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(!run_open_script_v1(contract("moth_spawn_container"), std::ref(random), std::ref(summon), report, error) &&
              !error.empty(), "random failure aborts");
    }
    // The empty example contract draws nothing and summons nothing.
    {
        ScriptedRandom random{};
        RecordingSummon summon;
        OpenScriptRunReportV1 report;
        std::string error;
        check(run_open_script_v1(contract("container"), std::ref(random), std::ref(summon), report, error) &&
              random.calls == 0 && summon.characters.empty(), "example contract is inert");
    }
    // The summonable set covers every Summon the original contracts can make.
    {
        const auto names = open_script_summon_characters_v1();
        auto has = [&](const char* name) {
            for (const auto& n : names) if (n == name) return true;
            return false;
        };
        check(has("Swamp_Moth_Minions") && has("InfectedBurned") && has("EarthTemple_Spider") &&
              has("EarthTemple_BigSpider") && has("EarthTemple_SmallPlant") && has("EarthTemple_SmallPlantMinion"),
              "summon set lists all original Summon targets");
        check(names.size() == 6, "summon set has exactly six distinct characters");
    }

    if (failures) {
        std::fprintf(stderr, "container_open_script_v1_tests: %d failure(s)\n", failures);
        return 1;
    }
    std::printf("PASS container_open_script_v1 moth=25-strict plant=3draws plant2=gated zombie=25 refused=continues\n");
    return 0;
}
