#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace dh2::native::ui {

// ActionScript's authored journal callbacks put the output container first.
// Keep the adapter independent of GamesWF so its source ABI can be exercised
// by the host regression target as well as the Android registration layer.
struct QuestJournalCallbackValueV1 {
    enum class Kind { other, number, string, object } kind = Kind::other;
    double number = 0.0;
    const char* text = nullptr;
    void* object = nullptr;
};

struct QuestJournalListCallV1 {
    void* output_array = nullptr;
    double player_index = 0.0;
    double start = 0.0;
    double count = 0.0;
    const char* predicate = nullptr;
};

inline bool decode_quest_journal_list_call_v1(
        const QuestJournalCallbackValueV1* args, std::size_t nargs,
        QuestJournalListCallV1& call) {
    // Optional sixth IsMultiplayer argument is present in the SWF call but
    // ignored by the native implementation, which reads arguments 0..4.
    if (!args || (nargs != 5 && nargs != 6) ||
        args[0].kind != QuestJournalCallbackValueV1::Kind::object ||
        args[1].kind != QuestJournalCallbackValueV1::Kind::number ||
        args[2].kind != QuestJournalCallbackValueV1::Kind::number ||
        args[3].kind != QuestJournalCallbackValueV1::Kind::number ||
        args[4].kind != QuestJournalCallbackValueV1::Kind::string ||
        !args[4].text || !args[0].object) return false;
    call = {args[0].object, args[1].number, args[2].number,
            args[3].number, args[4].text};
    return true;
}

struct QuestJournalDetailsCallV1 {
    void* output_object = nullptr;
    double player_index = 0.0;
    double quest_id = 0.0;
};

inline bool decode_quest_journal_details_call_v1(
        const QuestJournalCallbackValueV1* args, std::size_t nargs,
        QuestJournalDetailsCallV1& call) {
    // Authored SWF calls (outputObject, playerIndex, QuestID, isMultiplayer).
    if (!args || (nargs != 3 && nargs != 4) ||
        args[0].kind != QuestJournalCallbackValueV1::Kind::object ||
        args[1].kind != QuestJournalCallbackValueV1::Kind::number ||
        args[2].kind != QuestJournalCallbackValueV1::Kind::number ||
        !args[0].object) return false;
    call = {args[0].object, args[1].number, args[2].number};
    return true;
}

struct QuestJournalSetCurrentCallV1 {
    double player_index = 0.0;
    double quest_id = 0.0;
};

inline bool decode_quest_journal_set_current_call_v1(
        const QuestJournalCallbackValueV1* args, std::size_t nargs,
        QuestJournalSetCurrentCallV1& call) {
    // btn_Activate calls NativeSetCurrentQuest(0, SelectedQuest.ID).
    if (!args || nargs != 2 ||
        args[0].kind != QuestJournalCallbackValueV1::Kind::number ||
        args[1].kind != QuestJournalCallbackValueV1::Kind::number) return false;
    call = {args[0].number, args[1].number};
    return true;
}

template<class Rows>
inline const typename Rows::value_type* find_quest_journal_row_by_id_v1(
        const Rows& rows, std::int32_t quest_id) {
    for (const auto& row : rows) if (row.id == quest_id) return &row;
    return nullptr;
}

// GetQuestFunctor (0x440080) is shared by NativeGetQuestCount and
// NativeGetQuestIDsInRange. Its exact selection order is source-visible:
// DisplayAllQuestInLog selects every non-debug Quest; otherwise
// DisplayAllDebugOnlyQuestInLog selects debug Quests; otherwise only the
// literal predicate "Closed" selects state > 12, with every other string
// selecting states 6..12 (0x43b4f0, 0x43b514). Quest::IsDebug (0x47f850)
// is priority == v2QuestPriority.Debug.
struct QuestJournalPredicateV1 {
    bool display_all_non_debug = false;
    bool display_debug_only = false;
    std::int32_t debug_priority = -1;
};

template<class Row>
inline bool quest_journal_matches_v1(const Row& row, const char* predicate,
                                     const QuestJournalPredicateV1& policy) {
    const bool debug = row.priority == policy.debug_priority;
    if (policy.display_all_non_debug) return !debug;
    if (policy.display_debug_only) return debug;
    if (debug) return false;
    return predicate && std::strcmp(predicate, "Closed") == 0
        ? row.state > 12
        : row.state >= 6 && row.state <= 12;
}

} // namespace dh2::native::ui
