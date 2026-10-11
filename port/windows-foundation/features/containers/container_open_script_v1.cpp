#include "container_open_script_v1.hpp"

#include <algorithm>
#include <set>

namespace dh::foundation::containers {
namespace {

OpenScriptStepV1 draw(std::int32_t slot) {
    OpenScriptStepV1 step;
    step.kind = OpenScriptStepV1::Kind::draw;
    step.slot = slot;
    return step;
}

OpenScriptStepV1 if_less(std::int32_t slot, std::int32_t percent,
                         std::vector<OpenScriptStepV1> then_steps,
                         std::vector<OpenScriptStepV1> else_steps = {}) {
    OpenScriptStepV1 step;
    step.kind = OpenScriptStepV1::Kind::if_less;
    step.slot = slot;
    step.percent = percent;
    step.then_steps = std::move(then_steps);
    step.else_steps = std::move(else_steps);
    return step;
}

OpenScriptStepV1 summon(const char* character) {
    OpenScriptStepV1 step;
    step.kind = OpenScriptStepV1::Kind::summon;
    step.character = character;
    return step;
}

// Mirrors of data/scripts/objects/*.luac OnOpen bodies (Container Example
// Script family). Slot numbers follow the Lua locals in source order:
// 0 = calc_spawn, 1 = calc_spawnb, 2 = calc_spider.
std::vector<OpenScriptContractV1> build_contracts() {
    std::vector<OpenScriptContractV1> out;

    // moth_spawn_container: prob_spawn = 25; calc_spawn < 25 -> Summon(Swamp_Moth_Minions).
    out.push_back({"moth_spawn_container", {
        draw(0),
        if_less(0, 25, {summon("Swamp_Moth_Minions")}),
    }});

    // plant_spawn_container: the outer prob_spawn test is commented out in the
    // source, so every open draws three times and summons. All four branches
    // summon Swamp_Moth_Minions (the source differs only in its Trace text).
    out.push_back({"plant_spawn_container", {
        draw(0),
        draw(1),
        draw(2),
        if_less(2, 50,
            {if_less(1, 25, {summon("Swamp_Moth_Minions")}, {summon("Swamp_Moth_Minions")})},
            {if_less(1, 25, {summon("Swamp_Moth_Minions")}, {summon("Swamp_Moth_Minions")})}),
    }});

    // plant_spawn_container2: prob_spawn = 25 gates the plant; then a 50% branch
    // picks spider or small plant, and prob_spawnb = 20 picks the minion/big variant.
    out.push_back({"plant_spawn_container2", {
        draw(0),
        draw(1),
        if_less(0, 25, {
            draw(2),
            if_less(2, 50,
                {if_less(1, 20, {summon("EarthTemple_SmallPlantMinion")}, {summon("EarthTemple_SmallPlant")})},
                {if_less(1, 20, {summon("EarthTemple_BigSpider")}, {summon("EarthTemple_Spider")})}),
        }),
    }});

    // zombie_spawn_container: calc_spawn < 25 -> Summon(InfectedBurned).
    out.push_back({"zombie_spawn_container", {
        draw(0),
        if_less(0, 25, {summon("InfectedBurned")}),
    }});

    // container.luac (example): OnOpen calls DealDamages and actor:PlayFX only.
    // It has no Summon and no draws, so its contract is empty.
    out.push_back({"container", {}});
    return out;
}

bool run_steps(const std::vector<OpenScriptStepV1>& steps, std::array<std::int32_t, 3>& slots,
               const OpenScriptRandomFn& random, const OpenScriptSummonFn& summon,
               OpenScriptRunReportV1& report, std::string& error) {
    for (const auto& step : steps) {
        switch (step.kind) {
        case OpenScriptStepV1::Kind::draw: {
            if (step.slot < 0 || step.slot >= static_cast<std::int32_t>(slots.size())) {
                error = "Open script draw slot outside the contract's locals";
                return false;
            }
            std::int32_t value = 0;
            if (!random(0, 100, value, error)) return false;
            if (value < 0 || value > 100) {
                error = "Open script GetRand(0,100) returned outside its range";
                return false;
            }
            slots[static_cast<std::size_t>(step.slot)] = value;
            ++report.draws;
            break;
        }
        case OpenScriptStepV1::Kind::if_less: {
            if (step.slot < 0 || step.slot >= static_cast<std::int32_t>(slots.size())) {
                error = "Open script test slot outside the contract's locals";
                return false;
            }
            const bool taken = slots[static_cast<std::size_t>(step.slot)] < step.percent;
            if (!run_steps(taken ? step.then_steps : step.else_steps, slots, random, summon,
                           report, error))
                return false;
            break;
        }
        case OpenScriptStepV1::Kind::summon: {
            OpenScriptSummonRequestV1 request;
            request.character = step.character;
            request.spawn = true;
            std::string reason;
            ++report.summon_calls;
            const bool spawned = summon(request, reason);
            if (spawned) ++report.summons_spawned;
            report.summon_lines.push_back("Summon character=" + step.character + " spawn=1 " +
                                          (spawned ? std::string("outcome=spawned")
                                                   : "outcome=refused reason=" + reason));
            break;
        }
        }
    }
    return true;
}

void collect_summons(const std::vector<OpenScriptStepV1>& steps, std::set<std::string>& out) {
    for (const auto& step : steps) {
        if (step.kind == OpenScriptStepV1::Kind::summon) out.insert(step.character);
        collect_summons(step.then_steps, out);
        collect_summons(step.else_steps, out);
    }
}

} // namespace

const std::vector<OpenScriptContractV1>& open_script_contracts_v1() {
    static const std::vector<OpenScriptContractV1> contracts = build_contracts();
    return contracts;
}

const OpenScriptContractV1* find_open_script_contract_v1(const std::string& script) {
    for (const auto& contract : open_script_contracts_v1())
        if (contract.script == script) return &contract;
    return nullptr;
}

bool run_open_script_v1(const OpenScriptContractV1& contract, const OpenScriptRandomFn& random,
                        const OpenScriptSummonFn& summon, OpenScriptRunReportV1& report,
                        std::string& error) {
    error.clear();
    if (!random || !summon) {
        error = "Open script requires the GetRand and Summon services";
        return false;
    }
    // The original locals start as nil; a draw always writes its slot before a test reads it.
    std::array<std::int32_t, 3> slots{-1, -1, -1};
    return run_steps(contract.steps, slots, random, summon, report, error);
}

std::vector<std::string> open_contract_summon_characters_v1(const OpenScriptContractV1& contract) {
    std::set<std::string> names;
    collect_summons(contract.steps, names);
    return {names.begin(), names.end()};
}

std::vector<std::string> open_script_summon_characters_v1() {
    std::set<std::string> names;
    for (const auto& contract : open_script_contracts_v1()) collect_summons(contract.steps, names);
    return {names.begin(), names.end()};
}

} // namespace dh::foundation::containers
