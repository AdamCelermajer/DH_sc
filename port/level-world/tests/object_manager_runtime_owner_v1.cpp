#include "../object_manager_runtime_owner_v1.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace dh2::object_manager_runtime_owner_v1;

namespace {

GameObject object(Address identity) {
    GameObject value{};
    value.identity = identity;
    value.is_zonable_target_c4 = 0x100;
    value.set_updating_target_3c = 0x200;
    value.room_zone_2f4 = 0x300;
    value.visual_object_2d8 = identity + 0x10;
    value.world_x = 12.5f;
    value.world_y = -4.25f;
    value.in_room_list_2ef = 1;
    value.zoning_enabled_2ee = 1;
    value.in_zone_2f0 = 0;
    value.visible_80 = 1;
    return value;
}

struct UpdateVisitTrace {
    std::vector<Address> identities;
    Owner* owner{};
    bool stage_during_first_visit{};
    GameObject* spawned{};
};

bool record_update_visit(void* raw, GameObject* row) noexcept {
    auto& trace = *static_cast<UpdateVisitTrace*>(raw);
    if (!row) return false;
    try {
        trace.identities.push_back(row->identity);
    } catch (...) {
        return false;
    }
    if (trace.stage_during_first_visit && trace.identities.size() == 1) {
        if (trace.owner->add_object(42, object(0x4200), &trace.spawned) !=
            Status::ok) return false;
        bool staged = false;
        return trace.owner->stage_spawn_update_object(trace.spawned->identity,
                   true, &staged) ==
                   Status::ok && staged;
    }
    return true;
}

void ordered_map_and_stable_identity() {
    Owner owner;
    GameObject* first_inserted = nullptr;
    const Address high_identity = sizeof(Address) >= 8
        ? static_cast<Address>(0x100000001ull) : static_cast<Address>(0x10001u);
    const Address same_low_bits = sizeof(Address) >= 8
        ? static_cast<Address>(0x200000001ull) : static_cast<Address>(0x20001u);

    assert(owner.add_object(21, object(high_identity), &first_inserted) == Status::ok);
    assert(first_inserted && first_inserted->source_handle == 21);
    assert(owner.add_object(-3, object(same_low_bits), nullptr) == Status::invalid_argument);
    GameObject* negative = nullptr;
    assert(owner.add_object(-3, object(same_low_bits), &negative) == Status::ok);
    assert(negative && negative->identity == same_low_bits);
    assert(owner.add_object(7, object(0x300), &negative) == Status::ok);
    assert(owner.find_by_identity(high_identity) == first_inserted);

    Owner::Cursor cursor{};
    owner.reset(&cursor);
    GameObject* current = nullptr;
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == -3);
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == 7);
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == 21);
    assert(owner.next(&cursor, &current) == Status::ok && current == nullptr);
    assert(owner.next(&cursor, &current) == Status::ok && current == nullptr);

    assert(owner.add_object(22, object(high_identity + 1), &negative) == Status::ok);
    assert(owner.find_by_identity(high_identity) == first_inserted);
    owner.reset(&cursor);
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == -3);
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == 7);
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == 21);
    assert(owner.next(&cursor, &current) == Status::ok && current->source_handle == 22);

    assert(owner.add_object(21, object(0x400), &negative) == Status::duplicate_source_handle);
    assert(owner.add_object(23, object(high_identity), &negative) == Status::duplicate_identity);
}

