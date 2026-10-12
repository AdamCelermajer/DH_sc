// glitch::collada::CColladaDatabase named-entry lookups. The original keeps one
// table per asset kind inside a record reached through two pointers
// (*this + 0x24, then +0x20). Each table is a count followed by a base pointer
// to fixed-size entries whose first word is the name. Here each table is a
// vector of entry pointers, so no stride arithmetic is needed.
//
// Entry types below are stand-ins: only the name (and, for SAnimation, the
// fields the lookups touch) is mapped. The full structs belong to the
// database loader, which is not reconstructed yet.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch::res {
class onDemandReader;  // layout not mapped in this category
}

namespace glitch {
class IReferenceCounted;
}  // namespace glitch

namespace glitch::collada {

class CColladaFactory;
class CRootSceneNode;
class CSkinnedMesh;
class CMorphingMesh;
class CModularSkinnedMesh;
class CCameraSceneNode;
class CLightSceneNode;
class IMesh;

// Stand-in entry types. `name` is the first word of each original entry,
// except SController where the name is at +0x04.
struct SAnimation { const char* name; /* stride 0x20 in the original */ };
struct SAnimationClip { const char* name; /* stride 0x0c */ };
struct SImage { const char* name; /* stride 0x14 */ };
struct SEffect { const char* name; /* stride 0x74 */ };
struct SMaterial { const char* name; /* stride 0x24 */ };
struct SGeometry {
    const char* name;                 // +0x00, stride 0x10
    std::uint32_t reserved04;         // +0x04
    std::uint32_t notInstantiated;    // +0x08, non-zero makes constructGeometry return null
    std::uint32_t reserved0c;         // +0x0c
};
struct SController { std::uint32_t reserved; const char* name; /* +0x04, stride 0x0c */ };
struct SLight { const char* name; /* stride 0x18 */ };
struct SCamera { const char* name; /* stride 0x1c */ };
struct SVisualScene { const char* name; /* stride 0x10 */ };
struct SForce {
    const char* name;            // +0x00, stride 0x10
    std::uint32_t reserved04;    // +0x04
    std::uint32_t kind;          // +0x08: 0 gravity, 1 wind, 2 deflector
    std::uint32_t reserved0c;    // +0x0c
};
struct SCoronas { const char* name; /* stride 0x24 */ };
struct SGNPSEmitter { const char* name; /* stride 0xe8 */ };
struct SEmitter { const char* name; /* stride 0x90 */ };

// Instance records name their target. The instance URL is "#<id>"; the
// original skips the leading '#' before looking the target up.
struct SInstanceForce {
    std::uint32_t reserved00;    // +0x00
    const char* url;             // +0x04, "#<name>"
};
struct SInstanceModularSkin { std::uint32_t reserved[1]; };  // opaque here

// Segment entry: 0x18 bytes, range [begin, end). Used by getAnimationSegment.
struct SAnimationSegment {
    std::int32_t begin;
    std::int32_t end;
    std::uint32_t reserved[4];

