// Declarations for decompiled-function reconstruction slice 18.
// Stand-in types only: the real layouts live in other slices and headers.
// Every member offset that is used is kept as a comment (// +0x..).
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::collada::recon_18 {

// Incomplete stand-ins for types owned by other slices.
struct CColladaDatabase;
struct IVideoDriver;
struct SGeometry;
struct SEffect;
struct SEffectList;
struct SLight;
struct SLibraryAnimationClips;
struct SNewParam;
struct CRootSceneNode;
struct CMaterialRenderer;
struct SEmitter;
struct SGNPSEmitter;
struct CMesh;
struct CLightSceneNode;
struct CSceneNodeAnimator;
struct CGlitchNewParticleSystemSceneNode;

// Particle record sizes: GNPSParticle = 39 floats (156 bytes), SParticle = 25 floats (100 bytes).
inline constexpr std::size_t kGNPSParticleFloats = 39;
inline constexpr std::size_t kSParticleFloats = 25;

// Random source used by the force and model code (stand-in for lrand48).
// Returns a non-negative 31-bit integer.
using NextInt31Fn = std::int32_t (*)();

// glitch::ps::IParticleContext::hashString (boost hash_combine style).
std::uint32_t hashString(const char* text);

// glitch::ps::PSRandom stand-in. Rand() returns a float in the range the
// original generator produces; the definition is supplied by the owner slice.
struct PSRandom {
    float Rand();
};

// glitch::ps::PSRandom::RandVec: fills out[0..2] with three consecutive Rand() calls.
void PSRandomRandVec(PSRandom* self, float out[3]);

// glitch::ps::PSManager::getInstance. Meyers singleton.
struct PSManager {
    static PSManager& getInstance();
};

// Force-owner view used by PWind and PGravity (fields at the offsets the
// reconstructed apply loops read).
struct ForceOwner {
    std::uint8_t pad_0x00[32];   // +0x00..+0x1f not modelled
    float direction[3];          // +0x20 (32)
    std::uint8_t pad_0x2c[4];    // +0x2c
    float origin[3];             // +0x30 (48)
    std::uint8_t pad_0x3c[4];    // +0x3c
    std::uint8_t normalized;     // +0x40 (64)
};

// PWind force record (the float** a1 target in the pseudocode).
struct WindForce {
    ForceOwner* owner;           // +0x00
    float strength;              // +0x04 (scaled by 1000 when applied)
    float decay;                 // +0x08
    float turbulence;            // +0x0c
    float turbulenceFrequency;   // +0x10
    std::int32_t hasOrigin;      // +0x18
};

// PGravity force record.
struct GravityForce {
    ForceOwner* owner;           // +0x00
    float strength;              // +0x04 (scaled by 1000 when applied)
    float decay;                 // +0x08
    std::int32_t hasOrigin;      // +0x0c
};

// glitch::ps::PWind::apply<T>. StrideFloats = particle record size in floats.
// Particle layout: position at [0..2]. The float* range is [first, last).
template <std::size_t StrideFloats>
void applyWind(WindForce* force, float* first, float* last, float timeStep, NextInt31Fn nextInt31);

// glitch::ps::PGravity::apply<T>. Velocity at [3..5].
template <std::size_t StrideFloats>
void applyGravity(GravityForce* force, float* first, float* last, float timeStep);

}  // namespace glitch::collada::recon_18
