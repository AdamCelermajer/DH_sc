// Slice 11 reconstruction. See include/glitch-scene-collada-gui/recon/slice_11.hpp for the
// stand-ins. Index arguments follow the original: for uchar[4] and float[4] paths the
// key arguments are element offsets into the flat data array, as the pseudocode computes
// them; for the float[2], float[3], uchar[3] and quaternion paths the key is a key index.

#include "glitch-scene-collada-gui/recon/slice_11.hpp"

#include <cstring>

namespace glitch::collada::recon_11 {

// ---------------------------------------------------------------------------
// Stand-in definitions (outside this slice).
// ---------------------------------------------------------------------------
namespace {
ResFileManagerStandIn g_resFileManagerInst;
}  // namespace

ResFileManagerStandIn& resFileManagerInst() noexcept { return g_resFileManagerInst; }
CColladaFactory* defaultFactory = nullptr;

SColladaRecord* resFileManagerGet(SColladaRecord*, const char*, int) noexcept { return nullptr; }
SColladaRecord* resFileManagerLoad(const CColladaDatabase*, const char*, glitch::io::IReadFile*) noexcept {
    return nullptr;
}
void resFileManagerUnload(const char*, int) noexcept {}
void animationStreamingRelease(const CColladaDatabase*) noexcept {}
void logMessage(const char*) noexcept {}

int constructAnimatorFromDatabase(CColladaDatabase*) noexcept { return 0; }
int constructGNPSEmitterEntry(const CColladaDatabase&, SGNPSEmitter*, video::IVideoDriver*, void*,
                              CRootSceneNode*) noexcept {
    return 0;
}
int constructCoronasEntry(const CColladaDatabase&, SCoronas*, video::IVideoDriver*, CRootSceneNode*) noexcept {
    return 0;
}
int constructEmitterEntry(const CColladaDatabase&, SEmitter*, video::IVideoDriver*, void*, CRootSceneNode*) noexcept {
    return 0;
}
OutSlotStandIn* constructControllerEntry(OutSlotStandIn* out, const CColladaDatabase&, video::IVideoDriver*,
                                         const char*, SController*, CRootSceneNode*) noexcept {
    return out;
}
OutSlotStandIn* constructImageEntry(OutSlotStandIn* out, const CColladaDatabase&, SImage*) noexcept { return out; }
CRootSceneNode* createRootSceneNode(CColladaFactory*, const CColladaDatabase&) noexcept { return nullptr; }
CRootSceneNode* constructNodeEntry(const CColladaDatabase&, video::IVideoDriver*, SNode*, CRootSceneNode*) noexcept {
    return nullptr;
}
CRootSceneNode* constructNodeByName(const CColladaDatabase&, video::IVideoDriver*, const char*,
                                    CRootSceneNode*) noexcept {
    return nullptr;
}
CRootSceneNode* constructSceneEntry(const CColladaDatabase&, video::IVideoDriver*) noexcept { return nullptr; }
OutSlotStandIn* constructGeometryEntry(OutSlotStandIn* out, const CColladaDatabase&, const char*,
                                       const char*) noexcept {
    return out;
}
OutSlotStandIn* constructMaterialEntry(OutSlotStandIn* out, const CColladaDatabase&, video::IVideoDriver*,
                                       SMaterial*, CRootSceneNode*) noexcept {
    return out;
}
SNode* findNode(const CColladaDatabase&, const char*) noexcept { return nullptr; }
int blendableOfAnimation(const CColladaDatabase&, const SAnimation*) noexcept { return 0; }
char* defaultValueOfAnimation(const CColladaDatabase&, const SAnimation*, void**) noexcept { return nullptr; }

void quaternionMultiply(float out[4], const float lhs[4], const float rhs[4]) noexcept {
    // Stand-in for glitch::core::quaternion::operator*; the operands are passed through unchanged.
    (void)lhs;
    (void)rhs;
    for (int i = 0; i < 4; ++i) out[i] = 0.0f;
}
void quaternionSlerp(float out[4], const float from[4], const float to[4], float t) noexcept {
    // Stand-in for glitch::core::quaternion::slerp.
    (void)to;
    (void)t;
    for (int i = 0; i < 4; ++i) out[i] = from[i];
}

namespace {

using U8 = std::uint8_t;

// ---------------------------------------------------------------------------
// Shared helpers for the interpolators.
// ---------------------------------------------------------------------------
const U8* bytesOf(const SAnimationAccessorStandIn& acc) noexcept {
    return static_cast<const U8*>(acc.getOutput(0)->data);
}
const float* floatsOf(const SAnimationAccessorStandIn& acc) noexcept {
    return static_cast<const float*>(acc.getOutput(0)->data);
}

// Default branch of the uchar[4] interpreters: the lane `lane` takes the computed value,
// the other lanes take the default value.
void writeLanes4(U8* out, int lane, U8 value, const U8* def) noexcept {
    for (int j = 0; j < 4; ++j) {
        out[j] = (j == lane) ? value : def[j];
    }
}

// Same for the float[4] interpolators.
void writeLanesF4(float* out, int lane, float value, const float* def) noexcept {
    for (int j = 0; j < 4; ++j) {
        out[j] = (j == lane) ? value : def[j];
    }
}

// uchar[4] interpolation with two keys and t: base + t * (b - base), in float.
unsigned int interp1U4Value(const U8* d, int a2, int a3, float t) noexcept {
    return static_cast<unsigned int>(static_cast<float>(d[a2]) +
                                     t * static_cast<float>(d[a3] - d[a2]));
}

// uchar[4] interpolation with three keys: the two deltas are stored as bytes (wrapping),
// then lerped in float.
unsigned int interp2U4Value(const U8* d, int a2, int a3, int a4, float t) noexcept {
    const int base = d[a2];
    const U8 v12 = static_cast<U8>(d[a4] - base);
    const U8 v13 = static_cast<U8>(d[a3] - base);
    return static_cast<unsigned int>(static_cast<float>(v13) +
                                     t * static_cast<float>(v12 - v13));
}

// Quaternion helpers. The original negates the x, y and z words of a quaternion by
// adding 0x80000000 to their bit patterns, which is a sign flip.
void conjugateXyz(float q[4]) noexcept {
    q[0] = -q[0];
    q[1] = -q[1];
    q[2] = -q[2];
}

// Plain copies of the key quaternions (four floats per key).
void loadQuatKey(const float* data, int key, float out[4]) noexcept {
    for (int i = 0; i < 4; ++i) out[i] = data[4 * key + i];
}

// Float interpolators with three keys (base, a3, a4) and t, lane K takes the value.
template <int Lane>
float interp2F4(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, float* out) noexcept {
    const float* f = floatsOf(acc);
    const float v11 = f[a2];
    const float v12 = f[a3] - v11;
    const float v13 = f[a4] - v11;
    const float result = v12 + t * (v13 - v12);
    if (acc.hasDefaultValue()) {
        writeLanesF4(out, Lane, result, static_cast<const float*>(acc.getDefaultValue()));
    } else {
        out[0] = result;
    }
    return result;
}

// uchar[4] interpolators, one per lane K. Direct: the key value; Diff: a difference of two
// keys; Interp1 / Interp2: lerps. Default branch: lane K takes the value, others the default.
template <int Lane>
const U8* directU4(const SAnimationAccessorStandIn& acc, int a2, U8* out) noexcept {
    const U8* d = bytesOf(acc);
    const U8 value = d[a2];
    if (acc.hasDefaultValue()) {
        const U8* def = static_cast<const U8*>(acc.getDefaultValue());
        writeLanes4(out, Lane, value, def);
        return def;
    }
    out[0] = value;
    return nullptr;
}

template <int Lane>
const U8* diffU4(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out) noexcept {
    const U8* d = bytesOf(acc);
    const U8 value = static_cast<U8>(d[a3] - d[a2]);
    if (acc.hasDefaultValue()) {
        const U8* def = static_cast<const U8*>(acc.getDefaultValue());
        writeLanes4(out, Lane, value, def);
        return def;
    }
    out[0] = value;
    return nullptr;
}

template <int Lane>
unsigned int interp1U4(const SAnimationAccessorStandIn& acc, int a2, int a3, float t, U8* out) noexcept {
    const unsigned int result = interp1U4Value(bytesOf(acc), a2, a3, t);
    if (acc.hasDefaultValue()) {
        writeLanes4(out, Lane, static_cast<U8>(result), static_cast<const U8*>(acc.getDefaultValue()));
    } else {
        out[0] = static_cast<U8>(result);
    }
    return result;
}

template <int Lane>
unsigned int interp2U4(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4, float t, U8* out) noexcept {
    const unsigned int result = interp2U4Value(bytesOf(acc), a2, a3, a4, t);
    if (acc.hasDefaultValue()) {
        writeLanes4(out, Lane, static_cast<U8>(result), static_cast<const U8*>(acc.getDefaultValue()));
    } else {
        out[0] = static_cast<U8>(result);
    }
    return result;
}

// Materials parameter write, through the stand-in sink.
template <typename T>
int setParam(MaterialParamsStandIn* params, const ApplicatorInfoStandIn* info, const void* value) noexcept {
    return setParameterCvt<T>(params, info->paramId, 0, value);
}

// CColladaDatabase tables, or null when the record is not loaded.
const SDatabaseTables* tablesOf(const CColladaDatabase& db) noexcept {
    if (db.record == nullptr || db.record->data == nullptr) {
        return nullptr;
    }
    return db.record->data->tables;
}

// Stand-in for the channel comparison of getAnimation: the channel record (type at +0x08,
// target at +0x04, sid at +0x0c) is not mapped in this slice, so no channel matches.
bool animationChannelMatches(const SAnimation*, const char*, int, const char*) noexcept { return false; }

// Shared body of the database destructor (D1 and D2 are the same code).
void destroyDatabase(CColladaDatabase* self) noexcept {
    SColladaRecord* record = self->record;
    if (record != nullptr && record->reserved00[1] != 0) {
        dropRecord(record);
        if (resFileManagerInst().streamingEnabled) {
            if (self->record != nullptr && self->record->reserved00[1] == 1) {
                animationStreamingRelease(self);
                resFileManagerUnload(self->record->fileName, 0);
            }
        }
    }
    self->record = nullptr;
}

// Shared body of the copy-from-file constructors (C1 and C2).
CColladaDatabase* initFromSource(CColladaDatabase* self, const CColladaDatabase* source, const char* name,
                                 CColladaFactory* factory) noexcept {
    SColladaRecord* record = resFileManagerGet(source->record, name, 1);
    self->record = record;
    self->factory = factory;
    if (record != nullptr) {
        ++record->reserved00[1];
    }
    return self;
}

// Shared body of constructAnimator: load the file, run the animator build on a temporary
// database that owns one reference, and restore the streaming flag.
int constructAnimatorLoaded(CColladaDatabase* self, SColladaRecord* loaded, CColladaFactory* factory) noexcept {
    (void)self;
    if (loaded == nullptr) {
        return 0;
    }
    ResFileManagerStandIn& inst = resFileManagerInst();
    const bool savedFlag = inst.streamingEnabled;
    inst.streamingEnabled = false;
    CColladaDatabase temp;
    temp.record = loaded;
    temp.factory = factory != nullptr ? factory : defaultFactory;
    ++loaded->reserved00[1];
    const int result = constructAnimatorFromDatabase(&temp);
    destroyDatabase(&temp);
    inst.streamingEnabled = savedFlag;
    return result;
}

}  // namespace

// ===========================================================================
// Float and float[N] value appliers and interpolators.
// ===========================================================================

// 0x618e88 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getIdentityValueEPv
void* CVirtualEx_f4_mix2_getIdentityValue(int self, void* value) {
    (void)self;
    return std::memset(value, 0, 0x10);
}

// 0x618e98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getIdentityValueEPv
void* CVirtualEx_f4_mix3_getIdentityValue(int self, void* value) {
    (void)self;
    return std::memset(value, 0, 0x10);
}

// 0x618f18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getIdentityValueEPv
void* CVirtualEx_f4_mixm1_getIdentityValue(int self, void* value) {
    (void)self;
    return std::memset(value, 0, 0x10);
}

// 0x618ffc _ZN6glitch7collada15animation_track13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEE17applyAddedValueExEPvPfiSA_PNS1_15CApplicatorInfoE
int CApplyValueEx_f1_mixm1_applyAddedValueEx(const float* a1, const float* addend, int count,
                                             MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    float value = 0.0f;
    if (count == 1) {
        value = a1[0];
    } else if (count != 0) {
        float acc = 0.0f;
        for (int i = 0; i < count; ++i) {
            acc = acc + a1[i] * addend[i];
        }
        value = acc;
    }
    return setParam<float>(params, info, &value);
}

// 0x619088 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE15applyAddedValueEPvPfiSC_PNS1_15CApplicatorInfoE
int CVirtualEx_f1_mixm1_applyAddedValue(int self, const float* a2, const float* a3, int a4,
                                        MaterialParamsStandIn* a5, ApplicatorInfoStandIn* a6) {
    (void)self;
    return CApplyValueEx_f1_mixm1_applyAddedValueEx(a2, a3, a4, a5, a6);
}

// 0x6190a4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_mix0_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector2dF>(params, 0, 0, value);
}