void named_handle_create_miss_counter_and_collision() {
    Owner owner;
    assert(owner.next_source_handle() == 1);
    SourceHandle handle = 99;
    bool created = true;
    assert(owner.get_or_reserve_named_handle("Potion", false, &handle, &created) ==
           Status::not_found);
    assert(handle == 0 && !created && owner.next_source_handle() == 1);

    assert(owner.get_or_reserve_named_handle("Potion", true, &handle, &created) ==
           Status::ok);
    assert(handle == 1 && created && owner.next_source_handle() == 2);
    auto* reservation = owner.find_by_name("Potion");
    assert(reservation && reservation->source_handle == 1 &&
           reservation->identity == 0 && reservation->name == "Potion");
    assert(owner.find_by_name("potion") == nullptr);
    assert(owner.get_or_reserve_named_handle("Potion", true, &handle, &created) ==
           Status::ok);
    assert(handle == 1 && !created && owner.next_source_handle() == 2);

    GameObject* stored = nullptr;
    assert(owner.add_named_object(1, "Potion", object(0x991), &stored) == Status::ok);
    assert(stored == reservation && stored->identity == 0x991 &&
           owner.find_by_name("Potion") == stored);
    bool removed = false;
    assert(owner.remove_object(1, &removed) == Status::ok && removed &&
           owner.find_by_name("Potion") == nullptr);

    Owner collision;
    assert(collision.add_object(1, object(0x992), &stored) == Status::ok);
    assert(collision.get_or_reserve_named_handle("NewName", true, &handle,
                                                  &created) ==
           Status::ok);
    assert(handle == 2 && created && collision.next_source_handle() == 3 &&
           collision.find_by_source_handle(1)->identity == 0x992 &&
           collision.find_by_name("NewName") &&
           collision.find_by_name("NewName")->source_handle == 2);
    collision.reset_after_native_flush();
    assert(collision.next_source_handle() == 1 && collision.empty());
}

void canonical_views() {
    Owner owner;
    GameObject* value = nullptr;
    assert(owner.add_object(4, object(0x12345678), &value) == Status::ok);

    auto zoning = value->zoning_view();
    assert(zoning.identity == value->identity);
    assert(zoning.is_zonable_target_c4 == value->is_zonable_target_c4);
    assert(zoning.set_updating_target_3c == value->set_updating_target_3c);
    assert(zoning.room_zone_2f4 == value->room_zone_2f4);
    assert(zoning.visual_2d8 && zoning.visual_2d8->identity == value->visual_object_2d8);
    assert(zoning.visual_2d8->owner_identity == value->identity);

    auto enrollment = value->enrollment_view();
    assert(enrollment.identity == value->identity);
    assert(enrollment.world_x && *enrollment.world_x == 12.5f);
    assert(enrollment.world_y && *enrollment.world_y == -4.25f);
    assert(enrollment.room_zone && *enrollment.room_zone == value->room_zone_2f4);
    assert(enrollment.in_room_list && *enrollment.in_room_list == 1);
    assert(enrollment.visual_object && *enrollment.visual_object == value->visual_object_2d8);

    *enrollment.room_zone = 0x87654321;
    *enrollment.in_zone = 1;
    assert(value->room_zone_2f4 == 0x87654321);
    assert(value->in_zone_2f0 == 1);

    value->visual_object_2d8 = 0;
    assert(value->zoning_view().visual_2d8 == nullptr);
}

void borrowed_actor_fields_remain_canonical() {
    Owner owner;
    float actor_x = 1.0f;
    float actor_y = 2.0f;
    Address actor_zone = 0;
    Address actor_visual = 0x901;
    std::uint8_t actor_in_room = 0;
    std::uint8_t actor_zoning_enabled = 1;
    std::uint8_t actor_in_zone = 0;
    std::uint8_t actor_visible = 1;

    auto source = object(0x900);
    source.bind_live_fields({&actor_x, &actor_y, &actor_zone, &actor_in_room,
                             &actor_zoning_enabled, &actor_in_zone,
                             &actor_visual, &actor_visible});
    GameObject* stored = nullptr;
    assert(owner.add_object(3, source, &stored) == Status::ok && stored);

    // The map entry is a stable ordered projection, not a copied position or
    // membership owner. Both views must continue to address the actor fields.
    actor_x = -8.5f;
    actor_y = 14.25f;
    auto enrollment = stored->enrollment_view();
    assert(enrollment.world_x == &actor_x && *enrollment.world_x == -8.5f);
    assert(enrollment.world_y == &actor_y && *enrollment.world_y == 14.25f);
    assert(enrollment.room_zone == &actor_zone);
    assert(enrollment.in_room_list == &actor_in_room);
    assert(enrollment.in_zone == &actor_in_zone);
    *enrollment.room_zone = 0x902;
    *enrollment.in_room_list = 1;
    *enrollment.in_zone = 1;
    assert(actor_zone == 0x902 && actor_in_room == 1 && actor_in_zone == 1);

    const auto zoning = stored->zoning_view();
    assert(zoning.room_zone_2f4 == actor_zone && zoning.in_zone_2f0 == 1);
    assert(zoning.visual_2d8 && zoning.visual_2d8->identity == actor_visual);
}

