#include "projectile_factory_owner_v1.hpp"

#include <utility>

namespace dh2::projectile_factory_owner_v1 {

Status Owner::reserve(const Request& request, Reservation* output) noexcept {
    if (output == nullptr || request.projectile_id < 0 ||
        request.projectile_table_size == 0 ||
        static_cast<std::uint32_t>(request.projectile_id) >=
            request.projectile_table_size || request.projectile_manager == 0) {
        return Status::invalid_request;
    }

    const char* const source_type = request.laser_type
        ? "LaserTypeProjectile" : "Projectile";
    const auto factory = object_creation_map_v1::resolve(source_type);
    const auto expected = request.laser_type
        ? object_creation_map_v1::Constructor::laser_type_projectile
        : object_creation_map_v1::Constructor::projectile;
    if (factory.status != object_creation_map_v1::Status::resolved ||
        factory.entry == nullptr || factory.entry->constructor != expected) {
        return Status::source_factory_mismatch;
    }

    Reservation candidate{};
    candidate.name = lifecycle::factory_name(request.laser_type,
                                               request.source_serial);
    if (candidate.name.empty()) return Status::invalid_request;
    bool created = false;
    const auto reserve_status = object_manager_.get_or_reserve_named_handle(
        candidate.name, true, &candidate.source_handle, &created);
    if (reserve_status != objects::Status::ok || !created) {
        return Status::reservation_failed;
    }
    candidate.type = request.laser_type
        ? lifecycle::FactoryType::laser_type_projectile
        : lifecycle::FactoryType::projectile;
    candidate.request = request;
    *output = std::move(candidate);
    return Status::ok;
}

Status Owner::reserve_from_source(const Request& request,
                                 const data::ProjectileTableV1& table,
                                 Reservation* output) noexcept {
    if (output == nullptr) return Status::invalid_request;
    const auto* const row = data::projectile_row_v1(table,
                                                    request.projectile_id);
    if (row == nullptr || row->row_index !=
            static_cast<std::uint32_t>(request.projectile_id))
        return Status::invalid_request;

    data::ProjectileRowV1 source_record{};
    try {
        source_record = *row;
    } catch (...) {
        return Status::invalid_request;
    }

    Request canonical = request;
    canonical.projectile_table_size =
        static_cast<std::uint32_t>(table.rows.size());
    canonical.laser_type = row->is_laser;
    const auto status = reserve(canonical, output);
    if (status != Status::ok) return status;
    output->source_record = std::move(source_record);
    output->source_record_valid = true;
    return Status::ok;
}

SpawnResult Owner::spawn(Reservation* reservation,
                         const lifecycle::FactoryProduct& product,
                         const lifecycle::Hooks& hooks) noexcept {
    if (reservation == nullptr) {
        return {Status::invalid_request,
                {lifecycle::Status::invalid_argument,
                 lifecycle::Stage::none, nullptr, false}};
    }
    if (reservation->attempted) {
        return {Status::already_attempted,
                {lifecycle::Status::invalid_argument,
                 lifecycle::Stage::none, nullptr, false}};
    }
    const auto* reserved = object_manager_.find_by_source_handle(
        reservation->source_handle);
    if (reserved == nullptr || reserved->identity != 0 ||
        reserved->name != reservation->name || product.name != reservation->name ||
        product.source_handle != reservation->source_handle ||
        product.type != reservation->type || product.object.identity == 0) {
        return {Status::reservation_not_owned,
                {lifecycle::Status::object_not_owned,
                 lifecycle::Stage::none, nullptr, false}};
    }

    reservation->attempted = true;
    const auto& source = reservation->request;
    lifecycle::SpawnRequest request{};
    request.projectile_id = source.projectile_id;
    request.projectile_table_size = source.projectile_table_size;
    request.laser_type = source.laser_type;
    request.factory_serial = source.source_serial;
    request.factory_product = product;
    request.source_record = reservation->source_record_valid
        ? &reservation->source_record : nullptr;
    request.projectile_manager = source.projectile_manager;
    request.owner_identity = source.owner_identity;
    request.target_identity = source.target_identity;
    request.collision_check_callback = source.collision_check_callback;
    request.collision_hit_callback = source.collision_hit_callback;
    request.user_data = source.user_data;
    request.info_kind = source.info_kind;
    request.angle_degrees = source.angle_degrees;
    request.flag = source.flag;
    return {Status::ok,
            lifecycle::spawn_fresh(object_manager_, request, hooks)};
}

} // namespace dh2::projectile_factory_owner_v1
