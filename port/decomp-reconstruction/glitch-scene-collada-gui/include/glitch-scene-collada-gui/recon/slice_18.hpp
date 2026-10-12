// Declarations for decompiled-function reconstruction slice 18.
// Stand-in types only: the real layouts live in other slices and headers.
// Member offsets that are read are kept as comments (// +0x..).
// Functions marked partial in the ledger are declared here and stubbed in slice_18.cpp.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace glitch::collada::recon_18 {

// Incomplete stand-ins for types owned by other slices.
struct CColladaDatabase;
struct CColladaFactory;
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
struct CParticleSystemSceneNode;
struct CMaterialRendererManager;
struct IParticleContext;
struct IParticleSystem;
struct PSRandom;
struct PForcesModel;
struct PForce;
struct PDeflector;

// Particle record sizes: GNPSParticle = 39 floats (156 bytes), SParticle = 25 floats (100 bytes).
inline constexpr std::size_t kGNPSParticleFloats = 39;
inline constexpr std::size_t kSParticleFloats = 25;

// Random source used by the force code (stand-in for lrand48). Returns a non-negative 31-bit integer.
using NextInt31Fn = std::int32_t (*)();

// ---- group 1: glitch::collada factory and effect helpers -------------------
CMesh* CColladaFactory_createGeometry(const CColladaDatabase& db, IVideoDriver* driver, SGeometry* geometry);
void SEffectList_ctor(SEffectList* self, const CColladaDatabase& db, SEffect* effect);
std::string CColladaFactory_getEffectName(const CColladaDatabase& db, const char* a, const char* name);
CLightSceneNode* CColladaFactory_createLight(const CColladaDatabase& db, SLight* light);
CSceneNodeAnimator* CColladaFactory_createAnimator(const CColladaDatabase& db, SLibraryAnimationClips* clips);
bool setMaterialParameter_SNewParam(CMaterialRenderer* renderer, std::uint16_t parameter, SNewParam& value,
                                    CRootSceneNode* root);
void CColladaFactory_createMaterialRenderer_profile(CMaterialRenderer** out, const CColladaDatabase& db,
                                                    IVideoDriver* driver, SEffect* effect, const char* a,
                                                    const char* b, CRootSceneNode* root);
CMaterialRenderer** createMaterialRenderer_flags(CMaterialRenderer** out, IVideoDriver* driver, SEffectList* effects,
                                                 CRootSceneNode* root);
CMaterialRenderer** CColladaFactory_createMaterialRenderer(CMaterialRenderer** out, const CColladaDatabase& db,
                                                           IVideoDriver* driver, CMaterialRendererManager* manager,
                                                           SEffect* effect, const char* name, CRootSceneNode* root);

// ---- group 2: glitch::ps force models (wind, gravity, deflector) -----------
// Force-owner view used by PWind and PGravity.
struct ForceOwner {
    std::uint8_t pad_0x00[32];   // +0x00..+0x1f not modelled
    float direction[3];          // +0x20 (32)
    std::uint8_t pad_0x2c[4];    // +0x2c
    float origin[3];             // +0x30 (48)
    std::uint8_t pad_0x3c[4];    // +0x3c
    std::uint8_t normalized;     // +0x40 (64)
};

// PWind force record (float** target in the pseudocode).
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

// PWind::apply<T>. Particle position at [0..2]; StrideFloats is the particle record size.
template <std::size_t StrideFloats>
void applyWind(WindForce* force, float* first, float* last, float timeStep, NextInt31Fn nextInt31);

// PGravity::apply<T>. Particle velocity at [3..5].
template <std::size_t StrideFloats>
void applyGravity(GravityForce* force, float* first, float* last, float timeStep);

// PForceProxy<T,PWind>::apply and PForceProxy<T,PGravity>::apply forward to the force apply.
template <std::size_t StrideFloats>
void PForceProxy_applyWind(WindForce* force, float* first, float* last, float timeStep, NextInt31Fn nextInt31);
template <std::size_t StrideFloats>
void PForceProxy_applyGravity(GravityForce* force, float* first, float* last, float timeStep);

