// Preview 17 authored condition evaluator and event-driven watch tests.
// Usage: conditions_v1_tests <original-cache/data/pydata>   (needs v2conditions_* and v2quests_*)
#include "authored_conditions_v1.hpp"
#include "condition_watch_v1.hpp"
#include "../quest_runtime/quest_runtime_v1.hpp"
#include "../quest_runtime/quest_table_v1.hpp"
#include "../quest_runtime/quest_events_v1.hpp"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::conditions;
using namespace dh::foundation::quest_runtime;

namespace {

void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read " + path);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

// Fixed-state world: quest rows map to authored states; a missing row has no state.
ConditionWorldV1 fixed_world(std::int32_t witch_state, std::int32_t escape_state, bool witch_present = true) {
    ConditionWorldV1 world;
    world.quest_state = [=](std::int32_t quest, std::int32_t& state) {
        if (quest == 52) { if (!witch_present) return false; state = witch_state; return true; }
        if (quest == 50) { state = escape_state; return true; }
        return false;
    };
    world.current_level_row = [] { return 41; };
    return world;
}

void table_checks(const AuthoredConditionsV1& table) {
    check(table.size() == 118, "v2Conditions decodes 118 authored rows");
    check(table.contains("IsAfter_Swamp_KillWitch2"), "IsAfter_Swamp_KillWitch2 is an authored row");
    check(table.contains("IsBefore_Swamp_KillWitch"), "IsBefore_Swamp_KillWitch is an authored row");
    check(!table.contains("NotAuthoredCondition"), "unknown names are not rows");
    std::vector<std::int32_t> quests, events;
    check(table.dependencies("IsAfter_Swamp_KillWitch2", quests, events), "dependencies of an authored name");
    check(quests.size() == 1 && quests[0] == 52 && events.empty(), "IsAfter_Swamp_KillWitch2 reads quest 52 only");
    check(table.dependencies("IsBetween_witch_and_swampking", quests, events) && quests.size() == 2 &&
          quests[0] == 52 && quests[1] == 50, "IsBetween_witch_and_swampking reads quests 52 and 50");
}

void evaluator_checks(const AuthoredConditionsV1& table) {
    bool value = false;
    std::string error;
    // Witch quest (row 52) PostClosed = 13 is the authored "after" state.
    check(table.evaluate("IsAfter_Swamp_KillWitch2", fixed_world(13, 0), value, error) && value,
          "IsAfter_Swamp_KillWitch2 is true at PostClosed");
    check(table.evaluate("IsAfter_Swamp_KillWitch2", fixed_world(6, 0), value, error) && !value,
          "IsAfter_Swamp_KillWitch2 is false while the Witch quest is Active");
    check(table.evaluate("IsBefore_Swamp_KillWitch", fixed_world(6, 0), value, error) && value,
          "IsBefore_Swamp_KillWitch is true while Active (13 > 6)");
    check(table.evaluate("IsBefore_Swamp_KillWitch", fixed_world(13, 0), value, error) && !value,
          "IsBefore_Swamp_KillWitch is false at PostClosed");
    // A missing quest is false for every comparison (SG_GetQuestByID NULL -> return 0).
    check(table.evaluate("IsAfter_Swamp_KillWitch2", fixed_world(0, 0, false), value, error) && !value,
          "missing quest makes IsAfter false");
    check(table.evaluate("IsBefore_Swamp_KillWitch", fixed_world(0, 0, false), value, error) && !value,
          "missing quest makes IsBefore false");
    // Conjunction (ConditionList::Eval): both stubs must hold.
    check(table.evaluate("IsBetween_witch_and_swampking", fixed_world(13, 4), value, error) && value,
          "IsBetween holds when the Witch is closed and Escape is below 13");
    check(table.evaluate("IsBetween_witch_and_swampking", fixed_world(13, 13), value, error) && !value,
          "IsBetween fails when Escape reached 13");
    check(table.evaluate("IsBetween_witch_and_swampking", fixed_world(6, 4), value, error) && !value,
          "IsBetween fails while the Witch is not closed");
    // No condition.
    check(table.evaluate("Invalid", fixed_world(0, 0), value, error) && value, "Invalid is no condition (true)");
    check(table.evaluate("", fixed_world(0, 0), value, error) && value, "empty name is no condition (true)");
    // Unknown names and unbound services fail with an error (never a silent false).
    check(!table.evaluate("NotAuthoredCondition", fixed_world(0, 0), value, error) && !error.empty(),
          "unknown name fails with an error");
    ConditionWorldV1 unbound;
    check(!table.evaluate("IsAfter_Swamp_KillWitch2", unbound, value, error) && !error.empty(),
          "unbound quest service fails with an error");
}

struct Harness {
    CharacterState character;
    std::shared_ptr<const QuestTableV1> quests;
    std::unique_ptr<QuestRuntimeV1> runtime;
    std::shared_ptr<const AuthoredConditionsV1> conditions;
    std::unique_ptr<ConditionWatchV1> watch;

