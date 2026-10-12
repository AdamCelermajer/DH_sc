// glitch::scene particle affectors: attraction, gravity, rotation, size, spin
// and fade-out. Each affector keeps its settings in typed members at the
// original offsets (kept as comments). The shared interface flag at +0x04
// (set by every constructor, tested by affect) is named `enabled`. The
// interface base and the per-particle update are not reconstructed, so the
// methods are plain members and affect() only runs its guard.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::scene {

struct SParticle;

class CParticleAttractionAffector {
public:
    // 0x6f845c _ZN6glitch5scene27CParticleAttractionAffectorC2ERKNS_4core8vector3dIfEEfbbbb
    // 0x6f8528 _ZN6glitch5scene27CParticleAttractionAffectorC1ERKNS_4core8vector3dIfEEfbbbb
    // Constructor (C2 and C1). The point is copied; lastTime starts at zero.
    CParticleAttractionAffector(const core::vector3d<float>& point, float speed, bool attract,
                                bool affectX, bool affectY, bool affectZ);
    // 0x6f8634 _ZN6glitch5scene27CParticleAttractionAffectorD1Ev
    // (D1). Empty in the original.
    ~CParticleAttractionAffector() = default;

    // 0x6f86cc _ZN6glitch5scene27CParticleAttractionAffector6affectEjPNS0_9SParticleEj
    // Partial: the time step from the previous call is computed and
    // lastTime is stored. The per-particle attraction (ARM 372 bytes) is not
    // reconstructed.
    void affect(std::uint32_t now, SParticle* particles, std::uint32_t count);

    // 0x6f83a0 _ZN6glitch5scene27CParticleAttractionAffector8setPointERKNS_4core8vector3dIfEE
    void setPoint(const core::vector3d<float>& value);
    // 0x6f83bc _ZN6glitch5scene27CParticleAttractionAffector8setSpeedEf
    void setSpeed(float value);
    // 0x6f83c4 _ZN6glitch5scene27CParticleAttractionAffector10setAttractEb
    void setAttract(bool value);
    // 0x6f83cc _ZN6glitch5scene27CParticleAttractionAffector10setAffectXEb
    void setAffectX(bool value);
    // 0x6f83d4 _ZN6glitch5scene27CParticleAttractionAffector10setAffectYEb
    void setAffectY(bool value);
    // 0x6f83dc _ZN6glitch5scene27CParticleAttractionAffector10setAffectZEb
    void setAffectZ(bool value);

    // 0x6f83e4 _ZNK6glitch5scene27CParticleAttractionAffector8getPointEv
    const core::vector3d<float>& getPoint() const;
    // 0x6f83ec _ZNK6glitch5scene27CParticleAttractionAffector8getSpeedEv
    float getSpeed() const;
    // 0x6f83f4 _ZNK6glitch5scene27CParticleAttractionAffector10getAttractEv
    bool getAttract() const;
    // 0x6f83fc _ZNK6glitch5scene27CParticleAttractionAffector10getAffectXEv
    bool getAffectX() const;
    // 0x6f8404 _ZNK6glitch5scene27CParticleAttractionAffector10getAffectYEv
    bool getAffectY() const;
    // 0x6f840c _ZNK6glitch5scene27CParticleAttractionAffector10getAffectZEv
    bool getAffectZ() const;

private:
    bool enabled = false;                            // +0x04
    core::vector3d<float> point;                     // +0x08
    float speed = 0.0f;                              // +0x14
    bool affectX = false;                            // +0x18
    bool affectY = false;                            // +0x19
    bool affectZ = false;                            // +0x1a
    bool attract = false;                            // +0x1b
    std::uint32_t lastTime = 0;                      // +0x1c
};

class CParticleGravityAffector {
public:
    // 0x6faa54 _ZN6glitch5scene24CParticleGravityAffectorC2ERKNS_4core8vector3dIfEEj
    // 0x6fab1c _ZN6glitch5scene24CParticleGravityAffectorC1ERKNS_4core8vector3dIfEEj
    // Constructor (C2 or C1). timeForceLost is stored as a float.
    CParticleGravityAffector(const core::vector3d<float>& gravity, std::uint32_t timeForceLost);
    // 0x6fad94 _ZN6glitch5scene24CParticleGravityAffectorD1Ev
    // (D1). Empty in the original.
    ~CParticleGravityAffector() = default;

