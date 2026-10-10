#pragma once

#include "runtime_session_projectile_render_v1.hpp"
#include "../../playable_actor_bodies.hpp"
#include "../../../level-world/physical_world.hpp"

#include <functional>
#include <map>
#include <memory>

namespace dh::foundation::effects {

struct RuntimeSessionProjectileBodyPlanV1 {
    std::array<float, 3> source_bounds_min{}, source_bounds_max{};
    std::array<float, 2> physics_position{};
    float radius{};
    std::int16_t group_index{};
    std::uint16_t category_bits{32}, mask_bits{0x51f};
};

// Build the exact row20 source PhysicalObject circle from the decoded authored
// BDAE bounds. MP_NoCollisions is the original group override, not a new filter.
bool runtime_session_projectile_body_plan_v1(
    const RuntimeSessionProjectileRenderV1&,
    const RuntimeSessionProjectileVisualV1&, bool source_no_collisions,
    RuntimeSessionProjectileBodyPlanV1&, std::string& error);

struct RuntimeSessionProjectileContactServicesV1 {
    // Resolve only a real PlayableActorBodies physical context. `recognized`
    // false means a non-Session peer (for example a level object) whose source
    // Activate path is outside this character-result feature.
    std::function<bool(void* physical_context, ActorId& actor,
        bool& recognized, std::string& error)> resolve_peer_actor;
    // Source visible80 query for the resolved peer; no alive/targetable proxy.
    std::function<bool(ActorId, bool& visible, std::string& error)> peer_visible;
    // Exact Debug::Get("MP_NoCollisions") at projectile physical creation.
    std::function<bool(bool& disabled, std::string& error)> source_no_collisions;
};

// One actual same-NativeWorld sensor per current Session projectile record.
// PlayableActorBodies owns actor bodies; this feature owns only projectile
// sensors and the identity bridge to RuntimeSessionProjectileV1.
class RuntimeSessionProjectileContactBodiesV1 final {
public:
    RuntimeSessionProjectileContactBodiesV1(CombatSession&,
        RuntimeSessionProjectileV1&, dh2::physical::NativeWorld&,
        RuntimeSessionProjectileContactServicesV1);
    RuntimeSessionProjectileContactBodiesV1(
        const RuntimeSessionProjectileContactBodiesV1&) = delete;
    RuntimeSessionProjectileContactBodiesV1& operator=(
        const RuntimeSessionProjectileContactBodiesV1&) = delete;
    ~RuntimeSessionProjectileContactBodiesV1();

    // Call once after an accepted state-5 launch and before its next World Step.
    bool register_projectile(std::uint64_t projectile_id,
        const RuntimeSessionProjectileRenderV1&, std::uintptr_t& source_identity,
        std::string& error);
    bool unregister_projectile(std::uint64_t projectile_id,
        std::string& error);

    // Call after the one existing PlayableActorBodies::step_world for this
    // source frame. The filter has already queued any actual overlap occurrence;
    // native XY is now published before RuntimeSessionProjectileV1::update.
    bool after_world_step(std::uint64_t source_frame, std::string& error);

    // Call after the projectile runtime update/result drain. This writes the
    // source speed command for the NEXT World Step and unregisters expired or
    // terminal records. Paused frames must skip both hooks with the world step.
    bool after_runtime_update(std::string& error);

    bool resolve_source_identity(std::uintptr_t source_identity,
        std::uint64_t& projectile_id, std::string& error) const;
    bool clear(std::string& error);
    std::size_t size() const noexcept { return entries_.size(); }

private:
    struct Entry;
    CombatSession& session_;
    RuntimeSessionProjectileV1& runtime_;
    dh2::physical::NativeWorld& world_;
    RuntimeSessionProjectileContactServicesV1 services_;
    std::weak_ptr<const CombatSessionLifetime> lifetime_;
    std::weak_ptr<const void> binding_lease_;
    std::map<std::uint64_t, std::unique_ptr<Entry>> entries_;
    std::map<std::uintptr_t, std::uint64_t> source_identities_;
    std::uint64_t last_step_frame_{};
    bool has_step_frame_{};

    bool synchronize(std::string& error);
    bool session_alive() const noexcept;
    bool clear_entries(std::string& error);
    bool remove_entry(std::uint64_t projectile_id, bool detach_runtime,
                      std::string& error);
    static unsigned test(void*, void*, const dh2::physical::Filter*,
                         const dh2::physical::Filter*);
    static void contact(void*, dh2::physical::ContactEvent, void*,
                        const float*, unsigned);
    static void velocity(void*, float*);
    static bool same_lease(const std::weak_ptr<const void>&,
                           const std::weak_ptr<const void>&) noexcept;
};

} // namespace dh::foundation::effects
