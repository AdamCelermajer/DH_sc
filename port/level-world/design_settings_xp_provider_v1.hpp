#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::design_settings_xp_provider_v1 {

// Borrowed serialized DesignSettingsTable bytes. Keep the vector or asset
// buffer alive for the full XP dispatch; this view does not copy table state.
struct View {
    const std::uint8_t* bytes = nullptr;
    std::size_t size = 0;
};

enum class Status : std::uint32_t {
    complete = 0,
    invalid_argument,
    invalid_table,
    unsupported_offset,
};

// Implements character_distribute_xp_v1::Services::design_setting directly.
// The serialized row omits its runtime vptr, so source member offset N maps to
// cache row offset N-4. Current XP calls read members+140..164 from row zero.
std::int32_t read(void* context, std::uint32_t source_member_offset,
                  float* value, std::string& error);

// DesignSettings::CombatMusicTrigger is an integer at source member +0x14.
// The retained cache view is shared with the XP provider and omits the source
// vptr from each serialized row.
std::int32_t read_combat_music_trigger(void* context,std::int32_t* value,
                                      std::string& error);

} // namespace dh2::design_settings_xp_provider_v1
