#pragma once

#include "effects_render_bridge.hpp"
#include "../../combat_session.hpp"
#include "../../original_character.hpp"

#include <memory>
#include <set>

namespace dh::foundation::effects {

struct RuntimeCombatEffectsBindingsV1 {
    // Root renderer resolves the actual authored pass/texture and retains this
    // frame loan until its source render queue has flushed.
    EffectRenderServices render;
};

// Session-scoped effects composition. It borrows the actual source manager and
// CombatSession; it owns no clock, pose, actor, tables, or FX resource pool.
class RuntimeCombatEffectsV1 final {
public:
    RuntimeCombatEffectsV1(CombatSession&, dh2::fx::CharacterMeshFxOwnerV4&,
                           RuntimeCombatEffectsBindingsV1);
    RuntimeCombatEffectsV1(const RuntimeCombatEffectsV1&) = delete;
    RuntimeCombatEffectsV1& operator=(const RuntimeCombatEffectsV1&) = delete;
    ~RuntimeCombatEffectsV1() = default;

    // Call after the source event28 consumer/forwarding, with that batch's
    // original occurrence ordinal. ActorId remains the stable key; conversion
    // to the effects owner's uintptr identity is checked for width.
    DispatchResult event(ActorId, const RetainedAnimationEvent&,
                         std::uint64_t occurrence_ordinal, std::string& error);

    // One root frame token may advance FX once. scene-frame sampling precedes
    // VisualFXManager update, and both receive exactly the same source App dt.
    bool update(std::uint64_t source_frame, std::int32_t source_absolute_ms,
                std::int32_t source_app_dt, std::string& error);

    // Frame loans borrow source Scene/material pointers as well as retaining
    // packet backing. Drop them only after the root render queue has flushed.
    bool prepare_render_frame(std::shared_ptr<const EffectRenderFrame>&,
                              std::string& error);

    // Authored model-space socket matrix from the same actor pose/CharacterVisual.
    // Root applies the actual actor placement; this does not create a second pose.
    bool socket_world(ActorId, const std::string& authored_name,
                      const Mat4& local_offset, Mat4& output,
                      std::string& error);

    // Observes CombatSession's host lease. On renewal it releases prior actor
    // anchors and event tokens before admitting work for the new binding.
    bool synchronize_session_binding(std::string& error);
    void release_actor(ActorId) noexcept;

private:
    bool current_actor(ActorId, const RetainedPosePlayback*&,
                       const CharacterVisual*&, const ActorState*&, std::string&) const;
    bool reset_for_renewed_binding(const std::weak_ptr<const void>&, std::string&);
    void prune_frame_loans() const;

    CombatSession& session_;
    dh2::fx::CharacterMeshFxOwnerV4& manager_;
    EffectRenderServices render_services_;
    std::weak_ptr<const void> binding_lease_;
    std::unique_ptr<RetainedEffectsAdapter> events_;
    std::unique_ptr<EffectsRenderBridge> renderer_;
    mutable std::vector<std::weak_ptr<const EffectRenderFrame>> frame_loans_;
    std::set<ActorId> actors_seen_;
    bool update_consumed_{};
    std::uint64_t last_source_frame_{};
};

} // namespace dh::foundation::effects
