#pragma once
#include "character_fx_kernels_v1.hpp"
#include "../asset-payloads/payloads.hpp"
#include <string>
namespace dh2::fx {
// One genuine CTextureTransformEx accessor. The immutable BRES image owns the
// default tuple and key vectors; it must outlive this borrowed accessor.
bool texture_sample_v1(TextureTransform20V1&,const assets::Animation&,std::int32_t ms,bool interpolate,std::string&);
}
extern "C" int dh2_fx_texture_sample_test_v1(void* output20,const void* bytes,std::uint32_t size,std::int32_t animation,std::int32_t segment,std::int32_t ms,std::uint32_t interpolate);
