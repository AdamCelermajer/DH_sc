#pragma once
#include "character_props_id_owner_v1.hpp"
#include "../game-data/player_save_load_owner_v1.hpp"
#include "../game-data/player_save_write_owner_v1.hpp"
namespace dh2::player {
// Source Save C1/SetCharacter already executed by the existing native player.
// This owner adopts that SAME Save; it never allocates a replacement Save or
// replays InitializePlayerSavegame. One actual profile field is shared by
// InitPost, character-menu reload and subsequent SG_Save.
class PlayerSaveIdentityAuthorityV29 {
 std::shared_ptr<data::PlayerSavegameV1> save_;
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> load_;
 std::unique_ptr<data::PlayerSaveWriteOwnerV1> write_;
 std::uintptr_t character_{};
 // Additive native field transport for the same Character's source store at
 // 3b36d8, after the caller's already completed Save C1. t.save/shared Save
 // references remain ownership leases; this is the unique queryable slot.
 std::uintptr_t source_save14e8_{};
 std::int16_t* property_id13c8_{};
 const data::CharacterTable* characters_{};
 data::LootRandom8V2* random_{};
 character::CharacterPropsIdServicesV1 source_;
 static bool source_load(void*,data::PlayerSavegameV1&,std::int32_t,std::string&);
public:
 PlayerSaveIdentityAuthorityV29(std::shared_ptr<data::PlayerSavegameV1>,
  std::uintptr_t actual_character,std::int16_t& same_property_id13c8,
  const data::CharacterTable&,data::LootRandom8V2&,
  character::CharacterPropsIdServicesV1,
  data::PlayerSaveLoadServicesV1,data::PlayerSaveWriteServicesV1);
 PlayerSaveIdentityAuthorityV29(const PlayerSaveIdentityAuthorityV29&)=delete;
 bool safe_properties_id(std::int32_t&,std::string&);
 bool set_slot(std::int32_t actual_PlayerInfo_slot664,std::string&);
 bool load(std::int32_t source_mask,std::string&);
 bool save(std::string&);
 data::PlayerSavegameV1& receiver()const noexcept{return *save_;}
 const std::shared_ptr<data::PlayerSaveLoadOwnerV1>& load_owner()const noexcept{return load_;}
 std::int16_t* property_id_field()const noexcept{return property_id13c8_;}
 const std::uintptr_t* source_save_slot14e8()const noexcept{return &source_save14e8_;}
};
}
