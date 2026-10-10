#pragma once
// Thin, general Windows quest runtime (Preview 16).
//
// Semantics follow the original Quest::Update chain (IDA 0x481818; transitions in
// UpdateLocked/Available/Active/Completed/... and SetState 0x480c78) as recorded
// in level-world native_quest_runtime_v76 (state order and script indices).
// Quest scripts are not executed here (no script runtime in this build); script
// waits are treated as finished and every unsupported authored feature is logged
// once in diagnostics(), never silently dropped.
//
// The state of every authored row lives in the CQPG envelope owned by the
// CharacterState (CharacterQuestProgressV1); objective counters use the CQPG v2
// counter section (QuestObjectiveCounterV2). Nothing here is map- or act-specific.
#include "quest_events_v1.hpp"
#include "quest_table_v1.hpp"
#include "../../character_state.hpp"
#include "../quests/character_quest_progress_v1.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dh::foundation::quest_runtime {

// Original v2QuestState values (v2quests_pycst; native_quest_runtime_v76 keys).
enum class QuestStateV1 : std::int32_t {
    locked = 0, post_locked, pre_available, available, post_available,
    pre_active, active, post_active, pre_completed, completed,
    post_completed, pre_closed, closed, post_closed,
};

// Original v2QuestObjectiveType values (v2quests_pycst).
enum class QuestObjectiveTypeV1 : std::int32_t {
    kill_x_enemies = 0, clear_enemies = 1, trigger_plate = 2, destroy_game_object = 3,
    move_in_zone = 4, talk_to_npc = 5, automatic = 6, open_game_object = 7,
    trigger_on = 8, picked_up_liftable = 9, kill_enemy_template = 10,
    clear_enemy_template = 11, gather_loot = 12, invalid = 13,
};

// Original v2ConditionOperator values used by quest prerequisites.
enum class QuestConditionOperatorV1 : std::int32_t {
    is_quest_in_state = 0, is_quest_state_lower = 1, is_quest_state_higher = 2, is_player_in_level = 3,
};

// Original v2QuestRewardType values.
enum class QuestRewardTypeV1 : std::int32_t {
    gold = 0, xp = 1, character_props = 2, loot = 3, consume_loot = 4, invalid = 5,
};

// v2QuestPriority.Debug (v2quests_pycst). Priority Primary 0, Secondary 1, Dialog 3.
inline constexpr std::int32_t kQuestPriorityDebugV1 = 2;

// Quest Log list policy (features/quests): Active 6..12 is Assigned, above 12 Completed.
inline constexpr std::int32_t kQuestAssignedFirstStateV1 = 6;
inline constexpr std::int32_t kQuestAssignedLastStateV1 = 12;

// Objective slots inside the CQPG v2 counter section. Authored objectives use
// their index (< kAcceptObjectiveV1); accept/end get fixed slots.
inline constexpr std::uint32_t kAcceptObjectiveV1 = 62;
inline constexpr std::uint32_t kEndObjectiveV1 = 63;

struct QuestBannerV1 {
    // new_quest = original GLOBAL_QUEST_NEW dialog (state entered Active);
    // completed = original QuestCompletedMsgDialog (state entered Closed).
    enum class Kind : std::uint8_t { new_quest, completed };
    Kind kind{Kind::new_quest};
    std::int32_t row{-1};
    std::int32_t difficulty{0};
    std::int32_t objective_text_id{-1}; // authored objective description StringID
    std::string text;                   // resolved text, empty when no resolver answers
    std::int32_t reward_xp{0};          // authored XP reward granted (completed banner)
    std::int32_t reward_gold{0};        // authored gold reward granted (completed banner)
};

struct QuestRuntimeServicesV1 {
    // Experience owner (the same award path as kill XP). Absent owner: XP rewards are
    // reported as not granted (diagnostic), never faked into CharacterState.
    std::function<bool(std::int32_t amount, std::string& error)> give_experience;
    // StringID to localized text (quest StringIDs, e.g. objective descriptions).
    std::function<bool(std::int32_t string_id, std::string& text)> text;
    // Current level row (Level::m_row, the original IsPlayerInLevel key). -1 when unknown:
    // IsPlayerInLevel prerequisites then stay false and are reported.
    std::function<std::int32_t()> current_level_row;
};

