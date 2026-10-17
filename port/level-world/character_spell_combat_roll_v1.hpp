#pragma once

#include "../game-data/combat_result.hpp"
#include <cstdint>
#include <string>

namespace dh2::character_spell_combat_roll_v1 {

// Recovered Character::_SpellCombatRoll source Value tags.
constexpr std::uint32_t userdata_type = 2;
constexpr std::uint32_t object_type = 7;
constexpr std::uint32_t false_value_tag = 8;
constexpr std::uint32_t spell_attack_mask = 0x1005554au;

enum class Disposition : std::uint32_t { unchanged, integer, boolean_false };
struct Request {
    std::uintptr_t character;
    std::uint32_t argument_count;
    std::uint32_t argument_type;
    // Result of Value::getUserData for Arguments[0], or zero when it returns null.
    std::uintptr_t userdata;
};
struct Result {
    std::uintptr_t character = 0;
    std::uintptr_t target = 0;
    std::int32_t faery_slot = -1;
    std::int32_t element = 0;
    std::int32_t amount = 0;
    std::uint32_t provider_calls = 0;
    Disposition disposition = Disposition::unchanged;
};
struct Services {
    void* context;
    // Character::SG_GetCurrentFaerieId(-1), then Character::GetCharFaery(slot).
    int (*current_faery)(void*, std::uintptr_t, std::int32_t*, std::string&);
    int (*faery_element)(void*, std::uintptr_t, std::int32_t, std::int32_t*, std::string&);
    // ObjectHandle conversion. Zero denotes the source's null Character* result.
    int (*resolve_character)(void*, std::uintptr_t, std::uintptr_t*, std::string&);
    // First virtual Value::getValue result; the second virtual call runs only
    // when the source compares that integer result with 8.
    int (*read_value_tag)(void*, std::uintptr_t, std::uint32_t*, std::string&);
    int (*consume_false_value)(void*, std::uintptr_t, std::string&);
    // F_SpellAttack, followed by F_ApplyResult(..., false), in source order.
    int (*spell_attack)(void*, std::uintptr_t, std::uintptr_t, std::int32_t,
                        std::uint32_t, std::int32_t, std::int32_t,
                        data::CombatResult*, std::string&);
    int (*apply_result)(void*, std::uintptr_t, std::uintptr_t, std::uint32_t,
                        data::CombatResult*, std::string&);
};

enum class Status : std::int32_t { complete = 1, invalid_argument = -1, provider_failed = -2 };
Status execute(const Request*, const Services*, Result*, std::string& error);

} // namespace dh2::character_spell_combat_roll_v1
