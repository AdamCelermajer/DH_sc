// Test for the wave 10 reconstruction. The stand-ins declared in
// wave_10.hpp are defined here as test doubles only.
//
// Build: g++ -std=c++17 -Wall -Iinclude tests/recon_v2_wave_10_test.cpp src/recon/v2/wave_10.cpp -o $SCRATCH/v2_10

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#include "glitch-scene-collada-gui/recon/v2/wave_10.hpp"

// The factory is only ever compared by address (the canonical CColladaFactory is abstract), so the tests use tagged storage.
namespace {
glitch::collada::CColladaFactory* fakeFactory(int index = 0) {
    static char storage[2][16];
    return reinterpret_cast<glitch::collada::CColladaFactory*>(storage[index]);
}
}  // namespace

using namespace glitch::collada::recon_v2_10;
using glitch::collada::CResFile;
using glitch::collada::CResFileManager;
using glitch::collada::SAnimationAccessor;
using glitch::collada::animation_track::CApplicatorInfo;

// ---------------------------------------------------------------------------
// Fake accessor state shared by all acc_* doubles.
// ---------------------------------------------------------------------------
namespace {

std::vector<std::uint8_t> g_data;
bool g_has_default = false;
std::vector<std::uint8_t> g_default;

// Recorded setParameterCvt call.
struct SetCall {
    void* target = nullptr;
    std::uint32_t id = 0;
    int flag = -1;
    ParamType type = ParamType::F1;
    std::uint8_t bytes[16] = {};
    int calls = 0;
};
SetCall g_set;

// Recorded quaternion product inputs.
float g_mul_lhs[4];
float g_mul_rhs[4];

CResFileManager g_mgr;
int g_res_get_calls = 0;
CResFile g_record;
int g_drops = 0;

std::uint32_t g_slot152[4] = {11, 22, 33, 44};

}  // namespace

// ---------------------------------------------------------------------------
// Test doubles for the stand-ins.
// ---------------------------------------------------------------------------
namespace glitch::collada::recon_v2_10 {

std::intptr_t acc_output_record(SAnimationAccessor*) { return 0x1000; }
std::uint8_t* acc_output_data(SAnimationAccessor*) { return g_data.data(); }
bool acc_has_default(SAnimationAccessor*) { return g_has_default; }
const std::uint8_t* acc_default_value(SAnimationAccessor*) { return g_default.data(); }

int set_param_cvt(void* target, std::uint32_t id, int flag, ParamType t, const void* value) {
    g_set.target = target;
    g_set.id = id;
    g_set.flag = flag;
    g_set.type = t;
    const std::size_t n = (t == ParamType::F1) ? 4 : (t == ParamType::F2) ? 8
                        : (t == ParamType::F3) ? 12 : (t == ParamType::F4) ? 16 : 4;
    std::memcpy(g_set.bytes, value, n);
    ++g_set.calls;
    return 1;
}

void quat_mul(float out[4], const float lhs[4], const float rhs[4]) {
    std::memcpy(g_mul_lhs, lhs, sizeof(g_mul_lhs));
    std::memcpy(g_mul_rhs, rhs, sizeof(g_mul_rhs));
    for (int i = 0; i < 4; ++i) out[i] = lhs[i] + rhs[i];
}

void quat_slerp(float out[4], const float a[4], const float b[4], float t) {
    for (int i = 0; i < 4; ++i) out[i] = a[i] + (b[i] - a[i]) * t;
}

void interp_f1_lerp_ex(SAnimationAccessor*, int, int, float, float* out) { *out = 0.0f; }

const std::uint32_t* node_quat_slot152(void*) { return g_slot152; }

void root_call(void*, int, void*) {}
void node_on_post_load(void*) {}
void drop_subobject(void*) {}
void* scene_field1(void*) { return nullptr; }
int scene_node_count(void*) { return 0; }
void* scene_node_at(void*, int) { return nullptr; }
const void* animation_blendable_link(const void*) { return nullptr; }

void* image_resource(void*) { return nullptr; }
CColladaDatabase* image_database(void*) { return nullptr; }
void image_set_vptr(void*, int) {}

CResFileManager* res_manager() { return &g_mgr; }
CResFile* res_get(CResFileManager*, CResFile*, const char*, int) {
    ++g_res_get_calls;
    return &g_record;
}
CResFile* res_load(CResFileManager*, const void*, const void*, const void*, void*) { return &g_record; }
void res_unload(CResFileManager*, const char*, int) {}
void ref_drop(void*) { ++g_drops; }
void streaming_release(void*, CColladaDatabase*) {}
void* streaming_instance() { return nullptr; }
void log_msg(const char*, int) {}
CColladaFactory* db_default_factory() {
    return fakeFactory(1);
}

void* db_construct_animator(CColladaDatabase*) { return nullptr; }
void* db_get_gnps_emitter(const CColladaDatabase*, const char*) { return nullptr; }
void* db_construct_gnps_emitter(const CColladaDatabase*, void*, int, int, int) { return nullptr; }
void* db_get_coronas(const CColladaDatabase*, const char*) { return nullptr; }
void* db_construct_coronas(const CColladaDatabase*, void*, int, int) { return nullptr; }
void* db_get_emitter(const CColladaDatabase*, const char*) { return nullptr; }
void* db_construct_emitter(const CColladaDatabase*, void*, int, int, int) { return nullptr; }
void* db_get_controller(void*, const char*) { return nullptr; }
void db_construct_controller(void**, void*, const char*, void*, void*) {}
void* db_get_material_of(void*, const char*) { return nullptr; }
void* db_get_material_index_of(void*, int) { return nullptr; }
void* db_get_material(CColladaDatabase*, const char*) { return nullptr; }
void* material_construct_body(void* out, void*, const char*, void*, void*) { return out; }
void db_construct_geometry(void** out, void*, const char*, const char*) { *out = nullptr; }
void* db_get_image(const CColladaDatabase*, const char*) { return nullptr; }
void db_construct_image(void*, const CColladaDatabase*, void*) {}
void* db_get_visual_scene(const CColladaDatabase*, const char*) { return nullptr; }
void* db_get_visual_scene_index(const CColladaDatabase*, int) { return nullptr; }
void* db_get_node(const CColladaDatabase*, const char*) { return nullptr; }
void* db_construct_node_entry(const CColladaDatabase*, const char*, void*, void*) { return nullptr; }
void* db_make_root(const CColladaDatabase*) { return nullptr; }
void* db_construct_scene_basic(const CColladaDatabase*, void*) { return nullptr; }
void* db_construct_scene_named(const CColladaDatabase*, void*, const char*, void*) { return nullptr; }
const void* db_find_animation(const CColladaDatabase*, const char*, int, const char*) { return nullptr; }
void* db_get_default(const CColladaDatabase*, const void*, void**) { return nullptr; }

}  // namespace glitch::collada::recon_v2_10

