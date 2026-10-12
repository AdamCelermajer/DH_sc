#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include <functional>
namespace dh2::world {
struct GameObjectVisualAssetServicesV1 {
 std::shared_ptr<void> owner;
 // Actual VisualObject constructor owns its root, bounds and base controller.
 // A completed constructor may produce root==0, the source asset-miss branch.
 std::function<bool(std::uintptr_t,const std::string&,const std::string&,std::uintptr_t&,std::string&)> construct;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> root;
 std::function<bool(std::uintptr_t,std::string&)> destroy;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> set_root_game_object;
};
// Source SetVisualObject(pointer)394338, SetVisualObject(names,force)394d34
// and LoadVisualObject394eb0, sharing the canonical source strings and pointer.
// Failed asset construction keeps the previous visual, as the original does.
class GameObjectVisualAssetOwnerV1 {
 CanonicalGameObjectBaseOwnerV1& base_;GameObjectVisualAssetServicesV1 services_;
 bool missing(const char*,std::string&)const;
public:
 GameObjectVisualAssetOwnerV1(CanonicalGameObjectBaseOwnerV1& b,GameObjectVisualAssetServicesV1 s):base_(b),services_(std::move(s)){}
 bool set_visual(std::uintptr_t,std::string&);
 bool set_visual(const char* model,const char* xref,bool force,std::string&);
 bool load_visual(std::string&);
};
}
