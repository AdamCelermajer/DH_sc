#pragma once
#include "character_set_position_v7.hpp"
#include "retained_character_actor_v1.hpp"
#include "native_body.hpp"
namespace dh2::character {
struct RetainedCharacterPositionBackendsV7 {
 std::shared_ptr<void> world;
 std::function<bool(std::uintptr_t,float*&,std::string&)> attached_position;
 // Exact physical receiver -> its same NativeBody; no alternate body lookup
 // by nearest actor, handle key, or visual appearance.
 std::function<bool(std::uintptr_t,physical::NativeBody*&,std::string&)> physical_body;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync_position;
};
class RetainedCharacterPositionOwnerV7 {
 RetainedCharacterActorV1& actor_;CharacterPositionFieldsV7& fields_;
 RetainedCharacterPositionBackendsV7 backends_;
 CharacterPositionResultV7 result_{};
public:
 RetainedCharacterPositionOwnerV7(RetainedCharacterActorV1& actor,RetainedCharacterPositionBackendsV7 services):actor_(actor),fields_(actor.position_fields_v7()),backends_(std::move(services)){}
 RetainedCharacterPositionOwnerV7(RetainedCharacterActorV1& actor,CharacterPositionFieldsV7& same_fields,RetainedCharacterPositionBackendsV7 services):actor_(actor),fields_(same_fields),backends_(std::move(services)){}
 bool set_position(const float*,bool destination,std::string&);
 // Wire from actual physical initializer assignment/release, not frame facts.
 bool publish_physical(std::uintptr_t actor,std::uintptr_t same_physical_identity,
  const physical::NativeBody*,bool assigned,std::string&);
 const CharacterPositionResultV7& result()const noexcept{return result_;}
};
}
