#include "../character_ai_association.hpp"
#include "../character_ai_initialization.hpp"
#include "../character_faery_links_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace links = dh2::character_faery_links_v1;
namespace placement = dh2::character_faery_placement_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace ai = dh2::character_ai_initialization;

void require(bool value, const char* message) {
    if (!value) { std::fprintf(stderr, "Faery link regression: %s\n", message); std::abort(); }
}

struct Fixture {
    std::array<int, 22> components{};
    ai::State ai_states[2]{};
    std::uint32_t queue_calls{};
    std::vector<aggro::Character*> roster;
    factory::Record* player{};
    factory::Record* faery{};
    std::uint32_t delegated{};
    std::uint32_t change_faery_calls{};
    std::uint32_t master_query_calls{};
};

int is_host_player(void* raw, std::uintptr_t character, bool* out) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!out || !character) return 1;
    ++self.master_query_calls;
    *out = character == self.player->constructor.identity;
    return 0;
}
int target_position(void* raw, std::uintptr_t character,
                    placement::Vector3* out) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!out || !character) return 1;
    ++self.master_query_calls;
    if (character == self.faery->constructor.identity) *out = {10, 20, 30};
    else if (character == self.player->constructor.identity) *out = {11, 22, 33};
    else if (character == 0x9001) *out = {11, 20, 30};
    else return 1;
    return 0;
}
int owner_view_distance(void* raw, std::uintptr_t character, float* out) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!out || !self.faery || character != self.faery->constructor.identity) return 1;
    ++self.master_query_calls;
    *out = 2.0f;
    return 0;
}

int append_ai(void* raw, ai::State* state, std::uintptr_t identity) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!state || state->identity != identity) return 1;
    ++self.queue_calls;
    return 0;
}

int component(void* raw, ctor::Component id, ctor::Identity,
              factory::ComponentStorage::Slot* slot, std::string&) {
    auto& self = *static_cast<Fixture*>(raw);
    if (id == ctor::Component::ai) {
        // Factory Character identities are record addresses, so assign one AI
        // state per construction in source order rather than deriving identity.
        static std::uint32_t next = 0;
        const auto ai_index = next++;
        if (ai_index >= 2) return 1;
        auto& state = self.ai_states[ai_index];
        state.identity = reinterpret_cast<std::uintptr_t>(&state);
        ai::Services services{&self, append_ai}; ai::Result result{};
        if (ai::construct(&state, 0x1000 + ai_index, &services, &result) !=
                ai::Status::complete || result.queued != 1) return 1;
        slot->canonical_owner = &state;
        return 0;
    }
    const auto index = static_cast<std::size_t>(id);
    slot->canonical_owner = &self.components[index];
    return 0;
}

// CharacterFactory's AI-association callback receives the canonical
// Character identity separately in production. This fixture sets it using
// source-order constructor context before the factory reaches association.
int associate_character(void*, ctor::Association id, ctor::Identity identity,
              const factory::ComponentStorage& components, std::string&) {
    if (id != ctor::Association::ai) return 0;
    auto* state = static_cast<ai::State*>(components.find(ctor::Component::ai));
    if (!state) return 1;
    return dh2::character_ai_association::associate(state, identity) ==
        dh2::character_ai_association::Status::complete ? 0 : 1;
}

int register_state(void*, ctor::Identity, std::uint32_t, std::string&) { return 0; }
void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
              factory::ComponentStorage&) noexcept {}
int enroll(void* raw, aggro::Character* value, bool duplicate, bool* appended) {
    auto& self = *static_cast<Fixture*>(raw);
    if (!value || duplicate || !appended) return 1;
    self.roster.push_back(value); *appended = true; return 0;
}
int remove_character(void* raw, aggro::Character* value, std::size_t* removed) {
    auto& roster = static_cast<Fixture*>(raw)->roster; *removed = 0;
    for (auto it = roster.begin(); it != roster.end();) {
        if (*it == value) { it = roster.erase(it); ++*removed; } else ++it;
    }
    return 0;
}
int init_post(void*, factory::Record&, std::string&) { return 0; }
int init_final(void*, factory::Record&, std::string&) { return 0; }

