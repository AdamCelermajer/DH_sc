// Slice 22 reconstruction (see include/glitch-scene-collada-gui/recon/slice_22.hpp).
// Each function keeps its original address and mangled symbol in its comment.
// Stubbed bodies say what is missing; the ledger note for the row repeats it.

#include "glitch-scene-collada-gui/recon/slice_22.hpp"

namespace glitch::collada::recon_22 {

// ---------------------------------------------------------------------------
// CAnimationSet
// ---------------------------------------------------------------------------

// 0x65f2ac _ZN6glitch7collada13CAnimationSetD1Ev
CAnimationSet::~CAnimationSet()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); D1 body frees the member buffers and the database vector
}

// 0x6600a4 _ZN6glitch7collada13CAnimationSet20addAnimationTemplateEPNS0_16CColladaDatabaseE
void CAnimationSet::addAnimationTemplate(CColladaDatabase* database)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the transformation template class is not modelled
}

// 0x6600d0 _ZN6glitch7collada13CAnimationSet20addAnimationTemplateEPNS0_10CSceneNodeE
void CAnimationSet::addAnimationTemplate(CSceneNode* node)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the transformation template class is not modelled
}


// ---------------------------------------------------------------------------
// CSceneNodeAnimatorSet
// ---------------------------------------------------------------------------

// 0x65f15c _ZN6glitch7collada21CSceneNodeAnimatorSet10getBindURIEi
int CSceneNodeAnimatorSet::getBindURI(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); child animator dispatch through a vtable slot whose method name is not identified
    return 0;
}

// 0x65f364 _ZN6glitch7collada21CSceneNodeAnimatorSet16getAnimationDataEi
int CSceneNodeAnimatorSet::getAnimationData(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); reads the animation block via CAnimationStreamingManager which is not modelled
    return 0;
}

// 0x65f8ac _ZNK6glitch7collada21CSceneNodeAnimatorSet25getAnimationVisualSceneIDEi
int CSceneNodeAnimatorSet::getAnimationVisualSceneID(int index) const
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); CAnimationSet::getDatabase and CColladaDatabase::getVisualScene not modelled
    return 0;
}

// 0x65fa08 _ZN6glitch7collada21CSceneNodeAnimatorSetD1Ev
CSceneNodeAnimatorSet::~CSceneNodeAnimatorSet()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); D1 body frees the member arrays and drops the database reference
}

// 0x65fb2c _ZN6glitch7collada21CSceneNodeAnimatorSetD2Ev
void CSceneNodeAnimatorSet::destroyAsSecondaryBase(int* vtt)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); secondary base variant with the int* table argument; same body as the D1 destructor
}


// ---------------------------------------------------------------------------
// CSceneNodeAnimatorSynchronizedBlender
// ---------------------------------------------------------------------------

// 0x660db0 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender17getAnimationTrackEi
int CSceneNodeAnimatorSynchronizedBlender::getAnimationTrack(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); vtable slot 84 on the first child animator not named
    return 0;
}

// 0x660dd4 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender19getAnimationTrackExEi
int CSceneNodeAnimatorSynchronizedBlender::getAnimationTrackEx(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); vtable slot 88 on the first child animator not named
    return 0;
}

// 0x660df8 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
int CSceneNodeAnimatorSynchronizedBlender::setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); target and applicator arrays at +0x60 and +0x6c not in layout headers
    return 0;
}

// 0x660e60 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender10getBindURIEi
int CSceneNodeAnimatorSynchronizedBlender::getBindURI(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); vtable slot 108 on the first child animator not named
    return 0;
}

// 0x660e84 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender14getTargetCountEv
int CSceneNodeAnimatorSynchronizedBlender::getTargetCount()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); vtable slot 112 on the first child animator not named
    return 0;
}

// 0x660ea8 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender13getTargetSizeEi
int CSceneNodeAnimatorSynchronizedBlender::getTargetSize(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); vtable slot 116 on the first child animator not named
    return 0;
}

// 0x660ecc _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender14getTargetsSizeEv
int CSceneNodeAnimatorSynchronizedBlender::getTargetsSize()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); vtable slot 120 on the first child animator not named
    return 0;
}

