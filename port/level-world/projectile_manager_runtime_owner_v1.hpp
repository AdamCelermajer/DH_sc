#pragma once

#include "projectile_factory_owner_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace dh2::projectile_manager_runtime_owner_v1 {

namespace objects = object_manager_runtime_owner_v1;
namespace factory = projectile_factory_owner_v1;
namespace lifecycle = projectile_lifecycle_runtime_v1;
namespace data = dh2::data;

enum class Status : std::uint8_t {
    ok,
    invalid_request,
    factory_failed,
    manager_busy,
    stale_pool_slot,
    pool_registration_failed,
    pool_not_found,
    pool_slot_inactive,
    allocation_failed,
};

enum class AcquisitionKind : std::uint8_t { fresh_product, pooled_product };

struct Acquisition {
    AcquisitionKind kind{AcquisitionKind::fresh_product};
    bool laser_type{};
    std::uint32_t slot_index{};
    std::uint32_t serial{};
    objects::SourceHandle source_handle{};
    factory::Request launch{};
    factory::Reservation factory_reservation{};
    data::ProjectileRowV1 source_record{};
    bool attempted{};
    bool pool_registered{};
    const void* owner_cookie{};
};

struct PoolSnapshot {
    std::size_t slot_count{};
    std::size_t source_capacity{};
    std::uint32_t next_index{};
    std::uint32_t next_serial{};
    std::size_t active_count{};
};

struct SlotSnapshot {
    objects::SourceHandle source_handle{};
    std::uint32_t serial{};
    bool active{};
};

struct SpawnResult {
    Status status{Status::invalid_request};
    lifecycle::Result lifecycle{};
};

// One typed owner for the source ProjectileManager's separate ordinary and
// laser pools. ObjectManager, source product construction and lifecycle hooks
// stay externally owned. manager_identity must be the one runtime's canonical
// manager identity; this projection does not mint another manager singleton.
class Owner final {
public:
    Owner(objects::Owner& object_manager,
          objects::Address manager_identity) noexcept;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    Status acquire(const factory::Request& launch,
                   const data::ProjectileTableV1& table,
                   Acquisition* output) noexcept;
    SpawnResult spawn(Acquisition* acquisition,
                      const lifecycle::FactoryProduct* fresh_product,
                      const lifecycle::Hooks& hooks) noexcept;
    lifecycle::Result despawn(objects::SourceHandle source_handle,
                              const lifecycle::Hooks& hooks) noexcept;

    PoolSnapshot ordinary_pool() const noexcept;
    PoolSnapshot laser_pool() const noexcept;
    bool slot(bool laser_type, std::size_t index,
              SlotSnapshot* output) const noexcept;
    objects::Address identity() const noexcept { return manager_identity_; }

private:
    struct Slot { objects::SourceHandle source_handle{}; std::uint32_t serial{}; bool in_use{}; };
    struct Pool {
        std::vector<Slot> slots;
        // Source vector capacity grows 0 -> 1 -> 2x on accepted insertions.
        // Track it separately from std::vector::capacity(), whose growth is
        // an implementation detail and need not match the ELF's vector.
        std::size_t source_capacity{};
        std::uint32_t next_index{};
        std::uint32_t next_serial{};
    };
    struct PoolCommitContext { Owner* owner{}; Acquisition* acquisition{}; };

    static lifecycle::Status register_pool_object(void*, objects::GameObject&,
                                                  bool, std::uint32_t) noexcept;
    Pool& pool(bool laser_type) noexcept;
    const Pool& pool(bool laser_type) const noexcept;
    bool take_free_slot(Pool&, std::size_t&, std::uint32_t&) noexcept;
    lifecycle::SpawnRequest lifecycle_request(const Acquisition&,
                                              const lifecycle::FactoryProduct&) const noexcept;

    objects::Owner& object_manager_;
    factory::Owner factory_owner_;
    objects::Address manager_identity_{};
    Pool ordinary_{};
    Pool laser_{};
    Acquisition* pending_acquisition_{};
};

} // namespace dh2::projectile_manager_runtime_owner_v1
