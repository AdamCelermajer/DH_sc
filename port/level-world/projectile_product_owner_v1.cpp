#include "projectile_product_owner_v1.hpp"

#include <utility>
#include <stdexcept>

namespace dh2::projectile_product_owner_v1 {

Owner::Owner(objects::Owner& object_manager,
             const data::ProjectileTableV1& table,
             objects::Address manager_identity)
    : object_manager_(object_manager), table_(table),
      manager_(object_manager, manager_identity) {
    if (!manager_identity) throw std::invalid_argument("ProjectileManager identity is null");
}

Payload* Owner::mutable_payload(objects::Address identity) noexcept {
    const auto found = by_identity_.find(identity);
    return found == by_identity_.end() ? nullptr : found->second;
}

const Payload* Owner::find(objects::Address identity) const noexcept {
    const auto found = by_identity_.find(identity);
    return found == by_identity_.end() ? nullptr : found->second;
}

lifecycle::Hooks Owner::hooks() noexcept {
    lifecycle::Hooks result{};
    result.context = this;
    result.set_manager = set_manager;
    result.set_updating = set_updating;
    result.set_info_float = set_info_float;
    result.set_info_bool = set_info_bool;
    result.set_active = set_active;
    return result;
}

lifecycle::Status Owner::set_manager(void* raw, objects::GameObject& object,
                                     objects::Address manager) noexcept {
    auto* self = static_cast<Owner*>(raw);
    auto* payload = self ? self->mutable_payload(object.identity) : nullptr;
    if (!payload || manager != self->manager_.identity())
        return lifecycle::Status::object_not_owned;
    payload->manager_identity = manager;
    return lifecycle::Status::ok;
}

lifecycle::Status Owner::set_updating(void* raw, objects::GameObject& object,
                                      bool enabled) noexcept {
    auto* self = static_cast<Owner*>(raw);
    auto* payload = self ? self->mutable_payload(object.identity) : nullptr;
    if (!payload) return lifecycle::Status::object_not_owned;
    const auto status = self->object_manager_.set_updating(
        object.identity, enabled ? 1u : 0u);
    if (status != objects::Status::ok) return lifecycle::Status::hook_failed;
    if (enabled && !payload->update_staged) {
        bool staged = false;
        if (self->object_manager_.stage_spawn_update_object(
                object.identity, true, &staged) != objects::Status::ok || !staged)
            return lifecycle::Status::hook_failed;
        payload->update_staged = true;
    }
    return lifecycle::Status::ok;
}

lifecycle::Status Owner::set_info_float(void* raw, objects::GameObject& object,
    const lifecycle::SetInfoArgs& args, float angle) noexcept {
    auto* self = static_cast<Owner*>(raw);
    auto* payload = self ? self->mutable_payload(object.identity) : nullptr;
    const auto* row = self ? data::projectile_row_v1(self->table_, args.projectile_id) : nullptr;
    if (!payload || !row || !args.owner || !args.source_record ||
        row->row_index != args.source_record->row_index)
        return lifecycle::Status::object_not_owned;
    payload->projectile_id = args.projectile_id;
    payload->source_row = *args.source_record;
    payload->owner_identity = args.owner->identity;
    payload->target_identity = args.target ? args.target->identity : 0;
    payload->start_position = {args.owner->world_x,args.owner->world_y,
                               payload->position.z};
    payload->user_data = args.user_data;
    payload->check_callback = args.collision_check_callback;
    payload->hit_callback = args.collision_hit_callback;
    payload->info_kind = lifecycle::SetInfoKind::angle_float;
    payload->angle_degrees = angle;
    payload->flag = false;
    payload->vertical_velocity = row->velocity;
    payload->vertical_rate = row->velocity_damping;
    payload->max_distance_squared = row->max_distance >= 0.0f
        ? row->max_distance * row->max_distance : -1.0f;
    payload->remaining_lifetime_ms = row->timer;
    payload->collision_blocked = 0;
    payload->collision_expired = 0;
    payload->collision_target_identity = 0;
    payload->expired = false;
    payload->has_position = false;
    payload->has_destination = false;
    return lifecycle::Status::ok;
}

lifecycle::Status Owner::set_info_bool(void* raw, objects::GameObject& object,
    const lifecycle::SetInfoArgs& args, bool flag) noexcept {
    auto* self = static_cast<Owner*>(raw);
    auto* payload = self ? self->mutable_payload(object.identity) : nullptr;
    const auto* row = self ? data::projectile_row_v1(self->table_, args.projectile_id) : nullptr;
    if (!payload || !row || !args.owner || !args.source_record ||
        row->row_index != args.source_record->row_index)
        return lifecycle::Status::object_not_owned;
    payload->projectile_id = args.projectile_id;
    payload->source_row = *args.source_record;
    payload->owner_identity = args.owner->identity;
    payload->target_identity = args.target ? args.target->identity : 0;
    payload->start_position = {args.owner->world_x,args.owner->world_y,
                               payload->position.z};
    payload->user_data = args.user_data;
    payload->check_callback = args.collision_check_callback;
    payload->hit_callback = args.collision_hit_callback;
    payload->info_kind = lifecycle::SetInfoKind::flag_bool;
    payload->angle_degrees = 0.0f;
    payload->flag = flag;
    payload->vertical_velocity = row->velocity;
    payload->vertical_rate = row->velocity_damping;
    payload->max_distance_squared = row->max_distance >= 0.0f
        ? row->max_distance * row->max_distance : -1.0f;
    payload->remaining_lifetime_ms = row->timer;
    payload->collision_blocked = 0;
    payload->collision_expired = 0;
    payload->collision_target_identity = 0;
    payload->expired = false;
    payload->has_position = false;
    payload->has_destination = false;
    return lifecycle::Status::ok;
}

lifecycle::Status Owner::set_active(void* raw, objects::GameObject& object,
                                    bool enabled) noexcept {
    auto* self = static_cast<Owner*>(raw);
    auto* payload = self ? self->mutable_payload(object.identity) : nullptr;
    if (!payload) return lifecycle::Status::object_not_owned;
    payload->active = enabled;
    return lifecycle::Status::ok;
}

SpawnResult Owner::spawn(const skill_projectile_delivery_v1::Launch& launch) {
    SpawnResult output{};
    // Skill projectiles carry their CharAISkillScript as user data (Launch::
    // skill_script, validated by skill_projectile_delivery_v1::spawn). A basic
    // attack ProjectileManager::Spawn from CharAI::_OnAnimEvent passes a null
    // user-data pointer, so only the owning Character is required here.
    if (!launch.character || launch.projectile_id < 0 ||
        static_cast<std::size_t>(launch.projectile_id) >= table_.rows.size()) {
        output.status = Status::invalid_request;
        return output;
    }
    const auto* row = data::projectile_row_v1(table_, launch.projectile_id);
    if (!row || row->row_index != static_cast<std::uint32_t>(launch.projectile_id)) {
        output.status = Status::table_unavailable;
        return output;
    }

    projectile_factory_owner_v1::Request request{};
    request.projectile_id = launch.projectile_id;
    request.source_serial = 0; // Owner::acquire supplies the manager's serial.
    request.owner_identity = launch.character;
    request.collision_check_callback = launch.collision_check_callback;
    request.collision_hit_callback = launch.collision_hit_callback;
    request.user_data = launch.skill_script;
    request.info_kind = launch.info_kind;
    request.angle_degrees = launch.angle_degrees;
    request.flag = launch.flag;
    projectile_manager_runtime_owner_v1::Acquisition acquisition{};
    if (manager_.acquire(request, table_, &acquisition) !=
        projectile_manager_runtime_owner_v1::Status::ok) {
        output.status = Status::manager_failed;
        return output;
    }

    lifecycle::FactoryProduct product{};
    const lifecycle::FactoryProduct* product_pointer = nullptr;
    if (acquisition.kind == projectile_manager_runtime_owner_v1::AcquisitionKind::fresh_product) {
        try {
            auto payload = std::make_unique<Payload>();
            payload->identity = reinterpret_cast<objects::Address>(payload.get());
            payload->source_handle = acquisition.source_handle;
            payload->name = lifecycle::factory_name(acquisition.laser_type,
                                                    acquisition.serial);
            payload->type = acquisition.laser_type
                ? lifecycle::FactoryType::laser_type_projectile
                : lifecycle::FactoryType::projectile;
            Payload* raw_payload = payload.get();
            payloads_.push_back(std::move(payload));
            if (!by_identity_.emplace(raw_payload->identity, raw_payload).second) {
                output.status = Status::product_unavailable;
                return output;
            }
            objects::GameObject object{};
            object.identity = raw_payload->identity;
            object.source_handle = raw_payload->source_handle;
            object.name = raw_payload->name;
            product = {raw_payload->source_handle, raw_payload->name,
                       raw_payload->type, std::move(object)};
            product_pointer = &product;
        } catch (...) {
            output.status = Status::product_unavailable;
            return output;
        }
    } else {
        auto* stored = object_manager_.find_by_source_handle(acquisition.source_handle);
        if (!stored || !mutable_payload(stored->identity)) {
            output.status = Status::product_unavailable;
            return output;
        }
    }

    const auto result = manager_.spawn(&acquisition, product_pointer, hooks());
    if (result.status != projectile_manager_runtime_owner_v1::Status::ok ||
        result.lifecycle.status != lifecycle::Status::ok ||
        !result.lifecycle.object) {
        output.status = Status::manager_failed;
        output.stopped_at = result.lifecycle.stopped_at;
        if (result.lifecycle.object) output.projectile = result.lifecycle.object->identity;
        return output;
    }
    output.status = Status::ok;
    output.projectile = result.lifecycle.object->identity;
    if (auto* payload = mutable_payload(output.projectile))
        payload->skill_index = launch.skill_index;
    return output;
}

Status Owner::set_target(objects::Address projectile,
                         objects::Address target) noexcept {
    auto* payload = mutable_payload(projectile);
    if (!payload || !object_manager_.find_by_identity(projectile))
        return Status::product_unavailable;
    if (target && !object_manager_.find_by_identity(target))
        return Status::target_unavailable;
    payload->target_identity = target;
    return Status::ok;
}

Status Owner::set_position(objects::Address projectile,
    const skill_projectile_delivery_v1::Point3f& point) noexcept {
    auto* payload = mutable_payload(projectile);
    auto* object = object_manager_.find_by_identity(projectile);
    if (!payload || !object) return Status::product_unavailable;
    payload->position = {point.x, point.y, point.z};
    payload->has_position = true;
    object->world_x = point.x;
    object->world_y = point.y;
    return Status::ok;
}

Status Owner::set_destination(objects::Address projectile,
    const skill_projectile_delivery_v1::Point3f& point) noexcept {
    auto* payload = mutable_payload(projectile);
    if (!payload || !object_manager_.find_by_identity(projectile))
        return Status::product_unavailable;
    payload->destination = {point.x, point.y, point.z};
    payload->has_destination = true;
    return Status::ok;
}

lifecycle::CollisionResult Owner::on_collision(
    objects::Address projectile, objects::Address target,
    const lifecycle::Point2f& point, const lifecycle::Hooks& callbacks) noexcept {
    auto* payload = mutable_payload(projectile);
    if (!payload || !object_manager_.find_by_identity(projectile))
        return {lifecycle::Status::object_not_owned,
                lifecycle::Stage::collision_gate, 0, nullptr, nullptr};
    const lifecycle::CollisionFields fields{
        &payload->collision_blocked, &payload->collision_expired,
        &payload->collision_target_identity, &payload->collision_point,
        payload->owner_identity, payload->check_callback,
        payload->hit_callback, payload->user_data,
        payload->source_row.dedicated,
        payload->source_row.friendly_fire,
        payload->source_row.stay_on_floor};
    const lifecycle::CollisionRequest request{target, point};
    return lifecycle::on_collision(object_manager_, payload->source_handle,
                                   fields, request, callbacks);
}

} // namespace dh2::projectile_product_owner_v1
