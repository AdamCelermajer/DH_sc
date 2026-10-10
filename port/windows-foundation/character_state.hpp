#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation {

inline constexpr std::uint32_t character_schema_version = 4;
// Schema history: 1 base, 2 source stats/equipment/skills/faery, 3 CQPG quest blob,
// 4 menu metadata + current/unlocked difficulty + visited-module section.
inline constexpr std::uint32_t character_difficulty_count = 3;
inline constexpr std::size_t character_visited_limit = 16384;
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

// Per-slot metadata the main-menu slot panel shows (original LNAM/QEST/PDFL
// fields written by PlayerSavegame at SG_SavePlayer / FS_StartGame). known is
// false on v1-v3 saves and until the first save point stamps the block; the
// panel then shows blank metadata instead of inventing it.
struct CharacterMenuMetadata {
    bool known = false;
    // Source SG_SetSaveDate word (time(nullptr), read back as signed 32-bit).
    std::uint32_t save_time = 0;
    // Source LevelList row per difficulty (LNAM level word); -1 = unset.
    std::array<std::int32_t, 3> level_row{-1, -1, -1};
    // Source QEST current act per difficulty (SG_GetCurrentAct); 1 = Act 1.
    std::array<std::int32_t, 3> current_act{1, 1, 1};
};

// Optional visited-room record (source Module::visited3fc, one byte per
// module). Reserved for the map stream; may stay empty. Key = (level_uri,
// module_id), unique, stored sorted by that key.
struct CharacterVisitedModule {
    std::string level_uri;
    std::uint32_t module_id = 0;
    std::uint8_t visited = 0;
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
    // Schema v4. Source PlayerSavegame::m_difficultyLevel / UnlockedDiff (PDFL):
    // 0 Normal, 1 Hard, 2 Heroic. Legacy saves load as Normal/Normal. Read via
    // character_current_difficulty()/character_unlocked_difficulty().
    std::int32_t current_difficulty = 0;
    std::int32_t unlocked_difficulty = 0;
    CharacterMenuMetadata menu_metadata;
    std::vector<CharacterVisitedModule> visited_modules;
};

// The single accessors other streams use for the active difficulty (0..2).
inline std::int32_t character_current_difficulty(const CharacterState& state) noexcept {
    return state.current_difficulty;
}
inline std::int32_t character_unlocked_difficulty(const CharacterState& state) noexcept {
    return state.unlocked_difficulty;
}

// Temporary controller state is deliberately not part of CharacterState/save data.
enum class CharacterAction { idle, moving, attacking, casting, hurt, dead, knocked_back };
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
