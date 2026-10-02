#pragma once
#include "../asset-payloads/payloads.hpp"
#include "../engine-math/math.hpp"
#include "../scene-payloads/scene.hpp"

// Checked immutable views; the BRES bytes must outlive every view.
namespace dh2::skin {
using dh2::resources::BresView;
using dh2::math::Matrix4f;
enum class Error : std::uint32_t {
    ok, argument, range, controller_type, string, layout, geometry,
    nonfinite, joint, weight, capacity
};
struct Skin {
    BresView image;
    const char* id;
    const char* geometry_url;
    std::uint32_t record, joints, joint_names, inverse_matrices;
    std::uint32_t vertices, influences, weights, weight_stride;
    std::int32_t geometry_index;
    Matrix4f bind_shape;
};
struct Influence {
    std::uint32_t count;
    std::uint8_t joints[4];
    float weights[4];
};
}
extern "C" {
dh2::skin::Error dh2_skin_open(dh2::skin::Skin*, const dh2::resources::BresView*,
                              std::int32_t controller);
const char* dh2_skin_joint_name(const dh2::skin::Skin*, std::int32_t joint);
dh2::skin::Error dh2_skin_inverse_bind(const dh2::skin::Skin*, std::int32_t joint,
                                      dh2::math::Matrix4f*);
dh2::skin::Error dh2_skin_influence(const dh2::skin::Skin*, std::uint32_t vertex,
                                   dh2::skin::Influence*);
// Software technique: joint_world * inverse_bind * bind_shape, column major.
// Caller resolves joint names in the scene scope and supplies all joint worlds.
// Inputs and output may alias. Does not normalize the original stored weights.
dh2::skin::Error dh2_skin_palette(const dh2::skin::Skin*,
                                 const dh2::math::Matrix4f* joint_world,
                                 std::size_t world_count,
                                 dh2::math::Matrix4f* output, std::size_t capacity);
// Resolve the serialized SNode scope ID (+8) inside one visual scene. Reject
// missing/duplicate bones; no identity substitution for unresolved joints.
dh2::skin::Error dh2_skin_scene_palette(const dh2::skin::Skin*,
                                       const dh2::scene::Visual*,
                                       dh2::math::Matrix4f*, std::size_t capacity);
dh2::skin::Error dh2_skin_position(const dh2::skin::Skin*, std::uint32_t vertex,
                                  const dh2::math::Matrix4f* palette,
                                  std::size_t palette_count,
                                  const dh2::math::Vector3f* input,
                                  dh2::math::Vector3f* output);
}
