#pragma once

#include "../game-data/animation_tables.hpp"
#include "../game-data/properties.hpp"
#include "character_animation_ai.hpp"

#include <cstdint>
#include <limits>

namespace dh2::character_anim_table_resolver_v1 {

struct CharacterView {
    std::uintptr_t identity = 0;
    const data::PropertyView* properties = nullptr;
};

struct Result {
    std::int32_t cached_property_index2 = 0;
    std::int32_t table_id = 17;
    std::uint32_t table_count = 0;
    const data::CharacterAnimations* row = nullptr;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    invalid_source_fact,
};

// Character::GetCharAnimTableId reads cached PropertyView property 2, then
// returns that signed index only when it is in the current CharAnimTable
// count; otherwise it returns the source fallback row 17. SM_SetDeadState
// performs its separate row-count check after this getter, so a fallback row
// is usable only when row 17 exists in the freshly supplied table.
inline Status resolve(const CharacterView* character,
                      const data::AnimationTables* tables,
                      Result* output) noexcept {
    if (!character || !character->identity || !character->properties ||
        !output || dh2_property_validate(character->properties))
        return Status::invalid_argument;
    if (!tables || tables->characters.size() >
            static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
        return Status::invalid_source_fact;

    Result result{};
    result.cached_property_index2 = character->properties->resolved[2];
    result.table_count = static_cast<std::uint32_t>(tables->characters.size());
    result.table_id = dh2_character_animation_table_id(
        result.cached_property_index2, static_cast<std::int32_t>(result.table_count));
    if (result.table_id >= 0 &&
        static_cast<std::uint32_t>(result.table_id) < result.table_count)
        result.row = &tables->characters[static_cast<std::size_t>(result.table_id)];
    *output = result;
    return Status::complete;
}

} // namespace dh2::character_anim_table_resolver_v1
