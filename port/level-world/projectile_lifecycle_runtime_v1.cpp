#include "projectile_lifecycle_runtime_v1.hpp"

#include <cstdio>
#include <cstring>

namespace dh2::projectile_lifecycle_runtime_v1 {

std::string factory_name(bool laser_type, std::uint32_t serial) {
    char buffer[32]{};
    const int written = std::snprintf(
        buffer, sizeof(buffer), laser_type ? "LTProjectile_%03u" : "Projectile_%03u",
        static_cast<unsigned>(serial));
    if (written < 0 || static_cast<std::size_t>(written) >= sizeof(buffer)) return {};
    return std::string(buffer, static_cast<std::size_t>(written));
}

namespace {

Result stopped(Status status, Stage stage, objects::GameObject* object = nullptr,
               bool registered = false) noexcept {
    return {status, stage, object, registered};
}

Status apply_hook(Status status) noexcept {
    return status == Status::ok ? Status::ok : Status::hook_failed;
}

} // namespace

Result spawn_fresh(objects::Owner& object_manager,
                   const SpawnRequest& request,
                   const Hooks& hooks) noexcept {
    if (request.projectile_id < 0 || request.projectile_table_size == 0 ||
        static_cast<std::uint32_t>(request.projectile_id) >= request.projectile_table_size) {
        return stopped(Status::invalid_projectile_id, Stage::none);
    }
    if (request.projectile_manager == 0 || request.factory_product.object.identity == 0 ||
        request.factory_product.name.empty()) {
        return stopped(Status::invalid_argument, Stage::none);
    }

    const std::string expected_name = factory_name(request.laser_type,
                                                   request.factory_serial);
    if (expected_name.empty() || request.factory_product.name != expected_name) {
        return stopped(Status::factory_name_mismatch, Stage::none);
    }
    const FactoryType expected_type = request.laser_type
        ? FactoryType::laser_type_projectile : FactoryType::projectile;
    if (request.factory_product.type != expected_type) {
        return stopped(Status::wrong_factory_type, Stage::none);
    }
    if (request.source_record != nullptr &&
        (request.source_record->row_index !=
             static_cast<std::uint32_t>(request.projectile_id) ||
         request.source_record->is_laser != request.laser_type)) {
        return stopped(Status::invalid_projectile_id, Stage::none);
    }
    if (const auto* named = object_manager.find_by_name(request.factory_product.name)) {
        // ObjectManager::GetObjectByName reserves the canonical keyed row
        // before GetNewObject calls the class constructor. Add then fills that
        // same row. Accept only this exact name-only reservation; an existing
        // concrete object remains a duplicate.
        if (named->identity != 0 ||
            named->source_handle != request.factory_product.source_handle ||
            (!named->name.empty() &&
             named->name != request.factory_product.name)) {
            return stopped(Status::duplicate_factory_name, Stage::none);
        }
    }
    objects::GameObject* owner = request.owner_identity == 0
        ? nullptr : object_manager.find_by_identity(request.owner_identity);
    objects::GameObject* target = request.target_identity == 0
        ? nullptr : object_manager.find_by_identity(request.target_identity);
    if ((request.owner_identity != 0 && owner == nullptr) ||
        (request.target_identity != 0 && target == nullptr)) {
        return stopped(Status::object_not_owned, Stage::none);
    }
    const bool missing_info =
        (request.info_kind == SetInfoKind::angle_float && hooks.set_info_float == nullptr) ||
        (request.info_kind == SetInfoKind::flag_bool && hooks.set_info_bool == nullptr);
    if (hooks.set_manager == nullptr || hooks.set_updating == nullptr ||
        hooks.set_active == nullptr || missing_info) {
        return stopped(Status::hook_unavailable, Stage::none);
    }

    objects::GameObject* stored = nullptr;
    const objects::Status add_status = object_manager.add_named_object(
        request.factory_product.source_handle, request.factory_product.name,
        request.factory_product.object, &stored);
    if (add_status != objects::Status::ok || stored == nullptr) {
        return stopped(Status::object_manager_rejected, Stage::object_manager_spawn);
    }

    // Source ProjectileManager::_Create records a newly accepted product in
    // its class pool before Projectile::SetManager associates the manager.
    if (hooks.register_pool_object != nullptr) {
        const Status pool_status = hooks.register_pool_object(
            hooks.pool_context, *stored, request.laser_type,
            request.factory_serial);
        if (pool_status != Status::ok) {
            return stopped(apply_hook(pool_status), Stage::projectile_pool_register,
                            stored, true);
        }
    }

    // `_Create` has completed ObjectManager::Spawn and pool insertion before
    // Projectile::SetManager. The caller supplies that existing manager's
    // identity; this port creates no independent manager or projectile pool.
    Status status = hooks.set_manager(hooks.context, *stored,
                                      request.projectile_manager);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::set_projectile_manager, stored, true);
    }

    // ProjectileManager::Spawn invokes virtual SetUpdating(1), then virtual
    // SetInfo with all original arguments, then sets Projectile+0x85 active.
    status = hooks.set_updating(hooks.context, *stored, true);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::set_updating, stored, true);
    }

    const SetInfoArgs info{request.projectile_id, owner, target,
                           request.collision_check_callback,
                           request.collision_hit_callback, request.user_data,
                           request.source_record};
    status = request.info_kind == SetInfoKind::angle_float
        ? hooks.set_info_float(hooks.context, *stored, info, request.angle_degrees)
        : hooks.set_info_bool(hooks.context, *stored, info, request.flag);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::set_info, stored, true);
    }

    status = hooks.set_active(hooks.context, *stored, true);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::activate, stored, true);
    }
    return stopped(Status::ok, Stage::none, stored, true);
}

