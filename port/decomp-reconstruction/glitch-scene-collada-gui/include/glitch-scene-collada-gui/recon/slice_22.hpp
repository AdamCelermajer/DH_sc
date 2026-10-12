// Slice 22 reconstruction: glitch::collada scene-node animator and skinned-mesh
// classes plus the animation_track applicator helper. Most rows are partial: the
// member layouts of these classes are not in the layout headers yet, so the bodies
// are stubs and each ledger note says what is missing.
// The stand-in classes live in glitch::collada::recon_22 and have no members.
#pragma once

#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/layouts/collada_db.hpp"
#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::collada::recon_22 {

// Incomplete stand-ins for types outside this slice.
class CSceneNode;
class SNode;
class CRootSceneNode;
class SController;
class SChannel;
class IVideoDriver;
class CMaterial;
class CMaterialVertexAttributeMap;
class CMatrix4;
class SQuaternion;
class IMesh;
class SSkinBuffer;

// Stand-in for glitch::collada::CAnimationSet (layout not in layout headers; no members).
class CAnimationSet {
public:
    // 0x65f2ac _ZN6glitch7collada13CAnimationSetD1Ev
    ~CAnimationSet();
    // 0x6600a4 _ZN6glitch7collada13CAnimationSet20addAnimationTemplateEPNS0_16CColladaDatabaseE
    void addAnimationTemplate(CColladaDatabase* database);
    // 0x6600d0 _ZN6glitch7collada13CAnimationSet20addAnimationTemplateEPNS0_10CSceneNodeE
    void addAnimationTemplate(CSceneNode* node);
};

// Stand-in for glitch::collada::CSceneNodeAnimatorSet (layout not in layout headers; no members).
class CSceneNodeAnimatorSet {
public:
    // 0x65f15c _ZN6glitch7collada21CSceneNodeAnimatorSet10getBindURIEi
    int getBindURI(int index);
    // 0x65f364 _ZN6glitch7collada21CSceneNodeAnimatorSet16getAnimationDataEi
    int getAnimationData(int index);
    // 0x65f8ac _ZNK6glitch7collada21CSceneNodeAnimatorSet25getAnimationVisualSceneIDEi
    int getAnimationVisualSceneID(int index) const;
    // 0x65fa08 _ZN6glitch7collada21CSceneNodeAnimatorSetD1Ev
    ~CSceneNodeAnimatorSet();
    // 0x65fb2c _ZN6glitch7collada21CSceneNodeAnimatorSetD2Ev
    void destroyAsSecondaryBase(int* vtt);
};

// Stand-in for glitch::collada::CSceneNodeAnimatorSynchronizedBlender (layout not in layout headers; no members).
class CSceneNodeAnimatorSynchronizedBlender {
public:
    // 0x660db0 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender17getAnimationTrackEi
    int getAnimationTrack(int index);
    // 0x660dd4 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender19getAnimationTrackExEi
    int getAnimationTrackEx(int index);
    // 0x660df8 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
    int setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator);
    // 0x660e60 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender10getBindURIEi
    int getBindURI(int index);
    // 0x660e84 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender14getTargetCountEv
    int getTargetCount();
    // 0x660ea8 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender13getTargetSizeEi
    int getTargetSize(int index);
    // 0x660ecc _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender14getTargetsSizeEv
    int getTargetsSize();
    // 0x660ef0 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender9getLengthEv
    int getLength() const;
    // 0x660f8c _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender32applySynchronizedAnimationValuesEf
    float applySynchronizedAnimationValues(float time);
    // 0x660fec _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender34computeSynchronizedAnimationValuesEf
    int computeSynchronizedAnimationValues(float time);
    // 0x6610d8 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender11animateNodeEPNS_5scene10ISceneNodeEj
    int animateNode(scene::ISceneNode* node, unsigned int time);
    // 0x6610f8 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender13hasReachedEndEv
    int hasReachedEnd();
    // 0x661120 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender18fillInputTimeStartEv
    int fillInputTimeStart();
    // 0x661184 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender16fillInputTimeEndEv
    int fillInputTimeEnd();
    // 0x661200 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender35fillInputSynchronizationElapsedTimeEv
    int fillInputSynchronizationElapsedTime();
    // 0x6612bc _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender22computeAnimationValuesEj
    int computeAnimationValues(unsigned int time);
    // 0x661408 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender20applyAnimationValuesEj
    int applyAnimationValues(unsigned int time);
    // 0x66186c _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderD1Ev
    ~CSceneNodeAnimatorSynchronizedBlender();
    // 0x661a08 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderD2Ev
    void destroyAsSecondaryBase(int* vtt);
    // 0x661b74 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderC1Ev
    CSceneNodeAnimatorSynchronizedBlender();
    // 0x661c78 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlenderC2Ev
    void constructAsSecondaryBase(int* vtt);
    // 0x662290 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender17getAnimationValueEiiPv
    void getAnimationValue(int index, int time, void* out);
    // 0x6623d4 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender7compileEPSt6vectorIhNS_4core10SAllocatorIhLNS_6memory13E_MEMORY_HINTE0EEEE
    int compile(std::vector<unsigned char>* buffer);
};

