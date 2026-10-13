#pragma once

#include "projectile_manager_runtime_owner_v1.hpp"
#include "skill_projectile_delivery_v1.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dh2::projectile_product_owner_v1 {

namespace objects = object_manager_runtime_owner_v1;
namespace data = dh2::data;
namespace lifecycle = projectile_lifecycle_runtime_v1;

struct Payload {
    objects::Address identity{};
    objects::SourceHandle source_handle{};
    std::string name;
    lifecycle::FactoryType type{lifecycle::FactoryType::projectile};
    std::int32_t projectile_id{-1};
    data::ProjectileRowV1 source_row{};
    std::int32_t skill_index{-1};
    objects::Address manager_identity{};
    objects::Address owner_identity{};
    objects::Address target_identity{};
    // Projectile+0x384 target lock and +0x3cc last collision target are
    // distinct source fields and may be live at the same time.
    objects::Address collision_target_identity{};
    objects::Address user_data{};
    lifecycle::ProjectileCallback check_callback{};
    lifecycle::ProjectileCallback hit_callback{};
    lifecycle::SetInfoKind info_kind{lifecycle::SetInfoKind::flag_bool};
    float angle_degrees{};
    bool flag{};
    lifecycle::Point3f position{};
    lifecycle::Point3f destination{};
    lifecycle::Point3f start_position{};
    float vertical_velocity{};
    float vertical_rate{};
    float max_distance_squared{};
    std::int32_t remaining_lifetime_ms{};
    bool has_position{};
    bool has_destination{};
    bool active{};
    bool expired{};
    std::uint8_t collision_blocked{};
    std::uint8_t collision_expired{};
    lifecycle::Point2f collision_point{};
    bool update_staged{};
    // Counts Projectile::SetInfo (0x3e58cc) runs on this pooled Projectile.
    // A pool slot reused by ProjectileManager::Spawn gets a new VisualObject
    // flight (SetVisualObject, AnimController::PlayClip(0), Sync), so a
    // renderer that retains the slot's visual restarts it when this changes.
    std::uint32_t spawn_serial{};
};

enum class Status : std::uint8_t {
    ok, invalid_request, table_unavailable, manager_failed,
    product_unavailable, target_unavailable
};

struct SpawnResult {
    Status status{Status::invalid_request};
    objects::Address projectile{};
    lifecycle::Stage stopped_at{lifecycle::Stage::none};
};

// One projectile-manager projection borrowing the Level's canonical
// ObjectManager. Factory products are retained payloads associated with rows in
// that manager; there is no second ObjectManager or update loop.
class Owner final {
public:
    Owner(objects::Owner&, const data::ProjectileTableV1&,
          objects::Address manager_identity);
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    SpawnResult spawn(const skill_projectile_delivery_v1::Launch&);
    Status set_target(objects::Address projectile, objects::Address target) noexcept;
    Status set_position(objects::Address projectile,
                        const skill_projectile_delivery_v1::Point3f&) noexcept;
    Status set_destination(objects::Address projectile,
                          const skill_projectile_delivery_v1::Point3f&) noexcept;
    lifecycle::CollisionResult on_collision(
        objects::Address projectile, objects::Address target,
        const lifecycle::Point2f&, const lifecycle::Hooks&) noexcept;
    const Payload* find(objects::Address) const noexcept;
    projectile_manager_runtime_owner_v1::Owner& manager() noexcept { return manager_; }
    const data::ProjectileTableV1& table() const noexcept { return table_; }

private:
    static lifecycle::Status set_manager(void*, objects::GameObject&,
                                         objects::Address) noexcept;
    static lifecycle::Status set_updating(void*, objects::GameObject&,
                                          bool) noexcept;
    static lifecycle::Status set_info_float(void*, objects::GameObject&,
                                            const lifecycle::SetInfoArgs&,
                                            float) noexcept;
    static lifecycle::Status set_info_bool(void*, objects::GameObject&,
                                           const lifecycle::SetInfoArgs&,
                                           bool) noexcept;
    static lifecycle::Status set_active(void*, objects::GameObject&,
                                        bool) noexcept;
    Payload* mutable_payload(objects::Address) noexcept;
    lifecycle::Hooks hooks() noexcept;

    objects::Owner& object_manager_;
    const data::ProjectileTableV1& table_;
    projectile_manager_runtime_owner_v1::Owner manager_;
    std::vector<std::unique_ptr<Payload>> payloads_;
    std::unordered_map<objects::Address, Payload*> by_identity_;
};

} // namespace dh2::projectile_product_owner_v1
