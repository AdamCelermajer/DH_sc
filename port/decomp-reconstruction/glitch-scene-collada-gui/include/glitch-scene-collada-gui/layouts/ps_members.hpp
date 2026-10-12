// Member layouts for the glitch::ps PD (particle domain) classes and the
// glitch::scene particle emitter/affector classes (placed in glitch::ps::scene),
// from the Hex-Rays pseudocode in recovered/native/hexrays-all (32-bit ARM offsets).
// Declarations only: no function bodies. Each named member cites the body that
// touches it in its comment. Anything not touched by a cited body is a pad marked
// "not confirmed". The vtable pointer at +0x00 is a 4-byte pad because the
// 32-bit guest layout puts it there (vptr stores in every ctor).
// PSRandom is not declared here: none of the cited bodies reads a PSRandom field
// directly; it is only reached through its Rand/RandVec/NRandf member calls.
// ICursorControl (gui namespace) is not declared here.
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_PS_MEMBERS_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_PS_MEMBERS_HPP

#include <cstddef>
#include <cstdint>

namespace glitch::ps {

// glitch::ps::PDomain. Only the vtable pointer is written by the derived ctors
// (PDBox, PDSphere, PDBlob, PDCone, PDCylinder, PDLine, PDPoint all store their
// vptr at +0x00 and their fields from +0x04). Base size of 4 is inferred, not confirmed.
// The destructor body (0x69ac8c) is empty.
struct PDomain {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer only (base size inferred, not confirmed)
};

// glitch::ps::PDPoint. copy (0x69c45c) allocates 0x10 bytes and copies +0x04..+0x0F.
// transform (0x69ae04) adds the matrix translation (CMatrix4 elements 12..14) to position.
struct PDPoint {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float position[3];                  // +0x04 (x, y, z); transform (0x69ae04) adds the translation
};
static_assert(offsetof(PDPoint, position) == 0x04, "PDPoint::position");
static_assert(sizeof(PDPoint) == 0x10, "PDPoint size from copy (0x69c45c)");

// glitch::ps::PDLine. ctor (0x69e45c/0x69e57c) stores start, axis and the normalized
// axis; the length is the last field. generate (0x69ae54) reads start (+0x04) and axis (+0x10).
// within (0x69e350) reads the normalized direction (+0x1C) and length (+0x28).
struct PDLine {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float start[3];                     // +0x04 start point; ctor (0x69e45c) and generate (0x69ae54)
    float axis[3];                      // +0x10 end - start (raw, not normalized); ctor (0x69e45c) and generate (0x69ae54)
    float dir_normalized[3];            // +0x1C normalized axis; ctor normalizes it, within (0x69e350) reads it
    float length;                       // +0x28 |axis|; ctor (0x69e45c) computes it, within (0x69e350) divides by it
};
static_assert(offsetof(PDLine, start) == 0x04, "PDLine::start");
static_assert(offsetof(PDLine, axis) == 0x10, "PDLine::axis");
static_assert(offsetof(PDLine, dir_normalized) == 0x1C, "PDLine::dir_normalized");
static_assert(offsetof(PDLine, length) == 0x28, "PDLine::length");
static_assert(sizeof(PDLine) == 0x2C, "PDLine size from copy (0x69c3d4)");

// glitch::ps::PDBox. ctor(vec3, vec3) (0x69aecc/0x69b074) takes two corners, orders
// them per axis into min/max, and stores extents. ctor(float,float,float) (0x69b21c/0x69b354)
// centers the box on the origin. generate (0x69b534) uses the three axis vectors
// at +0x38, +0x44, +0x50 with min (+0x04). within (0x69b48c) reads min and max corners.
// transform (0x69efac) recenters min from size (+0x2C) and writes the result to +0x04.
struct PDBox {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float min_corner[3];                // +0x04 (x, y, z); ctor (0x69aecc), within (0x69b48c), generate (0x69b534)
    float max_corner[3];                // +0x10 (x, y, z); ctor (0x69aecc), within (0x69b48c)
    float extent[3];                    // +0x1C (x, y, z) = max - min; both ctors write it, role of the copy at +0x2C is not confirmed
    float extent_sum;                   // +0x28 sum of the three extents; ctor(float,float,float) writes a4 + (a2 + a3)
    float size[3];                      // +0x2C (x, y, z) full size; ctor(float,float,float) (0x69b21c) and transform (0x69efac) read it
    float axis_x[3];                    // +0x38 first generate axis; generate (0x69b534) reads a2[14..16]. Diagonal form (sx, 0, 0)
    float axis_y[3];                    // +0x44 second generate axis; generate (0x69b534) reads a2[17..19]. Diagonal form (0, sy, 0)
    float axis_z[3];                    // +0x50 third generate axis; generate (0x69b534) reads a2[20..22]. Diagonal form (0, 0, sz)
};
static_assert(offsetof(PDBox, min_corner) == 0x04, "PDBox::min_corner");
static_assert(offsetof(PDBox, max_corner) == 0x10, "PDBox::max_corner");
static_assert(offsetof(PDBox, extent) == 0x1C, "PDBox::extent");
static_assert(offsetof(PDBox, extent_sum) == 0x28, "PDBox::extent_sum");
static_assert(offsetof(PDBox, size) == 0x2C, "PDBox::size");
static_assert(offsetof(PDBox, axis_x) == 0x38, "PDBox::axis_x");
static_assert(offsetof(PDBox, axis_y) == 0x44, "PDBox::axis_y");
static_assert(offsetof(PDBox, axis_z) == 0x50, "PDBox::axis_z");
static_assert(sizeof(PDBox) == 0x5C, "PDBox size from copy (0x69c2ec)");

// glitch::ps::PDSphere. ctor (0x69b9cc/0x69bb80) sorts the two radii into min and max,
// stores squared radii, the range, and a volume or area. generate (0x69c6ec) reads
// center (+0x04), max radius (+0x10), min radius (+0x14), range (+0x20) and the flag (+0x28).
// within (0x69bd34) reads the squared radii (+0x18, +0x1C). transform (0x69bdec) assigns the
// matrix translation to center.
struct PDSphere {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float center[3];                    // +0x04 (x, y, z); ctor, generate, transform (0x69bdec)
    float radius_max;                   // +0x10 max of the two radii; ctor (0x69b9cc), generate (0x69c6ec)
    float radius_min;                   // +0x14 min of the two radii; ctor, generate (0x69c6ec)
    float radius_max_sq;                // +0x18 radius_max squared; ctor, within (0x69bd34)
    float radius_min_sq;                // +0x1C radius_min squared; ctor, within (0x69bd34)
    float radius_range;                 // +0x20 radius_max - radius_min; ctor, generate (0x69c6ec)
    float volume_or_area;               // +0x24 surface area when the flag is set, else shell volume; ctor (0x69b9cc)
    std::uint8_t is_shell;              // +0x28 1 when radius_min == radius_max; ctor (0x69b9cc), generate (0x69c6ec)
};
static_assert(offsetof(PDSphere, center) == 0x04, "PDSphere::center");
static_assert(offsetof(PDSphere, radius_max) == 0x10, "PDSphere::radius_max");
static_assert(offsetof(PDSphere, radius_min) == 0x14, "PDSphere::radius_min");
static_assert(offsetof(PDSphere, radius_max_sq) == 0x18, "PDSphere::radius_max_sq");
static_assert(offsetof(PDSphere, radius_min_sq) == 0x1C, "PDSphere::radius_min_sq");
static_assert(offsetof(PDSphere, radius_range) == 0x20, "PDSphere::radius_range");
static_assert(offsetof(PDSphere, volume_or_area) == 0x24, "PDSphere::volume_or_area");
static_assert(offsetof(PDSphere, is_shell) == 0x28, "PDSphere::is_shell");
static_assert(sizeof(PDSphere) == 0x2C, "PDSphere size from copy (0x69c0ac)");

// glitch::ps::PDBlob. ctor (0x69be10/0x69beb8) stores center and sigma and derives
// two Gaussian constants. generate (0x69c688) reads center (+0x04) and sigma (+0x10).
// within (0x69df40) reads center, -1/(2 sigma^2) (+0x14) and the norm (+0x18).
struct PDBlob {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float center[3];                    // +0x04 (x, y, z); ctor (0x69be10), within (0x69df40), generate (0x69c688)
    float sigma;                        // +0x10 ctor argument; ctor (0x69be10), generate (0x69c688)
    float neg_half_inv_sigma_sq;        // +0x14 -0.5 / sigma^2 (ctor computes 1/(sigma+1e-12) squared times -0.5); within (0x69df40) reads it
    float gauss_norm;                   // +0x18 0.39894 / sigma; within (0x69df40) multiplies by it
};
static_assert(offsetof(PDBlob, center) == 0x04, "PDBlob::center");
static_assert(offsetof(PDBlob, sigma) == 0x10, "PDBlob::sigma");
static_assert(offsetof(PDBlob, neg_half_inv_sigma_sq) == 0x14, "PDBlob::neg_half_inv_sigma_sq");
static_assert(offsetof(PDBlob, gauss_norm) == 0x18, "PDBlob::gauss_norm");
static_assert(sizeof(PDBlob) == 0x1C, "PDBlob size from copy (0x69c044)");

// glitch::ps::PDCone. ctor (0x69c860/0x69ccc0) takes start, end and two radii.
// It builds start, axis, an orthonormal pair (u, v) around the axis, the radii, their squares,
// the range, 1/|axis|^2, a volume or area, and a flag. generate (0x69e008) and
// within (0x69b804) read those fields.
struct PDCone {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float start[3];                     // +0x04 start point; ctor (0x69c860), generate (0x69e008), within (0x69b804)
    float axis[3];                      // +0x10 end - start (raw); ctor, generate, within
    float u[3];                         // +0x1C normalized vector perpendicular to axis; ctor (normalize call), generate
    float v[3];                         // +0x28 axis-dir x u style third basis vector; ctor, generate
    float radius_max;                   // +0x34 max of the two radii; ctor, within (0x69b804)
    float radius_min;                   // +0x38 min of the two radii; ctor, generate, within
    float radius_max_sq;                // +0x3C radius_max squared; ctor, within
    float radius_min_sq;                // +0x40 radius_min squared; ctor, within
    float radius_range;                 // +0x44 radius_max - radius_min; ctor, generate (0x69e008)
    float inv_axis_len_sq;              // +0x48 1 / |axis|^2; ctor, within (multiplies projection)
    float volume_or_area;               // +0x4C volume or lateral area, chosen by the flag; ctor (0x69c860)
    std::uint8_t is_uniform;            // +0x50 1 when radius_min == radius_max; ctor (0x69c860)
};
static_assert(offsetof(PDCone, start) == 0x04, "PDCone::start");
static_assert(offsetof(PDCone, axis) == 0x10, "PDCone::axis");
static_assert(offsetof(PDCone, u) == 0x1C, "PDCone::u");
static_assert(offsetof(PDCone, v) == 0x28, "PDCone::v");
static_assert(offsetof(PDCone, radius_max) == 0x34, "PDCone::radius_max");
static_assert(offsetof(PDCone, radius_min) == 0x38, "PDCone::radius_min");
static_assert(offsetof(PDCone, radius_max_sq) == 0x3C, "PDCone::radius_max_sq");
static_assert(offsetof(PDCone, radius_min_sq) == 0x40, "PDCone::radius_min_sq");
static_assert(offsetof(PDCone, radius_range) == 0x44, "PDCone::radius_range");
static_assert(offsetof(PDCone, inv_axis_len_sq) == 0x48, "PDCone::inv_axis_len_sq");
static_assert(offsetof(PDCone, volume_or_area) == 0x4C, "PDCone::volume_or_area");
static_assert(offsetof(PDCone, is_uniform) == 0x50, "PDCone::is_uniform");
static_assert(sizeof(PDCone) == 0x54, "PDCone size from copy (0x69c134)");

// glitch::ps::PDCylinder. The (vec3, vec3, r1, r2) ctor (0x69d6c8/0x69db04) has the same
// field set as PDCone, with the flag at +0x54. The (float height, float radius) ctor
// (0x69d120/0x69d3f4) builds an axis-aligned cylinder along y and writes height at +0x50.
// generate (0x69e1bc) and transform (0x69e69c) read height and the start/axis/u/v fields.
struct PDCylinder {
    std::uint8_t pad_0x00[4];           // +0x00 vtable pointer
    float start[3];                     // +0x04 start point; both ctors, generate, transform
    float axis[3];                      // +0x10 end - start (raw); vec ctor (0x69d6c8) writes it; float ctor writes (0, height, 0)
    float u[3];                         // +0x1C normalized vector perpendicular to axis; both ctors (normalize), generate
    float v[3];                         // +0x28 third basis vector; both ctors, generate
    float radius_max;                   // +0x34 max radius; both ctors, generate
    float radius_min;                   // +0x38 min radius; both ctors, generate (0x69e1bc)
    float radius_max_sq;                // +0x3C radius_max squared; both ctors, within
    float radius_min_sq;                // +0x40 radius_min squared; both ctors, within
    float radius_range;                 // +0x44 radius_max - radius_min; both ctors, generate (0x69e1bc)
    float inv_axis_len_sq;              // +0x48 1 / |axis|^2; both ctors
    float volume_or_area;               // +0x4C volume (pi r^2 h in the float ctor); both ctors
    float height;                       // +0x50 height; float ctor (0x69d120) writes a2, transform (0x69e69c) reads it. Not written by the vec ctor
    std::uint8_t is_uniform;            // +0x54 1 when radius_min == radius_max; vec ctor (0x69d6c8) writes it
};
static_assert(offsetof(PDCylinder, start) == 0x04, "PDCylinder::start");
static_assert(offsetof(PDCylinder, axis) == 0x10, "PDCylinder::axis");
static_assert(offsetof(PDCylinder, u) == 0x1C, "PDCylinder::u");
static_assert(offsetof(PDCylinder, v) == 0x28, "PDCylinder::v");
static_assert(offsetof(PDCylinder, radius_max) == 0x34, "PDCylinder::radius_max");
static_assert(offsetof(PDCylinder, radius_min) == 0x38, "PDCylinder::radius_min");
static_assert(offsetof(PDCylinder, radius_max_sq) == 0x3C, "PDCylinder::radius_max_sq");
static_assert(offsetof(PDCylinder, radius_min_sq) == 0x40, "PDCylinder::radius_min_sq");
static_assert(offsetof(PDCylinder, radius_range) == 0x44, "PDCylinder::radius_range");
static_assert(offsetof(PDCylinder, inv_axis_len_sq) == 0x48, "PDCylinder::inv_axis_len_sq");
static_assert(offsetof(PDCylinder, volume_or_area) == 0x4C, "PDCylinder::volume_or_area");
static_assert(offsetof(PDCylinder, height) == 0x50, "PDCylinder::height");
static_assert(offsetof(PDCylinder, is_uniform) == 0x54, "PDCylinder::is_uniform");
static_assert(sizeof(PDCylinder) == 0x58, "PDCylinder size from copy (0x69c20c)");

}  // namespace glitch::ps

