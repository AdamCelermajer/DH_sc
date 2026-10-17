#pragma once

#include "skill_tables.hpp"
#include "player_save_level_states_v1.hpp"
#include "quest_savegame_v1.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh2::data {

struct PropertyView;

struct SavedSkill8V1 {
    std::int32_t id{};
    std::uint16_t level{};
    std::uint8_t flag{};
    std::uint8_t reserved{};
};

struct PlayerProfileSpan24V1 {
    const std::uint8_t* data{};
    std::uint32_t size{};
    std::uint32_t cursor{};
    std::uint32_t reserved0{};
    std::uint32_t reserved1{};
};

struct SavedSkillsView16V1 {
    SavedSkill8V1* rows{};
    std::uint32_t count{};
    std::uint32_t reserved{};
};

struct SavedSkillsLoadServices32V1 {
    void* context{};
    std::int32_t (*skill_id)(void*, const std::uint8_t*, std::uint32_t){};
    std::uint32_t* (*slot_value)(void*, std::uint32_t set, std::int32_t key){};
    void (*read_observer)(void*, std::uint32_t bytes){};
};

static_assert(sizeof(SavedSkill8V1) == 8);
static_assert(sizeof(PlayerProfileSpan24V1) == 24);
static_assert(sizeof(SavedSkillsView16V1) == 16);
static_assert(sizeof(SavedSkillsLoadServices32V1) == 32);

struct SavedSkillUpdateServicesV1 {
    void* context{};
    bool (*update_skills)(void*, std::uintptr_t character, std::string&){};
};

struct SavedFaery4V1 {
    std::uint8_t state{};
    std::uint8_t reserved{};
    std::uint16_t level{};
};

static_assert(sizeof(SavedFaery4V1) == 4);

// Exact LNAM destinations. +0xfc/+0x15c are the corresponding act slots inside
// this Save's two source quest logs. Both logs borrow these canonical words.
struct SavedLevelNameFieldsV1 {
    // Historical projection name retained for compatibility: +0x38 is the
    // raw save date, written by SG_SetSaveDate, not a LevelTable association.
    std::uint32_t level_id{};
    std::array<std::int32_t, 3> word50{}, word5c{};
    std::array<std::int32_t, 3> quest_wordfc{}, quest_word15c{};
};

// The one source-owned PlayerSavegame projection. It owns saved fields only;
// Character properties, SkillTables, inventory and profile-file orchestration
// remain borrowed owners at their original boundaries.
class PlayerSavegameV1 {
public:
    PlayerSavegameV1() {
        // Blank source C1(0x465ae0) constructs real QuestSavegame logs at
        // +0xb8/+0x118; each QuestSavegame C1 writes +0x44[3] to one.
        level_name_fields_.quest_wordfc.fill(1);
        level_name_fields_.quest_word15c.fill(1);
        source_quest_sync_20_ = 0;
    }
    PlayerSavegameV1(const PlayerSavegameV1&) = delete;
    PlayerSavegameV1& operator=(const PlayerSavegameV1&) = delete;

