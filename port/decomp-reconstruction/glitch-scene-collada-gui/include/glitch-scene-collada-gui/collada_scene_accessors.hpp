// glitch::collada scene-node, mesh, animation-set and value-track accessors that
// read one member or forward to one other accessor. Each class maps only the
// members these accessors touch; the rest of the layout is a pad. Offsets are
// kept as comments, and the code uses typed members only.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/animation_set.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

namespace glitch::scene {
class ISceneNode;
class CMeshBuffer;
}

namespace glitch::video {
class CMaterial;
class CMaterialVertexAttributeMap;
}

namespace glitch {
class IReferenceCounted;
}

namespace glitch::video {
class CMaterial;
class CMaterialVertexAttributeMap;
}  // namespace glitch::video

namespace glitch::collada {

class IMesh;

// Uid holder: the first word of the object it points to is the uid string.
struct SUidHolder {
    const char* uid = nullptr;  // word 0
};

// Named mesh record; the uid word is at +0x08 in the original.
struct SNamedMeshData {
    std::uint32_t reserved[2] = {0, 0};  // +0x00..+0x07
    const char* uid = nullptr;           // +0x08
};

// One channel record of a dynamic animation set (16 bytes in the original).
struct SChannelRecord {
    std::uint32_t words[4] = {0, 0, 0, 0};
};

struct CColladaDatabase;
struct SAnimation;

class CDynamicAnimationSet {
public:
    // 0x62dba0 _ZN6glitch7collada20CDynamicAnimationSet10getChannelEj
    SChannelRecord* getChannel(unsigned index);
    // 0x62dbac _ZNK6glitch7collada20CDynamicAnimationSet10getChannelEj
    const SChannelRecord* getChannel(unsigned index) const;
    // 0x62fc58 _ZN6glitch7collada20CDynamicAnimationSet8clearSetEv
    // Partial: the database entries are dropped without releasing their references.
    void clearSet();
    // 0x62dbb8 _ZN6glitch7collada20CDynamicAnimationSet16getDatabaseIndexERNS0_16CColladaDatabaseE
    // Declared only: the database table element type (8 bytes) is not in the layout yet.
    int getDatabaseIndex(CColladaDatabase& database);
    // 0x62e8a8 _ZN6glitch7collada20CDynamicAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
    // Declared only: the base CAnimationSet::addAnimationLibrary is not reconstructed.
    int addAnimationLibrary(const CColladaDatabase& database);
    // 0x62ee9c _ZN6glitch7collada20CDynamicAnimationSet27addAnimationLibraryBindingsERKNS0_16CColladaDatabaseE
    // Declared only: needs the binding and int vector members.
    void addAnimationLibraryBindings(const CColladaDatabase& database);
    // 0x62f354 _ZN6glitch7collada20CDynamicAnimationSet12addAnimationEPKNS0_10SAnimationE
    // Declared only: needs the channel compatibility table and the track vector.
    int addAnimation(const SAnimation* animation);
    // 0x62f54c _ZN6glitch7collada20CDynamicAnimationSet11clearTracksEv
    // Declared only: needs the binding, int and channel vector members.
    void clearTracks();
    // 0x62fc90 _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryERKNS0_16CColladaDatabaseE
    // Declared only: needs the default CColladaDatabase value at +0x68.
    void setDefaultAnimationLibrary(const CColladaDatabase& database);
    // 0x62fcf8 _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryEj
    // Declared only: needs the default CColladaDatabase value at +0x68 and the database table.
    void setDefaultAnimationLibrary(unsigned index);

    std::vector<std::uint32_t> databases;  // +0x24 (begin) .. +0x28 (end); elements not mapped
    std::uint8_t pad_0030[0x40] = {};      // +0x30..+0x6f (not mapped)
    bool setCleared = false;               // +0x70 (set by clearSet)
    std::uint8_t pad_0071[0x03] = {};      // +0x71..+0x73 (not mapped)
    SChannelRecord* channels = nullptr;    // +0x74
};

class CCoronasSceneNode {
public:
    // 0x6e5a38 _ZNK6glitch7collada17CCoronasSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;

    core::aabbox3d<float> boundingBox;  // +0x150
};

class CMesh {
public:
    // 0x644b80 _ZNK6glitch7collada5CMesh14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x644b88 _ZN6glitch7collada5CMesh14setBoundingBoxERKNS_4core8aabbox3dIfEE
    void setBoundingBox(const core::aabbox3d<float>& box);