namespace glitch::ps::scene {

// glitch::scene::CParticleBoxEmitter. serializeAttributes (0x6f8d5c) reads box corners
// (+0x10 min, +0x1C max) and derives the half-extent, then Direction (+0x28, 3 floats),
// the particle rate, colors, life and angle. Bytes 0x00..0x0F and 0x4C..0x53 are not read by
// serializeAttributes; the 0x4C gap is not confirmed.
struct CParticleBoxEmitter {
    std::uint8_t pad_0x00[16];          // +0x00..+0x0F not read by serializeAttributes (base class / vtable, not confirmed)
    float box_min[3];                   // +0x10 (x, y, z) min corner; serializeAttributes (0x6f8d5c) reads it
    float box_max[3];                   // +0x1C (x, y, z) max corner; serializeAttributes (0x6f8d5c) reads it
    float direction[3];                 // +0x28 emission direction; serializeAttributes passes its address to the Direction setter
    std::int32_t min_particles_per_second;  // +0x34 int, serialized via vtbl+0x4C (int setter, inferred)
    std::int32_t max_particles_per_second;  // +0x38 int, serialized via vtbl+0x4C (int setter, inferred)
    std::uint32_t min_start_color;      // +0x3C packed color, serialized via vtbl+0x118
    std::uint32_t max_start_color;      // +0x40 packed color, serialized via vtbl+0x118
    std::int32_t min_life_time;         // +0x44 int, serialized via vtbl+0x4C (int setter, inferred)
    std::int32_t max_life_time;         // +0x48 int, serialized via vtbl+0x4C (int setter, inferred)
    std::uint8_t pad_0x4c[8];           // +0x4C..+0x53 not read by serializeAttributes (not confirmed)
    std::int32_t max_angle_degrees;     // +0x54 int, serialized via vtbl+0x4C (int setter, inferred)
};
static_assert(offsetof(CParticleBoxEmitter, box_min) == 0x10, "CParticleBoxEmitter::box_min");
static_assert(offsetof(CParticleBoxEmitter, box_max) == 0x1C, "CParticleBoxEmitter::box_max");
static_assert(offsetof(CParticleBoxEmitter, direction) == 0x28, "CParticleBoxEmitter::direction");
static_assert(offsetof(CParticleBoxEmitter, min_particles_per_second) == 0x34, "CParticleBoxEmitter::min_particles_per_second");
static_assert(offsetof(CParticleBoxEmitter, max_particles_per_second) == 0x38, "CParticleBoxEmitter::max_particles_per_second");
static_assert(offsetof(CParticleBoxEmitter, min_start_color) == 0x3C, "CParticleBoxEmitter::min_start_color");
static_assert(offsetof(CParticleBoxEmitter, max_start_color) == 0x40, "CParticleBoxEmitter::max_start_color");
static_assert(offsetof(CParticleBoxEmitter, min_life_time) == 0x44, "CParticleBoxEmitter::min_life_time");
static_assert(offsetof(CParticleBoxEmitter, max_life_time) == 0x48, "CParticleBoxEmitter::max_life_time");
static_assert(offsetof(CParticleBoxEmitter, max_angle_degrees) == 0x54, "CParticleBoxEmitter::max_angle_degrees");

// glitch::scene::CParticlePointEmitter. serializeAttributes (0x6fbcd8) reads Direction (+0x48)
// and the same rate/color/life/angle group as the box emitter, shifted by +0x30.
// Bytes 0x00..0x47 are not read by serializeAttributes (not confirmed).
struct CParticlePointEmitter {
    std::uint8_t pad_0x00[72];          // +0x00..+0x47 not read by serializeAttributes (base class / vtable, not confirmed)
    float direction[3];                 // +0x48 emission direction; serializeAttributes (0x6fbcd8) passes its address
    std::int32_t min_particles_per_second;  // +0x54 int, serialized via vtbl+0x4C (int setter, inferred)
    std::int32_t max_particles_per_second;  // +0x58 int, serialized via vtbl+0x4C (int setter, inferred)
    std::uint32_t min_start_color;      // +0x5C packed color, serialized via vtbl+0x118
    std::uint32_t max_start_color;      // +0x60 packed color, serialized via vtbl+0x118
    std::int32_t min_life_time;         // +0x64 int, serialized via vtbl+0x4C (int setter, inferred)
    std::int32_t max_life_time;         // +0x68 int, serialized via vtbl+0x4C (int setter, inferred)
    std::int32_t max_angle_degrees;     // +0x6C int, serialized via vtbl+0x4C (int setter, inferred)
};
static_assert(offsetof(CParticlePointEmitter, direction) == 0x48, "CParticlePointEmitter::direction");
static_assert(offsetof(CParticlePointEmitter, min_particles_per_second) == 0x54, "CParticlePointEmitter::min_particles_per_second");
static_assert(offsetof(CParticlePointEmitter, max_particles_per_second) == 0x58, "CParticlePointEmitter::max_particles_per_second");
static_assert(offsetof(CParticlePointEmitter, min_start_color) == 0x5C, "CParticlePointEmitter::min_start_color");
static_assert(offsetof(CParticlePointEmitter, max_start_color) == 0x60, "CParticlePointEmitter::max_start_color");
static_assert(offsetof(CParticlePointEmitter, min_life_time) == 0x64, "CParticlePointEmitter::min_life_time");
static_assert(offsetof(CParticlePointEmitter, max_life_time) == 0x68, "CParticlePointEmitter::max_life_time");
static_assert(offsetof(CParticlePointEmitter, max_angle_degrees) == 0x6C, "CParticlePointEmitter::max_angle_degrees");

// glitch::scene::CParticleFadeOutAffector. serializeAttributes (0x6fa750) packs TargetColor from
// the four bytes at +0x05..+0x08 and writes FadeOutTime (+0x0C). deserializeAttributes (0x6fa804)
// stores a 32-bit color at +0x05 (unaligned), so it is kept as a byte array.
struct CParticleFadeOutAffector {
    std::uint8_t pad_0x00[5];           // +0x00..+0x04 not read by the (de)serializers (not confirmed)
    std::uint8_t target_color_bytes[4]; // +0x05 packed TargetColor, one byte each (b0 at +0x05, b3 at +0x08); serialize (0x6fa750) and deserialize (0x6fa804) use it
    std::uint8_t pad_0x09[3];           // +0x09..+0x0B not read by the (de)serializers (not confirmed)
    float fade_out_time;                // +0x0C FadeOutTime; serialize (0x6fa750) via vtbl+0x64, deserialize (0x6fa804) via vtbl+0x74
};
static_assert(offsetof(CParticleFadeOutAffector, target_color_bytes) == 0x05, "CParticleFadeOutAffector::target_color_bytes");
static_assert(offsetof(CParticleFadeOutAffector, fade_out_time) == 0x0C, "CParticleFadeOutAffector::fade_out_time");

// glitch::scene::CParticleGravityAffector. serializeAttributes (0x6fad3c) writes Gravity (+0x0C,
// vec3) and TimeForceLost (+0x08). deserializeAttributes (0x6fadd8) stores the same fields.
struct CParticleGravityAffector {
    std::uint8_t pad_0x00[8];           // +0x00..+0x07 not read by the (de)serializers (vtable / base, not confirmed)
    float time_force_lost;              // +0x08 TimeForceLost; serialize (0x6fad3c) via vtbl+0x64, deserialize (0x6fadd8) via vtbl+0x74
    float gravity[3];                   // +0x0C (x, y, z) Gravity vector; serialize (0x6fad3c) via vtbl+0x1A8, deserialize (0x6fadd8) via vtbl+0x1B8
};
static_assert(offsetof(CParticleGravityAffector, time_force_lost) == 0x08, "CParticleGravityAffector::time_force_lost");
static_assert(offsetof(CParticleGravityAffector, gravity) == 0x0C, "CParticleGravityAffector::gravity");

// glitch::scene::CParticleSizeAffector. serializeAttributes (0x6fd400) writes TargetSize (+0x08),
// Variation (+0x0C), GrowForTime (+0x10) and FadeForTime (+0x14). deserializeAttributes (0x6fd4e4)
// reads them in that order. Field types follow the float setter (vtbl+0x64).
struct CParticleSizeAffector {
    std::uint8_t pad_0x00[8];           // +0x00..+0x07 not read by the (de)serializers (vtable / base, not confirmed)
    float target_size;                  // +0x08 TargetSize; serialize (0x6fd400), deserialize (0x6fd4e4)
    float variation;                    // +0x0C Variation; serialize (0x6fd400), deserialize (0x6fd4e4)
    float grow_for_time;                // +0x10 GrowForTime; serialize (0x6fd400), deserialize (0x6fd4e4)
    float fade_for_time;                // +0x14 FadeForTime; serialize (0x6fd400), deserialize (0x6fd4e4)
};
static_assert(offsetof(CParticleSizeAffector, target_size) == 0x08, "CParticleSizeAffector::target_size");
static_assert(offsetof(CParticleSizeAffector, variation) == 0x0C, "CParticleSizeAffector::variation");
static_assert(offsetof(CParticleSizeAffector, grow_for_time) == 0x10, "CParticleSizeAffector::grow_for_time");
static_assert(offsetof(CParticleSizeAffector, fade_for_time) == 0x14, "CParticleSizeAffector::fade_for_time");

// glitch::scene::CParticleSpinAffector. serializeAttributes (0x6fe498) writes SpinTime as float
// converted from the unsigned value at +0x08, and Variation (+0x0C). deserializeAttributes
// (0x6fe540) converts the float back into the unsigned slot at +0x08.
struct CParticleSpinAffector {
    std::uint8_t pad_0x00[8];           // +0x00..+0x07 not read by the (de)serializers (vtable / base, not confirmed)
    std::uint32_t spin_time;            // +0x08 SpinTime, unsigned; serialize (0x6fe498) converts to float, deserialize (0x6fe540) truncates back
    float variation;                    // +0x0C Variation; serialize (0x6fe498), deserialize (0x6fe540)
};
static_assert(offsetof(CParticleSpinAffector, spin_time) == 0x08, "CParticleSpinAffector::spin_time");
static_assert(offsetof(CParticleSpinAffector, variation) == 0x0C, "CParticleSpinAffector::variation");

}  // namespace glitch::ps::scene

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_PS_MEMBERS_HPP
