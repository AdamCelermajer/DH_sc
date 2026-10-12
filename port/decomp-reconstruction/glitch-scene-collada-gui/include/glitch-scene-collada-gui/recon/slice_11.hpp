// Slice 11: animation_track value appliers and getKeyBasedValue interpolators for
// CApplyValueEx/CInterpreter instantiations (float, float[2..4], uchar[3..4], quaternion),
// the CVirtualEx forwarders, and the CColladaDatabase loader/constructor entries
// in the same address range.
//
// Stand-ins below cover code outside this slice (material parameter sink, file
// manager, reference counting, scene node and factory vtable calls, unmapped
// CColladaDatabase overloads, quaternion library math). Rows that depend on a
// stand-in are ledgered partial with the stand-in named in the note.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/collada_database.hpp"

namespace glitch::io {
class IReadFile;
}

namespace glitch::collada {
class CResFile;
struct SNode;  // COLLADA node record; its loader struct is not mapped in this slice.
}  // namespace glitch::collada

namespace glitch::collada::recon_11 {

// ---------------------------------------------------------------------------
// Stand-in: material parameter sink (IMaterialParameters::setParameterCvt<T>).
// The parameter table is not reconstructed here, so the write is a no-op.
// ---------------------------------------------------------------------------
struct MaterialParamsStandIn {};

// CApplicatorInfo, reduced to the parameter id the appliers read (u16 at +0x08).
struct ApplicatorInfoStandIn {
    std::uint8_t reserved00[8] = {};
    std::uint16_t paramId = 0;  // +0x08
};

// Value tags for setParameterCvt<T>, named after the glitch::core / glitch::video types.
struct CoreVector2dF {};
struct CoreVector3dF {};
struct CoreVector4dF {};
struct VideoSColor {};
struct CoreQuaternionF {};

template <typename T>
int setParameterCvt(MaterialParamsStandIn*, std::uint16_t, int, const void*) {
    return 0;
}

// ---------------------------------------------------------------------------
// SAnimationAccessor, reduced to what the getKeyBasedValue paths touch. The output
// record's data pointer is at +0x04; the default value is an optional pointer.
// ---------------------------------------------------------------------------
struct SAnimationOutput {
    std::uint32_t reserved00 = 0;
    const void* data = nullptr;  // +0x04
};

struct SAnimationAccessorStandIn {
    const SAnimationOutput* output = nullptr;
    const void* defaultValue = nullptr;

    const SAnimationOutput* getOutput(int) const noexcept { return output; }
    bool hasDefaultValue() const noexcept { return defaultValue != nullptr; }
    const void* getDefaultValue() const noexcept { return defaultValue; }
};

// Scene node as seen by the quaternion retrieveValue paths. The original calls the
// virtual at vtable +0x98 (slot 38); the slot is named here by its result.
class ISceneNodeStandIn {
public:
    virtual ~ISceneNodeStandIn() = default;
    virtual const std::uint32_t* getRotationWords() const = 0;
};

// Reference-counted object (IReferenceCounted): vptr at +0x00, count at +0x04.
struct RefCountedStandIn {
    std::uint32_t vptr = 0;
    std::uint32_t refCount = 0;
};

// Out-parameter slot for the functions the original returns through a hidden
// reference (intrusive_ptr-style result). Holds the object pointer only.
struct OutSlotStandIn {
    RefCountedStandIn* object = nullptr;
};

// ---------------------------------------------------------------------------
// Stand-ins: reference counting and the resource manager (CResFileManager).
// ---------------------------------------------------------------------------
// IReferenceCounted::drop on a COLLADA record; the record count is at +0x04.
inline void dropRecord(SColladaRecord* record) noexcept {
    if (record != nullptr && record->reserved00[1] != 0) {
        --record->reserved00[1];
    }
}

// CResFileManager::Inst, reduced to the flag at +0x28 that the database code toggles.
struct ResFileManagerStandIn {
    std::uint8_t reserved00[40] = {};
    bool streamingEnabled = false;  // +0x28
};
ResFileManagerStandIn& resFileManagerInst() noexcept;

// Stand-in: CResFileManager::get / load / unload, CAnimationStreamingManager::release.
// Return no file and do nothing; callers keep their original control flow.
SColladaRecord* resFileManagerGet(SColladaRecord* file, const char* name, int flag) noexcept;
SColladaRecord* resFileManagerLoad(const CColladaDatabase* self, const char* name, glitch::io::IReadFile* file) noexcept;
void resFileManagerUnload(const char* name, int flag) noexcept;
void animationStreamingRelease(const CColladaDatabase* self) noexcept;
void logMessage(const char* message) noexcept;  // glitch::os::Printer::log stand-in

// CColladaDatabase::DefaultFactory.
extern CColladaFactory* defaultFactory;

// ---------------------------------------------------------------------------
// Stand-ins: CColladaDatabase overloads whose declarations are not in
// collada_database.hpp, the factory vtable calls, and the quaternion library math.
// ---------------------------------------------------------------------------
int constructAnimatorFromDatabase(CColladaDatabase* database) noexcept;
int constructGNPSEmitterEntry(const CColladaDatabase& db, SGNPSEmitter* entry, video::IVideoDriver* driver,
                              void* strings, CRootSceneNode* root) noexcept;
int constructCoronasEntry(const CColladaDatabase& db, SCoronas* entry, video::IVideoDriver* driver,
                          CRootSceneNode* root) noexcept;
int constructEmitterEntry(const CColladaDatabase& db, SEmitter* entry, video::IVideoDriver* driver,
                          void* strings, CRootSceneNode* root) noexcept;
OutSlotStandIn* constructControllerEntry(OutSlotStandIn* out, const CColladaDatabase& db, video::IVideoDriver* driver,
                                         const char* name, SController* entry, CRootSceneNode* root) noexcept;
OutSlotStandIn* constructImageEntry(OutSlotStandIn* out, const CColladaDatabase& db, SImage* entry) noexcept;
CRootSceneNode* createRootSceneNode(CColladaFactory* factory, const CColladaDatabase& db) noexcept;
CRootSceneNode* constructNodeEntry(const CColladaDatabase& db, video::IVideoDriver* driver, SNode* entry,
                                   CRootSceneNode* root) noexcept;
CRootSceneNode* constructNodeByName(const CColladaDatabase& db, video::IVideoDriver* driver, const char* name,
                                    CRootSceneNode* root) noexcept;
CRootSceneNode* constructSceneEntry(const CColladaDatabase& db, video::IVideoDriver* driver) noexcept;
OutSlotStandIn* constructGeometryEntry(OutSlotStandIn* out, const CColladaDatabase& db, const char* a3,
                                       const char* a5) noexcept;
OutSlotStandIn* constructMaterialEntry(OutSlotStandIn* out, const CColladaDatabase& db, video::IVideoDriver* driver,
                                       SMaterial* material, CRootSceneNode* root) noexcept;
SNode* findNode(const CColladaDatabase& db, const char* name) noexcept;
int blendableOfAnimation(const CColladaDatabase& db, const SAnimation* animation) noexcept;
char* defaultValueOfAnimation(const CColladaDatabase& db, const SAnimation* animation, void** out) noexcept;

// Quaternion library math (glitch::core::quaternion::operator* and ::slerp).
void quaternionMultiply(float out[4], const float lhs[4], const float rhs[4]) noexcept;
void quaternionSlerp(float out[4], const float from[4], const float to[4], float t) noexcept;

}  // namespace glitch::collada::recon_11
