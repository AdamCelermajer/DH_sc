#include "native_character_roster_transaction.hpp"

namespace dh2::native::character_roster_transaction {

Status register_after_add(Manager* manager, CharacterList* characters,
                          SourceHandle handle, const ManagerObject* object,
                          Character* character, bool duplicate_resolved,
                          Result* result, std::string_view manager_name) noexcept {
    if (!result) return Status::invalid_argument;
    *result = {};
    if (duplicate_resolved) {
        result->status = Status::duplicate_noop;
        return result->status;
    }
    if (!manager || !characters || !object || !character ||
        object->identity == 0 || character->identity == 0 ||
        character->identity != object->identity || !character->object ||
        character->object->identity != character->identity) {
        result->status = Status::invalid_argument;
        return result->status;
    }

    std::size_t listed_identities = 0;
    const bool listed = characters->contains_identity(character->identity,
                                                       &listed_identities);
    if (manager->find_by_identity(object->identity) || listed) {
        result->status = Status::duplicate_identity;
        return result->status;
    }
    if (manager->find_by_source_handle(handle)) {
        result->manager_status =
            dh2::object_manager_runtime_owner_v1::Status::duplicate_source_handle;
        result->status = Status::object_manager_failed;
        return result->status;
    }

    dh2::object_manager_runtime_owner_v1::GameObject* stored = nullptr;
    result->manager_status = manager_name.empty()
        ? manager->add_object(handle, *object, &stored)
        : manager->add_named_object(handle, manager_name, *object, &stored);
    if (result->manager_status !=
            dh2::object_manager_runtime_owner_v1::Status::ok || !stored) {
        result->status = Status::object_manager_failed;
        return result->status;
    }
    result->map_committed = true;

    bool appended = false;
    result->character_status = characters->enroll_after_add(
        character, false, &appended);
    if (result->character_status ==
            dh2::native::character_list::Owner::Status::ok && appended) {
        result->character_committed = true;
        result->status = Status::complete;
        return result->status;
    }

    bool removed = false;
    const auto rollback = manager->remove_object(handle, &removed);
    if (rollback == dh2::object_manager_runtime_owner_v1::Status::ok && removed) {
        result->map_committed = false;
        result->map_rolled_back = true;
        result->status = Status::character_list_failed;
        return result->status;
    }
    result->manager_status = rollback;
    result->status = Status::rollback_failed;
    return result->status;
}

Status remove_after_remove(Manager* manager, CharacterList* characters,
                           SourceHandle handle, Character* character,
                           Result* result) noexcept {
    if (!result) return Status::invalid_argument;
    *result = {};
    if (!manager || !characters || !character || character->identity == 0) {
        result->status = Status::invalid_argument;
        return result->status;
    }

    const auto* object = manager->find_by_source_handle(handle);
    if (!object || object->identity != character->identity) {
        result->status = Status::not_registered;
        return result->status;
    }
    std::size_t listed_identities = 0;
    if (!characters->contains_identity(character->identity, &listed_identities) ||
        listed_identities != 1) {
        result->status = Status::inconsistent_registration;
        return result->status;
    }

    result->character_status =
        characters->remove_after_remove(character, &result->removed_character_nodes);
    if (result->character_status !=
            dh2::native::character_list::Owner::Status::ok ||
        result->removed_character_nodes != 1) {
        result->status = Status::teardown_failed;
        return result->status;
    }
    result->character_committed = false;

    bool removed = false;
    result->manager_status = manager->remove_object(handle, &removed);
    if (result->manager_status !=
            dh2::object_manager_runtime_owner_v1::Status::ok || !removed) {
        // The preflighted owner operations should make this unreachable absent
        // concurrent mutation or an owner invariant violation.
        result->status = Status::inconsistent_registration;
        return result->status;
    }
    result->map_committed = false;
    result->status = Status::complete;
    return result->status;
}

} // namespace dh2::native::character_roster_transaction
