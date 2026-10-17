#include "../character_faery_level_registry_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace registry = dh2::character_faery_level_registry_v1;
namespace binding = dh2::character_faery_live_binding_v1;
namespace placement = dh2::character_faery_placement_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace position = dh2::game_object_position_owner_v1;

void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "Faery Level registry regression: %s\n", message);
        std::abort();
    }
}

struct Fixture {
    std::array<int, factory::component_count> components{};
    std::uint32_t placement_effects{};
    std::uint32_t faery_links{};
    std::uint32_t master_writes{};
    std::uint32_t zoning_disables{};
    std::uintptr_t first_master{};
    std::uintptr_t restored_master{};
    std::uintptr_t changed_character{};
    std::int32_t changed_faery{};
    std::uint32_t position_updates{};
    std::uint32_t physical_updates{};
    std::uint32_t visual_updates{};
    std::uint32_t destination_updates{};
    std::uint32_t force_updates{};
    position::Point3 position{};
    placement::Identity actor{};
    placement::Identity other_actor{0xdead};
    std::uint32_t list_size{1};
    bool is_faery{true};
};

int component(void* raw, ctor::Component id, ctor::Identity,
              factory::ComponentStorage::Slot* slot, std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    slot->canonical_owner = &self.components[static_cast<std::size_t>(id)];
    return 0;
}
int associate(void*, ctor::Association, ctor::Identity,
              const factory::ComponentStorage&, std::string&) { return 0; }
int register_state(void*, ctor::Identity, std::uint32_t, std::string&) { return 0; }
void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
              factory::ComponentStorage&) noexcept {}
int enroll(void*, dh2::character::aggro_search::Character*, bool, bool* appended) {
    *appended = true;
    return 0;
}
int remove_character(void*, dh2::character::aggro_search::Character*,
                     std::size_t* removed) { *removed = 0; return 0; }
int lifecycle(void*, factory::Record&, std::string&) { return 0; }
int script_construct(void*, dh2::character_faery_script_session_v1::Identity,
    dh2::character_faery_script_session_v1::Identity,
    dh2::character_faery_script_session_v1::Session&, std::string&) { return 0; }
int script_dispatch(void*, const dh2::character_faery_script_session_v1::Request&,
                    dh2::character_faery_script_session_v1::Session&,
                    std::string&) { return 0; }

int level(void* raw, const placement::Request& request, placement::Reply* reply) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!reply) return 1;
    switch (request.operation) {
    case placement::Operation::list_begin: reply->identity = 100; break;
    case placement::Operation::list_end: reply->identity = 200; break;
    case placement::Operation::list_value:
        reply->identity = request.subject == 101 ? self.other_actor : self.actor;
        break;
    case placement::Operation::list_next:
        reply->identity = request.subject == 100 && self.list_size > 1 ? 101 : 200;
        break;
    case placement::Operation::is_faery: reply->word = self.is_faery ? 1 : 0; break;
    case placement::Operation::is_follower: reply->word = self.is_faery ? 0 : 1; break;
    case placement::Operation::look_at_vector: reply->vector = {1, 2, 3}; break;
    case placement::Operation::target_position: reply->vector = {4, 5, 6}; break;
    case placement::Operation::player_count: reply->word = 1; break;
    case placement::Operation::player_at: reply->identity = 0x705; break;
    case placement::Operation::previous_ai_master: reply->identity = 0x702; break;
    case placement::Operation::local_player: reply->identity = 0x703; break;
    case placement::Operation::player_character:
        reply->identity = request.subject == 0x705 ? 0x706 : 0x704;
        break;
    case placement::Operation::current_faery_id: reply->word = 2; break;
    case placement::Operation::set_player_faery:
        ++self.faery_links;
        if (request.subject != 0x706 || request.argument != self.actor) return 1;
        break;
    case placement::Operation::set_ai_master:
        if (self.master_writes++ == 0) self.first_master = request.argument;
        else self.restored_master = request.argument;
        ++self.placement_effects;
        break;
    case placement::Operation::change_faery:
        self.changed_character = request.subject;
        self.changed_faery = request.value;
        ++self.placement_effects;
        break;
    case placement::Operation::disable_zoning:
        ++self.zoning_disables;
        ++self.placement_effects;
        break;
    default: return 1;
    }
    return 0;
}
int translate(void* raw, position::Identity, position::Identity,
              position::Point3 delta, std::string&) {
    auto& p = static_cast<Fixture*>(raw)->position;
    p = {p.x + delta.x, p.y + delta.y, p.z + delta.z};
    return 0;
}
int aabb(void* raw, position::Identity, std::string&) {
    ++static_cast<Fixture*>(raw)->position_updates; return 0;
}
int physical(void* raw, position::Identity, position::Identity, float, float,
             std::string&) { ++static_cast<Fixture*>(raw)->physical_updates; return 0; }
int visual(void* raw, position::Identity, position::Identity, std::string&) {
    ++static_cast<Fixture*>(raw)->visual_updates; return 0;
}
int destination(void* raw, position::Identity, position::Point3 p, std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    ++self.destination_updates; self.position = p; return 0;
}
int force(void* raw, position::Identity, position::Identity, std::string&) {
    ++static_cast<Fixture*>(raw)->force_updates; return 0;
}
int graph(void*, const factory::Record& record, const position::Owner& pos,
          const binding::Components& c, std::string&) {
    return record.constructor.identity == pos.identity &&
        (!c.ais_faery ||
            record.components.find(ctor::Component::ai) == c.ais_faery) &&
        pos.physical_object == reinterpret_cast<position::Identity>(c.physical) &&
        pos.visual_object == reinterpret_cast<position::Identity>(c.visual) &&
        c.physical && c.visual &&
        ((!c.ais_faery && !c.pofaerie) || (c.ais_faery && c.pofaerie)) ? 0 : 1;
}

