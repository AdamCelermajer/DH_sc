#pragma once

#include "object_creation_map_v1.hpp"
#include "projectile_lifecycle_runtime_v1.hpp"
#include "../game-data/projectile_table_v1.hpp"

#include <string>

namespace dh2::projectile_factory_owner_v1 {

namespace objects = object_manager_runtime_owner_v1;
namespace lifecycle = projectile_lifecycle_runtime_v1;
namespace data = dh2::data;

enum class Status : unsigned char {
    ok,
    invalid_request,
    source_factory_mismatch,
    reservation_failed,
    reservation_not_owned,
    already_attempted,
};

struct Request {
    std::int32_t projectile_id{};
    std::uint32_t projectile_table_size{};
    bool laser_type{};
    // Supplied by the existing ProjectileManager::_Create caller. This owner
    // deliberately does not maintain a second serial counter.
    std::uint32_t source_serial{};
    objects::Address projectile_manager{};
    objects::Address owner_identity{};
    objects::Address target_identity{};
    lifecycle::ProjectileCallback collision_check_callback{};
    lifecycle::ProjectileCallback collision_hit_callback{};
    objects::Address user_data{};
    lifecycle::SetInfoKind info_kind{lifecycle::SetInfoKind::angle_float};
    float angle_degrees{};
    bool flag{};
};

// A single-use reservation from the canonical ObjectManager. `source_serial`
// is preserved for provenance; it is never allocated here.
struct Reservation {
    objects::SourceHandle source_handle{};
    std::string name{};
    lifecycle::FactoryType type{lifecycle::FactoryType::projectile};
    Request request{};
    // Present when the manager's type selection came from the decoded source
    // row. Owns the model path for the reservation's entire lifetime.
    data::ProjectileRowV1 source_record{};
    bool source_record_valid{};
    bool attempted{};
};

struct SpawnResult {
    Status status{Status::invalid_request};
    lifecycle::Result lifecycle{};
};

// This is only the typed bridge from source factory metadata into the existing
// lifecycle kernel. The real source Projectile/LaserTypeProjectile constructor
// and its field/vtable initialization must supply `product` before spawn.
class Owner {
public:
    explicit Owner(objects::Owner& object_manager) noexcept
        : object_manager_(object_manager) {}

    Status reserve(const Request& request, Reservation* output) noexcept;
    // Canonical source-backed variant: derives the row count and laser class
    // from the decoded ProjectileTable instead of caller-supplied guesses.
    Status reserve_from_source(const Request& request,
                               const data::ProjectileTableV1& table,
                               Reservation* output) noexcept;
    SpawnResult spawn(Reservation* reservation,
                      const lifecycle::FactoryProduct& product,
                      const lifecycle::Hooks& hooks) noexcept;

private:
    objects::Owner& object_manager_;
};

} // namespace dh2::projectile_factory_owner_v1
