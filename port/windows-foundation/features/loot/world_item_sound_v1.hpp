#pragma once

#include "runtime_world_item_adapter_v1.hpp"
#include "../../../game-data/loot_audiovisual_v8.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::loot {

// B048 (Preview 15): maps a world item to its original generated-Sounds ordinal.
// ItemTable AudioVisualID selects a ItemAudioVisualTable row (the item's
// visual_row). Drop plays row+4 (AudioDrop) from ItemObject::InitAgain
// (0x3ec0f0); pickup plays row+8 (AudioPickup) from ItemObject::Interact
// (0x3ed144) at the item position. The value is a source ordinal into the
// generated Sounds table (for example DropGold = 62 -> XML uid 151), not an
// XML uid. The mapping only selects the cue; the audio runtime resolves it.
bool world_item_sound_ordinal_v1(const dh2::data::LootAudioVisualV8::Borrow& audiovisual,
                                 std::int32_t visual_row,
                                 WorldItemSoundEventV1 event,
                                 std::int32_t& source_ordinal,
                                 std::string& error);

} // namespace dh::foundation::loot