Result spawn_reused(objects::Owner& object_manager,
                    const SpawnRequest& request,
                    const Hooks& hooks) noexcept {
    if (request.projectile_id < 0 || request.projectile_table_size == 0 ||
        static_cast<std::uint32_t>(request.projectile_id) >= request.projectile_table_size) {
        return stopped(Status::invalid_projectile_id, Stage::none);
    }
    const FactoryType expected_type = request.laser_type
        ? FactoryType::laser_type_projectile : FactoryType::projectile;
    const std::string expected_name = factory_name(request.laser_type,
                                                   request.factory_serial);
    if (request.projectile_manager == 0 || request.factory_product.object.identity == 0 ||
        expected_name.empty() || request.factory_product.name != expected_name ||
        request.factory_product.type != expected_type) {
        return stopped(Status::invalid_argument, Stage::none);
    }
    if (request.source_record != nullptr &&
        (request.source_record->row_index !=
             static_cast<std::uint32_t>(request.projectile_id) ||
         request.source_record->is_laser != request.laser_type)) {
        return stopped(Status::invalid_projectile_id, Stage::none);
    }

    objects::GameObject* stored = object_manager.find_by_source_handle(
        request.factory_product.source_handle);
    if (stored == nullptr || stored->identity != request.factory_product.object.identity ||
        stored->name != request.factory_product.name) {
        return stopped(Status::object_not_owned, Stage::none);
    }
    objects::GameObject* owner = request.owner_identity == 0
        ? nullptr : object_manager.find_by_identity(request.owner_identity);
    objects::GameObject* target = request.target_identity == 0
        ? nullptr : object_manager.find_by_identity(request.target_identity);
    if ((request.owner_identity != 0 && owner == nullptr) ||
        (request.target_identity != 0 && target == nullptr)) {
        return stopped(Status::object_not_owned, Stage::none, stored, true);
    }
    const bool missing_info =
        (request.info_kind == SetInfoKind::angle_float && hooks.set_info_float == nullptr) ||
        (request.info_kind == SetInfoKind::flag_bool && hooks.set_info_bool == nullptr);
    if (hooks.set_updating == nullptr || hooks.set_active == nullptr || missing_info) {
        return stopped(Status::hook_unavailable, Stage::none, stored, true);
    }

    Status status = hooks.set_updating(hooks.context, *stored, true);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::set_updating, stored, true);
    }
    const SetInfoArgs info{request.projectile_id, owner, target,
                           request.collision_check_callback,
                           request.collision_hit_callback, request.user_data,
                           request.source_record};
    status = request.info_kind == SetInfoKind::angle_float
        ? hooks.set_info_float(hooks.context, *stored, info, request.angle_degrees)
        : hooks.set_info_bool(hooks.context, *stored, info, request.flag);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::set_info, stored, true);
    }
    status = hooks.set_active(hooks.context, *stored, true);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::activate, stored, true);
    }
    return stopped(Status::ok, Stage::none, stored, true);
}

Result despawn(objects::Owner& object_manager,
               objects::SourceHandle source_handle,
               const Hooks& hooks) noexcept {
    objects::GameObject* stored = object_manager.find_by_source_handle(source_handle);
    if (stored == nullptr) return stopped(Status::object_not_owned, Stage::none);
    if (hooks.set_updating == nullptr || hooks.stop == nullptr ||
        hooks.set_active == nullptr) {
        return stopped(Status::hook_unavailable, Stage::none, stored, true);
    }

    Status status = hooks.set_updating(hooks.context, *stored, false);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::disable_updating, stored, true);
    }
    status = hooks.stop(hooks.context, *stored);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::stop, stored, true);
    }
    status = hooks.set_active(hooks.context, *stored, false);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::deactivate, stored, true);
    }
    return stopped(Status::ok, Stage::none, stored, true);
}

