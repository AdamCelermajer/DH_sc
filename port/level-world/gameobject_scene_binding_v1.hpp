#pragma once
#include "visual_motion.hpp"
namespace dh2::visual {
// Source RootSceneNode with displacement1ec=0. Character SceneBinding keeps
// its strict GetAnimRoot contract; this independent graph binding never calls
// that source branch and never creates an artificial character animation root.
class GameObjectSceneBindingV1 {
 SceneBinding storage_; // sole retained Root field storage, no second pose
 std::vector<std::string> identities_;
public:
 Root& root;
 // Character's replacement animator uses the SAME Root field storage.
 SceneBinding& character_binding_v6()noexcept{return storage_;}
 GameObjectSceneBindingV1():root(storage_.root){root.presence=0;root.flags=0x60fu;}
 GameObjectSceneBindingV1(const GameObjectSceneBindingV1&)=delete;
 bool bind(const scene::Scene&,std::string&);
 bool set_rotation(const float* euler){return storage_.set_rotation(euler);}
 bool update_world(scene::Scene&,std::string&)const;
 bool sample(scene::Scene&,const animation::Player&,std::int32_t,std::string&);
};
}