    core::aabbox3d<float> boundingBox;  // +0x24

    // glitch::collada::CMesh::SBuffer, 12 bytes (same shape as glitch::scene::CMesh::SBuffer in
    // layouts/scene.hpp). The first word is a reference-counted buffer; +0x04 and +0x08 are
    // intrusive_ptr handles copied with the buffer.
    struct SBuffer {
        // 0x644c88 _ZN6glitch7collada5CMesh7SBufferD1Ev
        // Partial: see the note on the definition in src/tb_scene-collada-gui-glitch_collada_CMesh_SBuffer-0.cpp.
        ~SBuffer();

        glitch::IReferenceCounted* buffer = nullptr;                                  // +0x00
        boost::intrusive_ptr<video::CMaterial> material;                              // +0x04
        boost::intrusive_ptr<video::CMaterialVertexAttributeMap> vertexAttributeMap;  // +0x08
    };

    // 0x644a44 _ZNK6glitch7collada5CMesh18getMeshBufferCountEv
    std::uint32_t getMeshBufferCount() const;
    // 0x644a6c _ZNK6glitch7collada5CMesh13getMeshBufferEj
    // PARTIAL: the reference bump on the buffer (+0x04) is not performed (see the .cpp).
    glitch::IReferenceCounted* getMeshBuffer(std::uint32_t index) const;
    // 0x644ac8 _ZNK6glitch7collada5CMesh11getMaterialEj
    // PARTIAL: the reference bump on the material (+0x00) is not performed.
    boost::intrusive_ptr<video::CMaterial> getMaterial(std::uint32_t index) const;
    // 0x644b24 _ZNK6glitch7collada5CMesh24getMaterialVertexAttributeMapEj
    // PARTIAL: the reference bump on the attribute map (+0x00) is not performed.
    boost::intrusive_ptr<video::CMaterialVertexAttributeMap> getMaterialVertexAttributeMap(
        std::uint32_t index) const;
    // 0x644bdc _ZN6glitch7collada5CMesh11setMaterialEjRKN5boost13intrusive_ptrINS_5video9CMaterialEEERKNS4_INS5_27CMaterialVertexAttributeMapEEE
    // PARTIAL: the reference counts of the new and the replaced material and map are not moved.
    void setMaterial(std::uint32_t index, const boost::intrusive_ptr<video::CMaterial>& material,
                     const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& vertexAttributeMap);

    std::vector<SBuffer> buffers;  // +0x18 (begin) .. +0x1c (end); 12-byte SBuffer stride in the original
};

class CMorphingMesh {
public:
    // 0x64990c _ZNK6glitch7collada13CMorphingMesh5cloneEv
    // Returns a null handle (the original stores 0 into the result slot).
    boost::intrusive_ptr<IMesh> clone() const;
};

class CModularSkinnedMeshSceneNode {
public:
    // 0x64943c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode22getCategoryModuleCountEi
    std::uint32_t getCategoryModuleCount(int category) const;
    // 0x649444 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode16getCategoryCountEv
    std::uint32_t getCategoryCount() const;
    // 0x64945c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode18getCurrentModuleIdEi
    std::int32_t getCurrentModuleId(int category) const;
    // The five accessors below are tail calls into the mesh (thunks in the original).
    // 0x64944c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode13getModuleNameEii
    const char* getModuleName(int category, int module) const;
    // 0x649454 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode20getCurrentModuleNameEi
    const char* getCurrentModuleName(int category) const;
    // 0x649464 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode11getModuleIdEPKc
    std::int32_t getModuleId(const char* name) const;
    // 0x64946c _ZNK6glitch7collada28CModularSkinnedMeshSceneNode15getCategoryNameEi
    const char* getCategoryName(int category) const;
    // 0x6494e0 _ZNK6glitch7collada28CModularSkinnedMeshSceneNode13getCategoryIdEPKc
    std::int32_t getCategoryId(const char* name) const;

