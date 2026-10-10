#include "runtime_effects_renderer_v1.hpp"

#include <GL/gl.h>

#include <limits>
#include <utility>

namespace dh::foundation::effects {
namespace {
bool fail(std::string& error, const char* reason) {
    error = reason;
    return false;
}
}

RuntimeEffectsRendererV1::RuntimeEffectsRendererV1(Renderer& renderer) noexcept
    : renderer_(renderer) {}

void RuntimeEffectsRendererV1::bind_factory_services(
    RuntimeEffectsFactoryBindingsV1& bindings) {
    bindings.textures.upload = [this](const TextureImage& image,
                                      std::uint32_t& texture,
                                      std::string& error) {
        texture = 0;
        if (!image.width || !image.height ||
            image.width > static_cast<std::uint32_t>(std::numeric_limits<int>::max()) ||
            image.height > static_cast<std::uint32_t>(std::numeric_limits<int>::max()) ||
            std::size_t(image.width) * image.height >
                std::numeric_limits<std::size_t>::max() / 4 ||
            image.rgba.size() != std::size_t(image.width) * image.height * 4)
            return fail(error, "Invalid decoded original effect texture pixels");
        texture = renderer_.createTexture(static_cast<int>(image.width),
                                          static_cast<int>(image.height),
                                          image.rgba.data());
        if (!texture) return fail(error, "Root Renderer rejected original effect texture");
        ++texture_uploads_;
        error.clear();
        return true;
    };
    bindings.textures.release = [this](std::uint32_t texture) {
        if (texture) {
            renderer_.destroyTexture(texture);
            ++texture_releases_;
        }
    };
    bindings.submit = [this](std::shared_ptr<const EffectRenderFrame> frame,
                             std::string& error) {
        return enqueue(std::move(frame), error);
    };
}

bool RuntimeEffectsRendererV1::validate(const EffectRenderFrame& frame,
                                        std::string& error) const {
    for (const auto& packet : frame.packets) {
        if (!packet.source_retention || packet.mesh.vertices.empty() ||
            packet.mesh.indices.empty() || packet.mesh.ranges.empty() ||
            !packet.source.resource_bytes || packet.source.resource_bytes->empty() ||
            packet.source.resource_uri.empty())
            return fail(error, "FX packet lacks retained source resource/geometry backing");
        for (auto index : packet.mesh.indices) {
            if (index >= packet.mesh.vertices.size())
                return fail(error, "FX packet index is outside retained vertex stream");
        }
        for (const auto& range : packet.mesh.ranges) {
            if (!range.indexCount || range.firstIndex > packet.mesh.indices.size() ||
                range.indexCount > packet.mesh.indices.size() - range.firstIndex ||
                !range.material.sourcePass)
                return fail(error, "FX packet lacks a bounded source material pass");
        }
    }
    error.clear();
    return true;
}

bool RuntimeEffectsRendererV1::enqueue(
    std::shared_ptr<const EffectRenderFrame> frame, std::string& error) {
    if (!frame) return fail(error, "Null FX render frame");
    if (draw_started_)
        return fail(error, "Cannot enqueue FX frame during an undrained render batch");
    if (!validate(*frame, error)) return false;
    if (!frame->packets.empty()) frames_.push_back(std::move(frame));
    error.clear();
    return true;
}

std::size_t RuntimeEffectsRendererV1::pending_frames() const noexcept {
    return frames_.size();
}

std::size_t RuntimeEffectsRendererV1::pending_packets() const noexcept {
    std::size_t count = 0;
    for (const auto& frame : frames_) count += frame->packets.size();
    return count;
}

std::size_t RuntimeEffectsRendererV1::texture_uploads() const noexcept {
    return texture_uploads_;
}

std::size_t RuntimeEffectsRendererV1::texture_releases() const noexcept {
    return texture_releases_;
}

bool RuntimeEffectsRendererV1::draw_queued(std::string& error) {
    if (draw_started_)
        return fail(error, "FX queue was already drawn; drain before another draw");
    if (frames_.empty()) {
        error.clear();
        return true;
    }
    // The root caller owns beginFrame/endFrame and chooses this call's place
    // relative to other SceneManager nodes. Preserve accepted frame/packet order.
    for (const auto& frame : frames_) {
        for (const auto& packet : frame->packets) {
            for (const auto& range : packet.mesh.ranges)
                renderer_.drawRange(packet.mesh, range, packet.world);
        }
    }
    draw_started_ = true;
    error.clear();
    return true;
}

bool RuntimeEffectsRendererV1::finish_and_drain(std::string& error) {
    if (frames_.empty()) {
        draw_started_ = false;
        error.clear();
        return true;
    }
    if (!draw_started_)
        return fail(error, "FX frame loans require draw and GL completion before drain");
    glFinish();
    const auto gl_error = glGetError();
    if (gl_error != GL_NO_ERROR)
        return fail(error, "Root Renderer reported a GL error before FX queue drain");
    frames_.clear();
    draw_started_ = false;
    error.clear();
    return true;
}

} // namespace dh::foundation::effects