    // 0x60bee4 _ZN6glitch7collada17SAnimationSegment7getDataERNS_3res14onDemandReaderE
    // Declared only (partial). The body needs glitch::res::onDemandReader and
    // onDemand<SAnimationData>::get, neither of which is mapped here.
    SAnimationSegment* getData(glitch::res::onDemandReader& reader, int a3, int a4);
};

// SInstanceMaterial, reduced to the intrusive link that linkInstanceMaterial sets.
struct SInstanceMaterial {
    SInstanceMaterial* linkNext = nullptr;  // +0x38
};

// Table holder: the record the original reaches as *(*this + 0x24) + 0x20.
// The comments give the original count and base offsets of each table.
struct SDatabaseTables {
    std::uint32_t version = 0;                    // +0x00 read by getVersion()
    std::vector<SAnimation*> animations;          // +0x24 / +0x28
    std::vector<SAnimationClip*> animationClips;  // +0x34 / +0x38
    std::vector<SImage*> images;                  // +0x4c / +0x50
    std::vector<SEffect*> effects;                // +0x54 / +0x58
    std::vector<SMaterial*> materials;            // +0x5c / +0x60
    std::vector<SGeometry*> geometries;           // +0x68 / +0x6c
    std::vector<SController*> controllers;        // +0x70 / +0x74
    std::vector<SLight*> lights;                  // +0x44 / +0x48
    std::vector<SCamera*> cameras;                // +0x3c / +0x40
    std::vector<SVisualScene*> visualScenes;      // +0x98 / +0x9c
    std::vector<SForce*> forces;                  // +0x88 / +0x8c
    std::vector<SCoronas*> coronas;               // +0x90 / +0x94
    std::vector<SGNPSEmitter*> gnpsEmitters;      // +0x80 / +0x84
    std::vector<SEmitter*> emitters;              // +0x78 / +0x7c
    std::vector<SAnimationSegment> segments;      // segment record at +0x30 of the holder
    SInstanceMaterial* instanceMaterials = nullptr;  // +0x08, head of the link list
};

// Mask bits accepted by find(); the original tries them in this order.
enum : unsigned {
    kFindAnimation = 0x01,
    kFindAnimationClip = 0x02,
    kFindImage = 0x04,
    kFindEffect = 0x08,
    kFindMaterial = 0x10,
    kFindGeometry = 0x20,
    kFindController = 0x40,
    kFindVisualScene = 0x80,
};

// 0x61be98 _ZNK6glitch7collada16CColladaDatabase16getAnimationClipEPKc
SAnimationClip* getAnimationClip(const SDatabaseTables& tables, const char* name);
// 0x61bef8 _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKc
SAnimation* getAnimation(const SDatabaseTables& tables, const char* name);
// 0x61a574 _ZNK6glitch7collada16CColladaDatabase14getGNPSEmitterEPKc
SGNPSEmitter* getGNPSEmitter(const SDatabaseTables& tables, const char* name);
// 0x61a6f0 _ZNK6glitch7collada16CColladaDatabase10getCoronasEPKc
SCoronas* getCoronas(const SDatabaseTables& tables, const char* name);
// 0x61a7f4 _ZNK6glitch7collada16CColladaDatabase10getEmitterEPKc
SEmitter* getEmitter(const SDatabaseTables& tables, const char* name);
// 0x61a690 _ZNK6glitch7collada16CColladaDatabase14getVisualSceneEPKc
SVisualScene* getVisualScene(const SDatabaseTables& tables, const char* name);
// 0x61a910 _ZNK6glitch7collada16CColladaDatabase8getForceEPKc
SForce* getForce(const SDatabaseTables& tables, const char* name);
// 0x61a9a0 _ZNK6glitch7collada16CColladaDatabase13getControllerEPKc
SController* getController(const SDatabaseTables& tables, const char* name);
// 0x61aa48 _ZNK6glitch7collada16CColladaDatabase11getGeometryEPKc
SGeometry* getGeometry(const SDatabaseTables& tables, const char* name);
// 0x61ac28 _ZNK6glitch7collada16CColladaDatabase11getMaterialEPKc
SMaterial* getMaterial(const SDatabaseTables& tables, const char* name);
// 0x61b0ac _ZNK6glitch7collada16CColladaDatabase9getEffectEPKc
SEffect* getEffect(const SDatabaseTables& tables, const char* name);
// 0x61b154 _ZNK6glitch7collada16CColladaDatabase8getImageEPKc
SImage* getImage(const SDatabaseTables& tables, const char* name);
// 0x61b1ec _ZNK6glitch7collada16CColladaDatabase8getLightEPKc
SLight* getLight(const SDatabaseTables& tables, const char* name);
// 0x61b270 _ZNK6glitch7collada16CColladaDatabase9getCameraEPKc
SCamera* getCamera(const SDatabaseTables& tables, const char* name);

// 0x61bf58 _ZNK6glitch7collada16CColladaDatabase4findEPKcRj
// Tries the kinds selected in `mask` in order; on a hit, `mask` is set to the
// kind that matched and the entry is returned (as void*, the kind is in mask).
const void* find(const SDatabaseTables& tables, const char* name, unsigned& mask);

// 0x60e2bc _ZNK6glitch7collada16CColladaDatabase19getAnimationSegmentEi
// Returns the segment whose [begin, end) contains `frame`; otherwise the last
// segment. Null when the table is empty.
const SAnimationSegment* getAnimationSegment(const SDatabaseTables& tables, int frame);

// 0x60e28c _ZNK6glitch7collada16CColladaDatabase20linkInstanceMaterialEPNS0_17SInstanceMaterialE
void linkInstanceMaterial(SDatabaseTables& tables, SInstanceMaterial* instance);


// The loaded COLLADA file that a CColladaDatabase holds (+0x00, intrusive count
// at +0x04). Only the fields the lookups and constructors read are mapped.
struct SColladaFileData;
struct SColladaRecord {
    std::uint32_t reserved00[8];       // +0x00..+0x1c (refcount is +0x04)
    const char* fileName = nullptr;    // +0x20
    SColladaFileData* data = nullptr;  // +0x24
};

// Per-file data block reached through SColladaRecord::data.
struct SColladaFileData {
    std::uint32_t reserved00[8];       // +0x00..+0x1c
    SDatabaseTables* tables = nullptr; // +0x20, the table holder
};

// glitch::collada::CColladaDatabase. The original's two pointer words are
// mapped as `record` (+0x00) and `factory` (+0x04); the table holder is reached
// as record->data->tables (original *(*this + 0x24) + 0x20).
//
// Member functions below are the ones reconstructed so far; each comment gives
// the original address. Overloads taking an index use the unchecked
// base + index * stride reads of the original.
class CColladaDatabase {
public:
    SColladaRecord* record = nullptr;   // +0x00
    CColladaFactory* factory = nullptr; // +0x04

