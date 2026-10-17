#include "room_zone_enrollment.hpp"

namespace dh2::room_zone_enrollment { namespace {
bool valid(const RoomZone* zone, const GameObject* object, const Services* services, const Result* out) {
    return zone && zone->identity && zone->bounds.min_x && zone->bounds.min_y &&
        zone->bounds.max_x && zone->bounds.max_y && object && object->identity &&
        object->world_x && object->world_y && object->room_zone && object->in_room_list &&
        object->in_zone && object->zoning_enabled && object->visual_object &&
        object->visible && services && services->invoke && out;
}

Status call(const Services& services, Result& out, Operation operation,
            Address object, Address zone, Address auxiliary, std::uint32_t value,
            std::uint32_t* result = nullptr) {
    const Request request{operation, object, zone, auxiliary, value};
    std::uint32_t ignored = 0;
    ++out.calls;
    try {
        return services.invoke(services.context, &request, result ? result : &ignored)
            ? Status::service_failed : Status::complete;
    } catch (...) {
        return Status::service_failed;
    }
}

void finish(Result& out, const GameObject& object) {
    out.resulting_room_zone = *object.room_zone;
    out.resulting_in_room_list = *object.in_room_list;
    out.resulting_in_zone = *object.in_zone;
}

Status change_zone(GameObject* object, const Services* services, Result* out, bool entering) {
    // Bounds validation is intentionally unnecessary for the source leaf, but
    // shared projections still require stable callback/field owners.
    if (!object || !object->identity || !object->room_zone || !object->in_room_list ||
        !object->in_zone || !object->zoning_enabled || !object->visual_object ||
        !object->visible || !services || !services->invoke || !out)
        return Status::invalid_argument;
    *object->in_zone = entering ? 1 : 0;

    // Both leaves may synchronize a VisualObject. Entry additionally gates
    // that call on the GameObject's current +0x80 visibility byte.
    if (*object->zoning_enabled && *object->visual_object &&
        (!entering || *object->visible)) {
        const auto status = call(*services, *out, Operation::sync_visibility,
                                 object->identity, *object->room_zone,
                                 *object->visual_object, 0);
        if (status != Status::complete) { finish(*out, *object); return status; }
    }

    std::uint32_t zonable = 0;
    auto status = call(*services, *out, Operation::is_zonable,
                       object->identity, *object->room_zone, 0, 0, &zonable);
    if (status != Status::complete) { finish(*out, *object); return status; }

    // The source reloads both +2ee and +2f0 after IsZonable. If the virtual
    // provider mutates them, the callback sees those fresh values.
    const std::uint32_t callback_value = (zonable && *object->zoning_enabled)
        ? *object->in_zone : 1u;
    status = call(*services, *out, Operation::zone_state_callback,
                  object->identity, *object->room_zone, 0, callback_value);
    finish(*out, *object);
    return status;
}
} // namespace

Status add_initial_object(RoomZone* zone, GameObject* object,
                          const Services* services, Result* out) {
    if (!valid(zone, object, services, out)) return Status::invalid_argument;
    *out = {};

    std::uint32_t zonable = 0;
    auto status = call(*services, *out, Operation::is_zonable,
                       object->identity, *object->room_zone, 0, 0, &zonable);
    if (status != Status::complete) { finish(*out, *object); return status; }
    if (!zonable) {
        out->rejection = Rejection::not_zonable;
        finish(*out, *object);
        return Status::complete;
    }

    // The source captures object X once, then performs the four calls to
    // __aeabi_fcmple in order: minX<=X, X<=maxX, minY<=Y, Y<=maxY.
    const float x = *object->world_x;
    if (!(*zone->bounds.min_x <= x) || !(x <= *zone->bounds.max_x)) {
        out->rejection = Rejection::outside_bounds;
        finish(*out, *object);
        return Status::complete;
    }
    const float y = *object->world_y;
    if (!(*zone->bounds.min_y <= y) || !(y <= *zone->bounds.max_y)) {
        out->rejection = Rejection::outside_bounds;
        finish(*out, *object);
        return Status::complete;
    }

    // +2ef is the source's current-membership gate: a nonzero byte permits
    // removal from a previous RoomZone before assignment; zero skips it.
    // DebugSwitches logging associated with that branch is omitted.
    const bool was_in_room_list = *object->in_room_list != 0;
    const Address old_zone = *object->room_zone;
    if (was_in_room_list && old_zone) {
        status = call(*services, *out, Operation::remove_from_room,
                      object->identity, old_zone, 0, 0);
        if (status != Status::complete) { finish(*out, *object); return status; }
    }

    *object->room_zone = zone->identity;
    *object->in_room_list = 1;
    status = change_zone(object, services, out, true);
    if (status != Status::complete) { finish(*out, *object); return status; }
    status = call(*services, *out, Operation::append_to_room,
                  object->identity, zone->identity, 0, 0);
    out->accepted = status == Status::complete;
    finish(*out, *object);
    return status;
}

Status zone_entered(GameObject* object, const Services* services, Result* out) {
    // The source zone event does not read bounds, but its +2f4 room pointer is
    // live and is passed to callback adapters for lifecycle diagnostics.
    if (!out) return Status::invalid_argument;
    *out = {};
    return change_zone(object, services, out, true);
}

Status zone_exited(GameObject* object, const Services* services, Result* out) {
    if (!out) return Status::invalid_argument;
    *out = {};
    return change_zone(object, services, out, false);
}

} // namespace dh2::room_zone_enrollment