    CModularSkinnedMesh* mesh = nullptr;  // +0x134
};

namespace animation_track {
class CApplicatorInfo;
}  // namespace animation_track

class CSceneNodeAnimatorSet {
public:
    // 0x662c90 _ZN6glitch7collada21CSceneNodeAnimatorSetC2Ev
    // Partial: the VTT-taking base constructor. The base ISceneNodeAnimator and the vtable stores are not mapped.
    CSceneNodeAnimatorSet();
    // 0x660c20 _ZN6glitch7collada21CSceneNodeAnimatorSetC1ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
    // 0x660ce4 _ZN6glitch7collada21CSceneNodeAnimatorSetC2ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
    explicit CSceneNodeAnimatorSet(const boost::intrusive_ptr<CAnimationSet>& set);
    // 0x65f0fc _ZN6glitch7collada21CSceneNodeAnimatorSet18getAnimationLengthEi
    std::uint32_t getAnimationLength(int index);
    // 0x65f104 _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationStartEi
    std::uint32_t getAnimationStart(int index);
    // 0x65f10c _ZN6glitch7collada21CSceneNodeAnimatorSet15getAnimationEndEi
    std::uint32_t getAnimationEnd(int index);
    // 0x65f17c _ZN6glitch7collada21CSceneNodeAnimatorSet14getTargetCountEv
    std::uint32_t getTargetCount();
    // 0x65f114 _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
    std::int32_t getCurrentAnimation() const;
    // 0x65f188 _ZN6glitch7collada21CSceneNodeAnimatorSet13getTargetSizeEi
    int getTargetSize(int index);
    // 0x65f1ac _ZN6glitch7collada21CSceneNodeAnimatorSet14getTargetsSizeEv
    int getTargetsSize();
    // 0x65f208 _ZN6glitch7collada21CSceneNodeAnimatorSet9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
    int setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator);
    // 0x65f270 _ZN6glitch7collada21CSceneNodeAnimatorSet11animateNodeEPNS_5scene10ISceneNodeEj
    void animateNode(glitch::scene::ISceneNode* node, unsigned int time);
    // 0x65f418 _ZN6glitch7collada21CSceneNodeAnimatorSet20applyAnimationValuesEj
    void applyAnimationValues(unsigned int time);
    // 0x65f5dc _ZN6glitch7collada21CSceneNodeAnimatorSet22computeAnimationValuesEj
    void computeAnimationValues(unsigned int time);
    // 0x65f7b4 _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationValueEiiPv
    void* getAnimationValue(int a, int b, void* out);
    // 0x65f8c8 _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
    glitch::IReferenceCounted* setCurrentAnimation(int index);
    // 0x660af4 _ZN6glitch7collada21CSceneNodeAnimatorSet4initERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
    void init(const boost::intrusive_ptr<CAnimationSet>& set);

    CAnimationSet* animationSet = nullptr;  // +0x24
    std::int32_t currentAnimation = 0;      // +0x50 index passed to CAnimationSet::getDatabase
};

// Scene-node property block (pointed to by +0x154 of the node). The uid string is the
// word at +0x00, the scope id the word at +0x08 and the user property word at +0x48.
struct SNodeUserProperties {
    const char* uid = nullptr;         // +0x00
    std::uint8_t pad_0004[0x04] = {};  // +0x04..+0x07 (not mapped)
    const char* scopeID = nullptr;     // +0x08
    std::uint8_t pad_0010[0x38] = {};  // +0x10..+0x47 (not mapped)
    std::int32_t value = 0;            // +0x48
};

class CSceneNode {
public:
    // 0x65cd70 _ZNK6glitch7collada10CSceneNode15getUserPropertyEv
    // Returns 0 when the node has no property block.
    std::int32_t getUserProperty() const;
    // 0x65cd28 _ZNK6glitch7collada10CSceneNode6getUIDEv (returns "" when there is no block)
    const char* getUID() const;
    // 0x65cd4c _ZNK6glitch7collada10CSceneNode10getScopeIDEv (returns "" when there is no block)
    const char* getScopeID() const;
    // 0x65cd80 _ZNK6glitch7collada10CSceneNode18getUserPropertyStrEv
    // PARTIAL: returns the uid word of the holder that the +0x48 word points to. The pointee cannot be
    // followed on host pointer width (see the .cpp); the body returns nullptr.
    const char* getUserPropertyStr() const;
    // 0x65cda8 _ZN6glitch7collada10CSceneNode18computeBoundingBoxEv
    // PARTIAL: child walk needs virtual slots and the intrusive child list (see the .cpp). Body is a no-op.
    void computeBoundingBox();
    // 0x65cf8c _ZNK6glitch7collada10CSceneNode18computeBoundingBoxERNS_4core8aabbox3dIfEE
    // PARTIAL: same child walk as above, recursive for node children. Body returns 0 (see the .cpp).
    int computeBoundingBox(core::aabbox3d<float>& box) const;

