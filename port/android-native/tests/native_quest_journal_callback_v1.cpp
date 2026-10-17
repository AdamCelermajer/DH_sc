#include "../app/src/main/cpp/native_quest_journal_callback_v1.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <vector>

using dh2::native::ui::QuestJournalCallbackValueV1;

int main() {
    int output_array = 0, output_object = 0;
    constexpr auto object = QuestJournalCallbackValueV1::Kind::object;
    constexpr auto number = QuestJournalCallbackValueV1::Kind::number;
    constexpr auto string = QuestJournalCallbackValueV1::Kind::string;
    std::array<QuestJournalCallbackValueV1, 6> list_args{{
        {object, 0.0, nullptr, &output_array},
        {number, 0.0, nullptr, nullptr},
        {number, 10.0, nullptr, nullptr},
        {number, 25.0, nullptr, nullptr},
        {string, 0.0, "Active", nullptr},
        {number, 0.0, nullptr, nullptr},
    }};
    dh2::native::ui::QuestJournalListCallV1 list{};
    assert(dh2::native::ui::decode_quest_journal_list_call_v1(
        list_args.data(), list_args.size(), list));
    assert(list.output_array == &output_array && list.player_index == 0.0 &&
           list.start == 10.0 && list.count == 25.0 &&
           list.predicate[0] == 'A');
    // The prior adapter's assumed [player, filter, start, count, output]
    // order must not be accepted as the authored callback ABI.
    std::array<QuestJournalCallbackValueV1, 5> reversed{{
        {number, 0.0, nullptr, nullptr}, {string, 0.0, "Active", nullptr},
        {number, 0.0, nullptr, nullptr}, {number, 25.0, nullptr, nullptr},
        {object, 0.0, nullptr, &output_array},
    }};
    assert(!dh2::native::ui::decode_quest_journal_list_call_v1(
        reversed.data(), reversed.size(), list));

    std::array<QuestJournalCallbackValueV1, 4> detail_args{{
        {object, 0.0, nullptr, &output_object},
        {number, 0.0, nullptr, nullptr},
        {number, 9007.0, nullptr, nullptr},
        {number, 0.0, nullptr, nullptr},
    }};
    dh2::native::ui::QuestJournalDetailsCallV1 details{};
    assert(dh2::native::ui::decode_quest_journal_details_call_v1(
        detail_args.data(), detail_args.size(), details));
    assert(details.output_object == &output_object && details.player_index == 0.0 &&
           details.quest_id == 9007.0);

    std::array<QuestJournalCallbackValueV1, 2> activate_args{{
        {number, 0.0, nullptr, nullptr},
        {number, 9007.0, nullptr, nullptr},
    }};
    dh2::native::ui::QuestJournalSetCurrentCallV1 activate{};
    assert(dh2::native::ui::decode_quest_journal_set_current_call_v1(
        activate_args.data(), activate_args.size(), activate));
    assert(activate.player_index == 0.0 && activate.quest_id == 9007.0);
    assert(!dh2::native::ui::decode_quest_journal_set_current_call_v1(
        activate_args.data(), 1, activate));

    struct Row { std::int32_t id; };
    const std::vector<Row> rows{{9007}, {9012}};
    assert(dh2::native::ui::find_quest_journal_row_by_id_v1(rows, 9012)->id == 9012);
    assert(dh2::native::ui::find_quest_journal_row_by_id_v1(rows, 1) == nullptr);

    // These rows exercise the predicates selected by the shipped
    // dqcharmenu.swf journal calls and the original GetQuestFunctor switch.
    struct QuestRow { std::int32_t id, state, priority; };
    const std::vector<QuestRow> journal_rows{
        {1, 5, 1}, {2, 6, 1}, {3, 12, 1}, {4, 13, 1},
        {5, 6, 9}, {6, 13, 9},
    };
    dh2::native::ui::QuestJournalPredicateV1 policy{false, false, 9};
    auto count_matching = [&](const char* predicate) {
        std::size_t count = 0;
        for (const auto& row : journal_rows)
            count += dh2::native::ui::quest_journal_matches_v1(
                row, predicate, policy);
        return count;
    };
    assert(count_matching("Active") == 2);
    assert(count_matching("Closed") == 1);
    assert(count_matching("UnexpectedPredicate") == 2);
    policy.display_all_non_debug = true;
    assert(count_matching("Closed") == 4);
    policy.display_debug_only = true;
    assert(count_matching("Active") == 4); // first source switch wins
    policy.display_all_non_debug = false;
    assert(count_matching("Active") == 2);
    assert(count_matching("Closed") == 2);
}
