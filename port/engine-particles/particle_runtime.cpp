#include "particle_runtime.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh2::particles {
namespace {

bool finite(float v) { return std::isfinite(v); }
float dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
Vec3 add(Vec3 a, Vec3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
Vec3 scale(Vec3 a, float k) { return {a.x*k, a.y*k, a.z*k}; }
Vec3 rotate_xy(Vec3 v, float degrees) {
    const float radians = degrees * 0.01745329251994329577f;
    const float c = std::cos(radians), s = std::sin(radians);
    return {v.x*c-v.y*s, v.x*s+v.y*c, v.z};
}
Vec3 rotate_yz(Vec3 v, float degrees) {
    const float radians = degrees * 0.01745329251994329577f;
    const float c = std::cos(radians), s = std::sin(radians);
    return {v.x, v.y*c-v.z*s, v.y*s+v.z*c};
}
Vec3 rotate_xz(Vec3 v, float degrees) {
    const float radians = degrees * 0.01745329251994329577f;
    const float c = std::cos(radians), s = std::sin(radians);
    return {v.x*c+v.z*s, v.y, -v.x*s+v.z*c};
}

class Sampler {
public:
    explicit Sampler(RandomSource source) : source_(source) {}
    bool valid() const { return source_.next_unit != nullptr; }
    bool unit(float& out) const {
        if (!source_.next_unit || !source_.next_unit(source_.user, &out))
            return false;
        return finite(out) && out >= 0.0f && out < 1.0f;
    }
    bool centered(float& out) const {
        if (!unit(out)) return false;
        out -= 0.5f;
        return true;
    }
    bool vector(Vec3& out) const {
        return unit(out.x) && unit(out.y) && unit(out.z);
    }
private:
    RandomSource source_;
};

Vec3 point(const Transform& t, Vec3 v) {
    const auto& m = t.matrix;
    return {m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12],
            m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13],
            m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]};
}

Vec3 direction(const Transform& t, Vec3 v) {
    const auto& m = t.matrix;
    return {m[0]*v.x + m[4]*v.y + m[8]*v.z,
            m[1]*v.x + m[5]*v.y + m[9]*v.z,
            m[2]*v.x + m[6]*v.y + m[10]*v.z};
}

bool normalize(Vec3& v) {
    const float length2 = dot(v, v);
    if (!finite(length2) || length2 <= std::numeric_limits<float>::min())
        return false;
    v = scale(v, 1.0f/std::sqrt(length2));
    return true;
}

bool sample_surface(Sampler& rng, Vec3& out) {
    // Native PDSphere::generate uses rejection inside a half-unit cube,
    // then normalizes the accepted vector onto the sphere surface.
    for (unsigned tries = 0; tries != 128; ++tries) {
        if (!rng.vector(out)) return false;
        out = {out.x-0.5f, out.y-0.5f, out.z-0.5f};
        if (dot(out, out) <= 0.25f) return normalize(out);
    }
    return false;
}

bool spawn_position(const resources::Emitter& e, const Transform& transform,
                    Sampler& rng, Vec3& out) {
    // PEmitterModel::initPEmitterModel @0x64e750 selects PDBox, PDSphere,
    // or PDCylinder; native sampling is @0x69b534, @0x69c6ec, @0x69e1bc.
    if (e.emitter_type == 0) {
        float x, y, z;
        if (!rng.centered(x) || !rng.centered(y) || !rng.centered(z)) return false;
        out = {x*e.shape_width, y*e.shape_height, z*e.radius_length};
    } else if (e.emitter_type == 1) {
        Vec3 unit;
        if (!sample_surface(rng, unit)) return false;
        out = scale(unit, e.radius_length);
    } else if (e.emitter_type == 2) {
        float along, angle, radius;
        if (!rng.unit(along) || !rng.unit(angle) || !rng.unit(radius)) return false;
        angle *= 6.2832f;
        radius *= e.radius_length;
        // Native PDCylinder is centered on local Y, samples height uniformly,
        // then samples radius linearly (not area-uniformly).
        out = {std::cos(angle)*radius,
               (along-0.5f)*e.shape_height,
               std::sin(angle)*radius};
    } else {
        return false;
    }
    out = point(transform, out);
    return finite(out.x) && finite(out.y) && finite(out.z);
}