    // 0x6fac14 _ZN6glitch5scene24CParticleGravityAffector6affectEjPNS0_9SParticleEj
    // Partial: only the enabled and count guard is reconstructed; the
    // per-particle update is not.
    void affect(std::uint32_t now, SParticle* particles, std::uint32_t count);

    // 0x6fa9fc _ZN6glitch5scene24CParticleGravityAffector16setTimeForceLostEf
    void setTimeForceLost(float value);
    // 0x6faa04 _ZN6glitch5scene24CParticleGravityAffector10setGravityERKNS_4core8vector3dIfEE
    void setGravity(const core::vector3d<float>& value);

    // 0x6faa20 _ZNK6glitch5scene24CParticleGravityAffector16getTimeForceLostEv
    float getTimeForceLost() const;
    // 0x6faa28 _ZNK6glitch5scene24CParticleGravityAffector10getGravityEv
    const core::vector3d<float>& getGravity() const;

private:
    bool enabled = false;                            // +0x04
    float timeForceLost = 0.0f;                      // +0x08
    core::vector3d<float> gravity;                   // +0x0c
};

class CParticleRotationAffector {
public:
    // 0x6fcd74 _ZN6glitch5scene25CParticleRotationAffectorC2ERKNS_4core8vector3dIfEES6_
    // 0x6fce34 _ZN6glitch5scene25CParticleRotationAffectorC1ERKNS_4core8vector3dIfEES6_
    // Constructor (C2 or C1). The pivot point and speed are copied; lastTime
    // starts at zero.
    CParticleRotationAffector(const core::vector3d<float>& pivotPoint,
                              const core::vector3d<float>& speed);
    // 0x6fcf28 _ZN6glitch5scene25CParticleRotationAffectorD1Ev
    // (D1). Empty in the original.
    ~CParticleRotationAffector() = default;

    // 0x6fcf6c _ZN6glitch5scene25CParticleRotationAffector6affectEjPNS0_9SParticleEj
    // Partial: the time step from the previous call is computed and
    // lastTime is stored. The per-particle rotation is not reconstructed.
    void affect(std::uint32_t now, SParticle* particles, std::uint32_t count);

    // 0x6fcd08 _ZN6glitch5scene25CParticleRotationAffector13setPivotPointERKNS_4core8vector3dIfEE
    void setPivotPoint(const core::vector3d<float>& value);
    // 0x6fcd24 _ZN6glitch5scene25CParticleRotationAffector8setSpeedERKNS_4core8vector3dIfEE
    void setSpeed(const core::vector3d<float>& value);

    // 0x6fcd40 _ZNK6glitch5scene25CParticleRotationAffector13getPivotPointEv
    const core::vector3d<float>& getPivotPoint() const;
    // 0x6fcd48 _ZNK6glitch5scene25CParticleRotationAffector8getSpeedEv
    const core::vector3d<float>& getSpeed() const;

private:
    bool enabled = false;                            // +0x04
    core::vector3d<float> pivotPoint;                // +0x08
    core::vector3d<float> speed;                     // +0x14
    std::uint32_t lastTime = 0;                      // +0x20
};

class CParticleSizeAffector {
public:
    // 0x6fd250 _ZN6glitch5scene21CParticleSizeAffectorC2Effjj
    // 0x6fd310 _ZN6glitch5scene21CParticleSizeAffectorC1Effjj
    // Constructor (C2 or C1). The two uint times are converted to floats.
    CParticleSizeAffector(float targetSize, float variation, std::uint32_t growForTime,
                          std::uint32_t fadeForTime);
    // 0x6fd4a0 _ZN6glitch5scene21CParticleSizeAffectorD1Ev
    // (D1). Empty in the original.
    ~CParticleSizeAffector() = default;

    // 0x6fd63c _ZN6glitch5scene21CParticleSizeAffector6affectEjPNS0_9SParticleEj
    // Partial: only the enabled and count guard is reconstructed; the
    // per-particle size update is not.
    void affect(std::uint32_t now, SParticle* particles, std::uint32_t count);

    // 0x6fd1ec _ZN6glitch5scene21CParticleSizeAffector13setTargetSizeEf
    void setTargetSize(float value);
    // 0x6fd1f4 _ZN6glitch5scene21CParticleSizeAffector12setVariationEf
    void setVariation(float value);
    // 0x6fd1fc _ZN6glitch5scene21CParticleSizeAffector14setGrowForTimeEf
    void setGrowForTime(float value);
    // 0x6fd204 _ZN6glitch5scene21CParticleSizeAffector14setFadeForTimeEf
    void setFadeForTime(float value);