void no_room_list_and_player_path() {
    Owner owner;
    GameObject* regular = nullptr;
    GameObject* player = nullptr;
    assert(owner.add_object(1, object(0x1001), &regular) == Status::ok);
    assert(owner.add_object(2, object(0x1002), &player) == Status::ok);

    // PlayerManager's fallback registers the player in the same +0x88 owner;
    // this owner leaves RoomZone and +0x2ef membership to their own source path.
    player->room_zone_2f4 = 0x5000;
    player->in_room_list_2ef = 0;
    bool added = false;
    assert(owner.register_player_no_room(player->identity, &added) == Status::ok);
    assert(added && player->no_room_member_2f8 == 1);
    assert(player->room_zone_2f4 == 0x5000 && player->in_room_list_2ef == 0);

    assert(owner.add_no_room_object(regular->identity, &added) == Status::ok && added);
    assert(owner.add_no_room_object(regular->identity, &added) == Status::ok && !added);
    assert(owner.no_room_count() == 2);
    Address member = 0;
    assert(owner.no_room_at(0, &member) && member == player->identity);
    assert(owner.no_room_at(1, &member) && member == regular->identity);
    assert(!owner.no_room_at(2, &member));

    bool removed = false;
    assert(owner.remove_no_room_object(player->identity, &removed) == Status::ok && removed);
    assert(player->no_room_member_2f8 == 0 && owner.no_room_count() == 1);
    assert(owner.remove_object(1, &removed) == Status::ok && removed);
    assert(owner.find_by_identity(regular->identity) == nullptr);
    assert(owner.no_room_count() == 0);
    assert(owner.find_by_identity(player->identity) == player);
}

void active_room_list_source_duplicate_and_delete() {
    Owner owner;
    bool added = false;
    constexpr Address room_zone = 0x7000;
    assert(owner.add_room_object(room_zone, &added) == Status::ok && added);
    assert(owner.room_object_count() == 1);

    // AddRoomObjects' duplicate assertion does not prevent its list insertion.
    assert(owner.add_room_object(room_zone, &added) == Status::ok && added);
    assert(owner.room_object_count() == 2);
    Address identity = 0;
    assert(owner.room_object_at(0, &identity) && identity == room_zone);
    assert(owner.room_object_at(1, &identity) && identity == room_zone);

    bool removed = false;
    assert(owner.remove_room_object(room_zone, &removed) == Status::ok && removed);
    assert(owner.room_object_count() == 0);
    assert(owner.remove_room_object(room_zone, &removed) == Status::ok && !removed);
}

