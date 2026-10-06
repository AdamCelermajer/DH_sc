#pragma once
#include "character_combat_sound_v1.hpp"
#include <string>
#include <vector>
namespace dh2::character {
// Owns original sounds_pyarray's first table, the CharSounds rows. Other sound
// banks/listeners are not synthesized by this view.
class CharacterCombatSoundTablesV2 {
 struct Row {std::vector<std::int32_t> lists[4];std::uint8_t flesh{},metal{};CombatSoundRowV1 view;};
 std::vector<Row> rows_;
public:
 bool load(const std::vector<std::uint8_t>& original_array,std::string&);
 const CombatSoundRowV1* get(std::int32_t cached_sound_id)const noexcept;
 std::size_t size()const noexcept{return rows_.size();}
};
}