// 0x6190bc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_mix1_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector2dF>(params, 0, 0, value);
}

// 0x6190d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_mixm1_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector2dF>(params, 0, 0, value);
}

// 0x6190ec _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f2_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key,
                                                MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    const float* f = floatsOf(acc);
    const float value[2] = {f[2 * key], f[2 * key + 1]};
    return setParam<CoreVector2dF>(params, info, value);
}

// 0x61913c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f2_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key,
                                           MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    (void)self;
    return CApplyValueEx_f2_mixm1_applyKeyBasedValueEx(acc, key, params, info);
}

// 0x619150 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f3_mix0_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector3dF>(params, 0, 0, value);
}

// 0x619168 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f3_mixm1_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector3dF>(params, 0, 0, value);
}

// 0x619180 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f3_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key,
                                                MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    const float* f = floatsOf(acc);
    const float value[3] = {f[3 * key], f[3 * key + 1], f[3 * key + 2]};
    return setParam<CoreVector3dF>(params, info, value);
}

// 0x6191e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f3_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key,
                                           MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    (void)self;
    return CApplyValueEx_f3_mixm1_applyKeyBasedValueEx(acc, key, params, info);
}

// 0x6191f4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mixm1_applyBlendedValue(int self, const float* values, const float* weights, int count,
                                          MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    (void)self;
    float out[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    if (count == 1) {
        for (int i = 0; i < 4; ++i) out[i] = values[i];
    } else if (count != 0) {
        for (int k = 0; k < count; ++k) {
            for (int i = 0; i < 4; ++i) {
                out[i] = weights[k] * values[4 * k + i] + out[i];
            }
        }
    }
    return setParam<CoreVector4dF>(params, info, out);
}

