#pragma once
// v21.8 subclass: a subclass CharacterTable row such as "KnightPlayerBase_Berserker" is its own actor row (SkillTree,
// ClassString, ClassDescString and Loot differ), but its ModelFile, AnimTable, AI, Sounds, Scale, ClassID and AIFaction
// are those of the base row. The original IDA (ItemInstance::IsEquippableBy, 0x3fa330) names the subclass rows directly.
// coordination/v21.8/check_subclass_bindings_v21_8.py verifies the field equality against the original CharacterTable.
//
// Player presentation lookups (creation class, actor-profiles-v2.xml, locomotion and attack-bank profile) use the family
// base row returned here. The actor definition and CharacterState.class_id keep the subclass row, as the original does.
#include <string>
#include <string_view>

namespace dh::foundation {

// "KnightPlayerBase_Berserker" -> "KnightPlayerBase"; "KingPlayer" and base rows are returned unchanged.
inline std::string playable_base_class_row(std::string_view row) {
    constexpr std::string_view kPlayerBase = "PlayerBase";
    const auto underscore = row.find('_');
    if (underscore == std::string_view::npos) return std::string(row);
    const auto base = row.substr(0, underscore);
    if (base.size() < kPlayerBase.size() || base.substr(base.size() - kPlayerBase.size()) != kPlayerBase) return std::string(row);
    return std::string(base);
}

} // namespace dh::foundation
