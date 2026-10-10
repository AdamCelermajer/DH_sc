// Preview 16 thin quest runtime tests.
// Usage: quest_runtime_v1_tests <original-cache/data/pydata> [--dump]
// Scenarios use the authored Swamp rows (level row 41) and rows of other acts from the same table.
#include "quest_runtime_v1.hpp"
#include "quest_zones_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::quest_runtime;

namespace {

void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

void dump(const QuestTableV1& table) {
    std::printf("rows=%zu act1=%zu act2=%zu\n", table.rows().size(),
                quest_rows_in_act_v1(table, 1), quest_rows_in_act_v1(table, 2));
    for (std::size_t i = 0; i < table.rows().size(); ++i) {
        const auto& r = table.rows()[i];
        std::printf("%02zu %-26s act=%d state=%d objs=%zu\n", i, r.name.c_str(), r.act, r.state, r.objectives.size());
        for (const auto& p : r.prerequisites)
            std::printf("   prereq op=%d p1=%d p2=%d\n", p.type, p.parameter1, p.parameter2);
        std::printf("   accept type=%d oid1=%d oid2=%d str2='%s'\n", r.accept.type, r.accept.oid1,
                    r.accept.oid2, r.accept.str2.c_str());
        for (const auto& o : r.objectives)
            std::printf("   obj type=%d oid1=%d oid2=%d val=%d str2='%s'\n", o.type, o.oid1, o.oid2, o.value,
                        o.str2.c_str());
    }
}

std::size_t row_index(const QuestTableV1& table, const char* name) {
    for (std::size_t i = 0; i < table.rows().size(); ++i)
        if (table.rows()[i].name == name) return i;
    throw std::runtime_error(std::string("authored row missing: ") + name);
}

struct Harness {
    CharacterState character;
    std::shared_ptr<const QuestTableV1> table;
    std::vector<std::int32_t> xp_awards;
    std::int32_t level_row = 41; // The Boglands (Swamp), IsPlayerInLevel key
    std::unique_ptr<QuestRuntimeV1> runtime;