// ===========================================================================
// CColladaDatabase: construction, destruction and animator entries.
// ===========================================================================

// 0x6192f0 _ZN6glitch7collada16CColladaDatabaseC1EPKS1_PKcPNS0_15CColladaFactoryE
CColladaDatabase* CColladaDatabase_C1_fromSource(CColladaDatabase* self, const CColladaDatabase* source,
                                                 const char* name, CColladaFactory* factory) {
    return initFromSource(self, source, name, factory);
}

// 0x619344 _ZN6glitch7collada16CColladaDatabaseC2EPKS1_PKcPNS0_15CColladaFactoryE
CColladaDatabase* CColladaDatabase_C2_fromSource(CColladaDatabase* self, const CColladaDatabase* source,
                                                 const char* name, CColladaFactory* factory) {
    return initFromSource(self, source, name, factory);
}

// 0x619474 _ZN6glitch7collada16CColladaDatabaseD1Ev
void CColladaDatabase_D1(CColladaDatabase* self) {
    destroyDatabase(self);
}

// 0x619510 _ZN6glitch7collada16CColladaDatabase17constructAnimatorEPNS_2io9IReadFileEbPNS0_15CColladaFactoryE
int CColladaDatabase_constructAnimator_file(CColladaDatabase* self, glitch::io::IReadFile* file,
                                            CColladaFactory* factory, CColladaFactory* unusedFactory) {
    (void)unusedFactory;
    SColladaRecord* loaded = resFileManagerLoad(self, nullptr, file);
    return constructAnimatorLoaded(self, loaded, factory);
}

