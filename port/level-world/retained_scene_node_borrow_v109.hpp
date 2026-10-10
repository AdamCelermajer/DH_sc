#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "batch_animation_borrow_v112.hpp"
namespace dh2::scene {struct Material;struct InstanceMaterialBindingV1;}
namespace dh2::world {
struct RetainedMeshNodeV91;
// A synchronous loan over actual retained node/mesh membership and fields.
// Host pins keep borrowed cells valid; they do not add engine reference grabs.
struct RetainedSceneNodeBorrowV109 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 const std::string* name24{};
 const std::uint32_t* flags11c{};
 std::shared_ptr<RetainedMeshNodeV91> mesh_v111;
 std::function<bool(std::vector<scene::InstanceMaterialBindingV1>&,std::string&)> materials_v111;
 bool dynamic_source_v111{};
 std::function<bool(BatchAnimationBorrowV112&,std::string&)> animation_v112;
 std::uintptr_t animation_identity_v112{};
 std::function<bool(std::size_t&,std::string&)> child_count;
 std::function<bool(std::size_t,RetainedSceneNodeBorrowV109&,std::string&)> child;
 std::function<bool(bool,std::string&)> set_visible48;
};
}