bool spawn_velocity(const resources::Emitter& e, const Transform& transform,
                    Sampler& rng, Vec3& out) {
    const Vec3 source{e.direction[0], e.direction[1], e.direction[2]};
    const float variation_degrees = std::max(0.0f, e.direction_variation) * 180.0f;
    const bool authored_direction = dot(source, source)
        > std::numeric_limits<float>::min();
    if (!authored_direction) {
        Vec3 random_direction;
        if (!rng.vector(random_direction)) return false;
        random_direction = {random_direction.x-0.5f, random_direction.y-0.5f,
                            random_direction.z-0.5f};
        if (!normalize(random_direction)) return false;
        out = random_direction;
    } else {
        out = source;
        if (variation_degrees != 0) {
            float xy, yz, xz;
            if (!rng.unit(xy) || !rng.unit(yz) || !rng.unit(xz)) return false;
            out = rotate_xy(out, (xy-0.5f)*variation_degrees);
            out = rotate_yz(out, (yz-0.5f)*variation_degrees);
            out = rotate_xz(out, (xz-0.5f)*variation_degrees);
        }
        // PMotion init transforms and normalizes authored directions in the
        // scene basis; its no-direction random branch remains world-oriented.
        out = direction(transform, out);
        if (!normalize(out)) return false;
    }
    float speed_delta = 0;
    if (e.speed*e.speed_variation != 0) {
        if (!rng.centered(speed_delta)) return false;
        speed_delta *= e.speed*e.speed_variation;
    }
    out = scale(out, e.speed+speed_delta);
    return finite(out.x) && finite(out.y) && finite(out.z);
}

bool emit_one(const resources::Emitter& e, const Transform& transform,
              float dt_seconds, Color color, Sampler& rng, Particle& out) {
    // New-range initializers run emitter, size, motion, spin, then life in
    // IParticleSystem<SParticle>::update @0x6532cc.
    if (!spawn_position(e, transform, rng, out.position)) return false;
    // Native size initialization @0x64bd1c samples target size variation;
    // the current size begins at zero only when growth is enabled.
    float size_random;
    if (!rng.unit(size_random)) return false;
    out.target_size = e.target_size
        + (size_random-0.5f)*e.target_size*e.size_variation;
    out.age_seconds = 0;
    out.size = e.size_growth_time > 0 ? 0.0f : out.target_size;

    if (!spawn_velocity(e, transform, rng, out.velocity)) return false;

    // Native spin init @0x64e3c8 samples period, phase, then any axis spread.
    float period_random = 0.0f;
    if (e.spin_time*e.spin_variation != 0
        && !rng.centered(period_random)) return false;
    out.spin_period_seconds = e.spin_time
        + period_random*e.spin_time*e.spin_variation;
    float phase_random = 0.0f;
    if (e.spin_phase*e.spin_phase_variation != 0
        && !rng.centered(phase_random)) return false;
    out.spin_phase_radians = e.spin_phase
        + phase_random*e.spin_phase*e.spin_phase_variation;

    if (e.spin_axis_type == 1) {
        out.spin_axis = {e.spin_axis[0], e.spin_axis[1], e.spin_axis[2]};
        if (e.spin_axis_variation > 0) {
            float xy, yz, xz;
            if (!rng.unit(xy) || !rng.unit(yz) || !rng.unit(xz)) return false;
            const float spread = e.spin_axis_variation*180.0f;
            out.spin_axis = rotate_xy(out.spin_axis, (xy-0.5f)*spread);
            out.spin_axis = rotate_yz(out.spin_axis, (yz-0.5f)*spread);
            out.spin_axis = rotate_xz(out.spin_axis, (xz-0.5f)*spread);
        }
    } else if (e.spin_axis_type == 2) {
        out.spin_axis = out.position;
    } else {
        Vec3 random_axis;
        if (!rng.vector(random_axis)) return false;
        out.spin_axis = {random_axis.x-0.5f, random_axis.y-0.5f,
                         random_axis.z-0.5f};
    }
    if (!normalize(out.spin_axis)) out.spin_axis = {0, 1, 0};

    // Life is initialized after spin; clamp behavior is in @0x64c2fc.
    float life_random;
    if (!rng.unit(life_random)) return false;
    out.life_seconds = e.life + (life_random-0.5f)*e.life_variation;
    if (out.life_seconds < dt_seconds
        && dt_seconds < out.life_seconds*4.0f)
        out.life_seconds = dt_seconds*1.5f;
    out.color = color;
    return true;
}

