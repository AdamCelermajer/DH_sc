#include "runtime_session_projectile_frame_v1.hpp"

namespace dh::foundation::effects {

bool runtime_session_projectile_frame_v1(
    RuntimeSessionProjectileV1& projectiles,
    RuntimeSessionProjectileRenderV1& renderer,
    std::uint64_t source_frame, std::int32_t app_dt_ms, bool paused,
    RuntimeSessionProjectileFrameOutputV1& output, std::string& error) {
    output = {};
    if (!projectiles.synchronize_session_binding(error)) return false;
    if (!projectiles.update(source_frame, app_dt_ms, paused, error)) return false;
    output.updated = true;

    // Recheck immediately after callbacks/result processing before exposing
    // packets as belonging to a still-current Session.
    if (!projectiles.synchronize_session_binding(error)) return false;
    output.impacts = projectiles.take_impacts();
    output.results = projectiles.take_results();
    output.visuals = projectiles.visuals();

    // The renderer's submit callback owns the prepared frame until its current
    // RenderQueue drains. A lease change here must not publish stale visuals.
    if (!projectiles.synchronize_session_binding(error)) {
        output.visuals.clear();
        return false;
    }
    if (!renderer.submit(output.visuals, error)) return false;
    output.submitted = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::effects
