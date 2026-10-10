#include "../character_ai_association.hpp"
#include "../character_faery_factory_bridge_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace bridge = dh2::character_faery_factory_bridge_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace ai = dh2::character_ai_initialization;
namespace placement = dh2::character_faery_placement_v1;
namespace links = dh2::character_faery_links_v1;
namespace data = dh2::data;

void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "Faery factory bridge regression: %s\n", message);
        std::abort();
    }
}

struct Fixture {
    std::array<int, 22> components{};
    std::array<ai::State, 2> ai_states{};
    std::size_t next_ai{};
    std::vector<aggro::Character*> roster;
};

int append_ai(void*, ai::State* state, std::uintptr_t identity) {
    return state && state->identity == identity ? 0 : 1;
}

int make_component(void* raw, ctor::Component id, ctor::Identity,
                   factory::ComponentStorage::Slot* slot, std::string&) {
    auto& fixture = *static_cast<Fixture*>(raw);
    if (id == ctor::Component::ai) {
        if (fixture.next_ai >= fixture.ai_states.size()) return 1;
        auto& state = fixture.ai_states[fixture.next_ai++];
        state.identity = reinterpret_cast<std::uintptr_t>(&state);
        const ai::Services services{&fixture, append_ai};
        ai::Result result{};
        if (ai::construct(&state, 0x1000 + fixture.next_ai, &services,
                          &result) != ai::Status::complete || !result.queued)
            return 1;
        slot->canonical_owner = &state;
        return 0;
    }
    slot->canonical_owner = &fixture.components[static_cast<std::size_t>(id)];
    return 0;
}

int associate(void*, ctor::Association id, ctor::Identity identity,
              const factory::ComponentStorage& components, std::string&) {
    if (id != ctor::Association::ai) return 0;
    auto* state = static_cast<ai::State*>(components.find(ctor::Component::ai));
    return state && dh2::character_ai_association::associate(state, identity) ==
        dh2::character_ai_association::Status::complete ? 0 : 1;
}

int register_state(void*, ctor::Identity, std::uint32_t, std::string&) {
    return 0;
}
void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
              factory::ComponentStorage&) noexcept {}

int enroll(void* raw, aggro::Character* character, bool duplicate,
           bool* appended) {
    auto& roster = static_cast<Fixture*>(raw)->roster;
    if (!character || duplicate || !appended) return 1;
    roster.push_back(character);
    *appended = true;
    return 0;
}
int remove_character(void* raw, aggro::Character* character,
                     std::size_t* removed) {
    auto& roster = static_cast<Fixture*>(raw)->roster;
    if (!removed) return 1;
    *removed = 0;
    for (auto it = roster.begin(); it != roster.end();) {
        if (*it == character) { it = roster.erase(it); ++*removed; }
        else ++it;
    }
    return 0;
}
int lifecycle(void*, factory::Record&, std::string&) { return 0; }

int main() {
    manager::Owner object_manager;
    Fixture fixture;
    const factory::RosterServices roster{&fixture, enroll, remove_character};
    factory::Owner characters(object_manager, roster);
    const factory::Services services{&fixture, make_component, associate,
                                     register_state, rollback};
    const factory::LifecycleServices lifecycle_services{
        &fixture, lifecycle, lifecycle};
    factory::Record *player = nullptr, *faery = nullptr;
    factory::Result result{};
    std::string error;
    require(characters.create(31, {}, {}, services, &player, &result, error) ==
            factory::Status::complete &&
            characters.create(32, {}, {}, services, &faery, &result, error) ==
            factory::Status::complete, "factory Characters did not construct");

    links::Owner link_owner;
    require(bridge::register_character(characters, player->source_handle, 1,
                links::Role::player, link_owner, error) ==
            bridge::Status::link_owner_rejected,
            "bridge registered a Character before InitPost/InitFinal");
    for (auto* record : {player, faery}) {
        require(characters.mark_source_properties_ready(record->source_handle,
                    &result, error) == factory::Status::complete &&
                characters.run_init_post(record->source_handle,
                    lifecycle_services, &result, error) == factory::Status::complete &&
                characters.run_init_final(record->source_handle,
                    lifecycle_services, &result, error) == factory::Status::complete,
                "source lifecycle did not reach InitFinal");
    }

    auto* canonical_ai = faery->canonical_ai();
    require(canonical_ai && canonical_ai == faery->components.find(ctor::Component::ai),
            "typed accessor did not resolve this Record's canonical AI slot");
    require(bridge::register_character(characters, player->source_handle, 1,
                links::Role::player, link_owner, error) == bridge::Status::complete,
            "bridge failed to register the already-resolved player type");
    faery->nonplayer_source_ai_id = 2;
    faery->nonplayer_ai_postload_complete = true;
    require(bridge::register_faery_from_source(characters, faery->source_handle,
                data::AiTables{}, link_owner, error) == bridge::Status::source_type_unavailable,
            "Faery registration did not require a live source AIProps table");
    data::AiTables ai_tables;
    ai_tables.rows.resize(3);
    ai_tables.rows[2].type = 2;
    require(bridge::register_faery_from_source(characters, faery->source_handle,
                ai_tables, link_owner, error) == bridge::Status::link_owner_rejected,
            "Faery registration inferred type 3 from its role instead of source row");
    ai_tables.rows[2].type = 3;
    require(bridge::register_faery_from_source(characters, faery->source_handle,
                ai_tables, link_owner, error) == bridge::Status::complete,
            "bridge failed to publish canonical factory owners");
    require(bridge::register_character(characters, 999, 3, links::Role::faery,
                link_owner, error) == bridge::Status::missing_factory_character,
            "bridge accepted a missing source handle");

    auto link_services = link_owner.services(nullptr, nullptr);
    placement::Request request{};
    placement::Reply reply{};
    request.operation = placement::Operation::is_faery;
    request.subject = faery->constructor.identity;
    require(link_services.invoke(link_services.context, request, &reply) == 0 &&
            reply.word == 1, "canonical Faery Character was not classified");
    request = {};
    request.operation = placement::Operation::set_player_faery;
    request.subject = player->constructor.identity;
    request.argument = faery->constructor.identity;
    require(link_services.invoke(link_services.context, request, &reply) == 0 &&
            player->faery_character_420 == faery->constructor.identity,
            "Faery association did not land on the canonical Player Record");

    require(bridge::unregister_character(characters, faery->source_handle,
                link_owner, error) == bridge::Status::complete &&
            player->faery_character_420 == 0,
            "Faery teardown did not clear the canonical Character+0x420 link");
    require(bridge::unregister_character(characters, player->source_handle,
                link_owner, error) == bridge::Status::complete,
            "Player role binding did not unregister");
    require(bridge::unregister_character(characters, 999, link_owner, error) ==
            bridge::Status::missing_factory_character,
            "bridge teardown accepted a missing source handle");

    const auto player_handle = player->source_handle;
    const auto faery_handle = faery->source_handle;
    require(characters.retire(player_handle, &result, error) == factory::Status::complete &&
            characters.retire(faery_handle, &result, error) == factory::Status::complete,
            "factory retirement failed after bridge teardown");
    std::puts("PASS canonical factory Character/Faery bridge lifecycle and identity");
}
