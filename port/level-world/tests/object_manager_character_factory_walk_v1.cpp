#include "../object_manager_character_factory_walk_v1.hpp"
#include "../character_factory_role_query_v1.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace walk = dh2::object_manager_character_factory_walk_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace classification = dh2::character_ai_classification;

struct Fixture {
    std::uintptr_t component_storage{};
    std::vector<manager::SourceHandle> visits;
    bool fail_visit{};
    std::int32_t type{};
    std::uint32_t classification_calls{};
    bool mismatched_capture{};
    classification::AiRow row{};
    classification::AiTable table{};
};

static int component(void* raw, ctor::Component id, ctor::Identity,
                     factory::ComponentStorage::Slot* slot, std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    slot->component = id;
    slot->canonical_owner = &f.component_storage;
    return 0;
}
static int associate(void*, ctor::Association, ctor::Identity,
                     const factory::ComponentStorage&, std::string&) { return 0; }
static int register_state(void*, ctor::Identity, std::uint32_t,
                          std::string&) { return 0; }
static void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
                     factory::ComponentStorage&) noexcept {}
static int enroll(void*, aggro::Character*, bool duplicate, bool* appended) {
    *appended = !duplicate;
    return 0;
}
static int remove_character(void*, aggro::Character*, std::size_t* removed) {
    *removed = 1;
    return 0;
}
static int visit(void* raw, const factory::Record& record, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    f.visits.push_back(record.source_handle);
    if (f.fail_visit) {
        error = "injected visitor failure";
        return 1;
    }
    return 0;
}
static int classification_call(void* raw, classification::State*,
                               const classification::Request* request,
                               classification::Response* response) {
    auto& f = *static_cast<Fixture*>(raw);
    ++f.classification_calls;
    if (request->operation == classification::Operation::ai_count) {
        response->count = 1;
        return 0;
    }
    if (request->operation == classification::Operation::ai_table) {
        f.row.type = f.type;
        f.table = {&f.row, 1};
        response->table = &f.table;
        return 0;
    }
    if (request->operation == classification::Operation::find_player_name) {
        response->match = f.mismatched_capture ? request->name + 1 : request->name;
        return 0;
    }
    return 1;
}
static int capture_classification(void* raw, const factory::Record& record,
                                  classification::State* state,
                                  classification::Services* services,
                                  std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    state->character = f.mismatched_capture ? record.character.identity + 1
                                             : record.character.identity;
    state->ai_id = 0;
    state->faction_id = 0;
    state->name = "PlayerCharacter_01";
    services->context = &f;
    services->invoke = classification_call;
    return 0;
}

int main() {
    manager::Owner objects;
    Fixture fixture;
    factory::Owner characters(objects, {nullptr, enroll, remove_character});
    const factory::Services providers{&fixture, component, associate,
                                      register_state, rollback};
    factory::Result created{};
    std::string error;
    factory::Record* high = nullptr;
    factory::Record* low = nullptr;
    factory::Record* middle = nullptr;
    const manager::GameObject seed{};
    const aggro::GameObject aggro_seed{};
    assert(characters.create(20, seed, aggro_seed, providers, &high, &created,
                             error) == factory::Status::complete);
    assert(characters.create(3, seed, aggro_seed, providers, &low, &created,
                             error) == factory::Status::complete);
    assert(characters.create(11, seed, aggro_seed, providers, &middle, &created,
                             error) == factory::Status::complete);
    manager::GameObject ordinary{};
    ordinary.identity = 0xfeed;
    manager::GameObject* ordinary_stored = nullptr;
    assert(objects.add_object(7, ordinary, &ordinary_stored) == manager::Status::ok);

    walk::Result result{};
    const walk::Services services{&fixture, visit};
    assert(walk::walk(objects, characters, services, &result, error) ==
           walk::Status::complete);
    assert(result.manager_rows == 4 && result.characters == 3);
    assert((fixture.visits == std::vector<manager::SourceHandle>{3, 11, 20}));
    assert(fixture.visits[0] == low->source_handle &&
           fixture.visits[1] == middle->source_handle &&
           fixture.visits[2] == high->source_handle);
    assert(low->character.identity ==
           objects.find_by_source_handle(3)->identity);

    const dh2::character_factory_role_query_v1::Services role_services{
        &fixture, capture_classification};
    dh2::character_factory_role_query_v1::Result role_result{};
    fixture.type = 0;
    assert(dh2::character_factory_role_query_v1::query(*low,
        dh2::character_factory_role_query_v1::Role::player, role_services,
        &role_result, error) ==
        dh2::character_factory_role_query_v1::Status::complete);
    assert(role_result.value && role_result.character_identity == low->character.identity &&
           role_result.source_calls == 3); // type read + exact name-prefix import
    fixture.type = 7;
    assert(dh2::character_factory_role_query_v1::query(*middle,
        dh2::character_factory_role_query_v1::Role::merchant, role_services,
        &role_result, error) ==
        dh2::character_factory_role_query_v1::Status::complete);
    assert(role_result.value && role_result.character_identity == middle->character.identity &&
           role_result.source_calls == 2); // type 7 is source IsMerchant
    fixture.mismatched_capture = true;
    const auto calls_before_mismatch = fixture.classification_calls;
    assert(dh2::character_factory_role_query_v1::query(*high,
        dh2::character_factory_role_query_v1::Role::player, role_services,
        &role_result, error) ==
        dh2::character_factory_role_query_v1::Status::identity_mismatch);
    assert(fixture.classification_calls == calls_before_mismatch);
    fixture.mismatched_capture = false;

    fixture.visits.clear();
    fixture.fail_visit = true;
    assert(walk::walk(objects, characters, services, &result, error) ==
           walk::Status::callback_failed);
    assert(result.characters == 0 && error == "injected visitor failure" &&
           fixture.visits == std::vector<manager::SourceHandle>{3});
    std::puts("{\"manager_order\":true,\"factory_identity\":true,\"noncharacter_skipped\":true,\"visitor_failure_propagated\":true,\"player_prefix_predicate\":true,\"merchant_type_predicate\":true,\"role_identity_gate\":true}");
}
