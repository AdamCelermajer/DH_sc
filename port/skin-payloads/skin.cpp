#include "skin.hpp"
#include <cmath>
#include <cstring>
#include <limits>

using namespace dh2::skin;
namespace {
std::uint32_t word(const std::uint8_t* p) {
    return p[0] | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16)
        | (std::uint32_t(p[3]) << 24);
}
float real(const std::uint8_t* p) {
    const auto bits = word(p); float f; std::memcpy(&f, &bits, 4); return f;
}
const std::uint8_t* at(const BresView& v, std::uint64_t o, std::uint64_t n) {
    return v.bytes && o <= v.size && n <= v.size - o ? v.bytes + o : nullptr;
}
const char* text(const BresView& v, std::uint32_t o) {
    if (!o || !at(v, o, 1)) return nullptr;
    const auto* end = std::memchr(v.bytes + o, 0, v.size - o);
    return end ? reinterpret_cast<const char*>(v.bytes + o) : nullptr;
}
Error matrix(const std::uint8_t* p, Matrix4f& m) {
    if (!p) return Error::range;
    for (int i = 0; i < 16; ++i) {
        m.m[i] = real(p + i * 4);
        if (!std::isfinite(m.m[i])) return Error::nonfinite;
    }
    if (m.m[3] != 0 || m.m[7] != 0 || m.m[11] != 0 || m.m[15] != 1)
        return Error::layout;
    m.identity_hint = 0;
    return Error::ok;
}
bool finite(const Matrix4f& m) {
    for (float x : m.m) if (!std::isfinite(x)) return false;
    return m.m[3] == 0 && m.m[7] == 0 && m.m[11] == 0 && m.m[15] == 1;
}
void multiply(Matrix4f& out, const Matrix4f& a, const Matrix4f& b) {
    Matrix4f result{};
    for (int c = 0; c < 4; ++c) for (int r = 0; r < 4; ++r)
        result.m[c * 4 + r] = ((a.m[r] * b.m[c * 4] + a.m[4 + r] * b.m[c * 4 + 1])
            + a.m[8 + r] * b.m[c * 4 + 2]) + a.m[12 + r] * b.m[c * 4 + 3];
    out = result;
}
struct Resolve {
    const Skin* skin;
    Matrix4f worlds[256];
    bool found[256];
    Error error;
};
bool resolve_node(const dh2::scene::Node* node, const Matrix4f* world,
                  std::uint32_t, void* user) {
    auto& r = *static_cast<Resolve*>(user);
    const auto* p = at(node->image, std::uint64_t(node->record) + 8, 4);
    if (!p) { r.error = Error::range; return false; }
    const auto* scope = text(node->image, word(p));
    if (!scope) { r.error = Error::string; return false; }
    if (!scope[0]) return true;
    for (std::uint32_t j = 0; j < r.skin->joints; ++j) {
        const auto* name = dh2_skin_joint_name(r.skin, j);
        if (name && std::strcmp(name, scope) == 0) {
            if (r.found[j]) { r.error = Error::joint; return false; }
            r.found[j] = true; r.worlds[j] = *world;
        }
    }
    return true;
}
}