// Stand-in for glitch::collada::CSceneNodeAnimatorSynchronizedSet (layout not in layout headers; no members).
class CSceneNodeAnimatorSynchronizedSet {
public:
    // 0x662794 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSet34computeSynchronizedAnimationValuesEf
    int computeSynchronizedAnimationValues(float time);
    // 0x662884 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSet32applySynchronizedAnimationValuesEf
    int applySynchronizedAnimationValues(float time);
    // 0x662b60 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetD1Ev
    ~CSceneNodeAnimatorSynchronizedSet();
    // 0x662c10 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetD2Ev
    void destroyAsSecondaryBase(int* vtt);
    // 0x662d10 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetC1ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEERKSt6vectorISsNS_4core10SAllocatorISsLNS_6memory13E_MEMORY_HINTE0EEEE
    CSceneNodeAnimatorSynchronizedSet(const CAnimationSet* const& animationSet, const std::vector<std::string>& names);
    // 0x662df0 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSetC2ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEERKSt6vectorISsNS_4core10SAllocatorISsLNS_6memory13E_MEMORY_HINTE0EEEE
    void constructAsSecondaryBase(int* vtt, const CAnimationSet* const& animationSet, const std::vector<std::string>& names);
    // 0x663324 _ZN6glitch7collada33CSceneNodeAnimatorSynchronizedSet19setCurrentAnimationEi
    int setCurrentAnimation(int index);
};

// Stand-in for glitch::collada::CSkinnedMesh (layout not in layout headers; no members).
class CSkinnedMesh {
public:
    // 0x663538 _ZN6glitch7collada12CSkinnedMesh6attachEPNS_5scene10ISceneNodeE
    int attach(scene::ISceneNode* node);
    // 0x663584 _ZN6glitch7collada12CSkinnedMesh9onAnimateEj
    int onAnimate(unsigned int time);
    // 0x6635b4 _ZNK6glitch7collada12CSkinnedMesh18getMeshBufferCountEv
    int getMeshBufferCount() const;
    // 0x6636a4 _ZNK6glitch7collada12CSkinnedMesh14getBoundingBoxEv
    core::aabbox3d<float> getBoundingBox() const;
    // 0x66383c _ZNK6glitch7collada12CSkinnedMesh21reverifySkinTechniqueERKNS0_11SSkinBufferE
    int reverifySkinTechnique(const SSkinBuffer& buffer) const;
    // 0x6638bc _ZNK6glitch7collada12CSkinnedMesh16needOutputBufferEv
    int needOutputBuffer() const;
    // 0x66391c _ZN6glitch7collada12CSkinnedMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS3_INS4_27CMaterialVertexAttributeMapEEE
    int setMaterial(unsigned int index, CMaterial* material, CMaterialVertexAttributeMap* attributeMap);
    // 0x6639f4 _ZN6glitch7collada12CSkinnedMesh20releaseProcessBufferEPNS_5video12IVideoDriverEj
    int releaseProcessBuffer(IVideoDriver* driver, unsigned int index);
    // 0x664c84 _ZN6glitch7collada12CSkinnedMesh20setIsSkinningEnabledEb
    int setIsSkinningEnabled(bool enabled);
    // 0x664e0c _ZN6glitch7collada12CSkinnedMesh15updateTechniqueEj
    void* updateTechnique(unsigned int index);
    // 0x665580 _ZNK6glitch7collada12CSkinnedMesh12getTransformERKNS_4core8CMatrix4IfEE
    const CMatrix4* getTransform(const CMatrix4& matrix) const;
    // 0x665678 _ZN6glitch7collada12CSkinnedMesh4initEPNS_5video12IVideoDriverEb
    void init(IVideoDriver* driver, bool flag);
    // 0x665938 _ZN6glitch7collada12CSkinnedMeshD1Ev
    ~CSkinnedMesh();
    // 0x665a38 _ZN6glitch7collada12CSkinnedMeshD2Ev
    void destroyCompleteObject();
    // 0x665fe8 _ZN6glitch7collada12CSkinnedMeshC2ERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERNS0_11SControllerEPNS0_14CRootSceneNodeE
    CSkinnedMesh(const CColladaDatabase& database, IVideoDriver* driver, SController& controller, CRootSceneNode* root);
};

