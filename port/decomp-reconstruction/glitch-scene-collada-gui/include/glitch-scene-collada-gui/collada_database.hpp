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

namespace glitch::collada {

struct CColladaDatabase;
struct CColladaFactory;

// Stand-in entry types. `name` is the first word of each original entry,
// except SController where the name is at +0x04.
struct SAnimation { const char* name; /* stride 0x20 in the original */ };
struct SAnimationClip { const char* name; /* stride 0x0c */ };
struct SImage { const char* name; /* stride 0x14 */ };
struct SEffect { const char* name; /* stride 0x74 */ };
struct SMaterial { const char* name; /* stride 0x24 */ };
struct SGeometry { const char* name; /* stride 0x10 */ };
struct SController { std::uint32_t reserved; const char* name; /* +0x04, stride 0x0c */ };
struct SLight { const char* name; /* stride 0x18 */ };
struct SCamera { const char* name; /* stride 0x1c */ };
struct SVisualScene { const char* name; /* stride 0x10 */ };
struct SForce { const char* name; /* stride 0x10 */ };
struct SCoronas { const char* name; /* stride 0x24 */ };
struct SGNPSEmitter { const char* name; /* stride 0xe8 */ };
struct SEmitter { const char* name; /* stride 0x90 */ };

// Segment entry: 0x18 bytes, range [begin, end). Used by getAnimationSegment.
struct SAnimationSegment {
    std::int32_t begin;
    std::int32_t end;
    std::uint32_t reserved[4];
};

// SInstanceMaterial, reduced to the intrusive link that linkInstanceMaterial sets.
struct SInstanceMaterial {
    SInstanceMaterial* linkNext = nullptr;  // +0x38
};

// Table holder: the record the original reaches as *(*this + 0x24) + 0x20.
// The comments give the original count and base offsets of each table.
struct SDatabaseTables {
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

}  // namespace glitch::collada