    // The sole Save+10 store. Recovered callers separately perform their
    // reached embedded QuestSavegame stores in original instruction order.
    void set_character_only(std::uintptr_t identity) noexcept { character_ = identity; }
    // Convenience composition of Character::SG_SetPlayer28B's exact nonnull
    // stores: embedded quest+174, Save+10, then embedded quest+114.
    void set_character(std::uintptr_t identity) noexcept {
        quest_log_118_.character_5c=identity;
        set_character_only(identity);
        quest_log_b8_.character_5c=identity;
    }
    void set_slot(std::int32_t slot) noexcept { slot_ = slot; }
    // Character::SG_SetPlayerClass writes the sole Save +0x34 word.
    void set_class(std::int32_t value) noexcept { class_ = value; }
    // Source indexed ctor metadata projection; only a fresh, unbound Save is
    // accepted. The caller subsequently performs its genuine SG_Load(1).
    bool initialize_new_profile_metadata(std::int32_t slot, std::string&);
    void set_player_level(std::int32_t value) noexcept { level_ = value; }
    void set_player_name(const std::string& value) { name_ = value; }
    // Character::SG_SetLevelEntryPointEii: difficulty -1 resolves to the
    // source process difficulty before this same Save's LEPT word is written.
    bool set_level_entry_point(std::uint32_t difficulty, std::int32_t value) noexcept {
        if (difficulty >= level_entry_points_.size()) return false;
        level_entry_points_[difficulty] = value;
        return true;
    }
    void set_unlocked_difficulty(std::int32_t value) noexcept { unlocked_difficulty_ = value; }
    void set_save_date(std::uint32_t value) noexcept { level_name_fields_.level_id = value; }
    std::uint32_t save_date() const noexcept { return level_name_fields_.level_id; }
    void generate_profile_seeds(std::uint32_t source_time) noexcept;
    bool set_new_profile_locations(std::uint32_t difficulty_count, std::string&);
    void set_saved_properties_byte_194(std::uint8_t value) noexcept { properties_byte_194_ = value; }
    bool source_save_blocked() const noexcept { return save_blocked_; }
    void set_source_save_blocked(bool value) noexcept { save_blocked_ = value; }
    std::int32_t source_save_mode() const noexcept { return save_mode_; }
    void set_source_save_mode(std::int32_t value) noexcept { save_mode_ = value; }
    // Whole __LoadProperties on this Save's nonnull Character and the same
    // existing saved sheet. No recalculation; raw byte +0x194 is retained.
    bool load_properties(Bytes, PropertyView&, std::size_t& consumed,
                         std::string& error);
    std::uint8_t saved_properties_byte_194() const noexcept { return properties_byte_194_; }

    // Mirrors Character::GetCharSkillListId's row-3 fallback, then the
    // PlayerSavegame::_InitSkills ownership boundary. Values begin at zero.
    bool initialize_skills(const SkillTables& tables,
                           std::int32_t live_skill_tree_selector,
                           std::string& error);
    // The original body receives this list from Character::GetCharSkillList.
    // Kept as a narrow boundary for direct instruction-derived fixtures.
    bool initialize_skills_from_character_list(
        const std::vector<std::int32_t>& selected_ids, std::string& error);
    // Character::SG_ReloadSkills' same-Save replacement: retire the previous
    // row allocation, publish the null state, rebuild from the selected
    // Character list, then clear both source slot maps. The caller delivers
    // the subsequent SG_Load(8) through this Save's existing LoadOwner.
    bool reload_skills_from_character_list(
        const std::vector<std::int32_t>& selected_ids, std::string& error);

    // Reads only the original named Skills payload. No outer SG_Load/profile
    // lifecycle, disk I/O, or property/inventory copies are performed here.
    int load_skills(Bytes bytes, const SkillTables& tables,
                    std::size_t& consumed, std::string& error);
    bool load_name(Bytes bytes, std::size_t& consumed, std::string& error);
    bool load_level(Bytes bytes, std::size_t& consumed, std::string& error);
    bool load_class(Bytes bytes, const std::vector<std::string>& class_names,
                    std::size_t& consumed, std::string& error);
    bool load_level_name(Bytes, std::size_t& consumed, std::string&);
    bool load_level_entry_points(Bytes, std::size_t& consumed, std::string&);
    bool load_use_spawn_points(Bytes, std::size_t& consumed, std::string&);
    // NativeStartGame clears the selected LUSP byte after copying its old
    // value to Application::LoadLevel arguments. Only a loaded indexed Save
    // can receive that source mutation.
    bool clear_use_spawn_point(std::size_t difficulty, std::string& error);

    bool set_skill_level(std::uint32_t row, std::int32_t level,
                         std::string& error);
    bool set_skill_in_slot(std::int32_t slot, std::uint32_t row,
                           const SavedSkillUpdateServicesV1& services,
                           std::string& error);
    std::int32_t skill_id(std::uint32_t row) const noexcept;
    std::int32_t skill_level(std::uint32_t row) const noexcept;
    std::int32_t skill_in_slot(std::int32_t slot) const noexcept;
    std::int32_t skill_slot(std::uint32_t row) const noexcept;

