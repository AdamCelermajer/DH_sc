#pragma once

#include "../game-data/combat_result.hpp"
#include <cstdint>
#include <string>

namespace dh2::character_skill_combat_roll_v1 {

constexpr std::uint32_t userdata_type = 2;
constexpr std::uint32_t object_type = 7;
constexpr std::uint32_t false_value_tag = 8;
constexpr std::uint32_t skill_type_active = 2;
constexpr std::uint32_t skill_type_passive = 7;
constexpr std::uint32_t dual_wield_flag = 0x00800000u;
constexpr std::uint32_t offhand_attack_flag = 0x04000000u;

enum class Disposition : std::uint8_t { unchanged, attacks, boolean_false };
struct Request {
    std::uintptr_t character{};
    std::uint32_t argument_count{};
    std::uint32_t skill_index{};
    std::uint32_t skill_list_size{};
    std::uint32_t target_argument_type{};
    std::uintptr_t target_userdata{};
};
struct Result {
    struct Attack {
        std::uint32_t flags{};
        std::int32_t skill_element{};
        std::int32_t amount{};
    } attacks[2]{};
    std::uintptr_t character{};
    std::uintptr_t target{};
    std::uint32_t provider_calls{};
    std::uint32_t attack_count{};
    Disposition disposition{Disposition::unchanged};
};
struct Services {
    void* context{};
    // Character::GetCharSkill(skill_index): read type at +116, flags at +28,
    // and field +20. Source accepts only skill types 2 and 7.
    int (*skill_metadata)(void*, std::uintptr_t, std::uint32_t,
                          std::uint32_t*, std::uint32_t*, std::int32_t*,
                          std::string&){};
    // ObjectHandle conversion; zero denotes an unresolved/non-Character target.
    int (*resolve_character)(void*, std::uintptr_t, std::uintptr_t*, std::string&){};
    // Only dual-wield skills query these, in main-hand then offhand order.
    int (*has_main_hand_weapon)(void*, std::uintptr_t, bool*, std::string&){};
    int (*has_off_hand_weapon)(void*, std::uintptr_t, bool*, std::string&){};
    // Non-Character fallback: Value::getValue, then vtable +0x98 for tag 8.
    int (*read_value_tag)(void*, std::uintptr_t, std::uint32_t*, std::string&){};
    int (*consume_false_value)(void*, std::uintptr_t, std::string&){};
    // Each leg receives the source skill value +20; the adapter resolves the
    // equipped-item damage category inside F_SkillAttack's formula.
    int (*skill_attack)(void*, std::uintptr_t, std::uintptr_t, std::uint32_t,
                        std::int32_t, data::CombatResult*, std::string&){};
    int (*apply_result)(void*, std::uintptr_t, std::uintptr_t, std::uint32_t,
                        data::CombatResult*, std::string&){};
};

enum class Status : std::int32_t { complete = 1, invalid_argument = -1, provider_failed = -2 };
Status execute(const Request*, const Services*, Result*, std::string& error);

} // namespace dh2::character_skill_combat_roll_v1
