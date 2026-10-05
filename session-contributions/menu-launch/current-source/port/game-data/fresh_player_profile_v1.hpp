#pragma once
#include "data.hpp"
#include <array>
namespace dh2::data {
struct FreshPlayerProfileV1 {
    std::string name,character;
    std::int32_t character_row{-1};
    std::array<std::uint32_t,3> seeds{};
    std::uint32_t saved_date{};
    // Source Savegame count/size/four-byte-tag/payload representation.
    std::vector<std::uint8_t> bytes;
};
// NativeCreateSaveSlot 0x43f718..0x43f7d8 fresh metadata. Caller owns genuine
// free-slot lookup, time() seconds, file delivery and later full profile loading.
// Does not write files or claim an initialized gameplay Character/inventory.
bool fresh_player_profile_v1(const CharacterTable&,const char* character,
    const char* name,std::uint32_t real_time,std::uint32_t saved_date,FreshPlayerProfileV1&,std::string&);
}
