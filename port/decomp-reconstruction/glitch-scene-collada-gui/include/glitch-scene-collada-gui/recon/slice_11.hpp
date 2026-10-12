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


// ---------------------------------------------------------------------------
// Slice functions (definitions in src/recon/slice_11.cpp). Comment line gives the address.
// ---------------------------------------------------------------------------
// CImage, reduced to the members its destructor touches. The vptr writes of the original
// are not modelled; the database base sits at +0x0c and the image reference at +0x14.
struct CImageStandIn {
    std::uint32_t vptr = 0;               // +0x00
    std::uint32_t reserved04[2] = {};     // +0x04..+0x0b
    CColladaDatabase database;            // +0x0c
    RefCountedStandIn* image = nullptr;   // +0x14
};

// 0x618e88 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getIdentityValueEPv
void* CVirtualEx_f4_mix2_getIdentityValue(int self, void* value);
// 0x618e98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getIdentityValueEPv
void* CVirtualEx_f4_mix3_getIdentityValue(int self, void* value);
// 0x618f18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getIdentityValueEPv
void* CVirtualEx_f4_mixm1_getIdentityValue(int self, void* value);
// 0x618ffc _ZN6glitch7collada15animation_track13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEE17applyAddedValueExEPvPfiSA_PNS1_15CApplicatorInfoE
int CApplyValueEx_f1_mixm1_applyAddedValueEx(const float* a1, const float* addend, int count, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x619088 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE15applyAddedValueEPvPfiSC_PNS1_15CApplicatorInfoE
int CVirtualEx_f1_mixm1_applyAddedValue(int self, const float* a2, const float* a3, int a4, MaterialParamsStandIn* a5, ApplicatorInfoStandIn* a6);
// 0x6190a4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_mix0_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x6190bc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_mix1_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x6190d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_mixm1_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x6190ec _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f2_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x61913c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f2_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x619150 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f3_mix0_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x619168 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f3_mixm1_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x619180 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f3_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x6191e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f3_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x6191f4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mixm1_applyBlendedValue(int self, const float* values, const float* weights, int count, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x6192f0 _ZN6glitch7collada16CColladaDatabaseC1EPKS1_PKcPNS0_15CColladaFactoryE
CColladaDatabase* CColladaDatabase_C1_fromSource(CColladaDatabase* self, const CColladaDatabase* source, const char* name, CColladaFactory* factory);
// 0x619344 _ZN6glitch7collada16CColladaDatabaseC2EPKS1_PKcPNS0_15CColladaFactoryE
CColladaDatabase* CColladaDatabase_C2_fromSource(CColladaDatabase* self, const CColladaDatabase* source, const char* name, CColladaFactory* factory);
// 0x619474 _ZN6glitch7collada16CColladaDatabaseD1Ev
void CColladaDatabase_D1(CColladaDatabase* self);
// 0x619510 _ZN6glitch7collada16CColladaDatabase17constructAnimatorEPNS_2io9IReadFileEbPNS0_15CColladaFactoryE
int CColladaDatabase_constructAnimator_file(CColladaDatabase* self, glitch::io::IReadFile* file, CColladaFactory* factory, CColladaFactory* unusedFactory);
// 0x6195c8 _ZN6glitch7collada16CColladaDatabase17constructAnimatorEPNS_2io9IReadFileEPNS0_15CColladaFactoryE
int CColladaDatabase_constructAnimator_fileOnly(CColladaDatabase* self, CColladaFactory* factory, CColladaFactory* unused);
// 0x61967c _ZN6glitch7collada16CColladaDatabase17constructAnimatorEPKcPNS0_15CColladaFactoryE
int CColladaDatabase_constructAnimator_name(CColladaDatabase* self, CColladaFactory* factory);
// 0x619728 _ZN6glitch7collada6CImageD1Ev
void CImage_D1(CImageStandIn* self);
// 0x619784 _ZN6glitch7collada16CColladaDatabaseD2Ev
void CColladaDatabase_D2(CColladaDatabase* self);
// 0x619978 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m3_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, U8* out);
// 0x619a64 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m3_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, U8* a7);
// 0x619a88 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m0_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out);
// 0x619b00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m0_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4);
// 0x619b10 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m0_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3, float t, U8* out);
// 0x619be4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m0_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, float a5, U8* a6);
// 0x619c00 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const U8* CInterpreter_u4_m0_getKeyBasedValueEx_diff(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out);
// 0x619c74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const U8* CVirtualEx_u4_m0_getKeyBasedValue_diff(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, U8* a5);
// 0x619c88 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m0_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, U8* out);
// 0x619d74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m0_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, U8* a7);
// 0x619d98 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m1_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out);
// 0x619e10 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m1_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4);
// 0x619e20 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m1_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3, float t, U8* out);
// 0x619f1c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m1_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, float a5, U8* a6);
// 0x619f38 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const U8* CInterpreter_u4_m1_getKeyBasedValueEx_diff(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out);
// 0x619fac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const U8* CVirtualEx_u4_m1_getKeyBasedValue_diff(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, U8* a5);
// 0x619fc0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m1_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, U8* out);
// 0x61a0ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m1_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, U8* a7);
// 0x61a0d0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m2_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out);
// 0x61a148 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m2_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4);
// 0x61a158 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m2_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3, float t, U8* out);
// 0x61a244 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m2_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, float a5, U8* a6);
// 0x61a260 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const U8* CInterpreter_u4_m2_getKeyBasedValueEx_diff(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out);
// 0x61a2d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const U8* CVirtualEx_u4_m2_getKeyBasedValue_diff(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, U8* a5);
// 0x61a2e8 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m2_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, U8* out);
// 0x61a3d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m2_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, U8* a7);
// 0x61a3fc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m3_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out);
// 0x61a474 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m3_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4);
// 0x61a484 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m3_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3, float t, U8* out);
// 0x61a558 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m3_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, float a5, U8* a6);
// 0x61a5d4 _ZNK6glitch7collada16CColladaDatabase20constructGNPSEmitterEPKcPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
int CColladaDatabase_constructGNPSEmitter(const CColladaDatabase& self, const char* name, video::IVideoDriver* driver, void* strings, CRootSceneNode* root);
// 0x61a750 _ZNK6glitch7collada16CColladaDatabase16constructCoronasEPKcPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
int CColladaDatabase_constructCoronas(const CColladaDatabase& self, const char* name, video::IVideoDriver* driver, CRootSceneNode* root);
// 0x61a854 _ZNK6glitch7collada16CColladaDatabase16constructEmitterEPKcPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
int CColladaDatabase_constructEmitter(const CColladaDatabase& self, const char* name, video::IVideoDriver* driver, void* strings, CRootSceneNode* root);
// 0x61aa00 _ZNK6glitch7collada16CColladaDatabase19constructControllerEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructController(OutSlotStandIn* out, const CColladaDatabase& self, video::IVideoDriver* driver, const char* name, CRootSceneNode* root);
// 0x61aae0 _ZNK6glitch7collada16CColladaDatabase17constructGeometryEPNS_5video12IVideoDriverEPKcS6_
OutSlotStandIn* CColladaDatabase_constructGeometry_file(OutSlotStandIn* out, const CColladaDatabase* source, const char* a3, const char* fileName, const char* geometryName);
// 0x61ac88 _ZNK6glitch7collada16CColladaDatabase11getMaterialEPKcS3_
SMaterial* CColladaDatabase_getMaterial_file(const CColladaDatabase* source, const char* name, const char* materialName);
// 0x61b1b4 _ZNK6glitch7collada16CColladaDatabase14constructImageEPKcPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructImage(OutSlotStandIn* out, const CColladaDatabase& self, const char* name);
// 0x61b8bc _ZNK6glitch7collada16CColladaDatabase20constructVisualSceneEPNS_5video12IVideoDriverEPNS0_12SVisualSceneEPNS0_14CRootSceneNodeE
CRootSceneNode* CColladaDatabase_constructVisualScene(const CColladaDatabase& self, video::IVideoDriver* driver, SVisualScene* entry, CRootSceneNode* root);
// 0x61b988 _ZNK6glitch7collada16CColladaDatabase20constructVisualSceneEPNS_5video12IVideoDriverEiPNS0_14CRootSceneNodeE
CRootSceneNode* CColladaDatabase_constructVisualScene_index(const CColladaDatabase& self, video::IVideoDriver* driver, int index, CRootSceneNode* root);
// 0x61b9b8 _ZNK6glitch7collada16CColladaDatabase20constructVisualSceneEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
CRootSceneNode* CColladaDatabase_constructVisualScene_name(const CColladaDatabase& self, video::IVideoDriver* driver, const char* name, CRootSceneNode* root);
// 0x61babc _ZNK6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEb
CRootSceneNode* CColladaDatabase_constructScene_bool(const CColladaDatabase& self, video::IVideoDriver* driver, bool animate);
// 0x61bb20 _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPKcPNS0_15CColladaFactoryE
int CColladaDatabase_constructScene_file(CColladaDatabase* self, video::IVideoDriver* driver, const char* name, CColladaFactory* factory);
// 0x61bbd4 _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPKcbPNS0_15CColladaFactoryE
int CColladaDatabase_constructScene_fileAnimated(CColladaDatabase* self, video::IVideoDriver* driver, const char* name, bool animate, CColladaFactory* factory);
// 0x61be14 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeE
CRootSceneNode* CColladaDatabase_constructNode_entry(const CColladaDatabase& self, video::IVideoDriver* driver, SNode* entry);
// 0x61c1fc _ZNK6glitch7collada16CColladaDatabase21getBlendableAnimationEPKNS0_10SAnimationE
int CColladaDatabase_getBlendableAnimation(const CColladaDatabase& self, const SAnimation* animation);
// 0x61c6ec _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKNS0_10SAnimationEPPv
char* CColladaDatabase_getDefaultValue(const CColladaDatabase& self, const SAnimation* animation, void** out);
// 0x61c6f4 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPKc
CRootSceneNode* CColladaDatabase_constructNode_name(const CColladaDatabase& self, video::IVideoDriver* driver, const char* name);
// 0x61c878 _ZN6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPKcS6_PNS0_15CColladaFactoryE
CRootSceneNode* CColladaDatabase_constructNode_file(const CColladaDatabase* source, video::IVideoDriver* driver, const char* name, const char* fileName, CColladaFactory* factory);
// 0x61c91c _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKcNS0_8SChannel4TypeES3_
const SAnimation* CColladaDatabase_getAnimation(const CColladaDatabase& self, const char* target, int type, const char* sid);
// 0x61cca8 _ZNK6glitch7collada16CColladaDatabase17constructMaterialEPNS_5video12IVideoDriverEPNS0_9SMaterialEPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructMaterial_entry(OutSlotStandIn* out, const CColladaDatabase* self, video::IVideoDriver* driver, SMaterial* material, CRootSceneNode* root);
// 0x61cefc _ZNK6glitch7collada16CColladaDatabase17constructMaterialEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructMaterial_name(OutSlotStandIn* out, const CColladaDatabase& self, video::IVideoDriver* driver, const char* name, CRootSceneNode* root);
// 0x61cf44 _ZNK6glitch7collada16CColladaDatabase17constructMaterialEPNS_5video12IVideoDriverEiPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructMaterial_index(OutSlotStandIn* out, const CColladaDatabase& self, video::IVideoDriver* driver, int index, CRootSceneNode* root);
// 0x61cb7c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatAngleMixC_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out);
// 0x61cbb8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatAngleMixS_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out);
// 0x61cc30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatMixC_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out);
// 0x61cc6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatMixS_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out);
// 0x61caa8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u3_mixm1_applyValue(int self, const U8* value, MaterialParamsStandIn* params);
// 0x61caf0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELin1EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u3_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x61cb68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u3_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x61c9b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix0_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x61c9d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix1_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x61c9e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix2_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x61ca00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix3_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x61ca18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mixm1_applyValue(int self, const float* value, MaterialParamsStandIn* params);
// 0x61ca30 _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f4_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x61ca94 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f4_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x61cf8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const SAnimationOutput* CVirtualEx_quatMixF_getKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key, float* out);
// 0x61cfd8 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float CInterpreterQuaternion_quatMixF_getKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int a2, int a3, float* out);
// 0x61d0ec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float CVirtualEx_quatMixF_getKeyBasedValue_ex(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, float* a5);
// 0x61d100 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreterQuaternion_quatMixF_getKeyBasedValueEx_t(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, float* out);
// 0x61d2a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_quatMixF_getKeyBasedValue_t(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, float* a7);
// 0x61d2cc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi3EfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_f4_m3_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, float* out);
// 0x61d3ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_f4_m3_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, float* a7);
// 0x61d3d0 _ZN6glitch7collada15animation_track13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f1_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int a2, MaterialParamsStandIn* params, ApplicatorInfoStandIn* info);
// 0x61d414 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f1_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int a3, MaterialParamsStandIn* a4, ApplicatorInfoStandIn* a5);
// 0x61d428 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi1EfEEfLi4ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_f4_m1_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, float* out);
// 0x61d504 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_f4_m1_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5, float a6, float* a7);

}  // namespace glitch::collada::recon_11
