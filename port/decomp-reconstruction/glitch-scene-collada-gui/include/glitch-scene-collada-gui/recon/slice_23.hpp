// Slice 23 reconstruction: SAnimationAccessor, the COLLADA skin techniques
// (matrix, quaternion, texture, software) and the glitch::ps shape domains.
// Free functions only. Nothing here edits an existing header. Types whose layout is
// not in a layout header are forward declared as stand-ins in this namespace and are
// never dereferenced. Each function keeps the pseudocode signature shape.
#pragma once

#include <cmath>
#include <cstdint>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/ps_types.hpp"

namespace glitch::collada::recon_23 {

// Stand-ins for types without a layout in any layout header. Incomplete on purpose.
struct SkinStandIn;
struct SkinBufferStandIn;
struct SkinCacheStandIn;
struct MeshBufferStandIn;
struct VideoDriverStandIn;
struct STechniqueStandIn;
struct PDomainStandIn;
template <typename T>
struct ResVectorStandIn;

// glitch::collada::SAnimationAccessor: canonical class in collada/SAnimationAccessor.hpp (src/collada/SAnimationAccessor.cpp).

// ---------------------------------------------------------------------------
// glitch::collada::detail skin techniques. Each family keeps its own opaque
// technique stand-in. Constructors C1 and C2 share one body.
// ---------------------------------------------------------------------------
namespace matrix_skin {
struct Technique;
void construct(Technique& self, SkinStandIn& skin, SkinCacheStandIn& cache, bool flag);
void destroy(Technique& self);  // D1
bool checkAvailabilityStatic(const STechniqueStandIn& technique);
bool checkAvailability(const Technique& self, const STechniqueStandIn& technique);
void skin(Technique& self, SkinBufferStandIn& buffer, MeshBufferStandIn* mesh);
void preparePtrCache(Technique& self);
int init(Technique& self, SkinBufferStandIn& buffer, MeshBufferStandIn* mesh,
         VideoDriverStandIn* driver, bool flag);
void prepareCache(Technique& self);
}  // namespace matrix_skin

namespace quat_skin {
struct Technique;
void construct(Technique& self, SkinStandIn& skin, SkinCacheStandIn& cache, bool flag);
void destroy(Technique& self);  // D1
bool checkAvailabilityStatic(const STechniqueStandIn& technique);
bool checkAvailability(const Technique& self, const STechniqueStandIn& technique);
void skin(Technique& self, SkinBufferStandIn& buffer, MeshBufferStandIn* mesh);
void preparePtrCache(Technique& self);
int init(Technique& self, SkinBufferStandIn& buffer, MeshBufferStandIn* mesh,
         VideoDriverStandIn* driver, bool flag);
void prepareCache(Technique& self);
}  // namespace quat_skin

namespace texture_skin {
struct Technique;
void construct(Technique& self, SkinStandIn& skin, SkinCacheStandIn& cache, bool flag);
void destroy(Technique& self);  // D1
bool checkAvailabilityStatic(const STechniqueStandIn& technique);
bool checkAvailability(const Technique& self, const STechniqueStandIn& technique);
void skin(Technique& self, SkinBufferStandIn& buffer, MeshBufferStandIn* mesh);
void preparePtrCache(Technique& self);
int init(Technique& self, SkinBufferStandIn& buffer, MeshBufferStandIn* mesh,
         VideoDriverStandIn* driver, bool flag);
void prepareCache(Technique& self);
}  // namespace texture_skin

namespace software_skin {
struct Technique;
void construct(Technique& self, SkinStandIn& skin, SkinCacheStandIn& cache, bool flag);
void destroy(Technique& self);  // D1
}  // namespace software_skin

// glitch::collada::detail::IColladaSkinTechnique base destructor (D1 and D2 share a body).
namespace skin_technique {
struct Interface;
void destroyBase(Interface& self);
}  // namespace skin_technique

// ---------------------------------------------------------------------------
// glitch::ps shape domains and PSRandom helpers.
// ---------------------------------------------------------------------------
namespace ps {

using glitch::ps::PDBlob;
using glitch::ps::PDBox;
using glitch::ps::PDCone;
using glitch::ps::PDCylinder;
using glitch::ps::PDLine;
using glitch::ps::PDPoint;
using glitch::ps::PDSphere;
using glitch::ps::PSRandom;

// PSRandom::NRandf core. draw() returns one PSRandom::Rand value. Rejection sampling as
// in the original: a pair (x, y) in [-1, 1) is redrawn while x^2 + y^2 is above 1 or
// exactly 0. The sample is x * sqrt(-2 ln s / s) * scale with s = x^2 + y^2.
template <typename Draw>
float normalSample(Draw&& draw, float scale) {
    float x = 0.0f;
    float y = 0.0f;
    float s = 0.0f;
    do {
        do {
            x = 2.0f * draw() - 1.0f;
            y = 2.0f * draw() - 1.0f;
            s = x * x + y * y;
        } while (s > 1.0f);
    } while (s == 0.0f);
    const float factor = std::sqrt(std::log(s) * -2.0f / s);
    return x * factor * scale;
}

// PSRandom::NRandVec core. The first pair gives x and y with the same factor; z is a fresh
// NRandf draw. Each Rand draw is taken in the original order.
template <typename Draw>
core::vector3d<float> normalVector(Draw&& draw, float scale) {
    float x = 0.0f;
    float y = 0.0f;
    float s = 0.0f;
    do {
        do {
            x = 2.0f * draw() - 1.0f;
            y = 2.0f * draw() - 1.0f;
            s = x * x + y * y;
        } while (s > 1.0f);
    } while (s == 0.0f);
    const float factor = std::sqrt(std::log(s) * -2.0f / s);
    core::vector3d<float> out;
    out.X = x * factor * scale;
    out.Y = y * factor * scale;
    out.Z = normalSample(draw, scale);
    return out;
}

void domainDtor(PDomainStandIn& self);                                    // PDomain::~PDomain
core::vector3d<float> pointTransform(core::vector3d<float>& point,
                                     const core::CMatrix4<float>& matrix);  // PDPoint::transform
core::vector3d<float> lineGenerate(const PDLine& self, PSRandom& random);  // PDLine::generate
void boxConstructVec(PDBox& self, const core::vector3d<float>& min,
                     const core::vector3d<float>& max);                    // PDBox ctor (C1 and C2)
void boxConstructFloat(PDBox& self, float x, float y, float z);           // PDBox ctor (C1 and C2)
bool boxWithin(const PDBox& self, const core::vector3d<float>& value);
core::vector3d<float> boxGenerate(const PDBox& self, PSRandom& random);
bool cylinderWithin(const PDCylinder& self, const core::vector3d<float>& value);
bool coneWithin(const PDCone& self, const core::vector3d<float>& value);
void sphereConstruct(PDSphere& self, const core::vector3d<float>& centre, float radius,
                     float extra);                                        // PDSphere ctor (C1 and C2)
bool sphereWithin(const PDSphere& self, const core::vector3d<float>& value);
void sphereTransform(PDSphere& self, const core::CMatrix4<float>& matrix);
void blobConstruct(PDBlob& self, const core::vector3d<float>& centre, float radius);  // PDBlob ctor
void blobTransform(PDBlob& self, const core::CMatrix4<float>& matrix);
PDomainStandIn* blobCopy(const PDBlob& self);
PDomainStandIn* sphereCopy(const PDSphere& self);
PDomainStandIn* coneCopy(const PDCone& self);
PDomainStandIn* cylinderCopy(const PDCylinder& self);
PDomainStandIn* boxCopy(const PDBox& self);
PDomainStandIn* lineCopy(const PDLine& self);
PDomainStandIn* pointCopy(const PDPoint& self);
float randNormalf(PSRandom& random, float scale);                          // PSRandom::NRandf
core::vector3d<float> randNormalVec(PSRandom& random, float scale);        // PSRandom::NRandVec
core::vector3d<float> blobGenerate(const PDBlob& self, PSRandom& random);
core::vector3d<float> sphereGenerate(const PDSphere& self, PSRandom& random);
core::vector3d<float> coneGenerate(const PDCone& self, PSRandom& random);
core::vector3d<float> cylinderGenerate(const PDCylinder& self, PSRandom& random);
bool blobWithin(const PDBlob& self, const core::vector3d<float>& value);
bool lineWithin(const PDLine& self, const core::vector3d<float>& value);
void lineConstruct(PDLine& self, const core::vector3d<float>& start,
                   const core::vector3d<float>& end);                      // PDLine ctor (C1 and C2)
void coneConstructVec(PDCone& self, const core::vector3d<float>& a,
                      const core::vector3d<float>& b, float r0, float r1);  // PDCone ctor (C1 and C2)
void cylinderConstructFloat(PDCylinder& self, float a, float b);            // PDCylinder ctor (C1 and C2)
void cylinderConstructVec(PDCylinder& self, const core::vector3d<float>& a,
                          const core::vector3d<float>& b, float r0, float r1);  // PDCylinder ctor
void cylinderTransform(PDCylinder& self, const core::CMatrix4<float>& matrix);
void lineTransform(PDLine& self, const core::CMatrix4<float>& matrix);
void boxTransform(PDBox& self, const core::CMatrix4<float>& matrix);

}  // namespace ps

// glitch::gui::ICursorControl base destructor (empty body).
namespace gui_cursor {
struct ControlStandIn;
void destroyBase(ControlStandIn& self);
}  // namespace gui_cursor

}  // namespace glitch::collada::recon_23
