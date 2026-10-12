// glitch::scene particle emitters: cylinder, point, box and ring. Each emitter
// keeps its settings in typed members at the original offsets (kept as
// comments). The original classes derive from an interface base whose vtable
// and virtual-base stores are not reconstructed, so the methods are plain
// members here, and the constructors do not write the vtable or virtual-base
// pointers.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"

namespace glitch::scene {

// Particle record. Its layout is not mapped yet; emitt() only passes pointers.
struct SParticle;
class IMesh;
class IAnimatedMeshSceneNode;

// The particle array at +0x04 of every emitter, as begin, end and capacity
// pointers. The destructor releases the storage.
struct SParticleBuffer {
    SParticle* begin = nullptr;        // +0x04
    SParticle* end = nullptr;          // +0x08
    SParticle* capacityEnd = nullptr;  // +0x0c
};

// Per-mesh-buffer int array of the mesh emitter, same begin/end/capacity shape.
struct IntBuffer {
    std::int32_t* begin = nullptr;
    std::int32_t* end = nullptr;
    std::int32_t* capacityEnd = nullptr;
};

class CParticleCylinderEmitter {
public:
    // 0x6f9b38 _ZN6glitch5scene24CParticleCylinderEmitterC2ERKNS_4core8vector3dIfEEfS6_fbS6_jjRKNS_5video6SColorESA_jji
    // 0x6f9c88 _ZN6glitch5scene24CParticleCylinderEmitterC1ERKNS_4core8vector3dIfEEfS6_fbS6_jjRKNS_5video6SColorESA_jji
    // Constructor (C2 and C1). Members are copied from the arguments; the
    // time and emitted counters start at zero. The original's virtual-base
    // setup is not reproduced.
    CParticleCylinderEmitter(const core::vector3d<float>& center, float radius,
                             const core::vector3d<float>& normal, float length, bool outlineOnly,
                             const core::vector3d<float>& direction,
                             std::uint32_t minParticlesPerSecond, std::uint32_t maxParticlesPerSecond,
                             video::SColor minStartColor, video::SColor maxStartColor,
                             std::uint32_t lifeTimeMin, std::uint32_t lifeTimeMax,
                             std::int32_t maxAngleDegrees);
    CParticleCylinderEmitter(const CParticleCylinderEmitter&) = delete;
    CParticleCylinderEmitter& operator=(const CParticleCylinderEmitter&) = delete;
    // 0x6f9e28 _ZN6glitch5scene24CParticleCylinderEmitterD1Ev
    // (D1). Releases the particle array.
    ~CParticleCylinderEmitter();

    // 0x6f9ecc _ZN6glitch5scene24CParticleCylinderEmitter5emittEjjRPNS0_9SParticleE
    // Partial: the time accumulation is reconstructed. The
    // particle-count and array-growth code (about 1340 bytes of ARM) is not,
    // so no particles are produced and the result is 0.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6f9a08 _ZN6glitch5scene24CParticleCylinderEmitter9setCenterERKNS_4core8vector3dIfEE
    void setCenter(const core::vector3d<float>& value);
    // 0x6f9a24 _ZN6glitch5scene24CParticleCylinderEmitter9setNormalERKNS_4core8vector3dIfEE
    void setNormal(const core::vector3d<float>& value);
    // 0x6f9a40 _ZN6glitch5scene24CParticleCylinderEmitter9setRadiusEf
    void setRadius(float value);
    // 0x6f9a48 _ZN6glitch5scene24CParticleCylinderEmitter9setLengthEf
    void setLength(float value);
    // 0x6f9a50 _ZN6glitch5scene24CParticleCylinderEmitter14setOutlineOnlyEb
    void setOutlineOnly(bool value);
    // 0x6f9a58 _ZN6glitch5scene24CParticleCylinderEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6f9a74 _ZN6glitch5scene24CParticleCylinderEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6f9a7c _ZN6glitch5scene24CParticleCylinderEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6f9a84 _ZN6glitch5scene24CParticleCylinderEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6f9a98 _ZN6glitch5scene24CParticleCylinderEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);

