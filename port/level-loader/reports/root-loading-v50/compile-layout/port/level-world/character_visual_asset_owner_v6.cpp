#include "character_visual_asset_owner_v6.hpp"
#include <optional>
namespace dh2::world {
bool CharacterVisualAssetOwnerV6::missing(const char* name,std::string& e)const{e=std::string("Required same Character visual ")+name;return false;}
bool CharacterVisualAssetOwnerV6::set_visual(std::uintptr_t value,std::string& e){
 e.clear();if(visual_==value)return true;
 if(visual_){if(!services_.owner||!services_.destroy)return missing("deleting destructor",e);if(!services_.destroy(visual_,e))return false;visual_=0;}
 visual_=value;return true;
}
bool CharacterVisualAssetOwnerV6::set_visual(const char* model,const char* xref,bool force,std::string& e){
 e.clear();if(!force&&model&&model_==model&&(!xref||xref_==xref))return true;
 const std::optional<std::string> requested_model=model?std::optional<std::string>(model):std::nullopt;
 const std::optional<std::string> requested_xref=xref?std::optional<std::string>(xref):std::nullopt;
 model_=requested_model?*requested_model:std::string{};xref_=requested_model&&requested_xref?*requested_xref:std::string{};
 if(model_.empty())return set_visual(std::uintptr_t{},e);
 if(!identity_||!services_.owner||!services_.construct||!services_.root)return missing("constructor/root",e);
 std::uintptr_t candidate=0;if(!services_.construct(identity_,model_,xref_,candidate,e))return false;
 if(!candidate)return missing("constructor receiver",e);
 std::uintptr_t root=0;if(!services_.root(candidate,root,e))return false;
 if(!root){if(!services_.destroy)return missing("asset-miss destructor",e);return services_.destroy(candidate,e);}
 if(!set_visual(candidate,e))return false;
 if(!services_.set_root_game_object)return missing("root204 producer",e);
 return services_.set_root_game_object(root,identity_,e);
}
bool CharacterVisualAssetOwnerV6::load_visual(std::string& e){return set_visual(model_.c_str(),xref_.c_str(),true,e);}
}
