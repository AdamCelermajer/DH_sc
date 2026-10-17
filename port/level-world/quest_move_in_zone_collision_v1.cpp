#include "quest_move_in_zone_collision_v1.hpp"

#include <cmath>
#include <exception>

namespace dh2::level_world::quest_move_in_zone_collision_v1 {
namespace {

void set_error(std::string& error, const char* text) noexcept {
    try {
        error = text;
    } catch (...) {
        error.clear();
    }
}

bool is_live_object(objects::Owner& manager,
                    objects::GameObject* object) noexcept {
    return object && object->identity &&
        manager.find_by_identity(object->identity) == object;
}

}  // namespace

Status bind(objects::Owner& manager, objects::SourceHandle source_handle,
            std::int32_t source_network_id, BoundZone& output,
            std::string& error) noexcept {
    output = {};
    error.clear();
    if (source_handle < 0) {
        set_error(error, "QuestMoveInZone source identity is incomplete");
        return Status::invalid_argument;
    }
    auto* object = manager.find_by_source_handle(source_handle);
    if (!object || !object->identity) {
        set_error(error, "QuestMoveInZone was not committed to the active ObjectManager");
        return Status::stale_object;
    }
    output = {&manager, object, source_network_id};
    return Status::complete;
}

Status build_physical_sensor(const float world_position[3],
                             const float authored_scale[3],
                             const float dimensions[3],
                             PhysicalSensor& output,
                             std::string& error) noexcept {
    output = {};
    error.clear();
    if (!world_position || !authored_scale || !dimensions) {
        set_error(error, "QuestMoveInZone geometry inputs are incomplete");
        return Status::invalid_argument;
    }
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (!std::isfinite(world_position[axis]) ||
            !std::isfinite(authored_scale[axis]) ||
            !std::isfinite(dimensions[axis]) ||
            authored_scale[axis] <= 0.0f || dimensions[axis] <= 0.0f) {
            set_error(error, "QuestMoveInZone geometry contains an invalid source value");
            return Status::invalid_argument;
        }
    }
    constexpr float source_to_physics = 0.01f;
    output.position[0] = world_position[0] * source_to_physics;
    output.position[1] = world_position[1] * source_to_physics;
    output.half_extents[0] =
        dimensions[0] * authored_scale[0] * source_to_physics * 0.5f;
    output.half_extents[1] =
        dimensions[1] * authored_scale[1] * source_to_physics * 0.5f;
    if (!std::isfinite(output.position[0]) ||
        !std::isfinite(output.position[1]) ||
        !std::isfinite(output.half_extents[0]) ||
        !std::isfinite(output.half_extents[1]) ||
        output.half_extents[0] <= 0.0f || output.half_extents[1] <= 0.0f) {
        output = {};
        set_error(error, "QuestMoveInZone physical sensor bounds overflowed");
        return Status::invalid_argument;
    }
    return Status::complete;
}

Status build_physical_sensor_from_aabb(const float world_position[3],
                                       const float local_aabb_xyxy[4],
                                       PhysicalSensor& output,
                                       std::string& error) noexcept {
    output = {};
    error.clear();
    if (!world_position || !local_aabb_xyxy) {
        set_error(error, "QuestMoveInZone canonical AABB inputs are incomplete");
        return Status::invalid_argument;
    }
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (!std::isfinite(world_position[axis])) {
            set_error(error, "QuestMoveInZone world position is invalid");
            return Status::invalid_argument;
        }
    }
    for (unsigned extent = 0; extent < 4; ++extent) {
        if (!std::isfinite(local_aabb_xyxy[extent])) {
            set_error(error, "QuestMoveInZone canonical AABB is invalid");
            return Status::invalid_argument;
        }
    }
    if (local_aabb_xyxy[0] >= local_aabb_xyxy[2] ||
        local_aabb_xyxy[1] >= local_aabb_xyxy[3]) {
        set_error(error, "QuestMoveInZone canonical AABB is unordered");
        return Status::invalid_argument;
    }

    constexpr float source_to_physics = 0.01f;
    output.position[0] = world_position[0] * source_to_physics;
    output.position[1] = world_position[1] * source_to_physics;
    output.half_extents[0] = (local_aabb_xyxy[2] - local_aabb_xyxy[0]) *
        source_to_physics * 0.5f;
    output.half_extents[1] = (local_aabb_xyxy[3] - local_aabb_xyxy[1]) *
        source_to_physics * 0.5f;
    if (!std::isfinite(output.position[0]) ||
        !std::isfinite(output.position[1]) ||
        !std::isfinite(output.half_extents[0]) ||
        !std::isfinite(output.half_extents[1]) ||
        output.half_extents[0] <= 0.0f || output.half_extents[1] <= 0.0f) {
        output = {};
        set_error(error, "QuestMoveInZone canonical sensor bounds overflowed");
        return Status::invalid_argument;
    }
    return Status::complete;
}

Status build_init_post_local_aabb(const float authored_scale[3],
                                  const float dimensions[3],
                                  float local_aabb_xyxy[4],
                                  std::string& error) noexcept {
    error.clear();
    if (!authored_scale || !dimensions || !local_aabb_xyxy) {
        set_error(error, "QuestMoveInZone InitPost AABB inputs are incomplete");
        return Status::invalid_argument;
    }
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (!std::isfinite(authored_scale[axis]) ||
            !std::isfinite(dimensions[axis]) || authored_scale[axis] <= 0.0f ||
            dimensions[axis] <= 0.0f) {
            set_error(error, "QuestMoveInZone InitPost dimensions or scale are invalid");
            return Status::invalid_argument;
        }
    }
    local_aabb_xyxy[0] = -dimensions[0] * authored_scale[0] * 0.5f;
    local_aabb_xyxy[1] = -dimensions[1] * authored_scale[1] * 0.5f;
    local_aabb_xyxy[2] = dimensions[0] * authored_scale[0] * 0.5f;
    local_aabb_xyxy[3] = dimensions[1] * authored_scale[1] * 0.5f;
    for (const float extent : {local_aabb_xyxy[0], local_aabb_xyxy[1],
                               local_aabb_xyxy[2], local_aabb_xyxy[3]}) {
        if (!std::isfinite(extent)) {
            local_aabb_xyxy[0] = local_aabb_xyxy[1] =
                local_aabb_xyxy[2] = local_aabb_xyxy[3] = 0.0f;
            set_error(error, "QuestMoveInZone InitPost AABB overflowed");
            return Status::invalid_argument;
        }
    }
    return Status::complete;
}

