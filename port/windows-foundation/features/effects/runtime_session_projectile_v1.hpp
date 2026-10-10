#pragma once

#include "runtime_source_projectile_v1.hpp"
#include "../../combat_session.hpp"
#include "../../original_combat_properties.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../../level-world/character_combat_queries.hpp"

#include <functional>
#include <map>
#include <optional>
#include <vector>

namespace dh::foundation::effects {

// A source pose loan taken synchronously from this projectile's owner at the
// accepted state-5 marker. `origin` is the live projectile_node when present;
// `forward` is the owner's current look vector. Both use Session world units.
struct RuntimeSessionProjectileLaunchPoseV1 {
    ActorId owner{invalid_actor_id};
    std::array<float, 3> origin{};
    std::array<float, 3> forward{};
};

// Renderer-facing value packet. It owns no actor or graphics object; the
// consumer resolves `model_uri` through the existing renderer/resource path.
struct RuntimeSessionProjectileVisualV1 {
    std::uint64_t projectile_id{};
    ActorId owner{invalid_actor_id};
    std::int32_t source_row{-1}, source_model{-1};
    std::string model_uri;
    std::array<float, 3> position{}, forward{};
    float yaw_radians{};
};

struct RuntimeSessionProjectileImpactV1 {
    std::uint64_t projectile_id{};
    ActorId owner{invalid_actor_id}, target{invalid_actor_id};
    std::int32_t source_row{-1}, kind{};
    std::array<float, 3> position{};
    std::int32_t fx{-1}, sound{-1};
    std::optional<DamageEvent> result;
    // Exact F_MeleeAttack output recovered from the same Session result queue.
    // The original _HandleProjectile call passes false as F_ApplyResult's
    // fourth argument; secondary lifecycle consumers receive it explicitly.
    std::optional<OriginalMeleeResolution> source_calculation;
    bool source_apply_result_flag{};
};

struct RuntimeSessionProjectileContactV1 {
    std::uint64_t projectile_id{};
    ActorId peer{invalid_actor_id};
    std::array<float, 3> point{};
    bool character_peer{true};
};

struct RuntimeSessionProjectileImpactCandidateV1 {
    std::uint64_t projectile_id{}, source_generation{}, source_occurrence{};
    ActorId owner{invalid_actor_id}, target{invalid_actor_id};
    std::int32_t source_row{-1};
    std::array<float, 3> contact_point{};
    std::string source_marker;
    bool source_critical{};
};

// Constructs the exact Character-peer F_MeleeAttack request used by the
// ordinary projectile callback. Equipment is borrowed at impact from the
// caller's current source inventory/item table, not cached at launch.
bool runtime_session_projectile_melee_hit_v1(
    const std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>&,
    const RuntimeSessionProjectileImpactCandidateV1&,
    const dh2::character::CombatInventory16*,
    const dh2::character::CombatItemRecord164*, std::uint32_t item_count,
    CombatSessionSourceHit&, std::string& error);

// This must query the same source AI_IsEnemy semantics used by the admitted
// projectile contact path. `owner_has_ai=false` preserves the original branch
// that skips the enemy query when the projectile owner has no CharAI.
using RuntimeSessionProjectileEnemyQueryV1 = std::function<bool(
    ActorId owner, ActorId peer, bool& owner_has_ai, bool& enemy,
    std::string& error)>;

// Supplies F_MeleeAttack's exact result inputs for a reached projectile hit.
// The runtime overwrites actor/lease/occurrence fields before calling
// CombatSession::apply_source_result; it never derives a hit from launch or
// from geometric proximity.
using RuntimeSessionProjectileHitSemanticsV1 = std::function<bool(
    const RuntimeSessionProjectileImpactCandidateV1&,
    CombatSessionSourceHit&, std::string& error)>;

// _HandleProjectile calls owner CharAI::OnProjectileHit(peer GameObject) before
// F_MeleeAttack. A production caller must bind its current source CharAI/AIS
// owner here; absence is an error on a reached Character hit, never a no-op.
using RuntimeSessionProjectileOwnerHitV1 = std::function<bool(
    const RuntimeSessionProjectileImpactCandidateV1&, std::string& error)>;

struct RuntimeSessionProjectileServicesV1 {
    RuntimeSessionProjectileEnemyQueryV1 source_is_enemy;
    RuntimeSessionProjectileOwnerHitV1 source_owner_on_projectile_hit;
    RuntimeSessionProjectileHitSemanticsV1 source_hit_semantics;
};

struct RuntimeSessionProjectileLaunchV1 {
    ActorId owner{invalid_actor_id};
    std::uint64_t occurrence{};
    RetainedAnimationEvent event;
    dh2::data::CombatEventContext source_event{};
    const dh2::character::CombatProperties896* current_properties{};
    const dh2::character::CombatInventory16* current_inventory{};
    const dh2::character::CombatItemRecord164* item_rows{};
    std::uint32_t item_count{};
    RuntimeSessionProjectileLaunchPoseV1 pose;
};

// Modern same-CombatSession projectile continuation for an admitted original
// state-5 ranged marker. It owns only projectile records/visual values and
// contact occurrences; ActorState, source inventory, pose and Session RNG stay
// with their existing owners.
class RuntimeSessionProjectileV1 final {
public:
    explicit RuntimeSessionProjectileV1(CombatSession&,
                                        RuntimeSessionProjectileServicesV1);
    RuntimeSessionProjectileV1(const RuntimeSessionProjectileV1&) = delete;
    RuntimeSessionProjectileV1& operator=(const RuntimeSessionProjectileV1&) = delete;

