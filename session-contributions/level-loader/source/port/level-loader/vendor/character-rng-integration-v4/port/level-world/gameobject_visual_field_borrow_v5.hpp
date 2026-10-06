#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
// Actual Character derives GameObject (C1 3aa1b4 calls38c398 first), so the
// generic VisualObject constructor must use the SAME parent fields. This is a
// typed capability over storage, not an ObjectBase/Handle/Runtime allocation.
struct GameObjectVisualFieldBorrowV5 {
 std::shared_ptr<void> receiver_lease;
 std::uintptr_t identity{};
 const std::uint8_t* static84{};
 const float* position160{};
 const float* rotation16c{};
 const float* scale120{};
 // Derived virtual SetRelativeAABB owns flat/bounds/PF. Character must invoke
 // exact Character3a4398, not the generic Decor receiver arithmetic.
 std::function<bool(const float*,std::string&)> apply_mesh_box;
 // Actual VisualObject+28 marker flag forwarded by ApplyMeshBox470a54.
 std::function<bool(const float*,bool,std::string&)> apply_mesh_box_v6;
};
}
