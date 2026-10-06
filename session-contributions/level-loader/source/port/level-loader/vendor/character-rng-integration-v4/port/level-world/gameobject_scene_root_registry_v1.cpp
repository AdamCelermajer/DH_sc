#include "gameobject_scene_root_registry_v1.hpp"
#include <algorithm>
namespace dh2::world {
bool GameObjectSceneRootRegistryV1::add_child(GameObjectSceneRootBorrowV1 b,std::string& e){
 // addChild ignores NULL/self. For non-null actual nodes all field producers
 // are mandatory; foreign parent migration is outside this scoped owner.
 if(!b.identity||b.identity==identity_)return true;
 if(!identity_||!b.owner||!b.flags11c||!b.parentec||!b.notify_visibility||!b.remove_animators){e="Required SAME scene root fields/visibility/animator owner";return false;}
 if(*b.parentec&&*b.parentec!=identity_){e="Required actual previous scene parent RemoveChild owner";return false;}
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& v){return v.root.identity==b.identity;});
 if(p!=children_.end())children_.erase(p); // source grab/remove/reappend order
 children_.push_back({std::move(b),false});auto& root=children_.back().root;
 *root.parentec=identity_;*root.flags11c|=0x40u; // setParent source field/stale transform
 fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;
 return root.notify_visibility((root_flags_&1u)!=0,e);
}
bool GameObjectSceneRootRegistryV1::release_visual_root(std::uintptr_t id,std::string& e){
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& v){return v.root.identity==id;});
 if(p==children_.end()){e="Required registered SAME visual root release receiver";return false;}
 // VisualD1 virtual74 then virtual68/drop. Keep progress if a required
 // animator service fails; do not unlink a still-live registered receiver.
 if(!p->animators_removed){if(!p->root.remove_animators(e))return false;p->animators_removed=true;fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;}
 if(*p->root.parentec!=identity_){e="Canonical scene root parent changed without source RemoveChild";return false;}
 *p->root.parentec=0;children_.erase(p);return true;
}
std::vector<std::uintptr_t> GameObjectSceneRootRegistryV1::roots()const{std::vector<std::uintptr_t> out;for(auto& p:children_)out.push_back(p.root.identity);return out;}
}