    // 0x6f9aac _ZNK6glitch5scene24CParticleCylinderEmitter9getCenterEv
    const core::vector3d<float>& getCenter() const;
    // 0x6f9ab4 _ZNK6glitch5scene24CParticleCylinderEmitter9getNormalEv
    const core::vector3d<float>& getNormal() const;
    // 0x6f9abc _ZNK6glitch5scene24CParticleCylinderEmitter9getRadiusEv
    float getRadius() const;
    // 0x6f9ac4 _ZNK6glitch5scene24CParticleCylinderEmitter9getLengthEv
    float getLength() const;
    // 0x6f9acc _ZNK6glitch5scene24CParticleCylinderEmitter14getOutlineOnlyEv
    bool getOutlineOnly() const;
    // 0x6f9ad4 _ZNK6glitch5scene24CParticleCylinderEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6f9adc _ZNK6glitch5scene24CParticleCylinderEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6f9ae4 _ZNK6glitch5scene24CParticleCylinderEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6f9aec _ZNK6glitch5scene24CParticleCylinderEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6f9af4 _ZNK6glitch5scene24CParticleCylinderEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6f9afc _ZNK6glitch5scene24CParticleCylinderEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6f9b04 _ZNK6glitch5scene24CParticleCylinderEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6f9b0c _ZNK6glitch5scene24CParticleCylinderEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    SParticleBuffer particles;                       // +0x04
    core::vector3d<float> center;                    // +0x10
    core::vector3d<float> normal;                    // +0x1c
    float radius = 0.0f;                             // +0x28
    float length = 0.0f;                             // +0x2c
    bool outlineOnly = false;                        // +0x30
    core::vector3d<float> direction;                 // +0x34
    std::uint32_t minParticlesPerSecond = 0;        // +0x40
    std::uint32_t maxParticlesPerSecond = 0;        // +0x44
    video::SColor minStartColor;                     // +0x48
    video::SColor maxStartColor;                     // +0x4c
    std::uint32_t minLifeTime = 0;                   // +0x50
    std::uint32_t maxLifeTime = 0;                   // +0x54
    std::uint32_t time = 0;                          // +0x58
    std::uint32_t emitted = 0;                       // +0x5c
    std::int32_t maxAngleDegrees = 0;                // +0x60
};

class CParticlePointEmitter {
public:
    // 0x6fba68 _ZN6glitch5scene21CParticlePointEmitterC2ERKNS_4core8vector3dIfEEjjNS_5video6SColorES8_jji
    // 0x6fbb80 _ZN6glitch5scene21CParticlePointEmitterC1ERKNS_4core8vector3dIfEEjjNS_5video6SColorES8_jji
    // Constructor (C2 and C1). The vector argument is the emission direction;
    // the point emitter has no centre.
    CParticlePointEmitter(const core::vector3d<float>& direction, std::uint32_t minParticlesPerSecond,
                          std::uint32_t maxParticlesPerSecond, video::SColor minStartColor,
                          video::SColor maxStartColor, std::uint32_t lifeTimeMin,
                          std::uint32_t lifeTimeMax, std::int32_t maxAngleDegrees);
    CParticlePointEmitter(const CParticlePointEmitter&) = delete;
    CParticlePointEmitter& operator=(const CParticlePointEmitter&) = delete;
    // 0x6fbe08 _ZN6glitch5scene21CParticlePointEmitterD1Ev
    // (D1). Empty in the original.
    ~CParticlePointEmitter() = default;

    // 0x6fc0b4 _ZN6glitch5scene21CParticlePointEmitter5emittEjjRPNS0_9SParticleE
    // Partial: the time accumulation is reconstructed. The particle
    // generation is not, so the result is 0.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6fb9d4 _ZN6glitch5scene21CParticlePointEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6fb9f0 _ZN6glitch5scene21CParticlePointEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6fb9f8 _ZN6glitch5scene21CParticlePointEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6fba00 _ZN6glitch5scene21CParticlePointEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6fba14 _ZN6glitch5scene21CParticlePointEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);

