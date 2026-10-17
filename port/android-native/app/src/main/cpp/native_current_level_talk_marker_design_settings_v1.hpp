#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace dh2::native::current_level_talk_marker_design_settings_v1 {

// Borrowed serialized DesignSettingsTable bytes from the existing
// OriginalUiSession owner. The source table stores a 4-byte row count followed
// by 172-byte rows; the runtime row's vptr is omitted from each serialized
// row, so source member offset N maps to serialized offset N-4.
struct View {
    const std::uint8_t* bytes{};
    std::size_t size{};
};

inline std::uint32_t read_u32(const std::uint8_t* bytes) noexcept {
    return std::uint32_t(bytes[0]) |
           (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) |
           (std::uint32_t(bytes[3]) << 24);
}

// IDA source evidence: Objective_TalkToNPC::InstallObjectiveMarker at 0x47d8e8
// selects one of four integer
// members from DesignSettingsTable.members (source offsets +100/+104/+108/
// +112) using priority==1 and quest_state>7. Return the exact source value
// passed to VisualFXManager::GrabAnimFX; validity against the retained
// AnimatedEffectTable belongs to that canonical manager.
inline bool read_effect_id(const View& view, std::int32_t priority,
                           std::int32_t quest_state, std::int32_t& output,
                           std::string& error) {
    output = 0;
    if (!view.bytes || view.size < 4) {
        error = "TalkToNPC marker DesignSettings table is unavailable";
        return false;
    }
    const auto rows = read_u32(view.bytes);
    constexpr std::size_t row_size = 172;
    if (rows == 0 || rows > 4096 ||
        std::size_t(rows) > (view.size - 4) / row_size) {
        error = "TalkToNPC marker DesignSettings table has an invalid row count";
        return false;
    }

    std::uint32_t source_member_offset = 0;
    if (quest_state > 7)
        source_member_offset = priority == 1 ? 112u : 104u;
    else
        source_member_offset = priority == 1 ? 108u : 100u;
    const auto serialized_offset = std::size_t(source_member_offset - 4u);
    if (serialized_offset > row_size - sizeof(std::uint32_t)) {
        error = "TalkToNPC marker DesignSettings member exceeds its source row";
        return false;
    }
    const auto bits = read_u32(view.bytes + 4 + serialized_offset);
    std::memcpy(&output, &bits, sizeof(output));
    error.clear();
    return true;
}

} // namespace dh2::native::current_level_talk_marker_design_settings_v1
