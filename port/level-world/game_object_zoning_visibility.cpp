#include "game_object_zoning_visibility.hpp"

#include <limits>

namespace dh2::game_object_zoning_visibility { namespace {

bool aligned_range(const void* pointer, std::size_t size, std::size_t alignment) {
    if (!pointer || (reinterpret_cast<Address>(pointer) % alignment) != 0) return false;
    return reinterpret_cast<Address>(pointer) <=
        std::numeric_limits<Address>::max() - size;
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const Address a = reinterpret_cast<Address>(left);
    const Address b = reinterpret_cast<Address>(right);
    return a < b + right_size && b < a + left_size;
}

bool valid_services_result(const Services* services, const Result* result) {
    if (!aligned_range(services, sizeof(*services), alignof(Services)) ||
        !aligned_range(result, sizeof(*result), alignof(Result)) ||
        !services->invoke || overlaps(services, sizeof(*services), result, sizeof(*result)))
        return false;
    if (services->context_extent) {
        if (!aligned_range(services->context, services->context_extent, alignof(std::max_align_t)) ||
            overlaps(services->context, services->context_extent, services, sizeof(*services)) ||
            overlaps(services->context, services->context_extent, result, sizeof(*result))) return false;
    }
    return true;
}

bool valid_common(const GameObject* object, const Services* services,
                  const Result* result) {
    if (!aligned_range(object, sizeof(*object), alignof(GameObject)) ||
        !object->identity || !valid_services_result(services, result)) return false;
    if (overlaps(object, sizeof(*object), services, sizeof(*services)) ||
        overlaps(object, sizeof(*object), result, sizeof(*result)) ||
        overlaps(services, sizeof(*services), result, sizeof(*result))) return false;
    if (services->context_extent &&
        overlaps(services->context, services->context_extent, object, sizeof(*object))) return false;
    return true;
}

bool valid_visual_memory(const VisualObject* visual, const Services* services,
                         const Result* result) {
    if (!aligned_range(visual, sizeof(*visual), alignof(VisualObject))) return false;
    if (overlaps(visual, sizeof(*visual), services, sizeof(*services)) ||
        overlaps(visual, sizeof(*visual), result, sizeof(*result))) return false;
    if (services->context_extent &&
        overlaps(visual, sizeof(*visual), services->context, services->context_extent)) return false;
    return true;
}

bool valid_visual(const VisualObject* visual, const GameObject* owner,
                  const Services* services, const Result* result) {
    if (!valid_visual_memory(visual, services, result)) return false;
    if (!owner) return visual->identity != 0 && visual->owner_identity == 0;
    if (!aligned_range(owner, sizeof(*owner), alignof(GameObject)) ||
        visual->identity == 0 || visual->owner_identity != owner->identity) return false;
    return !overlaps(visual, sizeof(*visual), owner, sizeof(*owner));
}

Status invoke(const Services& services, Result& result, Operation operation,
              Address object, Address related, std::uint32_t argument,
              std::uint32_t* raw = nullptr) {
    if (!services.invoke) return Status::service_unavailable;
    const Request request{operation, object, related, argument};
    std::uint32_t value = 0;
    ++result.service_calls;
    try {
        if (services.invoke(services.context, &request, raw ? raw : &value) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    if (raw) result.last_raw_result = *raw;
    return Status::complete;
}

Status query_zonable(GameObject& object, const Services& services, Result& result,
                     std::uint32_t& raw) {
    // The source loads the current vptr and +0xc4 target immediately before
    // each dispatch; a prior callback may have changed the projection.
    const Address target = object.is_zonable_target_c4;
    if (!target) return Status::invalid_source_fact;
    return invoke(services, result, Operation::is_zonable,
                  object.identity, target, 0, &raw);
}

void finish(Result& result, const GameObject& object) {
    result.final_room_zone = object.room_zone_2f4;
    result.final_zoning_enabled = object.zoning_enabled_2ee;
    result.final_in_zone = object.in_zone_2f0;
    result.final_visibility = object.visibility_80;
}

Status sync_impl(VisualObject* visual, GameObject* owner,
                 const Services* services, Result* result) {
    if (!visual || !valid_visual(visual, owner, services, result))
        return Status::invalid_source_fact;
    if (!owner) return Status::complete;

    // VisualObject::SyncVisibility captures owner=visual+4, then reads the
    // current visibility byte. It does not use the scene node's own bit here.
    if (!owner->visibility_80) {
        const auto status = invoke(*services, *result, Operation::set_visible,
                                   visual->identity, owner->identity, 0);
        if (status == Status::complete) {
            result->last_visible_argument = 0;
            ++result->source_writes;
        }
        finish(*result, *owner);
        return status;
    }

    std::uint32_t zonable = 0;
    auto status = query_zonable(*owner, *services, *result, zonable);
    if (status != Status::complete) { finish(*result, *owner); return status; }

    // The source selects true unless the owner is zonable AND zoning is
    // currently enabled AND the fresh in-zone byte is zero.
    const bool visible = !(zonable && owner->zoning_enabled_2ee &&
                           owner->in_zone_2f0 == 0);
    status = invoke(*services, *result, Operation::set_visible,
                    visual->identity, owner->identity, visible ? 1u : 0u);
    if (status == Status::complete) {
        result->last_visible_argument = visible ? 1u : 0u;
        ++result->source_writes;
    }
    finish(*result, *owner);
    return status;
}

Status set_updating(GameObject* object, const Services* services, Result* result,
                    std::uint32_t raw_zonable, Address captured_target) {
    // The source virtual target is captured before IsZonable, while its bool
    // argument is selected from fresh +2ee/+2f0 reads afterward.
    const std::uint32_t value = raw_zonable && object->zoning_enabled_2ee
        ? static_cast<std::uint32_t>(object->in_zone_2f0) : 1u;
    const auto status = invoke(*services, *result, Operation::set_updating,
                               object->identity, captured_target, value);
    if (status == Status::complete) {
        result->last_updating_argument = value;
        ++result->source_writes;
    }
    finish(*result, *object);
    return status;
}

Status common_enable_tail(GameObject* object, const Services* services,
                          Result* result) {
    // This block is reached after the optional SyncVisibility call. The
    // source captures +0x3c immediately before the fresh IsZonable call.
    const Address captured_target = object->set_updating_target_3c;
    if (!captured_target) { finish(*result, *object); return Status::invalid_source_fact; }
    std::uint32_t raw = 0;
    auto status = query_zonable(*object, *services, *result, raw);
    if (status != Status::complete) { finish(*result, *object); return status; }
    return set_updating(object, services, result, raw, captured_target);
}

Status begin(GameObject* object, const Services* services, Result* result) {
    if (!result || !valid_common(object, services, result)) return Status::invalid_argument;
    *result = {};
    return Status::complete;
}

} // namespace

Status disable_zoning(GameObject* object, const Services* services, Result* result) {
    auto status = begin(object, services, result);
    if (status != Status::complete) return status;

    if (object->zoning_enabled_2ee) {
        const Address old_room = object->room_zone_2f4;
        if (old_room) {
            status = invoke(*services, *result, Operation::room_remove_object,
                            old_room, object->identity, 0);
            if (status != Status::complete) { finish(*result, *object); return status; }
        }
        status = invoke(*services, *result, Operation::object_manager_add_no_room,
                        object->identity, 0, 0);
        if (status != Status::complete) { finish(*result, *object); return status; }
        object->zoning_enabled_2ee = 0;
        ++result->source_writes;
    }

    // ldr [vptr,#0x3c] is before the +0xc4 IsZonable virtual dispatch.
    const Address captured_target = object->set_updating_target_3c;
    if (!captured_target) { finish(*result, *object); return Status::invalid_source_fact; }
    std::uint32_t zonable = 0;
    status = query_zonable(*object, *services, *result, zonable);
    if (status != Status::complete) { finish(*result, *object); return status; }
    return set_updating(object, services, result, zonable, captured_target);
}

Status enable_zoning(GameObject* object, const Services* services, Result* result) {
    auto status = begin(object, services, result);
    if (status != Status::complete) return status;

    if (object->zoning_enabled_2ee) {
        std::uint32_t zonable = 0;
        status = query_zonable(*object, *services, *result, zonable);
        if (status != Status::complete) { finish(*result, *object); return status; }
        if (zonable) {
            // +2d8 is loaded after the first IsZonable callback. The pointer
            // projection is therefore refreshed after that callback.
            VisualObject* visual = object->visual_2d8;
            if (visual) {
                status = sync_impl(visual, object, services, result);
                if (status != Status::complete) { finish(*result, *object); return status; }
            }
        }
        return common_enable_tail(object, services, result);
    }

    // Zero->one path stores +2ee before manager/list calls; a later service
    // failure intentionally keeps this and all preceding source mutations.
    object->zoning_enabled_2ee = 1;
    ++result->source_writes;
    status = invoke(*services, *result, Operation::object_manager_remove_no_room,
                    object->identity, 0, 0);
    if (status != Status::complete) { finish(*result, *object); return status; }

    Address room = object->room_zone_2f4;
    if (room) {
        status = invoke(*services, *result, Operation::room_add_object,
                        room, object->identity, 0);
        if (status != Status::complete) { finish(*result, *object); return status; }

        // The source reloads +2f4 after AddObject, then queries its +0x389
        // in-zone byte. AddObject may have changed which room is current.
        room = object->room_zone_2f4;
        // There is no second null check in the source before loading byte
        // +0x389. If a callback replaces the pointer, the current value (even
        // zero) is the exact address base supplied to this source read.
        std::uint32_t room_is_zoned = 0;
        status = invoke(*services, *result, Operation::room_is_zoned,
                        room, object->identity, 0, &room_is_zoned);
        if (status != Status::complete) { finish(*result, *object); return status; }
        if (room_is_zoned > 0xffu) {
            finish(*result, *object);
            return Status::invalid_source_fact;
        }
        status = invoke(*services, *result,
                        room_is_zoned ? Operation::zone_entered : Operation::zone_exited,
                        object->identity, room, 0);
        if (status != Status::complete) { finish(*result, *object); return status; }
    }

    std::uint32_t zonable = 0;
    status = query_zonable(*object, *services, *result, zonable);
    if (status != Status::complete) { finish(*result, *object); return status; }
    if (zonable) {
        VisualObject* visual = object->visual_2d8;
        if (visual) {
            status = sync_impl(visual, object, services, result);
            if (status != Status::complete) { finish(*result, *object); return status; }
        }
    }
    return common_enable_tail(object, services, result);
}

Status sync_visibility(VisualObject* visual, GameObject* owner,
                       const Services* services, Result* result) {
    if (!result || !visual || !valid_services_result(services, result) ||
        !valid_visual_memory(visual, services, result))
        return Status::invalid_argument;
    *result = {};
    if (!owner) {
        if (visual->identity != 0 && visual->owner_identity == 0) return Status::complete;
        return Status::invalid_source_fact;
    }
    if (!valid_common(owner, services, result) || !valid_visual(visual, owner, services, result))
        return Status::invalid_source_fact;
    return sync_impl(visual, owner, services, result);
}

} // namespace dh2::game_object_zoning_visibility
