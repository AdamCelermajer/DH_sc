#include "../character_faery_script_process_v1.hpp"

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

#undef assert
#define assert(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "assertion failed at line %d: %s\n", __LINE__, #condition); \
    return 1; \
} } while (false)

namespace process = dh2::character_faery_script_process_v1;
namespace session = dh2::character_faery_script_session_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace lifecycle = dh2::character;

struct Fixture {
    std::uintptr_t character{};
    std::uintptr_t ai{};
    std::uintptr_t ais{0x600000};
    std::uintptr_t lua{0x700000};
    std::vector<session::Operation> faery_calls;
    std::vector<std::uint32_t> source_calls;
};

static int construct(void* raw, session::Identity character,
                     session::Identity ai, session::Session& owner,
                     std::string&) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.character = character;
    fixture.ai = ai;
    owner.kind = session::ScriptKind::faery;
    owner.char_ai_owner = ai;
    owner.ais = owner.char_ai_script = fixture.ais;
    owner.lua_instance = fixture.ais + 4;
    owner.lua_state = fixture.lua;
    owner.constructed = owner.close_owned = true;
    return 0;
}

static int dispatch_faery(void* raw, const session::Request& request,
                          session::Session& owner, std::string&) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.faery_calls.push_back(request.operation);
    if (request.character != fixture.character ||
        request.char_ai_owner != fixture.ai || request.ais != fixture.ais ||
        request.lua_instance != fixture.ais + 4)
        return 1;
    switch (request.operation) {
    case session::Operation::bind_functions: owner.functions_bound = true; return 0;
    case session::Operation::bind_character:
        owner.character = fixture.character; owner.character_bound = true; return 0;
    case session::Operation::load_common: owner.common_loaded = true; return 0;
    case session::Operation::initialize: owner.initialized = true; return 0;
    case session::Operation::init_post: return 0;
    case session::Operation::init_final: return 0;
    case session::Operation::update: return 0;
    case session::Operation::close: owner = {}; return 0;
    case session::Operation::construct_faery_ais: return 1;
    }
    return 1;
}

static int source(void* raw, lifecycle::ScriptLifecycleState64* state,
                  const lifecycle::ScriptLifecycleRequest32* request,
                  lifecycle::ScriptLifecycleResponse16* response,
                  const lifecycle::ScriptLifecycleServices16* nested) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.source_calls.push_back(request->service);
    if (!state || !request || !response || !nested || !nested->invoke) return 1;
    const auto run_nested = [&](std::uint32_t operation) {
        return dh2_character_script_lifecycle(state, operation, 0, nested) == 1;
    };
    switch (request->service) {
    case lifecycle::script_ai_init:
        return run_nested(lifecycle::script_on_init) ? 0 : 1;
    case lifecycle::script_ai_init_post:
        return run_nested(lifecycle::script_on_init_post) ? 0 : 1;
    case lifecycle::script_ai_init_final:
        return run_nested(lifecycle::script_on_init_final) ? 0 : 1;
    case lifecycle::script_owner_is_dead: response->word = 0; return 0;
    case lifecycle::script_design_tick: response->word = 60; return 0;
    case lifecycle::script_timer_start:
        response->word = request->argument1 == 0x33 ? 11 : 12; return 0;
    case lifecycle::script_timer_stop:
    case lifecycle::script_refresh_vitals:
    case lifecycle::script_configure_skills:
    case lifecycle::script_update_skills:
        return 0;
    default:
        return 1;
    }
}

static void seed_record(factory::Record& record,
                        dh2::character_ai_initialization::State& ai) {
    const auto identity = reinterpret_cast<std::uintptr_t>(&record.game_object);
    record.game_object.identity = record.character.identity = identity;
    record.aggro_object.identity = identity;
    record.character.object = &record.aggro_object;
    record.constructor.identity = identity;
    record.constructor.constructor_complete = true;
    record.object_registered = record.character_listed = true;
    auto& slot = record.components.slots[
        static_cast<std::size_t>(factory::ctor::Component::ai)];
    slot.component = factory::ctor::Component::ai;
    slot.canonical_owner = &ai;
    slot.constructed = true;
    slot.character_record = &record;
    ai.identity = reinterpret_cast<std::uintptr_t>(&ai);
    ai.owner_04 = identity;
    ai.pointer_28 = 0;
    ai.word_10 = ai.word_14 = 0xffffffffu;
}

int main() {
    std::string error;
    Fixture fixture;
    factory::Record record;
    dh2::character_ai_initialization::State ai{};
    seed_record(record, ai);
    const process::Services services{
        {&fixture, &construct, &dispatch_faery}, {&fixture, &source}};
    process::Result result{};

    auto status = process::load_process(record, services, &result, error);
    if (status != process::Status::complete) {
        std::fprintf(stderr, "first load failed status=%u error=%s calls=%u last=%u\n",
            unsigned(status), error.c_str(), result.lifecycle_calls,
            result.last_lifecycle_service);
        return 1;
    }
    assert(result.session_constructed && result.faery_ais == fixture.ais);
    assert(!result.load_process_complete); // The source needs the publish pass at step 6.
    assert(ai.active_ais_1c == 0 && ai.alternate_ais_20 == fixture.ais);

    status = process::load_process(record, services, &result, error);
    if (status != process::Status::complete) {
        std::fprintf(stderr, "second load failed status=%u error=%s calls=%u last=%u\n",
            unsigned(status), error.c_str(), result.lifecycle_calls,
            result.last_lifecycle_service);
        return 1;
    }
    assert(result.load_process_complete && ai.active_ais_1c == fixture.ais);
    status = process::init_process(record, true, services, &result, error);
    if (status != process::Status::complete) {
        std::fprintf(stderr, "init failed status=%u error=%s calls=%u last=%u\n",
            unsigned(status), error.c_str(), result.lifecycle_calls,
            result.last_lifecycle_service);
        return 1;
    }
    assert(result.init_process_complete);
    assert(record.faery_script.session().init_post_called);
    assert(record.faery_script.session().init_final_called);
    assert((fixture.faery_calls == std::vector<session::Operation>{
        session::Operation::bind_functions,
        session::Operation::bind_character,
        session::Operation::load_common,
        session::Operation::initialize,
        session::Operation::init_post,
        session::Operation::init_final}));
    assert(record.canonical_ai() == &ai && ai.owner_04 == record.character.identity);
    return 0;
}