void update_size(Particle& p, const resources::Emitter& e) {
    float size = p.target_size;
    if (e.size_growth_time > 0 && p.age_seconds < e.size_growth_time)
        size *= std::max(0.0f, p.age_seconds/e.size_growth_time);
    if (e.size_fade_time > 0) {
        const float remaining = p.life_seconds - p.age_seconds;
        if (e.size_fade_time > remaining)
            size = p.target_size
                * std::max(0.0f, remaining/e.size_fade_time);
    }
    p.size = size;
}

bool valid_emitter(const resources::Emitter& e) {
    const float values[] = {e.radius_length, e.shape_width, e.shape_height,
        e.birth_rate, e.life, e.life_variation, e.target_size,
        e.size_variation, e.size_growth_time, e.size_fade_time,
        e.speed, e.speed_variation, e.direction_variation, e.spin_time,
        e.spin_variation, e.spin_phase, e.spin_phase_variation,
        e.spin_axis_variation};
    for (float value : values) if (!finite(value)) return false;
    for (float value : e.direction) if (!finite(value)) return false;
    for (float value : e.spin_axis) if (!finite(value)) return false;
    if (e.max_particles < 0 || e.birth_rate < 0 || e.life < 0
        || e.radius_length < 0 || e.shape_width < 0 || e.shape_height < 0
        || e.life_variation < 0 || e.target_size < 0 || e.size_variation < 0
        || e.size_growth_time < 0 || e.size_fade_time < 0
        || e.speed_variation < 0 || e.direction_variation < 0
        || e.spin_time < 0 || e.spin_variation < 0
        || e.spin_phase_variation < 0 || e.spin_axis_variation < 0)
        return false;
    return true;
}

} // namespace

