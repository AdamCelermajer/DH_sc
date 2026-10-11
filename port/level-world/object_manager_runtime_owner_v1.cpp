#include "object_manager_runtime_owner_v1.hpp"

#include <algorithm>
#include <iterator>
#include <limits>
#include <new>

namespace dh2::object_manager_runtime_owner_v1 {

dh2::game_object_zoning_visibility::GameObject GameObject::zoning_view() noexcept {
    auto* room_zone = live_fields.room_zone_2f4
        ? live_fields.room_zone_2f4 : &room_zone_2f4;
    auto* zoning_enabled = live_fields.zoning_enabled_2ee
        ? live_fields.zoning_enabled_2ee : &zoning_enabled_2ee;
    auto* in_zone = live_fields.in_zone_2f0
        ? live_fields.in_zone_2f0 : &in_zone_2f0;
    auto* visible = live_fields.visible_80
        ? live_fields.visible_80 : &visible_80;
    auto* visual_object = live_fields.visual_object_2d8
        ? live_fields.visual_object_2d8 : &visual_object_2d8;
    visual_view_.identity = *visual_object;
    visual_view_.owner_identity = identity;
    return {identity,
            is_zonable_target_c4,
            set_updating_target_3c,
            *room_zone,
            *visual_object ? &visual_view_ : nullptr,
            *zoning_enabled,
            *in_zone,
            *visible};
}

dh2::room_zone_enrollment::GameObject GameObject::enrollment_view() noexcept {
    auto* world_x_field = live_fields.world_x ? live_fields.world_x : &this->world_x;
    auto* world_y_field = live_fields.world_y ? live_fields.world_y : &this->world_y;
    auto* room_zone = live_fields.room_zone_2f4
        ? live_fields.room_zone_2f4 : &room_zone_2f4;
    auto* in_room_list = live_fields.in_room_list_2ef
        ? live_fields.in_room_list_2ef : &in_room_list_2ef;
    auto* in_zone = live_fields.in_zone_2f0
        ? live_fields.in_zone_2f0 : &in_zone_2f0;
    auto* zoning_enabled = live_fields.zoning_enabled_2ee
        ? live_fields.zoning_enabled_2ee : &zoning_enabled_2ee;
    auto* visual_object = live_fields.visual_object_2d8
        ? live_fields.visual_object_2d8 : &visual_object_2d8;
    auto* visible = live_fields.visible_80
        ? live_fields.visible_80 : &visible_80;
    return {identity,
            world_x_field,
            world_y_field,
            room_zone,
            in_room_list,
            in_zone,
            zoning_enabled,
            visual_object,
            visible};
}

Status Owner::add_object(SourceHandle source_handle, const GameObject& object,
                         GameObject** stored) noexcept {
    return add_named_object(source_handle, {}, object, stored);
}

Status Owner::add_named_object(SourceHandle source_handle, std::string_view name,
                               const GameObject& object,
                               GameObject** stored) noexcept {
    if (stored == nullptr || object.identity == 0) return Status::invalid_argument;
    *stored = nullptr;
    auto existing = objects_.find(source_handle);
    if (existing != objects_.end()) {
        // GetObjectByName may reserve the keyed ObjectListItem before the
        // factory's Add publishes its concrete object. Fill that exact row in
        // place; unrelated or differently named rows remain collisions.
        if (existing->second.identity != 0) return Status::duplicate_source_handle;
        if (!name.empty() && existing->second.name != name)
            return Status::source_handle_collision;
        if (find_by_identity(object.identity) != nullptr)
            return Status::duplicate_identity;
        try {
            GameObject value = object;
            value.source_handle = source_handle;
            value.name = existing->second.name;
            value.visual_view_ = {};
            existing->second = std::move(value);
            *stored = &existing->second;
            return Status::ok;
        } catch (...) {
            return Status::allocation_failed;
        }
    }
    if (find_by_identity(object.identity) != nullptr) return Status::duplicate_identity;

    try {
        GameObject value = object;
        value.source_handle = source_handle;
        if (!name.empty()) value.name.assign(name.data(), name.size());
        else value.name.clear();
        value.visual_view_ = {};
        const auto inserted = objects_.emplace(source_handle, value);
        if (!inserted.second) return Status::duplicate_source_handle;
        if (!name.empty()) {
            try {
                names_[inserted.first->second.name].insert(source_handle);
            } catch (...) {
                const auto indexed = names_.find(inserted.first->second.name);
                if (indexed != names_.end() && indexed->second.empty()) names_.erase(indexed);
                objects_.erase(inserted.first);
                return Status::allocation_failed;
            }
        }
        *stored = &inserted.first->second;
        // SourceLevel construction may publish an already assigned ordered
        // ledger key before any dynamic GetObjectByName call. Keep the one
        // canonical +0x4c counter beyond every successfully retained key so
        // later runtime spawns (including Module-created RoomZones) continue
        // the same allocation stream instead of retrying from handle 1.
        if (source_handle >= next_source_handle_) {
            next_source_handle_ = source_handle ==
                    std::numeric_limits<SourceHandle>::max()
                ? source_handle : static_cast<SourceHandle>(source_handle + 1);
        }
        return Status::ok;
    } catch (const std::bad_alloc&) {
        return Status::allocation_failed;
    } catch (...) {
        return Status::allocation_failed;
    }
}

Status Owner::get_or_reserve_named_handle(std::string_view name,
                                           bool create_if_missing,
                                           SourceHandle* source_handle,
                                           bool* created) noexcept {
    if (!source_handle || !created || name.empty()) return Status::invalid_argument;
    *source_handle = 0;
    *created = false;
    if (const auto* existing = find_by_name(name)) {
        *source_handle = existing->source_handle;
        return Status::ok;
    }
    if (!create_if_missing) return Status::not_found;
    if (next_source_handle_ == std::numeric_limits<SourceHandle>::max())
        return Status::source_handle_exhausted;

    // The ARM source pre-increments +0x4c, then indexes the signed-key map.
    // Preserve the consumed key on a collision/failure, matching that order.
    const SourceHandle reserved = next_source_handle_++;
    *source_handle = reserved;
    const auto occupied = objects_.find(reserved);
    if (occupied != objects_.end()) return Status::source_handle_collision;

    try {
        GameObject row{};
        row.source_handle = reserved;
        row.name.assign(name.data(), name.size());
        const auto inserted = objects_.emplace(reserved, std::move(row));
        if (!inserted.second) return Status::source_handle_collision;
        try {
            names_[inserted.first->second.name].insert(reserved);
        } catch (...) {
            const auto indexed = names_.find(inserted.first->second.name);
            if (indexed != names_.end() && indexed->second.empty()) names_.erase(indexed);
            objects_.erase(inserted.first);
            return Status::allocation_failed;
        }
        *created = true;
        return Status::ok;
    } catch (...) {
        return Status::allocation_failed;
    }
}

Status Owner::remove_object(SourceHandle source_handle, bool* removed) noexcept {
    if (removed == nullptr) return Status::invalid_argument;
    *removed = false;
    auto found = objects_.find(source_handle);
    if (found == objects_.end()) return Status::not_found;

    bool no_room_removed = false;
    if (found->second.identity != 0) {
        update_objects_.remove(found->second.identity);
        staged_update_objects_.remove(found->second.identity);
        const Status no_room_status =
            remove_no_room_object(found->second.identity, &no_room_removed);
        (void)no_room_removed;
        if (no_room_status != Status::ok) return no_room_status;
    }
    if (!found->second.name.empty()) {
        const auto named = names_.find(found->second.name);
        if (named != names_.end()) {
            named->second.erase(source_handle);
            if (named->second.empty()) names_.erase(named);
        }
    }
    objects_.erase(found);
    *removed = true;
    return Status::ok;
}

Status Owner::append_init_post_update_object(Address identity,
                                             bool source_is_updatable,
                                             bool* appended) noexcept {
    if (appended == nullptr || identity == 0) return Status::invalid_argument;
    *appended = false;
    if (!source_is_updatable) return Status::ok;
    if (find_by_identity(identity) == nullptr) return Status::not_found;
    try {
        update_objects_.push_back(identity);
    } catch (...) {
        return Status::allocation_failed;
    }
    *appended = true;
    return Status::ok;
}

Status Owner::stage_spawn_update_object(Address identity,
                                        bool source_is_updatable,
                                        bool* staged) noexcept {
    if (staged == nullptr || identity == 0) return Status::invalid_argument;
    *staged = false;
    if (!source_is_updatable) return Status::ok;
    if (find_by_identity(identity) == nullptr) return Status::not_found;
    try {
        staged_update_objects_.push_back(identity);
    } catch (...) {
        return Status::allocation_failed;
    }
    *staged = true;
    return Status::ok;
}

void Owner::begin_update_frame() noexcept {
    update_objects_.splice(update_objects_.end(), staged_update_objects_);
}

bool Owner::update_object_at(std::size_t index, GameObject** object) noexcept {
    if (object == nullptr) return false;
    *object = nullptr;
    if (index >= update_objects_.size()) return false;
    auto found = update_objects_.begin();
    std::advance(found, static_cast<std::ptrdiff_t>(index));
    *object = find_by_identity(*found);
    return *object != nullptr;
}

Status Owner::visit_update_objects(void* context, UpdateCallback callback,
                                   std::size_t* visited) noexcept {
    if (callback == nullptr || visited == nullptr) return Status::invalid_argument;
    *visited = 0;
    for (const Address identity : update_objects_) {
        GameObject* object = find_by_identity(identity);
        if (object == nullptr) return Status::not_found;
        if (!callback(context, object)) return Status::update_callback_failed;
        ++*visited;
    }
    return Status::ok;
}

GameObject* Owner::find_by_source_handle(SourceHandle source_handle) noexcept {
    const auto found = objects_.find(source_handle);
    return found == objects_.end() ? nullptr : &found->second;
}

const GameObject* Owner::find_by_source_handle(SourceHandle source_handle) const noexcept {
    const auto found = objects_.find(source_handle);
    return found == objects_.end() ? nullptr : &found->second;
}

GameObject* Owner::find_by_identity(Address identity) noexcept {
    if (identity == 0) return nullptr;
    for (auto& item : objects_) {
        if (item.second.identity == identity) return &item.second;
    }
    return nullptr;
}

const GameObject* Owner::find_by_identity(Address identity) const noexcept {
    if (identity == 0) return nullptr;
    for (const auto& item : objects_) {
        if (item.second.identity == identity) return &item.second;
    }
    return nullptr;
}

GameObject* Owner::find_by_name(std::string_view name) noexcept {
    const auto found = names_.find(name);
    if (found == names_.end() || found->second.empty()) return nullptr;
    return find_by_source_handle(*found->second.begin());
}

const GameObject* Owner::find_by_name(std::string_view name) const noexcept {
    const auto found = names_.find(name);
    if (found == names_.end() || found->second.empty()) return nullptr;
    return find_by_source_handle(*found->second.begin());
}

GameObject* Owner::find_by_name(std::string_view name, std::int32_t module_filter) noexcept {
    return const_cast<GameObject*>(static_cast<const Owner&>(*this).find_by_name(name, module_filter));
}

const GameObject* Owner::find_by_name(std::string_view name, std::int32_t module_filter) const noexcept {
    const auto found = names_.find(name);
    if (found == names_.end()) return nullptr;
    for (const auto handle : found->second) {
        const auto* object = find_by_source_handle(handle);
        if (!object || !object->identity) continue;
        const auto module = object->live_fields.module_id_64
            ? *object->live_fields.module_id_64 : object->module_id_64;
        const auto global = object->live_fields.global_87
            ? *object->live_fields.global_87 : object->global_87;
        if (module_filter == -1 || module_filter == module || global != 0)
            return object;
    }
    return nullptr;
}

void Owner::reset(Cursor* cursor) const noexcept {
    if (cursor == nullptr) return;
    *cursor = {};
    cursor->finished = objects_.empty();
}

Status Owner::next(Cursor* cursor, GameObject** object) noexcept {
    if (cursor == nullptr || object == nullptr) return Status::invalid_argument;
    *object = nullptr;
    if (cursor->finished) return Status::ok;

    auto found = cursor->started ? objects_.upper_bound(cursor->last_source_handle)
                                 : objects_.begin();
    if (found == objects_.end()) {
        cursor->finished = true;
        return Status::ok;
    }
    cursor->last_source_handle = found->first;
    cursor->started = true;
    *object = &found->second;
    return Status::ok;
}

Status Owner::add_no_room_object(Address identity, bool* added) noexcept {
    if (added == nullptr || identity == 0) return Status::invalid_argument;
    *added = false;
    GameObject* object = find_by_identity(identity);
    if (object == nullptr) return Status::not_found;

    // Source +0x2f8 is the duplicate gate. It is written only after the source
    // has allocated and tail-linked its 12-byte list node.
    if (object->no_room_member_2f8 != 0) return Status::ok;
    try {
        no_room_objects_.push_back(identity);
    } catch (const std::bad_alloc&) {
        return Status::allocation_failed;
    } catch (...) {
        return Status::allocation_failed;
    }
    object->no_room_member_2f8 = 1;
    *added = true;
    return Status::ok;
}

Status Owner::register_player_no_room(Address identity, bool* added) noexcept {
    // PlayerManager::_AddCharacter uses ObjectManager::AddNoRoomObject when
    // RoomZone::AddInitialObject rejects the player. This is a named call path
    // into the same source list authority, not a second Player membership list.
    return add_no_room_object(identity, added);
}

Status Owner::remove_no_room_object(Address identity, bool* removed) noexcept {
    if (removed == nullptr || identity == 0) return Status::invalid_argument;
    *removed = false;
    auto found = std::find(no_room_objects_.begin(), no_room_objects_.end(), identity);
    if (found == no_room_objects_.end()) return Status::ok;

    no_room_objects_.erase(found);
    if (GameObject* object = find_by_identity(identity)) {
        object->no_room_member_2f8 = 0;
    }
    *removed = true;
    return Status::ok;
}

Status Owner::add_room_object(Address identity, bool* added) noexcept {
    if (added == nullptr || identity == 0) return Status::invalid_argument;
    *added = false;
    // Source AddRoomObjects asserts on an existing identity in debug builds,
    // but still allocates and tail-inserts the new node after the assertion.
    try {
        room_objects_.push_back(identity);
    } catch (const std::bad_alloc&) {
        return Status::allocation_failed;
    } catch (...) {
        return Status::allocation_failed;
    }
    *added = true;
    return Status::ok;
}

Status Owner::remove_room_object(Address identity, bool* removed) noexcept {
    if (removed == nullptr || identity == 0) return Status::invalid_argument;
    *removed = false;
    for (auto found = room_objects_.begin(); found != room_objects_.end();) {
        if (*found == identity) {
            found = room_objects_.erase(found);
            *removed = true;
        } else {
            ++found;
        }
    }
    return Status::ok;
}

bool Owner::room_object_at(std::size_t index, Address* identity) const noexcept {
    if (identity == nullptr || index >= room_objects_.size()) return false;
    auto found = room_objects_.begin();
    std::advance(found, static_cast<std::ptrdiff_t>(index));
    *identity = *found;
    return true;
}

bool Owner::no_room_at(std::size_t index, Address* identity) const noexcept {
    if (identity == nullptr || index >= no_room_objects_.size()) return false;
    auto found = no_room_objects_.begin();
    std::advance(found, static_cast<std::ptrdiff_t>(index));
    *identity = *found;
    return true;
}

void Owner::reset_after_native_flush() noexcept {
    // Drop only this port-owned projection once another owner has completed
    // the source destruction phase. This does not reproduce the other
    // ObjectManager collections or native destructor side effects.
    objects_.clear();
    names_.clear();
    no_room_objects_.clear();
    room_objects_.clear();
    update_objects_.clear();
    staged_update_objects_.clear();
    visible_room_zone_count_ = 0;
    next_source_handle_ = 1;
}

} // namespace dh2::object_manager_runtime_owner_v1
