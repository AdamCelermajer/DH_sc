#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "openable_container_owner_v1.hpp"
#include "../game-data/game_object_dictionary_v11.hpp"
namespace dh2::world {
// ONE immutable World cache pair, shared by every canonical chest receiver.
class OpenableContainerDataConnectionV11 {
 OpenableContainerTableV1 containers_;
 data::GameObjectDictionaryV11 dictionary_;
 bool ready_{};
public:
 bool load(const std::uint8_t* container_records,std::size_t,const std::uint8_t* container_names,std::size_t,
  const std::uint8_t* dictionary_records,std::size_t,const std::uint8_t* dictionary_names,std::size_t,std::string&);
 bool resolve_row(const std::string&,std::int32_t&,OpenableContainerRowV1&,std::string&)const;
 bool visual_asset(CanonicalGameObjectBaseOwnerV1& same_base,std::int32_t source_visual_id,std::string&)const;
 std::size_t container_count()const noexcept{return containers_.size();}
 std::size_t dictionary_count()const noexcept{return dictionary_.size();}
};
}