// 0x6195c8 _ZN6glitch7collada16CColladaDatabase17constructAnimatorEPNS_2io9IReadFileEPNS0_15CColladaFactoryE
int CColladaDatabase_constructAnimator_fileOnly(CColladaDatabase* self, CColladaFactory* factory,
                                                CColladaFactory* unused) {
    (void)unused;
    SColladaRecord* loaded = resFileManagerLoad(self, nullptr, nullptr);
    return constructAnimatorLoaded(self, loaded, factory);
}

// 0x61967c _ZN6glitch7collada16CColladaDatabase17constructAnimatorEPKcPNS0_15CColladaFactoryE
int CColladaDatabase_constructAnimator_name(CColladaDatabase* self, CColladaFactory* factory) {
    SColladaRecord* loaded = resFileManagerLoad(self, nullptr, nullptr);
    return constructAnimatorLoaded(self, loaded, factory);
}


// 0x619728 _ZN6glitch7collada6CImageD1Ev
void CImage_D1(CImageStandIn* self) {
    if (self->image != nullptr && self->image->refCount != 0) {
        --self->image->refCount;
    }
    CColladaDatabase_D1(&self->database);
}

// 0x619784 _ZN6glitch7collada16CColladaDatabaseD2Ev
void CColladaDatabase_D2(CColladaDatabase* self) {
    destroyDatabase(self);
}

// ===========================================================================
// uchar[4] interpolators (CInterpreter and its CVirtualEx forwarders).
// ===========================================================================

// 0x619978 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m3_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           int a4, float t, U8* out) {
    return interp2U4<3>(acc, a2, a3, a4, t, out);
}

// 0x619a64 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m3_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, int a5, float a6, U8* a7) {
    (void)self;
    return CInterpreter_u4_m3_getKeyBasedValueEx_interp2(acc, a3, a4, a5, a6, a7);
}

// 0x619a88 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m0_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out) {
    return directU4<0>(acc, a2, out);
}

// 0x619b00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m0_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4) {
    (void)self;
    return CInterpreter_u4_m0_getKeyBasedValueEx_direct(acc, a3, a4);
}

// 0x619b10 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m0_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           float t, U8* out) {
    return interp1U4<0>(acc, a2, a3, t, out);
}

// 0x619be4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m0_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, float a5, U8* a6) {
    (void)self;
    return CInterpreter_u4_m0_getKeyBasedValueEx_interp1(acc, a3, a4, a5, a6);
}

// 0x619c00 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const U8* CInterpreter_u4_m0_getKeyBasedValueEx_diff(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out) {
    return diffU4<0>(acc, a2, a3, out);
}

// 0x619c74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const U8* CVirtualEx_u4_m0_getKeyBasedValue_diff(int self, const SAnimationAccessorStandIn& acc, int a3, int a4,
                                                 U8* a5) {
    (void)self;
    return CInterpreter_u4_m0_getKeyBasedValueEx_diff(acc, a3, a4, a5);
}

// 0x619c88 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi0EhEEhLi4ENS1_17SUseDefaultValuesILi0EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m0_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           int a4, float t, U8* out) {
    return interp2U4<0>(acc, a2, a3, a4, t, out);
}

// 0x619d74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m0_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, int a5, float a6, U8* a7) {
    (void)self;
    return CInterpreter_u4_m0_getKeyBasedValueEx_interp2(acc, a3, a4, a5, a6, a7);
}

// 0x619d98 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m1_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out) {
    return directU4<1>(acc, a2, out);
}

// 0x619e10 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m1_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4) {
    (void)self;
    return CInterpreter_u4_m1_getKeyBasedValueEx_direct(acc, a3, a4);
}

// 0x619e20 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m1_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           float t, U8* out) {
    return interp1U4<1>(acc, a2, a3, t, out);
}

// 0x619f1c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m1_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, float a5, U8* a6) {
    (void)self;
    return CInterpreter_u4_m1_getKeyBasedValueEx_interp1(acc, a3, a4, a5, a6);
}

// 0x619f38 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const U8* CInterpreter_u4_m1_getKeyBasedValueEx_diff(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out) {
    return diffU4<1>(acc, a2, a3, out);
}

// 0x619fac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const U8* CVirtualEx_u4_m1_getKeyBasedValue_diff(int self, const SAnimationAccessorStandIn& acc, int a3, int a4,
                                                 U8* a5) {
    (void)self;
    return CInterpreter_u4_m1_getKeyBasedValueEx_diff(acc, a3, a4, a5);
}

// 0x619fc0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi1EhEEhLi4ENS1_17SUseDefaultValuesILi1EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m1_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           int a4, float t, U8* out) {
    return interp2U4<1>(acc, a2, a3, a4, t, out);
}

// 0x61a0ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m1_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, int a5, float a6, U8* a7) {
    (void)self;
    return CInterpreter_u4_m1_getKeyBasedValueEx_interp2(acc, a3, a4, a5, a6, a7);
}

// 0x61a0d0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m2_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out) {
    return directU4<2>(acc, a2, out);
}

