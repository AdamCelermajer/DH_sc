// OPENING2: named activation conditions and quest script slots (unit tests on synthetic v2conditions bytes).
#include "quest_conditions_v1.hpp"
#include "quest_runtime_v1.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation::quest_runtime;

namespace {

void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void put_u32(std::vector<std::uint8_t>& b, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b.push_back(std::uint8_t(v >> (8 * i)));
}

// Two named records: "GameStartOnly" = quest 1 in state 0 (type 0), "InLevel" = level row 41 (type 3).
void synthetic_tables(std::vector<std::uint8_t>& array, std::vector<std::uint8_t>& names) {
    put_u32(array, 2); put_u32(array, 2); put_u32(array, 0); put_u32(array, 16);
    put_u32(array, 0); put_u32(array, 0); put_u32(array, 1); put_u32(array, 0);   // GameStartOnly: w0, type 0, p1 1, p2 0
    put_u32(array, 0); put_u32(array, 3); put_u32(array, 41); put_u32(array, 0);  // InLevel: type 3, p1 41
    put_u32(names, 2);
    const std::string a = "GameStartOnly", b = "InLevel";
    put_u32(names, std::uint32_t(a.size())); names.insert(names.end(), a.begin(), a.end());
    put_u32(names, std::uint32_t(b.size())); names.insert(names.end(), b.begin(), b.end());
}

void decode_and_evaluate() {
    std::vector<std::uint8_t> array, names;
    synthetic_tables(array, names);
    std::map<std::string, NamedConditionV1> table;
    std::string error;
    check(decode_named_conditions_v1(array, names, table, error), "synthetic v2conditions decode: " + error);
    check(table.size() == 2 && table.at("GameStartOnly").type == 0 && table.at("GameStartOnly").parameter1 == 1,
          "GameStartOnly decodes as quest 1 in-state");
    // New game: quest 1 has state 0 and the player is in level 41.
    auto fresh = [](std::int32_t row, std::int32_t& state) { state = 0; return row == 1; };
    bool met = false, unsupported = false;
    check(evaluate_named_condition_v1(table.at("GameStartOnly"), fresh, -1, met, unsupported) && met && !unsupported,
          "GameStartOnly is met for a new game (quest 1 state 0)");
    // Continued save: quest 1 has moved on.
    auto moved = [](std::int32_t row, std::int32_t& state) { state = row == 1 ? 6 : 0; return true; };
    check(evaluate_named_condition_v1(table.at("GameStartOnly"), moved, -1, met, unsupported) && !met,
          "GameStartOnly is not met after the quest moved on");
    // A quest row with no save record is false.
    auto missing = [](std::int32_t, std::int32_t&) { return false; };
    check(evaluate_named_condition_v1(table.at("GameStartOnly"), missing, -1, met, unsupported) && !met,
          "no save record evaluates false");
    check(evaluate_named_condition_v1(table.at("InLevel"), fresh, 41, met, unsupported) && met, "IsPlayerInLevel by level row");
    check(evaluate_named_condition_v1(table.at("InLevel"), fresh, 7, met, unsupported) && !met, "IsPlayerInLevel false elsewhere");
    NamedConditionV1 event{6, 1, 2};
    check(!evaluate_named_condition_v1(event, fresh, 0, met, unsupported) && unsupported, "event conditions are reported unsupported");
    // Malformed input is rejected atomically.
    std::vector<std::uint8_t> bad(array.begin(), array.begin() + 20);
    std::map<std::string, NamedConditionV1> none;
    check(!decode_named_conditions_v1(bad, names, none, error) && none.empty(), "truncated records reject");
}

void script_slots() {
    using S = QuestStateV1;
    // Quest::SetState: post_locked runs slot 9; available runs slot 1 from a non-active state and slot 4 from active.
    check(quest_script_slot_v1(S::post_locked, S::locked) == 9, "post_locked slot");
    check(quest_script_slot_v1(S::available, S::pre_available) == 1, "available from pre_available uses slot 1 (opening)");
    check(quest_script_slot_v1(S::available, S::active) == 4, "available from active uses slot 4");
    check(quest_script_slot_v1(S::locked, S::post_locked) == -1, "locked runs no script");
    check(quest_script_slot_v1(S::completed, S::active) == 3, "completed slot");
}

}  // namespace

int main() {
    try {
        decode_and_evaluate();
        script_slots();
        std::cout << "quest_conditions_v1 tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "quest_conditions_v1 test failed: " << e.what() << '\n';
        return 1;
    }
}
