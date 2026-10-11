#pragma once

// P17 FIREWALL: authored FX that belongs to a scene model (door, decor, object), not to an actor skill.
//
// A scene BDAE whose BRES carries an authored FX family (particle emitters with birth-rate tracks, FX materials,
// FX meshes) gets one composite FX resource per visual. The resource is sampled at the visual's current authored
// clip time (the SAME BRES timeline the visual's clips use), advanced on the session's FX clock, placed at the
// visual's world transform and drawn through the same effect frame as the actor FX. A model without that family
// (for example a plain wooden door) is not attached; its reason is reported once by the caller.
//
// Clip time: the visual's clock already wraps looping clips and clamps one-shot clips (CharacterVisual::update);
// the resource time is clip start + min(clock, clip span), so an 'open' clip that ends keeps the emitter in its
// authored end state.

#include "effects_render_bridge.hpp"
#include "runtime_effects_factory_v1.hpp"
#include "../../original_character.hpp"

#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::effects {

// Authored BRES timeline ms of a visual's current clip. False when the visual has no named clip or range.
bool embedded_clip_source_ms_v1(const CharacterVisual& visual, std::int32_t& source_ms);

class EmbeddedSceneFxV1 {
public:
    // Attaches the model's authored FX resource to one visual (key = stable declaration id). Returns true when
    // attached; false with reason set when the model has no authored FX family or the resource is refused.
    bool attach(std::uint64_t key, const std::string& uri,
                const std::shared_ptr<const std::vector<std::uint8_t>>& bytes,
                RuntimeEffectsFactoryV1& factory, std::string& reason, bool include_meshes = true);
    void detach(std::uint64_t key);
    void clear();
    bool attached(std::uint64_t key) const { return slots_.count(key) != 0; }
    // True when the resource was stepped this frame: its FX frame draws the visual's meshes (no second draw).
    bool active(std::uint64_t key) const {
        const auto found = slots_.find(key);
        return found != slots_.end() && found->second.active;
    }
    std::size_t size() const noexcept { return slots_.size(); }

    // Starts a frame: every resource becomes inactive until update() steps it again. An inactive resource
    // (its visual is absent, or it was not stepped) is not drawn this frame.
    void begin_frame();
    // Advances one attached resource: authored sample at source_ms, typed outer transform, emission clock.
    bool update(std::uint64_t key, std::int32_t source_ms, std::int32_t absolute_ms, std::int32_t dt_ms,
                const std::array<float, 16>& outer, std::string& error);

    // Every active resource, for RuntimeEffectsFactoryV1::set_embedded_sources (replaced each frame).
    std::vector<EmbeddedFxSourceV1> sources() const;

private:
    struct Slot {
        std::shared_ptr<dh2::fx::CharacterParticleFxResourceV2> resource;
        std::string uri;
        bool active = false;
        bool include_meshes = true;
        std::uint32_t debug_counter = 0; // DH_FX_DEBUG=2 sampling
    };
    std::map<std::uint64_t, Slot> slots_;
};

} // namespace dh::foundation::effects