void PDeflector_apply_S(void* self, float* first, float* last, float* timeStep);
void PDeflector_apply_GNP(void* self, float* first, float* last, float* timeStep);
void PDeflector_ctor(PDeflector* self, const void* parameters);
void PForceProxy_PDeflector_apply_S(void* self, float* first, float* last, float* timeStep);
void PForceProxy_PDeflector_apply_GNP(void* self, float* first, float* last, float* timeStep);
void IParticleSystem_bindForce_PDeflector_GNP(void* self, const void* parameters);
void IParticleSystem_bindForce_PDeflector_S(void* self, const void* parameters);
void CDeflectorForceSceneNode_bind_GNP(void* self, CGlitchNewParticleSystemSceneNode* node);
void CDeflectorForceSceneNode_bind_S(void* self, CParticleSystemSceneNode* node);
void SProfileGLES2Traits_createShader(void* manager, const char* name, char** parameters);
CParticleSystemSceneNode* CColladaFactory_createParticleSystem(const CColladaDatabase& db, IVideoDriver* driver,
                                                               SEmitter* emitter, void* strings,
                                                               CRootSceneNode* root);

// ---- group 3: PSRandom / PSManager / GNPS particle records -----------------
void PSRandom_RandVec(PSRandom* self, PSRandom* source);
void PSManager_dtor();
std::size_t CGlitchNewParticleSystemSceneNode_getParticleCount(const CGlitchNewParticleSystemSceneNode* self);
void IParticleSystem_init(void* self);
void GNPSParticle_copy(float* dst, const float* src);
void GNPSParticle_assign(float* dst, const float* src);
void* IParticleSystem_getPRand(void* self);
void GNPSSizeModel_initPSize(const float* model, float* first, float* last, PSRandom* random);
void GNPSLifeModel_initPLife(const float* model, float* first, float* last, PSRandom* random);
void GNPSColorModel_initPColor(const float* model, float* first, float* last);
void PEmitterModel_initPPosition(void* self, float* first, float* last);
void PRenderDataBillboardModel_initPRenderData(void* self, float* first, float* last);
std::uint32_t IParticleContext_hashString(const char* text);
void* PSManager_getInstance();

// ---- group 4: GNPS particle models, render data and destructors -------------
void PSGenericPositionBaker_convertVertexPosition(const float* position, std::uint32_t index, std::uint32_t count,
                                                  void* buffer);
void GNPSLifeModel_applyPLife(void* self, float* first, float* last);
void PRenderDataBillboardModel_applyPRenderData(void* self, char* first, char* last);
void PForcesModel_applyPForces(void* self, float* first, float* last);
void PForcesModel_initPForcesModel(void* self);
float CGlitchNewParticleSystemSceneNode_getBoundingBox_placeholder();
void IParticleContext_ctor(void* self);
void CGlitchNewParticleSystemSceneNode_onRegisterSceneNode(CGlitchNewParticleSystemSceneNode* self);
void PRenderDataBillboardModel_deallocateBuffer(void* self);
void PRenderDataBillboardModel_setRenderBuffer(void* self, void* buffer, std::uint32_t size);
void PSBillboardPositionBaker_getPerParticleSystemPosition(const void* context, const float* matrix);
void GNPSMotionModel_applyPMotion(void* self, float* first, float* last);
void GNPSSizeModel_applyPSize(void* self, float* first, float* last);
void PEmitterModel_initPEmitterModel(void* self);
void GNPSColorModel_applyPColor(void* self, float* first, float* last, void* color);
void PSGenericPositionBaker_getPerParticlePosition(const void* context, const float* particle);
void CGlitchNewParticleSystemSceneNode_ctor_db(void* self, const CColladaDatabase& db, SGNPSEmitter& emitter,
                                               void* strings, CRootSceneNode* root);
void CGlitchNewParticleSystemSceneNode_ctor_copy(void* self, const void* vtable, const void* emitter,
                                                 CRootSceneNode* root);
void GNPSSpinModel_initPSpin(void* self, float* first, float* last);
void GNPSSpinModel_applyPSpin(void* self, float* first, float* last);
void PForcesModel_removePForce_ptr(void* self, PForce* force);
void CGlitchNewParticleSystemSceneNode_attach(void* self, void* node);
void PForcesModel_removePForce_index(void* self, int index);

}  // namespace glitch::collada::recon_18
