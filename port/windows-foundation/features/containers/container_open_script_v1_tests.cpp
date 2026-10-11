// P16 CONTAINERS2 (T6) / v20 GENERAL: OnOpen of authored containers runs the authored Lua text.
// Fixtures are verbatim copies of data/scripts/objects/moth_spawn_container.luac and container.luac (LF line
// endings) plus one synthetic script that names other rows. Fake GetRand and Summon owners; no assets, no EXE.

#include "container_open_script_v1.hpp"

#include <cstdio>
#include <deque>
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

// moth_spawn_container.luac (verbatim, line endings normalized).
const char* const moth_source = R"LUA(--[[	Container Example Script

	Example of what a container script would looks like.

--]]


-- ----------------------------------------------------------------------------------------------
-- DATA
-- ----------------------------------------------------------------------------------------------
-- Constants
-- Chance in % to spawn a moth when destroying an object
local prob_spawn = 25;


-- Cached values
local calc_spawn;
-- ----------------------------------------------------------------------------------------------


-- 0 : actor represents what triggered the container to get opened
function OnOpen(actor)

calc_spawn = GetRand(0,100);

Trace("calc_spawn random 100: "..calc_spawn);

if(calc_spawn < prob_spawn) then
	Trace("Summoning a Moth out of the jar...");
	calc_spawn = -1;
	Summon(GetPyOID("CharacterTable", "Swamp_Moth_Minions"), true, 0.0, 0.0, 0.0, true);
end

end

-- 0 : event name
function OnAnimEvent(event)
end
)LUA";

// container.luac (verbatim): the opened branch damages the actor, the miraculous branch only traces.
const char* const container_source = R"LUA(--[[	Container Example Script

	Example of what a container script would looks like.

--]]


-- 0 : actor represents what triggered the container to get opened
function OnOpen(actor)
	if(actor) then
		Trace(GetName().." was opened by "..actor:GetName());

		local dmg = DealDamages(actor, GetPyOID("GameObjectDamager", "Level01Damage"), GetPyCst("Elemental", "Fire"));
	else
		Trace(GetName().." was opened miraculously");
	end
end
)LUA";

// Synthetic: different rows, a different draw bound and a non-relative offset; nothing is keyed by name.
const char* const other_source = R"LUA(
local prob = 60;
function OnOpen(actor)
	local roll = GetRand(0,100);
	if roll < prob then
		Summon(GetPyOID("CharacterTable", "Other_Row"), true, 1.0, 2.0, 3.0, false);
	end
	if roll < 10 then
		Summon(GetPyOID("CharacterTable", "Swamp_Moth_Minions"), false, 0.0, 0.0, 0.0, true);
	end
end
)LUA";

const std::vector<std::string> rows = {"Swamp_Moth_Minions", "InfectedBurned", "Other_Row"};

