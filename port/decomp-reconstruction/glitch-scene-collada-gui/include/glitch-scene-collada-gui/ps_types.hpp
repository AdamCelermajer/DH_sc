// glitch::ps shape types, particle-system accessors and the empty model
// initialisers. Each template maps only the member its accessor uses.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/ps/PDomain.hpp"

namespace glitch::core {
template <typename T>
class CMatrix4;  // only named by parameter types here; the layout is not mapped
}  // namespace glitch::core

namespace glitch::ps {

// The shapes (PDPoint .. PDCylinder) and PSRandom are the canonical classes of ps/PDomain.hpp.

// Particle system base: the world matrix is reached through a stored pointer.
template <typename Particle>
class IParticleSystem {
public:
    // 0x637bcc _ZNK6glitch2ps15IParticleSystemINS0_12GNPSParticleEE17getWorldMatrixPtrEv
    // 0x64bbc8 _ZNK6glitch2ps15IParticleSystemINS0_9SParticleEE17getWorldMatrixPtrEv
    // Returns the pointer stored at +0x0c, for both particle types.
    const float* getWorldMatrixPtr() const { return worldMatrixPtr; }

    const float* worldMatrixPtr = nullptr;  // +0x0c
};

// Model initialisers whose bodies are empty in the original. The template
// argument is the particle type of the instantiation.
template <typename Particle>
class GNPSSizeModel {
public:
    // 0x63802c _ZN6glitch2ps13GNPSSizeModelINS0_12GNPSParticleEE14initPSizeModelEv (empty)
    void initPSizeModel() {}
};

template <typename Particle>
class PSizeModel {
public:
    // 0x64bd08 _ZN6glitch2ps10PSizeModelINS0_9SParticleEE14initPSizeModelEv (empty)
    void initPSizeModel() {}
};

template <typename Particle>
class GNPSColorModel {
public:
    // 0x6380d8 _ZN6glitch2ps14GNPSColorModelINS0_12GNPSParticleEE15initPColorModelEv (empty)
    void initPColorModel() {}
    // 0x638524 _ZN6glitch2ps14GNPSColorModelINS0_12GNPSParticleEE22hasColorAnimationTrackEv
    // Returns 1 when the colour animation track at +0x08 is set.
    int hasColorAnimationTrack() const { return colorAnimationTrack != nullptr ? 1 : 0; }

    const void* colorAnimationTrack = nullptr;  // +0x08
};

template <typename Particle>
class PColorModel {
public:
    // 0x64beec _ZN6glitch2ps11PColorModelINS0_9SParticleEE15initPColorModelEv (empty)
    void initPColorModel() {}
    // 0x64c06c _ZN6glitch2ps11PColorModelINS0_9SParticleEE22hasColorAnimationTrackEv
    // Returns 1 when the colour animation track at +0x08 is set.
    int hasColorAnimationTrack() const { return colorAnimationTrack != nullptr ? 1 : 0; }

    const void* colorAnimationTrack = nullptr;  // +0x08
};

template <typename Particle>
class GNPSMotionModel {
public:
    // 0x638634 _ZN6glitch2ps15GNPSMotionModelINS0_12GNPSParticleEE16initPMotionModelEv (empty)
    void initPMotionModel() {}
};

template <typename Particle>
class PMotionModel {
public:
    // 0x64c17c _ZN6glitch2ps12PMotionModelINS0_9SParticleEE16initPMotionModelEv (empty)
    void initPMotionModel() {}
};

template <typename Particle>
class PForcesModel {
public:
    // 0x638648 _ZN6glitch2ps12PForcesModelINS0_12GNPSParticleEE11initPForcesEPS2_S4_ (empty)
    // 0x64c234 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE11initPForcesEPS2_S4_ (empty)
    void initPForces(Particle* /*first*/, Particle* /*second*/) {}
};

template <typename Particle>
class GNPSSpinModel {
public:
    // 0x63865c _ZN6glitch2ps13GNPSSpinModelINS0_12GNPSParticleEE14initPSpinModelEv (empty)
    void initPSpinModel() {}
};

template <typename Particle>
class PSpinModel {
public:
    // 0x64c248 _ZN6glitch2ps10PSpinModelINS0_9SParticleEE14initPSpinModelEv (empty)
    void initPSpinModel() {}
};

template <typename Particle>
class GNPSLifeModel {
public:
    // 0x638670 _ZN6glitch2ps13GNPSLifeModelINS0_12GNPSParticleEE14initPLifeModelEv (empty)
    void initPLifeModel() {}
};

template <typename Particle>
class PLifeModel {
public:
    // 0x64c2e8 _ZN6glitch2ps10PLifeModelINS0_9SParticleEE14initPLifeModelEv (empty)
    void initPLifeModel() {}
};

// Particle-system heap helpers. PS_NEW calls GlitchAlloc with hint 0 in the original; the
// port uses std::malloc (partial). PS_FREE is std::free in the original.
// 0x64d290 _ZN6glitch2ps6PS_NEWEj
void* PS_NEW(std::uint32_t size);
// 0x64d778 _ZN6glitch2ps7PS_FREEEPv
void PS_FREE(void* p);

}  // namespace glitch::ps