int main() {
    Fixture f;
    std::string error;
    manager::Owner objects;
    factory::Owner characters(objects, {&f, &enroll, &remove_character});
    const factory::Services constructors{&f, &component, &associate,
        &register_state, &rollback};
    const factory::LifecycleServices init{&f, &lifecycle, &lifecycle};
    manager::GameObject go{};
    dh2::character::aggro_search::GameObject aggro{};
    factory::Record* record = nullptr;
    factory::Result factory_result{};
    require(characters.create(41, go, aggro, constructors, &record,
        &factory_result, error) == factory::Status::complete && record,
        "canonical source Character factory setup failed");
    f.actor = record->constructor.identity;
    require(characters.mark_source_properties_ready(41, &factory_result, error) ==
        factory::Status::complete && characters.run_init_post(41, init,
        &factory_result, error) == factory::Status::complete &&
        characters.run_init_final(41, init, &factory_result, error) ==
        factory::Status::complete, "source Character lifecycle setup failed");

    int pofaerie = 1, physical_owner = 2, visual_owner = 3;
    const auto ais = reinterpret_cast<dh2::character_faery_script_session_v1::Identity>(
        record->components.find(ctor::Component::ai));
    dh2::character_faery_script_session_v1::Session session{};
    session.character = record->constructor.identity;
    session.kind = dh2::character_faery_script_session_v1::ScriptKind::faery;
    session.char_ai_owner = ais;
    session.ais = ais; session.char_ai_script = ais;
    session.lua_instance = ais + 0x4; session.lua_state = ais + 0x100;
    session.constructed = session.character_bound = session.functions_bound =
        session.close_owned = true;
    session.common_loaded = true;
    session.initialized = true;
    const dh2::character_faery_script_session_v1::Services script{
        &f, &script_construct, &script_dispatch};
    position::Owner pos{};
    pos.identity = record->constructor.identity;
    pos.instance_transform = 0x10;
    pos.physical_object = reinterpret_cast<position::Identity>(&physical_owner);
    pos.visual_object = reinterpret_cast<position::Identity>(&visual_owner);
    binding::Components components{record->components.find(ctor::Component::ai),
        &session, script, &pofaerie, &physical_owner, &visual_owner};
    const placement::Services level_services{&f, &level};
    const position::Services position_services{&f, &translate, &aabb, &physical,
        &visual, &destination, &force};
    binding::Binding graph_binding{record, &pos, position_services, components,
        level_services, &f, &graph};

    // A missing graph anywhere in the source Level roster is caught during
    // read-only prepare, before the dispatcher can issue any source mutation.
    f.list_size = 2;
    registry::Owner missing({{f.actor, &graph_binding}}, level_services);
    registry::Result result{};
    require(missing.preflight(&result, error) ==
        registry::Status::missing_character_graph && f.placement_effects == 0 &&
        f.position_updates == 0, "preflight allowed a partial source mutation");
    f.list_size = 1;

    auto missing_visual_components = components;
    missing_visual_components.visual = nullptr;
    binding::Binding missing_visual_binding{record, &pos, position_services,
        missing_visual_components, level_services, &f, &graph};
    registry::Owner missing_visual({{f.actor, &missing_visual_binding}},
        level_services);
    require(missing_visual.preflight(&result, error) ==
        registry::Status::graph_incomplete &&
        error.find("VisualObject owner is missing") != std::string::npos &&
        f.placement_effects == 0 && f.position_updates == 0,
        "Level preflight did not report missing same-Character visual owner");

    registry::Owner ready({{f.actor, &graph_binding}}, level_services);
    require(ready.preflight(&result, error) == registry::Status::complete &&
        result.classified_characters == 1 && result.faeries == 1,
        "complete same-Character graph did not pass Level preflight");
    require(ready.place(0x701, &result, error) == registry::Status::complete &&
        result.placement.placed_faeries == 1 && f.position_updates == 1 &&
        f.physical_updates == 1 && f.visual_updates == 1 &&
        f.destination_updates == 1 && f.force_updates == 1 &&
        f.faery_links == 1 && f.master_writes == 2 &&
        f.first_master == 0x704 && f.restored_master == 0x702 &&
        f.changed_character == 0x701 && f.changed_faery == 2 &&
        f.position.x == 5 && f.position.y == 7 && f.position.z == 9,
        "Level-wide placement did not route position through matching owner");

    // Followers share the Character/GameObject physical+visual graph but do
    // not require AISFaery, a Faery script session, or a POFaerie owner.
    f.is_faery = false;
    auto follower_components = components;
    follower_components.ais_faery = nullptr;
    follower_components.script_session = nullptr;
    follower_components.pofaerie = nullptr;
    binding::Binding follower_binding{record, &pos, position_services,
        follower_components, level_services, &f, &graph};
    registry::Owner follower({{f.actor, &follower_binding}}, level_services);
    const auto prior_zoning = f.zoning_disables;
    require(follower.preflight(&result, error) == registry::Status::complete &&
        result.followers == 1 && result.faeries == 0,
        "follower common Character graph was incorrectly gated on Faery owners");
    require(follower.place(0x701, &result, error) == registry::Status::complete &&
        result.placement.placed_followers == 1 && f.zoning_disables == prior_zoning + 1,
        "follower placement did not preserve source zoning behavior");

    std::puts("{\"missing_member_rejected_before_mutation\":true,\"complete_roster_preflight\":true,\"position_routed_by_identity\":true,\"faery_links_and_master_restore\":true,\"follower_common_graph\":true}");
    return 0;
}
