#pragma once

#include "../scene-materials/scene.hpp"

#include <cstdint>

namespace dh2::visual_node_queries_v1 {

using Identity = std::uintptr_t;

// Borrowed view of the live scene owned by one VisualObject/CharacterPose.
// Opaque handles are borrowed Node addresses. A query resolves the address
// against this exact scene before reading it, so another Record cannot reuse it.
struct View {
    Identity visual{};
    const scene::Scene* scene{};
};

struct Position {
    float x{}, y{}, z{};
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    not_found,
    stale_handle,
    invalid_transform,
};

// VisualObject::GetSpecificNode(name/int) equivalent over the selected
// retained Collada scene. Name resolution preserves graph order and checks
// the node name and Collada ID. Integer selection is the graph node index.
Status get_specific_node(const View*, const char* name, Identity* node_out) noexcept;
Status get_specific_node(const View*, std::int32_t index,
                         Identity* node_out) noexcept;

// Read the current absolute node position after the owning Player sample has
// updated the scene world matrices. The returned value is copied by value.
Status absolute_position(const View*, Identity node, Position* output) noexcept;

} // namespace dh2::visual_node_queries_v1