// 0x660ef0 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender9getLengthEv
int CSceneNodeAnimatorSynchronizedBlender::getLength() const
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); child length slot 72 and weight array at +0x3c not in layout headers
    return 0;
}

// 0x660f8c _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender32applySynchronizedAnimationValuesEf
float CSceneNodeAnimatorSynchronizedBlender::applySynchronizedAnimationValues(float time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); normalizeWeights not modelled; child slot 56 not named
    return 0.0f;
}

// 0x660fec _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender34computeSynchronizedAnimationValuesEf
int CSceneNodeAnimatorSynchronizedBlender::computeSynchronizedAnimationValues(float time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); normalizeWeights; child slot 56 and the applicator slot 16 not named
    return 0;
}

// 0x6610d8 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender11animateNodeEPNS_5scene10ISceneNodeEj
int CSceneNodeAnimatorSynchronizedBlender::animateNode(scene::ISceneNode* node, unsigned int time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); forwards to vtable slot 80 on this; which is not named
    return 0;
}

// 0x6610f8 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender13hasReachedEndEv
int CSceneNodeAnimatorSynchronizedBlender::hasReachedEnd()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); child slot 36 not named
    return 0;
}

// 0x661120 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender18fillInputTimeStartEv
int CSceneNodeAnimatorSynchronizedBlender::fillInputTimeStart()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); child slot 40 and the input array at +0x7c not in layout headers
    return 0;
}

// 0x661184 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender16fillInputTimeEndEv
int CSceneNodeAnimatorSynchronizedBlender::fillInputTimeEnd()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); child slots 44 and 72 and the input array at +0x7c not in layout headers
    return 0;
}

// 0x661200 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender35fillInputSynchronizationElapsedTimeEv
int CSceneNodeAnimatorSynchronizedBlender::fillInputSynchronizationElapsedTime()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); child slots 48; 60 and 72 not named
    return 0;
}

// 0x6612bc _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender22computeAnimationValuesEj
int CSceneNodeAnimatorSynchronizedBlender::computeAnimationValues(unsigned int time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the blended value loop and slots 30 and 184 and 196 not named; timeline clip at +0x78 not modelled
    return 0;
}

// 0x661408 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender20applyAnimationValuesEj
int CSceneNodeAnimatorSynchronizedBlender::applyAnimationValues(unsigned int time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); same as computeAnimationValues plus applicator slot 24 and 88 not named
    return 0;
}

// 0x66186c _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderD1Ev
CSceneNodeAnimatorSynchronizedBlender::~CSceneNodeAnimatorSynchronizedBlender()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); D1 body frees the member arrays and drops child references
}

// 0x661a08 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderD2Ev
void CSceneNodeAnimatorSynchronizedBlender::destroyAsSecondaryBase(int* vtt)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}

// 0x661b74 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderC1Ev
CSceneNodeAnimatorSynchronizedBlender::CSceneNodeAnimatorSynchronizedBlender()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the constructor zeroes the member words; their names are not in layout headers so they are not reproduced
}

// 0x661c78 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderC2Ev
void CSceneNodeAnimatorSynchronizedBlender::constructAsSecondaryBase(int* vtt)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}

// 0x662290 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender17getAnimationValueEiiPv
void CSceneNodeAnimatorSynchronizedBlender::getAnimationValue(int index, int time, void* out)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); per-child slots 88; 8 and 124 not named; the temporary vectors are not modelled
}

// 0x6623d4 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender7compileEPSt6vectorIhNS_4core10SAllocatorIhLNS_6memory13E_MEMORY_HINTE0EEEE
int CSceneNodeAnimatorSynchronizedBlender::compile(std::vector<unsigned char>* buffer)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); CTimelineController and the synchronization vectors are not modelled; the function is 852 bytes
    return 0;
}


// ---------------------------------------------------------------------------
// CSceneNodeAnimatorSynchronizedSet
// ---------------------------------------------------------------------------

