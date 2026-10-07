#pragma once
#include "game_object_visual_asset_owner_v1.hpp"
namespace dh2::world {
// Whole source visual assignment over Character's inherited SAME GameObject
// fields. No CanonicalGameObjectBaseOwner or copied strings are allocated.
class CharacterVisualAssetOwnerV6 {
 std::uintptr_t identity_;std::uintptr_t& visual_;std::string& model_;std::string& xref_;
 GameObjectVisualAssetServicesV1 services_;
 bool missing(const char*,std::string&)const;
public:
 CharacterVisualAssetOwnerV6(std::uintptr_t id,std::uintptr_t& visual,std::string& model,std::string& xref,GameObjectVisualAssetServicesV1 s):identity_(id),visual_(visual),model_(model),xref_(xref),services_(std::move(s)){}
 bool set_visual(std::uintptr_t,std::string&);
 bool set_visual(const char*,const char*,bool force,std::string&);
 bool load_visual(std::string&);
};
}