    // 0x6fba28 _ZNK6glitch5scene21CParticlePointEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6fba30 _ZNK6glitch5scene21CParticlePointEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6fba38 _ZNK6glitch5scene21CParticlePointEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6fba40 _ZNK6glitch5scene21CParticlePointEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6fba48 _ZNK6glitch5scene21CParticlePointEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6fba50 _ZNK6glitch5scene21CParticlePointEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6fba58 _ZNK6glitch5scene21CParticlePointEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6fba60 _ZNK6glitch5scene21CParticlePointEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    SParticleBuffer particles;                       // +0x04
    // +0x10..+0x47: state the constructor zeroes and no accessor reads yet.
    std::uint8_t unmapped[0x38] = {};
    core::vector3d<float> direction;                 // +0x48
    std::uint32_t minParticlesPerSecond = 0;        // +0x54
    std::uint32_t maxParticlesPerSecond = 0;        // +0x58
    video::SColor minStartColor;                     // +0x5c
    video::SColor maxStartColor;                     // +0x60
    std::uint32_t minLifeTime = 0;                   // +0x64
    std::uint32_t maxLifeTime = 0;                   // +0x68
    std::int32_t maxAngleDegrees = 0;                // +0x6c
    std::uint32_t time = 0;                          // +0x70
    std::uint32_t emitted = 0;                       // +0x74
};

class CParticleBoxEmitter {
public:
    // 0x6f8a48 _ZN6glitch5scene19CParticleBoxEmitterC2ERKNS_4core8aabbox3dIfEERKNS2_8vector3dIfEEjjNS_5video6SColorESC_jji
    // 0x6f8bb0 _ZN6glitch5scene19CParticleBoxEmitterC1ERKNS_4core8aabbox3dIfEERKNS2_8vector3dIfEEjjNS_5video6SColorESC_jji
    // Constructor (C2 and C1). The box is copied into +0x10.
    CParticleBoxEmitter(const core::aabbox3d<float>& box, const core::vector3d<float>& direction,
                        std::uint32_t minParticlesPerSecond, std::uint32_t maxParticlesPerSecond,
                        video::SColor minStartColor, video::SColor maxStartColor,
                        std::uint32_t lifeTimeMin, std::uint32_t lifeTimeMax,
                        std::int32_t maxAngleDegrees);
    CParticleBoxEmitter(const CParticleBoxEmitter&) = delete;
    CParticleBoxEmitter& operator=(const CParticleBoxEmitter&) = delete;
    // 0x6f8f30 _ZN6glitch5scene19CParticleBoxEmitterD1Ev
    // (D1). Releases the particle array.
    ~CParticleBoxEmitter();

    // 0x6f9508 _ZN6glitch5scene19CParticleBoxEmitter5emittEjjRPNS0_9SParticleE
    // Partial: the time accumulation is reconstructed. The particle
    // generation is not, so the result is 0.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6f8954 _ZN6glitch5scene19CParticleBoxEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6f89a8 _ZN6glitch5scene19CParticleBoxEmitter6setBoxERKNS_4core8aabbox3dIfEE
    void setBox(const core::aabbox3d<float>& value);
    // 0x6f8970 _ZN6glitch5scene19CParticleBoxEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6f8978 _ZN6glitch5scene19CParticleBoxEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6f8980 _ZN6glitch5scene19CParticleBoxEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6f8994 _ZN6glitch5scene19CParticleBoxEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);

