#include "../character_faery_live_binding_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace binding = dh2::character_faery_live_binding_v1;
namespace placement = dh2::character_faery_placement_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace position = dh2::game_object_position_owner_v1;

void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "Faery live-binding regression: %s\n", message);
        std::abort();
    }
}

struct Fixture {
    std::array<int, factory::component_count> component_owners{};
    std::vector<aggro::Character*> roster;
    std::uint32_t placement_calls{};
    std::uint32_t validation_calls{};
    std::uint32_t position_updates{};
    std::uint32_t physical_updates{};
    std::uint32_t visual_syncs{};
    std::uint32_t destination_updates{};
    std::uint32_t force_updates{};
    position::Point3 placed{};
    placement::Identity faery_identity{};
    bool graph_matches{true};
};

int component(void* raw, ctor::Component component_id, ctor::Identity,
              factory::ComponentStorage::Slot* slot, std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    const auto index = static_cast<std::size_t>(component_id);
    slot->canonical_owner = &self.component_owners[index];
    return 0;
}

int associate(void*, ctor::Association, ctor::Identity,
              const factory::ComponentStorage&, std::string&) {
    return 0;
}

int register_state(void*, ctor::Identity, std::uint32_t, std::string&) {
    return 0;
}

void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
              factory::ComponentStorage&) noexcept {}

int enroll(void* raw, aggro::Character* character, bool duplicate,
           bool* appended) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!character || duplicate || !appended) return 1;
    self.roster.push_back(character);
    *appended = true;
    return 0;
}

int remove_character(void* raw, aggro::Character* character,
                     std::size_t* removed) {
    auto& roster = static_cast<Fixture*>(raw)->roster;
    *removed = 0;
    for (auto it = roster.begin(); it != roster.end();) {
        if (*it == character) {
            it = roster.erase(it);
            ++*removed;
        } else {
            ++it;
        }
    }
    return 0;
}

int init_post(void*, factory::Record&, std::string&) { return 0; }
int init_final(void*, factory::Record&, std::string&) { return 0; }
int construct_faery_script(void*, dh2::character_faery_script_session_v1::Identity,
                           dh2::character_faery_script_session_v1::Identity,
                           dh2::character_faery_script_session_v1::Session&,
                           std::string&) { return 0; }
int dispatch_faery_script(void*,
    const dh2::character_faery_script_session_v1::Request&,
    dh2::character_faery_script_session_v1::Session&, std::string&) { return 0; }

int invoke_placement(void* raw, const placement::Request& request,
                     placement::Reply* reply) {
    auto& self = *static_cast<Fixture*>(raw);
    ++self.placement_calls;
    if (!reply) return 1;
    switch (request.operation) {
    case placement::Operation::list_begin: reply->identity = 100; break;
    case placement::Operation::list_end: reply->identity = 200; break;
    case placement::Operation::list_value: reply->identity = self.faery_identity; break;
    case placement::Operation::list_next: reply->identity = 200; break;
    case placement::Operation::is_faery: reply->word = 1; break;
    case placement::Operation::look_at_vector: reply->vector = {1, 2, 3}; break;
    case placement::Operation::target_position: reply->vector = {4, 5, 6}; break;
    case placement::Operation::player_count: reply->word = 0; break;
    case placement::Operation::previous_ai_master: reply->identity = 0x702; break;
    case placement::Operation::local_player: reply->identity = 0x703; break;
    case placement::Operation::player_character: reply->identity = 0x704; break;
    case placement::Operation::current_faery_id: reply->word = 2; break;
    case placement::Operation::set_ai_master:
    case placement::Operation::change_faery: break;
    default: return 1;
    }
    return 0;
}

int translate(void* raw, position::Identity, position::Identity,
              position::Point3 delta, std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    self.placed = {self.placed.x + delta.x, self.placed.y + delta.y,
                   self.placed.z + delta.z};
    return 0;
}
int aabb(void* raw, position::Identity, std::string&) {
    ++static_cast<Fixture*>(raw)->position_updates;
    return 0;
}
int physical_set(void* raw, position::Identity, position::Identity,
                 float, float, std::string&) {
    ++static_cast<Fixture*>(raw)->physical_updates;
    return 0;
}
int visual_sync(void* raw, position::Identity, position::Identity, std::string&) {
    ++static_cast<Fixture*>(raw)->visual_syncs;
    return 0;
}
int destination(void* raw, position::Identity, position::Point3 value,
                std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    ++self.destination_updates;
    self.placed = value;
    return 0;
}
int visual_force(void* raw, position::Identity, position::Identity,
                 std::string&) {
    ++static_cast<Fixture*>(raw)->force_updates;
    return 0;
}

