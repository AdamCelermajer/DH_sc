// P17 FIREWALL: scene-owned authored FX (see embedded_scene_fx_v1.hpp).
#include "embedded_scene_fx_v1.hpp"

#include "../../../level-world/character_authored_particle_fx_v4.hpp"
#include "../../../engine-math/math.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

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
                               RuntimeEffectsFactoryV1& factory, std::string& reason, bool include_meshes) {
    reason.clear();
    if (slots_.count(key)) return true;
    if (!include_meshes) {
        dh2::resources::BresView image{};
        if (!bytes || dh2_bres_open(&image,bytes->data(),bytes->size()) != dh2::resources::BresError::ok ||
            !dh2_bres_library_count(&image,dh2::resources::Library::emitter)) {
            reason = "actor model has no embedded emitters";
            return false;
        }
    }
    std::shared_ptr<dh2::fx::CharacterParticleFxResourceV2> resource;
    std::string error;
    if (!factory.create_embedded_fx(bytes, resource, error)) {
        reason = error.empty() ? "no authored FX resource" : error;
        return false;
    }
    slots_[key] = Slot{std::move(resource), uri, false, include_meshes};
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
    // B101/B105 diagnostic (DH_FX_DEBUG=2): per-part draw statistics every 30 calls of this key.
    static const bool debug = std::getenv("DH_FX_DEBUG") && std::string(std::getenv("DH_FX_DEBUG")) == "2";
    if (debug && (found->second.debug_counter++ % 30) == 0) {
        auto stats = [&](const char* kind, const dh2::skinning::VisualDrawPartV6& part, std::uint32_t material) {
            float lo[3]{1e30f, 1e30f, 1e30f}, hi[3]{-1e30f, -1e30f, -1e30f};
            for (const auto& p : part.positions) for (int k = 0; k < 3; ++k) { lo[k] = std::min(lo[k], p[k]); hi[k] = std::max(hi[k], p[k]); }
            std::cout << "FXDBG key=" << key << " ms=" << source_ms << " " << kind << " verts=" << part.positions.size();
            if (part.material_table && material < part.material_table->size()) {
                const auto& m = part.material_table->at(material);
                std::cout << " mat=" << m.id << " tex=" << m.diffuse << " add=" << m.additive << " color=" << m.color[0] << ',' << m.color[1] << ',' << m.color[2] << ',' << m.color[3];
            }
            std::cout << " bounds=" << lo[0] << ',' << lo[1] << ',' << lo[2] << ".." << hi[0] << ',' << hi[1] << ',' << hi[2];
            if (part.positions.size() >= 4 && std::string(kind) == "particles") {
                const auto& a = part.positions[0]; const auto& c = part.positions[2];
                std::cout << " quad0diag=" << std::sqrt((a[0]-c[0])*(a[0]-c[0]) + (a[1]-c[1])*(a[1]-c[1]) + (a[2]-c[2])*(a[2]-c[2]));
                if (part.geometry && !part.geometry->primitives.empty()) {
                    const auto index = part.geometry->primitives[0].attributes[2];
                    if (index >= 0 && std::size_t(index) < part.geometry->attributes.size() && part.geometry->attributes[index].values.size() >= 4) {
                        const auto& at = part.geometry->attributes[index];
                        std::cout << " vcolor0=" << at.values[0] << ',' << at.values[1] << ',' << at.values[2] << ',' << at.values[3];
                    }
                }
            }
            std::cout << '\n';
        };
        std::vector<dh2::fx::CharacterParticleDrawSourceV3> particles;
        std::string ignored;
        if (found->second.resource->draw_sources_v3(particles, ignored)) for (const auto& s : particles) stats("particles", s.part, s.material);
        std::vector<dh2::fx::CharacterFxMeshDrawSourceV4> meshes;
        if (composite->mesh_draw_sources_v4(meshes, ignored)) for (const auto& s : meshes) stats("mesh", s.part, s.material);
    }
    return true;
}

std::vector<EmbeddedFxSourceV1> EmbeddedSceneFxV1::sources() const {
    std::vector<EmbeddedFxSourceV1> result;
    result.reserve(slots_.size());
    for (const auto& entry : slots_)
        if (entry.second.active) result.push_back(EmbeddedFxSourceV1{entry.second.resource, entry.second.uri, entry.second.include_meshes});
    return result;
}

} // namespace dh::foundation::effects