struct QuestObjectiveProgressV1 {
    std::int32_t quantity{0};
    bool completed{false};
};

struct QuestRowProgressV1 {
    std::vector<QuestObjectiveProgressV1> objectives;
    QuestObjectiveProgressV1 accept;
    QuestObjectiveProgressV1 end;
};

class QuestRuntimeV1 {
public:
    // Borrows the character (same identity for its lifetime) and retains the
    // decoded table. Difficulty is read from the character each call.
    QuestRuntimeV1(CharacterState& character, std::shared_ptr<const QuestTableV1> table,
        QuestRuntimeServicesV1 services);

    // Loads the CQPG envelope and its counters. An empty CQPG field is the new-character
    // path: every row is initialized from its authored state and written back.
    bool load(std::string& error);
    // Writes the states, currentquest and objective counters to the CharacterState.
    bool save(std::string& error);

    // NPC/zone/dialogue accept entry point. The row must be Available (prerequisites
    // met); its accept objective is completed and transitions run.
    bool accept_quest(std::int32_t row, std::string& error);
    // Quest Log MAKE ACTIVE. Only Assigned rows (Active..Closed) may become current.
    bool make_active(std::int32_t row, std::string& error);

    // Event intake from the game. Matching objectives progress, then transitions run.
    // Returns the number of objective updates applied.
    std::size_t handle(const QuestEvent& event, std::string& error);
    // Runs transitions until no row changes (also used after load).
    bool update(std::string& error);

    // Banners queued since the last call (NEW QUEST / QUEST COMPLETED).
    std::vector<QuestBannerV1> take_banners();

    bool state_of(std::int32_t row, QuestStateV1& state) const;
    const QuestRowProgressV1* progress_of(std::int32_t row) const;
    std::int32_t current_quest() const;
    // Unsupported authored features and failed services, each reported once.
    const std::vector<std::string>& diagnostics() const noexcept { return diagnostics_; }
    const QuestTableV1& table() const noexcept { return *table_; }

private:
    using RowKey = std::pair<std::int32_t, std::int32_t>; // (difficulty, row)
    CharacterState& character_;
    std::shared_ptr<const QuestTableV1> table_;
    QuestRuntimeServicesV1 services_;
    CharacterQuestProgressV1 progress_;
    std::map<RowKey, QuestRowProgressV1> rows_;
    std::vector<QuestBannerV1> banners_;
    std::vector<std::string> diagnostics_;
    std::vector<std::string> reported_;

    std::int32_t difficulty() const;
    bool row_valid(std::int32_t row) const noexcept;
    QuestRowProgressV1& row_progress(std::int32_t difficulty, std::int32_t row);
    bool set_state(std::int32_t row, QuestStateV1 next, std::string& error);
    bool prerequisites_met(std::int32_t row, bool& met);
    bool condition_met(const dh2::data::QuestConditionDefinitionV51& c, bool& met);
    bool objective_matches(const dh2::data::QuestObjectiveDefinitionV51& def,
        const QuestEvent& event) const;
    void report_unsupported(const dh2::data::QuestObjectiveDefinitionV51& def, std::int32_t row,
        std::int32_t objective);
    // Applies one event to one objective slot. Returns true when it changed progress.
    bool apply_to(const dh2::data::QuestObjectiveDefinitionV51& def,
        QuestObjectiveProgressV1& slot, const QuestEvent& event);
    bool complete_automatic(const dh2::data::QuestObjectiveDefinitionV51& def,
        QuestObjectiveProgressV1& slot);
    bool grant_rewards(std::int32_t row, std::string& error);
    void report_once(const std::string& text);
    bool write_counters(std::string& error);
};

} // namespace dh::foundation::quest_runtime
