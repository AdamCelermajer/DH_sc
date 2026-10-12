#pragma once
#include <cstdint>

namespace dh2::animation {
// Native borrowed projection of SAnimationAccessor, not its ARM32 layout.
// Four-byte colors retain source storage order; component3 is the alpha byte.
// The component3 interpreter tests whether default metadata exists. The key
// path tolerates a null value pointer; interpolated paths dereference it.
struct ColorAccessor24 {
    const std::uint8_t* values;
    std::uint32_t count, has_default;
    const std::uint8_t* default_value;
};
// One already resolved runtime material parameter. Directory/name conversion,
// ownership and shader binding are required outside this bounded projection.
// type8 stores four float words; type16 stores packed bytes in words[0].
// stamps project CMaterial+0xc/+0x10, which become -1 on numerical change.
struct ColorParameter32 {
    std::uint32_t type, element_count, words[4];
    std::int32_t stamp_c, stamp_10;
};
static_assert(sizeof(ColorAccessor24)==24);
static_assert(sizeof(ColorParameter32)==32);
}
extern "C" {
// 0 complete, -1 malformed/overlapping caller storage, -2 original unsafe
// default-pointer case. A missing metadata record writes output byte0.
int dh2_material_alpha_key(std::uint8_t* output,const dh2::animation::ColorAccessor24*,std::uint32_t key);
int dh2_material_alpha_between(std::uint8_t* output,const dh2::animation::ColorAccessor24*,std::uint32_t key,std::uint32_t next,float fraction);
int dh2_material_alpha_delta(std::uint8_t* output,const dh2::animation::ColorAccessor24*,std::uint32_t reference,std::uint32_t key,std::uint32_t next,float fraction);
// Source count0 zeroes; count1 ignores its weight; other counts perform ordered
// f32 multiplication/addition, then original unsigned conversion and low byte.
int dh2_material_color_blend(std::uint8_t* output,const std::uint8_t* contiguous_color,const float* weights,std::int32_t count);
// 1 source acceptance, 0 source element-range rejection, -1 malformed caller,
// -2 unsupported runtime storage type. No name/factory acceptance is implied.
int dh2_material_color_set(dh2::animation::ColorParameter32*,std::uint32_t element,const std::uint8_t* color);
}
