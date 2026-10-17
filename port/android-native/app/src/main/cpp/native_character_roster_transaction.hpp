#pragma once

#include "native_character_list.hpp"
#include "../../../../../../port/level-world/object_manager_runtime_owner_v1.hpp"
#include <string_view>

namespace dh2::native::character_roster_transaction {

using Manager = dh2::object_manager_runtime_owner_v1::Owner;
using ManagerObject = dh2::object_manager_runtime_owner_v1::GameObject;
using Character = dh2::native::character_list::Owner::Character;
using CharacterList = dh2::native::character_list::Owner;
using SourceHandle = dh2::object_manager_runtime_owner_v1::SourceHandle;

enum class Status : unsigned char {
    complete,
    duplicate_noop,
    invalid_argument,
    duplicate_identity,
    object_manager_failed,
    character_list_failed,
    rollback_failed,
    not_registered,
    inconsistent_registration,
    teardown_failed,
};

struct Result {
    Status status = Status::invalid_argument;
    dh2::object_manager_runtime_owner_v1::Status manager_status =
        dh2::object_manager_runtime_owner_v1::Status::ok;
    dh2::native::character_list::Owner::Status character_status =
        dh2::native::character_list::Owner::Status::ok;
    bool map_committed = false;
    bool character_committed = false;
    bool map_rolled_back = false;
    std::size_t removed_character_nodes = 0;
};

// Commit one already-created native Character projection to both source-shaped
// owners. This does not create or own the Character; the Character projection
// and its GameObject view remain borrowed from their actor owner.
Status register_after_add(Manager*, CharacterList*, SourceHandle,
                          const ManagerObject*, Character*,
                          bool duplicate_resolved, Result*,
                          std::string_view manager_name = {}) noexcept;

// Remove a previously committed pair. Character-list unlink is attempted
// first so malformed-list failures leave the map entry intact. A valid,
// preflighted ObjectManager removal is allocation-free and follows the unlink.
Status remove_after_remove(Manager*, CharacterList*, SourceHandle,
                           Character*, Result*) noexcept;

} // namespace dh2::native::character_roster_transaction
