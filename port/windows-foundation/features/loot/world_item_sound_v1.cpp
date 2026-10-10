#include "world_item_sound_v1.hpp"

namespace dh::foundation::loot {

bool world_item_sound_ordinal_v1(const dh2::data::LootAudioVisualV8::Borrow& audiovisual,
                                 std::int32_t visual_row,
                                 WorldItemSoundEventV1 event,
                                 std::int32_t& source_ordinal,
                                 std::string& error) {
    source_ordinal = -1;
    if (!audiovisual) {
        error = "Required loaded ItemAudioVisualTable";
        return false;
    }
    const auto& rows = audiovisual.rows();
    if (visual_row < 0 || static_cast<std::size_t>(visual_row) >= rows.size()) {
        error = "ItemTable AudioVisualID is outside the ItemAudioVisualTable";
        return false;
    }
    const auto& row = rows[static_cast<std::size_t>(visual_row)];
    source_ordinal = event == WorldItemSoundEventV1::drop ? row.audio_drop : row.audio_pickup;
    if (source_ordinal < 0) {
        // The original Play3D returns early for a negative sound id: silent by source.
        error = "ItemAudioVisualTable row has no sound id";
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::loot
