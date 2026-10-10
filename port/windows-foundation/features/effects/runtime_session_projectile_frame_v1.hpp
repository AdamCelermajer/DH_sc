#pragma once

#include "runtime_session_projectile_render_v1.hpp"

namespace dh::foundation::effects {

// One caller-owned source frame after its existing world/contact producer has
// run. The bridge owns ordering and consumption only; it creates no contacts,
// hit results, collision geometry, or second projectile lifetime authority.
struct RuntimeSessionProjectileFrameOutputV1 {
    bool updated{};
    bool submitted{};
    std::vector<RuntimeSessionProjectileVisualV1> visuals;
    std::vector<RuntimeSessionProjectileImpactV1> impacts;
    std::vector<DamageEvent> results;
};

// Order: validate current Session binding, update once, revalidate, consume the
// update's impacts/results, snapshot current visuals, revalidate, then submit
// those visuals to the caller's existing renderer/RenderQueue callback.
// If rendering fails, updated packets/results remain in output for the caller;
// callers must not retry the source update to recover them.
bool runtime_session_projectile_frame_v1(
    RuntimeSessionProjectileV1&, RuntimeSessionProjectileRenderV1&,
    std::uint64_t source_frame, std::int32_t app_dt_ms, bool paused,
    RuntimeSessionProjectileFrameOutputV1&, std::string& error);

} // namespace dh::foundation::effects