// ---------------------------------------------------------------------------
// Checks.
// ---------------------------------------------------------------------------
static int g_failures = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main() {
    // 1. Constant applyValue: float[2] forwards the value with id 0, flag 0.
    {
        float v[2] = {1.5f, -2.5f};
        int dummy = 0;
        g_set = SetCall{};
        int r = w10_6190bc(0, v, &dummy);
        CHECK(r == 1);
        CHECK(g_set.target == &dummy && g_set.id == 0 && g_set.flag == 0);
        CHECK(g_set.type == ParamType::F2);
        float out[2];
        std::memcpy(out, g_set.bytes, 8);
        CHECK(near(out[0], 1.5f) && near(out[1], -2.5f));
    }

    // 2. applyKeyBasedValueEx float[2]: reads keyframe a2 (8 bytes) and uses the applicator id.
    {
        g_data.assign(64, 0);
        float kf[4] = {0.0f, 0.0f, 3.0f, 4.0f};  // keyframe 1 = (3, 4)
        std::memcpy(g_data.data(), kf, sizeof(kf));
        glitch::collada::animation_track::CMaterialApplicatorInfo app;
        app.parameterId = 7;
        int dummy = 0;
        g_set = SetCall{};
        w10_6190ec(nullptr, 1, &dummy, &app);
        float out[2];
        std::memcpy(out, g_set.bytes, 8);
        CHECK(g_set.id == 7 && near(out[0], 3.0f) && near(out[1], 4.0f));
    }

    // 3. applyBlendedValue float[4] with two weighted keyframes.
    {
        float src[8] = {1, 2, 3, 4, 10, 20, 30, 40};
        float w[2] = {0.25f, 0.75f};
        glitch::collada::animation_track::CMaterialApplicatorInfo app;
        app.parameterId = 3;
        int dummy = 0;
        g_set = SetCall{};
        w10_6191f4(0, src, w, 2, &dummy, &app);
        float out[4];
        std::memcpy(out, g_set.bytes, 16);
        CHECK(near(out[0], 0.25f + 7.5f) && near(out[1], 0.5f + 15.0f));
        CHECK(near(out[2], 0.75f + 22.5f) && near(out[3], 1.0f + 30.0f));
        CHECK(g_set.id == 3 && g_set.type == ParamType::F4);
    }

    // 4. uchar[4] interpolator, mixin index 3, with a default value.
    {
        g_data.assign(64, 0);
        g_data[0] = 10;   // a2 sample
        g_data[1] = 20;   // a3 sample
        g_data[2] = 30;   // a4 sample
        g_has_default = true;
        g_default = {1, 2, 3, 4};
        std::uint8_t out[4] = {0, 0, 0, 0};
        std::uint32_t r = w10_619978(nullptr, 0, 1, 2, 0.5f, out);
        // v11 = 10, v12 = 20, v13 = 10, result = 10 + 0.5 * (20 - 10) = 15
        CHECK(r == 15);
        CHECK(out[0] == 1 && out[1] == 2 && out[2] == 3 && out[3] == 15);
    }

    // 5. uchar[4] interpolator, mixin index 0, no default: returns the mixed value.
    {
        g_data.assign(64, 0);
        g_data[4] = 100;
        g_data[5] = 110;
        g_has_default = false;
        std::uint8_t out[4] = {0, 0, 0, 0};
        std::uint32_t r = w10_619b10(nullptr, 4, 5, 0.3f, out);
        // 100 + 0.3 * (110 - 100) = 103
        CHECK(r == 103 && out[0] == 103);
    }

    // 6. Quaternion getKeyBasedValueEx: components 0..2 of A are sign-flipped.
    {
        g_data.assign(64, 0);
        float a[4] = {1, 2, 3, 4};
        float b[4] = {5, 6, 7, 8};
        std::memcpy(g_data.data() + 0, a, 16);   // keyframe 0 = A
        std::memcpy(g_data.data() + 16, b, 16);  // keyframe 1 = B
        float out[4] = {0, 0, 0, 0};
        float r = w10_61cfd8(nullptr, 0, 1, out);
        CHECK(near(g_mul_lhs[0], -1) && near(g_mul_lhs[1], -2) && near(g_mul_lhs[2], -3));
        CHECK(near(g_mul_lhs[3], 4));
        CHECK(near(g_mul_rhs[0], 5) && near(g_mul_rhs[3], 8));
        CHECK(near(r, out[0]));
    }

    // 7. retrieveValue copies the four words that vtable slot 152 returns.
    {
        std::uint32_t dst[4] = {0, 0, 0, 0};
        w10_61cb7c(0, nullptr, dst);
        CHECK(dst[0] == 11 && dst[3] == 44);
    }

    // 8. CColladaDatabase(CResFile**, ...) stores the record and factory and bumps its count.
    {
        g_record = CResFile{};
        g_record.refCount = 1;
        glitch::collada::CColladaDatabase db;
        CResFile* slot = &g_record;
        CColladaFactory& fac = *fakeFactory();
        w10_6192f0(&db, &slot, "x", &fac);
        CHECK(db.record == &g_record && db.factory == &fac && g_record.refCount == 2);
        w10_619474(&db);
        CHECK(db.record == nullptr);
        CHECK(g_drops >= 1);
    }

    // 9. The duplicated bodies agree with each other.
    {
        g_record = CResFile{};
        glitch::collada::CColladaDatabase a;
        glitch::collada::CColladaDatabase b;
        CResFile* slot = &g_record;
        CColladaFactory& fac = *fakeFactory();
        w10_6192f0(&a, &slot, "x", &fac);
        w10_619344(&b, &slot, "x", &fac);
        CHECK(a.record == b.record && a.factory == b.factory);
    }

    // 10. The applyKeyBasedValue F1 path reads one float at 4 * a2.
    {
        g_data.assign(64, 0);
        float v = 2.5f;
        std::memcpy(g_data.data() + 8, &v, 4);
        glitch::collada::animation_track::CMaterialApplicatorInfo app;
        app.parameterId = 9;
        int dummy = 0;
        g_set = SetCall{};
        w10_61d3d0(nullptr, 2, &dummy, &app);
        float got;
        std::memcpy(&got, g_set.bytes, 4);
        CHECK(g_set.type == ParamType::F1 && g_set.id == 9 && near(got, 2.5f));
    }

    if (g_failures == 0) {
        std::printf("PASS wave_10: 10 checks\n");
        return 0;
    }
    std::printf("FAIL wave_10: %d check(s) failed\n", g_failures);
    return 1;
}
