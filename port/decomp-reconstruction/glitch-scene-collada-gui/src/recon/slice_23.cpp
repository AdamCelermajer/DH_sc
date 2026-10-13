// Slice 23 reconstruction (see include/glitch-scene-collada-gui/recon/slice_23.hpp).
// Each function keeps the original address and mangled symbol in its comment.
// Bodies marked "stub" keep the signature and return a neutral value: the original
// reads guest pointers or members whose layout is not in any layout header.

#include "glitch-scene-collada-gui/recon/slice_23.hpp"

namespace glitch::collada::recon_23 {

// glitch::collada::SAnimationAccessor: canonical class in collada/SAnimationAccessor.hpp (src/collada/SAnimationAccessor.cpp).

// ---------------------------------------------------------------------------
// Skin techniques. Constructors write the vtable, the technique SSkin and SSkinCache
// references, the flag and an empty map. Their members are not in a layout header, so
// the bodies are stubs. The D1 destructors erase the per-technique map and chain to the
// interface base. Bodies that call the base map and vtable are stubs too.
// ---------------------------------------------------------------------------
namespace matrix_skin {

// 0x66b880 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechniqueC2ERNS0_5SSkinERNS0_10SSkinCacheEb
// 0x66b8dc _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb
void construct(Technique&, SkinStandIn&, SkinCacheStandIn&, bool) {}  // stub: members unmapped

// 0x66b9cc _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechniqueD1Ev
void destroy(Technique&) {}  // stub: map erase and base chain need unmapped members

// 0x66ba50 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique23checkAvailabilityStaticERKNS_5video10STechniqueE
bool checkAvailabilityStatic(const STechniqueStandIn&) {
    return false;  // stub: IShader::getParameterID and STechnique layout not mapped
}

// 0x66beac _ZNK6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
bool checkAvailability(const Technique&, const STechniqueStandIn&) {
    return false;  // stub: depends on checkAvailabilityStatic and the skin-cache flag word
}

// 0x66c278 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
void skin(Technique&, SkinBufferStandIn&, MeshBufferStandIn*) {}  // stub: material parameter upload

// 0x66c544 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique15preparePtrCacheEv
void preparePtrCache(Technique&) {}  // stub: scene-node lookup by scope id not mapped

// 0x66c8a4 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
int init(Technique&, SkinBufferStandIn&, MeshBufferStandIn*, VideoDriverStandIn*, bool) {
    return 0;  // stub: initProxyBuffer and the skin-cache flag update not mapped
}

// 0x66cd18 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique12prepareCacheEv
void prepareCache(Technique&) {}  // stub: matrix product into the 68-byte cache entries not mapped

}  // namespace matrix_skin

namespace quat_skin {

// 0x66cfc0 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechniqueC2ERNS0_5SSkinERNS0_10SSkinCacheEb
// 0x66d01c _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb
void construct(Technique&, SkinStandIn&, SkinCacheStandIn&, bool) {}  // stub: members unmapped

// 0x66df04 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechniqueD1Ev
void destroy(Technique&) {}  // stub: map erase and base chain need unmapped members

// 0x66d730 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique23checkAvailabilityStaticERKNS_5video10STechniqueE
bool checkAvailabilityStatic(const STechniqueStandIn&) {
    return false;  // stub: IShader::getParameterID pair and the parameter table not mapped
}

// 0x66d8c8 _ZNK6glitch7collada6detail33CColladaHardwareQuatSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
bool checkAvailability(const Technique&, const STechniqueStandIn&) {
    return false;  // stub: depends on checkAvailabilityStatic and the skin-cache flag word
}

// 0x66dc40 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
void skin(Technique&, SkinBufferStandIn&, MeshBufferStandIn*) {}  // stub: rb-tree parameter cache not mapped

// 0x66df88 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique15preparePtrCacheEv
void preparePtrCache(Technique&) {}  // stub: scene-node lookup by scope id not mapped

// 0x66e204 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique12prepareCacheEv
void prepareCache(Technique&) {}  // stub: dual-quaternion cache build not mapped

// 0x66e634 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
int init(Technique&, SkinBufferStandIn&, MeshBufferStandIn*, VideoDriverStandIn*, bool) {
    return 0;  // stub: initProxyBuffer and the skin-cache flag update not mapped
}

}  // namespace quat_skin

namespace texture_skin {

// 0x66e6b8 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechniqueC2ERNS0_5SSkinERNS0_10SSkinCacheEb
// 0x66e714 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb
void construct(Technique&, SkinStandIn&, SkinCacheStandIn&, bool) {}  // stub: members unmapped

// 0x66ee10 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechniqueD1Ev
void destroy(Technique&) {}  // stub: map erase and base chain need unmapped members

// 0x66e7d0 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique23checkAvailabilityStaticERKNS_5video10STechniqueE
bool checkAvailabilityStatic(const STechniqueStandIn&) {
    return false;  // stub: two IShader::getParameterID lookups not mapped
}

// 0x66e89c _ZNK6glitch7collada6detail36CColladaHardwareTextureSkinTechnique17checkAvailabilityERKNS_5video10STechniqueE
bool checkAvailability(const Technique&, const STechniqueStandIn&) {
    return false;  // stub: depends on checkAvailabilityStatic and the skin-cache flag word
}

// 0x66ec14 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
void skin(Technique&, SkinBufferStandIn&, MeshBufferStandIn*) {}  // stub: texture and parameter upload

// 0x66ee94 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique15preparePtrCacheEv
void preparePtrCache(Technique&) {}  // stub: scene-node lookup by scope id not mapped

// 0x66f1f4 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
int init(Technique&, SkinBufferStandIn&, MeshBufferStandIn*, VideoDriverStandIn*, bool) {
    return 0;  // stub: initProxyBuffer and the skin-cache flag update not mapped
}

// 0x66f280 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique12prepareCacheEv
void prepareCache(Technique&) {}  // stub: texture-matrix cache build not mapped

}  // namespace texture_skin

namespace software_skin {

// 0x66f5f8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechniqueC2ERNS0_5SSkinERNS0_10SSkinCacheEb
// 0x66f658 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechniqueC1ERNS0_5SSkinERNS0_10SSkinCacheEb
void construct(Technique&, SkinStandIn&, SkinCacheStandIn&, bool) {}  // stub: members unmapped

// 0x66f724 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechniqueD1Ev
void destroy(Technique&) {}  // stub: map erase and base chain need unmapped members

}  // namespace software_skin

namespace skin_technique {

// 0x670dcc _ZN6glitch7collada6detail21IColladaSkinTechniqueD1Ev
// 0x670e58 _ZN6glitch7collada6detail21IColladaSkinTechniqueD2Ev
// Clears the vtable, releases the shared buffer at +0x08 when its count reaches zero.
void destroyBase(Interface&) {}  // stub: shared buffer layout not mapped

}  // namespace skin_technique

// ---------------------------------------------------------------------------
// glitch::ps shape domains and PSRandom helpers.
// ---------------------------------------------------------------------------
namespace ps {

// 0x69ac8c _ZN6glitch2ps7PDomainD1Ev
void domainDtor(PDomainStandIn&) {}

// 0x69ae04 _ZN6glitch2ps7PDPoint9transformERKNS_4core8CMatrix4IfEE
core::vector3d<float> pointTransform(core::vector3d<float>& point, const core::CMatrix4<float>&) {
    return point;  // stub: translation column of the matrix not mapped
}

// 0x69ae54 _ZNK6glitch2ps6PDLine8generateERNS0_8PSRandomE
core::vector3d<float> lineGenerate(const PDLine&, PSRandom&) {
    return core::vector3d<float>{};  // stub: PSRandom::Rand and the line parameters not mapped
}

// 0x69aecc _ZN6glitch2ps5PDBoxC2ERKNS_4core8vector3dIfEES6_
// 0x69b074 _ZN6glitch2ps5PDBoxC1ERKNS_4core8vector3dIfEES6_
void boxConstructVec(PDBox&, const core::vector3d<float>&, const core::vector3d<float>&) {}  // stub

// 0x69b21c _ZN6glitch2ps5PDBoxC2Efff
// 0x69b354 _ZN6glitch2ps5PDBoxC1Efff
void boxConstructFloat(PDBox&, float, float, float) {}  // stub: box extents not mapped

// 0x69b48c _ZNK6glitch2ps5PDBox6withinERKNS_4core8vector3dIfEE
bool boxWithin(const PDBox&, const core::vector3d<float>&) {
    return false;  // stub: box min and max words not mapped in PDBox
}

// 0x69b534 _ZNK6glitch2ps5PDBox8generateERNS0_8PSRandomE
core::vector3d<float> boxGenerate(const PDBox&, PSRandom&) {
    return core::vector3d<float>{};  // stub: PSRandom::RandVec and the box extents not mapped
}

// 0x69b668 _ZNK6glitch2ps10PDCylinder6withinERKNS_4core8vector3dIfEE
bool cylinderWithin(const PDCylinder&, const core::vector3d<float>&) {
    return false;  // stub: cylinder axis and radii not mapped in PDCylinder
}

// 0x69b804 _ZNK6glitch2ps6PDCone6withinERKNS_4core8vector3dIfEE
bool coneWithin(const PDCone&, const core::vector3d<float>&) {
    return false;  // stub: cone axis and radii not mapped in PDCone
}

// 0x69b9cc _ZN6glitch2ps8PDSphereC2ERKNS_4core8vector3dIfEEff
// 0x69bb80 _ZN6glitch2ps8PDSphereC1ERKNS_4core8vector3dIfEEff
void sphereConstruct(PDSphere&, const core::vector3d<float>&, float, float) {}  // stub

// 0x69bd34 _ZNK6glitch2ps8PDSphere6withinERKNS_4core8vector3dIfEE
bool sphereWithin(const PDSphere&, const core::vector3d<float>&) {
    return false;  // stub: sphere centre and radius words not mapped in PDSphere
}

// 0x69bdec _ZN6glitch2ps8PDSphere9transformERKNS_4core8CMatrix4IfEE
void sphereTransform(PDSphere&, const core::CMatrix4<float>&) {}  // stub: centre translation

// 0x69be10 _ZN6glitch2ps6PDBlobC2ERKNS_4core8vector3dIfEEf
// 0x69beb8 _ZN6glitch2ps6PDBlobC1ERKNS_4core8vector3dIfEEf
void blobConstruct(PDBlob&, const core::vector3d<float>&, float) {}  // stub

// 0x69bf60 _ZN6glitch2ps6PDBlob9transformERKNS_4core8CMatrix4IfEE
void blobTransform(PDBlob&, const core::CMatrix4<float>&) {}  // stub: centre translation

// 0x69c044 _ZNK6glitch2ps6PDBlob4copyEv
PDomainStandIn* blobCopy(const PDBlob&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c0ac _ZNK6glitch2ps8PDSphere4copyEv
PDomainStandIn* sphereCopy(const PDSphere&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c134 _ZNK6glitch2ps6PDCone4copyEv
PDomainStandIn* coneCopy(const PDCone&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c20c _ZNK6glitch2ps10PDCylinder4copyEv
PDomainStandIn* cylinderCopy(const PDCylinder&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c2ec _ZNK6glitch2ps5PDBox4copyEv
PDomainStandIn* boxCopy(const PDBox&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c3d4 _ZNK6glitch2ps6PDLine4copyEv
PDomainStandIn* lineCopy(const PDLine&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c45c _ZNK6glitch2ps7PDPoint4copyEv
PDomainStandIn* pointCopy(const PDPoint&) { return nullptr; }  // stub: copy-constructed clone

// 0x69c4ac _ZN6glitch2ps8PSRandom6NRandfEf
float randNormalf(PSRandom&, float) {
    return 0.0f;  // stub: needs PSRandom::Rand bound to the generator; see normalSample
}

// 0x69c57c _ZN6glitch2ps8PSRandom8NRandVecEf
core::vector3d<float> randNormalVec(PSRandom&, float) {
    return core::vector3d<float>{};  // stub: needs PSRandom::Rand bound to the generator; see normalVector
}

// 0x69c688 _ZNK6glitch2ps6PDBlob8generateERNS0_8PSRandomE
core::vector3d<float> blobGenerate(const PDBlob&, PSRandom&) {
    return core::vector3d<float>{};  // stub: NRandVec draw and centre offset not mapped
}

// 0x69c6ec _ZNK6glitch2ps8PDSphere8generateERNS0_8PSRandomE
core::vector3d<float> sphereGenerate(const PDSphere&, PSRandom&) {
    return core::vector3d<float>{};  // stub: radial sample and centre not mapped
}

// 0x69e008 _ZNK6glitch2ps6PDCone8generateERNS0_8PSRandomE
core::vector3d<float> coneGenerate(const PDCone&, PSRandom&) {
    return core::vector3d<float>{};  // stub: cone sample parameters not mapped
}

// 0x69e1bc _ZNK6glitch2ps10PDCylinder8generateERNS0_8PSRandomE
core::vector3d<float> cylinderGenerate(const PDCylinder&, PSRandom&) {
    return core::vector3d<float>{};  // stub: cylinder sample parameters not mapped
}

// 0x69df40 _ZNK6glitch2ps6PDBlob6withinERKNS_4core8vector3dIfEE
bool blobWithin(const PDBlob&, const core::vector3d<float>&) {
    return false;  // stub: blob centre and radius words not mapped
}

// 0x69e350 _ZNK6glitch2ps6PDLine6withinERKNS_4core8vector3dIfEE
bool lineWithin(const PDLine&, const core::vector3d<float>&) {
    return false;  // stub: line endpoints not mapped in PDLine
}

// 0x69e45c _ZN6glitch2ps6PDLineC1ERKNS_4core8vector3dIfEES6_
// 0x69e57c _ZN6glitch2ps6PDLineC2ERKNS_4core8vector3dIfEES6_
void lineConstruct(PDLine&, const core::vector3d<float>&, const core::vector3d<float>&) {}  // stub

// 0x69c860 _ZN6glitch2ps6PDConeC1ERKNS_4core8vector3dIfEES6_ff
// 0x69ccc0 _ZN6glitch2ps6PDConeC2ERKNS_4core8vector3dIfEES6_ff
void coneConstructVec(PDCone&, const core::vector3d<float>&, const core::vector3d<float>&, float, float) {}  // stub

// 0x69d120 _ZN6glitch2ps10PDCylinderC1Eff
// 0x69d3f4 _ZN6glitch2ps10PDCylinderC2Eff
void cylinderConstructFloat(PDCylinder&, float, float) {}  // stub: cylinder extents not mapped

// 0x69d6c8 _ZN6glitch2ps10PDCylinderC1ERKNS_4core8vector3dIfEES6_ff
// 0x69db04 _ZN6glitch2ps10PDCylinderC2ERKNS_4core8vector3dIfEES6_ff
void cylinderConstructVec(PDCylinder&, const core::vector3d<float>&, const core::vector3d<float>&, float, float) {}  // stub

// 0x69e69c _ZN6glitch2ps10PDCylinder9transformERKNS_4core8CMatrix4IfEE
void cylinderTransform(PDCylinder&, const core::CMatrix4<float>&) {}  // stub: axis transform not mapped

// 0x69ec90 _ZN6glitch2ps6PDLine9transformERKNS_4core8CMatrix4IfEE
void lineTransform(PDLine&, const core::CMatrix4<float>&) {}  // stub: endpoint transform not mapped

// 0x69efac _ZN6glitch2ps5PDBox9transformERKNS_4core8CMatrix4IfEE
void boxTransform(PDBox&, const core::CMatrix4<float>&) {}  // stub: corner transform not mapped

}  // namespace ps

// ---------------------------------------------------------------------------
// glitch::gui::ICursorControl base destructor.
// ---------------------------------------------------------------------------
namespace gui_cursor {

// 0x69ffe4 _ZN6glitch3gui14ICursorControlD1Ev
void destroyBase(ControlStandIn&) {}

}  // namespace gui_cursor

}  // namespace glitch::collada::recon_23
