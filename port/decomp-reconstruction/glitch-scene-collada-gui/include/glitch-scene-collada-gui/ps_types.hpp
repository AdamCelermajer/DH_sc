// glitch::ps shape types, particle-system accessors and the empty model
// initialisers. Each template maps only the member its accessor uses.
#pragma once

#include <cstdint>

namespace glitch::ps {

// Emitter distribution shapes; the type tag is the shape index. The shapes are
// stand-ins without the PDomain base, so each destructor is its own empty body.
class PDPoint {
public:
    // 0x69ac90 _ZN6glitch2ps7PDPointD1Ev (empty)
    ~PDPoint() = default;
    // 0x69ac94 _ZNK6glitch2ps7PDPoint7getTypeEv (returns 3)
    std::uint32_t getType() const;
};

class PDLine {
public:
    // 0x69ac9c _ZN6glitch2ps6PDLineD1Ev (empty)
    ~PDLine() = default;
    // 0x69aca0 _ZNK6glitch2ps6PDLine7getTypeEv (returns 4)
    std::uint32_t getType() const;
};

class PDBox {
public:
    // 0x69aca8 _ZN6glitch2ps5PDBoxD1Ev (empty)
    ~PDBox() = default;
    // 0x69acac _ZNK6glitch2ps5PDBox7getTypeEv (returns 0)
    std::uint32_t getType() const;
};

class PDCylinder {
public:
    // 0x69acb4 _ZN6glitch2ps10PDCylinderD1Ev (empty)
    ~PDCylinder() = default;
    // 0x69acb8 _ZNK6glitch2ps10PDCylinder7getTypeEv (returns 2)
    std::uint32_t getType() const;
};

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
    // 0x63802c GNPSSizeModel<GNPSParticle>::initPSizeModel() (empty)
    void initPSizeModel() {}
};

template <typename Particle>
class PSizeModel {
public:
    // 0x64bd08 PSizeModel<SParticle>::initPSizeModel() (empty)
    void initPSizeModel() {}
};

template <typename Particle>
class GNPSColorModel {
public:
    // 0x6380d8 GNPSColorModel<GNPSParticle>::initPColorModel() (empty)
    void initPColorModel() {}
};

template <typename Particle>
class PColorModel {
public:
    // 0x64beec PColorModel<SParticle>::initPColorModel() (empty)
    void initPColorModel() {}
};

template <typename Particle>
class GNPSMotionModel {
public:
    // 0x638634 GNPSMotionModel<GNPSParticle>::initPMotionModel() (empty)
    void initPMotionModel() {}
};

template <typename Particle>
class PMotionModel {
public:
    // 0x64c17c PMotionModel<SParticle>::initPMotionModel() (empty)
    void initPMotionModel() {}
};

template <typename Particle>
class PForcesModel {
public:
    // 0x638648 PForcesModel<GNPSParticle>::initPForces(GNPSParticle*, GNPSParticle*) (empty)
    // 0x64c234 PForcesModel<SParticle>::initPForces(SParticle*, SParticle*) (empty)
    void initPForces(Particle* /*first*/, Particle* /*second*/) {}
};

template <typename Particle>
class GNPSSpinModel {
public:
    // 0x63865c GNPSSpinModel<GNPSParticle>::initPSpinModel() (empty)
    void initPSpinModel() {}
};

template <typename Particle>
class PSpinModel {
public:
    // 0x64c248 PSpinModel<SParticle>::initPSpinModel() (empty)
    void initPSpinModel() {}
};

template <typename Particle>
class GNPSLifeModel {
public:
    // 0x638670 GNPSLifeModel<GNPSParticle>::initPLifeModel() (empty)
    void initPLifeModel() {}
};

template <typename Particle>
class PLifeModel {
public:
    // 0x64c2e8 PLifeModel<SParticle>::initPLifeModel() (empty)
    void initPLifeModel() {}
};

}  // namespace glitch::ps