// 0x662794 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSet34computeSynchronizedAnimationValuesEf
int CSceneNodeAnimatorSynchronizedSet::computeSynchronizedAnimationValues(float time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the base is CSceneNodeAnimatorSet; members at +0x7c and +0x80 are not in layout headers
    return 0;
}

// 0x662884 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSet32applySynchronizedAnimationValuesEf
int CSceneNodeAnimatorSynchronizedSet::applySynchronizedAnimationValues(float time)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the base is CSceneNodeAnimatorSet; members at +0x7c and +0x80 are not in layout headers
    return 0;
}

// 0x662b60 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetD1Ev
CSceneNodeAnimatorSynchronizedSet::~CSceneNodeAnimatorSynchronizedSet()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); D1 body destroys the name and synchronization vectors then the base
}

// 0x662c10 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetD2Ev
void CSceneNodeAnimatorSynchronizedSet::destroyAsSecondaryBase(int* vtt)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}

// 0x662d10 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetC1ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEERKSt6vectorISsNS_4core10SAllocatorISsLNS_6memory13E_MEMORY_HINTE0EEEE
CSceneNodeAnimatorSynchronizedSet::CSceneNodeAnimatorSynchronizedSet(const CAnimationSet* const& animationSet, const std::vector<std::string>& names)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); boost::intrusive_ptr replaced by a raw pointer stand-in
}

// 0x662df0 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetC2ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEERKSt6vectorISsNS_4core10SAllocatorISsLNS_6memory13E_MEMORY_HINTE0EEEE
void CSceneNodeAnimatorSynchronizedSet::constructAsSecondaryBase(int* vtt, const CAnimationSet* const& animationSet, const std::vector<std::string>& names)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}

// 0x663324 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSet19setCurrentAnimationEi
int CSceneNodeAnimatorSynchronizedSet::setCurrentAnimation(int index)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the name copy and the synchronization vector at +0x60 are not modelled; child slot 20 not named
    return 0;
}


// ---------------------------------------------------------------------------
// CSkinnedMesh
// ---------------------------------------------------------------------------

// 0x663538 _ZN6glitch7collada12CSkinnedMesh6attachEPNS_5scene10ISceneNodeE
int CSkinnedMesh::attach(scene::ISceneNode* node)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); attach hook not modelled
    return 0;
}

// 0x663584 _ZN6glitch7collada12CSkinnedMesh9onAnimateEj
int CSkinnedMesh::onAnimate(unsigned int time)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); virtual dispatch through the mesh slot not named
    return 0;
}

// 0x6635b4 _ZNK6glitch7collada12CSkinnedMesh18getMeshBufferCountEv
int CSkinnedMesh::getMeshBufferCount() const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); buffer array at +0x5c with 0x14 stride not in layout headers
    return 0;
}

// 0x6636a4 _ZNK6glitch7collada12CSkinnedMesh14getBoundingBoxEv
core::aabbox3d<float> CSkinnedMesh::getBoundingBox() const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); the pseudocode returns the member address as char*; returned by value here; box member not in layout headers
    return {};
}

// 0x66383c _ZNK6glitch7collada12CSkinnedMesh21reverifySkinTechniqueERKNS0_11SSkinBufferE
int CSkinnedMesh::reverifySkinTechnique(const SSkinBuffer& buffer) const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); technique check depends on the material and vertex attribute map layouts
    return 0;
}

// 0x6638bc _ZNK6glitch7collada12CSkinnedMesh16needOutputBufferEv
int CSkinnedMesh::needOutputBuffer() const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); per-buffer scan not in layout headers
    return 0;
}

// 0x66391c _ZN6glitch7collada12CSkinnedMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE
int CSkinnedMesh::setMaterial(unsigned int index, CMaterial* material, CMaterialVertexAttributeMap* attributeMap)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); refcount handling on the buffer entry not in layout headers
    return 0;
}

// 0x6639f4 _ZN6glitch7collada12CSkinnedMesh20releaseProcessBufferEPNS_5video12IVideoDriverEj
int CSkinnedMesh::releaseProcessBuffer(IVideoDriver* driver, unsigned int index)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); the video driver interface is not modelled
    return 0;
}

