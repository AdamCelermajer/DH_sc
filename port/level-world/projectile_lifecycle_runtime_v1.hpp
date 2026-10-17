#pragma once

#include "object_manager_runtime_owner_v1.hpp"
#include "../game-data/projectile_table_v1.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace dh2::projectile_lifecycle_runtime_v1 {

namespace objects = object_manager_runtime_owner_v1;
namespace data = dh2::data;

enum class Status : std::uint8_t {
    ok,
    invalid_argument,
    invalid_projectile_id,
    factory_name_mismatch,
    wrong_factory_type,
    duplicate_factory_name,
    object_manager_rejected,
    hook_unavailable,
    hook_failed,
    object_not_owned,
};

enum class Stage : std::uint8_t {
    none,
    object_manager_spawn,
    projectile_pool_register,
    set_projectile_manager,
    set_updating,
    set_info,
    activate,
    floor_sync,
    clear_collision_target,
    mark_expired,
    target_position,
    impact_fx,
    update_owner_lookup,
    update_motion,
    update_lifetime,
    update_distance,
    disable_updating,
    stop,
    deactivate,
    collision_gate,
    collision_target_lookup,
    collision_relation,
    collision_store_hit,
    collision_check_callback,
    collision_hit_callback,
    collision_impact_fx,
    collision_pending,
};

enum class FactoryType : std::uint8_t { projectile, laser_type_projectile };
enum class SetInfoKind : std::uint8_t { angle_float, flag_bool };

using ProjectileCallback = std::uintptr_t;

struct Point3f {
    float x{};
    float y{};
    float z{};
};

struct Point2f {
    float x{};
    float y{};
};

struct FactoryProduct {
    objects::SourceHandle source_handle{};
    std::string_view name;
    FactoryType type{FactoryType::projectile};
    objects::GameObject object;
};

struct SpawnRequest {
    std::int32_t projectile_id{};
    std::uint32_t projectile_table_size{};
    // ProjectileTable row byte +28, selected by ProjectileManager::_Create.
    bool laser_type{};
    // Serial belongs to the existing factory/manager caller. This adapter
    // formats the same source name but owns no counter or projectile pool.
    std::uint32_t factory_serial{};
    FactoryProduct factory_product;
    // Optional borrowed source row, normally owned by the single-use factory
    // reservation. It is valid through this synchronous Spawn/SetInfo call.
    const data::ProjectileRowV1* source_record{};
    objects::Address projectile_manager{};
    objects::Address owner_identity{};
    objects::Address target_identity{};
    // Source SetInfo callback slots: +0x3b8 check, +0x3bc hit.
    ProjectileCallback collision_check_callback{};
    ProjectileCallback collision_hit_callback{};
    objects::Address user_data{};
    SetInfoKind info_kind{SetInfoKind::angle_float};
    float angle_degrees{};
    bool flag{};
};

struct SetInfoArgs {
    std::int32_t projectile_id{};
    objects::GameObject* owner{};
    objects::GameObject* target{};
    ProjectileCallback collision_check_callback{};
    ProjectileCallback collision_hit_callback{};
    objects::Address user_data{};
    const data::ProjectileRowV1* source_record{};
};

struct Hooks {
    void* context{};
    // Optional canonical ProjectileManager pool owner. Runs after ObjectManager
    // Add accepts the new product and before source SetManager association.
    void* pool_context{};
    Status (*register_pool_object)(void*, objects::GameObject&, bool,
                                   std::uint32_t) noexcept{};
    Status (*set_manager)(void*, objects::GameObject&, objects::Address) noexcept{};
    Status (*set_updating)(void*, objects::GameObject&, bool) noexcept{};
    Status (*set_info_float)(void*, objects::GameObject&, const SetInfoArgs&,
                             float) noexcept{};
    Status (*set_info_bool)(void*, objects::GameObject&, const SetInfoArgs&,
                            bool) noexcept{};
    Status (*stop)(void*, objects::GameObject&) noexcept{};
    Status (*set_active)(void*, objects::GameObject&, bool) noexcept{};
    Status (*snap_to_floor)(void*, objects::GameObject&) noexcept{};
    Status (*get_target_position)(void*, objects::GameObject&, Point3f*) noexcept{};
    Status (*handle_impact_fx)(void*, objects::GameObject&, std::int32_t,
                               const Point3f&) noexcept{};
    bool (*is_character)(void*, objects::GameObject&) noexcept{};
    bool (*is_enemy)(void*, objects::GameObject&, objects::GameObject&) noexcept{};
    bool (*owner_target_allowed)(void*, objects::GameObject&,
                                objects::GameObject&) noexcept{};
    std::int32_t (*check_collision)(void*, objects::GameObject&,
                                   ProjectileCallback,
                                   objects::Address) noexcept{};
    Status (*on_collision_hit)(void*, objects::GameObject&,
                               ProjectileCallback,
                               objects::Address) noexcept{};
    Status (*handle_collision_impact_fx)(void*, objects::GameObject&,
                                         objects::GameObject&,
                                         const Point2f&) noexcept{};
};

struct Result {
    Status status{Status::ok};
    Stage stopped_at{Stage::none};
    objects::GameObject* object{};
    bool registered{};
};