int validate_graph(void* raw, const factory::Record& record,
                   const position::Owner& position_owner,
                   const binding::Components& components, std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    ++self.validation_calls;
    const auto identity = record.constructor.identity;
    const auto* ai = record.components.find(ctor::Component::ai);
    const auto* game_object = record.components.find(ctor::Component::game_object);
    return self.graph_matches && identity && record.character.identity == identity &&
        record.game_object.identity == identity && position_owner.identity == identity &&
        ai == components.ais_faery && game_object && components.pofaerie &&
        position_owner.physical_object == reinterpret_cast<position::Identity>(components.physical) &&
        position_owner.visual_object == reinterpret_cast<position::Identity>(components.visual)
        ? 0 : 1;
}

int main() {
    manager::Owner objects;
    Fixture fixture;
    const factory::RosterServices roster{&fixture, &enroll, &remove_character};
    factory::Owner character_factory(objects, roster);
    const factory::Services factory_services{
        &fixture, &component, &associate, &register_state, &rollback};
    const factory::LifecycleServices lifecycle{&fixture, &init_post, &init_final};
    manager::GameObject game_object{};
    aggro::GameObject aggro_object{};
    factory::Record* character = nullptr;
    factory::Result factory_result{};
    std::string error;

    require(character_factory.create(41, game_object, aggro_object,
                factory_services, &character, &factory_result, error) ==
            factory::Status::complete, "Character constructor fixture failed");
    require(character != nullptr, "Character record missing");
    fixture.faery_identity = character->constructor.identity;

    position::Owner position_owner{};
    position_owner.identity = character->constructor.identity;
    int pofaerie = 2, physical = 3, visual = 4;
    dh2::character_faery_script_session_v1::Session faery_session{};
    faery_session.kind = dh2::character_faery_script_session_v1::ScriptKind::faery;
    const auto ais_identity = reinterpret_cast<dh2::character_faery_script_session_v1::Identity>(
        character->components.find(ctor::Component::ai));
    faery_session.character = character->constructor.identity;
    faery_session.char_ai_owner = ais_identity;
    faery_session.ais = ais_identity;
    faery_session.char_ai_script = ais_identity;
    faery_session.lua_instance = ais_identity + 0x4;
    faery_session.lua_state = ais_identity + 0x100;
    faery_session.constructed = faery_session.character_bound =
        faery_session.functions_bound = faery_session.close_owned = true;
    faery_session.common_loaded = true;
    faery_session.initialized = true;
    const dh2::character_faery_script_session_v1::Services script_services{
        &fixture, &construct_faery_script, &dispatch_faery_script};
    position_owner.physical_object = reinterpret_cast<position::Identity>(&physical);
    position_owner.visual_object = reinterpret_cast<position::Identity>(&visual);
    binding::Components components{
        character->components.find(ctor::Component::ai), &faery_session,
        script_services, &pofaerie, &physical, &visual};
    const placement::Services placement_services{&fixture, &invoke_placement};
    const position::Services position_services{&fixture, &translate, &aabb,
        &physical_set, &visual_sync, &destination, &visual_force};
    binding::Binding live_binding{character, &position_owner, position_services, components,
        placement_services, &fixture, &validate_graph};
    binding::Owner live(live_binding);
    placement::Result result{};

    // Constructor/roster publication alone is not a placeable Faery graph.
    require(live.place(0x701, &result, error) == binding::Status::graph_incomplete,
            "incomplete lifecycle graph was accepted");
    require(fixture.placement_calls == 0 && fixture.validation_calls == 0,
            "incomplete graph reached placement providers");

    require(character_factory.mark_source_properties_ready(41, &factory_result,
                error) == factory::Status::complete, "property readiness failed");
    require(character_factory.run_init_post(41, lifecycle, &factory_result,
                error) == factory::Status::complete, "InitPost fixture failed");
    require(character_factory.run_init_final(41, lifecycle, &factory_result,
                error) == factory::Status::complete, "InitFinal fixture failed");

    auto missing_session_components = components;
    missing_session_components.script_session = nullptr;
    binding::Owner missing_session({character, &position_owner, position_services,
        missing_session_components, placement_services, &fixture, &validate_graph});
    require(missing_session.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            error.find("CharAI/LuaScript session is missing") != std::string::npos &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery without its own CharAIScript/Lua session reached placement");

    auto missing_ai_components = components;
    missing_ai_components.ais_faery = nullptr;
    binding::Owner missing_ai({character, &position_owner, position_services,
        missing_ai_components, placement_services, &fixture, &validate_graph});
    require(missing_ai.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            error.find("AISFaery owner is missing") != std::string::npos &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery without its AISFaery owner reached placement");

    auto missing_pofaerie_components = components;
    missing_pofaerie_components.pofaerie = nullptr;
    binding::Owner missing_pofaerie({character, &position_owner,
        position_services, missing_pofaerie_components, placement_services,
        &fixture, &validate_graph});
    require(missing_pofaerie.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            error.find("POFaerie physical callback owner is missing") != std::string::npos &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery without its POFaerie callback owner reached placement");

    auto missing_physical_components = components;
    missing_physical_components.physical = nullptr;
    binding::Owner missing_physical({character, &position_owner,
        position_services, missing_physical_components, placement_services,
        &fixture, &validate_graph});
    require(missing_physical.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            error.find("physical owner is missing") != std::string::npos &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery without its physical owner reached placement");

    auto missing_visual_components = components;
    missing_visual_components.visual = nullptr;
    binding::Owner missing_visual({character, &position_owner,
        position_services, missing_visual_components, placement_services,
        &fixture, &validate_graph});
    require(missing_visual.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            error.find("VisualObject owner is missing") != std::string::npos &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery without its visual owner reached placement");

    auto aliased_session = faery_session;
    aliased_session.character++;
    auto aliased_session_components = components;
    aliased_session_components.script_session = &aliased_session;
    binding::Owner wrong_session({character, &position_owner, position_services,
        aliased_session_components, placement_services, &fixture, &validate_graph});
    require(wrong_session.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery session bound to a different Character reached placement");

    auto foreign_ai_session = faery_session;
    foreign_ai_session.char_ai_owner++;
    auto foreign_ai_components = components;
    foreign_ai_components.script_session = &foreign_ai_session;
    binding::Owner wrong_ai_owner({character, &position_owner,
        position_services, foreign_ai_components, placement_services,
        &fixture, &validate_graph});
    require(wrong_ai_owner.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "Faery session owned by another factory CharAI reached placement");

    auto incomplete_position_services = position_services;
    incomplete_position_services.set_destination = nullptr;
    binding::Owner missing_destination({character, &position_owner,
        incomplete_position_services, components, placement_services,
        &fixture, &validate_graph});
    require(missing_destination.place(0x701, &result, error) ==
                binding::Status::graph_incomplete &&
            fixture.validation_calls == 0 && fixture.placement_calls == 0,
            "missing source destination provider reached graph validation/placement");

    // The component validator rejects a Faery script/visual pair that does not
    // belong to this Character before the source placement loop is entered.
    fixture.graph_matches = false;
    require(live.place(0x701, &result, error) == binding::Status::graph_stale,
            "mismatched Character components were accepted");
    require(fixture.placement_calls == 0 && fixture.validation_calls == 1,
            "mismatched graph reached placement providers");

    fixture.graph_matches = true;
    const auto ready_status = live.place(0x701, &result, error);
    if (ready_status != binding::Status::complete)
        std::fprintf(stderr, "ready status=%u error=%s validation=%u placement=%u\n",
            static_cast<unsigned>(ready_status), error.c_str(),
            fixture.validation_calls, fixture.placement_calls);
    require(ready_status == binding::Status::complete,
            "complete source graph did not reach placement");
    require(fixture.validation_calls == 2 && fixture.placement_calls == 16 &&
            fixture.position_updates == 1 && fixture.physical_updates == 1 &&
            fixture.visual_syncs == 1 && fixture.destination_updates == 1 &&
            fixture.force_updates == 1 &&
            result.visited == 1 && result.placed_faeries == 1 &&
            result.callbacks == 18 &&
            position_owner.position.x == 5 && position_owner.position.y == 7 &&
            position_owner.position.z == 9,
            "ready graph did not place the source Faery through GameObject providers");

    std::puts("{\"incomplete_graph_rejected\":true,\"mismatched_graph_rejected\":true,\"ready_faery_placed_once\":true,\"source_identity_preserved\":true}");
    return 0;
}