StepResult ParticleSystem::advance(const resources::Emitter& emitter,
                                   float dt_seconds,
                                   const Transform& emitter_to_world,
                                   Color material_color,
                                   RandomSource random) {
    StepResult result{};
    if (!finite(dt_seconds) || dt_seconds < 0) {
        result.error = StepError::invalid_time;
        result.alive = particles_.size();
        return result;
    }
    if (!valid_emitter(emitter)) {
        result.error = StepError::invalid_emitter;
        result.alive = particles_.size();
        return result;
    }
    if (emitter.emitter_type > 2) {
        result.error = StepError::unsupported_emitter_type;
        result.alive = particles_.size();
        return result;
    }
    if (emitter.geometry_mode != 0) {
        result.error = StepError::unsupported_shape;
        result.alive = particles_.size();
        return result;
    }
    if ((emitter.emitter_type == 0
         && (!emitter.shape_width_present || !emitter.shape_height_present))
        || (emitter.emitter_type == 2 && !emitter.shape_height_present)) {
        result.error = StepError::unsupported_shape;
        result.alive = particles_.size();
        return result;
    }
    if (emitter.spin_axis_type > 2) {
        result.error = StepError::unsupported_orientation;
        result.alive = particles_.size();
        return result;
    }
    // Factory lockAxis settings affect billboard/motion orientation. They are
    // consumed by the renderer bridge; never flatten them into spawn geometry.
    if ((emitter.shape_source_tag == 3 || emitter.shape_source_tag == 4)
        && (emitter.shape_flags != 0 || emitter.shape_subtype == 1
            || emitter.shape_subtype == 2 || emitter.shape_subtype == 3)) {
        result.error = StepError::unsupported_orientation;
        result.alive = particles_.size();
        return result;
    }
    if ((emitter.emitter_type == 0 || emitter.emitter_type == 1
         || emitter.emitter_type == 2)
        && emitter.shape_source_tag != 3 && emitter.shape_source_tag != 4) {
        result.error = StepError::unsupported_shape;
        result.alive = particles_.size();
        return result;
    }
    if ((emitter.emitter_type == 1 || emitter.emitter_type == 2)
        && emitter.radius_data_offset == 0) {
        result.error = StepError::invalid_emitter;
        result.alive = particles_.size();
        return result;
    }
    if (!finite(material_color.r) || !finite(material_color.g)
        || !finite(material_color.b) || !finite(material_color.a)) {
        result.error = StepError::invalid_emitter;
        result.alive = particles_.size();
        return result;
    }

    Sampler rng(random);
    const float rate = emitter.birth_rate;
    // PGenerationModel<SParticle>::generateParticles @0x65317c carries the
    // fractional BirthRate * seconds count between updates, then truncates.
    const float fractional_count = birth_fraction_ + rate*dt_seconds;
    if (!finite(fractional_count)
        || static_cast<double>(fractional_count)
             >= static_cast<double>(std::numeric_limits<std::size_t>::max())) {
        result.error = StepError::invalid_time;
        result.alive = particles_.size();
        return result;
    }
    std::size_t requested = static_cast<std::size_t>(fractional_count);
    float next_fraction = fractional_count-static_cast<float>(requested);
    std::size_t capacity = requested;
    if (emitter.max_particles > 0) {
        const auto maximum = static_cast<std::size_t>(emitter.max_particles);
        capacity = particles_.size() >= maximum ? 0
            : std::min(capacity, maximum-particles_.size());
    }
    if (capacity && !rng.valid()) {
        result.error = StepError::missing_random_source;
        result.alive = particles_.size();
        return result;
    }

    std::vector<Particle> spawned;
    spawned.reserve(capacity);
    for (std::size_t i = 0; i < capacity; ++i) {
        Particle p{};
        if (!emit_one(emitter, emitter_to_world, dt_seconds,
                      material_color, rng, p)) {
            result.error = rng.valid() ? StepError::invalid_random_value
                                       : StepError::missing_random_source;
            result.alive = particles_.size();
            return result;
        }
        spawned.push_back(p);
    }

    // The native generator advances its fractional accumulator even when the
    // source cap prevents all requested particles from being appended.
    birth_fraction_ = next_fraction;
    particles_.insert(particles_.end(), spawned.begin(), spawned.end());
    // Native PCloud onAnimate @0x64dac0 updates the cloud after supplying
    // seconds; the corresponding particle motion/size models are @0x64c190
    // and @0x64be08. Existing and newly emitted records take the same step.
    for (auto& p : particles_) {
        p.age_seconds += dt_seconds;
        p.position = add(p.position, scale(p.velocity, dt_seconds));
        if (p.spin_period_seconds != 0)
            p.spin_phase_radians += (6.2832f/p.spin_period_seconds)*dt_seconds;
        // PColorModel::applyPColor @0x64e830 copies the current material color
        // when no particle animation accessor is bound.
        p.color = material_color;
        update_size(p, emitter);
    }
    particles_.erase(std::remove_if(particles_.begin(), particles_.end(),
        [](const Particle& p) {
            return !(p.age_seconds < p.life_seconds && p.age_seconds >= 0.0f);
        }),
        particles_.end());
    result.emitted = spawned.size();
    result.alive = particles_.size();
    return result;
}

void ParticleSystem::clear() noexcept {
    particles_.clear();
    birth_fraction_ = 0;
}

} // namespace dh2::particles
