#pragma once
#include <retained_gameobject_visual_v1.hpp>
#include <functional>
namespace dh2::loader {
// Borrowed SAME GameObject cells. Character supplies its inherited field borrow
// and actual State56.flags producer; no auxiliary pose or Character surrogate.
struct GameObjectSetRotationBorrowV69 {
 std::shared_ptr<void> receiver;
 float* rotation16c{};float* heading178{};std::uintptr_t* visual2d8{};
 std::function<bool(bool&,std::string&)> updating_visual_with_rotation;
 std::function<bool(std::uintptr_t,const float*,std::string&)> sync_rotation;
};
inline bool game_object_set_rotation_v69(GameObjectSetRotationBorrowV69& b,const float* source,std::string& error){
 if(!b.receiver||!b.rotation16c||!b.heading178||!b.visual2d8||!source){error="Required SAME GameObject.SetRotation3938a0 field borrow";return false;}
 // Preserve original source loads/stores, including input aliasing. Visual is
 // captured between the Y load and Y store; Z is read again for heading178.
 b.rotation16c[0]=source[0];const auto y=source[1];const auto visual=*b.visual2d8;
 b.rotation16c[1]=y;b.rotation16c[2]=source[2];*b.heading178=source[2];
 if(!visual){error.clear();return true;}
 bool enabled{};if(!b.updating_visual_with_rotation||!b.updating_visual_with_rotation(enabled,error)){if(error.empty())error="Required actual GameObject virtual70";return false;}
 if(!enabled){error.clear();return true;}
 // Source rereads2d8 after the virtual delivery; callback cannot lend retired
 // native storage. The actual owner remains pinned for the whole delivery.
 const auto current=*b.visual2d8;if(!current||!b.sync_rotation){error="Required live actual VisualObject.SyncRotation472948";return false;}
 return b.sync_rotation(current,b.rotation16c,error);
}
inline bool retained_visual_sync_rotation_v69(world::RetainedGameObjectVisualV1& visual,const float* rotation,std::string& error){
 if(!visual.root_identity()){error.clear();return true;}
 float quaternion[4];if(dh2_visual_rotation(quaternion,rotation)){error="Actual VisualObject rotation kernel failed";return false;}
 const auto* current=visual.binding().root.quaternion;
 // Original __aeabi_fcmpeq checks each component, not a tolerance/memcmp.
 if(current[0]==quaternion[0]&&current[1]==quaternion[1]&&current[2]==quaternion[2]&&current[3]==quaternion[3]){error.clear();return true;}
 if(!visual.binding().set_rotation(rotation)){error="Actual Root rotation assignment failed";return false;}
 // Same retained root/world cache and CalcMeshBox47211c -> ApplyMeshBox470a54.
 return visual.binding().update_world(visual.scene(),error)&&visual.calc_mesh_box(error)&&visual.apply_mesh_box(error);
}
}