    const SDatabaseTables& tables() const { return *record->data->tables; }
    const char* fileName() const { return record ? record->fileName : nullptr; }

    // 0x60e2a8 getVersion() const
    std::uint32_t getVersion() const;
    // 0x60e334 getAnimationClipLibrary() const
    void* getAnimationClipLibrary() const;
    // 0x60e348 getScene() const
    void* getScene() const;
    // 0x60fa24 constructController(IVideoDriver*, SController*, CRootSceneNode*) const
    // The original returns through a hidden out slot (intrusive_ptr); the out slot is the first argument here.
    glitch::IReferenceCounted** constructController(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                    SController* controller, CRootSceneNode* root) const;
    // 0x60fb54 constructAnimator() const
    void* constructAnimator() const;

    // 0x60e4bc getExternalLightSceneNode(intrusive_ptr<CMaterial const> const&, unsigned short, unsigned int, char const*) const
    void getExternalLightSceneNode(const boost::intrusive_ptr<video::CMaterial const>& material, std::uint16_t a,
                                   std::uint32_t b, const char* name) const;
    // 0x60e504 getExternalLightSceneNode(intrusive_ptr<CMaterialRenderer const> const&, unsigned short, unsigned int, char const*) const
    void getExternalLightSceneNode(const boost::intrusive_ptr<video::CMaterialRenderer const>& renderer,
                                   std::uint16_t a, std::uint32_t b, const char* name) const;

    // 0x60e570 constructEffect(IVideoDriver*, SEffect*, CRootSceneNode*) const
    video::CMaterialRenderer* constructEffect(video::IVideoDriver* driver, SEffect* effect,
                                              CRootSceneNode* root) const;
    // 0x60e5e4 constructEffect(IVideoDriver*, int, CRootSceneNode*) const
    video::CMaterialRenderer* constructEffect(video::IVideoDriver* driver, int index, CRootSceneNode* root) const;
    // 0x61b10c constructEffect(IVideoDriver*, char const*, CRootSceneNode*) const
    video::CMaterialRenderer* constructEffect(video::IVideoDriver* driver, const char* name,
                                              CRootSceneNode* root) const;

