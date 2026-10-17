#pragma once

#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/class_tables.hpp"
#include "../game-data/data.hpp"
#include "../game-data/properties.hpp"

#include <cstdint>

namespace dh2::character_level_up_properties_v1 {

struct Owner {
    std::uintptr_t character = 0;
    data::PropertyView* properties = nullptr;
    data::PlayerSavegameV1* save = nullptr;
    std::int32_t* mutable_base = nullptr;
};

struct Result {
    std::uint32_t calls = 0;
    std::uint32_t level_add_reached = 0;
    std::uint32_t xp_reset_reached = 0;
    std::uint32_t base_reset_reached = 0;
    std::uint32_t base_row_loaded = 0;
    std::uint32_t class_recalc_reached = 0;
    std::uint32_t class_result = 0;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    property_failed
};

// IDA-backed Character::LevelUp prefix at 0x3bec6c-0x3bec88:
// PROPS_AddInt(19, 1), then PROPS_SetInt(33, 0). Save identity must match the
// same Character. UpdateBaseProperties, regeneration, player-level Save, SG_Save
// and the later presentation/script tail are separate, not implied here.
Status apply(const Owner*, Result*);

// CharacterProperties::UpdateBaseProperties(classid), pinned at 0x3e087c:
// reset the base sheet to source defaults, load CharacterTable row classid,
// then run the existing live base-class/property resolver. The caller supplies
// Character+0x13c8's class row and the canonical tables retained by gameplay.
Status update_base_properties(const Owner*, const data::PropertyRules*,
                              const data::CharacterTable*,
                              const data::ClassTables*, std::int32_t class_row,
                              Result*);

} // namespace dh2::character_level_up_properties_v1