// 0x61a148 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m2_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4) {
    (void)self;
    return CInterpreter_u4_m2_getKeyBasedValueEx_direct(acc, a3, a4);
}

// 0x61a158 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m2_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           float t, U8* out) {
    return interp1U4<2>(acc, a2, a3, t, out);
}

// 0x61a244 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m2_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, float a5, U8* a6) {
    (void)self;
    return CInterpreter_u4_m2_getKeyBasedValueEx_interp1(acc, a3, a4, a5, a6);
}

// 0x61a260 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
const U8* CInterpreter_u4_m2_getKeyBasedValueEx_diff(const SAnimationAccessorStandIn& acc, int a2, int a3, U8* out) {
    return diffU4<2>(acc, a2, a3, out);
}

// 0x61a2d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
const U8* CVirtualEx_u4_m2_getKeyBasedValue_diff(int self, const SAnimationAccessorStandIn& acc, int a3, int a4,
                                                 U8* a5) {
    (void)self;
    return CInterpreter_u4_m2_getKeyBasedValueEx_diff(acc, a3, a4, a5);
}

// 0x61a2e8 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi2EhEEhLi4ENS1_17SUseDefaultValuesILi2EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
unsigned int CInterpreter_u4_m2_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           int a4, float t, U8* out) {
    return interp2U4<2>(acc, a2, a3, a4, t, out);
}

// 0x61a3d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
unsigned int CVirtualEx_u4_m2_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, int a5, float a6, U8* a7) {
    (void)self;
    return CInterpreter_u4_m2_getKeyBasedValueEx_interp2(acc, a3, a4, a5, a6, a7);
}

// 0x61a3fc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
const U8* CInterpreter_u4_m3_getKeyBasedValueEx_direct(const SAnimationAccessorStandIn& acc, int a2, U8* out) {
    return directU4<3>(acc, a2, out);
}

// 0x61a474 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const U8* CVirtualEx_u4_m3_getKeyBasedValue_direct(int self, const SAnimationAccessorStandIn& acc, int a3, U8* a4) {
    (void)self;
    return CInterpreter_u4_m3_getKeyBasedValueEx_direct(acc, a3, a4);
}

// 0x61a484 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int CInterpreter_u4_m3_getKeyBasedValueEx_interp1(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           float t, U8* out) {
    return interp1U4<3>(acc, a2, a3, t, out);
}

// 0x61a558 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int CVirtualEx_u4_m3_getKeyBasedValue_interp1(int self, const SAnimationAccessorStandIn& acc, int a3,
                                                       int a4, float a5, U8* a6) {
    (void)self;
    return CInterpreter_u4_m3_getKeyBasedValueEx_interp1(acc, a3, a4, a5, a6);
}

// ===========================================================================
// CColladaDatabase entries that resolve a named entry and hand it to a constructor.
// ===========================================================================

// 0x61a5d4 _ZNK6glitch7collada16CColladaDatabase20constructGNPSEmitterEPKcPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
int CColladaDatabase_constructGNPSEmitter(const CColladaDatabase& self, const char* name,
                                          video::IVideoDriver* driver, void* strings, CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SGNPSEmitter* entry = t != nullptr ? getGNPSEmitter(*t, name) : nullptr;
    return constructGNPSEmitterEntry(self, entry, driver, strings, root);
}

// 0x61a750 _ZNK6glitch7collada16CColladaDatabase16constructCoronasEPKcPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
int CColladaDatabase_constructCoronas(const CColladaDatabase& self, const char* name, video::IVideoDriver* driver,
                                      CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SCoronas* entry = t != nullptr ? getCoronas(*t, name) : nullptr;
    return constructCoronasEntry(self, entry, driver, root);
}

// 0x61a854 _ZNK6glitch7collada16CColladaDatabase16constructEmitterEPKcPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
int CColladaDatabase_constructEmitter(const CColladaDatabase& self, const char* name, video::IVideoDriver* driver,
                                      void* strings, CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SEmitter* entry = t != nullptr ? getEmitter(*t, name) : nullptr;
    return constructEmitterEntry(self, entry, driver, strings, root);
}

// 0x61aa00 _ZNK6glitch7collada16CColladaDatabase19constructControllerEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructController(OutSlotStandIn* out, const CColladaDatabase& self,
                                                     video::IVideoDriver* driver, const char* name,
                                                     CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SController* entry = t != nullptr ? getController(*t, name) : nullptr;
    return constructControllerEntry(out, self, driver, name, entry, root);
}

// 0x61aae0 _ZNK6glitch7collada16CColladaDatabase17constructGeometryEPNS_5video12IVideoDriverEPKcS6_
OutSlotStandIn* CColladaDatabase_constructGeometry_file(OutSlotStandIn* out, const CColladaDatabase* source,
                                                        const char* a3, const char* fileName,
                                                        const char* geometryName) {
    SColladaRecord* file = resFileManagerGet(source->record, fileName, 1);
    if (file == nullptr) {
        logMessage("File not found");
        logMessage(fileName);
        out->object = nullptr;
        return out;
    }
    ResFileManagerStandIn& inst = resFileManagerInst();
    const bool savedFlag = inst.streamingEnabled;
    inst.streamingEnabled = false;
    CColladaDatabase pair;
    pair.record = file;
    pair.factory = source->factory;
    ++file->reserved00[1];
    OutSlotStandIn geometry;
    constructGeometryEntry(&geometry, pair, a3, geometryName);
    if (geometry.object != nullptr) {
        out->object = geometry.object;
        ++geometry.object->refCount;
    } else {
        logMessage("Geometry not found");
        logMessage(fileName);
        logMessage(geometryName);
        out->object = nullptr;
    }
    if (geometry.object != nullptr && geometry.object->refCount != 0) {
        --geometry.object->refCount;
    }
    destroyDatabase(&pair);
    inst.streamingEnabled = savedFlag;
    return out;
}