Result on_expire(objects::Owner& object_manager,
                 objects::SourceHandle source_handle,
                 bool align_to_floor,
                 std::int32_t impact_type,
                 const ExpireFields& fields,
                 const Hooks& hooks) noexcept {
    objects::GameObject* stored = object_manager.find_by_source_handle(source_handle);
    if (stored == nullptr) return stopped(Status::object_not_owned, Stage::none);
    if (fields.collision_target == nullptr || fields.expired == nullptr ||
        hooks.get_target_position == nullptr || hooks.handle_impact_fx == nullptr ||
        (align_to_floor && hooks.snap_to_floor == nullptr)) {
        return stopped(Status::hook_unavailable, Stage::none, stored, true);
    }

    if (align_to_floor) {
        const Status status = hooks.snap_to_floor(hooks.context, *stored);
        if (status != Status::ok) {
            return stopped(apply_hook(status), Stage::floor_sync, stored, true);
        }
    }

    // Projectile::OnExpire clears the last collision target before publishing
    // its expiry flag. A target-position query and impact-FX callback follow.
    *fields.collision_target = 0;
    *fields.expired = 1;
    Point3f target_position{};
    Status status = hooks.get_target_position(hooks.context, *stored,
                                             &target_position);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::target_position, stored, true);
    }
    status = hooks.handle_impact_fx(hooks.context, *stored, impact_type,
                                    target_position);
    if (status != Status::ok) {
        return stopped(apply_hook(status), Stage::impact_fx, stored, true);
    }
    return stopped(Status::ok, Stage::none, stored, true);
}

UpdateResult after_game_object_update(
    objects::Owner& object_manager,
    objects::SourceHandle source_handle,
    const PostMoveFields& fields,
    std::uint32_t dt_for_vertical_rate,
    std::uint32_t dt_for_lifetime,
    const Point3f& current_target_position) noexcept {
    objects::GameObject* stored = object_manager.find_by_source_handle(source_handle);
    if (stored == nullptr) {
        return {Status::object_not_owned, Stage::update_owner_lookup,
                UpdateStop::continue_update, nullptr};
    }
    if (fields.vertical_velocity == nullptr || fields.vertical_rate == nullptr ||
        fields.remaining_lifetime_ms == nullptr || fields.max_distance == nullptr) {
        return {Status::invalid_argument, Stage::none,
                UpdateStop::continue_update, stored};
    }

    // Keep the target binary's separate Application::GetDt values and
    // single-precision operation order. The selected target uses
    // -fno-fast-math/-ffp-contract=off for this library.
    const float rate_step = *fields.vertical_rate *
                            static_cast<float>(dt_for_vertical_rate);
    *fields.vertical_velocity += rate_step / -1000.0f;

    // ARM SUB is modulo 2^32; copy the resulting bits back to signed storage
    // without invoking C++ signed-overflow undefined behavior.
    std::uint32_t lifetime_bits = 0;
    std::memcpy(&lifetime_bits, fields.remaining_lifetime_ms,
                sizeof(lifetime_bits));
    lifetime_bits -= dt_for_lifetime;
    std::memcpy(fields.remaining_lifetime_ms, &lifetime_bits,
                sizeof(lifetime_bits));
    if (*fields.remaining_lifetime_ms <= 0) {
        return {Status::ok, Stage::update_lifetime,
                UpdateStop::expire_type1, stored};
    }
    if (*fields.vertical_velocity <= 0.0f) {
        return {Status::ok, Stage::update_motion,
                UpdateStop::expire_type1, stored};
    }

    if (*fields.max_distance >= 0.0f) {
        const float dx = current_target_position.x - fields.start_position.x;
        const float dy = current_target_position.y - fields.start_position.y;
        const float dz = current_target_position.z - fields.start_position.z;
        const float distance_xy = dx * dx + dy * dy;
        const float distance_squared = distance_xy + dz * dz;
        if (*fields.max_distance <= distance_squared) {
            return {Status::ok, Stage::update_distance,
                    UpdateStop::expire_type1, stored};
        }
    }
    return {Status::ok, Stage::none, UpdateStop::continue_update, stored};
}