    void initialize_faeries() noexcept;
    bool load_current_faery(Bytes bytes, std::size_t& consumed,
                            std::string& error);
    bool load_faeries(Bytes bytes, std::size_t& consumed,
                      bool& source_count_mismatch, std::string& error);
    // __LoadDifficulty first stores the static PlayerSavegame::m_difficultyLevel
    // (original 0x9a6060), then reads this Save's +0x3c unlocked word.
    bool load_difficulty(Bytes bytes, void* context,
                         bool (*store_selected)(void*, std::int32_t,
                                                std::string&),
                         std::size_t& consumed, std::string& error);
    bool set_faery_level(std::uint32_t id, std::int32_t value,
                         std::uint32_t difficulty, std::string& error);
    bool set_faery_state(std::uint32_t id, std::int32_t value,
                         std::uint32_t difficulty, std::string& error);
    // Character::SG_SetCurrentFaerie writes the selected faery index in the
    // requested difficulty's existing Save. This does not grant/unlock it.
    bool set_current_faery(std::uint32_t id, std::uint32_t difficulty,
                           std::string& error);
    std::int32_t faery_level(std::uint32_t id,
                             std::uint32_t difficulty) const noexcept;
    std::int32_t current_faery(std::uint32_t difficulty) const noexcept;

    bool skills_initialized() const noexcept { return skills_initialized_; }
    bool has_skill_slots() const noexcept { return !slots_[0].empty(); }
    const std::vector<SavedSkill8V1>& skills() const noexcept { return skills_; }
    const std::array<std::map<std::int32_t, std::uint32_t>, 2>&
    skill_slots() const noexcept { return slots_; }
    const std::array<std::array<SavedFaery4V1, 5>, 3>& faeries() const noexcept {
        return faeries_;
    }
    const std::array<bool, 3>& faeries_initialized() const noexcept {
        return faeries_initialized_;
    }
    const std::array<std::int32_t, 3>& current_faeries() const noexcept {
        return current_faery_;
    }
    std::int32_t unlocked_difficulty() const noexcept { return unlocked_difficulty_; }
    std::int32_t slot() const noexcept { return slot_; }
    std::int32_t level() const noexcept { return level_; }
    std::int32_t class_id() const noexcept { return class_; }
    const std::string& name() const noexcept { return name_; }
    std::uintptr_t character() const noexcept { return character_; }
    // Offline PlayerSavegame::SG_TryQuestSync stores this byte before
    // UpdateQuests. It is the canonical Save+0x14 state, initialized by C1.
    std::uint8_t source_quest_sync_20() const noexcept { return source_quest_sync_20_; }
    bool source_quest_sync_update(std::uintptr_t expected_character,
                                  bool online) noexcept {
        if (online || !expected_character || character_ != expected_character ||
            quest_log_b8_.character_5c != expected_character ||
            quest_log_118_.character_5c != expected_character) return false;
        source_quest_sync_20_ = 1;
        return true;
    }
    const SavedLevelNameFieldsV1& level_name_fields() const noexcept { return level_name_fields_; }
    // Sole canonical backing for embedded source QuestSavegame +0x44[3].
    std::array<std::int32_t,3>& quest_log_b8_act_words_v1() noexcept { return level_name_fields_.quest_wordfc; }
    std::array<std::int32_t,3>& quest_log_118_act_words_v1() noexcept { return level_name_fields_.quest_word15c; }
    quest_savegame_v1::QuestSavegame& source_quest_log_b8() noexcept { return quest_log_b8_; }
    quest_savegame_v1::QuestSavegame& source_quest_log_118() noexcept { return quest_log_118_; }
    const quest_savegame_v1::QuestSavegame& source_quest_log_b8() const noexcept { return quest_log_b8_; }
    const quest_savegame_v1::QuestSavegame& source_quest_log_118() const noexcept { return quest_log_118_; }
    const std::array<std::int32_t, 3>& level_entry_points() const noexcept { return level_entry_points_; }
    const std::array<std::uint8_t, 3>& use_spawn_points() const noexcept { return use_spawn_points_; }
    // Original blank ctor initializes entry points, but leaves level ID and
    // spawn bytes untouched. These flags prevent port zero storage from being
    // represented as a loaded campaign value before its source producer.
    bool level_name_loaded() const noexcept { return level_name_loaded_; }
    bool use_spawn_points_loaded() const noexcept { return use_spawn_points_loaded_; }
    // Source _InitLevelStates initializes only null +68/+74 arrays, separately
    // in each difficulty. The second table is WorldMap, not FastTravelList.
    bool initialize_level_states(const SavedLevelStateServicesV1&,std::string&);
    // Original blank/indexed C1 initialize six +17c..193 bitset words to zero.
    std::array<std::uint32_t,2>* source_fast_travel_bits(std::uint32_t difficulty) noexcept {
        return difficulty<3?&fast_travel_bits_[difficulty]:nullptr;
    }
    const std::array<std::uint32_t,2>* source_fast_travel_bits(std::uint32_t difficulty) const noexcept {
        return difficulty<3?&fast_travel_bits_[difficulty]:nullptr;
    }
    SavedStateArrayV1* source_level_states(std::uint32_t difficulty) noexcept {
        return difficulty<3?&level_states_[difficulty]:nullptr;
    }
    SavedStateArrayV1* source_world_map_states(std::uint32_t difficulty) noexcept {
        return difficulty<3?&world_map_states_[difficulty]:nullptr;
    }
    const SavedStateArrayV1* source_level_states(std::uint32_t difficulty) const noexcept {
        return difficulty<3?&level_states_[difficulty]:nullptr;
    }
    const SavedStateArrayV1* source_world_map_states(std::uint32_t difficulty) const noexcept {
        return difficulty<3?&world_map_states_[difficulty]:nullptr;
    }

private:
    std::int32_t slot_{-1};
    std::int32_t level_{};
    std::int32_t class_{-1};
    std::string name_;
    std::uintptr_t character_{};
    std::uint8_t source_quest_sync_20_{};
    bool skills_initialized_{};
    std::vector<SavedSkill8V1> skills_;
    std::array<std::map<std::int32_t, std::uint32_t>, 2> slots_;
    std::array<std::array<SavedFaery4V1, 5>, 3> faeries_{};
    std::array<bool, 3> faeries_initialized_{};
    std::array<std::int32_t, 3> current_faery_{};
    std::int32_t unlocked_difficulty_{};
    SavedLevelNameFieldsV1 level_name_fields_;
    // One vector/field owner per source embedded log. Factories retain child
    // leases and must finish the explicit QuestSavegame destruction closure
    // before this Save retires; C++ vector teardown does not delete QuestRefs.
    quest_savegame_v1::QuestSavegame quest_log_b8_{level_name_fields_.quest_wordfc};
    quest_savegame_v1::QuestSavegame quest_log_118_{level_name_fields_.quest_word15c};
    std::array<std::int32_t, 3> level_entry_points_{};
    std::array<std::uint8_t, 3> use_spawn_points_{};
    bool level_name_loaded_{}, use_spawn_points_loaded_{};
    std::uint8_t properties_byte_194_{};
    bool save_blocked_{}; // source +0xc
    std::int32_t save_mode_{}; // source +0x178, blank ctor zero
    std::array<std::array<std::uint32_t,2>,3> fast_travel_bits_{};
    std::array<SavedStateArrayV1,3> level_states_,world_map_states_;
    bool level_states_busy_{};
};

}  // namespace dh2::data

