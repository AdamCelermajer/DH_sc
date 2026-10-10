// P14 DROPS focused test: scatter, 5-slot pool, travel, sensor target, the
// ItemObject::Interact gates (owner window, inventory full, potion capacity,
// potion stacking, gold) and drop_item_to_world, over the ACTUAL original
// ItemTable/Loot snapshot. argv[1] = shared asset root.
#include "world_drop_rules_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../../game-data/loot_tables_v2.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::loot;
using namespace dh2::data;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
Bytes view(const std::vector<std::uint8_t>& b) { return {b.data(), b.size()}; }

struct Env {
    LootTablesV2 owner;
    LootTablesV2::Borrow tables;
    const LootEntry32V2* entry{};
    std::int32_t loot_table{-1};
    std::int32_t id_of(const std::string& identifier) const {
        const auto& ids = tables.items().identifiers;
        const auto found = std::find(ids.begin(), ids.end(), identifier);
        check(found != ids.end(), "missing actual ItemTable identifier " + identifier);
        return static_cast<std::int32_t>(found - ids.begin());
    }
    RuntimeWorldItemRecordV1 record(const std::string& identifier, std::uint8_t quantity,
                                    ActorId source = 5, std::optional<std::int32_t> gold = {}) const {
        RuntimeWorldItemRecordV1 r;
        const auto id = id_of(identifier);
        r.source_actor = source;
        r.killer_actor = 1;
        r.loot_table = loot_table;
        r.item_id = static_cast<std::int16_t>(id);
        r.quantity = quantity;
        r.authored_item = &tables.items().rows[std::size_t(id)];
        r.authored_entry = entry;
        r.resolved_gold_value = gold;
        return r;
    }
};

struct Ctx { ActorId id; std::shared_ptr<CharacterState> state; };
bool resolve(void* raw, ActorId id, std::shared_ptr<CharacterState>& out, std::string& e) {
    auto* c = static_cast<Ctx*>(raw);
    if (!c || c->id != id) { e = "wrong player"; return false; }
    out = c->state;
    return true;
}

ActorState actor(ActorId id, std::array<float, 3> p) {
    ActorState a;
    a.id = id;
    a.transform.position = p;
    return a;
}
}