// Stand-in for glitch::collada::SSkinCache (layout not in layout headers; no members).
class SSkinCache {
public:
    // 0x663b54 _ZN6glitch7collada10SSkinCacheD1Ev
    ~SSkinCache();
};

// Stand-in for glitch::collada::SSkinBuffer (layout not in layout headers; no members).
class SSkinBuffer {
public:
    // 0x664530 _ZN6glitch7collada11SSkinBufferaSERKS1_
    SSkinBuffer& operator=(const SSkinBuffer& other);
    // 0x6645f8 _ZN6glitch7collada11SSkinBufferD1Ev
    ~SSkinBuffer();
};

// Stand-in for glitch::collada::CSkinnedMeshSceneNode (layout not in layout headers; no members).
class CSkinnedMeshSceneNode {
public:
    // 0x666a04 _ZNK6glitch7collada21CSkinnedMeshSceneNode25getTransformedBoundingBoxEv
    core::aabbox3d<float> getTransformedBoundingBox() const;
    // 0x666a18 _ZN6glitch7collada21CSkinnedMeshSceneNode20setIsSkinningEnabledEb
    int setIsSkinningEnabled(bool enabled);
    // 0x666a54 _ZN6glitch7collada21CSkinnedMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS0_5SNodeEiRKNS_4core8vector3dIfEERKNSA_10quaternionESE_
    CSkinnedMeshSceneNode(IMesh* const& mesh, SNode* node, int id, const core::vector3d<float>& position, const SQuaternion& rotation, const core::vector3d<float>& scale);
    // 0x666b3c _ZN6glitch7collada21CSkinnedMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS0_5SNodeEiRKNS_4core8vector3dIfEERKNSA_10quaternionESE_
    void constructAsSecondaryBase(int* vtt, IMesh* const& mesh, SNode* node, int id, const core::vector3d<float>& position, const SQuaternion& rotation, const core::vector3d<float>& scale);
};

// Stand-in for glitch::collada::IMesh (layout not in layout headers; no members).
class IMesh {
public:
    // 0x667704 _ZN6glitch7collada5IMeshD1Ev
    ~IMesh();
};

// Stand-in for glitch::collada::CTimelineController (layout not in layout headers; no members).
class CTimelineController {
public:
    // 0x666d04 _ZNK6glitch7collada19CTimelineController13getClipLengthEi
    int getClipLength(int index) const;
    // 0x666d6c _ZNK6glitch7collada19CTimelineController20getCurrentClipLengthEv
    int getCurrentClipLength() const;
    // 0x666f44 _ZN6glitch7collada19CTimelineController7setClipEPKc
    int setClip(const char* name);
    // 0x6670a4 _ZNK6glitch7collada19CTimelineController12getClipIndexEPKc
    int getClipIndex(const char* name) const;
};

// Stand-in for glitch::collada::IAnimationSetTemplate. The members below come from the
// setUnAdded and addChannels pseudocode (batch e1-glitch-scene-collada-gui-glitch_collada_IAnimationSetTemplate-0).
// The vtable word at +0x00 is not modelled: no virtual functions are declared here.
class IAnimationSetTemplate {
public:
    // 16-byte record that addChannels allocates for each target. Field names are not recovered.
    struct TargetRecord {
        unsigned int reserved00 = 0;    // +0x00 not written by addChannels
        unsigned int value04 = 0;       // +0x04 set from virtual slot +0x54 of the target's source (interface not mapped)
        unsigned int kind08 = 0;        // +0x08 copied from Target::kind
        unsigned int reserved0c = 0;    // +0x0c not written by addChannels
    };
    // Element of the targets vector. The vector stores one pointer per element (stride 4).
    // The struct name is not recovered; it is a stand-in.
    struct Target {
        unsigned char added = 0;        // +0x00 flag: 0 = not added, 1 = added (setUnAdded clears it)
        unsigned int kind = 0;          // +0x04 kind, the switch selector in addChannels (enumerators not mapped)
        void* source = nullptr;         // +0x08 object whose virtual slot +0x54 gives TargetRecord::value04
        TargetRecord* record = nullptr; // +0x0c record created by addChannels; setUnAdded clears it
    };
    Target** targetsBegin = nullptr;    // +0x04 vector begin
    Target** targetsEnd = nullptr;      // +0x08 vector end

