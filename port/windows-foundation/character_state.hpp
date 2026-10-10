#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation {

inline constexpr std::uint32_t character_schema_version = 3;
inline constexpr std::size_t character_collection_limit = 4096;
inline constexpr std::size_t character_text_limit = 1024;

// This is owned game data, independent of the original engine's object layout.
// IDs refer to content definitions; item instance IDs identify owned objects.
struct CharacterStats {
    std::uint32_t level = 1;
    float health = 100.0f;
    float max_health = 100.0f;
    float resource = 100.0f;
    float max_resource = 100.0f;
    float strength = 0.0f;
    float dexterity = 0.0f;
    // Legacy generic field retained with its old meaning. Original source
    // profiles have Stat_Endurance/Stat_Energy; neither aliases this field.
    float intelligence = 0.0f;
    float endurance = 0.0f;
    float energy = 0.0f;
};

struct InventoryItem {
    std::string instance_id;
    std::string definition_id;
    std::uint32_t quantity = 1;
};

struct EquipmentBinding {
    std::string slot;
    std::string item_instance_id;
    // Exact source PlayerEquipment indices when known. -1 means an older
    // generic binding used only the stable textual slot name.
    std::int32_t equipment_set = -1;
    std::int32_t source_slot = -1;
};

// saved_skill_row indexes CharacterState::skills, not SkillTable dictionary IDs.
struct SkillSlotBinding {
    std::uint32_t equipment_set = 0;
    std::uint32_t slot = 0;
    std::uint32_t saved_skill_row = 0;
};

struct CharacterFaeryProgress {
    std::uint8_t state = 0;
    std::uint16_t level = 0;
};

struct CharacterFaeryDifficulty {
    std::int32_t current_faery = 0;
    std::array<CharacterFaeryProgress, 5> faeries{};
};

struct SkillProgress {
    std::string id;
    std::uint32_t rank = 1;
};

struct CharacterState {
    std::uint32_t schema_version = character_schema_version;
    std::string id;
    std::string name;
    std::string class_id;
    CharacterStats stats;
    std::uint64_t experience = 0;
    std::uint64_t gold = 0;
    std::uint32_t source_stat_points = 0;
    std::uint32_t source_skill_points = 0;
    std::vector<InventoryItem> inventory;
    std::vector<EquipmentBinding> equipment;
    std::vector<SkillProgress> skills;
    std::vector<std::string> unlocks;
    // These flags distinguish an old save's unknown fields from a source
    // producer that actually read the corresponding values.
    bool source_endurance_energy_known = false;
    bool source_points_known = false;
    bool source_skill_slots_known = false;
    bool source_faery_state_known = false;
    std::vector<SkillSlotBinding> skill_slots;
    std::int32_t source_faery_list_id = -1;
    std::array<CharacterFaeryDifficulty, 3> faery_by_difficulty{};
    // Portable source quest codec, owned by this character. Empty remains
    // unknown on older saves; only explicit fresh/source producers initialize.
    std::vector<std::uint8_t> source_quest_progress_cqpg;
};

// Temporary controller state is deliberately not part of CharacterState/save data.
enum class CharacterAction { idle, moving, attacking, casting, hurt, dead };
struct CharacterRuntimeState {
    CharacterAction action = CharacterAction::idle;
    float action_elapsed_seconds = 0.0f;
    std::string target_id;
};

struct ValidationResult {
    std::vector<std::string> errors;
    bool ok() const noexcept { return errors.empty(); }
};

// Defaults are foundation fixtures, not recovered campaign balance values.
CharacterState make_default_character(std::string id = "player",
                                      std::string name = "Player",
                                      std::string class_id = "warrior");
ValidationResult validate_character_state(const CharacterState& state);

} // namespace dh::foundation
