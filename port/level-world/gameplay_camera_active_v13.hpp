#pragma once
#include "gameobject_scene_root_registry_v1.hpp"
#include <map>
namespace dh2::camera {
struct CameraBaseBorrowV13 {
 std::weak_ptr<void> receiver;
 std::uintptr_t identity{};
 std::function<bool(std::string&)> activated,deactivated;
 world::GameObjectSceneCameraBorrowV13 scene_camera;
};
// One Application lifetime semantic owner of CameraBase::s_activeCamera.
// Raw active identity matches the original non-owning process pointer; the
// modern directory is weak and does not pin a World or create a lease cycle.
class GameplayCameraActiveV13 {
 std::uintptr_t source_active_{};
 std::map<std::uintptr_t,CameraBaseBorrowV13> receivers_;
 bool resolve(std::uintptr_t,CameraBaseBorrowV13&,std::string&)const;
public:
 bool register_receiver(CameraBaseBorrowV13,std::string&);
 bool set_active(std::uintptr_t,world::GameObjectSceneRootRegistryV1&,std::string&);
 // CameraBase D2 only clears matching self, with no Deactivated callback.
 void destroy_receiver(std::uintptr_t)noexcept;
 std::uintptr_t source_active()const noexcept{return source_active_;}
};
// Proven CameraBase Activated40e718/Deactivated40e71c bx-lr leaves.
// Only bind after actual selected vtable proof; Overview overrides both.
bool source_camera_base_hook_v13(std::string&)noexcept;
}
