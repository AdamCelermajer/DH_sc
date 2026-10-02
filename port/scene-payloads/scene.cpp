#include "scene.hpp"
#include <cmath>
#include <cstring>

using namespace dh2::scene;
namespace {
std::uint32_t word(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8)
        | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
float real(const std::uint8_t* p) {
    const auto bits = word(p);
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
bool span(const BresView& image, std::uint64_t offset, std::uint64_t size) {
    return image.bytes && offset <= image.size && size <= image.size - offset;
}
const std::uint8_t* at(const BresView& image, std::uint64_t offset, std::uint64_t size) {
    return span(image, offset, size) ? image.bytes + offset : nullptr;
}
const char* string(const BresView& image, std::uint32_t offset) {
    if (!offset || !span(image, offset, 1)) return nullptr;
    for (std::size_t i = offset; i < image.size; ++i)
        if (!image.bytes[i]) return reinterpret_cast<const char*>(image.bytes + offset);
    return nullptr;
}
Error node_at(const BresView& image, std::uint64_t offset, Node* out) {
    const auto* p = at(image, offset, 80);
    if (!p) return Error::range;
    Node candidate{};
    candidate.image = image;
    candidate.id = string(image, word(p));
    candidate.name = string(image, word(p + 4));
    if (!candidate.id || !candidate.name) return Error::string;
    candidate.record = static_cast<std::uint32_t>(offset);
    candidate.visible = word(p + 0x34);
    candidate.children = word(p + 0x38);
    candidate.child_offset = word(p + 0x3c);
    candidate.instances = word(p + 0x40);
    candidate.instance_offset = word(p + 0x44);
    candidate.extension_offset = word(p + 0x4c);
    if ((candidate.children && !at(image, candidate.child_offset,
                                  std::uint64_t(candidate.children) * 80))
        || (candidate.instances && !at(image, candidate.instance_offset,
                                       std::uint64_t(candidate.instances) * 8))) return Error::range;
    for (std::uint32_t i = 0; i < 3; ++i) {
        candidate.position[i] = real(p + 0x0c + i * 4);
        candidate.scale[i] = real(p + 0x28 + i * 4);
        if (!std::isfinite(candidate.position[i]) || !std::isfinite(candidate.scale[i]))
            return Error::nonfinite;
    }
    for (std::uint32_t i = 0; i < 4; ++i) {
        candidate.rotation[i] = real(p + 0x18 + i * 4);
        if (!std::isfinite(candidate.rotation[i])) return Error::nonfinite;
    }
    *out = candidate;
    return Error::ok;
}
const char* local_fragment(const char* url) {
    return url && url[0] == '#' && url[1] ? url + 1 : nullptr;
}
bool finite_affine(const dh2::math::Matrix4f& matrix) {
    for (float value : matrix.m) if (!std::isfinite(value)) return false;
    return matrix.m[3] == 0.0f && matrix.m[7] == 0.0f
        && matrix.m[11] == 0.0f && matrix.m[15] == 1.0f;
}
Error walk_node(const Node& node, const dh2::math::Matrix4f* parent,
                WalkCallback callback, void* user, std::uint32_t max_nodes,
                std::uint32_t& visited, std::uint32_t depth,
                std::uint32_t (&ancestors)[64]) {
    if (depth >= 64 || visited >= max_nodes) return Error::walk_limit;
    for (std::uint32_t i = 0; i < depth; ++i)
        if (ancestors[i] == node.record) return Error::walk_cycle;
    ancestors[depth] = node.record;
    dh2::math::Matrix4f world{};
    const auto result = dh2_scene_world_matrix(&node, parent, &world);
    if (result != Error::ok) return result;
    ++visited;
    if (!callback(&node, &world, depth, user)) return Error::walk_stopped;
    for (std::uint32_t i = 0; i < node.children; ++i) {
        Node child{};
        const auto opened = dh2_scene_child_node(&node, static_cast<std::int32_t>(i), &child);
        if (opened != Error::ok) return opened;
        const auto walked = walk_node(child, &world, callback, user,
                                      max_nodes, visited, depth + 1, ancestors);
        if (walked != Error::ok) return walked;
    }
    return Error::ok;
}
}

extern "C" {
Error dh2_scene_open(Scene* out, const BresView* image) {
    if (!out) return Error::argument;
    *out = {};
    if (!image || !at(*image, image->root_offset, 192)) return Error::argument;
    const auto* root = image->bytes + image->root_offset;
    Scene candidate{*image, word(root + 0xb8), word(root + 0xbc),
                    word(root + 0x98), word(root + 0x9c)};
    if ((candidate.references && !at(*image, candidate.reference_offset,
                                      std::uint64_t(candidate.references) * 8))
        || (candidate.visuals && !at(*image, candidate.visual_offset,
                                     std::uint64_t(candidate.visuals) * 16))) return Error::range;
    *out = candidate;
    return Error::ok;
}
Error dh2_scene_reference(const Scene* scene, std::int32_t index, Reference* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!scene || index < 0 || std::uint32_t(index) >= scene->references) return Error::range;
    const auto* entry = at(scene->image,
                           std::uint64_t(scene->reference_offset) + std::uint32_t(index) * 8, 8);
    if (!entry) return Error::range;
    // constructScene only follows type 6 (visual-scene) entries.
    if (word(entry) != 6) return Error::unsupported_reference;
    const auto* payload = at(scene->image, word(entry + 4), 8);
    if (!payload) return Error::range;
    const auto* url = string(scene->image, word(payload + 4));
    if (!url) return Error::string;
    *out = {6, url};
    return Error::ok;
}
Error dh2_scene_visual(const Scene* scene, std::int32_t index, Visual* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!scene || index < 0 || std::uint32_t(index) >= scene->visuals) return Error::range;
    const auto* p = at(scene->image,
                       std::uint64_t(scene->visual_offset) + std::uint32_t(index) * 16, 16);
    if (!p) return Error::range;
    Visual candidate{scene->image, string(scene->image, word(p)),
                     string(scene->image, word(p + 4)), word(p + 8), word(p + 12)};
    if (!candidate.id || !candidate.name) return Error::string;
    if (candidate.roots && !at(scene->image, candidate.root_offset,
                               std::uint64_t(candidate.roots) * 80)) return Error::range;
    *out = candidate;
    return Error::ok;
}
Error dh2_scene_root_node(const Visual* visual, std::int32_t index, Node* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!visual || index < 0 || std::uint32_t(index) >= visual->roots) return Error::range;
    return node_at(visual->image,
                   std::uint64_t(visual->root_offset) + std::uint32_t(index) * 80, out);
}
Error dh2_scene_child_node(const Node* parent, std::int32_t index, Node* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!parent || index < 0 || std::uint32_t(index) >= parent->children) return Error::range;
    return node_at(parent->image,
                   std::uint64_t(parent->child_offset) + std::uint32_t(index) * 80, out);
}
Error dh2_scene_instance(const Node* node, std::int32_t index, Instance* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!node || index < 0 || std::uint32_t(index) >= node->instances) return Error::range;
    const auto* entry = at(node->image,
                           std::uint64_t(node->instance_offset) + std::uint32_t(index) * 8, 8);
    if (!entry) return Error::range;
    Instance candidate{word(entry), word(entry + 4), nullptr};
    if (!at(node->image, candidate.payload_offset, 8)) return Error::range;
    if (candidate.type == 3) {
        candidate.geometry_url = string(node->image,
                                        word(node->image.bytes + candidate.payload_offset + 4));
        if (!candidate.geometry_url) return Error::string;
    }
    *out = candidate;
    return Error::ok;
}
std::int32_t dh2_scene_visual_index(const Scene* scene, const char* url) {
    const auto* id = local_fragment(url);
    if (!scene || !id) return -1;
    for (std::uint32_t i = 0; i < scene->visuals; ++i) {
        Visual visual{};
        if (dh2_scene_visual(scene, static_cast<std::int32_t>(i), &visual) != Error::ok) return -1;
        if (std::strcmp(visual.id, id) == 0) return static_cast<std::int32_t>(i);
    }
    return -1;
}
std::int32_t dh2_scene_geometry_index(const Scene* scene, const Instance* instance) {
    const auto* id = instance && instance->type == 3
        ? local_fragment(instance->geometry_url) : nullptr;
    if (!scene || !id) return -1;
    const auto count = dh2_bres_library_count(&scene->image, dh2::resources::Library::geometry);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto* record = dh2_bres_library_item(&scene->image,
                                                   dh2::resources::Library::geometry,
                                                   static_cast<std::int32_t>(i));
        const auto* candidate = record ? string(scene->image, word(record)) : nullptr;
        if (candidate && std::strcmp(candidate, id) == 0) return static_cast<std::int32_t>(i);
    }
    return -1;
}
Error dh2_scene_local_matrix(const Node* node, dh2::math::Matrix4f* out) {
    if (!node || !out) return Error::argument;
    for (float value : node->position) if (!std::isfinite(value)) return Error::nonfinite;
    for (float value : node->rotation) if (!std::isfinite(value)) return Error::nonfinite;
    for (float value : node->scale) if (!std::isfinite(value)) return Error::nonfinite;
    const dh2::math::Quaternion rotation{node->rotation[0], node->rotation[1],
                                         node->rotation[2], node->rotation[3]};
    dh2::math::Matrix4f local{};
    dh2_quat_matrix_transposed(&rotation, &local);
    // ISceneNode::getRelativeTransformation invokes CMatrix4::postScale:
    // columns 0, 1 and 2 are scaled by X, Y and Z respectively.
    for (std::uint32_t column = 0; column < 3; ++column)
        for (std::uint32_t row = 0; row < 3; ++row)
            local.m[column * 4 + row] *= node->scale[column];
    for (std::uint32_t row = 0; row < 3; ++row) local.m[12 + row] = node->position[row];
    local.m[3] = local.m[7] = local.m[11] = 0.0f;
    local.m[15] = 1.0f;
    local.identity_hint = 0;
    if (!finite_affine(local)) return Error::nonfinite;
    *out = local;
    return Error::ok;
}
Error dh2_scene_world_matrix(const Node* node, const dh2::math::Matrix4f* parent,
                             dh2::math::Matrix4f* out) {
    if (!out) return Error::argument;
    dh2::math::Matrix4f local{};
    const auto result = dh2_scene_local_matrix(node, &local);
    if (result != Error::ok) return result;
    if (!parent) { *out = local; return Error::ok; }
    if (!finite_affine(*parent)) return Error::nonaffine;
    dh2::math::Matrix4f world{};
    // CMatrix4Base::mult34(parent, local): three affine columns and a
    // translation column, all in the original column-major storage order.
    for (std::uint32_t column = 0; column < 3; ++column) {
        for (std::uint32_t row = 0; row < 3; ++row) {
            const auto a = parent->m[row] * local.m[column * 4];
            const auto b = parent->m[4 + row] * local.m[column * 4 + 1];
            const auto c = parent->m[8 + row] * local.m[column * 4 + 2];
            world.m[column * 4 + row] = (a + b) + c;
        }
        world.m[column * 4 + 3] = 0.0f;
    }
    for (std::uint32_t row = 0; row < 3; ++row) {
        const auto a = parent->m[row] * local.m[12];
        const auto b = parent->m[4 + row] * local.m[13];
        const auto c = parent->m[8 + row] * local.m[14];
        world.m[12 + row] = ((a + b) + c) + parent->m[12 + row];
    }
    world.m[15] = 1.0f;
    world.identity_hint = 0;
    if (!finite_affine(world)) return Error::nonfinite;
    *out = world;
    return Error::ok;
}
Error dh2_scene_walk_visual(const Visual* visual, WalkCallback callback,
                            void* user, std::uint32_t max_nodes) {
    if (!visual || !callback || !max_nodes) return Error::argument;
    std::uint32_t visited = 0, ancestors[64]{};
    for (std::uint32_t i = 0; i < visual->roots; ++i) {
        Node root{};
        const auto opened = dh2_scene_root_node(visual, static_cast<std::int32_t>(i), &root);
        if (opened != Error::ok) return opened;
        const auto walked = walk_node(root, nullptr, callback, user,
                                      max_nodes, visited, 0, ancestors);
        if (walked != Error::ok) return walked;
    }
    return Error::ok;
}
}
