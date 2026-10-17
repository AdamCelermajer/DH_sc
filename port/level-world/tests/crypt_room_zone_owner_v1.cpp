#include "../crypt_room_zone_owner_v1.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace dh2::crypt_room_zone_owner_v1;

namespace {

struct PositionFixture {
    std::vector<dh2::module_room_zone_bounds::Request> calls;
    int fail_on_call = 0;
};

std::int32_t set_position(void* context,
                          const dh2::module_room_zone_bounds::Request* request) {
    auto& fixture = *static_cast<PositionFixture*>(context);
    fixture.calls.push_back(*request);
    return fixture.fail_on_call == static_cast<int>(fixture.calls.size()) ? 1 : 0;
}

struct EnrollmentFixture {
    std::vector<dh2::room_zone_enrollment::Request> calls;
    std::uint32_t zonable = 1;
    Address non_zonable_actor = 0;
};

std::int32_t enrollment_call(void* context,
                             const dh2::room_zone_enrollment::Request* request,
                             std::uint32_t* value) {
    auto& fixture = *static_cast<EnrollmentFixture*>(context);
    fixture.calls.push_back(*request);
    *value = request->operation == dh2::room_zone_enrollment::Operation::is_zonable
        ? (request->object == fixture.non_zonable_actor ? 0 : fixture.zonable) : 0;
    return 0;
}

struct TransitionFixture {
    std::vector<TransitionRequest> calls;
    std::vector<std::uint32_t> visited_modules;
};

std::int32_t transition_call(void* context, const TransitionRequest* request,
                             std::uint32_t* value) {
    auto& fixture = *static_cast<TransitionFixture*>(context);
    fixture.calls.push_back(*request);
    if (request->operation == TransitionOperation::query_module_visited) {
        if (!value) return 1;
        *value = std::find(fixture.visited_modules.begin(),
                           fixture.visited_modules.end(), request->module_index) !=
                         fixture.visited_modules.end() ? 1u : 0u;
    } else if (request->operation == TransitionOperation::set_module_visited) {
        fixture.visited_modules.push_back(request->module_index);
    }
    return 0;
}

std::uint32_t float_word(float value) {
    std::uint32_t word = 0;
    std::memcpy(&word, &value, sizeof(word));
    return word;
}

dh2::crypt_module_bounds_registry_v1::Entry entry(
    std::uint32_t module_index, const char* name,
    float min_x, float min_y, float min_z,
    float max_x, float max_y, float max_z) {
    dh2::crypt_module_bounds_registry_v1::Entry value{};
    value.module_index = module_index;
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

dh2::crypt_module_bounds_registry_v1::Owner crypt_bounds() {
    dh2::crypt_module_bounds_registry_v1::Owner source{};
    source.catalogue_path = "crypt.bdae";
    // Deliberately non-contiguous module indices: source vector order must be
    // retained independently of the module key used for lookup.
    source.modules.push_back(entry(17, "crypt_room_a", -2, -2, 0, 2, 2, 4));
    source.modules.push_back(entry(29, "crypt_room_b", -1, -1, 1, 1, 1, 3));
    return source;
}

void activation_and_bounds() {
    auto source = crypt_bounds();
    PositionFixture calls{};
    const dh2::module_room_zone_bounds::Services services{&calls, &set_position};
    Owner owner;
    ActivationResult result{};
    assert(owner.activate(source, &services, &result) == Status::complete);
    assert(result.module_count == 2 && result.initialized_count == 2);
    assert(calls.calls.size() == 2);
    assert(calls.calls[0].operation == dh2::module_room_zone_bounds::Operation::set_position);
    assert(calls.calls[0].update_position == 1);
    assert(calls.calls[1].update_position == 1);

    ZoneView first{}, second{};
    assert(owner.zone_at(0, &first) && owner.zone_at(1, &second));
    assert(first.module_index == 17 && second.module_index == 29);
    assert(first.identity && second.identity && first.identity != second.identity);
    assert(std::string(first.module_name) == "crypt_room_a");
    assert(first.position[0] == 0 && first.position[1] == 0 && first.position[2] == 2);
    assert(first.dimensions[0] == 4 && first.dimensions[1] == 4 && first.dimensions[2] == 4);
    assert(first.relative_minimum[0] == -2 && first.relative_maximum[2] == 4);
    // The source SetPosition(true) refreshes absolute bounds as relative + center.
    assert(first.absolute_minimum[0] == -2 && first.absolute_maximum[0] == 2);
    assert(first.absolute_minimum[1] == -2 && first.absolute_maximum[1] == 2);
    assert(first.absolute_minimum[2] == 2 && first.absolute_maximum[2] == 6);
}

void membership_and_lifetime() {
    auto source = crypt_bounds();
    PositionFixture positions{};
    const dh2::module_room_zone_bounds::Services position_services{
        &positions, &set_position};
    Owner owner;
    ActivationResult activation{};
    assert(owner.activate(source, &position_services, &activation) == Status::complete);

    ZoneView first{}, second{};
    assert(owner.zone_at(0, &first) && owner.zone_at(1, &second));
    const auto first_id = first.identity;
    const auto second_id = second.identity;

    Address actor_room = 0;
    std::uint8_t in_room_list = 0, in_zone = 0, zoning = 0, visible = 0;
    const float x = 0.5f, y = 0.0f;
    const Address visual = 0;
    dh2::room_zone_enrollment::GameObject actor{
        0xabcdu, &x, &y, &actor_room, &in_room_list, &in_zone,
        &zoning, &visual, &visible};
    EnrollmentFixture operations{};
    const dh2::room_zone_enrollment::Services enrollment_services{
        &operations, &enrollment_call};

    EnrollmentResult enrollment{};
    assert(owner.add_initial_object(17, &actor, &enrollment_services, &enrollment)
           == Status::complete);
    assert(enrollment.enrollment.accepted);
    assert(actor_room == first_id && in_room_list == 1 && in_zone == 1);
    assert(operations.calls.size() == 4);
    assert(operations.calls[0].operation == dh2::room_zone_enrollment::Operation::is_zonable);
    assert(operations.calls[1].operation == dh2::room_zone_enrollment::Operation::is_zonable);
    assert(operations.calls[2].operation == dh2::room_zone_enrollment::Operation::zone_state_callback);
    assert(operations.calls[3].operation == dh2::room_zone_enrollment::Operation::append_to_room);
    assert(operations.calls[3].room_zone == first_id);
    assert(owner.zone_at(0, &first) && first.member_count == 1 && first.members[0] == actor.identity);

    // Re-enrollment follows the source remove -> assign/state -> append order.
    operations.calls.clear();
    assert(owner.add_initial_object(29, &actor, &enrollment_services, &enrollment)
           == Status::complete);
    assert(actor_room == second_id && in_room_list == 1);
    assert(operations.calls.size() == 5);
    assert(operations.calls[0].operation == dh2::room_zone_enrollment::Operation::is_zonable);
    assert(operations.calls[1].operation == dh2::room_zone_enrollment::Operation::remove_from_room);
    assert(operations.calls[1].room_zone == first_id);
    assert(operations.calls[2].operation == dh2::room_zone_enrollment::Operation::is_zonable);
    assert(operations.calls[3].operation == dh2::room_zone_enrollment::Operation::zone_state_callback);
    assert(operations.calls[4].operation == dh2::room_zone_enrollment::Operation::append_to_room);
    assert(operations.calls[4].room_zone == second_id);
    assert(owner.zone_at(0, &first) && first.member_count == 0);
    assert(owner.zone_at(1, &second) && second.member_count == 1 &&
           second.members[0] == actor.identity);

    assert(owner.activate(source, &position_services, &activation) == Status::members_present);
    assert(owner.clear() == Status::members_present);
    assert(owner.forget_source_unlinked_actor(29, actor.identity) == Status::complete);
    assert(owner.clear() == Status::complete);
    assert(owner.zone_count() == 0);
}

void ordered_object_list_scan() {
    auto source = crypt_bounds();
    PositionFixture positions{};
    const dh2::module_room_zone_bounds::Services position_services{
        &positions, &set_position};
    Owner owner;
    ActivationResult activation{};
    assert(owner.activate(source, &position_services, &activation) == Status::complete);
    ZoneView first{}, second{};
    assert(owner.zone_at(0, &first) && owner.zone_at(1, &second));
    const auto first_id = first.identity;
    const auto second_id = second.identity;

    Address actor_room = 0, player_room = 0;
    std::uint8_t actor_listed = 0, actor_in_zone = 0;
    std::uint8_t player_listed = 0, player_in_zone = 0;
    std::uint8_t zoning = 0, visible = 0;
    const float actor_x = 0.5f, actor_y = 0.0f;
    const float player_x = 0.0f, player_y = 0.0f;
    const Address no_visual = 0;
    dh2::room_zone_enrollment::GameObject actor{
        0xabcdu, &actor_x, &actor_y, &actor_room, &actor_listed,
        &actor_in_zone, &zoning, &no_visual, &visible};
    dh2::room_zone_enrollment::GameObject player{
        0xf00du, &player_x, &player_y, &player_room, &player_listed,
        &player_in_zone, &zoning, &no_visual, &visible};
    dh2::room_zone_enrollment::GameObject* source_order[]{&actor, &player};
    EnrollmentFixture fixture{};
    fixture.non_zonable_actor = player.identity;
    const dh2::room_zone_enrollment::Services services{&fixture, &enrollment_call};
    ObjectListResult scan{};
    assert(owner.initialize_object_lists(source_order, 2, &services, &scan) ==
           Status::complete);
    assert(scan.zones_completed == 2 && scan.object_visits == 4);
    assert(scan.successful_add_calls == 2);
    assert(fixture.calls.size() == 11);
    // Zone A's full object scan precedes Zone B's full scan.
    assert(fixture.calls[0].object == actor.identity);
    assert(fixture.calls[4].object == player.identity);
    assert(fixture.calls[5].object == actor.identity);
    assert(fixture.calls[10].object == player.identity);
    // Overlap transfers the actor to the later module through source removal.
    assert(fixture.calls[6].operation ==
           dh2::room_zone_enrollment::Operation::remove_from_room);
    assert(fixture.calls[6].room_zone == first_id);
    assert(actor_room == second_id && actor_listed == 1);
    assert(player_room == 0 && player_listed == 0);
    assert(owner.zone_at(0, &first) && first.member_count == 0);
    assert(owner.zone_at(1, &second) && second.member_count == 1 &&
           second.members[0] == actor.identity);
    const auto prior_call_count = fixture.calls.size();
    assert(owner.initialize_object_lists(source_order, 2, &services, &scan) ==
           Status::object_lists_already_initialized);
    assert(fixture.calls.size() == prior_call_count);
}

void frustum_activation_and_transitions() {
    auto source = crypt_bounds();
    PositionFixture positions{};
    const dh2::module_room_zone_bounds::Services position_services{
        &positions, &set_position};
    Owner owner;
    ActivationResult activation{};
    assert(owner.activate(source, &position_services, &activation) == Status::complete);

    ZoneView first{};
    assert(owner.zone_at(0, &first));
    Address actor_room = 0;
    std::uint8_t in_room_list = 0, in_zone = 0, zoning = 1, visible = 1;
    const float x = 0.5f, y = 0.0f;
    const Address visual = 0;
    dh2::room_zone_enrollment::GameObject actor{
        0xabcdu, &x, &y, &actor_room, &in_room_list, &in_zone,
        &zoning, &visual, &visible};
    EnrollmentFixture enrollment_fixture{};
    const dh2::room_zone_enrollment::Services enrollment_services{
        &enrollment_fixture, &enrollment_call};
    EnrollmentResult enrollment{};
    assert(owner.add_initial_object(17, &actor, &enrollment_services, &enrollment) ==
           Status::complete);
    assert(enrollment.enrollment.accepted);

    TransitionFixture transitions{};
    const UpdateServices update_services{&transitions, &transition_call};
    std::uint32_t planes[6][4]{}; // zero distance is inside in the source test
    const float player_position[3]{2.0f, 2.0f, 3.0f}; // inclusive XY boundary
    UpdateResult update{};
    assert(owner.update_frustum(planes, player_position, &update_services, &update) ==
           Status::complete);
    assert(update.zones_tested == 2 && update.zones_visible == 2);
    assert(update.zones_activated == 2 && update.zones_deactivated == 0);
    assert(update.zones_visited == 1);
    assert(transitions.calls.size() == 6);
    assert(transitions.calls[0].operation == TransitionOperation::zone_entered);
    assert(transitions.calls[0].actor == actor.identity);
    assert(transitions.calls[1].operation == TransitionOperation::add_room_object);
    assert(transitions.calls[2].operation == TransitionOperation::query_module_visited);
    assert(transitions.calls[3].operation == TransitionOperation::set_module_visited);
    assert(transitions.calls[4].operation == TransitionOperation::add_room_object);
    assert(transitions.calls[5].operation == TransitionOperation::query_module_visited);
    assert(transitions.visited_modules.size() == 1 &&
           transitions.visited_modules[0] == 17);

    transitions.calls.clear();
    assert(owner.mark_dirty(17) == Status::complete);
    assert(owner.update_frustum(planes, player_position, &update_services, &update) ==
           Status::complete);
    assert(update.zones_visible == 2 && update.zones_activated == 1 &&
           update.zones_deactivated == 0);
    assert(transitions.calls.size() == 4);
    assert(transitions.calls[0].operation == TransitionOperation::zone_entered);
    assert(transitions.calls[0].actor == actor.identity);
    assert(transitions.calls[1].operation == TransitionOperation::add_room_object);
    assert(transitions.calls[1].module_index == 17);
    assert(transitions.calls[2].operation == TransitionOperation::query_module_visited);
    assert(transitions.calls[3].operation == TransitionOperation::query_module_visited);

    transitions.calls.clear();
    planes[0][3] = float_word(3.0f); // positive support-vertex distance is outside
    assert(owner.update_frustum(planes, player_position, &update_services, &update) ==
           Status::complete);
    assert(update.zones_visible == 0 && update.zones_deactivated == 2);
    assert(transitions.calls.size() == 3);
    assert(transitions.calls[0].operation == TransitionOperation::zone_exited);
    assert(transitions.calls[0].actor == actor.identity);
    assert(transitions.calls[1].operation == TransitionOperation::remove_room_object);
    assert(transitions.calls[2].operation == TransitionOperation::remove_room_object);
    assert(owner.clear() == Status::members_present);
    assert(owner.forget_source_unlinked_actor(17, actor.identity) == Status::complete);
    assert(owner.clear() == Status::complete);
}

void failed_activation_is_transactional() {
    auto initial = crypt_bounds();
    initial.modules.resize(1);
    PositionFixture positions{};
    const dh2::module_room_zone_bounds::Services services{&positions, &set_position};
    Owner owner;
    ActivationResult result{};
    assert(owner.activate(initial, &services, &result) == Status::complete);
    ZoneView old{};
    assert(owner.zone_at(0, &old));
    assert(owner.activate(initial, &services, &result) == Status::already_active);
    const Address stable_id = old.identity;
    assert(owner.zone_at(0, &old) && old.identity == stable_id);
    assert(owner.clear() == Status::complete);

    auto replacement = crypt_bounds();
    positions.calls.clear();
    positions.fail_on_call = 2;
    assert(owner.activate(replacement, &services, &result) ==
           Status::bounds_initialization_failed);
    assert(result.initialized_count == 1 && result.failed_module_index == 29);
    assert(result.bounds_status == dh2::module_room_zone_bounds::Status::service_failed);
    assert(owner.zone_count() == 0);
}

} // namespace

int main() {
    activation_and_bounds();
    membership_and_lifetime();
    ordered_object_list_scan();
    frustum_activation_and_transitions();
    failed_activation_is_transactional();
    std::cout << "crypt_room_zone_owner_v1: PASS (stable zones, source ordering, membership, frustum transitions, lifetime guard)\n";
}
