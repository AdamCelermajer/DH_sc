#pragma once
#include "savegame_stream_v2.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include "../game-data/player_savegame_v1.hpp"
namespace dh2::level {
// Whole original __SaveInventory46a0b4 over the SAME two-set/item vector.
// Caller pins actual Tables/Powers/inventory across synchronous serialization.
bool player_save_inventory_writer_v45(const data::FreshInventoryOwnedV4&,
 const std::vector<std::string>& actual_power_names,SavegameStreamV2&,std::string&);
// Whole __SaveProperties4689d8: 224 named-property order words and source
// PlayerSavegame+194 byte. This campaign section has NO Structs vtable tag.
bool player_save_properties_writer_v45(const data::PropertyView&,
 const std::uint8_t* actual_save194,SavegameStreamV2&,std::string&);
bool player_save_properties_reader_v45(data::PlayerSavegameV1&,data::PropertyView&,
 data::Bytes,std::size_t& consumed,std::string&);
}
