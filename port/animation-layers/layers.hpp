#pragma once
#include "../animation-pose/pose.hpp"

// Diagnostic absolute-pose composition using the original mixing arithmetic.
// This does not recreate the animator's target compiler, transition scheduler,
// default-relative application or events. Borrowed clips/bytes must remain alive.
namespace dh2::layers {
struct Layer { const dh2::pose::Clip *clip; std::int32_t time; float weight; };
struct Layers { Layer items[8]; std::uint32_t count; };
}
extern "C" {
// Nonnegative finite weights, at most eight clips. Zero total selects the first
// layer through the original normalization rule. Errors leave outputs unchanged.
dh2::pose::Error dh2_layers_node(const dh2::layers::Layers *,
    const dh2::scene::Node *, dh2::scene::Node *);
dh2::pose::Error dh2_layers_skin_palette(const dh2::layers::Layers *,
    const dh2::skin::Skin *, const dh2::scene::Visual *,
    dh2::math::Matrix4f *, std::size_t capacity);
}
