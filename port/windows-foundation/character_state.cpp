#include "character_state.hpp"
#include "character_quest_blob.hpp"

#include <cmath>
#include <unordered_set>
#include <utility>

namespace dh::foundation {
namespace {

void check_text(ValidationResult& result, const std::string& value,
                const std::string& field) {
    if (value.empty() || value.size() > character_text_limit) {
        result.errors.push_back(field + " must contain 1..1024 bytes");
        return;
    }
    for (unsigned char ch : value) {
        if (ch < 0x20 || ch == 0x7f) {
            result.errors.push_back(field + " contains a control character");
            return;
        }
    }
}

void check_nonnegative(ValidationResult& result, float value,
                       const char* field) {
    if (!std::isfinite(value) || value < 0.0f)
        result.errors.push_back(std::string(field) + " must be finite and nonnegative");
}

bool check_count(ValidationResult& result, std::size_t size, const char* field) {
    if (size <= character_collection_limit) return true;
    result.errors.push_back(std::string(field) + " exceeds the 4096-entry limit");
    return false;
}

} // namespace

CharacterState make_default_character(std::string id, std::string name,
                                      std::string class_id) {
    CharacterState state;
    state.id = std::move(id);
    state.name = std::move(name);
    state.class_id = std::move(class_id);
    return state;
}

ValidationResult validate_character_state(const CharacterState& state) {
    std::string quest_error;
    ValidationResult result;
    if (state.schema_version != character_schema_version)
        result.errors.emplace_back("unsupported character schema version");
    check_text(result, state.id, "id");
    check_text(result, state.name, "name");
    check_text(result, state.class_id, "class_id");
    if (state.stats.level == 0) result.errors.emplace_back("level must be at least 1");
    check_nonnegative(result, state.stats.health, "health");
    check_nonnegative(result, state.stats.max_health, "max_health");
    check_nonnegative(result, state.stats.resource, "resource");
    check_nonnegative(result, state.stats.max_resource, "max_resource");
    check_nonnegative(result, state.stats.strength, "strength");
    check_nonnegative(result, state.stats.dexterity, "dexterity");
    check_nonnegative(result, state.stats.intelligence, "intelligence");
    check_nonnegative(result, state.stats.endurance, "endurance");
    check_nonnegative(result, state.stats.energy, "energy");
    if (state.stats.max_health <= 0.0f)
        result.errors.emplace_back("max_health must be positive");
    if (state.stats.health > state.stats.max_health)
        result.errors.emplace_back("health exceeds max_health");
    if (state.stats.resource > state.stats.max_resource)
        result.errors.emplace_back("resource exceeds max_resource");

    // Reject oversize collections before traversing them. Save readers use the
    // same limits to bound allocation before constructing this model.
    const bool inventory_ok = check_count(result, state.inventory.size(), "inventory");
    const bool equipment_ok = check_count(result, state.equipment.size(), "equipment");
    const bool skills_ok = check_count(result, state.skills.size(), "skills");
    const bool unlocks_ok = check_count(result, state.unlocks.size(), "unlocks");
    std::unordered_set<std::string> items;
    if (inventory_ok) for (const auto& item : state.inventory) {
        check_text(result, item.instance_id, "inventory.instance_id");
        check_text(result, item.definition_id, "inventory.definition_id");
        if (!items.insert(item.instance_id).second)
            result.errors.emplace_back("duplicate inventory instance: " + item.instance_id);
        if (item.quantity == 0)
            result.errors.emplace_back("inventory quantity must be at least 1");
    }
    std::unordered_set<std::string> slots;
    std::unordered_set<std::string> equipped_items;
    std::unordered_set<std::uint64_t> native_equipment_slots;
    if (equipment_ok) for (const auto& binding : state.equipment) {
        check_text(result, binding.slot, "equipment.slot");
        check_text(result, binding.item_instance_id, "equipment.item_instance_id");
        if (!slots.insert(binding.slot).second)
            result.errors.emplace_back("duplicate equipment slot: " + binding.slot);
        if (!equipped_items.insert(binding.item_instance_id).second)
            result.errors.emplace_back("item equipped in multiple slots: " + binding.item_instance_id);
        if (inventory_ok && items.count(binding.item_instance_id) == 0)
            result.errors.emplace_back("equipped item absent from inventory: " + binding.item_instance_id);
        const bool legacy_set = binding.equipment_set == -1;
        const bool legacy_slot = binding.source_slot == -1;
        if (legacy_set != legacy_slot ||
            (!legacy_set && (binding.equipment_set < 0 || binding.equipment_set >= 2 ||
                             binding.source_slot < 0 || binding.source_slot >= 9))) {
            result.errors.emplace_back("equipment native set/slot identity is incomplete or outside source bounds");
        } else if (!legacy_set) {
            const auto key = (std::uint64_t(std::uint32_t(binding.equipment_set)) << 32) |
                             std::uint32_t(binding.source_slot);
            if (!native_equipment_slots.insert(key).second)
                result.errors.emplace_back("duplicate native equipment set/slot binding");
        }
    }
    std::unordered_set<std::string> skill_ids;
    if (skills_ok) for (const auto& skill : state.skills) {
        check_text(result, skill.id, "skills.id");
        // Original SkillList is an ordered saved-row table and can contain the
        // same dictionary skill more than once. Source-backed slot maps index
        // those rows directly, so preserve duplicates when that exact row map
        // is known. Generic keyed skill collections remain unique.
        if (!skill_ids.insert(skill.id).second && !state.source_skill_slots_known)
            result.errors.emplace_back("duplicate skill: " + skill.id);
    }
    std::unordered_set<std::string> unlock_ids;
    if (unlocks_ok) for (const auto& unlock : state.unlocks) {
        check_text(result, unlock, "unlocks.id");
        if (!unlock_ids.insert(unlock).second)
            result.errors.emplace_back("duplicate unlock: " + unlock);
    }

    std::unordered_set<std::uint64_t> skill_slot_keys;
    for (const auto& binding : state.skill_slots) {
        if (binding.equipment_set >= 2 || binding.slot > character_collection_limit ||
            binding.saved_skill_row >= state.skills.size()) {
            result.errors.emplace_back("skill slot binding references an invalid set, slot, or saved skill row");
            continue;
        }
        const auto key = (std::uint64_t(binding.equipment_set) << 32) | binding.slot;
        if (!skill_slot_keys.insert(key).second)
            result.errors.emplace_back("duplicate skill slot key in one equipment set");
    }
    if (state.source_faery_list_id < -1)
        result.errors.emplace_back("source faery list ID must be -1 or nonnegative");
    for (const auto& difficulty : state.faery_by_difficulty) {
        if (difficulty.current_faery < 0 || difficulty.current_faery >= 5)
            result.errors.emplace_back("current faery ID outside source five-entry range");
    }
    // Schema v4: difficulty, menu metadata and visited-module section.
    if (state.current_difficulty < 0 || state.current_difficulty >= std::int32_t(character_difficulty_count))
        result.errors.emplace_back("current difficulty outside the original three modes");
    if (state.unlocked_difficulty < 0 || state.unlocked_difficulty >= std::int32_t(character_difficulty_count))
        result.errors.emplace_back("unlocked difficulty outside the original three modes");
    for (const auto row : state.menu_metadata.level_row)
        if (row < -1 || row > 4096) result.errors.emplace_back("menu metadata level row is outside -1..4096");
    for (const auto act : state.menu_metadata.current_act)
        if (act < 0 || act > 4096) result.errors.emplace_back("menu metadata act is outside 0..4096");
    if (!state.menu_metadata.known) {
        const CharacterMenuMetadata blank;
        if (state.menu_metadata.save_time != 0 || state.menu_metadata.level_row != blank.level_row ||
            state.menu_metadata.current_act != blank.current_act)
            result.errors.emplace_back("unknown menu metadata must remain blank");
    }
    if (state.visited_modules.size() > character_visited_limit) {
        result.errors.emplace_back("visited module section exceeds its entry limit");
    } else {
        for (std::size_t i = 0; i < state.visited_modules.size(); ++i) {
            const auto& entry = state.visited_modules[i];
            check_text(result, entry.level_uri, "visited_modules.level_uri");
            if (entry.visited > 1) result.errors.emplace_back("visited module byte must be 0 or 1");
            if (i > 0) {
                const auto& before = state.visited_modules[i - 1];
                if (!(before.level_uri < entry.level_uri ||
                      (before.level_uri == entry.level_uri && before.module_id < entry.module_id)))
                    result.errors.emplace_back("visited modules must be unique and sorted by (level_uri, module_id)");
            }
        }
    }
    if(!validate_character_quest_blob(state.id,state.source_quest_progress_cqpg,quest_error))result.errors.push_back(quest_error);
    return result;
}

} // namespace dh::foundation