Status on_collision_begins(const BoundZone& zone,
                           objects::GameObject* collided,
                           const Services& services, Result& output,
                           std::string& error) noexcept {
    output = {};
    error.clear();

    // OnCollisionBegins asserts its source argument before its first virtual
    // call. A missing callback argument is therefore not the ordinary no-op
    // result used for a filtered collision.
    if (!zone.object_manager || !zone.object ||
        !collided || !services.virtual_24 || !services.current_level ||
        !services.move_in_zone_constant ||
        !services.raise_current_level_event) {
        set_error(error, "QuestMoveInZone collision services or source object are incomplete");
        output.status = Status::invalid_argument;
        return output.status;
    }
    if (!is_live_object(*zone.object_manager, zone.object) ||
        !is_live_object(*zone.object_manager, collided)) {
        set_error(error, "QuestMoveInZone collision references an object outside the active ObjectManager");
        output.status = Status::stale_object;
        return output.status;
    }

    bool passes_virtual_gate = false;
    try {
        if (!services.virtual_24(services.context, *collided,
                                 passes_virtual_gate, error)) {
            if (error.empty())
                set_error(error, "ObjectBase virtual +0x24 callback failed");
            output.status = Status::source_callback_failed;
            return output.status;
        }
    } catch (const std::exception&) {
        set_error(error, "ObjectBase virtual +0x24 callback threw");
        output.status = Status::source_callback_failed;
        return output.status;
    } catch (...) {
        set_error(error, "ObjectBase virtual +0x24 callback threw");
        output.status = Status::source_callback_failed;
        return output.status;
    }
    output.source_gate_passed = passes_virtual_gate;
    if (!passes_virtual_gate) {
        output.status = Status::no_event;
        return output.status;
    }

    ActiveLevelView level{};
    bool present = false;
    try {
        if (!services.current_level(services.context, level, present, error)) {
            if (error.empty())
                set_error(error, "Application::GetCurrentLevel provider failed");
            output.status = Status::source_callback_failed;
            return output.status;
        }
    } catch (const std::exception&) {
        set_error(error, "Application::GetCurrentLevel provider threw");
        output.status = Status::source_callback_failed;
        return output.status;
    } catch (...) {
        set_error(error, "Application::GetCurrentLevel provider threw");
        output.status = Status::source_callback_failed;
        return output.status;
    }
    output.current_level_present = present;
    if (!present) {
        output.status = Status::no_event;
        return output.status;
    }
    // A valid current-Level callback borrows the same ObjectManager and event
    // table already selected by the active Level owner. Never fall back to a
    // process-global or per-zone event store.
    if (level.object_manager != zone.object_manager ||
        !is_live_object(*level.object_manager, zone.object) ||
        !is_live_object(*level.object_manager, collided)) {
        set_error(error, "Current Level no longer owns the QuestMoveInZone collision objects");
        output.status = Status::stale_object;
        return output.status;
    }

    std::int32_t event_type = -1;
    try {
        if (!services.move_in_zone_constant(services.context, event_type, error) ||
            event_type < 0) {
            if (error.empty())
                set_error(error, "Actual v2QuestObjectiveType.MoveInZone constant is unavailable");
            output.status = Status::constant_unavailable;
            return output.status;
        }
    } catch (const std::exception&) {
        set_error(error, "v2QuestObjectiveType.MoveInZone constant provider threw");
        output.status = Status::constant_unavailable;
        return output.status;
    } catch (...) {
        set_error(error, "v2QuestObjectiveType.MoveInZone constant provider threw");
        output.status = Status::constant_unavailable;
        return output.status;
    }

    events::Event event{};
    event.objective_type = event_type;
    event.character = collided->identity;
    event.network_id = zone.network_id;
    event.flag0 = 0;
    event.flag1 = 0;
    event.subject_id = -1;
    event.object_identity = zone.object->identity;
    try {
        if (!services.raise_current_level_event(services.context, event, error)) {
            if (error.empty())
                set_error(error, "Current-Level MoveInZone event delivery failed");
            output.status = Status::event_raise_failed;
            return output.status;
        }
    } catch (const std::exception&) {
        set_error(error, "Current-Level MoveInZone event delivery threw");
        output.status = Status::event_raise_failed;
        return output.status;
    } catch (...) {
        set_error(error, "Current-Level MoveInZone event delivery threw");
        output.status = Status::event_raise_failed;
        return output.status;
    }
    output.event_raised = true;
    output.status = Status::complete;
    error.clear();
    return output.status;
}

const char* status_name(Status status) noexcept {
    switch (status) {
        case Status::complete: return "complete";
        case Status::no_event: return "no event";
        case Status::invalid_argument: return "invalid argument";
        case Status::stale_object: return "stale object";
        case Status::source_callback_failed: return "source callback failed";
        case Status::constant_unavailable: return "constant unavailable";
        case Status::event_raise_failed: return "event delivery failed";
    }
    return "unknown status";
}

}  // namespace dh2::level_world::quest_move_in_zone_collision_v1
