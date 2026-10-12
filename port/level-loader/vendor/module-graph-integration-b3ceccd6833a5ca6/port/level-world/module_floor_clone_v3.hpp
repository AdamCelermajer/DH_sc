#pragma once
#include "scene_manager_map_owner_v2.hpp"
#include "floors.hpp"
namespace dh2::world {
// The real CopyMeshSceneNode successor: independent node TRS/cache/name,
// immutable geometry borrowed from the SAME loaded BRES and PF Record.
// It is never an original visible scene mesh or a second floor record.
class ModuleFloorCloneV3 : public std::enable_shared_from_this<ModuleFloorCloneV3> {
 std::shared_ptr<void> resource_,world_;
 floors::Record* record_{};
 std::weak_ptr<SceneManagerMapOwnerV2> map_;
 std::uintptr_t parent_{};std::uint32_t flags_{0x721};
 std::array<float,3> position_{},scale_{};
 std::array<float,4> rotation_{};
 std::array<float,16> cached_{};
 bool local_visible_{true},parent_visible_{true};
 ModuleFloorCloneV3(std::shared_ptr<void>,std::shared_ptr<void>,floors::Record&,
  const float*,const float*,std::shared_ptr<SceneManagerMapOwnerV2>);
public:
 static bool create(std::shared_ptr<void> resource,std::shared_ptr<void> world,
  floors::Record&,const float* rotation4,const float* scale3,
  std::shared_ptr<SceneManagerMapOwnerV2>,std::shared_ptr<ModuleFloorCloneV3>&,std::string&);
 SceneMapNodeBorrowV2 map_node();
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const floors::Record& floor()const noexcept{return *record_;}
 std::uintptr_t parent()const noexcept{return parent_;}
 std::uint32_t flags()const noexcept{return flags_;}
 const std::array<float,16>& cached_matrix()const noexcept{return cached_;}
};
}