namespace dh2::player_saved_skill_callbacks_v1 {

enum class Disposition : std::uint32_t {
    append_integer,
    no_return,
    source_assertion_boundary,
    owner_mismatch,
    missing_projection,
};

struct Result {
    Disposition disposition{Disposition::missing_projection};
    std::int32_t integer{};
};

// Synchronous read-only projections of Character::_GetSkillIDFromOID and
// Character::_GetCurrentSkillInfo__. `properties`, `tables`, and `savegame`
// are borrowed; the adapter never copies or stores them. Selector property 28
// is the source SkillTree value and is read live for each invocation.
Result get_skill_id_from_oid(const data::PropertyView* properties,
                             const data::SkillTables* tables,
                             std::int32_t object_id) noexcept;
Result get_current_skill_info(const data::PropertyView* properties,
                              const data::SkillTables* tables,
                              const data::PlayerSavegameV1* savegame,
                              std::uintptr_t character_identity,
                              std::int32_t skill_list_index) noexcept;

}  // namespace dh2::player_saved_skill_callbacks_v1

extern "C" {
// This preserves the bounded source section reader. Status 0 is success,
// -1 malformed native input, -2 truncated bytes, -3 required service failure.
int dh2_player_skills_v1_load(dh2::data::SavedSkillsView16V1*,
                              dh2::data::PlayerProfileSpan24V1*,
                              const dh2::data::SavedSkillsLoadServices32V1*) noexcept;
int dh2_saved_skill_v1_level(const dh2::data::SavedSkillsView16V1*,
                             std::uint32_t) noexcept;
int dh2_saved_skill_v1_set_level(dh2::data::SavedSkillsView16V1*,
                                 std::uint32_t, std::int32_t) noexcept;
}
