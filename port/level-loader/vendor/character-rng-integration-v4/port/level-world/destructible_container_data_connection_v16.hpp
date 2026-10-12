#pragma once
#include "destructible_container_data_v16.hpp"
#include "canonical_gameobject_base_owner_v1.hpp"
#include "../game-data/game_object_dictionary_v11.hpp"
namespace dh2::world {
// Borrows the SAME immutable World dictionary and Destructible row snapshot.
class DestructibleContainerDataConnectionV16 {
 std::shared_ptr<const DestructibleContainerTableV16> table_;
 std::shared_ptr<const data::GameObjectDictionaryV11> dictionary_;
public:
 DestructibleContainerDataConnectionV16(std::shared_ptr<const DestructibleContainerTableV16> t,std::shared_ptr<const data::GameObjectDictionaryV11> d):table_(std::move(t)),dictionary_(std::move(d)){}
 bool resolve(const std::string& name,std::int32_t& id,const DestructibleContainerRowV16*& row,std::string& e)const{
  if(!table_){e="Required SAME World DestructibleContainers snapshot";return false;}id=table_->data_id(name);row=table_->row(id);return true;
 }
 bool visual_asset(CanonicalGameObjectBaseOwnerV1& base,std::int32_t id,std::string& e)const{
  if(!dictionary_){e="Required SAME World GameObjectDict snapshot";return false;}const std::string* file{};if(!dictionary_->file(id,file,e))return false;
  if(!file){e="Required valid source GameObjectDict visual index";return false;}auto* target=base.string(0x290);if(!target){e="Required SAME base visual CString290";return false;}*target=*file;return true;
 }
};
}