void active_update_initpost_spawn_splice_and_retirement() {
    Owner owner;
    GameObject* low_key = nullptr;
    GameObject* high_key = nullptr;
    GameObject* non_character = nullptr;
    GameObject* spawn_later = nullptr;
    GameObject* spawn_earlier = nullptr;
    assert(owner.add_object(22, object(0x2200), &high_key) == Status::ok);
    assert(owner.add_object(-2, object(0x0200), &low_key) == Status::ok);
    assert(owner.add_object(12, object(0x1200), &non_character) == Status::ok);

    // InitPost traverses the signed-key map. Its actual virtual +0x38 result
    // decides membership; map order is only the order these registrations
    // arrive in, never the update owner’s independently inferred order.
    Owner::Cursor map_cursor{};
    owner.reset(&map_cursor);
    bool saw_low = false, saw_high = false, saw_non_character = false;
    for (;;) {
        GameObject* source_row = nullptr;
        assert(owner.next(&map_cursor, &source_row) == Status::ok);
        if (!source_row) break;
        const bool character_row = source_row == low_key || source_row == high_key;
        const bool source_is_updatable = character_row; // Character inherits GameObject::IsUpdatable()==1.
        bool inserted = false;
        assert(owner.append_init_post_update_object(source_row->identity,
            source_is_updatable, &inserted) == Status::ok);
        assert(inserted == character_row);
        saw_low |= source_row == low_key;
        saw_high |= source_row == high_key;
        saw_non_character |= source_row == non_character;
    }
    assert(saw_low && saw_high && saw_non_character);

    assert(owner.add_object(40, object(0x4000), &spawn_later) == Status::ok);
    assert(owner.add_object(41, object(0x4100), &spawn_earlier) == Status::ok);

    // Spawn's eligible result tail-inserts into +0x34. The next Update
    // splices that whole list after the pre-existing +0x2c order.
    bool inserted = false;
    assert(owner.stage_spawn_update_object(spawn_later->identity, true,
                                           &inserted) == Status::ok && inserted);
    assert(owner.stage_spawn_update_object(spawn_earlier->identity, true,
                                           &inserted) == Status::ok && inserted);
    assert(owner.stage_spawn_update_object(0xdead, true, &inserted) ==
           Status::not_found && !inserted);
    assert(owner.update_object_count() == 2 &&
           owner.staged_update_object_count() == 2);
    owner.begin_update_frame();
    assert(owner.update_object_count() == 4 &&
           owner.staged_update_object_count() == 0);
    GameObject* current = nullptr;
    assert(owner.update_object_at(0, &current) && current == low_key);
    assert(owner.update_object_at(1, &current) && current == high_key);
    assert(owner.update_object_at(2, &current) && current == spawn_later);
    assert(owner.update_object_at(3, &current) && current == spawn_earlier);
    assert(!owner.update_object_at(4, &current) && current == nullptr);

    UpdateVisitTrace first_frame{{}, &owner, true, nullptr};
    std::size_t visited = 0;
    assert(owner.visit_update_objects(&first_frame, record_update_visit,
                                      &visited) == Status::ok);
    const std::vector<Address> first_frame_expected{
        low_key->identity, high_key->identity,
        spawn_later->identity, spawn_earlier->identity};
    assert(visited == 4 && first_frame.identities == first_frame_expected);
    assert(owner.update_object_count() == 4 &&
           owner.staged_update_object_count() == 1);
    owner.begin_update_frame();
    UpdateVisitTrace second_frame{{}, &owner, false, nullptr};
    assert(owner.visit_update_objects(&second_frame, record_update_visit,
                                      &visited) == Status::ok);
    const std::vector<Address> second_frame_expected{
        low_key->identity, high_key->identity, spawn_later->identity,
        spawn_earlier->identity, first_frame.spawned->identity};
    assert(visited == 5 && second_frame.identities == second_frame_expected);
    bool callback_spawn_removed = false;
    assert(owner.remove_object(42, &callback_spawn_removed) == Status::ok &&
           callback_spawn_removed && owner.update_object_count() == 4);

    // A spawn during an update is not in the iteration snapshot until the
    // next frame boundary. Retirement removes all identity hooks before the
    // stable Character/GameObject Record can be freed.
    assert(owner.stage_spawn_update_object(0x2200, true, &inserted) ==
           Status::ok && inserted);
    assert(owner.update_object_count() == 4 &&
           owner.staged_update_object_count() == 1);
    bool removed = false;
    assert(owner.remove_object(22, &removed) == Status::ok && removed);
    assert(owner.update_object_count() == 3 &&
           owner.staged_update_object_count() == 0);
    assert(owner.update_object_at(0, &current) && current == low_key);
    assert(owner.update_object_at(1, &current) && current == spawn_later);
    assert(owner.update_object_at(2, &current) && current == spawn_earlier);
    owner.begin_update_frame();
    assert(owner.update_object_count() == 3 &&
           owner.staged_update_object_count() == 0);
}