int delegate(void* raw, const placement::Request& request, placement::Reply* reply) {
    auto& self = *static_cast<Fixture*>(raw); ++self.delegated;
    switch (request.operation) {
    case placement::Operation::list_begin: reply->identity = 100; return 0;
    case placement::Operation::list_end: reply->identity = 101; return 0;
    case placement::Operation::list_value: reply->identity = self.faery->constructor.identity; return 0;
    case placement::Operation::list_next: reply->identity = 101; return 0;
    case placement::Operation::online_state: reply->word = 0; return 0;
    case placement::Operation::is_follower: reply->word = 0; return 0;
    case placement::Operation::local_player: reply->identity = 200; return 0;
    case placement::Operation::player_character: reply->identity = self.player->constructor.identity; return 0;
    case placement::Operation::look_at_vector: reply->vector = {1, 0, 0}; return 0;
    case placement::Operation::target_position: reply->vector = {10, 20, 30}; return 0;
    case placement::Operation::set_position:
    case placement::Operation::force_update_position: return 0;
    case placement::Operation::player_count: reply->word = 1; return 0;
    case placement::Operation::player_at: reply->identity = self.player->constructor.identity; return 0;
    case placement::Operation::current_faery_id:
        if (request.subject != self.player->constructor.identity || request.value != -1) return 1;
        reply->word = 2; return 0;
    case placement::Operation::change_faery:
        if (request.subject != self.player->constructor.identity || request.value != 2) return 1;
        ++self.change_faery_calls; return 0;
    default: return 1;
    }
}