CollisionResult on_collision(objects::Owner& object_manager,
                             objects::SourceHandle source_handle,
                             const CollisionFields& fields,
                             const CollisionRequest& request,
                             const Hooks& hooks) noexcept {
    objects::GameObject* projectile =
        object_manager.find_by_source_handle(source_handle);
    if (projectile == nullptr) {
        return {Status::object_not_owned, Stage::collision_gate, 0, nullptr, nullptr};
    }
    if (fields.collision_blocked == nullptr || fields.expired == nullptr ||
        fields.collision_target == nullptr || fields.collision_point == nullptr) {
        return {Status::invalid_argument, Stage::none, 0, projectile, nullptr};
    }

    // The two source flags are checked before resolving or mutating the hit.
    if (*fields.collision_blocked != 0 || *fields.expired != 0) {
        return {Status::ok, Stage::collision_gate, 0, projectile, nullptr};
    }
    if (request.target_identity == 0 ||
        request.target_identity == fields.owner_identity) {
        return {Status::ok, Stage::collision_gate, 0, projectile, nullptr};
    }
    objects::GameObject* target =
        object_manager.find_by_identity(request.target_identity);
    if (target == nullptr) {
        return {Status::object_not_owned, Stage::collision_target_lookup,
                0, projectile, nullptr};
    }
    objects::GameObject* owner = fields.owner_identity == 0
        ? nullptr : object_manager.find_by_identity(fields.owner_identity);
    if (fields.owner_identity != 0 && owner == nullptr) {
        return {Status::object_not_owned, Stage::collision_target_lookup,
                0, projectile, target};
    }
    if (hooks.is_character == nullptr) {
        return {Status::hook_unavailable, Stage::collision_relation,
                0, projectile, target};
    }

    // Non-character collision targets are accepted only for ownerless shots.
    const bool target_is_character =
        hooks.is_character(hooks.context, *target);
    if (!target_is_character && owner != nullptr) {
        return {Status::ok, Stage::collision_relation, 0, projectile, target};
    }
    if (target_is_character && owner != nullptr) {
        if (fields.owner_target_restriction) {
            if (hooks.owner_target_allowed == nullptr) {
                return {Status::hook_unavailable, Stage::collision_relation,
                        0, projectile, target};
            }
            if (!hooks.owner_target_allowed(hooks.context, *owner, *target)) {
                return {Status::ok, Stage::collision_relation, 0, projectile,
                        target};
            }
        }
        if (!fields.allow_non_enemy_characters &&
            hooks.is_character(hooks.context, *owner)) {
            if (hooks.is_enemy == nullptr) {
                return {Status::hook_unavailable, Stage::collision_relation,
                        0, projectile, target};
            }
            if (!hooks.is_enemy(hooks.context, *owner, *target)) {
                return {Status::ok, Stage::collision_relation, 0, projectile,
                        target};
            }
        }
    }

    // The original stores the hit target/point only after all source filters.
    *fields.collision_target = request.target_identity;
    *fields.collision_point = request.point;

    if (fields.check_callback == 0) {
        *fields.expired = 1;
        return {Status::ok, Stage::collision_pending, 1, projectile, target};
    }
    if (hooks.check_collision == nullptr) {
        return {Status::hook_unavailable, Stage::collision_check_callback,
                0, projectile, target};
    }
    const std::int32_t callback_result = hooks.check_collision(
        hooks.context, *projectile, fields.check_callback, fields.user_data);
    if (callback_result == 1) {
        return {Status::ok, Stage::collision_check_callback, 0, projectile,
                target};
    }
    if (callback_result == 2) {
        if (fields.hit_callback != 0) {
            if (hooks.on_collision_hit == nullptr) {
                return {Status::hook_unavailable, Stage::collision_hit_callback,
                        0, projectile, target};
            }
            const Status hit_status = hooks.on_collision_hit(
                hooks.context, *projectile, fields.hit_callback, fields.user_data);
            if (hit_status != Status::ok) {
                return {apply_hook(hit_status), Stage::collision_hit_callback,
                        0, projectile, target};
            }
        }
        if (hooks.handle_collision_impact_fx == nullptr) {
            return {Status::hook_unavailable, Stage::collision_impact_fx,
                    0, projectile, target};
        }
        const Status fx_status = hooks.handle_collision_impact_fx(
            hooks.context, *projectile, *target, request.point);
        if (fx_status != Status::ok) {
            return {apply_hook(fx_status), Stage::collision_impact_fx,
                    0, projectile, target};
        }
        return {Status::ok, Stage::collision_impact_fx, 0, projectile, target};
    }
    if (callback_result == 3) {
        if (fields.expired == nullptr) {
            return {Status::invalid_argument, Stage::collision_pending,
                    0, projectile, target};
        }
        const ExpireFields expire_fields{fields.collision_target, fields.expired};
        const Result expired = on_expire(object_manager, source_handle,
            fields.align_to_floor, 1, expire_fields, hooks);
        if (expired.status != Status::ok) {
            return {expired.status, expired.stopped_at, 0, projectile, target};
        }
        *fields.expired = 1;
        return {Status::ok, Stage::collision_pending, 1, projectile, target};
    }

    // Other callback results follow the source default: queue type-1 expiry.
    *fields.expired = 1;
    return {Status::ok, Stage::collision_pending, 1, projectile, target};
}

} // namespace dh2::projectile_lifecycle_runtime_v1