// 0x664c84 _ZN6glitch7collada12CSkinnedMesh20setIsSkinningEnabledEb
int CSkinnedMesh::setIsSkinningEnabled(bool enabled)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); per-buffer technique update not in layout headers
    return 0;
}

// 0x664e0c _ZN6glitch7collada12CSkinnedMesh15updateTechniqueEj
void* CSkinnedMesh::updateTechnique(unsigned int index)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); the pseudocode returns a refcounted pointer; the material type is not modelled
    return nullptr;
}

// 0x665580 _ZNK6glitch7collada12CSkinnedMesh12getTransformERKNS_4core8CMatrix4IfEE
const CMatrix4* CSkinnedMesh::getTransform(const CMatrix4& matrix) const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); CMatrix4 is a stand-in pointer type here
    return nullptr;
}

// 0x665678 _ZN6glitch7collada12CSkinnedMesh4initEPNS_5video12IVideoDriverEb
void CSkinnedMesh::init(IVideoDriver* driver, bool flag)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); buffer setup not in layout headers
}

// 0x665938 _ZN6glitch7collada12CSkinnedMeshD1Ev
CSkinnedMesh::~CSkinnedMesh()
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); D1 body with the SSkinCache base member
}

// 0x665a38 _ZN6glitch7collada12CSkinnedMeshD2Ev
void CSkinnedMesh::destroyCompleteObject()
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); second ~CSkinnedMesh with the same demangled name; declared under a distinct name
}

// 0x665fe8 _ZN6glitch7collada12CSkinnedMeshC2ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERNS0_11SControllerEPNS0_14CRootSceneNodeE
CSkinnedMesh::CSkinnedMesh(const CColladaDatabase& database, IVideoDriver* driver, SController& controller, CRootSceneNode* root)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); the 1264 byte constructor is only outlined here
}


// ---------------------------------------------------------------------------
// SSkinCache
// ---------------------------------------------------------------------------

// 0x663b54 _ZN6glitch7collada10SSkinCacheD1Ev
SSkinCache::~SSkinCache()
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); cache destructor frees its buffers; layout not in layout headers
}


// ---------------------------------------------------------------------------
// SSkinBuffer
// ---------------------------------------------------------------------------

// 0x664530 _ZN6glitch7collada11SSkinBufferaSERKS1_
SSkinBuffer& SSkinBuffer::operator=(const SSkinBuffer& other)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); the copy of the buffer fields is not reproduced; layout not in layout headers
    return *this;
}

// 0x6645f8 _ZN6glitch7collada11SSkinBufferD1Ev
SSkinBuffer::~SSkinBuffer()
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); releases the attribute map and material references; layout not in layout headers
}


// ---------------------------------------------------------------------------
// CSkinnedMeshSceneNode
// ---------------------------------------------------------------------------

// 0x666a04 _ZNK6glitch7collada21CSkinnedMeshSceneNode25getTransformedBoundingBoxEv
core::aabbox3d<float> CSkinnedMeshSceneNode::getTransformedBoundingBox() const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); transform applied to the mesh box not modelled
    return {};
}

// 0x666a18 _ZN6glitch7collada21CSkinnedMeshSceneNode20setIsSkinningEnabledEb
int CSkinnedMeshSceneNode::setIsSkinningEnabled(bool enabled)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); forwards to the mesh through a vtable slot not named
    return 0;
}

// 0x666a54 _ZN6glitch7collada21CSkinnedMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS0_5SNodeEiRKNS_4core8vector3dIfEERKNSA_10quaternionESE_
CSkinnedMeshSceneNode::CSkinnedMeshSceneNode(IMesh* const& mesh, SNode* node, int id, const core::vector3d<float>& position, const SQuaternion& rotation, const core::vector3d<float>& scale)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); scene node base not in layout headers; quaternion is a stand-in
}

// 0x666b3c _ZN6glitch7collada21CSkinnedMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS0_5SNodeEiRKNS_4core8vector3dIfEERKNSA_10quaternionESE_
void CSkinnedMeshSceneNode::constructAsSecondaryBase(int* vtt, IMesh* const& mesh, SNode* node, int id, const core::vector3d<float>& position, const SQuaternion& rotation, const core::vector3d<float>& scale)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}


