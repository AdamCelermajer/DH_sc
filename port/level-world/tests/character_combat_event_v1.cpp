#include "../character_combat_event_v1.hpp"

#include <cstdio>
#include <cstdlib>

namespace kernel = dh2::character::combat_event_v1;
namespace character = dh2::character;
namespace data = dh2::data;

namespace {
void check(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}
}

int main() {
    character::CombatProperties896 properties{};
    properties.words[32] = -1;
    character::CombatItemInstance4 item{0};
    const character::CombatItemInstance4* item_ref = &item;
    character::CombatEquipSet8 set{&item_ref};
    character::CombatInventory16 inventory{&set, 1, 0};
    character::CombatItemRecord164 rows[1]{};
    kernel::Request request{};
    request.properties = &properties;
    request.inventory = &inventory;
    request.item_rows = rows;
    request.item_count = 1;
    request.event = {5, 3, 7, 0, -1};
    request.authored_name = "attack_mainhand";

    kernel::Result output{};
    check(kernel::route(&request, &output) == kernel::Status::complete,
          "source attack event route rejected an ordinary weapon");
    check(output.can_range == 0 && output.projectile == -1 &&
          output.action.kind == data::CombatEventKind::melee &&
          output.action.sequence_step == 3 && output.action.attack_step == 6 &&
          output.action.offhand == 0,
          "melee event lost the source sequence/clip-step fields");

    rows[0].words[22] = 4;
    rows[0].words[38] = 12;
    rows[0].words[39] = 48;
    rows[0].words[40] = 905;
    check(kernel::route(&request, &output) == kernel::Status::complete,
          "source attack event route rejected a ranged weapon");
    check(output.can_range == 1 && output.range_min == 12 &&
          output.range_max == 48 && output.projectile == 905 &&
          output.action.kind == data::CombatEventKind::projectile &&
          output.action.sequence_step == 905,
          "ranged event did not use source inventory projectile parameters");

    const data::CombatEventContext player_event{5, 1, 2, 0, -1};
    // Preserve source set 1 and ItemTable id 2; a compressed one-row adapter
    // would incorrectly read row 0 and could hide selection bugs.
    std::int32_t player_properties[224]{};
    player_properties[32] = -1;
    std::int32_t player_main_hands[2]{0, 2};
    const bool player_has_main_hands[2]{true, true};
    data::ItemTable player_table{};
    player_table.rows.resize(3);
    player_table.rows[0].record.words[22] = 4;
    player_table.rows[0].record.words[40] = 111;
    player_table.rows[2].record.words[22] = 5;
    player_table.rows[2].record.words[38] = 16;
    player_table.rows[2].record.words[39] = 64;
    player_table.rows[2].record.words[40] = 1201;
    check(kernel::route_snapshot(player_properties, 1, player_main_hands,
          player_has_main_hands, &player_table, &player_event,
          "attack_mainhand", &output) == kernel::Status::complete &&
          output.can_range == 1 && output.range_min == 16 &&
          output.range_max == 64 && output.projectile == 1201 &&
          output.action.kind == data::CombatEventKind::projectile &&
          output.action.sequence_step == 1201,
          "selected V4 set / indexed ItemTable row lost ranged output");

    player_properties[32] = 777;
    check(kernel::route_snapshot(player_properties, 1, nullptr, nullptr,
          nullptr, &player_event, "attack_mainhand", &output) ==
          kernel::Status::complete &&
          output.can_range == 1 && output.projectile == 777 &&
          output.action.kind == data::CombatEventKind::projectile,
          "property-selected projectile did not short-circuit inventory access");

    request.event.state = 4;
    request.properties = nullptr;
    request.inventory = nullptr;
    request.item_rows = nullptr;
    request.item_count = 0;
    check(kernel::route(&request, &output) == kernel::Status::complete &&
          output.action.kind == data::CombatEventKind::none,
          "non-attack state queried range owners or routed an attack");

    request.event.state = 5;
    const auto before = output;
    check(kernel::route(&request, &output) == kernel::Status::range_query_failed &&
          output.action.kind == before.action.kind,
          "missing range providers mutated the committed output");
    std::puts("PASS Character range parameters -> authored attack event routing");
}
