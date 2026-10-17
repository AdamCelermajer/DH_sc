#include "../crypt_room_zone_manager_bridge_v1.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace bounds = dh2::crypt_module_bounds_registry_v1;
namespace zones = dh2::crypt_room_zone_owner_v1;
namespace objects = dh2::object_manager_runtime_owner_v1;
namespace enrollment = dh2::room_zone_enrollment;
namespace bridge = dh2::crypt_room_zone_manager_bridge_v1;

namespace {

struct BoundsFixture {
    std::vector<dh2::module_room_zone_bounds::Request> calls;
};

std::int32_t set_position(void* context,
                          const dh2::module_room_zone_bounds::Request* request) {
    auto& fixture = *static_cast<BoundsFixture*>(context);
    fixture.calls.push_back(*request);
    return 0;
}

struct EnrollmentFixture {
    std::vector<enrollment::Request> calls;
    dh2::room_zone_enrollment::Address non_zonable = 0;
};

std::int32_t invoke_enrollment(void* context, const enrollment::Request* request,
                              std::uint32_t* value) {
    auto& fixture = *static_cast<EnrollmentFixture*>(context);
    fixture.calls.push_back(*request);
    *value = request->operation == enrollment::Operation::is_zonable &&
                     request->object == fixture.non_zonable
        ? 0u : 1u;
    return 0;
}

bounds::Entry module(std::uint32_t index, const char* name,
                     float min_x, float min_y, float min_z,
                     float max_x, float max_y, float max_z) {
    bounds::Entry value{};
    value.module_index = index;
    value.module_name = name;
    value.root_id = std::string(name) + "_root";
    value.bounds.minimum[0] = min_x;
    value.bounds.minimum[1] = min_y;
    value.bounds.minimum[2] = min_z;
    value.bounds.maximum[0] = max_x;
    value.bounds.maximum[1] = max_y;
    value.bounds.maximum[2] = max_z;
    return value;
}

bounds::Owner crypt_bounds() {
    bounds::Owner value{};
    value.catalogue_path = "crypt.bdae";
    // Deliberately differs from numeric module order. The source registry
    // vector, not the module index, controls RoomZone list initialization.
    value.modules.push_back(module(17, "entrance", -5, -5, 0, 5, 5, 6));
    value.modules.push_back(module(3, "crypt_room", -4, -4, 0, 4, 4, 6));
    return value;
}

objects::GameObject game_object(std::uintptr_t identity, float x, float y) {
    objects::GameObject value{};
    value.identity = identity;
    value.world_x = x;
    value.world_y = y;
    value.zoning_enabled_2ee = 1;
    value.visible_80 = 1;
    return value;
}

void composed_signed_order_and_separate_no_room_owner() {
    auto module_bounds = crypt_bounds();
    BoundsFixture bounds_calls{};
    EnrollmentFixture enrollment_calls{};
    const dh2::module_room_zone_bounds::Services position_services{
        &bounds_calls, &set_position};
    const enrollment::Services enrollment_services{
        &enrollment_calls, &invoke_enrollment};

    objects::Owner manager;
    objects::GameObject* stored = nullptr;
    constexpr std::uintptr_t actor_a = 0x1001;
    constexpr std::uintptr_t player = 0x1002;
    constexpr std::uintptr_t actor_b = 0x1003;
    // Insert in a different sequence; the signed keys are visited -4, 2, 9.
    assert(manager.add_object(9, game_object(actor_a, 1, 1), &stored) ==
           objects::Status::ok);
    assert(manager.add_object(-4, game_object(player, 1, 1), &stored) ==
           objects::Status::ok);
    assert(manager.add_object(2, game_object(actor_b, 1, 1), &stored) ==
           objects::Status::ok);
    bool player_added = false;
    assert(manager.register_player_no_room(player, &player_added) == objects::Status::ok);
    assert(player_added && manager.no_room_count() == 1);

    enrollment_calls.non_zonable = player;
    zones::Owner room_zones;
    bridge::Result result{};
    assert(bridge::initialize_object_lists(module_bounds, room_zones, manager,
           &position_services, &enrollment_services, &result) == bridge::Status::complete);
    assert(result.status == bridge::Status::complete);
    assert(result.object_count == 3);
    assert(result.activation.module_count == 2 && result.activation.initialized_count == 2);
    assert(bounds_calls.calls.size() == 2);
    assert(result.object_lists.zones_completed == 2);
    assert(result.object_lists.object_visits == 6);
    assert(result.object_lists.successful_add_calls == 4);

    zones::ZoneView first{}, second{};
    assert(room_zones.zone_at(0, &first) && room_zones.zone_at(1, &second));
    assert(first.module_index == 17 && second.module_index == 3);
    assert(std::string(first.module_name) == "entrance");
    assert(first.member_count == 0);
    assert(second.member_count == 2);

    // Every source RoomZone scan begins with signed-key order: player(-4),
    // actor_b(2), actor_a(9). The first zone's complete scan precedes the next.
    assert(enrollment_calls.calls.size() == 20);
    assert(enrollment_calls.calls[0].object == player);
    assert(enrollment_calls.calls[1].object == actor_b);
    assert(enrollment_calls.calls[5].object == actor_a);
    assert(enrollment_calls.calls[9].object == player);
    assert(enrollment_calls.calls[10].object == actor_b);
    assert(enrollment_calls.calls[15].object == actor_a);
    assert(enrollment_calls.calls[9].room_zone == 0);
    assert(enrollment_calls.calls[10].room_zone == first.identity);
    assert(enrollment_calls.calls[10].operation == enrollment::Operation::is_zonable);
    assert(enrollment_calls.calls[11].operation == enrollment::Operation::remove_from_room);
    assert(enrollment_calls.calls[11].room_zone == first.identity);

    auto* stored_player = manager.find_by_identity(player);
    auto* stored_a = manager.find_by_identity(actor_a);
    auto* stored_b = manager.find_by_identity(actor_b);
    assert(stored_player && stored_a && stored_b);
    assert(stored_player->room_zone_2f4 == 0 && stored_player->in_room_list_2ef == 0);
    assert(stored_player->no_room_member_2f8 == 1);
    assert(stored_a->room_zone_2f4 == second.identity && stored_a->in_room_list_2ef == 1);
    assert(stored_b->room_zone_2f4 == second.identity && stored_b->in_room_list_2ef == 1);
    std::uintptr_t no_room_identity = 0;
    assert(manager.no_room_at(0, &no_room_identity) && no_room_identity == player);

    const auto call_count = enrollment_calls.calls.size();
    assert(bridge::initialize_object_lists(module_bounds, room_zones, manager,
           &position_services, &enrollment_services, &result) ==
           bridge::Status::object_lists_initialization_failed);
    assert(result.room_zone_status == zones::Status::object_lists_already_initialized);
    assert(enrollment_calls.calls.size() == call_count);
}

void reject_mismatched_active_zone_registry() {
    auto first_bounds = crypt_bounds();
    BoundsFixture bounds_calls{};
    const dh2::module_room_zone_bounds::Services position_services{
        &bounds_calls, &set_position};
    zones::Owner room_zones;
    zones::ActivationResult activation{};
    assert(room_zones.activate(first_bounds, &position_services, &activation) ==
           zones::Status::complete);

    auto different_bounds = crypt_bounds();
    different_bounds.modules[1].bounds.maximum[0] = 4.5f;
    objects::Owner manager;
    objects::GameObject* stored = nullptr;
    assert(manager.add_object(1, game_object(0x2001, 1, 1), &stored) == objects::Status::ok);
    EnrollmentFixture enrollment_calls{};
    const enrollment::Services enrollment_services{
        &enrollment_calls, &invoke_enrollment};
    bridge::Result result{};
    assert(bridge::initialize_object_lists(different_bounds, room_zones, manager,
           &position_services, &enrollment_services, &result) ==
           bridge::Status::module_registry_mismatch);
    assert(enrollment_calls.calls.empty());
}

} // namespace

int main() {
    composed_signed_order_and_separate_no_room_owner();
    reject_mismatched_active_zone_registry();
    std::cout << "crypt_room_zone_manager_bridge_v1: PASS (signed map order, source zone order, canonical fields, separate +0x88 list)\n";
}
