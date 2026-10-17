#include "../particle_runtime.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <vector>

namespace {
struct Sequence {
    static constexpr float values[] = {
        0.75f, 0.25f, 0.625f, 0.375f, 0.875f, 0.125f, 0.5f,
        0.625f, 0.375f, 0.75f, 0.25f, 0.5f, 0.875f, 0.125f
    };
    std::size_t next = 0;
};

bool next_unit(void* user, float* value) {
    auto& sequence = *static_cast<Sequence*>(user);
    *value = Sequence::values[sequence.next++ % (sizeof(Sequence::values)/sizeof(float))];
    return true;
}

bool next_half(void*, float* value) {
    *value = 0.5f;
    return true;
}
}

int main(int argc, char** argv) {
    assert(argc == 2);
    std::ifstream input(argv[1], std::ios::binary);
    assert(input.good());
    std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(input)),
                                    std::istreambuf_iterator<char>());
    dh2::resources::BresView view{};
    assert(dh2_bres_open(&view, bytes.data(), bytes.size())
           == dh2::resources::BresError::ok);
    dh2::resources::Emitter emitter{};
    assert(dh2_bres_emitter(&view, 0, &emitter)
           == dh2::resources::EmitterError::ok);
    assert(emitter.emitter_type == 0 && emitter.max_particles == 100);
    assert(emitter.geometry_mode == 0 && emitter.shape_source_tag == 3);

    dh2::particles::ParticleSystem system;
    Sequence sequence;
    const dh2::particles::RandomSource random{next_unit, &sequence};
    const dh2::particles::Transform identity{};
    const dh2::particles::Color color{0.8f, 0.6f, 0.4f, 0.75f};
    const auto first = system.advance(emitter, 0.125f, identity, color, random);
    const auto second = system.advance(emitter, 0.125f, identity, color, random);
    assert(first.error == dh2::particles::StepError::ok && first.emitted == 7);
    assert(second.error == dh2::particles::StepError::ok && second.emitted == 8);
    assert(second.alive == 15 && system.particles().size() == 15);
    for (const auto& p : system.particles()) {
        assert(std::isfinite(p.position.x) && std::isfinite(p.position.y)
               && std::isfinite(p.position.z));
        assert(std::isfinite(p.velocity.x) && std::isfinite(p.velocity.y)
               && std::isfinite(p.velocity.z));
        assert(p.life_seconds > 0 && p.age_seconds > 0);
        assert(std::isfinite(p.size) && p.size >= 0);
        assert(p.color.r == color.r && p.color.a == color.a);
    }

    // No packaged emitter rows use type 2 in the current cache census. Reuse
    // the decoded record's checked shape backing and exercise native PDCylinder
    // sampling plus life/size/spin evolution with deterministic caller RNG.
    auto cylinder = emitter;
    cylinder.emitter_type = 2;
    cylinder.shape_height = 4.0f;
    cylinder.shape_height_present = true;
    cylinder.shape_width_present = false;
    cylinder.radius_length = 2.0f;
    cylinder.birth_rate = 4.0f;
    cylinder.max_particles = 2;
    cylinder.life = 0.2f;
    cylinder.life_variation = 0.0f;
    cylinder.target_size = 10.0f;
    cylinder.size_variation = 0.0f;
    cylinder.size_growth_time = 0.25f;
    cylinder.size_fade_time = 0.5f;
    cylinder.speed = 0.0f;
    cylinder.speed_variation = 0.0f;
    cylinder.direction[0] = 0.0f;
    cylinder.direction[1] = 1.0f;
    cylinder.direction[2] = 0.0f;
    cylinder.direction_variation = 0.0f;
    cylinder.spin_time = 2.0f;
    cylinder.spin_variation = 0.0f;
    cylinder.spin_phase = 0.0f;
    cylinder.spin_phase_variation = 0.0f;
    cylinder.spin_axis_type = 1;
    cylinder.spin_axis = {0.0f, 1.0f, 0.0f};
    const dh2::particles::RandomSource centered{next_half, nullptr};
    dh2::particles::ParticleSystem cylinder_system;
    const auto cylinder_step = cylinder_system.advance(
        cylinder, 0.5f, identity, color, centered);
    assert(cylinder_step.error == dh2::particles::StepError::ok);
    assert(cylinder_step.emitted == 2 && cylinder_step.alive == 2);
    for (const auto& p : cylinder_system.particles()) {
        assert(std::fabs(p.position.x + 1.0f) < 0.0001f);
        assert(std::fabs(p.position.y) < 0.0001f);
        assert(std::fabs(p.position.z) < 0.0001f);
        assert(std::fabs(p.life_seconds - 0.75f) < 0.0001f);
        assert(std::fabs(p.size - 5.0f) < 0.0001f);
        assert(std::fabs(p.spin_phase_radians - 1.5708f) < 0.001f);
        assert(std::fabs(p.spin_axis.y - 1.0f) < 0.0001f);
    }
}