    core::aabbox3d<float> boundingBox;              // +0x130 (24 bytes, written by computeBoundingBox)
    SNodeUserProperties* userProperties = nullptr;  // +0x154
};

// Light scene node. The word at +0x160 is read two ways: getLightID returns it as a
// value and getUID follows it as a pointer to the uid word.
class CLightSceneNode {
public:
    // 0x644404 _ZNK6glitch7collada15CLightSceneNode10getLightIDEv
    std::uint32_t getLightID() const;
    // 0x64440c _ZNK6glitch7collada15CLightSceneNode6getUIDEv
    // Partial: the pointee type of +0x160 is inferred from the two accesses.
    std::uint32_t getUID() const;

    union {
        std::uint32_t lightID;           // +0x160 (value read by getLightID)
        const std::uint32_t* uidSource;  // +0x160 (pointer followed by getUID)
    } slot160{};
};

class CParticleSystemSceneNode {
public:
    // 0x64bb78 _ZNK6glitch7collada24CParticleSystemSceneNode6getUIDEv
    const char* getUID() const;
    // 0x64bbc0 _ZNK6glitch7collada24CParticleSystemSceneNode25getTransformedBoundingBoxEv
    const core::aabbox3d<float>* getTransformedBoundingBox() const;

    const core::aabbox3d<float>* transformedBoundingBox = nullptr;  // +0x14c
    SUidHolder* uidHolder = nullptr;                                // +0x18c
};

class CGlitchNewParticleSystemSceneNode {
public:
    // 0x637b3c _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode6getUIDEv
    const char* getUID() const;
    // 0x637b84 _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode25getTransformedBoundingBoxEv
    const core::aabbox3d<float>* getTransformedBoundingBox() const;

    const core::aabbox3d<float>* transformedBoundingBox = nullptr;  // +0x14c
    SUidHolder* uidHolder = nullptr;                                // +0x17c
};

class CCameraSceneNode {
public:
    // 0x6e5160 _ZNK6glitch7collada16CCameraSceneNode13getTargetNodeEv
    glitch::scene::ISceneNode* getTargetNode() const;
    // 0x6e5174 _ZNK6glitch7collada16CCameraSceneNode6getUIDEv
    const char* getUID() const;

    glitch::scene::ISceneNode* targetNode = nullptr;  // +0x390
    SUidHolder* uidHolder = nullptr;                  // +0x394
};

class CMeshSceneNode {
public:
    // 0x646148 _ZNK6glitch7collada14CMeshSceneNode6getUIDEv
    // The holder at +0x130 is used when set; otherwise the uid of the mesh
    // record at +0x134.
    const char* getUID() const;
    // 0x646730 _ZN6glitch7collada14CMeshSceneNode6renderEPv
    // The original returns the leftover r0 value; no caller uses it, so void here.
    void render(void* context);

    SUidHolder* uidHolder = nullptr;      // +0x130
    SNamedMeshData* namedMesh = nullptr;  // +0x134
};

namespace animation_track {

// Float-valued track: identity value is 0.0.
class CFloatEx {
public:
    // 0x6e3118 _ZNK6glitch7collada15animation_track8CFloatEx16getIdentityValueEPv
    void getIdentityValue(void* out) const;
    // 0x6e31e4 _ZN6glitch7collada15animation_track8CFloatExD1Ev (empty)
    ~CFloatEx() = default;
};

// Weight-valued track: identity value is 0.0.
class CWeightEx {
public:
    // 0x6e4dbc _ZNK6glitch7collada15animation_track9CWeightEx16getIdentityValueEPv
    void getIdentityValue(void* out) const;
    // 0x6e4f48 _ZN6glitch7collada15animation_track9CWeightExD1Ev (empty)
    ~CWeightEx() = default;
};

}  // namespace animation_track

}  // namespace glitch::collada
