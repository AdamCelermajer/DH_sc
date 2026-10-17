#include "destructible_container_source_owner_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh2::destructible_container_source_owner_v1 {
namespace {

bool valid_transform(const world::Transform& transform) noexcept {
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (!std::isfinite(transform.position[axis]) ||
            !std::isfinite(transform.rotation_degrees[axis]) ||
            !std::isfinite(transform.scale[axis]))
            return false;
    }
    return true;
}

void rollback(Manager& manager, std::int32_t handle,
              object_manager_runtime_owner_v1::Address identity) noexcept {
    if (identity) {
        bool removed_room = false;
        (void)manager.remove_room_object(identity, &removed_room);
    }
    if (handle >= 0) {
        bool removed_object = false;
        (void)manager.remove_object(handle, &removed_object);
    }
}

} // namespace

Status interaction_radius(const AbsoluteAabbXY& aabb, float& radius,
                          std::string& error) noexcept {
    radius = 0.0f;
    if (!aabb.ready) {
        error = "DestructibleContainer InitPost AABB is unavailable";
        return Status::aabb_unavailable;
    }
    if (!std::isfinite(aabb.min_x) || !std::isfinite(aabb.min_y) ||
        !std::isfinite(aabb.max_x) || !std::isfinite(aabb.max_y) ||
        aabb.max_x < aabb.min_x || aabb.max_y < aabb.min_y) {
        error = "DestructibleContainer InitPost AABB is invalid";
        return Status::invalid_argument;
    }
    const float width = aabb.max_x - aabb.min_x;
    const float height = aabb.max_y - aabb.min_y;
    const float candidate = std::min(width, height) * 0.5f;
    if (!std::isfinite(candidate) || candidate <= 0.0f) {
        error = "DestructibleContainer interaction AABB has no positive XY extent";
        return Status::invalid_argument;
    }
    radius = candidate;
    error.clear();
    return Status::complete;
}

Status init_final_absolute_bounds(const Record& record, float bounds[4],
                                  std::string& error) noexcept {
    if (!bounds || !record.object || record.source.source_handle < 0) {
        error = "DestructibleContainer InitFinal has no canonical owner or output bounds";
        return Status::invalid_argument;
    }
    const auto& aabb = record.init_post_facts.aabb;
    if (!aabb.ready) {
        error = "DestructibleContainer InitFinal source AABB is unavailable";
        return Status::aabb_unavailable;
    }
    if (!std::isfinite(aabb.min_x) || !std::isfinite(aabb.min_y) ||
        !std::isfinite(aabb.max_x) || !std::isfinite(aabb.max_y) ||
        aabb.max_x < aabb.min_x || aabb.max_y < aabb.min_y) {
        error = "DestructibleContainer InitFinal source AABB is invalid";
        return Status::invalid_argument;
    }
    const float candidate[4]{aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y};
    std::copy(candidate, candidate + 4, bounds);
    error.clear();
    return Status::complete;
}

Status initialize_init_post(Manager& manager,
                            const world::SourceObjectCensusV1& source,
                            const InitPostFacts& facts, Record& record,
                            std::string& error) noexcept {
    error.clear();
    if (record.source.source_handle >= 0 || record.object) {
        error = "DestructibleContainer owner record is already initialized";
        return Status::invalid_argument;
    }
    if (source.gametype != "DestructibleContainer" ||
        source.template_name != "DestructibleContainer" ||
        source.data_desc_pydata != "DestructibleContainers" ||
        source.data_desc.empty() ||
        source.duplicate_name || source.source_handle < 0 || source.name.empty() ||
        source.source_module_id < 0 ||
        !valid_transform(source.world) || facts.deleted_81 > 1 ||
        facts.condition_enabled > 1 || facts.visible_80 > 1 ||
        facts.save_eligibility_28 > 1 || facts.container_state_394 < 0 ||
        facts.container_state_394 > 4) {
        error = "MGP row or InitPost facts do not satisfy DestructibleContainer source contract";
        return Status::invalid_source_contract;
    }
    float radius = 0.0f;
    const bool radius_ready = facts.aabb.ready != 0;
    if (radius_ready) {
        const auto radius_status = interaction_radius(facts.aabb, radius, error);
        if (radius_status != Status::complete) return radius_status;
    }
    error.clear();

    GameObject projection{};
    projection.identity = reinterpret_cast<std::uintptr_t>(&record);
    if (!projection.identity) {
        error = "DestructibleContainer owner identity is null";
        return Status::invalid_argument;
    }
    projection.source_handle = source.source_handle;
    projection.name = source.name;
    projection.module_id_64 = source.source_module_id;
    projection.world_x = source.world.position[0];
    projection.world_y = source.world.position[1];
    // ObjectBase ctor: +0x29=0, +0x8a=1. GameObject ctor: +0x2ee=1.
    // Keep authored ObjectBase visibility separate from InitPost condition
    // state: false MeetCondition hides the visual and selects Container state 4.
    projection.locked_29_ready = 1;
    projection.serialized_flag_8a = 1;
    projection.serialized_flag_8a_ready = 0; // ordered TestEnableCondition receipt
    projection.zoning_enabled_2ee = 1;
    projection.visible_80 = facts.visible_80;
    projection.visible_80_ready = 1;
    projection.deleted_81 = facts.deleted_81;
    projection.save_eligibility_28 = facts.save_eligibility_28;
    projection.save_facts_ready = 1;
    projection.source_module_index = source.module_index;
    projection.source_record = source.source_record;
    projection.source_provenance_ready = 1;
    record.source_module_id = source.source_module_id;
    record.world_x = source.world.position[0];
    record.world_y = source.world.position[1];
    GameObject::LiveFields fields{};
    fields.world_x = &record.world_x;
    fields.world_y = &record.world_y;
    fields.module_id_64 = &record.source_module_id;
    fields.zoning_enabled_2ee = &record.zoning_enabled_2ee;
    fields.visible_80 = &record.visible_80;
    fields.serialized_flag_8a = &record.enabled_8a;
    fields.deleted_81 = &record.deleted_81;
    projection.bind_live_fields(fields);

    GameObject* stored = nullptr;
    const auto add_status = manager.add_named_object(
        source.source_handle, source.name, projection, &stored);
    if (add_status != object_manager_runtime_owner_v1::Status::ok || !stored) {
        error = "DestructibleContainer could not register at its canonical source handle";
        return Status::manager_failed;
    }
    const auto identity = projection.identity;
    bool room_added = false;
    const auto state_status = manager.publish_container_state(
        identity, facts.container_state_394);
    const auto room_status = state_status == object_manager_runtime_owner_v1::Status::ok
        ? manager.add_room_object(identity, &room_added)
        : state_status;
    if (state_status != object_manager_runtime_owner_v1::Status::ok ||
        room_status != object_manager_runtime_owner_v1::Status::ok || !room_added) {
        rollback(manager, source.source_handle, identity);
        error = "DestructibleContainer InitPost failed to publish class state or room-list identity";
        return Status::manager_failed;
    }
    if (radius_ready) {
        stored->interaction_radius_94 = radius;
        stored->interaction_radius_94_ready = 1;
    }
    stored->deleted_81 = facts.deleted_81;
    stored->save_facts_ready = 1;
    stored->in_room_list_2ef = 1;

    record.source = source;
    record.object = stored;
    record.init_post_facts = facts;
    record.deleted_81 = facts.deleted_81;
    record.visible_80 = facts.visible_80;
    record.enabled_8a = 1;
    record.zoning_enabled_2ee = 1;
    error.clear();
    return Status::complete;
}

} // namespace dh2::destructible_container_source_owner_v1