    // 0x6f89dc _ZNK6glitch5scene19CParticleBoxEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6f8a04 _ZNK6glitch5scene19CParticleBoxEmitter6getBoxEv
    const core::aabbox3d<float>& getBox() const;
    // 0x6f89e4 _ZNK6glitch5scene19CParticleBoxEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6f89ec _ZNK6glitch5scene19CParticleBoxEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6f89f4 _ZNK6glitch5scene19CParticleBoxEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6f89fc _ZNK6glitch5scene19CParticleBoxEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6f8a0c _ZNK6glitch5scene19CParticleBoxEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6f8a14 _ZNK6glitch5scene19CParticleBoxEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6f8a1c _ZNK6glitch5scene19CParticleBoxEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    SParticleBuffer particles;                       // +0x04
    core::aabbox3d<float> box;                       // +0x10
    core::vector3d<float> direction;                 // +0x28
    std::uint32_t minParticlesPerSecond = 0;        // +0x34
    std::uint32_t maxParticlesPerSecond = 0;        // +0x38
    video::SColor minStartColor;                     // +0x3c
    video::SColor maxStartColor;                     // +0x40
    std::uint32_t minLifeTime = 0;                   // +0x44
    std::uint32_t maxLifeTime = 0;                   // +0x48
    std::uint32_t time = 0;                          // +0x4c
    std::uint32_t emitted = 0;                       // +0x50
    std::int32_t maxAngleDegrees = 0;                // +0x54
};

class CParticleRingEmitter {
public:
    // 0x6fc4c8 _ZN6glitch5scene20CParticleRingEmitterC2ERKNS_4core8vector3dIfEEffS6_jjRKNS_5video6SColorESA_jji
    // 0x6fc5f4 _ZN6glitch5scene20CParticleRingEmitterC1ERKNS_4core8vector3dIfEEffS6_jjRKNS_5video6SColorESA_jji
    // Constructor (C2 and C1).
    CParticleRingEmitter(const core::vector3d<float>& center, float radius, float ringThickness,
                         const core::vector3d<float>& direction, std::uint32_t minParticlesPerSecond,
                         std::uint32_t maxParticlesPerSecond, video::SColor minStartColor,
                         video::SColor maxStartColor, std::uint32_t lifeTimeMin,
                         std::uint32_t lifeTimeMax, std::int32_t maxAngleDegrees);
    CParticleRingEmitter(const CParticleRingEmitter&) = delete;
    CParticleRingEmitter& operator=(const CParticleRingEmitter&) = delete;
    // 0x6fc770 _ZN6glitch5scene20CParticleRingEmitterD1Ev
    // (D1). Releases the particle array.
    ~CParticleRingEmitter();

    // 0x6fc814 _ZN6glitch5scene20CParticleRingEmitter5emittEjjRPNS0_9SParticleE
    // Partial: the time accumulation is reconstructed. The particle
    // generation is not, so the result is 0.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6fc3cc _ZN6glitch5scene20CParticleRingEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6fc3e8 _ZN6glitch5scene20CParticleRingEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6fc3f0 _ZN6glitch5scene20CParticleRingEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6fc3f8 _ZN6glitch5scene20CParticleRingEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6fc40c _ZN6glitch5scene20CParticleRingEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);
    // 0x6fc420 _ZN6glitch5scene20CParticleRingEmitter9setCenterERKNS_4core8vector3dIfEE
    void setCenter(const core::vector3d<float>& value);
    // 0x6fc43c _ZN6glitch5scene20CParticleRingEmitter9setRadiusEf
    void setRadius(float value);
    // 0x6fc444 _ZN6glitch5scene20CParticleRingEmitter16setRingThicknessEf
    void setRingThickness(float value);

