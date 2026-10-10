#pragma once

#include "runtime_effects_factory_v1.hpp"
#include "../../renderer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::effects {

// Bridges factory-produced immutable FX frames to the existing root Renderer.
// The caller inserts draw_queued() at its established SceneManager ordering
// point, then calls finish_and_drain() only after the GL work has completed.
// This helper does not create a Scene, camera, or source FX owner.
class RuntimeEffectsRendererV1 final {
public:
    explicit RuntimeEffectsRendererV1(Renderer& renderer) noexcept;
    RuntimeEffectsRendererV1(const RuntimeEffectsRendererV1&) = delete;
    RuntimeEffectsRendererV1& operator=(const RuntimeEffectsRendererV1&) = delete;

    // Installs real root-Renderer texture upload/release and exact-frame queue
    // callbacks into the factory's existing bindings. The factory and this
    // helper must share a current WGL context and the helper must outlive it.
    void bind_factory_services(RuntimeEffectsFactoryBindingsV1&);

    // Called by RuntimeEffectsFactoryV1 after source material preparation.
    // Frames are retained in insertion order until finish_and_drain succeeds.
    bool enqueue(std::shared_ptr<const EffectRenderFrame>, std::string& error);
    std::size_t pending_frames() const noexcept;
    std::size_t pending_packets() const noexcept;
    std::size_t texture_uploads() const noexcept;
    std::size_t texture_releases() const noexcept;

    // Caller owns beginFrame/endFrame and global SceneManager ordering. This
    // emits the queued packet ranges in frame and packet order without copying.
    bool draw_queued(std::string& error);

    // Calls glFinish, verifies no GL error, then releases retained frame/packet
    // loans. On failure the queue remains retained for diagnosis or retry.
    bool finish_and_drain(std::string& error);

private:
    bool validate(const EffectRenderFrame&, std::string& error) const;

    Renderer& renderer_;
    std::vector<std::shared_ptr<const EffectRenderFrame>> frames_;
    bool draw_started_{};
    std::size_t texture_uploads_{};
    std::size_t texture_releases_{};
};

} // namespace dh::foundation::effects
