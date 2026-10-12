#include "glitch-scene-collada-gui/particle_affectors.hpp"

namespace glitch::scene {

// ---- CParticleAttractionAffector ----

// 0x6f845c _ZN6glitch5scene27CParticleAttractionAffectorC2ERKNS_4core8vector3dIfEEfbbbb
// 0x6f8528 _ZN6glitch5scene27CParticleAttractionAffectorC1ERKNS_4core8vector3dIfEEfbbbb
CParticleAttractionAffector::CParticleAttractionAffector(const core::vector3d<float>& pointValue,
                                                         float speedValue, bool attractValue,
                                                         bool affectXValue, bool affectYValue,
                                                         bool affectZValue)
    : enabled(true),
      point(pointValue),
      speed(speedValue),
      affectX(affectXValue),
      affectY(affectYValue),
      affectZ(affectZValue),
      attract(attractValue),
      lastTime(0) {}

// 0x6f86cc _ZN6glitch5scene27CParticleAttractionAffector6affectEjPNS0_9SParticleEj
// Partial: the time step from the previous call is computed and lastTime is
// stored. The per-particle attraction is not reconstructed, so the step is
// not applied to any particle.
void CParticleAttractionAffector::affect(std::uint32_t now, SParticle* /*particles*/,
                                         std::uint32_t /*count*/) {
    if (lastTime != 0) {
        [[maybe_unused]] const float timeStep = static_cast<float>(now - lastTime) / 1000.0f;
    }
    lastTime = now;
}

// 0x6f83a0 _ZN6glitch5scene27CParticleAttractionAffector8setPointERKNS_4core8vector3dIfEE
void CParticleAttractionAffector::setPoint(const core::vector3d<float>& value) { point = value; }

// 0x6f83bc _ZN6glitch5scene27CParticleAttractionAffector8setSpeedEf
void CParticleAttractionAffector::setSpeed(float value) { speed = value; }

// 0x6f83c4 _ZN6glitch5scene27CParticleAttractionAffector10setAttractEb
void CParticleAttractionAffector::setAttract(bool value) { attract = value; }

// 0x6f83cc _ZN6glitch5scene27CParticleAttractionAffector10setAffectXEb
void CParticleAttractionAffector::setAffectX(bool value) { affectX = value; }

// 0x6f83d4 _ZN6glitch5scene27CParticleAttractionAffector10setAffectYEb
void CParticleAttractionAffector::setAffectY(bool value) { affectY = value; }

// 0x6f83dc _ZN6glitch5scene27CParticleAttractionAffector10setAffectZEb
void CParticleAttractionAffector::setAffectZ(bool value) { affectZ = value; }

// 0x6f83e4 _ZNK6glitch5scene27CParticleAttractionAffector8getPointEv
const core::vector3d<float>& CParticleAttractionAffector::getPoint() const { return point; }

// 0x6f83ec _ZNK6glitch5scene27CParticleAttractionAffector8getSpeedEv
float CParticleAttractionAffector::getSpeed() const { return speed; }

// 0x6f83f4 _ZNK6glitch5scene27CParticleAttractionAffector10getAttractEv
bool CParticleAttractionAffector::getAttract() const { return attract; }

// 0x6f83fc _ZNK6glitch5scene27CParticleAttractionAffector10getAffectXEv
bool CParticleAttractionAffector::getAffectX() const { return affectX; }

// 0x6f8404 _ZNK6glitch5scene27CParticleAttractionAffector10getAffectYEv
bool CParticleAttractionAffector::getAffectY() const { return affectY; }

// 0x6f840c _ZNK6glitch5scene27CParticleAttractionAffector10getAffectZEv
bool CParticleAttractionAffector::getAffectZ() const { return affectZ; }

// ---- CParticleGravityAffector ----

// 0x6faa54 _ZN6glitch5scene24CParticleGravityAffectorC2ERKNS_4core8vector3dIfEEj
// 0x6fab1c _ZN6glitch5scene24CParticleGravityAffectorC1ERKNS_4core8vector3dIfEEj
CParticleGravityAffector::CParticleGravityAffector(const core::vector3d<float>& gravityValue,
                                                   std::uint32_t timeForceLostValue)
    : enabled(true),
      timeForceLost(static_cast<float>(timeForceLostValue)),
      gravity(gravityValue) {}

// 0x6fac14 _ZN6glitch5scene24CParticleGravityAffector6affectEjPNS0_9SParticleEj
// Partial: the enabled and count guard is reconstructed; the per-particle
// update is not.
void CParticleGravityAffector::affect(std::uint32_t /*now*/, SParticle* /*particles*/,
                                      std::uint32_t count) {
    if (enabled && count != 0) {
        // per-particle gravity update not reconstructed
    }
}

// 0x6fa9fc _ZN6glitch5scene24CParticleGravityAffector16setTimeForceLostEf
void CParticleGravityAffector::setTimeForceLost(float value) { timeForceLost = value; }

// 0x6faa04 _ZN6glitch5scene24CParticleGravityAffector10setGravityERKNS_4core8vector3dIfEE
void CParticleGravityAffector::setGravity(const core::vector3d<float>& value) { gravity = value; }

// 0x6faa20 _ZNK6glitch5scene24CParticleGravityAffector16getTimeForceLostEv
float CParticleGravityAffector::getTimeForceLost() const { return timeForceLost; }

// 0x6faa28 _ZNK6glitch5scene24CParticleGravityAffector10getGravityEv
const core::vector3d<float>& CParticleGravityAffector::getGravity() const { return gravity; }

// ---- CParticleRotationAffector ----

// 0x6fcd74 _ZN6glitch5scene25CParticleRotationAffectorC2ERKNS_4core8vector3dIfEES6_
// 0x6fce34 _ZN6glitch5scene25CParticleRotationAffectorC1ERKNS_4core8vector3dIfEES6_
CParticleRotationAffector::CParticleRotationAffector(const core::vector3d<float>& pivotPointValue,
                                                     const core::vector3d<float>& speedValue)
    : enabled(true), pivotPoint(pivotPointValue), speed(speedValue), lastTime(0) {}

// 0x6fcf6c _ZN6glitch5scene25CParticleRotationAffector6affectEjPNS0_9SParticleEj
// Partial: the time step from the previous call is computed and lastTime is
// stored. The per-particle rotation is not reconstructed.
void CParticleRotationAffector::affect(std::uint32_t now, SParticle* /*particles*/,
                                       std::uint32_t /*count*/) {
    if (lastTime != 0) {
        [[maybe_unused]] const float timeStep = static_cast<float>(now - lastTime) / 1000.0f;
    }
    lastTime = now;
}

// 0x6fcd08 _ZN6glitch5scene25CParticleRotationAffector13setPivotPointERKNS_4core8vector3dIfEE
void CParticleRotationAffector::setPivotPoint(const core::vector3d<float>& value) {
    pivotPoint = value;
}

// 0x6fcd24 _ZN6glitch5scene25CParticleRotationAffector8setSpeedERKNS_4core8vector3dIfEE
void CParticleRotationAffector::setSpeed(const core::vector3d<float>& value) { speed = value; }

// 0x6fcd40 _ZNK6glitch5scene25CParticleRotationAffector13getPivotPointEv
const core::vector3d<float>& CParticleRotationAffector::getPivotPoint() const { return pivotPoint; }

// 0x6fcd48 _ZNK6glitch5scene25CParticleRotationAffector8getSpeedEv
const core::vector3d<float>& CParticleRotationAffector::getSpeed() const { return speed; }

// ---- CParticleSizeAffector ----

// 0x6fd250 _ZN6glitch5scene21CParticleSizeAffectorC2Effjj
// 0x6fd310 _ZN6glitch5scene21CParticleSizeAffectorC1Effjj
CParticleSizeAffector::CParticleSizeAffector(float targetSizeValue, float variationValue,
                                             std::uint32_t growForTimeValue,
                                             std::uint32_t fadeForTimeValue)
    : enabled(true),
      targetSize(targetSizeValue),
      variation(variationValue),
      growForTime(static_cast<float>(growForTimeValue)),
      fadeForTime(static_cast<float>(fadeForTimeValue)) {}

// 0x6fd63c _ZN6glitch5scene21CParticleSizeAffector6affectEjPNS0_9SParticleEj
// Partial: the enabled and count guard is reconstructed; the per-particle
// size update is not.
void CParticleSizeAffector::affect(std::uint32_t /*now*/, SParticle* /*particles*/,
                                   std::uint32_t count) {
    if (enabled && count != 0) {
        // per-particle size update not reconstructed
    }
}

// 0x6fd1ec _ZN6glitch5scene21CParticleSizeAffector13setTargetSizeEf
void CParticleSizeAffector::setTargetSize(float value) { targetSize = value; }

// 0x6fd1f4 _ZN6glitch5scene21CParticleSizeAffector12setVariationEf
void CParticleSizeAffector::setVariation(float value) { variation = value; }

// 0x6fd1fc _ZN6glitch5scene21CParticleSizeAffector14setGrowForTimeEf
void CParticleSizeAffector::setGrowForTime(float value) { growForTime = value; }

// 0x6fd204 _ZN6glitch5scene21CParticleSizeAffector14setFadeForTimeEf
void CParticleSizeAffector::setFadeForTime(float value) { fadeForTime = value; }

// 0x6fd20c _ZNK6glitch5scene21CParticleSizeAffector13getTargetSizeEv
float CParticleSizeAffector::getTargetSize() const { return targetSize; }

// 0x6fd214 _ZNK6glitch5scene21CParticleSizeAffector12getVariationEv
float CParticleSizeAffector::getVariation() const { return variation; }

// 0x6fd21c _ZNK6glitch5scene21CParticleSizeAffector14getGrowForTimeEv
float CParticleSizeAffector::getGrowForTime() const { return growForTime; }

// 0x6fd224 _ZNK6glitch5scene21CParticleSizeAffector14getFadeForTimeEv
float CParticleSizeAffector::getFadeForTime() const { return fadeForTime; }

// ---- CParticleSpinAffector ----

namespace {
// libgcc __fixunssfsi as the original calls it: negative inputs give 0 and
// inputs at or above 2^32 give the largest unsigned value.
std::uint32_t fixUnsignedFromFloat(float value) {
    if (!(value > 0.0f)) {
        return 0;
    }
    if (value >= 4294967296.0f) {
        return 0xffffffffu;
    }
    return static_cast<std::uint32_t>(value);
}
}  // namespace

// 0x6fe310 _ZN6glitch5scene21CParticleSpinAffectorC2Ejf
// 0x6fe3bc _ZN6glitch5scene21CParticleSpinAffectorC1Ejf
CParticleSpinAffector::CParticleSpinAffector(std::uint32_t spinTimeValue, float variationValue)
    : enabled(true), spinTime(spinTimeValue), variation(variationValue) {}

// 0x6fe600 _ZN6glitch5scene21CParticleSpinAffector6affectEjPNS0_9SParticleEj
// Partial: the enabled and count guard is reconstructed; the per-particle spin
// is not.
void CParticleSpinAffector::affect(std::uint32_t /*now*/, SParticle* /*particles*/,
                                   std::uint32_t count) {
    if (enabled && count != 0) {
        // per-particle spin update not reconstructed
    }
}

// 0x6fe2b4 _ZN6glitch5scene21CParticleSpinAffector11setSpinTimeEf
void CParticleSpinAffector::setSpinTime(float value) { spinTime = fixUnsignedFromFloat(value); }

// 0x6fe2cc _ZN6glitch5scene21CParticleSpinAffector12setVariationEf
void CParticleSpinAffector::setVariation(float value) { variation = value; }

// 0x6fe2d4 _ZNK6glitch5scene21CParticleSpinAffector11getSpinTimeEv
float CParticleSpinAffector::getSpinTime() const { return static_cast<float>(spinTime); }

// 0x6fe2e4 _ZNK6glitch5scene21CParticleSpinAffector12getVariationEv
float CParticleSpinAffector::getVariation() const { return variation; }

// ---- CParticleFadeOutAffector ----

// 0x6fa4d4 _ZN6glitch5scene24CParticleFadeOutAffectorC2ERKNS_5video6SColorEj
// 0x6fa5a4 _ZN6glitch5scene24CParticleFadeOutAffectorC1ERKNS_5video6SColorEj
CParticleFadeOutAffector::CParticleFadeOutAffector(const video::SColor& targetColorValue,
                                                   std::uint32_t fadeOutTimeValue)
    : enabled(true),
      targetColor(targetColorValue),
      fadeOutTime(static_cast<float>(fadeOutTimeValue)) {}

// 0x6fa69c _ZN6glitch5scene24CParticleFadeOutAffector6affectEjPNS0_9SParticleEj
// Partial: the enabled and count guard is reconstructed; the per-particle fade
// is not.
void CParticleFadeOutAffector::affect(std::uint32_t /*now*/, SParticle* /*particles*/,
                                      std::uint32_t count) {
    if (enabled && count != 0) {
        // per-particle fade not reconstructed
    }
}

// 0x6fa484 _ZN6glitch5scene24CParticleFadeOutAffector14setTargetColorERKNS_5video6SColorE
void CParticleFadeOutAffector::setTargetColor(const video::SColor& value) { targetColor = value; }

// 0x6fa498 _ZN6glitch5scene24CParticleFadeOutAffector14setFadeOutTimeEf
void CParticleFadeOutAffector::setFadeOutTime(float value) { fadeOutTime = value; }

// 0x6fa4a0 _ZNK6glitch5scene24CParticleFadeOutAffector14getTargetColorEv
const video::SColor& CParticleFadeOutAffector::getTargetColor() const { return targetColor; }

// 0x6fa4a8 _ZNK6glitch5scene24CParticleFadeOutAffector14getFadeOutTimeEv
float CParticleFadeOutAffector::getFadeOutTime() const { return fadeOutTime; }

}  // namespace glitch::scene