    // 0x6fc44c _ZNK6glitch5scene20CParticleRingEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6fc454 _ZNK6glitch5scene20CParticleRingEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6fc45c _ZNK6glitch5scene20CParticleRingEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6fc464 _ZNK6glitch5scene20CParticleRingEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6fc46c _ZNK6glitch5scene20CParticleRingEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6fc474 _ZNK6glitch5scene20CParticleRingEmitter9getCenterEv
    const core::vector3d<float>& getCenter() const;
    // 0x6fc47c _ZNK6glitch5scene20CParticleRingEmitter9getRadiusEv
    float getRadius() const;
    // 0x6fc484 _ZNK6glitch5scene20CParticleRingEmitter16getRingThicknessEv
    float getRingThickness() const;
    // 0x6fc48c _ZNK6glitch5scene20CParticleRingEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6fc494 _ZNK6glitch5scene20CParticleRingEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6fc49c _ZNK6glitch5scene20CParticleRingEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    SParticleBuffer particles;                       // +0x04
    core::vector3d<float> center;                    // +0x10
    float radius = 0.0f;                             // +0x1c
    float ringThickness = 0.0f;                      // +0x20
    core::vector3d<float> direction;                 // +0x24
    std::uint32_t minParticlesPerSecond = 0;        // +0x30
    std::uint32_t maxParticlesPerSecond = 0;        // +0x34
    video::SColor minStartColor;                     // +0x38
    video::SColor maxStartColor;                     // +0x3c
    std::uint32_t minLifeTime = 0;                   // +0x40
    std::uint32_t maxLifeTime = 0;                   // +0x44
    std::uint32_t time = 0;                          // +0x48
    std::uint32_t emitted = 0;                       // +0x4c
    std::int32_t maxAngleDegrees = 0;                // +0x50
};

class CParticleSphereEmitter {
public:
    // 0x6fd9e8 _ZN6glitch5scene22CParticleSphereEmitterC2ERKNS_4core8vector3dIfEEfS6_jjRKNS_5video6SColorESA_jji
    // 0x6fdb0c _ZN6glitch5scene22CParticleSphereEmitterC1ERKNS_4core8vector3dIfEEfS6_jjRKNS_5video6SColorESA_jji
    // Constructor (C2 and C1).
    CParticleSphereEmitter(const core::vector3d<float>& center, float radius,
                           const core::vector3d<float>& direction, std::uint32_t minParticlesPerSecond,
                           std::uint32_t maxParticlesPerSecond, video::SColor minStartColor,
                           video::SColor maxStartColor, std::uint32_t lifeTimeMin,
                           std::uint32_t lifeTimeMax, std::int32_t maxAngleDegrees);
    CParticleSphereEmitter(const CParticleSphereEmitter&) = delete;
    CParticleSphereEmitter& operator=(const CParticleSphereEmitter&) = delete;
    // 0x6fdc84 _ZN6glitch5scene22CParticleSphereEmitterD1Ev
    // (D1). Releases the particle array.
    ~CParticleSphereEmitter();

    // 0x6fdd28 _ZN6glitch5scene22CParticleSphereEmitter5emittEjjRPNS0_9SParticleE
    // Partial: the time accumulation is reconstructed. The particle
    // generation is not, so the result is 0.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6fd8fc _ZN6glitch5scene22CParticleSphereEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6fd918 _ZN6glitch5scene22CParticleSphereEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6fd920 _ZN6glitch5scene22CParticleSphereEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6fd928 _ZN6glitch5scene22CParticleSphereEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6fd93c _ZN6glitch5scene22CParticleSphereEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);
    // 0x6fd950 _ZN6glitch5scene22CParticleSphereEmitter9setCenterERKNS_4core8vector3dIfEE
    void setCenter(const core::vector3d<float>& value);
    // 0x6fd96c _ZN6glitch5scene22CParticleSphereEmitter9setRadiusEf
    void setRadius(float value);