int main() {
    manager::Owner objects; Fixture fixture;
    const factory::RosterServices roster{&fixture, enroll, remove_character};
    factory::Owner characters(objects, roster);
    const factory::Services factory_services{&fixture, component,
        associate_character, register_state, rollback};
    const factory::LifecycleServices lifecycle{&fixture, init_post, init_final};
    manager::GameObject player_object{}, faery_object{};
    aggro::GameObject player_aggro{}, faery_aggro{};
    factory::Record *player = nullptr, *faery = nullptr;
    factory::Result player_result{}, faery_result{}; std::string error;
    require(characters.create(3, player_object, player_aggro, factory_services,
        &player, &player_result, error) == factory::Status::complete,
        "Player factory construction failed");
    require(characters.create(44, faery_object, faery_aggro, factory_services,
        &faery, &faery_result, error) == factory::Status::complete,
        "Faery factory construction failed");
    fixture.player = player; fixture.faery = faery;
    auto* premature_ai = static_cast<ai::State*>(
        faery->components.find(ctor::Component::ai));
    links::Owner premature_links;
    require(premature_ai && premature_links.register_character(*faery,
        *premature_ai, 3, links::Role::faery, error) ==
        links::Status::source_character_incomplete,
        "Faery links accepted before source InitPost/Final");
    for (auto* record : {player, faery}) {
        factory::Result result{};
        require(characters.mark_source_properties_ready(record->source_handle,
            &result, error) == factory::Status::complete, "source properties stage failed");
        require(characters.run_init_post(record->source_handle, lifecycle,
            &result, error) == factory::Status::complete, "InitPost stage failed");
        require(characters.run_init_final(record->source_handle, lifecycle,
            &result, error) == factory::Status::complete, "InitFinal stage failed");
    }
    auto* player_ai = static_cast<ai::State*>(player->components.find(ctor::Component::ai));
    auto* faery_ai = static_cast<ai::State*>(faery->components.find(ctor::Component::ai));
    require(player_ai && faery_ai, "canonical CharAI components missing");
    faery_ai->master_50 = 0x9001; // retained prior master, outside this registry
    links::Owner link_owner;
    require(link_owner.register_character(*player, *player_ai, 1,
        links::Role::player, error) == links::Status::complete, "Player link registration failed");
    require(link_owner.register_character(*faery, *faery_ai, 3,
        links::Role::faery, error) == links::Status::complete, "Faery link registration failed");
    links::Owner unwired_links;
    require(unwired_links.register_character(*faery, *faery_ai, 3,
        links::Role::faery, error) == links::Status::complete,
        "unwired Faery link registration failed");
    const auto unwired = unwired_links.services(&fixture, delegate);
    placement::Request unwired_master{};
    placement::Reply unwired_reply{};
    unwired_master.operation = placement::Operation::set_ai_master;
    unwired_master.subject = faery->constructor.identity;
    unwired_master.argument = player->constructor.identity;
    require(unwired.invoke(unwired.context, unwired_master, &unwired_reply) != 0 &&
        faery_ai->master_50 == 0x9001,
        "non-null master changed while source providers were unavailable");
    links::Owner wrong_type_links;
    require(wrong_type_links.register_character(*faery, *faery_ai, 2,
        links::Role::faery, error) == links::Status::wrong_character_type,
        "non-Faery Character type registered as Faery");
    const links::MasterServices master{&fixture, is_host_player,
        target_position, owner_view_distance};
    const auto services = link_owner.services(&fixture, delegate, master);
    placement::Runtime placement_runtime(services); placement::Result placed{};
    const auto placement_status = placement_runtime.place(0, &placed);
    if (placement_status != placement::Status::complete)
        std::fprintf(stderr, "placement status=%u operation=%u callbacks=%u\n",
            unsigned(placement_status), unsigned(placed.last_operation), placed.callbacks);
    require(placement_status == placement::Status::complete,
        "source placement failed with factory-owned identities");
    require(player->faery_character_420 == faery->constructor.identity,
        "Character+0x420 did not link the factory-owned Faery");
    require(faery_ai->master_50 == 0x9001,
        "Faery CharAI+0x50 did not restore its previous master");
    require(faery_ai->byte_54 == 1 && faery_ai->byte_55 == 1,
        "AI_SetMaster host-player/distance bytes differ from source behavior");
    require(fixture.master_query_calls == 8,
        "AI_SetMaster did not query each installed/restored master's source state");
    require(fixture.change_faery_calls == 1 && placed.placed_faeries == 1 &&
        placed.player_faery_links == 1, "source ChangeFaery/link call counts differ");

    // The only Character+0x420 slot is on the same canonical Player Record.
    // Reject foreign identities, keep that value stable across view reads,
    // then clear it when the target Faery's exact Record is unregistered.
    const auto player_identity = player->constructor.identity;
    const auto faery_identity = faery->constructor.identity;
    placement::Request foreign_link{};
    placement::Reply foreign_reply{};
    foreign_link.operation = placement::Operation::set_player_faery;
    foreign_link.subject = player_identity + 8;
    foreign_link.argument = faery_identity;
    require(services.invoke(services.context, foreign_link, &foreign_reply) != 0 &&
        player->faery_character_420 == faery_identity,
        "foreign Character changed the canonical Record's Faery association");
    for (unsigned view = 0; view != 8; ++view)
        require(characters.find(player->source_handle) == player &&
            player->faery_character_420 == faery_identity,
            "same-Record Faery association changed across repeated view reads");
    require(link_owner.unregister_character(faery_identity, error) ==
        links::Status::complete && player->faery_character_420 == 0,
        "exact Faery teardown retained a stale Character+0x420 target");
    require(link_owner.unregister_character(player_identity, error) ==
        links::Status::complete && player->faery_character_420 == 0,
        "exact Player teardown retained its Character+0x420 association");
    const auto player_handle = player->source_handle;
    const auto faery_handle = faery->source_handle;
    factory::Result retired{};
    require(characters.retire(player_handle, &retired, error) ==
        factory::Status::complete && characters.find(player_handle) == nullptr &&
        characters.find(faery_handle) == faery,
        "Player Character Record teardown removed or retained the wrong Record");
    require(characters.retire(faery_handle, &retired, error) ==
        factory::Status::complete && characters.size() == 0,
        "Faery Character Record teardown did not retire its exact owner");

    std::puts("{\"factory_identity_links\":true,\"player_field_420\":true,\"ai_master_unwired_fail_closed\":true,\"ai_master_50_restored\":true,\"ai_master_bytes_54_55\":true,\"placement_completed\":true}");
    return 0;
}
