#pragma once

#include "../engine-resources/resources.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace dh2::particles {

struct Vec3 {
    float x = 0, y = 0, z = 0;
};

struct Color {
    float r = 1, g = 1, b = 1, a = 1;
};

struct Particle {
    Vec3 position;
    Vec3 velocity;
    float age_seconds = 0;
    float life_seconds = 0;
    float target_size = 0;
    float size = 0;
    float spin_period_seconds = 0;
    float spin_phase_radians = 0;
    Vec3 spin_axis;
    Color color;
};

// Column-major affine matrix, matching the scene material and renderer APIs.
struct Transform {
    std::array<float, 16> matrix{
        1, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 1
    };
};

using RandomUnit = bool (*)(void* user, float* value);
struct RandomSource {
    RandomUnit next_unit = nullptr;
    void* user = nullptr;
};

enum class StepError : std::uint32_t {
    ok,
    invalid_time,
    invalid_emitter,
    unsupported_emitter_type,
    unsupported_shape,
    unsupported_orientation,
    missing_random_source,
    invalid_random_value
};

struct StepResult {
    StepError error = StepError::ok;
    std::size_t emitted = 0;
    std::size_t alive = 0;
};

class ParticleSystem {
public:
    // Caller owns the frame clock and RNG state. `dt_seconds` is the delta
    // passed by CColladaParticleSystemSceneNode::onAnimate to PCloud::update.
    StepResult advance(const resources::Emitter& emitter, float dt_seconds,
                      const Transform& emitter_to_world, Color material_color,
                      RandomSource random);

    const std::vector<Particle>& particles() const noexcept { return particles_; }
    std::vector<Particle>& particles() noexcept { return particles_; }
    void clear() noexcept;

private:
    std::vector<Particle> particles_;
    float birth_fraction_ = 0;
};

} // namespace dh2::particles