    // Returns accepted=false for a replay of the same source occurrence or a
    // non-ranged marker. The source projectile row must equal the live native
    // range query result; no fallback row is selected.
    bool launch_state5(const std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>&,
        const RuntimeSessionProjectileLaunchV1&, bool& accepted,
        std::uint64_t& projectile_id, std::string& error);

    // The physical/contact owner reports actual same-session character contacts
    // here. Source filters are applied before a pending callback is recorded.
    bool contact(const RuntimeSessionProjectileContactV1&, bool& accepted,
                 std::string& error);

    // Call once per source frame with the actual App dt. A paused source frame
    // uses zero elapsed time. Pending native-style hit callbacks run before
    // movement; callback low bits decide whether the projectile continues.
    bool update(std::uint64_t source_frame, std::int32_t app_dt_ms, bool paused,
                std::string& error);

    std::vector<RuntimeSessionProjectileVisualV1> visuals() const;
    std::vector<RuntimeSessionProjectileImpactV1> take_impacts();
    std::vector<DamageEvent> take_results();
    std::size_t active_count() const noexcept;
    bool synchronize_session_binding(std::string& error);
    // The same-Session physical contact owner may attach a real NativeWorld
    // sensor. Once attached, position is published from that body after the
    // World step and this value record must not integrate XY a second time.
    bool attach_native_body(std::uint64_t projectile_id, std::string& error);
    bool detach_native_body(std::uint64_t projectile_id, std::string& error);
    bool publish_native_body_position(std::uint64_t projectile_id,
        const std::array<float, 3>& position, std::string& error);
    bool native_body_velocity(std::uint64_t projectile_id,
        std::array<float, 2>& velocity, std::string& error) const;
    void clear() noexcept;

private:
    struct Record {
        std::uint64_t id{}, source_generation{}, source_occurrence{};
        ActorId owner{invalid_actor_id}, explicit_target{invalid_actor_id};
        std::string marker;
        RuntimeSourceProjectilePlanV1 source;
        std::array<float, 3> origin{}, position{}, forward{};
        // The native body velocity is written after Level::Update's Physical
        // World step; it moves the projectile on the following source frame.
        std::array<float, 3> physics_velocity{};
        float speed{};
        std::int64_t remaining_ms{};
        std::optional<RuntimeSessionProjectileImpactCandidateV1> pending_hit;
        std::optional<RuntimeSessionProjectileContactV1> contact;
        bool live{true}, native_body{};
        bool owner_projectile_hit_delivered{};
    };
    struct LaunchKey {
        ActorId actor{invalid_actor_id};
        std::uint64_t generation{}, occurrence{};
        bool operator<(const LaunchKey& other) const noexcept;
    };
    bool synchronize(std::string& error);
    bool session_alive() const noexcept;
    bool current_actor(ActorId, std::string& error) const;
    bool process_pending(Record&, std::string& error);

    CombatSession& session_;
    RuntimeSessionProjectileServicesV1 services_;
    // Session destruction is separate from detach/restore. Inspect this weak
    // witness before touching session_ so borrowers cannot dereference a dead
    // CombatSession while an old actor binding remains retained.
    std::weak_ptr<const CombatSessionLifetime> session_lifetime_;
    std::weak_ptr<const void> binding_lease_;
    std::map<std::uint64_t, Record> projectiles_;
    std::map<LaunchKey, std::uint64_t> launched_;
    std::vector<RuntimeSessionProjectileImpactV1> impacts_;
    std::vector<DamageEvent> results_;
    std::uint64_t next_projectile_id_{1}, next_hit_occurrence_{1};
    std::uint64_t last_source_frame_{};
    bool has_source_frame_{};
};

} // namespace dh::foundation::effects
