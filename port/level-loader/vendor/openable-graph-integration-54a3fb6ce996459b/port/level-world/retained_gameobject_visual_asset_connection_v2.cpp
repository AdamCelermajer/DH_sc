#include "retained_gameobject_visual_asset_connection_v2.hpp"
#include <algorithm>
namespace dh2::world {
std::shared_ptr<RetainedGameObjectVisualV1> RetainedGameObjectVisualAssetConnectionV2::lookup(std::uintptr_t id)const{auto p=visuals_.find(id);return p==visuals_.end()?nullptr:p->second.visual;}
std::shared_ptr<RetainedGameObjectVisualV1> RetainedGameObjectVisualAssetConnectionV2::attached()const{return lookup(*const_cast<CanonicalGameObjectBaseOwnerV1&>(base_).pointer(0x2d8));}
std::shared_ptr<RetainedGameObjectVisualV1> RetainedGameObjectVisualAssetConnectionV2::lookup_root(std::uintptr_t id)const{if(!id)return nullptr;for(auto& p:visuals_)if(p.second.visual->root_identity()==id)return p.second.visual;return nullptr;}
std::size_t RetainedGameObjectVisualAssetConnectionV2::failed_count()const{std::size_t n=0;for(auto& p:visuals_)if(p.second.failed)++n;return n;}
bool RetainedGameObjectVisualAssetConnectionV2::destroy(std::uintptr_t id,std::string& e){auto p=visuals_.find(id);if(p==visuals_.end()){e="Required SAME retained visual deleting-destructor receiver";return false;}if(!p->second.visual->release(e))return false;visuals_.erase(p);creation_order_.erase(std::remove(creation_order_.begin(),creation_order_.end(),id),creation_order_.end());return true;}
GameObjectVisualAssetServicesV1 RetainedGameObjectVisualAssetConnectionV2::services(std::shared_ptr<void> lease){
 GameObjectVisualAssetServicesV1 s;s.owner=std::move(lease);
 s.construct=[this](auto parent,const auto& model,const auto& xref,std::uintptr_t& result,auto& e){
  result=0;if(parent!=base_.identity()){e="Required SAME canonical visual parent";return false;}
  auto visual=std::make_shared<RetainedGameObjectVisualV1>(base_,source_);auto id=reinterpret_cast<std::uintptr_t>(visual.get());
  visuals_.emplace(id,Candidate{visual,true});creation_order_.push_back(id);
  if(!visual->initialize(model.c_str(),xref.c_str(),e))return false;
  visuals_.at(id).failed=false;result=id;return true;
 };
 s.root=[this](auto id,auto& root,auto& e){auto v=lookup(id);if(!v){e="Required owned visual root receiver";return false;}root=v->root_identity();return true;};
 s.destroy=[this](auto id,auto& e){return destroy(id,e);};
 s.set_root_game_object=[this](auto root,auto parent,auto& e){for(auto& p:visuals_)if(p.second.visual->root_identity()==root)return p.second.visual->set_root_game_object(parent,e);e="Required SAME root204 receiver";return false;};
 return s;
}
bool RetainedGameObjectVisualAssetConnectionV2::discard_failed(std::string& e){
 auto ids=creation_order_;for(auto it=ids.rbegin();it!=ids.rend();++it){auto p=visuals_.find(*it);if(p==visuals_.end()||!p->second.failed)continue;
  if(*base_.pointer(0x2d8)==*it){e="Failed visual unexpectedly attached to canonical base";return false;}
  if(!destroy(*it,e))return false;
 }return true;
}
bool RetainedGameObjectVisualAssetConnectionV2::discard_unattached(std::string& e){
 if(*base_.pointer(0x2d8)){e="Detach canonical visual through source SetVisualObject before registry discard";return false;}
 auto ids=creation_order_;for(auto it=ids.rbegin();it!=ids.rend();++it)if(!destroy(*it,e))return false;return true;
}
}
