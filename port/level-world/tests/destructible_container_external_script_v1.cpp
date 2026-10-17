#include "../destructible_container_external_script_v1.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

namespace owner = dh2::destructible_container_external_script_v1;

struct Fixture {
    std::uint32_t draw{};
    std::uint32_t random_calls{};
    std::uint32_t py_oid_calls{};
    std::uint32_t summon_calls{};
    std::uintptr_t caller{};
    std::uintptr_t opener{};
    std::int32_t py_oid{73};
    bool reject_summon{};
};

static bool get_random(void* raw, std::uint32_t bound, std::int32_t& value,
                       std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.random_calls;
    if (!bound) { value = 0; error.clear(); return true; }
    value = static_cast<std::int32_t>(f.draw % bound);
    error.clear();
    return true;
}

static bool get_py_oid(void* raw, const char* category, const char* member,
                       std::int32_t& value, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.py_oid_calls;
    if (!category || std::strcmp(category, "CharacterTable") || !member ||
        std::strcmp(member, "Swamp_Moth_Minions")) {
        error = "wrong source PyOID request";
        return false;
    }
    value = f.py_oid;
    error.clear();
    return true;
}

static bool summon(void* raw, std::uintptr_t caller,
                   const dh2_script_value* args, std::uint32_t count,
                   owner::SummonResult& result, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.summon_calls;
    f.caller = caller;
    if (f.reject_summon) { error = "canonical NPC factory missing"; return false; }
    if (count != 6 || !args || args[0].type != DH2_SCRIPT_NUMBER ||
        static_cast<std::int32_t>(args[0].number) != f.py_oid ||
        args[1].type != DH2_SCRIPT_BOOLEAN || !args[1].boolean ||
        args[2].type != DH2_SCRIPT_NUMBER || args[2].number != 0.0f ||
        args[3].type != DH2_SCRIPT_NUMBER || args[3].number != 0.0f ||
        args[4].type != DH2_SCRIPT_NUMBER || args[4].number != 0.0f ||
        args[5].type != DH2_SCRIPT_BOOLEAN || !args[5].boolean) {
        error = "moth_spawn_container Summon arguments changed";
        return false;
    }
    result.returned_object = true;
    result.identity = 0x100000021ULL;
    error.clear();
    return true;
}

static void require(bool value, const char* message) {
    if (!value) throw message;
}

int main() {
    try {
        static constexpr char script[] =
            "function OnOpen(this)\n"
            "  assert(this == nil or type(this) == 'userdata')\n"
            "  local roll = GetRand(0, 100)\n"
            "  Trace('calc_spawn random 100: ' .. roll)\n"
            "  if roll < 25 then\n"
            "    Trace('Summoning a Moth out of the jar...')\n"
            "    Summon(GetPyOID('CharacterTable','Swamp_Moth_Minions'), true, 0, 0, 0, true)\n"
            "  end\n"
            "end\n";
        Fixture fixture{};
        fixture.draw = 20;
        fixture.opener = 0x200000002ULL;
        const owner::Services services{&fixture, get_random, get_py_oid, summon};
        owner::Owner session;
        std::string error;
        require(session.initialize({script, sizeof(script) - 1,
                                    "data/scripts/objects/moth_spawn_container.luac"},
                0x300000003ULL, services, error) == owner::Status::complete,
                error.c_str());
        require(session.ready() && session.caller_identity() == 0x300000003ULL &&
                session.vm_identity() != 0, "per-GameObject VM identity was not retained");
        require(session.on_open(fixture.opener, error) == owner::Status::complete,
                error.c_str());
        require(fixture.random_calls == 1 && fixture.py_oid_calls == 1 &&
                fixture.summon_calls == 1 && fixture.caller == 0x300000003ULL,
                "OnOpen did not call the source GetRand/GetPyOID/Summon chain");

        fixture.draw = 80;
        require(session.on_open(0, error) == owner::Status::complete, error.c_str());
        require(fixture.random_calls == 2 && fixture.py_oid_calls == 1 &&
                fixture.summon_calls == 1,
                "false source probability branch executed Summon");

        fixture.draw = 1;
        fixture.reject_summon = true;
        require(session.on_open(0, error) == owner::Status::callback_failed &&
                error.find("canonical NPC factory missing") != std::string::npos,
                "missing canonical factory did not fail at the reached Summon call");
        require(fixture.summon_calls == 2,
                "failed canonical Summon callback was not reached exactly once");

        require(session.reset(error) == owner::Status::complete && !session.ready(),
                "per-object VM did not reset cleanly");
        std::puts("PASS per-GameObject LuaScript OnOpen, source random/PyOID calls, Summon forwarding, and required-factory failure");
        return 0;
    } catch (const char* message) {
        std::fprintf(stderr, "container ExternalScript audit: %s\n", message);
        return 1;
    } catch (...) {
        std::fprintf(stderr, "container ExternalScript audit failed\n");
        return 1;
    }
}
