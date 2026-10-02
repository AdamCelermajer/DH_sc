#include "layers.hpp"
#include "../animation-mixing/mixing.hpp"
#include "../animation-values/values.hpp"
#include <cmath>
#include <cstring>

using dh2::pose::Error;
using dh2::layers::Layers;
namespace {
Error weights(const Layers *layers, float *values) {
    if (!layers) return Error::argument;
    if (!layers->count || layers->count > 8) return Error::limit;
    for (std::uint32_t i = 0; i < layers->count; ++i) {
        const auto &layer = layers->items[i];
        if (!layer.clip) return Error::argument;
        if (!layer.clip->count || layer.clip->count > 128) return Error::limit;
        if (!std::isfinite(layer.weight)) return Error::nonfinite;
        if (layer.weight < 0) return Error::range;
        values[i] = layer.weight;
    }
    return dh2_animation_weights_normalize(values, layers->count) == dh2::mixing::Error::ok
        ? Error::ok : Error::nonfinite;
}
Error sample_node(const Layers &layers, const float *weights,
                  const dh2::scene::Node &source, dh2::scene::Node &out) {
    dh2::math::Vector3f positions[8], scales[8];
    dh2::math::Quaternion rotations[8];
    for (std::uint32_t i = 0; i < layers.count; ++i) {
        dh2::scene::Node posed{};
        const auto &layer = layers.items[i];
        const auto error = dh2_pose_node(layer.clip, layer.time, &source, &posed);
        if (error != Error::ok) return error;
        std::memcpy(&positions[i], posed.position, 12);
        std::memcpy(&rotations[i], posed.rotation, 16);
        std::memcpy(&scales[i], posed.scale, 12);
    }
    dh2::math::Vector3f position{}, scale{};
    dh2::math::Quaternion rotation{};
    if (dh2_animation_vector_mix(positions, weights, layers.count, &position) != dh2::mixing::Error::ok ||
        dh2_animation_vector_mix(scales, weights, layers.count, &scale) != dh2::mixing::Error::ok ||
        dh2_animation_quaternion_blend(rotations, weights, layers.count, &rotation) != dh2::animation::Error::ok)
        return Error::nonfinite;
    out = source;
    std::memcpy(out.position, &position, 12);
    std::memcpy(out.rotation, &rotation, 16);
    std::memcpy(out.scale, &scale, 12);
    return Error::ok;
}
struct Context {
    const Layers *layers;
    const dh2::skin::Skin *skin;
    float weights[8];
    dh2::math::Matrix4f worlds[256];
    bool found[256];
    std::uint32_t matches[8][128], ancestors[64], nodes;
};
std::uint32_t word(const std::uint8_t *p) {
    return p[0] | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
Error walk(Context &c, const dh2::scene::Node &source,
           const dh2::math::Matrix4f *parent, std::uint32_t depth) {
    if (!source.id) return Error::scene;
    if (depth >= 64 || c.nodes >= 20000) return Error::limit;
    for (std::uint32_t i = 0; i < depth; ++i)
        if (c.ancestors[i] == source.record) return Error::scene;
    c.ancestors[depth] = source.record; ++c.nodes;
    for (std::uint32_t layer = 0; layer < c.layers->count; ++layer) {
        const auto *clip = c.layers->items[layer].clip;
        for (std::uint32_t track = 0; track < clip->count; ++track) {
            const auto *target = dh2_animation_target(&clip->tracks[track]);
            if (target && std::strcmp(target, source.id) == 0) ++c.matches[layer][track];
        }
    }
    dh2::scene::Node posed{};
    const auto error = sample_node(*c.layers, c.weights, source, posed);
    if (error != Error::ok) return error;
    dh2::math::Matrix4f world{};
    if (dh2_scene_world_matrix(&posed, parent, &world) != dh2::scene::Error::ok) return Error::scene;
    const auto &image = source.image;
    if (source.record > image.size || image.size - source.record < 12) return Error::range;
    const auto offset = word(image.bytes + source.record + 8);
    if (offset >= image.size || !std::memchr(image.bytes + offset, 0, image.size - offset)) return Error::range;
    const auto *scope = reinterpret_cast<const char *>(image.bytes + offset);
    for (std::uint32_t i = 0; i < c.skin->joints; ++i) {
        const auto *name = dh2_skin_joint_name(c.skin, i);
        if (name && std::strcmp(name, scope) == 0) {
            if (c.found[i]) return Error::duplicate;
            c.found[i] = true; c.worlds[i] = world;
        }
    }
    for (std::uint32_t i = 0; i < source.children; ++i) {
        dh2::scene::Node child{};
        if (dh2_scene_child_node(&source, i, &child) != dh2::scene::Error::ok) return Error::scene;
        const auto e = walk(c, child, &world, depth + 1);
        if (e != Error::ok) return e;
    }
    return Error::ok;
}
bool overlaps(const void *p, std::size_t n, const void *q, std::size_t m) {
    const auto a = reinterpret_cast<std::uintptr_t>(p), b = reinterpret_cast<std::uintptr_t>(q);
    return a <= b ? b - a < n : a - b < m;
}
}
extern "C" Error dh2_layers_node(const Layers *layers,
    const dh2::scene::Node *source, dh2::scene::Node *out) {
    if (!source || !out) return Error::argument;
    float normalized[8];
    const auto e = weights(layers, normalized);
    if (e != Error::ok) return e;
    dh2::scene::Node result{};
    const auto sampled = sample_node(*layers, normalized, *source, result);
    if (sampled == Error::ok) *out = result;
    return sampled;
}
extern "C" Error dh2_layers_skin_palette(const Layers *layers,
    const dh2::skin::Skin *skin, const dh2::scene::Visual *visual,
    dh2::math::Matrix4f *out, std::size_t capacity) {
    if (!skin || !visual || !out || skin->image.bytes != visual->image.bytes) return Error::argument;
    if (!skin->joints || skin->joints > 256 || capacity < skin->joints) return Error::limit;
    Context c{}; c.layers = layers; c.skin = skin;
    const auto e = weights(layers, c.weights);
    if (e != Error::ok) return e;
    const auto size = skin->joints * sizeof(*out);
    if (overlaps(skin->image.bytes, skin->image.size, out, size) || overlaps(layers, sizeof(*layers), out, size))
        return Error::argument;
    for (std::uint32_t i = 0; i < layers->count; ++i) {
        const auto *clip = layers->items[i].clip;
        if (overlaps(clip, sizeof(*clip), out, size)) return Error::argument;
        for (std::uint32_t j = 0; j < clip->count; ++j)
            if (overlaps(clip->tracks[j].image.bytes, clip->tracks[j].image.size, out, size)) return Error::argument;
    }
    for (std::uint32_t i = 0; i < visual->roots; ++i) {
        dh2::scene::Node node{};
        if (dh2_scene_root_node(visual, i, &node) != dh2::scene::Error::ok) return Error::scene;
        const auto result = walk(c, node, nullptr, 0);
        if (result != Error::ok) return result;
    }
    for (std::uint32_t i = 0; i < skin->joints; ++i) if (!c.found[i]) return Error::joint;
    for (std::uint32_t i = 0; i < layers->count; ++i)
        for (std::uint32_t j = 0; j < layers->items[i].clip->count; ++j)
            if (c.matches[i][j] != 1) return Error::scene;
    dh2::math::Matrix4f result[256]{};
    if (dh2_skin_palette(skin, c.worlds, skin->joints, result, 256) != dh2::skin::Error::ok) return Error::nonfinite;
    std::memcpy(out, result, size);
    return Error::ok;
}