// ELF ProjectileManager::_Create (0x3e693c) uses two independent counters and these exact
// names when the corresponding pool grows. Counter ownership remains with the
// caller; the adapter never tracks another pool or manager.
std::string factory_name(bool laser_type, std::uint32_t serial);

// Implements the fresh-object branch of ProjectileManager::Spawn (0x3e6edc,
// 0x3e701c) over the one canonical ObjectManager::Owner. The factory product
// is the exact source
// handle/name/type identity selected by the existing object factory. Existing
// projectiles from a pool are deliberately passed through that pool's owner;
// this function must not be used to emulate pool reuse.
Result spawn_fresh(objects::Owner& object_manager,
                   const SpawnRequest& request,
                   const Hooks& hooks) noexcept;

// Source pool-reuse path: the object is already registered and associated with
// this manager. It runs Spawn's update/SetInfo/activation sequence without
// ObjectManager Add, pool registration, or a second SetManager call.
Result spawn_reused(objects::Owner& object_manager,
                    const SpawnRequest& request,
                    const Hooks& hooks) noexcept;

// Source ProjectileManager::DeSpawn for the ordinary (non-laser) pool performs
// SetUpdating(false), GameObject::Stop, then clears the in-use byte. It leaves
// the GameObject registered for later pool reuse, so this adapter never removes
// it from ObjectManager. The source pool's free-slot bookkeeping is outside
// this bounded API.
Result despawn(objects::Owner& object_manager,
               objects::SourceHandle source_handle,
               const Hooks& hooks) noexcept;

struct ExpireFields {
    // Borrowed Projectile+0x3cc target pointer and +0x3d1 expiry byte.
    // Their storage remains owned by the factory-created Projectile object.
    objects::Address* collision_target{};
    std::uint8_t* expired{};
};

// Projectile::OnExpire (0x3e5078) call order: optional floor correction when
// ProjectileTable row byte +52 is set, clear Projectile+0x3cc, set +0x3d1,
// fetch target position, then run impact FX. This does not perform Update's
// later DeSpawn or manager-pool free-slot bookkeeping.
Result on_expire(objects::Owner& object_manager,
                 objects::SourceHandle source_handle,
                 bool align_to_floor,
                 std::int32_t impact_type,
                 const ExpireFields& fields,
                 const Hooks& hooks) noexcept;

struct PostMoveFields {
    // Borrowed Projectile fields at +0x3ac, +0x3b0, +0x3b4, and +0x3a8.
    float* vertical_velocity{};
    const float* vertical_rate{};
    std::int32_t* remaining_lifetime_ms{};
    const float* max_distance{};
    Point3f start_position{};
};

enum class UpdateStop : std::uint8_t { continue_update, expire_type1 };

struct UpdateResult {
    Status status{Status::ok};
    Stage stopped_at{Stage::none};
    UpdateStop disposition{UpdateStop::continue_update};
    objects::GameObject* object{};
};

// Implements the source post-GameObject::Update checks from Projectile::Update
// (0x3e51b0): vertical-rate adjustment using the first Application::GetDt,
// signed 32-bit lifetime decrement using its second GetDt, speed/lifetime
// expiry gates, then the inclusive squared maximum-distance gate. The caller
// supplies the actual two dt reads and current target position; world-floor,
// room, collision, and subsequent DeSpawn remain caller-owned.
UpdateResult after_game_object_update(objects::Owner& object_manager,
                                      objects::SourceHandle source_handle,
                                      const PostMoveFields& fields,
                                      std::uint32_t dt_for_vertical_rate,
                                      std::uint32_t dt_for_lifetime,
                                      const Point3f& current_target_position) noexcept;

struct CollisionFields {
    // Borrowed source Projectile fields: +0x3d0, +0x3d1, +0x3cc,
    // +0x3c4/+0x3c8. These remain owned by the native Projectile instance.
    const std::uint8_t* collision_blocked{};
    std::uint8_t* expired{};
    objects::Address* collision_target{};
    Point2f* collision_point{};
    objects::Address owner_identity{};
    ProjectileCallback check_callback{}; // Projectile+0x3b8
    ProjectileCallback hit_callback{};   // Projectile+0x3bc
    objects::Address user_data{};        // Projectile+0x3c0
    bool owner_target_restriction{};      // ProjectileTable row byte +4
    bool allow_non_enemy_characters{};    // ProjectileTable row byte +16
    bool align_to_floor{};               // ProjectileTable row byte +52
};

struct CollisionRequest {
    objects::Address target_identity{};
    Point2f point{};
};

struct CollisionResult {
    Status status{Status::ok};
    Stage stopped_at{Stage::none};
    // This is Projectile::OnCollision's integer return, not a hit/damage
    // amount. A result of one means the caller should continue expiry handling.
    std::int32_t source_return{};
    objects::GameObject* projectile{};
    objects::GameObject* target{};
};

// Source Projectile::OnCollision (0x3e55b4) filter/store/callback dispatch.
// Collision detection itself is still produced by the caller/physics backend.
// Target relation and callback bodies are injected, while every object pointer
// must resolve through the canonical ObjectManager owner.
CollisionResult on_collision(objects::Owner& object_manager,
                             objects::SourceHandle source_handle,
                             const CollisionFields& fields,
                             const CollisionRequest& request,
                             const Hooks& hooks) noexcept;

} // namespace dh2::projectile_lifecycle_runtime_v1
