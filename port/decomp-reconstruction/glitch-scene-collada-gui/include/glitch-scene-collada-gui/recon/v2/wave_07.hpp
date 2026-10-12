// Wave 07 of the partial-row rerun for glitch-scene-collada-gui.
// Declarations for the animation_track, particle-system force and collada factory
// bodies in this wave. Only the layout headers animation_track.hpp and collada_db.hpp
// are included. Guest-side behaviour that is not in a layout (virtual slots,
// material parameter setters, node and resource lookups) is passed in as an
// injected callable, so the reconstructed bodies can be tested on the host.
// The shared math types are not present yet, so plain float arrays are used.
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_07_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_07_HPP

#include <cstddef>
#include <cstdint>
#include <functional>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"
#include "glitch-scene-collada-gui/layouts/collada_db.hpp"

namespace glitch::collada::recon_v2_07 {

// Injected guest virtual dispatch. `slot_offset` is the vtable byte offset that the
// original code calls; `arg` is null for one-argument slots.
using wave_07_slot_call = std::function<int(void* receiver, std::uint32_t slot_offset, const void* arg)>;

// Injected dispatch for IParticleSystem::bindForce. base_of maps the particle system
// object to the base sub-object at its virtual-base offset (read from vptr-12 in the original).
struct wave_07_bind_ports {
    std::function<void*(void* self)> base_of;
    wave_07_slot_call call;
};

// Injected node accessors for the force scene nodes (node offsets are not in any layout).
struct wave_07_node_ports {
    std::function<void*(void* node)> particle_system_of;   // *(node + 376) in the original
    std::function<std::uint32_t(void* self)> force_address_of; // self + 320 in the original
};

// Injected material parameter setter: IMaterialParameters::setParameterCvt<vector3d<float>>.
using wave_07_param_set = std::function<int(void* target, std::uint16_t parameter_id, const void* value)>;

// Injected key interpolation: writes three floats to out and returns out[0].
using wave_07_key_interp = std::function<float(int a2, int a3, float a4, float* out)>;

// Injected resource lookup (device file system, name, flags 0) and type tag (vtable slot +0xBC).
using wave_07_resource_lookup = std::function<void*(const char* name, std::uint32_t zero)>;
using wave_07_tag_of = std::function<std::uint32_t(void* object)>;

// Type tag checked by getExternalLightSceneNodeDefault ('lght' as a little-endian word).
constexpr std::uint32_t kWave07LightTag = 1952999276u;

// Placeholder config record for CColladaFactory::get{Vertex,Index}BufferConfig. The
// layout is not in the headers, so the field names are placeholders.
struct wave_07_buffer_config {
    std::uint32_t word00;   // written as 4 by both getters
    std::uint8_t byte04;    // written as 0
    std::uint8_t byte05;    // written as 0
};

// Dequantized key track: the table base (already fetched from the accessor), and the
// per-component scale and offset (3 floats each). Key k starts at base + 3 * k.
template <typename T>
struct wave_07_quantized {
    const T* base;
    const float* scale;
    const float* offset;
};

// Force proxy allocated by IParticleSystem::bindForce: vtable, a2, 0, a2 (16 bytes).
struct wave_07_force_proxy {
    std::uint32_t vtable;
    std::uint32_t arg_a;
    std::uint32_t zero;
    std::uint32_t arg_b;
};
static_assert(sizeof(wave_07_force_proxy) == 0x10, "force proxy is 16 bytes");

// Weighted sum of key vectors, or a copy when count is 1 (the original's rule).
void wave_07_blend_vec3(const float* values, const float* weights, int count, float* out);

// Two-key lerp of dequantized keys a2 and a3 (the CInterpreter body for short and char).
template <typename T>
float wave_07_lerp_quantized(const wave_07_quantized<T>& track, int a2, int a3, float a4, float* out) {
    float va[3];
    float vb[3];
    const T* key_a = track.base + 3 * a2;
    const T* key_b = track.base + 3 * a3;
    for (int c = 0; c < 3; ++c) {
        va[c] = static_cast<float>(key_a[c]) * track.scale[c] + track.offset[c];
        vb[c] = static_cast<float>(key_b[c]) * track.scale[c] + track.offset[c];
    }
    const float one_minus = 1.0f - a4;
    for (int c = 0; c < 3; ++c) {
        out[c] = (a4 * vb[c]) + ((one_minus * va[c]) + 0.0f);
    }
    return out[0];
}

// Two-key lerp over a float vec3 block (the CInterpreter body for float[3] material keys).
float wave_07_lerp_float_pair(const float* base, int a2, float a4, float* out);

// IParticleSystem::bindForce common body. base_of gives the target, the proxy is owned by the callee.
int wave_07_bind_force(void* self, std::uint32_t a2, std::uint32_t proxy_vtable, const wave_07_bind_ports& ports);

// Force scene node bind: forwards bindForce with the node's particle system and force address.
int wave_07_bind_node(void* self, void* node, std::uint32_t proxy_vtable, const wave_07_node_ports& nodes,
                      const wave_07_bind_ports& ports);

// 0x626a38 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x626a38(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set);

// 0x626c30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x626c30(const void* self, const float* values, const float* weights, int count, float* out);

// 0x626d14 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x626d14(const void* self, const float* values, const float* weights, int count, float* out);

// 0x626edc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x626edc(const void* self, const float* values, const float* weights, int count, float* out);

// 0x626fc0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x626fc0(const void* self, const float* values, const float* weights, int count, float* out);

// 0x627188 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x627188(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62726c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62726c(const void* self, const float* values, const float* weights, int count, float* out);

// 0x627434 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x627434(const void* self, const float* values, const float* weights, int count, float* out);

// 0x627518 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x627518(const void* self, const float* values, const float* weights, int count, float* out);

// 0x6276e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x6276e0(const void* self, const float* values, const float* weights, int count, float* out);

// 0x6277c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x6277c4(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62798c _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62798c(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set);

// 0x627aa0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x627aa0(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set);

// 0x627bb4 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIsEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x627bb4(const wave_07_quantized<std::int16_t>& track, int a2, int a3, float a4, float* out);

// 0x627d14 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x627d14(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call);

// 0x627d98 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x627d98(const wave_07_quantized<std::int8_t>& track, int a2, int a3, float a4, float* out);

// 0x627efc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x627efc(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call);

// 0x627f80 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIcEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x627f80(const wave_07_quantized<std::int8_t>& track, int a2, int a3, float a4, float* out);

// 0x6280e4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x6280e4(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call);

// 0x628168 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x628168(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set);

// 0x62827c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_blended_0x62827c(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x628390 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_added_0x628390(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x6285b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_added_0x6285b8(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x6287cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x6287cc(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call);

// 0x628950 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x628950(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call);

// 0x6289d4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELin1EfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_float_0x6289d4(const float* base, int a2, int a3, float a4, float* out);

// 0x628af0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_material_0x628af0(const wave_07_key_interp& interp, int a2, int a3, float a4, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set);

// 0x628b48 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x628b48(const wave_07_quantized<std::int16_t>& track, int a2, int a3, float a4, float* out);

// 0x628ca8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x628ca8(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call);

// 0x628e40 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x628e40(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x628f54 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x628f54(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629068 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629068(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62917c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62917c(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629290 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629290(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x6294b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x6294b8(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x6295cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x6295cc(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x6296e0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x6296e0(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x6297f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x6297f4(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629908 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629908(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629b30 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629b30(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629c44 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_blended_0x629c44(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629d58 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_added_0x629d58(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x629e6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x629e6c(const void* self, const float* values, const float* weights, int count, float* out);

// 0x629f50 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x629f50(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a118 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a118(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a1fc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a1fc(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a3c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a3c4(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a4a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a4a8(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a58c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a58c(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a670 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a670(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a754 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62a754(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a838 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62a838(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62a91c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62a91c(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62aa00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62aa00(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62aae4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62aae4(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62abc8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62abc8(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62acac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62acac(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62ad90 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62ad90(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62ae74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62ae74(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62af58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62af58(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b03c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b03c(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b120 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b120(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b204 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62b204(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b2e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62b2e8(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b3cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62b3cc(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b4b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b4b0(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b594 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b594(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b678 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b678(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b75c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b75c(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b840 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b840(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62b924 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b924(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62ba08 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62ba08(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62baec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62baec(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62bbd0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62bbd0(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62bcb4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62bcb4(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62bd98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62bd98(const void* self, const float* values, const float* weights, int count, float* out);

// 0x62be7c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62be7c(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62bf90 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62bf90(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62c0a4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62c0a4(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62c1b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62c1b8(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62c3e0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62c3e0(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62c4f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62c4f4(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62c608 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62c608(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call);

// 0x62ff08 _ZN6glitch7collada15CColladaFactory21getVertexBufferConfigERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
wave_07_buffer_config* get_vertex_buffer_config_0x62ff08(const CColladaDatabase& db, const SGeometry* geom,
                                                           wave_07_buffer_config* result);

// 0x62ff20 _ZN6glitch7collada15CColladaFactory20getIndexBufferConfigERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
wave_07_buffer_config* get_index_buffer_config_0x62ff20(const CColladaDatabase& db, const SGeometry* geom,
                                                          wave_07_buffer_config* result);

// 0x62ff80 _ZN6glitch7collada12_GLOBAL__N_132getExternalLightSceneNodeDefaultEPKc
void* get_external_light_0x62ff80(const char* name, const wave_07_resource_lookup& lookup, const wave_07_tag_of& tag_of);

// 0x630018 _ZN6glitch2ps15IParticleSystemINS0_12GNPSParticleEE9bindForceINS0_5PWindEEEiRNT_15parameters_typeE
int bind_force_0x630018(void* self, std::uint32_t a2, const wave_07_bind_ports& ports);

// 0x630080 _ZN6glitch7collada15particle_system19CWindForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
int bind_node_0x630080(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports);

// 0x630090 _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE9bindForceINS0_5PWindEEEiRNT_15parameters_typeE
int bind_force_0x630090(void* self, std::uint32_t a2, const wave_07_bind_ports& ports);

// 0x6300f8 _ZN6glitch7collada15particle_system19CWindForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
int bind_node_0x6300f8(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports);

// 0x630108 _ZN6glitch2ps15IParticleSystemINS0_12GNPSParticleEE9bindForceINS0_8PGravityEEEiRNT_15parameters_typeE
int bind_force_0x630108(void* self, std::uint32_t a2, const wave_07_bind_ports& ports);

// 0x630170 _ZN6glitch7collada15particle_system22CGravityForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
int bind_node_0x630170(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports);

// 0x630180 _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE9bindForceINS0_8PGravityEEEiRNT_15parameters_typeE
int bind_force_0x630180(void* self, std::uint32_t a2, const wave_07_bind_ports& ports);

// 0x6301e8 _ZN6glitch7collada15particle_system22CGravityForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
int bind_node_0x6301e8(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports);

}  // namespace glitch::collada::recon_v2_07

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_07_HPP