// Draws come from a script; an empty queue fails the draw.
struct ScriptedRandom {
    std::deque<std::int32_t> values;
    std::size_t calls = 0;
    bool operator()(std::int32_t lo, std::int32_t hi, std::int32_t& value, std::string& error) {
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

struct FakeSummon {
    std::vector<OpenScriptSummonRequestV1> requests;
    bool spawn_result = true;
    bool operator()(const OpenScriptSummonRequestV1& request, std::string& reason) {
        requests.push_back(request);
        if (!spawn_result) reason = "no pool slot";
        return spawn_result;
    }
};

OpenScriptServicesV1 services(ScriptedRandom& random, FakeSummon& summon) {
    OpenScriptServicesV1 out;
    out.random = [&random](std::int32_t lo, std::int32_t hi, std::int32_t& v, std::string& e) { return random(lo, hi, v, e); };
    out.summon = [&summon](const OpenScriptSummonRequestV1& r, std::string& reason) { return summon(r, reason); };
    out.character_rows = &rows;
    out.object_name = "Swamp_Container_01";
    return out;
}

} // namespace

int main() {
    std::string error;

    // Moth: draw 10 < 25 -> one Summon of the authored row, spawn=true, offset 0 relative.
    {
        ScriptedRandom random{{10}};
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        check(run_authored_open_script_v1(moth_source, "moth_spawn_container", services(random, summon), report, error),
              "moth open runs");
        check(report.draws == 1 && random.calls == 1, "moth draws once");
        check(summon.requests.size() == 1, "moth summons once at draw 10");
        if (!summon.requests.empty()) {
            check(summon.requests[0].character == "Swamp_Moth_Minions", "moth summons the authored row");
            check(summon.requests[0].spawn && summon.requests[0].relative, "moth summon flags");
            check(summon.requests[0].offset[0] == 0.0f && summon.requests[0].offset[2] == 0.0f, "moth offset");
        }
        check(report.summons_spawned == 1, "moth summon spawned");
        check(report.trace_lines.size() == 2 && report.trace_lines[1] == "Summoning a Moth out of the jar...",
              "moth trace lines in order");
    }

    // Boundary: draw 25 is not < 25 -> no summon.
    {
        ScriptedRandom random{{25}};
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        check(run_authored_open_script_v1(moth_source, "moth_spawn_container", services(random, summon), report, error),
              "moth boundary runs");
        check(summon.requests.empty() && report.summon_calls == 0, "moth boundary draws no summon");
    }

    // Draw 0 summons (lower bound).
    {
        ScriptedRandom random{{0}};
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        check(run_authored_open_script_v1(moth_source, "moth_spawn_container", services(random, summon), report, error) &&
              summon.requests.size() == 1, "moth draw 0 summons");
    }

    // A refused summon is a normal outcome: the script continues and the report says so.
    {
        ScriptedRandom random{{3}};
        FakeSummon summon;
        summon.spawn_result = false;
        OpenScriptRunReportV1 report;
        check(run_authored_open_script_v1(moth_source, "moth_spawn_container", services(random, summon), report, error),
              "refused summon is not an error");
        check(report.summon_calls == 1 && report.summons_spawned == 0, "refused summon counted");
        check(!report.summon_lines.empty() &&
              report.summon_lines[0].find("refused reason=no pool slot") != std::string::npos,
              "refused summon line");
    }

    // container.luac without an actor: the miraculous branch, no summon, no draw.
    {
        ScriptedRandom random;
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        check(run_authored_open_script_v1(container_source, "container", services(random, summon), report, error),
              "container runs with nil actor");
        check(summon.requests.empty() && report.draws == 0, "container draws and summons nothing");
        check(report.trace_lines.size() == 1 && report.trace_lines[0] == "Swamp_Container_01 was opened miraculously",
              "container trace uses the object name");
    }

    // Synthetic script: other rows and offsets come from the script, both summons run.
    {
        ScriptedRandom random{{5}};
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        check(run_authored_open_script_v1(other_source, "synthetic", services(random, summon), report, error),
              "synthetic script runs");
        check(summon.requests.size() == 2, "synthetic script summons twice at draw 5");
        if (summon.requests.size() == 2) {
            check(summon.requests[0].character == "Other_Row" && !summon.requests[0].relative &&
                  summon.requests[0].offset[1] == 2.0f, "synthetic first summon arguments");
            check(summon.requests[1].character == "Swamp_Moth_Minions" && !summon.requests[1].spawn,
                  "synthetic second summon arguments");
        }
    }

    // Fail closed: a random failure, an unknown row, and an unbound game call each fail the open.
    {
        ScriptedRandom random;
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        check(!run_authored_open_script_v1(moth_source, "moth_spawn_container", services(random, summon), report, error),
              "random failure fails the open");
        check(error.find("GetRand failed") != std::string::npos, "random failure text");
    }
    {
        ScriptedRandom random{{1}};
        FakeSummon summon;
        OpenScriptServicesV1 s = services(random, summon);
        const std::vector<std::string> short_rows = {"InfectedBurned"};
        s.character_rows = &short_rows;
        OpenScriptRunReportV1 report;
        check(!run_authored_open_script_v1(moth_source, "moth_spawn_container", s, report, error),
              "unknown row fails the open");
        check(error.find("Swamp_Moth_Minions") != std::string::npos, "unknown row text");
        check(summon.requests.empty(), "unknown row never summons");
    }
    {
        // container.luac's damage branch needs actor/damage services that are not bound: the open fails.
        ScriptedRandom random;
        FakeSummon summon;
        OpenScriptRunReportV1 report;
        const char* const damaging = "function OnOpen(actor) DealDamages(actor, 1, 2) end";
        check(!run_authored_open_script_v1(damaging, "damaging", services(random, summon), report, error),
              "unbound game call fails the open");
        check(!error.empty(), "unbound game call error text");
    }

    // Reservation scan: literal Summon rows of the authored source; a commented-out summon does not count.
    {
        const auto moth_rows = authored_open_summon_rows_v1(moth_source);
        check(moth_rows.size() == 1 && moth_rows[0] == "Swamp_Moth_Minions", "moth reservation rows");
        check(authored_open_summon_rows_v1(container_source).empty(), "container reserves nothing");
        const auto other_rows = authored_open_summon_rows_v1(other_source);
        check(other_rows.size() == 2, "synthetic reservation rows");
        check(authored_open_summon_rows_v1("-- Summon(GetPyOID(\"CharacterTable\", \"Ghost\"), true)\n").empty(),
              "commented summon is not reserved");
    }

    if (failures == 0) std::printf("container_open_script_v1 tests passed\n");
    return failures == 0 ? 0 : 1;
}
