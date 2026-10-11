#include "../player_manager_update_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace h = dh2::player_manager_host_level;
namespace p = dh2::player_info_record_v1;
namespace n = dh2::netstruct_members_v1;
namespace o = dh2::player_offline_registry_v1;
namespace r = dh2::player_offline_registration_v1;
namespace u = dh2::player_manager_update_v1;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

struct Memory {
    n::Memory services() {
        return {this,
            [](void*, std::size_t size, int tag) -> void* {
                return tag == 2 ? std::malloc(size) : nullptr;
            },
            [](void*, void* value) { std::free(value); }};
    }
};

int main() {
    try {
        Memory memory;
        std::uint64_t serial = 0;
        p::Factory factory(&serial, memory.services());
        p::Record fallback;
        require(factory.construct_record(fallback) == p::Status::complete,
                "full canonical fallback record");
        h::PlayerInfoProjection fallback_view{
            0x7100, -1, &fallback.at(0x310)->header};
        h::PlayerRegistry registry{0x7200, nullptr, 0, &fallback_view};
        dh2::input_manager_v1::Manager input{};
        dh2::input_manager_v1::construct_win32(input, {});
        o::Owner records(registry, factory, fallback, input);

        struct Context {
            h::PlayerRegistry* registry;
            o::Owner* records;
            p::Record* fallback;
            std::uint8_t online{};
            bool deleted{};
            std::uintptr_t character{0x7700};
            unsigned controller_calls{}, manage_calls{}, death_calls{};
            std::string trace;

            static int read_online(void* raw, std::uint8_t* out) {
                *out = static_cast<Context*>(raw)->online;
                return 0;
            }
            static int deleted_81(void* raw, std::uintptr_t identity,
                                  std::uint8_t* out) {
                auto& self = *static_cast<Context*>(raw);
                if (identity != 0x7700) return 1;
                *out = self.deleted;
                return 0;
            }
            static int local_ready(void*, std::uint8_t* out) {
                *out = 1;
                return 0;
            }
            static int local_player(void* raw, h::PlayerInfoProjection** out) {
                auto& self = *static_cast<Context*>(raw);
                h::Services services{};
                services.context = &self;
                services.read_online_byte_5 = read_online;
                h::Result result{};
                return h::get_player_by_internal_id(self.registry, &services, 0,
                    0, out, &result) == h::Status::complete ? 0 : 1;
            }
            static int controllers(void* raw, h::PlayerRegistry* registry,
                                   std::string&) {
                auto& self = *static_cast<Context*>(raw);
                if (registry != self.registry) return 1;
                ++self.controller_calls;
                self.trace += 'C';
                return 0;
            }
            static int manage(void* raw, std::string&) {
                auto& self = *static_cast<Context*>(raw);
                ++self.manage_calls;
                self.trace += 'M';
                return 0;
            }
            static int local_deaths(void* raw, std::string&) {
                auto& self = *static_cast<Context*>(raw);
                ++self.death_calls;
                self.trace += 'D';
                return 0;
            }
        } context{};
        context.registry = &registry;
        context.records = &records;
        context.fallback = &fallback;
        context.deleted = true;

        const auto queries = [] (Context& c) {
            dh2::player_locality_v1::Services q{};
            q.context = &c;
            q.online = Context::read_online;
            q.internal_id_player = [](void* raw, const h::PlayerRegistry* registry,
                                      int id, unsigned flag,
                                      h::PlayerInfoProjection** out) {
                auto& self = *static_cast<Context*>(raw);
                if (registry != self.registry) return 1;
                h::Services services{};
                services.context = &self;
                services.read_online_byte_5 = Context::read_online;
                h::Result result{};
                return h::get_player_by_internal_id(registry, &services, id,
                    flag, out, &result) == h::Status::complete ? 0 : 1;
            };
            return q;
        }(context);

        auto registration_services = records.services(queries);
        r::Runtime registration(registry, registration_services);
        r::Result registration_result{};
        require(registration.check_local_controllers(&registration_result) ==
                    r::Status::complete && registry.entry_count == 1,
                "one actual canonical registered PlayerInfo");
        auto* row = registry.entries[0];
        auto* row_record = records.record(row);
        require(row_record != nullptr, "canonical PlayerInfo record lookup");
        row_record->character_660 = context.character;
        row_record->at(0x4c8)->header.reserved[0] = 1;
        row_record->at(0x4e8)->header.reserved[0] = 1;
        row_record->at(0x508)->header.reserved[0] = 1;
        row_record->at(0x528)->header.reserved[0] = 1;
        row_record->at(0x388)->header.value = 33;

        u::State state{};
        u::Result result{};
        std::string error;
        auto unknown_state = state;
        unknown_state.manager_identity = registry.manager_identity;
        require(u::update_offline(unknown_state, registry, records, {},
                    registry.manager_identity, registry.manager_identity,
                    &result, error) == u::Status::service_unavailable,
                "default-zero manager fields are not treated as source facts");
        require(u::manager_constructed(state, registry.manager_identity),
                "source PlayerManager constructor receipt establishes initial zeroes");
        require(u::player_character_added(state, registry.manager_identity,
                    context.character) && state.local_death_count_6c4 == 1,
                "successful canonical Player Character adoption increments manager count once");
        require(u::player_character_added(state, registry.manager_identity,
                    context.character) && state.local_death_count_6c4 == 1,
                "same Character adoption is idempotent");
        require(u::load_process_started(state, registry.manager_identity) &&
                    state.manager_remove_flag_6c9 == 1 &&
                    u::application_load_completed(state, registry.manager_identity) &&
                    state.manager_remove_flag_6c9 == 0,
                "load lifecycle retains source +0x6c9 set and completion writes");
        require(u::player_character_removed(state, registry.manager_identity,
                    context.character) && state.local_death_count_6c4 == 0,
                "completed canonical Character removal decrements manager count");
        require(u::player_character_added(state, registry.manager_identity,
                    context.character),
                "test returns the same Player Character to the manager before Update");
        state.loading_slot_6cc = 5;
        state.online_transition_719 = 1;
        state.local_online_transition_6ca = 1;
        state.loading_flag_6d0 = 1;
        state.quest_sync_flag_6cb = 1;
        state.quest_sync_flag_710 = 1;
        state.local_transition_flag_71a = 1;
        state.local_transition_flag_71b = 1;
        state.quest_sync_flag_711 = 1;
        state.global_death_state_714 = 1;
        state.quest_sync_buffer_6e0 = {1, 2, 3};

        // _ManageCharacters has a second ClearLoadingInfo immediately before
        // RemoveAllCharacters on the exact offline +0x6c9==0/+0x6c4>0 path.
        auto remove_all_state = state;
        u::Result remove_all_clear{};
        require(u::clear_loading_info_before_remove_all(
                    remove_all_state, *row_record, &remove_all_clear) ==
                    u::Status::complete &&
                    remove_all_clear.member_writes == 5 &&
                    row_record->at(0x4c8)->header.reserved[0] == 0 &&
                    row_record->at(0x4e8)->header.reserved[0] == 0 &&
                    row_record->at(0x508)->header.reserved[0] == 0 &&
                    row_record->at(0x528)->header.reserved[0] == 0 &&
                    row_record->at(0x388)->header.value == -1 &&
                    remove_all_state.local_death_count_6c4 == 1 &&
                    remove_all_state.counted_player_character == context.character,
                "second ClearLoadingInfo preserves the real one-Character count until RemoveCharacter completes");
        row_record->at(0x4c8)->header.reserved[0] = 1;
        row_record->at(0x4e8)->header.reserved[0] = 1;
        row_record->at(0x508)->header.reserved[0] = 1;
        row_record->at(0x528)->header.reserved[0] = 1;
        row_record->at(0x388)->header.value = 33;

        u::Services services{};
        services.context = &context;
        services.read_online = Context::read_online;
        services.read_character_deleted_81 = Context::deleted_81;
        services.read_local_transition_ready_4e5 = Context::local_ready;
        services.get_local_player = Context::local_player;
        services.check_local_controllers = Context::controllers;
        services.manage_characters = Context::manage;
        services.check_local_deaths = Context::local_deaths;
        require(u::services_ready(services),
                "offline readiness requires all reached branch providers");
        auto incomplete_services = services;
        incomplete_services.manage_characters = nullptr;
        require(!u::services_ready(incomplete_services),
                "offline readiness rejects missing character management");
        require(u::update_offline(state, registry, records, services,
                    registry.manager_identity, registry.manager_identity,
                    &result, error) == u::Status::complete,
                "source offline update pass");
        require(row_record->character_660 == 0 &&
                    result.stale_character_links_cleared == 1,
                "deleted Character cleared from same PlayerInfo");
        require(row_record->at(0x4c8)->header.reserved[0] == 0 &&
                    row_record->at(0x4e8)->header.reserved[0] == 0 &&
                    row_record->at(0x508)->header.reserved[0] == 0 &&
                    row_record->at(0x528)->header.reserved[0] == 0 &&
                    row_record->at(0x388)->header.value == -1,
                "offline ClearLoadingInfo updates canonical members");
        require(state.loading_slot_6cc == -1 && !state.online_transition_719 &&
                    !state.local_online_transition_6ca && !state.loading_flag_6d0 &&
                    !state.quest_sync_flag_6cb && !state.quest_sync_flag_710 &&
                    !state.local_transition_flag_71a &&
                    !state.local_transition_flag_71b &&
                    !state.quest_sync_flag_711 && !state.global_death_state_714 &&
                    state.quest_sync_buffer_6e0.empty(),
                "offline transition clears manager-owned source state");
        require(context.controller_calls == 1 && context.manage_calls == 1 &&
                    context.death_calls == 1 && context.trace == "CMD",
                "remaining offline helpers called in source order");
        require(result.entries_examined == 1 && result.member_writes == 5 &&
                    result.status == u::Status::complete && error.empty(),
                "update result describes concrete manager and member writes");

        context.online = 1;
        require(u::update_offline(state, registry, records, services,
                    registry.manager_identity, registry.manager_identity,
                    &result, error) == u::Status::online_unsupported,
                "network owner rejected without mutating offline branch");
        require(context.controller_calls == 1 && context.manage_calls == 1 &&
                    context.death_calls == 1,
                "unsupported online branch performs no helper callbacks");

        std::cout << "{\"validation\":\"PASS\",\"checks\":24,\"scope\":\"PlayerManager constructor/load/Character lifecycle receipts, unknown fact rejection, source offline prefix, same-record Character deletion detach, both ClearLoadingInfo NetStruct passes, manager transition/quest buffer reset, readiness preflight, existing controller registration and explicit remaining offline helper adapters\"}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
