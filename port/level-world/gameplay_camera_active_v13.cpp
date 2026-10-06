#include "gameplay_camera_active_v13.hpp"
namespace dh2::camera {
bool source_camera_base_hook_v13(std::string&)noexcept{return true;}
bool GameplayCameraActiveV13::resolve(std::uintptr_t id,CameraBaseBorrowV13& out,std::string& e)const{
 auto found=receivers_.find(id);if(found==receivers_.end()||found->second.receiver.expired()){e="Required actual retained CameraBase virtual receiver";return false;}out=found->second;return true;
}
bool GameplayCameraActiveV13::register_receiver(CameraBaseBorrowV13 b,std::string& e){
 if(!b.identity||b.receiver.expired()||!b.activated||!b.deactivated){e="Required source-constructed CameraBase receiver/vtable hooks";return false;}
 if(receivers_.count(b.identity)){e="CameraBase identity already registered";return false;}receivers_.emplace(b.identity,std::move(b));return true;
}
bool GameplayCameraActiveV13::set_active(std::uintptr_t id,world::GameObjectSceneRootRegistryV1& manager,std::string& e){
 if(source_active_==id)return true;
 CameraBaseBorrowV13 next;if(!resolve(id,next,e))return false;
 if(source_active_){CameraBaseBorrowV13 old;if(!resolve(source_active_,old,e)||!old.deactivated(e))return false;}
 source_active_=id;
 if(!manager.set_active_camera_v13(next.scene_camera,e))return false;
 // Source rereads s_activeCamera after SceneManager/drop callbacks. Do not
 // blindly invoke next after a reentrant activation changed the process slot.
 CameraBaseBorrowV13 current;if(!resolve(source_active_,current,e))return false;return current.activated(e);
}
void GameplayCameraActiveV13::destroy_receiver(std::uintptr_t id)noexcept{if(source_active_==id)source_active_=0;receivers_.erase(id);}
}
