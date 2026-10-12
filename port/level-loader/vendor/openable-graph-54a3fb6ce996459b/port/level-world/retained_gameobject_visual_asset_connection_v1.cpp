#include "retained_gameobject_visual_asset_connection_v1.hpp"
namespace dh2::world {
bool RetainedGameObjectVisualAssetConnectionV1::missing(const char* text,std::string& e)const{e=std::string("Required typed VisualObject asset connection ")+text;return false;}
std::shared_ptr<RetainedGameObjectVisualV1> RetainedGameObjectVisualAssetConnectionV1::lookup(std::uintptr_t id)const{auto p=visuals_.find(id);return p==visuals_.end()?nullptr:p->second;}
std::shared_ptr<RetainedGameObjectVisualV1> RetainedGameObjectVisualAssetConnectionV1::attached()const{
 auto* p=const_cast<CanonicalGameObjectBaseOwnerV1&>(base_).pointer(0x2d8);return p?lookup(*p):nullptr;
}
GameObjectVisualAssetServicesV1 RetainedGameObjectVisualAssetConnectionV1::services(std::shared_ptr<void> lease){
 GameObjectVisualAssetServicesV1 s;s.owner=std::move(lease);
 s.construct=[this](auto parent,const auto& model,const auto& xref,std::uintptr_t& result,auto& e){
  if(parent!=base_.identity())return missing("SAME canonical parent",e);
  auto visual=std::make_shared<RetainedGameObjectVisualV1>(base_,source_);
  // Preserve reached failed candidates for teardown rather than dropping their
  // registered scene root and exposing a dangling source registry pointer.
  auto identity=reinterpret_cast<std::uintptr_t>(visual.get());visuals_.emplace(identity,visual);
  if(!visual->initialize(model.c_str(),xref.c_str(),e))return false;
  result=identity;return true;
 };
 s.root=[this](auto id,std::uintptr_t& result,auto& e){auto visual=lookup(id);if(!visual)return missing("owned visual receiver",e);result=visual->root_identity();return true;};
 s.destroy=[this](auto id,auto& e){auto visual=lookup(id);if(!visual)return missing("deleting-destructor receiver",e);if(!visual->release(e))return false;visuals_.erase(id);return true;};
 s.set_root_game_object=[this](auto root,auto parent,auto& e){for(auto& entry:visuals_)if(entry.second->root_identity()==root)return entry.second->set_root_game_object(parent,e);return missing("SAME root+204 receiver",e);};
 return s;
}
}