// ---------------------------------------------------------------------------
// IMesh
// ---------------------------------------------------------------------------

// 0x667704 _ZN6glitch7collada5IMeshD1Ev
IMesh::~IMesh()
{
    // Stub: unattached: needs member declaration in layouts/collada_db.hpp (class layout not in layout headers); sets the vptr then destroys the CColladaDatabase member at +0x0c
}


// ---------------------------------------------------------------------------
// CTimelineController
// ---------------------------------------------------------------------------

// 0x666d04 _ZNK6glitch7collada19CTimelineController13getClipLengthEi
int CTimelineController::getClipLength(int index) const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); clip table at +0x34 not in layout headers
    return 0;
}

// 0x666d6c _ZNK6glitch7collada19CTimelineController20getCurrentClipLengthEv
int CTimelineController::getCurrentClipLength() const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); current clip index not in layout headers
    return 0;
}

// 0x666f44 _ZN6glitch7collada19CTimelineController7setClipEPKc
int CTimelineController::setClip(const char* name)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); clip lookup by name not modelled
    return 0;
}

// 0x6670a4 _ZNK6glitch7collada19CTimelineController12getClipIndexEPKc
int CTimelineController::getClipIndex(const char* name) const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); clip table at +0x34 not in layout headers
    return 0;
}


// ---------------------------------------------------------------------------
// IAnimationSetTemplate
// ---------------------------------------------------------------------------

// 0x667588 _ZN6glitch7collada21IAnimationSetTemplateD1Ev
IAnimationSetTemplate::~IAnimationSetTemplate()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); D1 body frees the two owned buffers
}

// 0x66762c _ZN6glitch7collada21IAnimationSetTemplateD2Ev
void IAnimationSetTemplate::destroyCompleteObject()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); second ~IAnimationSetTemplate with the same demangled name; declared under a distinct name
}


// ---------------------------------------------------------------------------
// IParticleSystemSceneNode
// ---------------------------------------------------------------------------

// 0x66777c _ZNK6glitch7collada24IParticleSystemSceneNode11getMaterialEj
CMaterial* IParticleSystemSceneNode::getMaterial(unsigned int index) const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); material array at +0x164 not in layout headers
    return nullptr;
}

// 0x66779c _ZNK6glitch7collada24IParticleSystemSceneNode16getMaterialCountEv
int IParticleSystemSceneNode::getMaterialCount() const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); material array size not in layout headers
    return 0;
}

// 0x66781c _ZN6glitch7collada24IParticleSystemSceneNodeD1Ev
IParticleSystemSceneNode::~IParticleSystemSceneNode()
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); D1 body frees the buffer and the material list then the scene node base
}

// 0x6678b8 _ZN6glitch7collada24IParticleSystemSceneNodeD2Ev
void IParticleSystemSceneNode::destroyAsSecondaryBase(int* vtt)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}

// 0x66792c _ZN6glitch7collada24IParticleSystemSceneNodeC1ERKNS0_16CColladaDatabaseEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
IParticleSystemSceneNode::IParticleSystemSceneNode(const CColladaDatabase& database, std::vector<std::string>* names, CRootSceneNode* root)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); particle parameter setup not modelled; res::vector replaced by std::vector stand-in
}

// 0x667a98 _ZN6glitch7collada24IParticleSystemSceneNodeC2ERKNS0_16CColladaDatabaseEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
void IParticleSystemSceneNode::constructAsSecondaryBase(int* vtt, const CColladaDatabase& database, std::vector<std::string>* names, CRootSceneNode* root)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}


// ---------------------------------------------------------------------------
// ISceneNodeAnimator
// ---------------------------------------------------------------------------

// 0x667d18 _ZN6glitch7collada18ISceneNodeAnimator11forceUnbindEPKc
int ISceneNodeAnimator::forceUnbind(const char* name)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the bound target list and the unbind slot not in layout headers
    return 0;
}

