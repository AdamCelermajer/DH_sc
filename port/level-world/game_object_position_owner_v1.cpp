#include "game_object_position_owner_v1.hpp"

namespace dh2::game_object_position_owner_v1 {
namespace {
bool valid(const Owner* owner, const Services* services) {
    return owner && owner->identity && services &&
        (!owner->instance_transform || services->translate_instance_transform) &&
        services->update_absolute_aabb &&
        (!owner->physical_object || services->physical_set_position) &&
        (!owner->visual_object || services->visual_sync_position);
}
}

Status set_position(Owner* owner, Point3 position, bool update_destination,
                    const Services* services, std::string& error) {
    if (!valid(owner, services) ||
        (update_destination && !services->set_destination)) {
        error = "GameObject position service set is incomplete";
        return Status::invalid_argument;
    }

    if (owner->instance_transform) {
        const Point3 delta{position.x - owner->position.x,
                           position.y - owner->position.y,
                           position.z - owner->position.z};
        if (services->translate_instance_transform(services->context,
                owner->identity, owner->instance_transform, delta, error))
            return Status::service_failed;
    }

    owner->position = position;
    if (services->update_absolute_aabb(services->context, owner->identity,
                                      error))
        return Status::service_failed;
    if (owner->physical_object && services->physical_set_position(
            services->context, owner->identity, owner->physical_object,
            position.x, position.y, error))
        return Status::service_failed;
    if (owner->visual_object && services->visual_sync_position(
            services->context, owner->identity, owner->visual_object, error))
        return Status::service_failed;
    if (update_destination && services->set_destination(services->context,
            owner->identity, position, error))
        return Status::service_failed;
    return Status::complete;
}

Status force_update_position(const Owner* owner, const Services* services,
                             std::string& error) {
    if (!owner || !owner->identity || !services ||
        (owner->visual_object && !services->visual_force_update_position)) {
        error = "GameObject force-update service set is incomplete";
        return Status::invalid_argument;
    }
    if (owner->visual_object && services->visual_force_update_position(
            services->context, owner->identity, owner->visual_object, error))
        return Status::service_failed;
    return Status::complete;
}

} // namespace dh2::game_object_position_owner_v1