    // 0x60e634 constructGeometry(IVideoDriver*, SGeometry*) const
    IMesh* constructGeometry(video::IVideoDriver* driver, SGeometry* geometry) const;
    // 0x60e6b8 constructGeometry(IVideoDriver*, int) const
    IMesh* constructGeometry(video::IVideoDriver* driver, int index) const;
    // 0x61aaa8 constructGeometry(IVideoDriver*, char const*) const
    IMesh* constructGeometry(video::IVideoDriver* driver, const char* name) const;

    // 0x60e6f0 constructModularSkin(SInstanceModularSkin*, CRootSceneNode*) const
    CModularSkinnedMesh* constructModularSkin(SInstanceModularSkin* instance, CRootSceneNode* root) const;

    // 0x60e88c constructForce(SForce*, CRootSceneNode*) const
    void* constructForce(SForce* force, CRootSceneNode* root) const;
    // 0x60e910 constructForce(int, CRootSceneNode*) const
    void* constructForce(int index, CRootSceneNode* root) const;
    // 0x61a970 constructForce(char const*, CRootSceneNode*) const
    void constructForce(const char* name, CRootSceneNode* root) const;
    // 0x61a994 constructForce(SInstanceForce*, CRootSceneNode*) const
    void constructForce(SInstanceForce* instance, CRootSceneNode* root) const;

    // 0x60f9a4 constructMorph(IVideoDriver*, SController*, CRootSceneNode*) const
    CMorphingMesh* constructMorph(video::IVideoDriver* driver, SController* controller, CRootSceneNode* root) const;

    // 0x619398 constructCamera(SCamera*, CRootSceneNode*) const
    CCameraSceneNode* constructCamera(SCamera* camera, CRootSceneNode* root) const;
    // 0x6193e0 constructCamera(int, CRootSceneNode*) const
    CCameraSceneNode* constructCamera(int index, CRootSceneNode* root) const;
    // 0x61b2d0 constructCamera(char const*, CRootSceneNode*) const
    void constructCamera(const char* name, CRootSceneNode* root) const;

    // 0x619404 constructLight(SLight*, CRootSceneNode*) const
    void* constructLight(SLight* light, CRootSceneNode* root) const;
    // 0x619450 constructLight(int, CRootSceneNode*) const
    void* constructLight(int index, CRootSceneNode* root) const;
    // 0x61b24c constructLight(char const*, CRootSceneNode*) const
    void constructLight(const char* name, CRootSceneNode* root) const;
};

// Index getters of the original (0x60e390..0x60e450 family): base + index * stride.
// They are unchecked, as in the original.
SEffect* effectAt(const SDatabaseTables& tables, int index);
SGeometry* geometryAt(const SDatabaseTables& tables, int index);
SForce* forceAt(const SDatabaseTables& tables, int index);
SLight* lightAt(const SDatabaseTables& tables, int index);
SCamera* cameraAt(const SDatabaseTables& tables, int index);
// Index getters of the remaining CColladaDatabase::getX(int) members (see collada_database_create.cpp for
// the members that are already written). Kept as free helpers: a member named getX would hide the free
// getX(tables, name) lookups that the constructX members call unqualified.
SAnimation* animationAt(const SDatabaseTables& tables, int index);        // 0x60e35c
SAnimationClip* animationClipAt(const SDatabaseTables& tables, int index); // 0x60e374
SImage* imageAt(const SDatabaseTables& tables, int index);                 // 0x60e3c8
SMaterial* materialAt(const SDatabaseTables& tables, int index);           // 0x60e400
SController* controllerAt(const SDatabaseTables& tables, int index);       // 0x60e434
SEmitter* emitterAt(const SDatabaseTables& tables, int index);             // 0x60e468
SGNPSEmitter* gnpsEmitterAt(const SDatabaseTables& tables, int index);     // 0x60e484
SCoronas* coronasAt(const SDatabaseTables& tables, int index);             // 0x60e4a0
SVisualScene* visualSceneAt(const SDatabaseTables& tables, int index);     // 0x60e54c

}  // namespace glitch::collada