// 0x61ac88 _ZNK6glitch7collada16CColladaDatabase11getMaterialEPKcS3_
SMaterial* CColladaDatabase_getMaterial_file(const CColladaDatabase* source, const char* name,
                                             const char* materialName) {
    CColladaDatabase temp;
    CColladaDatabase_C1_fromSource(&temp, source, name, defaultFactory);
    const SDatabaseTables* t = tablesOf(temp);
    SMaterial* material = t != nullptr ? getMaterial(*t, materialName) : nullptr;
    CColladaDatabase_D1(&temp);
    return material;
}

// 0x61b1b4 _ZNK6glitch7collada16CColladaDatabase14constructImageEPKcPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructImage(OutSlotStandIn* out, const CColladaDatabase& self,
                                                const char* name) {
    const SDatabaseTables* t = tablesOf(self);
    SImage* entry = t != nullptr ? getImage(*t, name) : nullptr;
    return constructImageEntry(out, self, entry);
}

// ===========================================================================
// CColladaDatabase: scene, node and material constructors.
// ===========================================================================

// 0x61b8bc _ZNK6glitch7collada16CColladaDatabase20constructVisualSceneEPNS_5video12IVideoDriverEPNS0_12SVisualSceneEPNS0_14CRootSceneNodeE
CRootSceneNode* CColladaDatabase_constructVisualScene(const CColladaDatabase& self, video::IVideoDriver* driver,
                                                      SVisualScene* entry, CRootSceneNode* root) {
    (void)driver;
    if (entry == nullptr) {
        return nullptr;
    }
    // Populating the root from the visual scene (vtable +0x28 setup and the node loop) is
    // not rebuilt: the SVisualScene record layout is not mapped in this slice.
    return root != nullptr ? root : createRootSceneNode(self.factory, self);
}

// 0x61b988 _ZNK6glitch7collada16CColladaDatabase20constructVisualSceneEPNS_5video12IVideoDriverEiPNS0_14CRootSceneNodeE
CRootSceneNode* CColladaDatabase_constructVisualScene_index(const CColladaDatabase& self, video::IVideoDriver* driver,
                                                            int index, CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SVisualScene* entry = nullptr;
    if (t != nullptr && index >= 0 && static_cast<std::size_t>(index) < t->visualScenes.size()) {
        entry = t->visualScenes[static_cast<std::size_t>(index)];
    }
    return CColladaDatabase_constructVisualScene(self, driver, entry, root);
}

// 0x61b9b8 _ZNK6glitch7collada16CColladaDatabase20constructVisualSceneEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
CRootSceneNode* CColladaDatabase_constructVisualScene_name(const CColladaDatabase& self, video::IVideoDriver* driver,
                                                           const char* name, CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SVisualScene* entry = t != nullptr ? getVisualScene(*t, name) : nullptr;
    return CColladaDatabase_constructVisualScene(self, driver, entry, root);
}

// 0x61babc _ZNK6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEb
CRootSceneNode* CColladaDatabase_constructScene_bool(const CColladaDatabase& self, video::IVideoDriver* driver,
                                                     bool animate) {
    CRootSceneNode* scene = constructSceneEntry(self, driver);
    // Attaching the animator (scene vtable +0x6c, constructAnimator) is not rebuilt; the
    // scene is returned without it when animate is set.
    (void)animate;
    return scene;
}

// 0x61bb20 _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPKcPNS0_15CColladaFactoryE
int CColladaDatabase_constructScene_file(CColladaDatabase* self, video::IVideoDriver* driver, const char* name,
                                         CColladaFactory* factory) {
    (void)self;
    (void)driver;
    (void)name;
    CColladaFactory* f = factory != nullptr ? factory : defaultFactory;
    (void)f;
    // The loader step passes the driver where a database is expected; the unresolved
    // argument is kept out of the rebuild, so the entry reports no scene.
    return 0;
}

// 0x61bbd4 _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPKcbPNS0_15CColladaFactoryE
int CColladaDatabase_constructScene_fileAnimated(CColladaDatabase* self, video::IVideoDriver* driver,
                                                 const char* name, bool animate, CColladaFactory* factory) {
    (void)self;
    (void)driver;
    (void)name;
    (void)animate;
    (void)factory;
    return 0;
}

// 0x61be14 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeE
CRootSceneNode* CColladaDatabase_constructNode_entry(const CColladaDatabase& self, video::IVideoDriver* driver,
                                                     SNode* entry) {
    if (entry == nullptr) {
        return nullptr;
    }
    CRootSceneNode* root = createRootSceneNode(self.factory, self);
    // The built node is attached to the root (vtable +0x5c), onPostLoad runs and the node is
    // released. Those calls are not rebuilt; the stand-in builds the node only.
    (void)constructNodeEntry(self, driver, entry, root);
    return root;
}