    // 0x6fd974 _ZNK6glitch5scene22CParticleSphereEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6fd97c _ZNK6glitch5scene22CParticleSphereEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6fd984 _ZNK6glitch5scene22CParticleSphereEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6fd98c _ZNK6glitch5scene22CParticleSphereEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6fd994 _ZNK6glitch5scene22CParticleSphereEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6fd99c _ZNK6glitch5scene22CParticleSphereEmitter9getCenterEv
    const core::vector3d<float>& getCenter() const;
    // 0x6fd9a4 _ZNK6glitch5scene22CParticleSphereEmitter9getRadiusEv
    float getRadius() const;
    // 0x6fd9ac _ZNK6glitch5scene22CParticleSphereEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6fd9b4 _ZNK6glitch5scene22CParticleSphereEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6fd9bc _ZNK6glitch5scene22CParticleSphereEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    SParticleBuffer particles;                       // +0x04
    core::vector3d<float> center;                    // +0x10
    float radius = 0.0f;                             // +0x1c
    core::vector3d<float> direction;                 // +0x20
    std::uint32_t minParticlesPerSecond = 0;        // +0x2c
    std::uint32_t maxParticlesPerSecond = 0;        // +0x30
    video::SColor minStartColor;                     // +0x34
    video::SColor maxStartColor;                     // +0x38
    std::uint32_t minLifeTime = 0;                   // +0x3c
    std::uint32_t maxLifeTime = 0;                   // +0x40
    std::uint32_t time = 0;                          // +0x44
    std::uint32_t emitted = 0;                       // +0x48
    std::int32_t maxAngleDegrees = 0;                // +0x4c
};

class CParticleMeshEmitter {
public:
    // 0x6fb708 _ZN6glitch5scene20CParticleMeshEmitterC1ERKN5boost13intrusive_ptrIKNS0_5IMeshEEEbRKNS_4core8vector3dIfEEfibjjRKNS_5video6SColorESH_jji
    // 0x6fb2e4 _ZN6glitch5scene20CParticleMeshEmitterC2ERKN5boost13intrusive_ptrIKNS0_5IMeshEEEbRKNS_4core8vector3dIfEEfibjjRKNS_5video6SColorESH_jji
    // Constructor (C1 and C2). The mesh is copied into +0x04. The per-buffer
    // count loop that follows the copy is not reconstructed (see setMesh).
    CParticleMeshEmitter(const boost::intrusive_ptr<const IMesh>& mesh, bool useNormalDirection,
                         const core::vector3d<float>& direction, float normalDirectionModifier,
                         std::int32_t meshBufferNumber, bool everyMeshVertex,
                         std::uint32_t minParticlesPerSecond, std::uint32_t maxParticlesPerSecond,
                         video::SColor minStartColor, video::SColor maxStartColor,
                         std::uint32_t lifeTimeMin, std::uint32_t lifeTimeMax,
                         std::int32_t maxAngleDegrees);

    // 0x6fb0fc _ZN6glitch5scene20CParticleMeshEmitterD1Ev
    // (D1). Partial: frees the per-buffer count array and the +0x28
    // buffer; the reference drop on the mesh is not modelled.
    ~CParticleMeshEmitter();

    // 0x6fb0d4 _ZN6glitch5scene20CParticleMeshEmitter5emittEjjRPNS0_9SParticleE
    // Returns 0 in the original; no particles are produced.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6fb57c _ZN6glitch5scene20CParticleMeshEmitter7setMeshERKN5boost13intrusive_ptrIKNS0_5IMeshEEE
    // Partial: stores the mesh pointer. The per-buffer loop that
    // computes the vertex totals needs the IMesh and mesh-buffer vtable slots,
    // which are not mapped, so those counts are left at zero.
    void setMesh(const boost::intrusive_ptr<const IMesh>& value);

    // 0x6fafd4 _ZN6glitch5scene20CParticleMeshEmitter21setUseNormalDirectionEb
    void setUseNormalDirection(bool value);
    // 0x6fafdc _ZN6glitch5scene20CParticleMeshEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6faff8 _ZN6glitch5scene20CParticleMeshEmitter26setNormalDirectionModifierEf
    void setNormalDirectionModifier(float value);
    // 0x6fb000 _ZN6glitch5scene20CParticleMeshEmitter18setEveryMeshVertexEb
    void setEveryMeshVertex(bool value);
    // 0x6fb008 _ZN6glitch5scene20CParticleMeshEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6fb010 _ZN6glitch5scene20CParticleMeshEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6fb018 _ZN6glitch5scene20CParticleMeshEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6fb02c _ZN6glitch5scene20CParticleMeshEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);

