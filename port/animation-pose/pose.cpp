#include "pose.hpp"
#include "../animation-values/values.hpp"
#include <cmath>
#include <cstring>
#include <limits>

using namespace dh2::pose;
namespace {
bool quaternion_ok(const float *q) {
    const float n = ((q[0] * q[0] + q[1] * q[1]) + q[2] * q[2]) + q[3] * q[3];
    return std::isfinite(n) && n > 0.5f && n < 1.5f;
}
struct Context {
    const Clip *clip;
    const dh2::skin::Skin *skin;
    std::int32_t time;
    dh2::math::Matrix4f worlds[256];
    bool found[256];
    std::uint32_t matches[128];
    std::uint32_t ancestors[64], nodes;
};
std::uint32_t word(const std::uint8_t *p) {
    return p[0] | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16) |
           (std::uint32_t(p[3]) << 24);
}
Error walk(Context &c, const dh2::scene::Node &source, const dh2::math::Matrix4f *parent,
           std::uint32_t depth) {
    if (depth >= 64 || c.nodes >= 20000)
        return Error::limit;
    for (std::uint32_t i = 0; i < depth; ++i)
        if (c.ancestors[i] == source.record)
            return Error::scene;
    c.ancestors[depth] = source.record;
    ++c.nodes;
    for (std::uint32_t i = 0; i < c.clip->count; ++i) {
        const auto* target = dh2_animation_target(&c.clip->tracks[i]);
        if (target && std::strcmp(target, source.id) == 0) ++c.matches[i];
    }
    dh2::scene::Node posed{};
    auto e = dh2_pose_node(c.clip, c.time, &source, &posed);
    if (e != Error::ok)
        return e;
    dh2::math::Matrix4f world{};
    if (dh2_scene_world_matrix(&posed, parent, &world) != dh2::scene::Error::ok)
        return Error::scene;
    const auto &image = source.image;
    if (source.record > image.size || image.size - source.record < 12)
        return Error::range;
    const auto o = word(image.bytes + source.record + 8);
    if (o >= image.size)
        return Error::range;
    if (!std::memchr(image.bytes + o, 0, image.size - o))
        return Error::range;
    const auto *scope = reinterpret_cast<const char *>(image.bytes + o);
    for (std::uint32_t j = 0; j < c.skin->joints; ++j) {
        const auto *name = dh2_skin_joint_name(c.skin, j);
        if (name && std::strcmp(name, scope) == 0) {
            if (c.found[j])
                return Error::duplicate;
            c.found[j] = true;
            c.worlds[j] = world;
        }
    }
    for (std::uint32_t i = 0; i < source.children; ++i) {
        dh2::scene::Node child{};
        if (dh2_scene_child_node(&source, i, &child) != dh2::scene::Error::ok)
            return Error::scene;
        e = walk(c, child, &world, depth + 1);
        if (e != Error::ok)
            return e;
    }
    return Error::ok;
}
} // namespace
extern "C" {
Error dh2_pose_clip_open(Clip *out, const dh2::resources::BresView *image, std::int32_t segment) {
    if (!out)
        return Error::argument;
    *out = {};
    if (!image)
        return Error::argument;
    const auto n = dh2_bres_library_count(image, dh2::resources::Library::animation);
    if (!n || n > 128)
        return Error::limit;
    // Fill caller storage after each validation; count is committed only when
    // the entire clip passes. Error paths cannot expose partially usable clips.
    out->start = std::numeric_limits<std::int32_t>::max();
    out->end = std::numeric_limits<std::int32_t>::min();
    for (std::uint32_t i = 0; i < n; ++i) {
        dh2::assets::Animation a{};
        if (dh2_animation_open(&a, image, i, segment) != dh2::assets::Error::ok)
            return Error::range;
        if (dh2_animation_channels(&a) != 1 || dh2_animation_samplers(&a) != 1 ||
            dh2_animation_animator(&a) != 0 || dh2_animation_scales(&a) ||
            dh2_animation_offsets(&a))
            return Error::unsupported;
        const auto type = dh2_animation_type(&a, 0);
        const auto components = type == 5 ? 4U : 3U;
        if (type != 1 && type != 5 && type != 10)
            return Error::unsupported;
        const auto *target = dh2_animation_target(&a);
        if (!target || !target[0])
            return Error::range;
        for (std::uint32_t j = 0; j < i; ++j)
            if (dh2_animation_type(&out->tracks[j], 0) == type &&
                std::strcmp(dh2_animation_target(&out->tracks[j]), target) == 0)
                return Error::duplicate;
        dh2::assets::Vector times{}, values{};
        if (!dh2_animation_vector(&a, 0, false, &times) ||
            !dh2_animation_vector(&a, 0, true, &values))
            return Error::range;
        if (values.type != 6 || values.components != components || !times.count ||
            times.count != values.count || times.count > 100000 || times.components != 1 ||
            (times.type != 1 && times.type != 3 && times.type != 4) ||
            dh2_animation_interpolation(&a, 0) > 1)
            return Error::unsupported;
        std::int32_t previous = 0;
        for (std::uint32_t k = 0; k < values.count; ++k) {
            const auto time = dh2_animation_key_time(&a, 0, k);
            if (k && time <= previous)
                return Error::keys;
            previous = time;
            float v[16]{};
            if (!dh2_vector_read(&values, k, v))
                return Error::range;
            for (std::uint32_t x = 0; x < components; ++x)
                if (!std::isfinite(v[x]))
                    return Error::nonfinite;
            if (type == 5 && !quaternion_ok(v))
                return Error::nonfinite;
        }
        const auto start = dh2_animation_start(&a, 0), end = dh2_animation_end(&a, 0);
        if (start < out->start)
            out->start = start;
        if (end > out->end)
            out->end = end;
        out->tracks[i] = a;
    }
    if (std::int64_t(out->end) - out->start > std::numeric_limits<std::int32_t>::max())
        return Error::limit;
    out->count = n;
    return Error::ok;
}
Error dh2_pose_sample(const Clip *clip, std::uint32_t track, std::int32_t ms, float *out) {
    if (!clip || !out)
        return Error::argument;
    if (track >= clip->count || clip->count > 128)
        return Error::range;
    const auto &a = clip->tracks[track];
    dh2::assets::Vector values{};
    if (!dh2_animation_vector(&a, 0, true, &values) || !values.count || values.type != 6 ||
        values.components > 4)
        return Error::range;
    std::int32_t key = 0;
    float fraction = 0;
    const bool interpolate = dh2_animation_find(&a, 0, ms, &key, &fraction);
    if (key < 0 || std::uint32_t(key) >= values.count)
        return Error::keys;
    const auto result = interpolate
        ? dh2_animation_float_interpolate(&a, key, std::uint32_t(key) + 1, fraction, out)
        : dh2_animation_float_key(&a, key, out);
    switch (result) {
    case dh2::animation::Error::ok: return Error::ok;
    case dh2::animation::Error::argument: return Error::argument;
    case dh2::animation::Error::unsupported: return Error::unsupported;
    case dh2::animation::Error::nonfinite: return Error::nonfinite;
    default: return Error::range;
    }
}
Error dh2_pose_node(const Clip *clip, std::int32_t ms, const dh2::scene::Node *source,
                    dh2::scene::Node *out) {
    if (!clip || !source || !out || !source->id)
        return Error::argument;
    if (!clip->count || clip->count > 128)
        return Error::range;
    auto result = *source;
    for (std::uint32_t i = 0; i < clip->count; ++i) {
        const auto &a = clip->tracks[i];
        const auto *target = dh2_animation_target(&a);
        if (!target)
            return Error::range;
        if (std::strcmp(target, source->id) != 0)
            continue;
        float v[4]{};
        const auto e = dh2_pose_sample(clip, i, ms, v);
        if (e != Error::ok)
            return e;
        const auto type = dh2_animation_type(&a, 0);
        if (type == 5) {
            if (!quaternion_ok(v))
                return Error::nonfinite;
            std::memcpy(result.rotation, v, 16);
        } else if (type == 1)
            std::memcpy(result.position, v, 12);
        else if (type == 10)
            std::memcpy(result.scale, v, 12);
        else
            return Error::unsupported;
    }
    *out = result;
    return Error::ok;
}
Error dh2_pose_skin_palette(const Clip *clip, std::int32_t ms, const dh2::skin::Skin *skin,
                            const dh2::scene::Visual *visual, dh2::math::Matrix4f *out,
                            std::size_t cap) {
    if (!clip || !skin || !visual || !out || skin->image.bytes != visual->image.bytes)
        return Error::argument;
    if (!clip->count || clip->count > 128 || !skin->joints || skin->joints > 256 || cap < skin->joints)
        return Error::limit;
    Context c{};
    c.clip = clip;
    c.skin = skin;
    c.time = ms;
    for (std::uint32_t i = 0; i < visual->roots; ++i) {
        dh2::scene::Node node{};
        if (dh2_scene_root_node(visual, i, &node) != dh2::scene::Error::ok)
            return Error::scene;
        const auto e = walk(c, node, nullptr, 0);
        if (e != Error::ok)
            return e;
    }
    for (std::uint32_t j = 0; j < skin->joints; ++j)
        if (!c.found[j])
            return Error::joint;
    for (std::uint32_t i = 0; i < clip->count; ++i)
        if (c.matches[i] != 1) return Error::scene;
    return dh2_skin_palette(skin, c.worlds, skin->joints, out, cap) == dh2::skin::Error::ok
               ? Error::ok
               : Error::nonfinite;
}
}