// 0x61c1fc _ZNK6glitch7collada16CColladaDatabase21getBlendableAnimationEPKNS0_10SAnimationE
int CColladaDatabase_getBlendableAnimation(const CColladaDatabase& self, const SAnimation* animation) {
    if (animation == nullptr) {
        return 0;
    }
    return blendableOfAnimation(self, animation);
}

// 0x61c6ec _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKNS0_10SAnimationEPPv
char* CColladaDatabase_getDefaultValue(const CColladaDatabase& self, const SAnimation* animation, void** out) {
    if (animation == nullptr) {
        return nullptr;
    }
    return defaultValueOfAnimation(self, animation, out);
}

// 0x61c6f4 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPKc
CRootSceneNode* CColladaDatabase_constructNode_name(const CColladaDatabase& self, video::IVideoDriver* driver,
                                                    const char* name) {
    return CColladaDatabase_constructNode_entry(self, driver, findNode(self, name));
}

// 0x61c878 _ZN6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPKcS6_PNS0_15CColladaFactoryE
CRootSceneNode* CColladaDatabase_constructNode_file(const CColladaDatabase* source, video::IVideoDriver* driver,
                                                    const char* name, const char* fileName,
                                                    CColladaFactory* factory) {
    SColladaRecord* file = resFileManagerLoad(source, fileName, nullptr);
    if (file == nullptr) {
        return nullptr;
    }
    CColladaDatabase pair;
    pair.record = file;
    pair.factory = factory;
    ++file->reserved00[1];
    CRootSceneNode* node = constructNodeByName(pair, driver, name, nullptr);
    destroyDatabase(&pair);
    return node;
}

// 0x61c91c _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKcNS0_8SChannel4TypeES3_
const SAnimation* CColladaDatabase_getAnimation(const CColladaDatabase& self, const char* target, int type,
                                                const char* sid) {
    const SDatabaseTables* t = tablesOf(self);
    if (t == nullptr || t->animations.empty()) {
        return nullptr;
    }
    for (const SAnimation* animation : t->animations) {
        if (animationChannelMatches(animation, target, type, sid)) {
            return animation;
        }
    }
    return nullptr;
}

// 0x61cca8 _ZNK6glitch7collada16CColladaDatabase17constructMaterialEPNS_5video12IVideoDriverEPNS0_9SMaterialEPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructMaterial_entry(OutSlotStandIn* out, const CColladaDatabase* self,
                                                         video::IVideoDriver* driver, SMaterial* material,
                                                         CRootSceneNode* root) {
    if (material == nullptr) {
        out->object = nullptr;
        return out;
    }
    // The material loader (string build, vtable calls and intrusive-pointer temporaries) is
    // not rebuilt; the stand-in produces no object.
    return constructMaterialEntry(out, *self, driver, material, root);
}

// 0x61cefc _ZNK6glitch7collada16CColladaDatabase17constructMaterialEPNS_5video12IVideoDriverEPKcPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructMaterial_name(OutSlotStandIn* out, const CColladaDatabase& self,
                                                        video::IVideoDriver* driver, const char* name,
                                                        CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SMaterial* material = t != nullptr ? getMaterial(*t, name) : nullptr;
    return CColladaDatabase_constructMaterial_entry(out, &self, driver, material, root);
}

// 0x61cf44 _ZNK6glitch7collada16CColladaDatabase17constructMaterialEPNS_5video12IVideoDriverEiPNS0_14CRootSceneNodeE
OutSlotStandIn* CColladaDatabase_constructMaterial_index(OutSlotStandIn* out, const CColladaDatabase& self,
                                                         video::IVideoDriver* driver, int index,
                                                         CRootSceneNode* root) {
    const SDatabaseTables* t = tablesOf(self);
    SMaterial* material = nullptr;
    if (t != nullptr && index >= 0 && static_cast<std::size_t>(index) < t->materials.size()) {
        material = t->materials[static_cast<std::size_t>(index)];
    }
    return CColladaDatabase_constructMaterial_entry(out, &self, driver, material, root);
}

// ===========================================================================
// Quaternion and float interpolators (CSceneNode quaternion mixins).
// ===========================================================================

// 0x61cb7c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatAngleMixC_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out) {
    (void)self;
    const std::uint32_t* r = node->getRotationWords();
    for (int i = 0; i < 4; ++i) out[i] = r[i];
    return r;
}

// 0x61cbb8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatAngleMixS_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out) {
    (void)self;
    const std::uint32_t* r = node->getRotationWords();
    for (int i = 0; i < 4; ++i) out[i] = r[i];
    return r;
}

// 0x61cc30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatMixC_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out) {
    (void)self;
    const std::uint32_t* r = node->getRotationWords();
    for (int i = 0; i < 4; ++i) out[i] = r[i];
    return r;
}

// 0x61cc6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE13retrieveValueEPvSA_
const std::uint32_t* CVirtualEx_quatMixS_retrieveValue(int self, ISceneNodeStandIn* node, std::uint32_t* out) {
    (void)self;
    const std::uint32_t* r = node->getRotationWords();
    for (int i = 0; i < 4; ++i) out[i] = r[i];
    return r;
}

// 0x61caa8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u3_mixm1_applyValue(int self, const U8* value, MaterialParamsStandIn* params) {
    (void)self;
    const U8 bytes[4] = {value[0], value[1], value[2], 0xff};
    return setParameterCvt<VideoSColor>(params, 0, 0, bytes);
}

// 0x61caf0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELin1EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u3_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key,
                                                MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    const U8* d = bytesOf(acc);
    const U8 bytes[4] = {d[3 * key], d[3 * key + 1], d[3 * key + 2], 0xff};
    return setParam<VideoSColor>(params, info, bytes);
}