    // 0x6fb040 _ZNK6glitch5scene20CParticleMeshEmitter7getMeshEv
    const boost::intrusive_ptr<const IMesh>& getMesh() const;
    // 0x6fb048 _ZNK6glitch5scene20CParticleMeshEmitter22isUsingNormalDirectionEv
    bool isUsingNormalDirection() const;
    // 0x6fb050 _ZNK6glitch5scene20CParticleMeshEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6fb058 _ZNK6glitch5scene20CParticleMeshEmitter26getNormalDirectionModifierEv
    float getNormalDirectionModifier() const;
    // 0x6fb060 _ZNK6glitch5scene20CParticleMeshEmitter18getEveryMeshVertexEv
    bool getEveryMeshVertex() const;
    // 0x6fb068 _ZNK6glitch5scene20CParticleMeshEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6fb070 _ZNK6glitch5scene20CParticleMeshEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6fb078 _ZNK6glitch5scene20CParticleMeshEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6fb080 _ZNK6glitch5scene20CParticleMeshEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6fb088 _ZNK6glitch5scene20CParticleMeshEmitter21getUseNormalDirectionEv
    bool getUseNormalDirection() const;
    // 0x6fb090 _ZNK6glitch5scene20CParticleMeshEmitter11getMBNumberEv
    std::int32_t getMBNumber() const;
    // 0x6fb098 _ZNK6glitch5scene20CParticleMeshEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6fb0a0 _ZNK6glitch5scene20CParticleMeshEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6fb0a8 _ZNK6glitch5scene20CParticleMeshEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    boost::intrusive_ptr<const IMesh> mesh;          // +0x04
    std::uint32_t totalVertexCount = 0;             // +0x08
    std::uint32_t meshBufferCount = 0;              // +0x0c
    std::int32_t meshBufferNumber = 0;              // +0x10
    IntBuffer bufferVertexCounts;                    // +0x14
    bool everyMeshVertex = false;                    // +0x20
    bool useNormalDirection = false;                 // +0x21
    float normalDirectionModifier = 0.0f;            // +0x24
    void* buffer28 = nullptr;                        // +0x28 (freed by D1; role not identified)
    std::uint8_t unmapped2c[8] = {};                 // +0x2c..+0x33
    core::vector3d<float> direction;                 // +0x34
    std::uint32_t minParticlesPerSecond = 0;        // +0x40
    std::uint32_t maxParticlesPerSecond = 0;        // +0x44
    video::SColor minStartColor;                     // +0x48
    video::SColor maxStartColor;                     // +0x4c
    std::uint32_t minLifeTime = 0;                   // +0x50
    std::uint32_t maxLifeTime = 0;                   // +0x54
    std::uint32_t time = 0;                          // +0x58
    std::uint32_t emitted = 0;                       // +0x5c
    std::int32_t maxAngleDegrees = 0;                // +0x60
};

class CParticleAnimatedMeshSceneNodeEmitter {
public:
    // 0x6f7a94 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitterC2EPNS0_22IAnimatedMeshSceneNodeEbRKNS_4core8vector3dIfEEfibjjRKNS_5video6SColorESC_jji
    // 0x6f7ff0 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitterC1EPNS0_22IAnimatedMeshSceneNodeEbRKNS_4core8vector3dIfEEfibjjRKNS_5video6SColorESC_jji
    // Constructor (C1 and C2). The scene node is stored into +0x04 without the
    // grab that the original performs in setAnimatedMeshSceneNode.
    CParticleAnimatedMeshSceneNodeEmitter(IAnimatedMeshSceneNode* node, bool useNormalDirection,
                                          const core::vector3d<float>& direction,
                                          float normalDirectionModifier, std::int32_t meshBufferNumber,
                                          bool everyMeshVertex, std::uint32_t minParticlesPerSecond,
                                          std::uint32_t maxParticlesPerSecond,
                                          video::SColor minStartColor, video::SColor maxStartColor,
                                          std::uint32_t lifeTimeMin, std::uint32_t lifeTimeMax,
                                          std::int32_t maxAngleDegrees);

    // 0x6f7820 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter5emittEjjRPNS0_9SParticleE
    // Returns 0 in the original; no particles are produced.
    std::int32_t emitt(std::uint32_t now, std::uint32_t timeSinceLastCall, SParticle*& outArray);

