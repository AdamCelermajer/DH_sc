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
struct CParticleSystemSceneNode;
struct CGlitchNewParticleSystemSceneNode;

// Particle record sizes (bytes): GNPSParticle = 39 floats, SParticle = 25 floats.
inline constexpr std::size_t kGNPSParticleFloats = 39;
inline constexpr std::size_t kSParticleFloats = 25;
inline constexpr std::size_t kGNPSParticleBytes = 156;

// Boost-style string hash used by IParticleContext::hashString.
std::uint32_t hashString(const char* text);

// glitch::collada::CColladaFactory::getEffectName
// (interface stand-in; the std::string member is built from the effect name)
struct CColladaFactory {
    static CMesh* createGeometry(const CColladaDatabase& db, IVideoDriver* driver, SGeometry* geometry);
    static CLightSceneNode* createLight(const CColladaDatabase& db, SLight* light);
    static CSceneNodeAnimator* createAnimator(const CColladaDatabase& db, SLibraryAnimationClips* clips);
};

// glitch::ps::PSRandom (stand-in for the generator used by the particle models)
struct PSRandom {
    float Rand();
    void RandVec(float out[3]);
};

// glitch::ps::PSManager singleton accessor.
struct PSManager {
    static PSManager& getInstance();
};

// glitch::ps::PWind / PGravity / PDeflector apply templates, stride in floats.
// The first float* is the force record (see slice_18.cpp for field offsets).
template <std::size_t StrideFloats>
void applyWind(float* force, float* first, float* last, float timeStep);

template <std::size_t StrideFloats>
void applyGravity(float* force, float* first, float* last, float timeStep);

template <std::size_t StrideFloats>
void applyDeflector(float* force, float* first, float* last, PSRandom* random, float timeStep);

}  // namespace glitch::collada::recon_18