    Harness(std::shared_ptr<const QuestTableV1> q, std::shared_ptr<const AuthoredConditionsV1> c)
        : quests(std::move(q)), conditions(std::move(c)) {
        character.id = "conditions-test-character";
        character.name = "Tester";
        open();
    }
    // A fresh runtime over the same character (reload). The watch is primed from its state.
    void open() {
        QuestRuntimeServicesV1 services;
        services.current_level_row = [] { return 41; };
        services.state_changed = [this](std::int32_t row, std::int32_t) {
            if (watch) watch->quest_changed(row);
        };
        runtime = std::make_unique<QuestRuntimeV1>(character, quests, std::move(services));
        std::string error;
        check(runtime->load(error), error.c_str());
        watch = std::make_unique<ConditionWatchV1>(conditions);
        check(watch->watch("IsAfter_Swamp_KillWitch2"), "watch IsAfter_Swamp_KillWitch2");
        check(watch->watch("IsBefore_Swamp_KillWitch"), "watch IsBefore_Swamp_KillWitch");
        check(!watch->watch("Invalid") && !watch->watch("NotAuthoredCondition"), "unwatchable names are refused");
        const auto world = world_from_quest_runtime(*runtime, [] { return 41; });
        check(watch->prime(world, error), error.c_str());
    }
    void zone(const std::string& name, std::int32_t level) {
        QuestEvent e;
        e.kind = QuestEvent::Kind::zone_enter;
        e.zone = name;
        e.level_row = level;
        std::string error;
        runtime->handle(e, error);
        check(error.empty(), error.c_str());
    }
    void kill_property(std::int32_t property) {
        QuestEvent e;
        e.kind = QuestEvent::Kind::kill;
        e.property_id = property;
        std::string error;
        runtime->handle(e, error);
        check(error.empty(), error.c_str());
    }
    std::vector<ConditionFlipV1> flush() {
        std::string error;
        const auto world = world_from_quest_runtime(*runtime, [] { return 41; });
        auto flips = watch->flush(world, error);
        check(error.empty(), error.c_str());
        return flips;
    }
};

void watch_checks(const std::shared_ptr<const QuestTableV1>& quests,
                  const std::shared_ptr<const AuthoredConditionsV1>& conditions) {
    // Fresh character: the Witch is Available, so IsAfter is false and IsBefore is true.
    Harness h(quests, conditions);
    bool value = false;
    check(h.watch->current("IsAfter_Swamp_KillWitch2", value) && !value, "fresh: IsAfter is false");
    check(h.watch->current("IsBefore_Swamp_KillWitch", value) && value, "fresh: IsBefore is true");
    check(h.watch->dirty_count() == 0 && h.flush().empty(), "no quest event means no evaluation or flip");

    // Zone accept activates the Witch quest: the watched names are dirty, but nothing flips.
    h.zone("_prim_WitchQuestStart", 41);
    check(h.watch->dirty_count() == 2, "quest state events mark exactly the dependent names dirty");
    check(h.flush().empty(), "Active is not a transition for either name");
    check(h.watch->dirty_count() == 0, "flush clears the dirty set");

    // Kill 435 completes the Witch quest, which settles to PostClosed (13) in the same event.
    h.kill_property(435);
    const auto flips = h.flush();
    check(flips.size() == 2, "closing the Witch quest flips exactly two names");
    for (const auto& flip : flips) {
        if (flip.name == "IsAfter_Swamp_KillWitch2") check(!flip.before && flip.after, "IsAfter flips false->true");
        else if (flip.name == "IsBefore_Swamp_KillWitch") check(flip.before && !flip.after, "IsBefore flips true->false");
        else throw std::runtime_error("unexpected flip " + flip.name);
    }
    check(h.watch->current("IsAfter_Swamp_KillWitch2", value) && value, "IsAfter holds after the flip");
    h.kill_property(435); // duplicate kill: no event changes the quest, so no re-evaluation
    check(h.flush().empty(), "a repeated kill neither flips nor re-evaluates");

    // Save, reload from the CQPG record, and rebind: the restored state is a baseline, not a flip.
    std::string error;
    check(h.runtime->save(error), error.c_str());
    h.open();
    check(h.watch->current("IsAfter_Swamp_KillWitch2", value) && value, "reload keeps IsAfter true");
    check(h.flush().empty(), "reload does not replay the flip");
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc < 2) throw std::runtime_error("usage: conditions_v1_tests <original-cache/data/pydata>");
        const std::string dir = argv[1];
        std::shared_ptr<const AuthoredConditionsV1> conditions;
        std::string error;
        check(AuthoredConditionsV1::decode(read_file(dir + "/v2conditions_pyarray.bin"),
                                           read_file(dir + "/v2conditions_pyarraynames.bin"),
                                           read_file(dir + "/v2conditions_pystructnames.bin"),
                                           conditions, error), error.c_str());
        std::shared_ptr<const AuthoredConditionsV1> rejected;
        check(!AuthoredConditionsV1::decode({}, {}, {}, rejected, error) && !error.empty() && !rejected,
              "empty condition input is rejected");
        std::vector<std::uint8_t> truncated = read_file(dir + "/v2conditions_pyarray.bin");
        truncated.resize(truncated.size() / 2);
        check(!AuthoredConditionsV1::decode(truncated, read_file(dir + "/v2conditions_pyarraynames.bin"),
                                            read_file(dir + "/v2conditions_pystructnames.bin"), rejected, error) &&
              !rejected, "truncated condition records are rejected atomically");
        table_checks(*conditions);
        evaluator_checks(*conditions);

        std::shared_ptr<const QuestTableV1> quests;
        check(decode_quest_table_v1(read_file(dir + "/v2quests_pyarray.bin"),
                                    read_file(dir + "/v2quests_pyarraynames.bin"), quests, error),
              error.c_str());
        watch_checks(quests, conditions);
        std::printf("conditions_v1: all tests passed (rows=%zu)\n", conditions->size());
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "conditions_v1 test failed: %s\n", e.what());
        return 1;
    }
}