// 0x667d9c _ZN6glitch7collada18ISceneNodeAnimator8onUnbindEPNS_5scene10ISceneNodeE
int ISceneNodeAnimator::onUnbind(scene::ISceneNode* node)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); unbind hook not modelled
    return 0;
}

// 0x667e40 _ZN6glitch7collada18ISceneNodeAnimator14forceUnbindAllEv
int ISceneNodeAnimator::forceUnbindAll()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the bound target list is not in layout headers
    return 0;
}

// 0x669538 _ZN6glitch7collada18ISceneNodeAnimator6onBindEPNS_5scene10ISceneNodeE
int ISceneNodeAnimator::onBind(scene::ISceneNode* node)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the pseudocode return type is CMaterial** which looks misdecompiled; declared int
    return 0;
}

// 0x669738 _ZN6glitch7collada18ISceneNodeAnimatorD1Ev
ISceneNodeAnimator::~ISceneNodeAnimator()
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); D1 body releases the bound target list then the animator base
}

// 0x6697c4 _ZN6glitch7collada18ISceneNodeAnimatorD2Ev
void ISceneNodeAnimator::destroyAsSecondaryBase(int* vtt)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); secondary base variant with the int* table argument
}

// 0x6699c4 _ZN6glitch7collada18ISceneNodeAnimator19hasCompatibleTargetERKNS0_8SChannelE
int ISceneNodeAnimator::hasCompatibleTarget(const SChannel& channel)
{
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); channel target matching depends on the animation target layouts (372 bytes)
    return 0;
}


// ---------------------------------------------------------------------------
// ISkinnedMesh
// ---------------------------------------------------------------------------

// 0x669bf4 _ZN6glitch7collada12ISkinnedMeshD1Ev
ISkinnedMesh::~ISkinnedMesh()
{
    // Stub: unattached: needs member declaration in layouts/collada_db.hpp (class layout not in layout headers); sets the vptr then destroys the CColladaDatabase member at +0x0c
}

// 0x669c28 _ZN6glitch7collada12ISkinnedMesh20releaseProcessBufferEPNS_5video12IVideoDriverEj
int ISkinnedMesh::releaseProcessBuffer(IVideoDriver* driver, unsigned int index)
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); the pseudocode return type is CMaterial** which looks misdecompiled; declared int
    return 0;
}

// 0x669d6c _ZNK6glitch7collada12ISkinnedMesh12getTransformERKNS_4core8CMatrix4IfEE
const CMatrix4* ISkinnedMesh::getTransform(const CMatrix4& matrix) const
{
    // Stub: unattached: needs member declaration in layouts/scene.hpp (class layout not in layout headers); CMatrix4 is a stand-in pointer type here
    return nullptr;
}

// 0x667ed0 _ZNK6glitch7collada15animation_track23CMaterialApplicatorInfo5cloneEv
animation_track::CMaterialApplicatorInfo* cloneMaterialApplicatorInfo(const animation_track::CMaterialApplicatorInfo* self) {
    // The original allocates 0x0c bytes and writes the vtable at 0x985310, which is not identified here.
    // The vptr is left null; words +0x04 and +0x08 are copied.
    auto* result = new animation_track::CMaterialApplicatorInfo{};
    result->word_0x04 = self->word_0x04;
    result->word_0x08 = self->word_0x08;
    return result;
}

// 0x66a010 _ZNK6glitch7collada18SAnimationAccessor10applyValueEiiPvPNS0_15animation_track15CApplicatorInfoERib
int applyValue(const SAnimationAccessor& accessor, int a, int b, void* target, animation_track::CApplicatorInfo* applicator, int* out, bool flag) {
    // Stub: unattached: needs member declaration in layouts/animation_track.hpp (class layout not in layout headers); the track pointer is a 32-bit guest word that cannot be dereferenced on a 64-bit host; the vtable slot 128 method is not named
    return 0;
}

// Not rebuilt (D0 deleting destructor thunks): 0x65f348 0x65fb10 0x6615f4 0x6619ec 0x662bf4 0x667020 0x667044 0x667088 0x667610 0x667738 0x66789c 0x667cf0 0x667d04 0x6697a8 0x669dc4

}  // namespace glitch::collada::recon_22