int main(int argc, char** argv) try {
    check(argc == 2, "supply the shared asset root");
    AssetCatalog assets(argv[1]);
    const auto read = [&](const char* name) {
        return assets.read(std::filesystem::path("original-cache/data/pydata") / name);
    };
    auto a = read("loot_table_pyarray.bin"), b = read("loot_table_pyarraynames.bin"),
         c = read("loot_table_pystructnames.bin");
    Env env;
    std::string error;
    check(env.owner.load(view(a), view(b), view(c), error), error);
    env.tables = env.owner.borrow();
    for (std::size_t i = 0; i < env.tables.loots().size() && !env.entry; ++i) {
        const auto& loot = env.tables.loots()[i];
        if (!loot.random_entries.empty()) { env.entry = &loot.random_entries.front(); env.loot_table = int(i); }
        else if (!loot.fixed_entries.empty()) { env.entry = &loot.fixed_entries.front(); env.loot_table = int(i); }
    }
    check(env.entry, "no Loot entry available");

    // ---- scatter: draw order and ranges (original _GetRandomDropPos).
    {
        LootRandom8V2 rng{1234, 0};
        std::array<float, 3> out{};
        const std::array<float, 3> victim{1000, 2000, 50}, killer{1000, 1000, 50};
        for (int i = 0; i < 64; ++i) {
            check(scatter_destination_v1(rng, victim, &killer, out, error), error);
            // Direction victim->killer is -Y; lateral axis is dir x K = (-1, 0, 0).
            const float along = victim[1] - out[1];
            const float across = victim[0] - out[0];
            check(along >= 150.0f - 0.01f && along < 350.0f, "scatter along-distance outside 150..349");
            check(across > -150.0f - 0.01f && across <= 150.0f, "scatter lateral outside -150..149");
            check(out[2] == 50.0f, "scatter changed Z");
        }
        check(rng.calls == 128, "killer scatter must consume exactly two draws per item");
        LootRandom8V2 rng2{1234, 0};
        for (int i = 0; i < 64; ++i) {
            check(scatter_destination_v1(rng2, victim, nullptr, out, error), error);
            check(std::fabs(out[0] - victim[0]) <= 250.0f && std::fabs(out[1] - victim[1]) <= 250.0f &&
                  out[2] == victim[2], "no-killer scatter outside +-250");
        }
        check(rng2.calls == 128, "no-killer scatter must consume two draws per item");
        LootRandom8V2 rng3{1234, 0}, rng4{1234, 0};
        std::array<float, 3> o1{}, o2{};
        check(scatter_destination_v1(rng3, victim, &killer, o1, error) &&
              scatter_destination_v1(rng4, victim, &killer, o2, error) && o1 == o2,
              "scatter must be deterministic for a seed");
        const std::array<float, 3> same{1, 2, 3};
        check(!scatter_destination_v1(rng3, same, &same, o1, error),
              "coincident killer must be rejected (NaN direction)");
    }

    // ---- publish + travel + pool recycling.
    RuntimeWorldItemAdapterV1 store(env.tables);
    const auto victim = actor(5, {0, 0, 10});
    LootRandom8V2 rng{99, 0};
    const std::array<float, 3> killer{500, 0, 10};
    RuntimeWorldItemIdV1 first{};
    check(store.publish_death_drop_scattered(env.record("Potion0", 1), victim, &killer, &rng, first, error), error);
    RuntimeWorldItemEntryV1 entry;
    check(store.inspect(first, entry, error), error);
    check(entry.source_position == victim.transform.position && entry.destination != entry.source_position,
          "a scattered drop starts at the victim and has a distinct landing point");
    check(entry.destination[0] >= 150.0f, "scatter toward the killer (+X) lands at least 150 units away");
    const auto landing = entry.destination;
    store.advance(16);
    check(store.inspect(first, entry, error) && entry.source_position != victim.transform.position &&
          entry.age_ms == 16, "advance moves the item toward its landing point");
    for (int i = 0; i < 200; ++i) store.advance(16);
    // ItemObject::Update stops the item inside the 80-unit arrival radius (XY); Z keeps its spawn height.
    check(store.inspect(first, entry, error), error);
    const float rest_dx = entry.source_position[0] - landing[0], rest_dy = entry.source_position[1] - landing[1];
    check(std::fabs(std::sqrt(rest_dx * rest_dx + rest_dy * rest_dy) - world_item_arrival_radius_v1) < 0.01f &&
              entry.source_position[2] == victim.transform.position[2],
          "item stops on the 80-unit arrival radius at its spawn height");
    const auto rested = entry.source_position;
    store.advance(16);
    check(store.inspect(first, entry, error) && entry.source_position == rested,
          "a stopped item does not move again");

    // Pool: 5 live per visual category; the sixth recycles the oldest.
    std::vector<RuntimeWorldItemIdV1> ids{first};
    for (int i = 0; i < 4; ++i) {
        RuntimeWorldItemIdV1 id{};
        check(store.publish_death_drop(env.record("Potion0", 1), victim, id, error), error);
        ids.push_back(id);
    }
    check(store.size() == 5 && store.pool_evictions() == 0, "five potions fit the pool");
    RuntimeWorldItemIdV1 sixth{};
    check(store.publish_death_drop(env.record("Potion0", 1), victim, sixth, error), error);
    check(store.size() == 5 && store.pool_evictions() == 1 &&
          !store.inspect(ids.front(), entry, error) && store.inspect(sixth, entry, error) &&
          store.inspect(ids[1], entry, error), "sixth drop of a category evicts only the oldest slot");
    // A different category is independent.
    RuntimeWorldItemIdV1 other{};
    check(store.publish_death_drop(env.record("Longsword01", 1), victim, other, error), error);
    check(store.size() == 6 && store.pool_evictions() == 1, "categories are pooled independently");

    // ---- sensor target.
    check(select_world_item_target_v1(store, {5000, 5000, 0}) == invalid_runtime_world_item_v1,
          "far player has no target");
    check(select_world_item_target_v1(store, {0, 0, 10}) != invalid_runtime_world_item_v1,
          "player standing on the items targets one");

    // ---- pickup gates over a fresh store.
    RuntimeWorldItemAdapterV1 world(env.tables);
    auto player_state = std::make_shared<CharacterState>(make_default_character());
    Ctx ctx{1, player_state};
    RuntimeWorldItemInteractionServicesV1 services;
    services.context = &ctx;
    services.resolve_character_state = resolve;
    const auto player = actor(1, {0, 0, 10});
    WorldItemPickupRulesV1 rules;
    rules.potion_capacity = 3;
    WorldItemPickupReportV1 report;

    // Gold: increments the same CharacterState.
    RuntimeWorldItemIdV1 gold_id{};
    check(world.publish_death_drop(env.record("GoldStack01", 1, 5, 7), victim, gold_id, error), error);
    const auto gold_before = player_state->gold;
    check(interact_world_item_v1(world, gold_id, 1, true, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::picked_up &&
          player_state->gold == gold_before + 7 && world.size() == 0,
          "gold pickup must add the resolved value and retire the item");
    check(!interact_world_item_v1(world, gold_id, 1, true, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::rejected_looted_or_unknown &&
          player_state->gold == gold_before + 7, "a consumed item cannot be looted twice");

    check(potion_capacity_from_property_v1(12 * 256) == 12 && potion_capacity_from_property_v1(-256) == 0 &&
          potion_capacity_from_property_v1(0) == 0, "potion capacity is read from the q8 property");

    // Potions: stacking and capacity (original: take while potions < capacity).
    auto potion_count = [&] {
        std::uint32_t n = 0;
        for (const auto& item : player_state->inventory)
            if (item.definition_id == "Potion0") n += item.quantity;
        return n;
    };
    const auto base_potions = potion_count();
    rules.potion_capacity = std::int32_t(base_potions) + 2;
    RuntimeWorldItemIdV1 p1{}, p2{}, p3{};
    check(world.publish_death_drop(env.record("Potion0", 1), victim, p1, error), error);
    check(world.publish_death_drop(env.record("Potion0", 1), victim, p2, error), error);
    check(world.publish_death_drop(env.record("Potion0", 1), victim, p3, error), error);
    const auto stacks_before = player_state->inventory.size();
    check(interact_world_item_v1(world, p1, 1, true, &player, services, rules, report) &&
          potion_count() == base_potions + 1, "first potion pickup stacks into the same item");
    check(interact_world_item_v1(world, p2, 1, true, &player, services, rules, report) &&
          potion_count() == base_potions + 2, "second potion pickup stacks");
    check(player_state->inventory.size() <= stacks_before + 1, "potions must stack, not add rows");
    check(!interact_world_item_v1(world, p3, 1, true, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::potion_capacity &&
          potion_count() == base_potions + 2 && world.inspect(p3, entry, error),
          "at potion capacity the item stays on the ground and nothing is taken");

    // Equippable: inventory full keeps the item; otherwise inserted.
    RuntimeWorldItemIdV1 sword{};
    check(world.publish_death_drop(env.record("Longsword01", 1), victim, sword, error), error);
    const auto saved_inventory = player_state->inventory;
    const auto fill = source_inventory_slot_limit_v1 - saved_inventory.size();
    for (std::size_t i = 0; i < fill; ++i)
        player_state->inventory.push_back({"filler-" + std::to_string(i), "Longsword01", 1});
    check(!interact_world_item_v1(world, sword, 1, true, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::inventory_full &&
          report.error == "GAMEPLAYMENUS_INVENTORY_FULL" &&
          player_state->inventory.size() == source_inventory_slot_limit_v1 &&
          world.inspect(sword, entry, error), "full inventory refuses an equippable and keeps it in the world");
    rules.infinite_inventory = true;
    check(interact_world_item_v1(world, sword, 1, true, &player, services, rules, report) &&
          player_state->inventory.size() == source_inventory_slot_limit_v1 + 1,
          "InfiniteInventory bypasses the full gate");
    rules.infinite_inventory = false;
    player_state->inventory = saved_inventory;
    RuntimeWorldItemIdV1 sword2{};
    check(world.publish_death_drop(env.record("Longsword01", 1), victim, sword2, error), error);
    check(interact_world_item_v1(world, sword2, 1, true, &player, services, rules, report) &&
          player_state->inventory.size() == saved_inventory.size() + 1, "equippable pickup inserts one row");
    rules.auto_transmute_option = 1;
    RuntimeWorldItemIdV1 sword3{};
    check(world.publish_death_drop(env.record("Longsword01", 1), victim, sword3, error), error);
    check(!interact_world_item_v1(world, sword3, 1, true, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::auto_transmute_unavailable &&
          world.inspect(sword3, entry, error), "AutoTransmute>0 is an explicit refusal, item stays");
    rules.auto_transmute_option = 0;

    // Wrong player / not local.
    const auto other_player = actor(2, {0, 0, 10});
    check(!interact_world_item_v1(world, sword3, 2, true, &other_player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::failed, "unbound player is not served");
    check(!interact_world_item_v1(world, sword3, 1, false, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::rejected_not_local_player, "non-local pickup refused");

    // ---- drop_item_to_world + owner protection window.
    const auto before_drop = *player_state;
    RuntimeWorldItemIdV1 dropped{};
    const std::string drop_id = player_state->inventory.back().instance_id;
    const auto drop_definition = player_state->inventory.back().definition_id;
    check(drop_item_to_world(world, *player_state, drop_id, 1, {10, 20, 30}, 1, dropped, error), error);
    check(player_state->inventory.size() + 1 == before_drop.inventory.size() &&
          world.inspect(dropped, entry, error) && entry.from_inventory &&
          entry.source_position == std::array<float, 3>{10, 20, 30} &&
          entry.owner_protect_ms == player_drop_protection_ms_v1 &&
          entry.inventory_instance_id == drop_id, "drop removes the row and publishes at the feet");
    check(!interact_world_item_v1(world, dropped, 1, true, &player, services, rules, report) &&
          report.outcome == WorldItemPickupOutcomeV1::rejected_owner_protection,
          "the dropper cannot pick the item up inside the 5 s window");
    world.advance(5001);
    check(interact_world_item_v1(world, dropped, 1, true, &player, services, rules, report) &&
          player_state->inventory.size() == before_drop.inventory.size() &&
          player_state->inventory.back().definition_id == drop_definition,
          "after the window the item returns to the inventory (round trip)");
    RuntimeWorldItemIdV1 bad{};
    const auto unchanged = *player_state;
    check(!drop_item_to_world(world, *player_state, "no-such-instance", 1, {0, 0, 0}, 1, bad, error) &&
          player_state->inventory.size() == unchanged.inventory.size(), "invalid drop leaves the state unchanged");
    check(!drop_item_to_world(world, *player_state, drop_id, 256, {0, 0, 0}, 1, bad, error),
          "quantity above the record limit is rejected");

    // Automatic pickup type is read from the ItemTable row.
    std::map<std::int32_t, int> pickup_histogram;
    for (const auto& row : env.tables.items().rows) ++pickup_histogram[std::int32_t(row.record.words[item_word_pickup_type_v1])];
    std::cout << "pickup_type_histogram:";
    for (const auto& pair : pickup_histogram) std::cout << ' ' << pair.first << '=' << pair.second;
    std::cout << '\n';
    RuntimeWorldItemIdV1 auto_id{};
    check(world.publish_death_drop(env.record("GoldStack01", 1, 5, 3), victim, auto_id, error), error);
    check(world.inspect(auto_id, entry, error), error);
    std::cout << "{\"validation\":\"PASS\",\"gold_pickup_type\":"
              << entry.authored_item->record.words[item_word_pickup_type_v1]
              << ",\"gold_automatic\":" << world_item_is_automatic_pickup_v1(entry)
              << ",\"potion_row_type\":" << int(env.tables.items().rows[std::size_t(env.id_of("Potion0"))].record.words[item_word_pickup_type_v1])
              << ",\"sword_pickup_type\":" << int(env.tables.items().rows[std::size_t(env.id_of("Longsword01"))].record.words[item_word_pickup_type_v1])
              << ",\"visual_rows\":{\"potion\":" << env.tables.items().rows[std::size_t(env.id_of("Potion0"))].record.words[item_word_audio_visual_v1]
              << ",\"gold\":" << entry.visual_row
              << ",\"sword\":" << env.tables.items().rows[std::size_t(env.id_of("Longsword01"))].record.words[item_word_audio_visual_v1]
              << "}}\n";

    // B063 walk-over: contact BEGIN is reported once per contact, nearest first.
    {
        RuntimeWorldItemAdapterV1 ground(env.tables);
        RuntimeWorldItemIdV1 near_id{}, far_id{}, moving_id{};
        check(ground.publish_death_drop(env.record("Potion0", 1), actor(5, {1000, 0, 10}), near_id, error), error);
        check(ground.publish_death_drop(env.record("GoldStack01", 1, 5, 3), actor(5, {1100, 150, 10}), far_id, error), error);
        WorldItemContactTrackerV1 contacts;
        // Outside every sensor box (225 half extent): walking past reports nothing.
        check(contacts.begin_contacts(ground, {1000.0f, 400.0f, 10.0f}, true).empty(), "outside the sensor box: no contact");
        check(contacts.begin_contacts(ground, {1000.0f, -226.0f, 10.0f}, true).empty(), "just outside the box edge: no contact");
        // Stepping onto the edge begins contact with the potion only.
        auto began = contacts.begin_contacts(ground, {1000.0f, -224.0f, 10.0f}, true);
        check(began.size() == 1 && began[0] == near_id, "walking onto the item begins contact once");
        // Standing on it (even after a rejected attempt) does not begin contact again.
        check(contacts.begin_contacts(ground, {1000.0f, -200.0f, 10.0f}, true).empty(), "standing inside does not retrigger");
        check(contacts.begin_contacts(ground, {1005.0f, 0.0f, 10.0f}, true).size() == 1, "second item enters while the first is held");
        check(contacts.begin_contacts(ground, {1005.0f, 0.0f, 10.0f}, true).empty(), "no retrigger for either item");
        // Contact that begins while the player stands still does nothing until the player moves (SM_IsMoving gate).
        check(contacts.begin_contacts(ground, {1000.0f, 900.0f, 10.0f}, false).empty(), "leaving while idle reports nothing");
        check(contacts.begin_contacts(ground, {1000.0f, 0.0f, 10.0f}, false).empty(), "idle contact begin reports nothing");
        { const auto started = contacts.begin_contacts(ground, {1000.0f, 20.0f, 10.0f}, true); check(started.size() == 2, "starting to walk inside the box collects (PC fallback for the action button)"); }
        check(contacts.begin_contacts(ground, {1000.0f, 30.0f, 10.0f}, true).empty(), "one attempt per contact");
        // Leave and come back: contact begins again (a dropped/rejected item can be picked after re-entering).
        check(contacts.begin_contacts(ground, {1000.0f, 900.0f, 10.0f}, true).empty(), "leaving reports nothing");
        began = contacts.begin_contacts(ground, {1050.0f, 100.0f, 10.0f}, true);
        check(began.size() == 2 && began[0] == far_id && began[1] == near_id, "re-entering reports both, nearest first");
        // An item the player dropped (owner set) needs a real re-entry: dropping while standing on it and walking away keeps it.
        {
            RuntimeWorldItemAdapterV1 dropped(env.tables);
            WorldItemContactTrackerV1 own;
            InventoryItem held; held.instance_id = "held-1"; held.definition_id = "Longsword01"; held.quantity = 1;
            RuntimeWorldItemIdV1 dropped_id{};
            check(dropped.publish_inventory_drop(held, {500.0f, 500.0f, 10.0f}, 1, 5000, dropped_id, error), error);
            check(own.begin_contacts(dropped, {500.0f, 500.0f, 10.0f}, false).empty(), "dropped item under an idle player: nothing");
            check(own.begin_contacts(dropped, {520.0f, 500.0f, 10.0f}, true).empty(), "walking away from a dropped item keeps it");
            check(own.begin_contacts(dropped, {500.0f, 900.0f, 10.0f}, true).empty(), "left the dropped item's box");
            check(own.begin_contacts(dropped, {500.0f, 730.0f, 10.0f}, true).empty(), "just outside the box (225 units)");
            const auto back = own.begin_contacts(dropped, {500.0f, 650.0f, 10.0f}, true);
            check(back.size() == 1 && back[0] == dropped_id, "re-entering the dropped item's box collects it");
        }
        // A retired item (store cleared) drops out of the contact set; a new item sliding into a standing player begins contact.
        ground.clear();
        check(contacts.begin_contacts(ground, {1050.0f, 100.0f, 10.0f}, true).empty(), "retired item reports nothing");
        check(ground.publish_death_drop(env.record("Potion0", 1), actor(5, {1050.0f, 100.0f, 10.0f}), moving_id, error), error);
        began = contacts.begin_contacts(ground, {1050.0f, 100.0f, 10.0f}, true);
        check(began.size() == 1 && began[0] == moving_id, "an item landing on a standing player begins contact");
        contacts.clear();
        check(contacts.begin_contacts(ground, {1050.0f, 100.0f, 10.0f}, true).size() == 1, "clear forgets contacts (session rebind)");
        std::cout << "{\"walkover_contact\":\"PASS\"}\n";
    }

    // Motion curve (GameObject::UpdateTargetPosition + IsAtDestination). Ground plane only: no apex,
    // no bounce; speed 600 units/s; rest 80 units short of the landing point.
    {
        const std::array<float, 3> spawn{0.0f, 0.0f, 50.0f};
        const std::array<float, 3> landing{250.0f, 0.0f, 30.0f};
        check(std::fabs(world_item_speed_units_per_second_v1 - 600.0f) < 1e-3f, "item speed is 6 m/s = 600 units/s");
        const auto first = advance_world_item_step_v1(spawn, landing, 1.0f / 30.0f);
        check(std::fabs(first[0] - 20.0f) < 1e-3f && first[1] == 0.0f && first[2] == spawn[2],
              "one 1/30 s tick moves 20 units along the ground and keeps Z");
        // Settle: 250 - 80 = 170 units at 600 units/s = 0.2833 s, reached in fixed 1/120 s steps.
        auto position = spawn;
        float elapsed = 0.0f, previous_x = position[0];
        bool monotonic = true, z_fixed = true;
        for (int step = 0; step < 1200; ++step) {
            const auto next = advance_world_item_step_v1(position, landing, 1.0f / 120.0f);
            if (next == position) break;
            monotonic = monotonic && next[0] >= previous_x;
            z_fixed = z_fixed && next[2] == spawn[2];
            previous_x = next[0];
            position = next;
            elapsed += 1.0f / 120.0f;
        }
        check(monotonic && z_fixed, "item slides monotonically with constant Z (no apex or bounce)");
        check(std::fabs(position[0] - 170.0f) < 0.01f && std::fabs(elapsed - 170.0f / 600.0f) < 1.0f / 120.0f,
              "item settles 80 units short of its landing point after about 0.283 s");
        // Scatter bound: the farthest scatter (349 along + 150 lateral) travels about 0.5 s at most.
        const std::array<float, 3> far_landing{349.0f, 150.0f, 0.0f};
        float far_elapsed = 0.0f;
        auto far_position = spawn;
        for (int step = 0; step < 1200; ++step) {
            const auto next = advance_world_item_step_v1(far_position, far_landing, 1.0f / 120.0f);
            if (next == far_position) break;
            far_position = next;
            far_elapsed += 1.0f / 120.0f;
        }
        const float far_distance = std::sqrt(349.0f * 349.0f + 150.0f * 150.0f);
        check(far_elapsed <= (far_distance - 80.0f) / 600.0f + 1.0f / 120.0f && far_elapsed > 0.4f,
              "farthest scatter settles within about 0.5 s");
        // A landing point already inside the radius (no-killer scatter can do this) does not move the item.
        const std::array<float, 3> near_landing{60.0f, 0.0f, 0.0f};
        check(advance_world_item_step_v1(spawn, near_landing, 1.0f / 30.0f) == spawn,
              "an item inside the arrival radius stays where it spawned");
        check(advance_world_item_step_v1(spawn, landing, 0.0f) == spawn, "zero time does not move the item");
    }
    std::cout << "{\"motion_curve\":\"PASS\"}\n";
    return 0;
} catch (const std::exception& exception) {
    std::cerr << "FAIL: " << exception.what() << '\n';
    return 1;
}
