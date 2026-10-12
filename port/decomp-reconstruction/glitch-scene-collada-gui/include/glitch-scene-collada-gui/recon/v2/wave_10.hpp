// Wave 10 reconstruction (namespace glitch::collada::recon_v2_10).
//
// Covers glitch::collada::animation_track applicator and interpolator bodies, and
// glitch::collada::CColladaDatabase bodies, from the Hex-Rays pseudocode.
//
// Layouts used: animation_track.hpp and collada_db.hpp only.
//
// Pointer-valued results that the pseudocode types as `int` or `_DWORD *` are
// returned as host pointers (void *). Unmapped pieces are declared below as
// stand-ins. They are defined only by the test double file, never by src.
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_10_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_10_HPP

#include <cstdint>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/layouts/collada_db.hpp"

namespace glitch::collada::recon_v2_10 {

using glitch::collada::CColladaDatabase;
using glitch::collada::CColladaFactory;
using glitch::collada::CResFile;
using glitch::collada::CResFileManager;
using glitch::collada::SAnimationAccessor;
using glitch::collada::animation_track::CApplicatorInfo;

// Value kinds passed to setParameterCvt<T>. The T names come from the pseudocode.
enum class ParamType : std::uint8_t { F1, F2, F3, F4, SColor };

// ---------------------------------------------------------------------------
// Stand-ins for unmapped functions (not reconstructed in this wave).
// ---------------------------------------------------------------------------

// SAnimationAccessor::getOutput(a, 0): the record pointer, and the data base read
// from its +4 word (every pseudocode body reads the data base this way).
std::intptr_t acc_output_record(SAnimationAccessor* a);
std::uint8_t* acc_output_data(SAnimationAccessor* a);
// SAnimationAccessor::hasDefaultValue / getDefaultValue.
bool acc_has_default(SAnimationAccessor* a);
const std::uint8_t* acc_default_value(SAnimationAccessor* a);

// IMaterialParameters::setParameterCvt<T> sink.
int set_param_cvt(void* target, std::uint32_t id, int flag, ParamType t, const void* value);

// glitch::core::quaternion::operator* and slerp.
void quat_mul(float out[4], const float lhs[4], const float rhs[4]);
void quat_slerp(float out[4], const float a[4], const float b[4], float t);

// CInterpreter<CMixin<float,1,..>,float,1,SUseDefaultLerp<float>>::getKeyBasedValueEx.
void interp_f1_lerp_ex(SAnimationAccessor* a1, int a2, int a3, float a4, float* out);

// Vtable slot +152 of the scene node that retrieveValue reads.
const std::uint32_t* node_quat_slot152(void* node);

// CImage (not mapped in collada_db.hpp): resource slot at +0x14, embedded
// CColladaDatabase at +0x0c, and the two vtable writes of the destructor.
void* image_resource(void* self);
CColladaDatabase* image_database(void* self);
void image_set_vptr(void* self, int which);

// Scene-node virtual calls and fields (CRootSceneNode, SVisualScene, SAnimation).
void root_call(void* obj, int slot, void* arg);
void node_on_post_load(void* root);
void drop_subobject(void* p);
void* scene_field1(void* scene);
int scene_node_count(void* scene);
void* scene_node_at(void* scene, int index);
const void* animation_blendable_link(const void* animation);

// CResFileManager, reference counting, streaming and logging.
CResFileManager* res_manager();
CResFile* res_get(CResFileManager* mgr, CResFile* reader, const char* name, int flag);
CResFile* res_load(CResFileManager* mgr, const void* self, const void* a, const void* b, void* reader);
void res_unload(CResFileManager* mgr, const char* name, int flag);
void ref_drop(void* obj);
void streaming_release(void* manager, CColladaDatabase* db);
void* streaming_instance();
void log_msg(const char* msg, int level);
CColladaFactory* db_default_factory();

// CColladaDatabase members not declared in collada_db.hpp.
void* db_construct_animator(CColladaDatabase* db);
void* db_get_gnps_emitter(const CColladaDatabase* db, const char* name);
void* db_construct_gnps_emitter(const CColladaDatabase* db, void* entry, int a3, int a4, int a5);
void* db_get_coronas(const CColladaDatabase* db, const char* name);
void* db_construct_coronas(const CColladaDatabase* db, void* entry, int a3, int a4);
void* db_get_emitter(const CColladaDatabase* db, const char* name);
void* db_construct_emitter(const CColladaDatabase* db, void* entry, int a3, int a4, int a5);
void* db_get_controller(void* driver, const char* name);
void db_construct_controller(void** out, void* driver, const char* name, void* controller, void* root);
void* db_get_material_of(void* driver, const char* name);
void* db_get_material_index_of(void* driver, int index);
void* db_get_material(CColladaDatabase* db, const char* name);
void* material_construct_body(void* out, void* driver, const char* name, void* entry, void* root);
void db_construct_geometry(void** out, void* driver, const char* name, const char* arg);
void* db_get_image(const CColladaDatabase* db, const char* name);
void db_construct_image(void* out, const CColladaDatabase* db, void* entry);
void* db_get_visual_scene(const CColladaDatabase* db, const char* name);
void* db_get_visual_scene_index(const CColladaDatabase* db, int index);
void* db_get_node(const CColladaDatabase* db, const char* name);
void* db_construct_node_entry(const CColladaDatabase* db, const char* driver, void* node, void* root);
void* db_make_root(const CColladaDatabase* db);
void* db_construct_scene_basic(const CColladaDatabase* db, void* driver);
void* db_construct_scene_named(const CColladaDatabase* db, void* driver, const char* name, void* factory);
const void* db_find_animation(const CColladaDatabase* db, const char* name, int channelType, const char* target);
void* db_get_default(const CColladaDatabase* db, const void* blendable, void** out);

// ---------------------------------------------------------------------------
// CVirtualEx / CApplyValueEx applicator bodies.
// ---------------------------------------------------------------------------
int w10_6190bc(int a1, const float* a2, void* a3);
int w10_6190d4(int a1, const float* a2, void* a3);
int w10_6190ec(SAnimationAccessor* a1, int a2, void* a3, CApplicatorInfo* a4);
int w10_619150(int a1, const float* a2, void* a3);
int w10_619168(int a1, const float* a2, void* a3);
int w10_619180(SAnimationAccessor* a1, int a2, void* a3, CApplicatorInfo* a4);
int w10_6191f4(int a1, const void* a2, const float* a3, int a4, void* a5, CApplicatorInfo* a6);
int w10_61c9b8(int a1, const float* a2, void* a3);
int w10_61c9d0(int a1, const float* a2, void* a3);
int w10_61c9e8(int a1, const float* a2, void* a3);
int w10_61ca00(int a1, const float* a2, void* a3);
int w10_61ca18(int a1, const float* a2, void* a3);
int w10_61ca30(SAnimationAccessor* a1, int a2, void* a3, CApplicatorInfo* a4);
int w10_61caa8(int a1, const std::uint8_t* a2, void* a3);
int w10_61caf0(SAnimationAccessor* a1, int a2, void* a3, CApplicatorInfo* a4);
int w10_61d3d0(SAnimationAccessor* a1, int a2, void* a3, CApplicatorInfo* a4);
int w10_6202a0(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, CApplicatorInfo* a6);

// Quaternion retrieveValue (vtable slot +152) bodies.
const std::uint32_t* w10_61cb7c(int a1, void* a2, std::uint32_t* a3);
const std::uint32_t* w10_61cbb8(int a1, void* a2, std::uint32_t* a3);
const std::uint32_t* w10_61cc30(int a1, void* a2, std::uint32_t* a3);
const std::uint32_t* w10_61cc6c(int a1, void* a2, std::uint32_t* a3);

// getKeyBasedValue (quaternion and float) bodies.
std::intptr_t w10_61cf8c(int a1, SAnimationAccessor* a2, int a3, float* a4);
float w10_61cfd8(SAnimationAccessor* a1, int a2, int a3, float* a4);
float w10_61d100(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, float* a6);
float w10_61d2cc(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, void* a6);
float w10_61d428(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, void* a6);

// ---------------------------------------------------------------------------
// CInterpreter<CMixin<unsigned char,4,...>> getKeyBasedValueEx, mixin index 0..3.
// ---------------------------------------------------------------------------
std::uint32_t w10_619978(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint8_t* a6);
const std::uint8_t* w10_619a88(SAnimationAccessor* a1, int a2, std::uint8_t* a3);
std::uint32_t w10_619b10(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint8_t* a5);
const std::uint8_t* w10_619c00(SAnimationAccessor* a1, int a2, int a3, std::uint8_t* a4);
std::uint32_t w10_619c88(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint8_t* a6);
const std::uint8_t* w10_619d98(SAnimationAccessor* a1, int a2, std::uint8_t* a3);
std::uint32_t w10_619e20(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint8_t* a5);
const std::uint8_t* w10_619f38(SAnimationAccessor* a1, int a2, int a3, std::uint8_t* a4);
std::uint32_t w10_619fc0(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint8_t* a6);
const std::uint8_t* w10_61a0d0(SAnimationAccessor* a1, int a2, std::uint8_t* a3);
std::uint32_t w10_61a158(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint8_t* a5);
const std::uint8_t* w10_61a260(SAnimationAccessor* a1, int a2, int a3, std::uint8_t* a4);
std::uint32_t w10_61a2e8(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint8_t* a6);
const std::uint8_t* w10_61a3fc(SAnimationAccessor* a1, int a2, std::uint8_t* a3);
std::uint32_t w10_61a484(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint8_t* a5);

// ---------------------------------------------------------------------------
// CColladaDatabase bodies.
// ---------------------------------------------------------------------------
CColladaDatabase* w10_6192f0(CColladaDatabase* self, CResFile** a2, const char* a3, CColladaFactory* a4);
CColladaDatabase* w10_619344(CColladaDatabase* self, CResFile** a2, const char* a3, CColladaFactory* a4);
void w10_619474(CColladaDatabase* self);
void w10_619784(CColladaDatabase* self);
void* w10_619510(CColladaDatabase* self, void* a2, int* a3, CColladaFactory* a4);
void* w10_6195c8(CColladaDatabase* self, void* a2, CColladaFactory* a3);
void* w10_61967c(CColladaDatabase* self, void* a2, CColladaFactory* a3);
void w10_619728(void* self);
void* w10_61a5d4(const CColladaDatabase* a1, const char* a2, int a3, int a4, int a5);
void* w10_61a750(const CColladaDatabase* a1, const char* a2, int a3, int a4);
void* w10_61a854(const CColladaDatabase* a1, const char* a2, int a3, int a4, int a5);
void** w10_61aa00(void** self, void* a2, const char* a3, void* a4, void* a5);
CColladaDatabase* w10_61aae0(CColladaDatabase* self, CColladaDatabase* a2, const char* a3, const char* a4,
                             const char* a5);
const char** w10_61ac88(CResFile** rec, const char* a2, const char* a3);
void* w10_61b1b4(void* out, CColladaDatabase* self, const char* entryName);
void* w10_61b8bc(const CColladaDatabase* a1, const char* a2, void* scene, void* root);
void* w10_61b988(const CColladaDatabase* a1, void* driver, int index, void* root);
void* w10_61b9b8(const CColladaDatabase* a1, void* driver, const char* name, void* root);
void* w10_61babc(const CColladaDatabase* a1, void* driver, bool animate);
void* w10_61bb20(const CColladaDatabase* a1, void* driver, void* a3, CColladaFactory* a4);
void* w10_61bbd4(const CColladaDatabase* a1, void* driver, const char* name, void* a4, CColladaFactory* a5);
void* w10_61be14(const CColladaDatabase* a1, const char* driver, void* node);
const void* w10_61c1fc(const CColladaDatabase* a1, const void* animation);
const std::uint8_t* w10_61c6ec(const CColladaDatabase* a1, const void* animation, void** out);
void* w10_61c6f4(const CColladaDatabase* a1, void* driver, const char* name);
void* w10_61c878(const CColladaDatabase* a1, void* driver, const char* name, void* a4, CColladaFactory* a5);
const void* w10_61c91c(const CColladaDatabase* a1, const char* name, int channelType, const char* target);
void* w10_61cca8(void* out, void* a2, const char* a3, void* a4, void* a5);
void* w10_61cefc(CColladaDatabase* self, void* driver, const char* name, void* root, void* a5);
void* w10_61cf44(CColladaDatabase* self, void* driver, int id, void* root, void* a5);

}  // namespace glitch::collada::recon_v2_10

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_10_HPP
