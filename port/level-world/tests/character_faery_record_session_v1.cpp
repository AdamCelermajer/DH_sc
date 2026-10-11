#include "../character_faery_record_session_v1.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace record_session = dh2::character_faery_record_session_v1;
namespace session = dh2::character_faery_script_session_v1;
namespace factory = dh2::character_runtime_factory_v1;

struct Fixture {
    session::Identity character{};
    session::Identity ai_owner{};
    session::Identity ais{0x600000};
    session::Identity lua_state{0x700000};
    std::vector<session::Operation> operations;
    bool fail_construction{};
};

static int construct(void* raw, session::Identity character,
                     session::Identity ai, session::Session& owner,
                     std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    if (f.fail_construction) {
        error = "source AISFaery provider unavailable";
        return 1;
    }
    f.character = character;
    f.ai_owner = ai;
    owner.kind = session::ScriptKind::faery;
    owner.char_ai_owner = ai;
    owner.ais = owner.char_ai_script = f.ais;
    owner.lua_instance = f.ais + 4;
    owner.lua_state = f.lua_state;
    owner.constructed = owner.close_owned = true;
    return 0;
}

static int dispatch(void* raw, const session::Request& request,
                    session::Session& owner, std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    f.operations.push_back(request.operation);
    if (request.character != f.character ||
        request.char_ai_owner != owner.char_ai_owner ||
        request.ais != owner.ais || request.lua_instance != owner.ais + 4)
        return 1;
    switch (request.operation) {
    case session::Operation::bind_functions:
        owner.functions_bound = true; return 0;
    case session::Operation::bind_character:
        owner.character = request.character;
        owner.character_bound = true; return 0;
    case session::Operation::load_common:
        owner.common_loaded = true; return 0;
    case session::Operation::initialize:
        owner.initialized = true; return 0;
    case session::Operation::init_post:
    case session::Operation::init_final:
        return owner.initialized ? 0 : 1;
    case session::Operation::update:
        return owner.initialized ? 0 : 1;
    case session::Operation::close:
        owner = {}; return 0;
    case session::Operation::construct_faery_ais:
        return 1;
    }
    return 1;
}

static void make_record(factory::Record& record, int& ai_owner) {
    const auto identity = reinterpret_cast<factory::ctor::Identity>(
        &record.game_object);
    record.constructor.identity = identity;
    record.character.identity = identity;
    auto& slot = record.components.slots[
        static_cast<std::size_t>(factory::ctor::Component::ai)];
    slot.component = factory::ctor::Component::ai;
    slot.canonical_owner = &ai_owner;
    slot.constructed = true;
}

int main() {
    std::string error;
    int ai_owner = 10;
    factory::Record record;
    make_record(record, ai_owner);
    Fixture fixture;
    const session::Services services{&fixture, &construct, &dispatch};

    assert(record_session::construct(record, services, error) ==
           record_session::Status::complete);
    assert(record.faery_script.session().character == 0);
    assert(record.faery_script.matches(record.character.identity,
           reinterpret_cast<session::Identity>(&ai_owner)));
    assert(record.faery_script.session().char_ai_owner ==
           reinterpret_cast<session::Identity>(&ai_owner));
    assert(record_session::bind_functions(record, error) ==
           record_session::Status::complete);
    assert(record_session::bind_character(record, error) ==
           record_session::Status::complete);
    assert(record_session::load_common(record, error) ==
           record_session::Status::complete);
    assert(record_session::initialize(record, error) ==
           record_session::Status::complete);
    assert(record_session::init_post(record, error) ==
           record_session::Status::complete);
    assert(record_session::init_final(record, error) ==
           record_session::Status::complete);
    assert(record_session::init_final(record, error) ==
           record_session::Status::session_rejected);

    // A different factory AI component cannot update this Character's AIS.
    auto& ai_slot = record.components.slots[
        static_cast<std::size_t>(factory::ctor::Component::ai)];
    int replacement_ai = 11;
    ai_slot.canonical_owner = &replacement_ai;
    assert(record_session::update(record, error) ==
           record_session::Status::identity_mismatch);
    ai_slot.canonical_owner = &ai_owner;
    assert(record_session::update(record, error) ==
           record_session::Status::complete);
    assert(record_session::close(record, error) ==
           record_session::Status::complete);
    assert(record.faery_script.stage() == session::Stage::closed);
    assert((fixture.operations == std::vector<session::Operation>{
        session::Operation::bind_functions,
        session::Operation::bind_character,
        session::Operation::load_common,
        session::Operation::initialize,
        session::Operation::init_post,
        session::Operation::init_final,
        session::Operation::init_final,
        session::Operation::update,
        session::Operation::close}));

    int ai_two = 12;
    factory::Record incomplete;
    make_record(incomplete, ai_two);
    const auto ai_index = static_cast<std::size_t>(factory::ctor::Component::ai);
    incomplete.components.slots[ai_index].constructed = false;
    assert(record_session::construct(incomplete, services, error) ==
           record_session::Status::incomplete_record);

    int ai_three = 13;
    factory::Record unavailable;
    make_record(unavailable, ai_three);
    Fixture missing;
    missing.fail_construction = true;
    assert(record_session::construct(unavailable,
           {&missing, &construct, &dispatch}, error) ==
           record_session::Status::session_rejected);
    assert(unavailable.faery_script.stage() == session::Stage::faulted);
    assert(record_session::close(unavailable, error) ==
           record_session::Status::complete);
    assert(unavailable.faery_script.stage() == session::Stage::closed);
    return 0;
}