// 0x61cb68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u3_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key,
                                           MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    (void)self;
    return CApplyValueEx_u3_mixm1_applyKeyBasedValueEx(acc, key, params, info);
}

// 0x61c9b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix0_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector4dF>(params, 0, 0, value);
}

// 0x61c9d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix1_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector4dF>(params, 0, 0, value);
}

// 0x61c9e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix2_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector4dF>(params, 0, 0, value);
}

// 0x61ca00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mix3_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector4dF>(params, 0, 0, value);
}

// 0x61ca18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f4_mixm1_applyValue(int self, const float* value, MaterialParamsStandIn* params) {
    (void)self;
    return setParameterCvt<CoreVector4dF>(params, 0, 0, value);
}

// 0x61ca30 _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f4_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int key,
                                                MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    const float* f = floatsOf(acc);
    const float value[4] = {f[4 * key], f[4 * key + 1], f[4 * key + 2], f[4 * key + 3]};
    return setParam<CoreVector4dF>(params, info, value);
}

// 0x61ca94 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f4_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key,
                                           MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    (void)self;
    return CApplyValueEx_f4_mixm1_applyKeyBasedValueEx(acc, key, params, info);
}

// 0x61cf8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
const SAnimationOutput* CVirtualEx_quatMixF_getKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int key,
                                                             float* out) {
    (void)self;
    const SAnimationOutput* result = acc.getOutput(0);
    loadQuatKey(static_cast<const float*>(result->data), key, out);
    return result;
}

// 0x61cfd8 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
float CInterpreterQuaternion_quatMixF_getKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                         float* out) {
    const float* data = floatsOf(acc);
    float value[4];
    float base[4];
    loadQuatKey(data, a3, value);
    loadQuatKey(data, a2, base);
    conjugateXyz(base);
    float product[4];
    quaternionMultiply(product, base, value);
    for (int i = 0; i < 4; ++i) out[i] = product[i];
    return product[0];
}

// 0x61d0ec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
float CVirtualEx_quatMixF_getKeyBasedValue_ex(int self, const SAnimationAccessorStandIn& acc, int a3, int a4,
                                              float* a5) {
    (void)self;
    return CInterpreterQuaternion_quatMixF_getKeyBasedValueEx(acc, a3, a4, a5);
}

// 0x61d100 _ZN6glitch7collada15animation_track22CInterpreterQuaternionINS1_25CSceneNodeQuaternionMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreterQuaternion_quatMixF_getKeyBasedValueEx_t(const SAnimationAccessorStandIn& acc, int a2, int a3,
                                                           int a4, float t, float* out) {
    const float* data = floatsOf(acc);
    float from[4];
    float to[4];
    float base[4];
    loadQuatKey(data, a3, from);
    loadQuatKey(data, a4, to);
    loadQuatKey(data, a2, base);
    float blended[4];
    quaternionSlerp(blended, from, to, t);
    conjugateXyz(base);
    float product[4];
    quaternionMultiply(product, base, blended);
    for (int i = 0; i < 4; ++i) out[i] = product[i];
    return product[0];
}

// 0x61d2a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_quatMixF_getKeyBasedValue_t(int self, const SAnimationAccessorStandIn& acc, int a3, int a4, int a5,
                                             float a6, float* a7) {
    (void)self;
    return CInterpreterQuaternion_quatMixF_getKeyBasedValueEx_t(acc, a3, a4, a5, a6, a7);
}

// 0x61d2cc _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi3EfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_f4_m3_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4,
                                                    float t, float* out) {
    return interp2F4<3>(acc, a2, a3, a4, t, out);
}

// 0x61d3ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_f4_m3_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4,
                                                int a5, float a6, float* a7) {
    (void)self;
    return CInterpreter_f4_m3_getKeyBasedValueEx_interp2(acc, a3, a4, a5, a6, a7);
}

// 0x61d3d0 _ZN6glitch7collada15animation_track13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f1_mixm1_applyKeyBasedValueEx(const SAnimationAccessorStandIn& acc, int a2,
                                                MaterialParamsStandIn* params, ApplicatorInfoStandIn* info) {
    const float value = floatsOf(acc)[a2];
    return setParam<float>(params, info, &value);
}

// 0x61d414 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_f1_mixm1_applyKeyBasedValue(int self, const SAnimationAccessorStandIn& acc, int a3,
                                           MaterialParamsStandIn* a4, ApplicatorInfoStandIn* a5) {
    (void)self;
    return CApplyValueEx_f1_mixm1_applyKeyBasedValueEx(acc, a3, a4, a5);
}

// 0x61d428 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELi1EfEEfLi4ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
float CInterpreter_f4_m1_getKeyBasedValueEx_interp2(const SAnimationAccessorStandIn& acc, int a2, int a3, int a4,
                                                    float t, float* out) {
    return interp2F4<1>(acc, a2, a3, a4, t, out);
}

// 0x61d504 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float CVirtualEx_f4_m1_getKeyBasedValue_interp2(int self, const SAnimationAccessorStandIn& acc, int a3, int a4,
                                                int a5, float a6, float* a7) {
    (void)self;
    return CInterpreter_f4_m1_getKeyBasedValueEx_interp2(acc, a3, a4, a5, a6, a7);
}

}  // namespace glitch::collada::recon_11