    // 0x667588 _ZN6glitch7collada21IAnimationSetTemplateD1Ev
    ~IAnimationSetTemplate();
    // 0x66762c _ZN6glitch7collada21IAnimationSetTemplateD2Ev
    void destroyCompleteObject();
    // 0x667320 _ZN6glitch7collada21IAnimationSetTemplate10setUnAddedEv
    IAnimationSetTemplate& setUnAdded();
    // 0x667384 _ZN6glitch7collada21IAnimationSetTemplate11addChannelsEPSt6vectorIPKNS0_8SChannelENS_4core10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEEPS2_IPKNS0_17CAnimationTrackExENS7_ISF_LS9_0EEEE
    // The two vector arguments of the symbol are never read by the pseudocode, so they are unnamed.
    void addChannels(void*, void*);
};

// Stand-in for glitch::collada::IParticleSystemSceneNode (layout not in layout headers; no members).
class IParticleSystemSceneNode {
public:
    // 0x66777c _ZNK6glitch7collada24IParticleSystemSceneNode11getMaterialEj
    CMaterial* getMaterial(unsigned int index) const;
    // 0x66779c _ZNK6glitch7collada24IParticleSystemSceneNode16getMaterialCountEv
    int getMaterialCount() const;
    // 0x66781c _ZN6glitch7collada24IParticleSystemSceneNodeD1Ev
    ~IParticleSystemSceneNode();
    // 0x6678b8 _ZN6glitch7collada24IParticleSystemSceneNodeD2Ev
    void destroyAsSecondaryBase(int* vtt);
    // 0x66792c _ZN6glitch7collada24IParticleSystemSceneNodeC1ERKNS0_16CColladaDatabaseEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
    IParticleSystemSceneNode(const CColladaDatabase& database, std::vector<std::string>* names, CRootSceneNode* root);
    // 0x667a98 _ZN6glitch7collada24IParticleSystemSceneNodeC2ERKNS0_16CColladaDatabaseEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
    void constructAsSecondaryBase(int* vtt, const CColladaDatabase& database, std::vector<std::string>* names, CRootSceneNode* root);
};

// Stand-in for glitch::collada::ISceneNodeAnimator (layout not in layout headers; no members).
class ISceneNodeAnimator {
public:
    // 0x667d18 _ZN6glitch7collada18ISceneNodeAnimator11forceUnbindEPKc
    int forceUnbind(const char* name);
    // 0x667d9c _ZN6glitch7collada18ISceneNodeAnimator8onUnbindEPNS_5scene10ISceneNodeE
    int onUnbind(scene::ISceneNode* node);
    // 0x667e40 _ZN6glitch7collada18ISceneNodeAnimator14forceUnbindAllEv
    int forceUnbindAll();
    // 0x669538 _ZN6glitch7collada18ISceneNodeAnimator6onBindEPNS_5scene10ISceneNodeE
    int onBind(scene::ISceneNode* node);
    // 0x669738 _ZN6glitch7collada18ISceneNodeAnimatorD1Ev
    ~ISceneNodeAnimator();
    // 0x6697c4 _ZN6glitch7collada18ISceneNodeAnimatorD2Ev
    void destroyAsSecondaryBase(int* vtt);
    // 0x6699c4 _ZN6glitch7collada18ISceneNodeAnimator19hasCompatibleTargetERKNS0_8SChannelE
    int hasCompatibleTarget(const SChannel& channel);
};

// Stand-in for glitch::collada::ISkinnedMesh (layout not in layout headers; no members).
class ISkinnedMesh {
public:
    // 0x669bf4 _ZN6glitch7collada12ISkinnedMeshD1Ev
    ~ISkinnedMesh();
    // 0x669c28 _ZN6glitch7collada12ISkinnedMesh20releaseProcessBufferEPNS_5video12IVideoDriverEj
    int releaseProcessBuffer(IVideoDriver* driver, unsigned int index);
    // 0x669d6c _ZNK6glitch7collada12ISkinnedMesh12getTransformERKNS_4core8CMatrix4IfEE
    const CMatrix4* getTransform(const CMatrix4& matrix) const;
};

// animation_track::CMaterialApplicatorInfo::clone, as a free function over the layout struct.
// 0x667ed0 _ZNK6glitch7collada15animation_track23CMaterialApplicatorInfo5cloneEv
animation_track::CMaterialApplicatorInfo* cloneMaterialApplicatorInfo(const animation_track::CMaterialApplicatorInfo* self);

// 0x66a010 _ZNK6glitch7collada18SAnimationAccessor10applyValueEiiPvPNS0_15animation_track15CApplicatorInfoERib
int applyValue(const SAnimationAccessor& accessor, int a, int b, void* target, animation_track::CApplicatorInfo* applicator, int* out, bool flag);

}  // namespace glitch::collada::recon_22