    // 0x6f76fc _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter21setUseNormalDirectionEb
    void setUseNormalDirection(bool value);
    // 0x6f7704 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter12setDirectionERKNS_4core8vector3dIfEE
    void setDirection(const core::vector3d<float>& value);
    // 0x6f7720 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter26setNormalDirectionModifierEf
    void setNormalDirectionModifier(float value);
    // 0x6f7728 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter18setEveryMeshVertexEb
    void setEveryMeshVertex(bool value);
    // 0x6f7730 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter24setMinParticlesPerSecondEj
    void setMinParticlesPerSecond(std::uint32_t value);
    // 0x6f7738 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter24setMaxParticlesPerSecondEj
    void setMaxParticlesPerSecond(std::uint32_t value);
    // 0x6f7740 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter16setMinStartColorERKNS_5video6SColorE
    void setMinStartColor(const video::SColor& value);
    // 0x6f7754 _ZN6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter16setMaxStartColorERKNS_5video6SColorE
    void setMaxStartColor(const video::SColor& value);

    // 0x6f7768 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter24getAnimatedMeshSceneNodeEv
    IAnimatedMeshSceneNode* getAnimatedMeshSceneNode() const;
    // 0x6f7770 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter22isUsingNormalDirectionEv
    bool isUsingNormalDirection() const;
    // 0x6f7778 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter12getDirectionEv
    const core::vector3d<float>& getDirection() const;
    // 0x6f7780 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter26getNormalDirectionModifierEv
    float getNormalDirectionModifier() const;
    // 0x6f7788 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter18getEveryMeshVertexEv
    bool getEveryMeshVertex() const;
    // 0x6f7790 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter24getMinParticlesPerSecondEv
    std::uint32_t getMinParticlesPerSecond() const;
    // 0x6f7798 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter24getMaxParticlesPerSecondEv
    std::uint32_t getMaxParticlesPerSecond() const;
    // 0x6f77a0 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter16getMinStartColorEv
    const video::SColor& getMinStartColor() const;
    // 0x6f77a8 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter16getMaxStartColorEv
    const video::SColor& getMaxStartColor() const;
    // 0x6f77b0 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter21getUseNormalDirectionEv
    bool getUseNormalDirection() const;
    // 0x6f77b8 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter11getMBNumberEv
    std::int32_t getMBNumber() const;
    // 0x6f77c0 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter14getMinLifeTimeEv
    std::uint32_t getMinLifeTime() const;
    // 0x6f77c8 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter14getMaxLifeTimeEv
    std::uint32_t getMaxLifeTime() const;
    // 0x6f77d0 _ZNK6glitch5scene37CParticleAnimatedMeshSceneNodeEmitter18getMaxAngleDegreesEv
    std::int32_t getMaxAngleDegrees() const;

private:
    IAnimatedMeshSceneNode* node = nullptr;          // +0x04
    std::uint8_t unmapped08[0x10] = {};              // +0x08..+0x17
    std::int32_t meshBufferNumber = 0;              // +0x18
    std::uint8_t unmapped1c[0x0c] = {};              // +0x1c..+0x27
    bool everyMeshVertex = false;                    // +0x28
    bool useNormalDirection = false;                 // +0x29
    std::uint8_t unmapped2a[0x02] = {};              // +0x2a..+0x2b
    float normalDirectionModifier = 0.0f;            // +0x2c
    std::uint8_t unmapped30[0x0c] = {};              // +0x30..+0x3b
    core::vector3d<float> direction;                 // +0x3c
    std::uint32_t minParticlesPerSecond = 0;        // +0x48
    std::uint32_t maxParticlesPerSecond = 0;        // +0x4c
    video::SColor minStartColor;                     // +0x50
    video::SColor maxStartColor;                     // +0x54
    std::uint32_t minLifeTime = 0;                   // +0x58
    std::uint32_t maxLifeTime = 0;                   // +0x5c
    std::uint8_t unmapped60[0x08] = {};              // +0x60..+0x67
    std::int32_t maxAngleDegrees = 0;                // +0x68
};

}  // namespace glitch::scene
