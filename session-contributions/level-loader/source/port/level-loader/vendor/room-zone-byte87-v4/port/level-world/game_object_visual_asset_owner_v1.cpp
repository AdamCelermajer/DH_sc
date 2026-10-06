#include "game_object_visual_asset_owner_v1.hpp"
#include <optional>
namespace dh2::world {
bool GameObjectVisualAssetOwnerV1::missing(const char* t,std::string& e)const{e=std::string("Required original VisualObject ")+t;return false;}
bool GameObjectVisualAssetOwnerV1::set_visual(std::uintptr_t visual,std::string& e){
 e.clear();auto* current=base_.pointer(0x2d8);if(!current)return missing("canonical+2d8 field",e);
 if(*current==visual)return true;
 if(*current){if(!services_.owner||!services_.destroy)return missing("deleting destructor provider",e);
  if(!services_.destroy(*current,e))return false;*current=0;}
 *current=visual;return true;
}
bool GameObjectVisualAssetOwnerV1::set_visual(const char* model,const char* xref,bool force,std::string& e){
 e.clear();auto* saved_model=base_.string(0x290);auto* saved_xref=base_.string(0x2a8);
 if(!saved_model||!saved_xref)return missing("canonical model/xref fields",e);
 if(!force&&model&&*saved_model==model&&(!xref||*saved_xref==xref))return true;
 // Snapshot arguments before replacing SAME CString storage, including when
 // LoadVisualObject passes that storage's own data pointers back to this call.
 const std::optional<std::string> requested_model=model?std::optional<std::string>(model):std::nullopt;
 const std::optional<std::string> requested_xref=xref?std::optional<std::string>(xref):std::nullopt;
 *saved_model=requested_model?*requested_model:std::string{};
 *saved_xref=requested_model&&requested_xref?*requested_xref:std::string{};
 if(saved_model->empty())return set_visual(std::uintptr_t{0},e);
 if(!services_.owner||!services_.construct||!services_.root)return missing("retained constructor/root provider",e);
 std::uintptr_t candidate{};
 if(!services_.construct(base_.identity(),*saved_model,*saved_xref,candidate,e))return false;
 if(!candidate)return missing("constructor receiver",e);
 std::uintptr_t root{};if(!services_.root(candidate,root,e))return false;
 if(!root){if(!services_.destroy)return missing("failed-load deleting destructor provider",e);return services_.destroy(candidate,e);}
 if(!set_visual(candidate,e))return false;
 // Original writes RootSceneNode+204 only AFTER replacing the owning visual.
 if(!services_.set_root_game_object)return missing("root+204 GameObject producer",e);
 return services_.set_root_game_object(root,base_.identity(),e);
}
bool GameObjectVisualAssetOwnerV1::load_visual(std::string& e){
 auto* model=base_.string(0x290);auto* xref=base_.string(0x2a8);
 if(!model||!xref)return missing("LoadVisualObject source strings",e);
 return set_visual(model->c_str(),xref->c_str(),true,e);
}
}