    // 0x6fd20c _ZNK6glitch5scene21CParticleSizeAffector13getTargetSizeEv
    float getTargetSize() const;
    // 0x6fd214 _ZNK6glitch5scene21CParticleSizeAffector12getVariationEv
    float getVariation() const;
    // 0x6fd21c _ZNK6glitch5scene21CParticleSizeAffector14getGrowForTimeEv
    float getGrowForTime() const;
    // 0x6fd224 _ZNK6glitch5scene21CParticleSizeAffector14getFadeForTimeEv
    float getFadeForTime() const;

private:
    bool enabled = false;                            // +0x04
    float targetSize = 0.0f;                         // +0x08
    float variation = 0.0f;                          // +0x0c
    float growForTime = 0.0f;                        // +0x10
    float fadeForTime = 0.0f;                        // +0x14
};

class CParticleSpinAffector {
public:
    // 0x6fe310 _ZN6glitch5scene21CParticleSpinAffectorC2Ejf
    // 0x6fe3bc _ZN6glitch5scene21CParticleSpinAffectorC1Ejf
    // Constructor (C2 or C1).
    CParticleSpinAffector(std::uint32_t spinTime, float variation);
    // 0x6fe4fc _ZN6glitch5scene21CParticleSpinAffectorD1Ev
    // (D1). Empty in the original.
    ~CParticleSpinAffector() = default;

    // 0x6fe600 _ZN6glitch5scene21CParticleSpinAffector6affectEjPNS0_9SParticleEj
    // Partial: only the enabled and count guard is reconstructed; the
    // per-particle spin is not.
    void affect(std::uint32_t now, SParticle* particles, std::uint32_t count);

    // 0x6fe2b4 _ZN6glitch5scene21CParticleSpinAffector11setSpinTimeEf
    void setSpinTime(float value);
    // 0x6fe2cc _ZN6glitch5scene21CParticleSpinAffector12setVariationEf
    void setVariation(float value);

    // 0x6fe2d4 _ZNK6glitch5scene21CParticleSpinAffector11getSpinTimeEv
    float getSpinTime() const;
    // 0x6fe2e4 _ZNK6glitch5scene21CParticleSpinAffector12getVariationEv
    float getVariation() const;

private:
    bool enabled = false;                            // +0x04
    std::uint32_t spinTime = 0;                      // +0x08
    float variation = 0.0f;                          // +0x0c
};

class CParticleFadeOutAffector {
public:
    // 0x6fa4d4 _ZN6glitch5scene24CParticleFadeOutAffectorC2ERKNS_5video6SColorEj
    // 0x6fa5a4 _ZN6glitch5scene24CParticleFadeOutAffectorC1ERKNS_5video6SColorEj
    // Constructor (C2 or C1). The colour is copied to +0x05 in the original,
    // which is unaligned there; it is stored here as a whole colour value.
    CParticleFadeOutAffector(const video::SColor& targetColor, std::uint32_t fadeOutTime);
    // 0x6fa7c0 _ZN6glitch5scene24CParticleFadeOutAffectorD1Ev
    // (D1). Empty in the original.
    ~CParticleFadeOutAffector() = default;

    // 0x6fa69c _ZN6glitch5scene24CParticleFadeOutAffector6affectEjPNS0_9SParticleEj
    // Partial: only the enabled and count guard is reconstructed; the
    // per-particle fade is not.
    void affect(std::uint32_t now, SParticle* particles, std::uint32_t count);

    // 0x6fa484 _ZN6glitch5scene24CParticleFadeOutAffector14setTargetColorERKNS_5video6SColorE
    void setTargetColor(const video::SColor& value);
    // 0x6fa498 _ZN6glitch5scene24CParticleFadeOutAffector14setFadeOutTimeEf
    void setFadeOutTime(float value);

    // 0x6fa4a0 _ZNK6glitch5scene24CParticleFadeOutAffector14getTargetColorEv
    const video::SColor& getTargetColor() const;
    // 0x6fa4a8 _ZNK6glitch5scene24CParticleFadeOutAffector14getFadeOutTimeEv
    float getFadeOutTime() const;

private:
    bool enabled = false;                            // +0x04
    video::SColor targetColor;                       // +0x05 (unaligned in the original)
    float fadeOutTime = 0.0f;                        // +0x0c
};

}  // namespace glitch::scene
