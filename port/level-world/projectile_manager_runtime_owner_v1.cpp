#include "projectile_manager_runtime_owner_v1.hpp"

#include <limits>
#include <utility>

namespace dh2::projectile_manager_runtime_owner_v1 {

Owner::Owner(objects::Owner& object_manager,
             objects::Address manager_identity) noexcept
    : object_manager_(object_manager), factory_owner_(object_manager),
      manager_identity_(manager_identity) {}

Owner::Pool& Owner::pool(bool laser_type) noexcept {
    return laser_type ? laser_ : ordinary_;
}

const Owner::Pool& Owner::pool(bool laser_type) const noexcept {
    return laser_type ? laser_ : ordinary_;
}

bool Owner::take_free_slot(Pool& value, std::size_t& index,
                           std::uint32_t& serial) noexcept {
    const auto count = value.slots.size();
    if (count == 0) return false;
    const std::size_t start = value.next_index < count
        ? value.next_index : 0;
    for (std::size_t candidate = start; candidate < count; ++candidate) {
        auto& slot = value.slots[candidate];
        if (slot.in_use) continue;
        slot.in_use = true;
        index = candidate;
        serial = slot.serial;
        value.next_index = static_cast<std::uint32_t>(candidate + 1);
        return true;
    }
    for (std::size_t candidate = 0; candidate < start; ++candidate) {
        auto& slot = value.slots[candidate];
        if (slot.in_use) continue;
        slot.in_use = true;
        index = candidate;
        serial = slot.serial;
        value.next_index = static_cast<std::uint32_t>(candidate + 1);
        return true;
    }
    return false;
}

Status Owner::acquire(const factory::Request& launch,
                      const data::ProjectileTableV1& table,
                      Acquisition* output) noexcept {
    if (output == nullptr || manager_identity_ == 0 ||
        pending_acquisition_ != nullptr || launch.projectile_id < 0) {
        return pending_acquisition_ != nullptr ? Status::manager_busy
                                                : Status::invalid_request;
    }
    const auto* row = data::projectile_row_v1(table, launch.projectile_id);
    if (row == nullptr || row->row_index !=
            static_cast<std::uint32_t>(launch.projectile_id) ||
        table.rows.size() > std::numeric_limits<std::uint32_t>::max()) {
        return Status::invalid_request;
    }

    Acquisition candidate{};
    try {
        candidate.source_record = *row;
    } catch (...) {
        return Status::allocation_failed;
    }
    candidate.laser_type = row->is_laser;
    candidate.owner_cookie = this;
    candidate.launch = launch;
    candidate.launch.projectile_table_size =
        static_cast<std::uint32_t>(table.rows.size());
    candidate.launch.laser_type = row->is_laser;
    candidate.launch.projectile_manager = manager_identity_;
    auto& selected_pool = pool(row->is_laser);

    std::size_t slot_index = 0;
    std::uint32_t serial = 0;
    if (take_free_slot(selected_pool, slot_index, serial)) {
        const auto& slot = selected_pool.slots[slot_index];
        const auto* stored = object_manager_.find_by_source_handle(
            slot.source_handle);
        const auto expected_name = lifecycle::factory_name(row->is_laser,
                                                            slot.serial);
        const auto expected_type = row->is_laser
            ? lifecycle::FactoryType::laser_type_projectile
            : lifecycle::FactoryType::projectile;
        if (stored == nullptr || stored->identity == 0 ||
            stored->name != expected_name || expected_name.empty()) {
            selected_pool.slots[slot_index].in_use = false;
            selected_pool.next_index = static_cast<std::uint32_t>(slot_index);
            return Status::stale_pool_slot;
        }
        (void)expected_type; // The pool itself is the source class partition.
        candidate.kind = AcquisitionKind::pooled_product;
        candidate.slot_index = static_cast<std::uint32_t>(slot_index);
        candidate.serial = serial;
        candidate.source_handle = slot.source_handle;
        candidate.launch.source_serial = serial;
        *output = std::move(candidate);
        return Status::ok;
    }

    std::size_t next_source_capacity = selected_pool.source_capacity;
    if (selected_pool.slots.size() == selected_pool.source_capacity) {
        if (next_source_capacity == 0) {
            next_source_capacity = 1;
        } else {
            if (next_source_capacity >
                std::numeric_limits<std::size_t>::max() / 2) {
                return Status::allocation_failed;
            }
            next_source_capacity *= 2;
        }
    }
    try {
        selected_pool.slots.reserve(next_source_capacity);
    } catch (...) {
        return Status::allocation_failed;
    }
    candidate.kind = AcquisitionKind::fresh_product;
    candidate.serial = selected_pool.next_serial;
    candidate.launch.source_serial = candidate.serial;
    candidate.launch.projectile_manager = manager_identity_;
    const auto factory_status = factory_owner_.reserve_from_source(
        candidate.launch, table, &candidate.factory_reservation);
    if (factory_status != factory::Status::ok) return Status::factory_failed;
    candidate.source_handle = candidate.factory_reservation.source_handle;
    candidate.serial = candidate.factory_reservation.request.source_serial;
    candidate.launch = candidate.factory_reservation.request;
    candidate.owner_cookie = this;
    *output = std::move(candidate);
    pending_acquisition_ = output;
    return Status::ok;
}

lifecycle::SpawnRequest Owner::lifecycle_request(
    const Acquisition& acquisition,
    const lifecycle::FactoryProduct& product) const noexcept {
    lifecycle::SpawnRequest request{};
    request.projectile_id = acquisition.launch.projectile_id;
    request.projectile_table_size = acquisition.launch.projectile_table_size;
    request.laser_type = acquisition.laser_type;
    request.factory_serial = acquisition.serial;
    request.factory_product = product;
    request.source_record = &acquisition.source_record;
    request.projectile_manager = manager_identity_;
    request.owner_identity = acquisition.launch.owner_identity;
    request.target_identity = acquisition.launch.target_identity;
    request.collision_check_callback =
        acquisition.launch.collision_check_callback;
    request.collision_hit_callback = acquisition.launch.collision_hit_callback;
    request.user_data = acquisition.launch.user_data;
    request.info_kind = acquisition.launch.info_kind;
    request.angle_degrees = acquisition.launch.angle_degrees;
    request.flag = acquisition.launch.flag;
    return request;
}

lifecycle::Status Owner::register_pool_object(
    void* raw, objects::GameObject& object, bool laser_type,
    std::uint32_t serial) noexcept {
    auto& context = *static_cast<PoolCommitContext*>(raw);
    if (context.owner == nullptr || context.acquisition == nullptr) {
        return lifecycle::Status::invalid_argument;
    }
    auto& owner = *context.owner;
    auto& acquisition = *context.acquisition;
    if (owner.pending_acquisition_ != &acquisition ||
        acquisition.owner_cookie != &owner ||
        acquisition.kind != AcquisitionKind::fresh_product ||
        acquisition.laser_type != laser_type || acquisition.serial != serial ||
        acquisition.source_handle != object.source_handle ||
        object.identity == 0 || object.name != lifecycle::factory_name(laser_type, serial)) {
        return lifecycle::Status::object_not_owned;
    }
    auto& selected_pool = owner.pool(laser_type);
    const bool source_capacity_grows =
        selected_pool.slots.size() == selected_pool.source_capacity;
    const auto next_source_capacity = selected_pool.source_capacity == 0
        ? std::size_t{1} : selected_pool.source_capacity * 2;
    if (selected_pool.next_serial != serial ||
        selected_pool.slots.size() >= selected_pool.slots.capacity() ||
        (source_capacity_grows &&
         selected_pool.slots.size() + 1 > next_source_capacity) ||
        (!source_capacity_grows &&
         selected_pool.slots.size() >= selected_pool.source_capacity)) {
        return lifecycle::Status::object_manager_rejected;
    }
    try {
        selected_pool.slots.push_back({object.source_handle, serial, true});
    } catch (...) {
        return lifecycle::Status::object_manager_rejected;
    }
    if (source_capacity_grows) selected_pool.source_capacity = next_source_capacity;
    ++selected_pool.next_serial;
    selected_pool.next_index = 0;
    acquisition.pool_registered = true;
    return lifecycle::Status::ok;
}

SpawnResult Owner::spawn(Acquisition* acquisition,
                         const lifecycle::FactoryProduct* fresh_product,
                         const lifecycle::Hooks& hooks) noexcept {
    if (acquisition == nullptr || acquisition->owner_cookie != this ||
        acquisition->attempted) {
        return {Status::invalid_request,
                {lifecycle::Status::invalid_argument,
                 lifecycle::Stage::none, nullptr, false}};
    }
    if (acquisition->kind == AcquisitionKind::fresh_product) {
        if (pending_acquisition_ != acquisition || fresh_product == nullptr) {
            return {Status::invalid_request,
                    {lifecycle::Status::invalid_argument,
                     lifecycle::Stage::none, nullptr, false}};
        }
        acquisition->attempted = true;
        PoolCommitContext commit{this, acquisition};
        auto bound_hooks = hooks;
        bound_hooks.pool_context = &commit;
        bound_hooks.register_pool_object = register_pool_object;
        const auto spawned = factory_owner_.spawn(
            &acquisition->factory_reservation, *fresh_product, bound_hooks);
        pending_acquisition_ = nullptr;
        if (spawned.status != factory::Status::ok) {
            return {Status::factory_failed, spawned.lifecycle};
        }
        if (!acquisition->pool_registered) {
            return {Status::pool_registration_failed,
                    {lifecycle::Status::hook_failed,
                     lifecycle::Stage::projectile_pool_register,
                     spawned.lifecycle.object, spawned.lifecycle.registered}};
        }
        return {Status::ok, spawned.lifecycle};
    }

    if (fresh_product != nullptr || acquisition->slot_index >=
            pool(acquisition->laser_type).slots.size()) {
        return {Status::invalid_request,
                {lifecycle::Status::invalid_argument,
                 lifecycle::Stage::none, nullptr, false}};
    }
    auto& selected_pool = pool(acquisition->laser_type);
    auto& slot = selected_pool.slots[acquisition->slot_index];
    if (!slot.in_use || slot.source_handle != acquisition->source_handle ||
        slot.serial != acquisition->serial) {
        return {Status::stale_pool_slot,
                {lifecycle::Status::object_not_owned,
                 lifecycle::Stage::none, nullptr, false}};
    }
    auto* stored = object_manager_.find_by_source_handle(slot.source_handle);
    if (stored == nullptr || stored->identity == 0) {
        return {Status::stale_pool_slot,
                {lifecycle::Status::object_not_owned,
                 lifecycle::Stage::none, nullptr, false}};
    }
    const auto type = acquisition->laser_type
        ? lifecycle::FactoryType::laser_type_projectile
        : lifecycle::FactoryType::projectile;
    const lifecycle::FactoryProduct product{
        slot.source_handle, stored->name, type, *stored};
    acquisition->attempted = true;
    return {Status::ok, lifecycle::spawn_reused(
        object_manager_, lifecycle_request(*acquisition, product), hooks)};
}

lifecycle::Result Owner::despawn(objects::SourceHandle source_handle,
                                 const lifecycle::Hooks& hooks) noexcept {
    Pool* owner_pool = nullptr;
    std::size_t owner_index = 0;
    for (Pool* candidate : {&ordinary_, &laser_}) {
        for (std::size_t i = 0; i < candidate->slots.size(); ++i) {
            if (candidate->slots[i].source_handle == source_handle) {
                if (!candidate->slots[i].in_use) {
                    return {lifecycle::Status::object_not_owned,
                            lifecycle::Stage::none, nullptr, true};
                }
                owner_pool = candidate;
                owner_index = i;
                break;
            }
        }
        if (owner_pool != nullptr) break;
    }
    if (owner_pool == nullptr) {
        return {lifecycle::Status::object_not_owned,
                lifecycle::Stage::none, nullptr, false};
    }
    auto result = lifecycle::despawn(object_manager_, source_handle, hooks);
    if (result.status == lifecycle::Status::ok) {
        owner_pool->slots[owner_index].in_use = false;
        owner_pool->next_index = static_cast<std::uint32_t>(owner_index);
    }
    return result;
}

PoolSnapshot Owner::ordinary_pool() const noexcept {
    PoolSnapshot result{ordinary_.slots.size(), ordinary_.source_capacity,
                        ordinary_.next_index, ordinary_.next_serial, 0};
    for (const auto& slot : ordinary_.slots) result.active_count += slot.in_use;
    return result;
}

PoolSnapshot Owner::laser_pool() const noexcept {
    PoolSnapshot result{laser_.slots.size(), laser_.source_capacity,
                        laser_.next_index, laser_.next_serial, 0};
    for (const auto& slot : laser_.slots) result.active_count += slot.in_use;
    return result;
}

bool Owner::slot(bool laser_type, std::size_t index,
                 SlotSnapshot* output) const noexcept {
    if (output == nullptr) return false;
    const auto& selected = pool(laser_type);
    if (index >= selected.slots.size()) return false;
    const auto& source = selected.slots[index];
    *output = {source.source_handle, source.serial, source.in_use};
    return true;
}

} // namespace dh2::projectile_manager_runtime_owner_v1
