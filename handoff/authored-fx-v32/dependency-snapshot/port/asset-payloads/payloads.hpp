#pragma once
#include "../engine-resources/resources.hpp"

// Borrowed, immutable views. The source BRES bytes must outlive every view.
// These are new port interfaces, not the original ARM32 C++ object ABI.
namespace dh2::assets {
using dh2::resources::BresView;
enum class Error : std::uint32_t {
    ok, argument, range, geometry_type, stream_layout, attribute, primitive,
    index, segment_state, animation, vector, scalar_type, string
};
struct Mesh {
    BresView image;
    const char* id;
    const char* name;
    std::uint32_t vertices, stride, attributes, primitives;
    std::uint32_t stream, buffers;
    float minimum[3], maximum[3];
};
struct Attribute {
    const std::uint8_t* data;
    std::uint32_t type, components, stride, vertices;
};
struct Primitive {
    const char* material;
    const std::uint8_t* indices;
    std::uint32_t collada_type, engine_type, declared_count;
    std::uint32_t index_count, index_width, minimum_index, maximum_index;
    std::int32_t attributes[18];
};
struct Vector {
    const std::uint8_t* data;
    std::uint32_t count, type, components;
};
struct Animation {
    BresView image;
    std::uint32_t record, data, data_size, entries;
    std::int32_t segment_start, segment_end;
};
}

extern "C" {
dh2::assets::Error dh2_mesh_open(dh2::assets::Mesh*, const dh2::resources::BresView*, std::int32_t geometry);
dh2::assets::Error dh2_mesh_attribute(const dh2::assets::Mesh*, std::int32_t, dh2::assets::Attribute*);
dh2::assets::Error dh2_mesh_primitive(const dh2::assets::Mesh*, std::int32_t, dh2::assets::Primitive*);
// Raw numeric values, without shader normalization or coordinate transforms.
bool dh2_attribute_read(const dh2::assets::Attribute*, std::uint32_t vertex, float* components);
bool dh2_index_read(const dh2::assets::Primitive*, std::uint32_t index, std::uint32_t*);
std::uint32_t dh2_animation_segments(const dh2::resources::BresView*);
dh2::assets::Error dh2_animation_open(dh2::assets::Animation*, const dh2::resources::BresView*, std::int32_t animation, std::int32_t segment);
const char* dh2_animation_target(const dh2::assets::Animation*);
std::uint32_t dh2_animation_type(const dh2::assets::Animation*, std::int32_t channel);
const std::uint8_t* dh2_animation_channel(const dh2::assets::Animation*, std::int32_t);
std::uint32_t dh2_animation_channels(const dh2::assets::Animation*);
std::uint32_t dh2_animation_samplers(const dh2::assets::Animation*);
std::uint32_t dh2_animation_time_type(const dh2::assets::Animation*, std::int32_t);
std::uint32_t dh2_animation_interpolation(const dh2::assets::Animation*, std::int32_t);
std::uint32_t dh2_animation_animator(const dh2::assets::Animation*);
bool dh2_animation_has_default(const dh2::assets::Animation*);
const std::uint8_t* dh2_animation_default(const dh2::assets::Animation*);
const std::uint8_t* dh2_animation_offsets(const dh2::assets::Animation*);
const std::uint8_t* dh2_animation_scales(const dh2::assets::Animation*);
std::uint32_t dh2_animation_scale_type(const dh2::assets::Animation*);
bool dh2_animation_vector(const dh2::assets::Animation*, std::int32_t sampler, bool output, dh2::assets::Vector*);
bool dh2_vector_read(const dh2::assets::Vector*, std::uint32_t key, float* components);
std::int32_t dh2_animation_key_time(const dh2::assets::Animation*, std::int32_t sampler, std::int32_t key);
std::int32_t dh2_animation_start(const dh2::assets::Animation*, std::int32_t);
std::int32_t dh2_animation_end(const dh2::assets::Animation*, std::int32_t);
std::int32_t dh2_animation_length(const dh2::assets::Animation*, std::int32_t);
// Returns whether interpolation is active. Always writes the selected key on
// valid input; leaves fraction untouched when interpolation is inactive.
// Input key times must be ordered, as required by the original binary search.
bool dh2_animation_find_raw_index(const dh2::assets::Vector*, std::int32_t milliseconds, std::int32_t* key);
bool dh2_animation_find_index(const dh2::assets::Animation*, std::int32_t sampler, std::int32_t milliseconds, std::int32_t* key);
bool dh2_animation_find(const dh2::assets::Animation*, std::int32_t sampler, std::int32_t milliseconds, std::int32_t* key, float* fraction);
}