extern "C" {
Error dh2_skin_open(Skin* out, const BresView* v, std::int32_t controller) {
    if (!out) return Error::argument;
    *out = {};
    if (!v) return Error::argument;
    const auto* c = dh2_bres_library_item(v, dh2::resources::Library::controller, controller);
    if (!c) return Error::range;
    if (word(c) != 0) return Error::controller_type;
    const auto offset = word(c + 8);
    const auto* p = at(*v, offset, 156);
    if (!p) return Error::range;
    Skin s{}; s.image = *v; s.id = text(*v, word(c + 4)); s.record = offset;
    s.geometry_url = text(*v, word(p + 0x70));
    if (!s.id || !s.geometry_url) return Error::string;
    s.joints = word(p + 0x74); s.joint_names = word(p + 0x78);
    s.inverse_matrices = word(p + 4);
    s.influences = p[0x98]; s.weight_stride = (s.influences + 1) * 4;
    const auto weight_words = word(p + 0x7c);
    if (weight_words % (s.influences + 1)) return Error::layout;
    s.vertices = weight_words / (s.influences + 1);
    if (!s.joints || s.joints > 256 || !s.influences || s.influences > 4 ||
        word(p) != s.joints * 16 || !s.vertices) return Error::layout;
    if (!s.joint_names || !s.inverse_matrices ||
        !at(*v, s.joint_names, std::uint64_t(s.joints) * 4) ||
        !at(*v, s.inverse_matrices, std::uint64_t(s.joints) * 64)) return Error::range;
    auto error = matrix(p + 0x10, s.bind_shape);
    if (error != Error::ok) return error;
    const auto* root = at(*v, v->root_offset, 192);
    if (!root) return Error::range;
    s.weights = word(p + 0x80);
    const auto weight_bytes = std::uint64_t(s.vertices) * s.weight_stride;
    if (word(root + 100)) {
        const auto* d = at(*v, s.weights, 16);
        if (!d || word(d + 12) || weight_bytes > (word(d + 8) & ~3U)) return Error::range;
        s.weights = word(d + 4);
    }
    if (!s.weights || !at(*v, s.weights, weight_bytes)) return Error::range;
    s.geometry_index = -1;
    if (s.geometry_url[0] != '#' || !s.geometry_url[1]) return Error::geometry;
    const auto count = dh2_bres_library_count(v, dh2::resources::Library::geometry);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto* g = dh2_bres_library_item(v, dh2::resources::Library::geometry, i);
        const auto* id = text(*v, word(g));
        if (id && std::strcmp(id, s.geometry_url + 1) == 0) {
            dh2::assets::Mesh m{};
            if (dh2_mesh_open(&m, v, i) != dh2::assets::Error::ok || m.vertices != s.vertices)
                return Error::geometry;
            s.geometry_index = static_cast<std::int32_t>(i); break;
        }
    }
    if (s.geometry_index < 0) return Error::geometry;
    for (std::uint32_t j = 0; j < s.joints; ++j) {
        if (!dh2_skin_joint_name(&s, j)) return Error::string;
        Matrix4f m{}; error = dh2_skin_inverse_bind(&s, j, &m);
        if (error != Error::ok) return error;
    }
    for (std::uint32_t i = 0; i < s.vertices; ++i) {
        Influence influence{}; error = dh2_skin_influence(&s, i, &influence);
        if (error != Error::ok) return error;
    }
    *out = s; return Error::ok;
}
const char* dh2_skin_joint_name(const Skin* s, std::int32_t j) {
    if (!s || j < 0 || std::uint32_t(j) >= s->joints) return nullptr;
    const auto* p = at(s->image, std::uint64_t(s->joint_names) + std::uint32_t(j) * 4, 4);
    return p ? text(s->image, word(p)) : nullptr;
}
Error dh2_skin_inverse_bind(const Skin* s, std::int32_t j, Matrix4f* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!s || j < 0 || std::uint32_t(j) >= s->joints) return Error::joint;
    return matrix(at(s->image, std::uint64_t(s->inverse_matrices) + std::uint32_t(j) * 64, 64), *out);
}
Error dh2_skin_influence(const Skin* s, std::uint32_t i, Influence* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!s || i >= s->vertices || !s->influences || s->influences > 4) return Error::range;
    const auto* p = at(s->image, std::uint64_t(s->weights) + std::uint64_t(i) * s->weight_stride,
                       4 + s->influences * 4);
    if (!p) return Error::range;
    Influence v{}; v.count = s->influences;
    for (std::uint32_t j = 0; j < v.count; ++j) {
        v.joints[j] = p[j]; v.weights[j] = real(p + 4 + j * 4);
        if (!std::isfinite(v.weights[j])) return Error::nonfinite;
        if (v.weights[j] < 0 || v.weights[j] > 1) return Error::weight;
        // Unused zero-weight slots can contain arbitrary index bytes.
        if (v.weights[j] != 0 && v.joints[j] >= s->joints) return Error::joint;
    }
    *out = v; return Error::ok;
}
Error dh2_skin_palette(const Skin* s, const Matrix4f* worlds, std::size_t n,
                       Matrix4f* out, std::size_t capacity) {
    if (!s || !worlds || !out) return Error::argument;
    if (n < s->joints || capacity < s->joints) return Error::capacity;
    if (!finite(s->bind_shape)) return Error::nonfinite;
    for (std::uint32_t j = 0; j < s->joints; ++j) {
        if (!finite(worlds[j])) return Error::nonfinite;
        Matrix4f inverse{}, product{};
        const auto error = dh2_skin_inverse_bind(s, j, &inverse);
        if (error != Error::ok) return error;
        multiply(product, worlds[j], inverse);
        multiply(product, product, s->bind_shape);
        if (!finite(product)) return Error::nonfinite;
        out[j] = product;
    }
    return Error::ok;
}
Error dh2_skin_scene_palette(const Skin* s, const dh2::scene::Visual* visual,
                             Matrix4f* out, std::size_t capacity) {
    if (!s || !visual || !out || s->image.bytes != visual->image.bytes)
        return Error::argument;
    if (!s->joints || s->joints > 256 || capacity < s->joints) return Error::capacity;
    Resolve r{}; r.skin = s;
    const auto walked = dh2_scene_walk_visual(visual, resolve_node, &r, 20000);
    if (r.error != Error::ok) return r.error;
    if (walked != dh2::scene::Error::ok) return Error::range;
    for (std::uint32_t j = 0; j < s->joints; ++j) if (!r.found[j]) return Error::joint;
    return dh2_skin_palette(s, r.worlds, s->joints, out, capacity);
}
Error dh2_skin_position(const Skin* s, std::uint32_t vertex, const Matrix4f* palette,
                        std::size_t n, const dh2::math::Vector3f* input,
                        dh2::math::Vector3f* out) {
    if (!s || !palette || !input || !out) return Error::argument;
    if (n < s->joints) return Error::capacity;
    const auto position = *input;
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))
        return Error::nonfinite;
    Influence v{}; const auto error = dh2_skin_influence(s, vertex, &v);
    if (error != Error::ok) return error;
    float result[3]{};
    for (std::uint32_t j = 0; j < v.count; ++j) {
        if (!v.weights[j]) continue;
        const auto& transform = palette[v.joints[j]];
        if (!finite(transform)) return Error::nonfinite;
        for (int axis = 0; axis < 3; ++axis) {
            const auto* m = transform.m;
            const float p = ((m[axis] * position.x + m[4 + axis] * position.y)
                + m[8 + axis] * position.z) + m[12 + axis];
            result[axis] += p * v.weights[j];
        }
    }
    for (float x : result) if (!std::isfinite(x)) return Error::nonfinite;
    *out = {result[0], result[1], result[2]}; return Error::ok;
}
}
