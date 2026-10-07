#pragma once
#include "../engine-math/math.hpp"
#include <cstdint>
namespace dh2::fx {
// Native projections of source AnimFX/AnimFXData, never ARM32 overlays.
struct FxStep24V1 {
 // Authored table labels: source byte+10 OrientOnce, +11 OrientWithAnchor.
 std::uint32_t orient_once{},orient_with_anchor{},scale_with_anchor{};
 float speed{1};std::int32_t loop{},play_time{-1};
};
struct FxData32V1 {
 // Actual GetAnimFXData swaps those two bytes before SetAnimFX consumes them.
 std::uint32_t orient_once{},orient_with_anchor{},scale_with_anchor{};
 float speed{1};std::int32_t loop{},play_time{-1};std::uintptr_t set_identity{};
};
struct TextureTransform20V1 {float offset_u{},offset_v{},rotation_degrees{},scale_u{1},scale_v{1};};
static_assert(sizeof(FxStep24V1)==24&&sizeof(FxData32V1)==32&&sizeof(TextureTransform20V1)==20);
}
extern "C" {
// Exact GetAnimFXData4933e4. Invalid native projections reject before writes.
int dh2_fx_data_v1(dh2::fx::FxData32V1*,const dh2::fx::FxStep24V1*,std::int32_t set_type,std::int32_t set_loop,std::uintptr_t set_identity);
// Original texture sampler's scalar key/between/delta arithmetic. Key selection
// and channel-to-field routing are supplied by the genuine accessor caller.
int dh2_fx_texture_key_v1(float*,const float*,std::uint32_t count,std::uint32_t key);
int dh2_fx_texture_between_v1(float*,const float*,std::uint32_t count,std::uint32_t key,std::uint32_t next,float fraction);
int dh2_fx_texture_delta_v1(float*,const float*,std::uint32_t count,std::uint32_t reference,std::uint32_t key,std::uint32_t next,float fraction);
// CTextureTransformEx.applyValueEx6e38c4/buildTextureTransform6e377c: rotation
// conversion and center(.5,.5), source matrix layout and identity hint.
int dh2_fx_texture_matrix_v1(dh2::math::Matrix4f*,const dh2::fx::TextureTransform20V1*);
}
