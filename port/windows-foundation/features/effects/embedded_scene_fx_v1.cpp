// P17 FIREWALL: scene-owned authored FX (see embedded_scene_fx_v1.hpp).
#include "embedded_scene_fx_v1.hpp"

#include "../../../level-world/character_authored_particle_fx_v4.hpp"
#include "../../../engine-math/math.hpp"

#include <algorithm>
#include <cstring>

namespace dh::foundation::effects {

bool embedded_clip_source_ms_v1(const CharacterVisual& visual, std::int32_t& source_ms) {
    const char* name = visual.active_clip_name();
    if (!name || !*name) return false;
    std::int32_t start = 0, end = 0;
    std::string error;
    if (!visual.animation_range(name, start, end, error) || end < start) return false;
    const double span_ms = double(end - start);
    const double local_ms = std::min(visual.animation_elapsed_seconds() * 1000.0, span_ms);
    source_ms = start + std::int32_t(local_ms);
    return true;
}

bool EmbeddedSceneFxV1::attach(std::uint64_t key, const std::string& uri,
                               const std::shared_ptr<const std::vector<std::uint8_t>>& bytes,
                               RuntimeEffectsFactoryV1& factory, std::string& reason) {
    reason.clear();
    if (slots_.count(key)) return true;
    std::shared_ptr<dh2::fx::CharacterParticleFxResourceV2> resource;
    std::string error;
    if (!factory.create_embedded_fx(bytes, resource, error)) {
        reason = error.empty() ? "no authored FX resource" : error;
        return false;
    }
    slots_[key] = Slot{std::move(resource), uri};
    return true;
}

void EmbeddedSceneFxV1::detach(std::uint64_t key) { slots_.erase(key); }

void EmbeddedSceneFxV1::clear() { slots_.clear(); }

void EmbeddedSceneFxV1::begin_frame() {
    for (auto& entry : slots_) entry.second.active = false;
}

bool EmbeddedSceneFxV1::update(std::uint64_t key, std::int32_t source_ms, std::int32_t absolute_ms,
                               std::int32_t dt_ms, const std::array<float, 16>& outer, std::string& error) {
    error.clear();
    const auto found = slots_.find(key);
    if (found == slots_.end()) return true;
    auto* composite = dynamic_cast<dh2::fx::CharacterAuthoredCompositeFxResourceV4*>(found->second.resource.get());
    if (!composite) {
        error = "Required composite authored FX resource for a scene model";
        return false;
    }
    if (!composite->sample_animation(source_ms, error)) return false;
    dh2::math::Matrix4f placement{};
    std::memcpy(placement.m, outer.data(), sizeof(placement.m));
    if (!composite->source_scene_frame_v4(absolute_ms, dt_ms, placement, error)) return false;
    found->second.active = true;
    return true;
}

std::vector<EmbeddedFxSourceV1> EmbeddedSceneFxV1::sources() const {
    std::vector<EmbeddedFxSourceV1> result;
    result.reserve(slots_.size());
    for (const auto& entry : slots_)
        if (entry.second.active) result.push_back(EmbeddedFxSourceV1{entry.second.resource, entry.second.uri});
    return result;
}

} // namespace dh::foundation::effects