    explicit Harness(std::shared_ptr<const QuestTableV1> t) : table(std::move(t)) {
        character.id = "quest-test-character";
        character.name = "Tester";
        character.current_difficulty = 0;
        character.gold = 0;
        character.experience = 0;
        open();
    }
    void open() {
        QuestRuntimeServicesV1 services;
        services.give_experience = [this](std::int32_t amount, std::string&) {
            xp_awards.push_back(amount);
            return true;
        };
        services.current_level_row = [this] { return level_row; };
        runtime = std::make_unique<QuestRuntimeV1>(character, table, std::move(services));
        std::string error;
        check(runtime->load(error), error.c_str());
    }
    QuestStateV1 state(std::size_t row) const {
        QuestStateV1 s{};
        check(runtime->state_of(std::int32_t(row), s), "state query");
        return s;
    }
    void kill_property(std::int32_t property) {
        QuestEvent e;
        e.kind = QuestEvent::Kind::kill;
        e.property_id = property;
        std::string error;
        runtime->handle(e, error);
        check(error.empty(), error.c_str());
    }
    void kill_template(std::int32_t templ) {
        QuestEvent e;
        e.kind = QuestEvent::Kind::kill;
        e.template_id = templ;
        std::string error;
        runtime->handle(e, error);
        check(error.empty(), error.c_str());
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
    void talk(std::int32_t npc, std::int32_t second) {
        QuestEvent e;
        e.kind = QuestEvent::Kind::talk_to_npc;
        e.object_id = npc;
        e.secondary_id = second;
        std::string error;
        runtime->handle(e, error);
        check(error.empty(), error.c_str());
    }
};

constexpr int kNoProgress = -1;

void swamp_chain(const std::shared_ptr<const QuestTableV1>& table) {
    const auto& t = *table;
    const auto escape = row_index(t, "Swamp_Escape");
    const auto moths = row_index(t, "Swamp_Moths");
    const auto lizman = row_index(t, "Swamp_KillFiveLizman");
    const auto witch = row_index(t, "Swamp_KillWitch");

    // Escape: IsPlayerInLevel(41) prerequisite, automatic accept, kill property 358 once.
    Harness h(table);
    check(h.state(escape) == QuestStateV1::active, "Escape becomes Active in level 41 with an automatic accept");
    check(h.state(moths) == QuestStateV1::available, "Moths waits for its TalkToNPC accept (Escape > 3 holds)");
    check(h.state(lizman) == QuestStateV1::available, "Lizman waits for its TalkToNPC accept");
    check(h.state(witch) == QuestStateV1::available, "Witch waits for its zone accept");
    check(h.runtime->current_quest() == std::int32_t(escape), "Escape becomes the current quest");

    // Accept is entry-point only: a row that is not Available is rejected.
    std::string error;
    check(!h.runtime->accept_quest(std::int32_t(escape), error) && !error.empty(), "accept rejects non-Available");

    // Moths: TalkToNPC accept (oid 365/41) then 8 template-94 kills; 7 kills stay Active.
    check(h.runtime->accept_quest(std::int32_t(moths), error), error.c_str());
    check(h.state(moths) == QuestStateV1::active, "accepted Moths becomes Active");
    const auto banners = h.runtime->take_banners();
    bool moths_new = false;
    for (const auto& b : banners) moths_new = moths_new || (b.kind == QuestBannerV1::Kind::new_quest && b.row == std::int32_t(moths));
    check(moths_new, "NEW QUEST banner for Moths");
    for (int i = 0; i < 7; ++i) h.kill_template(94);
    check(h.state(moths) == QuestStateV1::active, "7 Moth kills keep Moths Active");
    check(h.runtime->progress_of(std::int32_t(moths))->objectives[0].quantity == 7, "7 kills recorded");
    h.kill_template(93); // Lizman's template does not advance Moths
    check(h.runtime->progress_of(std::int32_t(moths))->objectives[0].quantity == 7, "other template ignored");
    h.kill_template(94);
    check(h.state(moths) == QuestStateV1::post_closed, "8th Moth kill completes and closes Moths");
    check(h.character.gold == 150, "Moths reward gold 150 once");
    const auto after = h.runtime->take_banners();
    check(!after.empty() && after.front().kind == QuestBannerV1::Kind::completed &&
          after.front().reward_xp == 20 && after.front().reward_gold == 150, "QUEST COMPLETED banner with rewards");
    check(h.xp_awards.size() == 1 && h.xp_awards[0] == 20, "Moths reward XP 20 once");
    h.kill_template(94); // duplicate after completion is ignored
    check(h.character.gold == 150 && h.xp_awards.size() == 1, "duplicate kill grants nothing");

    // Persistence: reload keeps the closed state, the counters and the rewards (no double).
    std::string load_error;
    check(h.runtime->save(load_error), load_error.c_str());
    h.open();
    check(h.state(moths) == QuestStateV1::post_closed, "reload keeps Moths closed");
    check(h.character.gold == 150 && h.xp_awards.size() == 1, "reload does not repay rewards");

    // Lizman partial progress survives a save/reload and completes after 2 more kills.
    check(h.runtime->accept_quest(std::int32_t(lizman), error), error.c_str());
    for (int i = 0; i < 3; ++i) h.kill_template(93);
    check(h.runtime->save(load_error), load_error.c_str());
    h.open();
    check(h.runtime->progress_of(std::int32_t(lizman))->objectives[0].quantity == 3, "Lizman counter reloads as 3");
    check(h.state(lizman) == QuestStateV1::active, "Lizman stays Active after reload");
    h.kill_template(93);
    h.kill_template(93);
    check(h.state(lizman) == QuestStateV1::post_closed, "5 Lizman kills complete Lizman");
    check(h.character.gold == 150 + 200, "Lizman gold 200 once");
    check(h.xp_awards.size() == 2 && h.xp_awards[1] == 20, "Lizman XP 20 once (after Moths XP 20)");
    h.kill_template(93);
    check(h.xp_awards.size() == 2, "no further Lizman reward");

    // Witch: zone accept requires the trigger name AND the level row (oid1 = 41).
    h.zone("_prim_WitchQuestStart", 4);
    check(h.state(witch) == QuestStateV1::available, "wrong level does not accept the Witch");
    h.zone("_prim_NotTheWitch", 41);
    check(h.state(witch) == QuestStateV1::available, "wrong zone does not accept the Witch");
    h.zone("_prim_WitchQuestStart", 41);
    check(h.state(witch) == QuestStateV1::active, "zone enter accepts and activates the Witch quest");
    h.kill_property(435);
    check(h.state(witch) == QuestStateV1::post_closed, "Bogwitch property kill completes the Witch quest");
    check(h.character.gold == 150 + 200 + 50, "Witch gold 50");
    check(h.xp_awards.size() == 3 && h.xp_awards[2] == 50, "Witch XP 50 once");
}

void make_active_and_log(const std::shared_ptr<const QuestTableV1>& table) {
    const auto escape = row_index(*table, "Swamp_Escape");
    const auto lizman = row_index(*table, "Swamp_KillFiveLizman");
    Harness h(table);
    std::string error;
    // Escape is Assigned and Active; Make Active sets the current quest.
    check(h.runtime->make_active(std::int32_t(escape), error), error.c_str());
    check(h.runtime->current_quest() == std::int32_t(escape), "Make Active sets currentquest");
    // Lizman is Available, not Assigned: Make Active is refused.
    check(!h.runtime->make_active(std::int32_t(lizman), error) && !error.empty(), "Available row cannot be made active");
    // Escape kill completes it; its completed state leaves the Assigned range and clears the current quest.
    h.kill_property(358);
    check(h.state(escape) == QuestStateV1::post_closed, "Escape completes with its property kill");
    check(h.runtime->current_quest() == -1, "PostActive clears currentquest");
    check(!h.runtime->make_active(std::int32_t(escape), error), "Completed quest cannot be made active");
    check(h.xp_awards.size() == 1 && h.xp_awards[0] == 100, "Escape reward XP 100 once");
}

void other_act_rows(const std::shared_ptr<const QuestTableV1>& table) {
    // Rows of other acts load and run through the same runtime (no act-specific code).
    check(quest_rows_in_act_v1(*table, 2) >= 1, "act 2 rows exist in the authored table");
    bool has_high_act = false;
    for (const auto& r : table->rows()) has_high_act = has_high_act || r.act >= 8;
    check(has_high_act, "rows of later acts exist in the authored table");
    Harness h(table);
    std::string error;
    // The rows of other acts start in their authored states and keep their own rewards.
    for (std::size_t i = 0; i < table->rows().size(); ++i) {
        QuestStateV1 s{};
        check(h.runtime->state_of(std::int32_t(i), s), "every row has a state");
        check(std::int32_t(s) >= 0 && std::int32_t(s) < 14, "state inside the original range");
    }
    // A later-act quest with a TalkToNPC accept takes the same accept entry point.
    const auto abbey = row_index(*table, "Abbey_Rescue");
    if (h.state(abbey) == QuestStateV1::available) check(h.runtime->accept_quest(std::int32_t(abbey), error), error.c_str());
}

void table_rejects_truncation(const std::vector<std::uint8_t>& array, const std::vector<std::uint8_t>& names) {
    std::shared_ptr<const QuestTableV1> out;
    std::string error;
    std::vector<std::uint8_t> cut(array.begin(), array.begin() + std::ptrdiff_t(array.size() / 2));
    check(!decode_quest_table_v1(cut, names, out, error) && !out, "truncated v2quests PyArray rejects atomically");
    check(!decode_quest_table_v1(array, {}, out, error) && !out, "missing names table rejects");
}

void event_bus(const std::shared_ptr<const QuestTableV1>& table) {
    QuestEvent e;
    e.kind = QuestEvent::Kind::kill;
    e.template_id = 94;
    check(!raise_quest_event(e), "raise without a bound sink is dropped and reported");
    Harness h(table);
    int delivered = 0;
    auto previous = bind_quest_event_sink([&](const QuestEvent& event) {
        ++delivered;
        std::string error;
        h.runtime->handle(event, error);
    });
    check(!previous, "no previous sink");
    check(raise_quest_event(e) && delivered == 1, "bound sink receives the event");
    bind_quest_event_sink({});
    check(!raise_quest_event(e) && delivered == 1, "unbound sink drops events");
}

// P16 QUESTUI: quest trigger zones. Names come from MoveInZone objectives; boxes from the
// level declarations (any level); rising edge only; rotated and missing zones are reported.
void zone_builder(const std::shared_ptr<const QuestTableV1>& table) {
    const auto names = quest_zone_names_v1(*table);
    check(names.size() >= 1, "quest table authors at least one MoveInZone zone");
    check(std::find(names.begin(), names.end(), std::string("_prim_WitchQuestStart")) != names.end(),
          "Witch accept zone name is collected from the table");
    const auto witch = make_quest_zone_declaration_v1("_prim_WitchQuestStart",
        {{"type", "Block"}, {"scale", "7.30955,5.70483,2.02567"}, {"rotation", "0.0,0.0,0.0"}, {"gametype", "QuestMoveInZone"}},
        {178.335f, 246.863f, 255.074f});
    const auto rotated = make_quest_zone_declaration_v1("_prim_WitchQuestStart",
        {{"type", "Block"}, {"scale", "1,1,1"}, {"rotation", "0,0,90"}}, {0, 0, 0});
    const auto other = make_quest_zone_declaration_v1("_prim_Unrelated",
        {{"type", "Block"}, {"scale", "1,1,1"}}, {0, 0, 0});
    QuestZoneSetV1 zones;
    zones.build(*table, {other, witch});
    check(zones.zones().size() == 1 && zones.zones()[0].name == "_prim_WitchQuestStart", "one Witch box is built");
    check(zones.notes().empty(), "a plain Block builds without notes");
    check(std::fabs(zones.zones()[0].min[0] - (178.335f - 730.955f)) < 0.01f, "box min is position - 100 * |scale|");
    check(std::fabs(zones.zones()[0].max[0] - zones.zones()[0].min[0] - 2.0f * 730.955f) < 0.01f, "box width is 2 * 100 * scale_x");
    // Rising edge: a start inside does not fire, leaving and re-entering does.
    const std::array<float, 3> inside{178.0f, 246.0f, 255.0f}, outside{-5000.0f, 0.0f, 255.0f};
    check(zones.update(inside).empty(), "first frame inside only primes the state");
    check(zones.update(outside).empty(), "leaving a zone fires nothing");
    check(zones.update(inside).size() == 1, "re-entering fires once");
    check(zones.update(inside).empty(), "staying inside does not repeat");
    QuestZoneSetV1 rotated_set;
    rotated_set.build(*table, {rotated});
    check(rotated_set.zones().empty() && !rotated_set.notes().empty(), "rotated zones are reported, not built");
    std::printf("quest_runtime_v1: zone builder ok (names=%zu)\n", names.size());
}

} // namespace

int main(int argc, char** argv) {
    try {
        check(argc >= 2, "usage: quest_runtime_v1_tests <original-cache/data/pydata> [--dump]");
        const std::string root = argv[1];
        std::shared_ptr<const QuestTableV1> table;
        std::string error;
        check(load_quest_table_v1(root + "/v2quests_pyarray.bin", root + "/v2quests_pyarraynames.bin", table, error),
              error.c_str());
        if (argc >= 3 && std::strcmp(argv[2], "--dump") == 0) {
            dump(*table);
            return 0;
        }
        std::vector<std::uint8_t> array, names;
        {
            std::FILE* a = std::fopen((root + "/v2quests_pyarray.bin").c_str(), "rb");
            std::FILE* n = std::fopen((root + "/v2quests_pyarraynames.bin").c_str(), "rb");
            check(a && n, "quest table files reopen");
            int c;
            while ((c = std::fgetc(a)) != EOF) array.push_back(std::uint8_t(c));
            while ((c = std::fgetc(n)) != EOF) names.push_back(std::uint8_t(c));
            std::fclose(a);
            std::fclose(n);
        }
        check(table->rows().size() == 64, "authored v2Quest table has 64 rows");
        table_rejects_truncation(array, names);
        swamp_chain(table);
        make_active_and_log(table);
        other_act_rows(table);
        event_bus(table);
        zone_builder(table);
        std::printf("quest_runtime_v1: all tests passed (rows=%zu)\n", table->rows().size());
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "quest_runtime_v1 test failed: " << e.what() << '\n';
        return 1;
    }
}
