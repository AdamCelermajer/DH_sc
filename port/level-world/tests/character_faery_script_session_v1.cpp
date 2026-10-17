#include "../character_faery_script_session_v1.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace session = dh2::character_faery_script_session_v1;

struct Fixture {
    session::Identity requested_character{};
    session::Identity char_ai_owner{0x400000};
    session::Identity ais{0x100000};
    session::Identity lua_state{0x200000};
    std::vector<session::Operation> operations;
    bool corrupt_character{};
    bool fail_update{};
    unsigned constructions{};
};

static int construct(void* raw, session::Identity character,
                     session::Identity char_ai_owner,
                     session::Session& owner, std::string&) {
    auto& fixture = *static_cast<Fixture*>(raw);
    ++fixture.constructions;
    fixture.requested_character = character;
    fixture.char_ai_owner = char_ai_owner;
    owner.kind = session::ScriptKind::faery;
    owner.char_ai_owner = char_ai_owner;
    owner.ais = fixture.ais;
    owner.char_ai_script = fixture.ais;
    // LuaScript's Instance subobject is constructed at AIS+4 by the
    // CharAIScript base constructor, before StepBindFunction/SetCharacter.
    owner.lua_instance = fixture.ais + 0x4;
    owner.lua_state = fixture.lua_state;
    owner.constructed = true;
    owner.close_owned = true;
    return 0;
}

static int dispatch(void* raw, const session::Request& request,
                    session::Session& owner, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.operations.push_back(request.operation);
    if (request.char_ai_owner != fixture.char_ai_owner ||
        request.ais != fixture.ais || request.char_ai_script != fixture.ais) {
        error = "AIS/CharAIScript identity changed";
        return 1;
    }
    switch (request.operation) {
    case session::Operation::bind_functions:
        if (request.character != fixture.requested_character || owner.character_bound ||
            request.lua_instance != fixture.ais + 0x4) return 1;
        owner.functions_bound = true;
        return 0;
    case session::Operation::bind_character:
        if (request.character != fixture.requested_character) return 1;
        owner.character = fixture.corrupt_character
            ? request.character + 1 : request.character;
        owner.character_bound = true;
        return 0;
    case session::Operation::load_common:
        if (!owner.character_bound || !owner.functions_bound ||
            request.lua_instance != fixture.ais + 0x4) return 1;
        owner.common_loaded = true;
        return 0;
    case session::Operation::initialize:
        if (!owner.functions_bound || !owner.common_loaded) return 1;
        owner.initialized = true;
        return 0;
    case session::Operation::update:
        if (fixture.fail_update) {
            error = "simulated source callback failure";
            return 1;
        }
        if (!owner.initialized || request.lua_instance != owner.ais + 0x4)
            return 1;
        return 0;
    case session::Operation::close:
        owner = {};
        return 0;
    case session::Operation::construct_faery_ais:
        return 1;
    }
    return 1;
}

static session::Services services(Fixture& fixture) {
    return {&fixture, &construct, &dispatch};
}

int main() {
    std::string error;
    Fixture fixture;
    session::Owner owner;
    assert(owner.construct(0x300000, fixture.char_ai_owner,
                           services(fixture), error) ==
           session::Status::complete);
    assert(owner.stage() == session::Stage::ais_constructed);
    assert(owner.session().lua_instance == fixture.ais + 0x4);
    assert(owner.bind_character(error) == session::Status::wrong_stage);
    assert(owner.bind_functions(error) == session::Status::complete);
    assert(owner.bind_character(error) == session::Status::complete);
    assert(owner.session().lua_state == fixture.lua_state);
    assert(owner.bind_functions(error) == session::Status::wrong_stage);
    assert(owner.initialize(error) == session::Status::wrong_stage);
    assert(owner.load_common(error) == session::Status::complete);
    assert(owner.initialize(error) == session::Status::complete);
    assert(session::ready(owner.session(), 0x300000, fixture.char_ai_owner,
                          fixture.ais,
                          services(fixture)));
    assert(!session::ready(owner.session(), 0x300000,
                           fixture.char_ai_owner + 1, fixture.ais,
                           services(fixture)));
    assert(owner.update(error) == session::Status::complete);
    assert(owner.update_count() == 1);
    assert(owner.session().lua_state == fixture.lua_state);
    assert(owner.close(error) == session::Status::complete);
    assert(owner.stage() == session::Stage::closed);
    assert(fixture.constructions == 1);
    assert((fixture.operations == std::vector<session::Operation>{
        session::Operation::bind_functions,
        session::Operation::bind_character,
        session::Operation::load_common,
        session::Operation::initialize,
        session::Operation::update,
        session::Operation::close}));

    Fixture mismatch;
    mismatch.corrupt_character = true;
    session::Owner invalid;
    assert(invalid.construct(0x300001, mismatch.char_ai_owner,
                             services(mismatch), error) ==
           session::Status::complete);
    assert(invalid.bind_functions(error) == session::Status::complete);
    assert(invalid.bind_character(error) == session::Status::identity_mismatch);
    assert(invalid.stage() == session::Stage::faulted);
    assert(invalid.close(error) == session::Status::complete);

    Fixture failed_update;
    failed_update.fail_update = true;
    session::Owner faulted;
    assert(faulted.construct(0x300002, failed_update.char_ai_owner,
                             services(failed_update), error) ==
           session::Status::complete);
    assert(faulted.bind_functions(error) == session::Status::complete);
    assert(faulted.bind_character(error) == session::Status::complete);
    assert(faulted.load_common(error) == session::Status::complete);
    assert(faulted.initialize(error) == session::Status::complete);
    assert(faulted.update(error) == session::Status::provider_failed);
    assert(faulted.update_count() == 0);
    assert(faulted.close(error) == session::Status::complete);

    session::Owner no_provider;
    assert(no_provider.construct(0x300003, 0x400003, {}, error) ==
           session::Status::invalid_argument);
    assert(no_provider.stage() == session::Stage::empty);
    return 0;
}