void native_flush_reset_retires_only_the_manager_projection() {
    Owner owner;
    GameObject* player = nullptr;
    GameObject* monster = nullptr;
    assert(owner.add_object(3, object(0x3003), &player) == Status::ok);
    assert(owner.add_object(4, object(0x4004), &monster) == Status::ok);

    // Actor transform fields remain owned outside this map. Flush removes the
    // source-key and manager-list projection without writing into borrowed live
    // actor state or requiring another ObjectManager instance.
    float actor_x = 91.0f;
    player->bind_live_fields({&actor_x, nullptr, nullptr, nullptr, nullptr,
                              nullptr, nullptr, nullptr});
    bool added = false;
    assert(owner.register_player_no_room(player->identity, &added) == Status::ok && added);
    assert(owner.add_no_room_object(monster->identity, &added) == Status::ok && added);
    assert(owner.add_room_object(0x7000, &added) == Status::ok && added);
    assert(owner.add_room_object(0x7000, &added) == Status::ok && added);
    owner.add_visible_room_zone();
    owner.add_visible_room_zone();

    assert(owner.object_count() == 2);
    assert(owner.no_room_count() == 2);
    assert(owner.room_object_count() == 2);
    assert(owner.visible_room_zone_count() == 2);
    owner.reset_after_native_flush();

    assert(owner.object_count() == 0);
    assert(owner.no_room_count() == 0);
    assert(owner.room_object_count() == 0);
    assert(owner.visible_room_zone_count() == 0);
    assert(owner.find_by_source_handle(3) == nullptr);
    assert(owner.find_by_source_handle(4) == nullptr);
    Address identity = 0;
    assert(!owner.no_room_at(0, &identity));
    assert(!owner.room_object_at(0, &identity));
    assert(actor_x == 91.0f);

    // Teardown is safe when repeated and the owner can represent a later
    // source lifecycle after external actors have been retired.
    owner.reset_after_native_flush();
    GameObject* next_level = nullptr;
    assert(owner.add_object(3, object(0x5005), &next_level) == Status::ok);
    assert(next_level && next_level->identity == 0x5005);
}

} // namespace

void source_module_filtered_names() {
    Owner owner;
    auto early = object(0x1100), late = object(0x2200);
    early.module_id_64 = 3;
    late.module_id_64 = 4;
    GameObject *first = nullptr, *second = nullptr;
    assert(owner.add_named_object(103, "Merchant", late, &second) == Status::ok);
    assert(owner.add_named_object(-2, "Merchant", early, &first) == Status::ok);
    assert(owner.find_by_name("Merchant", 4) == second);
    assert(owner.find_by_name("Merchant", 3) == first);
    assert(owner.find_by_name("Merchant", 103) == nullptr);
    assert(owner.find_by_name("Merchant", -1) == first);
    assert(owner.find_by_name("merchant", -1) == nullptr);
    std::int32_t live_module = 5;
    std::uint8_t live_global = 0;
    first->live_fields.module_id_64 = &live_module;
    first->live_fields.global_87 = &live_global;
    assert(owner.find_by_name("Merchant", 3) == nullptr);
    assert(owner.find_by_name("Merchant", 5) == first);
    live_global = 7; // The original accepts any nonzero byte, not only 1.
    assert(owner.find_by_name("Merchant", 4) == first);
    const Owner& read_only = owner;
    assert(read_only.find_by_name("Merchant", 99) == first);
    SourceHandle reservation = -1;
    bool created = false;
    assert(owner.get_or_reserve_named_handle("Reserved", true, &reservation, &created) == Status::ok && created);
    assert(owner.find_by_name("Reserved", -1) == nullptr);
}

int main() {
    source_module_filtered_names();
    ordered_map_and_stable_identity();
    named_handle_create_miss_counter_and_collision();
    canonical_views();
    borrowed_actor_fields_remain_canonical();
    no_room_list_and_player_path();
    active_room_list_source_duplicate_and_delete();
    active_update_initpost_spawn_splice_and_retirement();
    native_flush_reset_retires_only_the_manager_projection();
    std::cout << "ObjectManager runtime owner v1 host checks passed: ordered registration, manager lists, bounded post-Flush reset\n";
}
