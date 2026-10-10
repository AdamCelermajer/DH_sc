#include "runtime_world_item_adapter_v1.hpp"
#include "runtime_world_item_interaction_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../../game-data/loot_table_selection_v8.hpp"
#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::loot;
using namespace dh2::data;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
Bytes view(const std::vector<std::uint8_t>& bytes) { return {bytes.data(), bytes.size()}; }

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    check(bool(file), "missing actual source cache: " + path.string());
    return {std::istreambuf_iterator<char>(file), {}};
}

bool entry(void*, const LootEntryRequestV8& request, std::int32_t& value,
           std::string&) {
    switch (request.operation) {
    case LootEntryOperationV8::debug_load:
    case LootEntryOperationV8::debug_query:
    case LootEntryOperationV8::mage_count:
    case LootEntryOperationV8::rogue_count:
    case LootEntryOperationV8::warrior_count:
    case LootEntryOperationV8::assertion:
        value = request.operation == LootEntryOperationV8::warrior_count ? 1 : 0;
        return true;
    }
    return false;
}

LootItemInfoV8 actual_outcome(const LootTablesV2::Borrow& tables,
                              const LootPowerResourcesV7::Borrow& powers,
                              std::int32_t loot_id, bool want_gold,
                              std::uint32_t& seed_out,
                              std::uint32_t& calls_out,
                              std::uint32_t& end_seed_out) {
    for (std::uint32_t seed = 1; seed < 10000; ++seed) {
        LootRandom8V2 random{seed, 0};
        LootTableSelectionV8 select(tables, powers, random, {nullptr, entry});
        std::vector<const LootEntry32V2*> entries;
        std::string error;
        if (!select.select(loot_id, entries, error)) continue;
        LootItemSelectionV8 expand(tables, random, {nullptr, entry});
        std::vector<LootItemInfoV8> items;
        if (!expand.expand(entries, false, items, error)) continue;
        auto found = std::find_if(items.begin(), items.end(), [&](const LootItemInfoV8& item) {
            return item.item && item.entry && item.quantity > 0 &&
                   (item_type(*item.item) == 13) == want_gold;
        });
        if (found != items.end()) {
            seed_out = seed;
            calls_out = random.calls;
            end_seed_out = random.seed;
            return *found;
        }
    }
    throw std::runtime_error("No matching outcome found from the requested actual Loot table");
}

struct PlayerStateContext { ActorId id; std::shared_ptr<CharacterState> state; };
bool resolve_player_state(void* raw, ActorId id, std::shared_ptr<CharacterState>& state,
                          std::string& error) {
    auto* context = static_cast<PlayerStateContext*>(raw);
    if (!context || context->id != id || !context->state) {
        error = "player state identity mismatch";
        return false;
    }
    state = context->state;
    error.clear();
    return true;
}

RuntimeWorldItemRecordV1 record_for(const LootTablesV2::Borrow& tables,
                                    std::int32_t loot_id,
                                    const LootItemInfoV8& item,
                                    ActorId source, ActorId killer) {
    const auto item_id = static_cast<std::int32_t>(item.id);
    check(item_id >= 0 && std::size_t(item_id) < tables.items().rows.size(),
          "actual selector returned an invalid ItemTable index");
    check(item.item == &tables.items().rows[std::size_t(item_id)],
          "actual selector row does not point into the source ItemTable");
    return {source, killer, loot_id, item.id, item.quantity, item.item, item.entry};
}

bool same_inventory(const CharacterState& a, const CharacterState& b) {
    if (a.inventory.size() != b.inventory.size() || a.gold != b.gold ||
        a.experience != b.experience || a.stats.level != b.stats.level ||
        a.stats.health != b.stats.health || a.stats.max_health != b.stats.max_health ||
        a.stats.resource != b.stats.resource || a.stats.max_resource != b.stats.max_resource ||
        a.source_stat_points != b.source_stat_points ||
        a.source_skill_points != b.source_skill_points) return false;
    for (std::size_t i = 0; i < a.inventory.size(); ++i) {
        if (a.inventory[i].instance_id != b.inventory[i].instance_id ||
            a.inventory[i].definition_id != b.inventory[i].definition_id ||
            a.inventory[i].quantity != b.inventory[i].quantity) return false;
    }
    return true;
}
}

int main(int argc, char** argv) try {
    check(argc == 3, "supply original shared asset root and source loot-power cache");
    AssetCatalog assets(argv[1]);
    std::string error;
    const auto read_asset = [&](const char* name) {
        return assets.read(std::filesystem::path("original-cache/data/pydata") / name);
    };
    auto item_records = read_asset("loot_table_pyarray.bin");
    auto item_names = read_asset("loot_table_pyarraynames.bin");
    auto item_schema = read_asset("loot_table_pystructnames.bin");
    LootTablesV2 loot_owner;
    check(loot_owner.load(view(item_records), view(item_names), view(item_schema), error), error);

    const std::filesystem::path power_root(argv[2]);
    const char* power_names[] = {
        "item_powers_pyarray.bin", "item_powers_pyarraynames.bin", "item_powers_pystructnames.bin",
        "item_powers_monopoly_pyarray.bin", "item_powers_monopoly_pyarraynames.bin",
        "item_powers_monopoly_pystructnames.bin", "num_prob_records_v7.bin",
        "loot_table_pyarraynames.bin", "loot_table_pystructnames.bin"};
    std::vector<std::vector<std::uint8_t>> power_raw;
    for (const auto* name : power_names) power_raw.push_back(read_file(power_root / name));
    LootPowerInputsV7 power_input{view(power_raw[0]), view(power_raw[1]), view(power_raw[2]),
        view(power_raw[3]), view(power_raw[4]), view(power_raw[5]), view(power_raw[6]),
        view(power_raw[7]), view(power_raw[8])};
    ItemPowerTablesV5 definitions;
    check(definitions.load(power_input.powers, power_input.power_names,
                           power_input.power_schema, error), error);
    LootPowerResourcesV7 power_owner;
    check(power_owner.load(power_input, definitions.borrow(), error), error);

    auto tables = loot_owner.borrow();
    auto powers = power_owner.borrow();
    const auto& loot_names = tables.loot_names();
    const auto barrel = std::find(loot_names.begin(), loot_names.end(), "Barrel_Level_01");
    check(barrel != loot_names.end(), "actual authored Barrel_Level_01 Loot row is absent");
    const auto barrel_id = static_cast<std::int32_t>(std::distance(loot_names.begin(), barrel));
    std::uint32_t gold_seed{}, gold_calls{}, gold_end_seed{};
    auto gold_outcome = actual_outcome(tables, powers, barrel_id, true,
                                       gold_seed, gold_calls, gold_end_seed);
    check(item_type(*gold_outcome.item) == 13, "Barrel_Level_01 actual outcome is not gold type13");
    const auto gold_item_id = static_cast<std::int32_t>(gold_outcome.id);
    const auto& gold_row = tables.items().rows[std::size_t(gold_item_id)];
    check(gold_item_id == 418 && tables.items().identifiers[std::size_t(gold_item_id)] == "GoldStack01" &&
          gold_row.record.words[22] == 13 && gold_row.record.words[27] == 1 &&
          gold_row.record.words[28] == 10,
          "actual GoldStack01 ItemTable identity/type/value bounds changed");

    const auto swamp = std::find(loot_names.begin(), loot_names.end(), "Swamp_Basic_Loot");
    check(swamp != loot_names.end(), "actual authored Swamp_Basic_Loot row is absent");
    const auto swamp_id = static_cast<std::int32_t>(std::distance(loot_names.begin(), swamp));
    std::uint32_t ordinary_seed{}, ordinary_calls{}, ordinary_end_seed{};
    auto ordinary_outcome = actual_outcome(tables, powers, swamp_id, false,
                                           ordinary_seed, ordinary_calls, ordinary_end_seed);
    const auto supported_loot_id = swamp_id;
    const auto supported_loot_name = std::string("Swamp_Basic_Loot");

    RuntimeWorldItemAdapterV1 store(tables);
    ActorState victim;
    victim.id = 77;
    victim.transform.position = {-120.5f, 44.25f, 13.0f};
    const auto ordinary = record_for(tables, supported_loot_id, ordinary_outcome, victim.id, 8);
    RuntimeDeathRewardServicesV1 reward_bridge;
    reward_bridge.context = &store;
    reward_bridge.spawn_world_item = RuntimeWorldItemAdapterV1::spawn_world_item_thunk;
    check(reward_bridge.spawn_world_item(reward_bridge.context, ordinary, victim, nullptr, error), error);
    std::vector<RuntimeWorldItemRenderV1> render;
    check(store.render_items(render, error), error);
    check(render.size() == 1, "reward callback did not publish into the current runtime world-item store");
    const RuntimeWorldItemIdV1 world_id = render[0].identity;
    check(world_id == 1 && store.size() == 1, "canonical current-game store did not publish one drop");

    RuntimeWorldItemEntryV1 inspected;
    check(store.inspect(world_id, inspected, error), error);
    check(inspected.source_outcome.item_id == ordinary.item_id &&
          inspected.source_outcome.authored_item == ordinary.authored_item &&
          inspected.source_outcome.authored_entry == ordinary.authored_entry &&
          inspected.quantity == ordinary.quantity && inspected.source_position == victim.transform.position,
          "world item did not retain exact original Loot/Item outcome and victim position");

    check(store.render_items(render, error), error);
    const auto actual_item_id = static_cast<std::int32_t>(ordinary.item_id);
    const auto& actual_row = tables.items().rows[std::size_t(actual_item_id)];
    check(render.size() == 1 && render[0].identity == world_id &&
          render[0].item_id == actual_item_id &&
          render[0].item_identifier == tables.items().identifiers[std::size_t(actual_item_id)] &&
          render[0].quantity == ordinary.quantity && render[0].position == victim.transform.position &&
          render[0].exact_icon_name == actual_row.icon_name &&
          render[0].source_name_text_oid == actual_row.record.words[17] &&
          render[0].has_exact_icon == !actual_row.icon_name.empty(),
          "render enumeration diverged from actual source ItemTable/drop data");

    auto character = std::make_shared<CharacterState>(
        make_default_character("pickup-player", "Pickup Player", "warrior"));
    PlayerStateContext player_context{8, character};
    RuntimeWorldItemInteractionServicesV1 interaction_services{
        &player_context, resolve_player_state};
    ActorState current_player;
    current_player.id = player_context.id;
    // Source ItemObject::Interact has no distance/radius check. The original
    // target/collision path must produce this explicit request; the adapter
    // retains both exact positions but never invents proximity eligibility.
    current_player.transform.position = {3000.0f, -900.0f, 12.0f};
    RuntimeWorldItemSourceInteractV1 source_interaction{player_context.id, world_id};
    RuntimeWorldItemInteractionReceiptV1 interaction_receipt;
    RuntimeWorldItemInteractionV1 interaction;
    check(!interaction.dispatch_live_player(player_context.id + 1, true, &current_player,
                                             source_interaction, interaction_services,
                                             store, interaction_receipt, error) && store.size() == 1,
          "wrong current player consumed an explicitly targeted source item");
    auto wrong_item_request = source_interaction;
    wrong_item_request.item = world_id + 100;
    check(!interaction.dispatch_live_player(player_context.id, true, &current_player,
                                             wrong_item_request, interaction_services,
                                             store, interaction_receipt, error) && store.size() == 1 &&
          character->inventory.empty(),
          "mismatched source item identity consumed a drop or changed inventory");
    RuntimeWorldItemPickupReceiptV1 picked;
    const bool source_dispatch_result = interaction.dispatch_live_player(
        player_context.id, true, &current_player, source_interaction,
        interaction_services, store, interaction_receipt, error);
    if (source_dispatch_result) {
        check(interaction_receipt.player == player_context.id &&
              interaction_receipt.item == world_id &&
              interaction_receipt.player_position == current_player.transform.position &&
              interaction_receipt.item_position == victim.transform.position &&
              interaction_receipt.pickup.completed,
              "source interaction receipt did not preserve player/item identity and exact positions");
    } else {
        check(store.size() == 1 && character->inventory.empty(),
              "unsupported source pickup branch changed the item or CharacterState");
        check(store.pickup(world_id, *character, picked, error), error);
    }
    check(store.size() == 0 && character->inventory.size() == 1 &&
          character->inventory[0].definition_id == tables.items().identifiers[std::size_t(actual_item_id)] &&
          character->inventory[0].quantity == ordinary.quantity,
          "source outcome pickup did not atomically add the original item and remove its drop");
    const auto committed = *character;
    check(!interaction.dispatch_live_player(player_context.id, true, &current_player,
                                            source_interaction, interaction_services,
                                            store, interaction_receipt, error) && store.size() == 0 &&
          same_inventory(*character, committed),
          "duplicate source interaction transferred the same world item twice");

    // Same real item result, but a malformed existing owner forces Presenter
    // validation to reject before commit.
    const auto rejected_outcome = record_for(tables, supported_loot_id, ordinary_outcome, victim.id, 8);
    RuntimeWorldItemIdV1 rejected_id{};
    check(store.publish_death_drop(rejected_outcome, victim, rejected_id, error), error);
    auto rejected_owner = make_default_character("reject-player", "Reject Player", "warrior");
    rejected_owner.stats.health = rejected_owner.stats.max_health + 1.0f;
    const auto rejected_before = rejected_owner;
    check(!store.pickup(rejected_id, rejected_owner, picked, error) &&
          store.size() == 1 && same_inventory(rejected_owner, rejected_before),
          "forced destination validation failure changed the item store or CharacterState");

    // A persisted CharacterState may already contain an earlier runtime
    // namespace. The deterministic collision suffix keeps the incoming item
    // unique without random IDs or dropping its source identity.
    RuntimeWorldItemIdV1 collision_id{};
    check(store.publish_death_drop(rejected_outcome, victim, collision_id, error), error);
    auto collision_owner = make_default_character("collision-player", "Collision Player", "warrior");
    collision_owner.inventory.push_back({"world-drop-3", tables.items().identifiers[std::size_t(actual_item_id)], 1});
    check(store.pickup(collision_id, collision_owner, picked, error), error);
    check(picked.completed && picked.inventory_instance_id == "world-drop-3-1" &&
          store.size() == 1, "world drop instance identity collided with existing character inventory");

    // The actual source creates GoldStack's value once as part of loot-item
    // construction; pickup then uses the stored value without RNG.
    LootRandom8V2 gold_creation_rng{gold_end_seed, gold_calls};
    std::int32_t gold_created_value{};
    check(dh2_loot_item_value_v7(&gold_created_value, &gold_creation_rng,
                                  &gold_row.record, nullptr, 0, 0) == 0 &&
          gold_created_value >= gold_row.record.words[27] &&
          gold_created_value <= gold_row.record.words[28] &&
          gold_creation_rng.calls == gold_calls + 1,
          "actual GoldStack01 source value calculation did not use its range and shared loot stream");
    auto gold = record_for(tables, barrel_id, gold_outcome, victim.id, 8);
    gold.resolved_gold_value = gold_created_value;
    RuntimeWorldItemIdV1 gold_id{};
    check(store.publish_death_drop(gold, victim, gold_id, error), error);
    const auto gold_rng_before_pickup = gold_creation_rng;
    auto gold_owner = make_default_character("gold-player", "Gold Player", "warrior");
    gold_owner.gold = 7;
    const auto gold_before = gold_owner;
    const auto gold_limit = 10;
    auto gold_character = std::make_shared<CharacterState>(gold_owner);
    PlayerStateContext gold_player_context{8, gold_character};
    RuntimeWorldItemInteractionServicesV1 gold_interaction_services{
        &gold_player_context, resolve_player_state, gold_limit};
    const auto gold_request = RuntimeWorldItemSourceInteractV1{8, gold_id};
    check(interaction.dispatch_live_player(8, true, &current_player, gold_request,
                                           gold_interaction_services, store,
                                           interaction_receipt, error), error);
    gold_owner = *gold_character;
    check(picked.completed && gold_owner.gold == 10 && gold_owner.inventory.empty() &&
          store.size() == 1 &&
          gold_creation_rng.seed == gold_rng_before_pickup.seed &&
          gold_creation_rng.calls == gold_rng_before_pickup.calls,
          "source-valued gold pickup failed its limit, inventory, or RNG contract");
    const auto gold_committed = gold_owner;
    check(!store.pickup(gold_id, gold_owner, picked, error, gold_limit) &&
          store.size() == 1 && same_inventory(gold_owner, gold_committed),
          "duplicate gold pickup changed gold or inventory");
    const auto unvalued = record_for(tables, barrel_id, gold_outcome, victim.id, 8);
    RuntimeWorldItemIdV1 unvalued_id{};
    check(!store.publish_death_drop(unvalued, victim, unvalued_id, error) &&
          unvalued_id == invalid_runtime_world_item_v1 && store.size() == 1,
          "unvalued source GoldStack was published into the pickup store");

    std::cout << "{\"validation\":\"PASS\",\"ordinary_loot_table\":\"" << supported_loot_name << "\","
              << "\"barrel_loot_table\":\"Barrel_Level_01\",\"world_id\":" << world_id
              << ",\"quantity\":" << unsigned(ordinary.quantity)
              << ",\"exact_position_retained\":true,\"explicit_source_interaction_admission\":true,"
              << "\"source_has_no_radius_gate\":true,"
              << "\"interaction_succeeded\":" << (source_dispatch_result ? "true" : "false") << ","
              << "\"ordinary_item_id\":" << actual_item_id << ","
              << "\"source_pickup_type_word3\":" << actual_row.record.words[3] << ","
              << "\"source_item_type_word22\":" << actual_row.record.words[22] << ","
              << "\"source_equipment_word26\":" << actual_row.record.words[26] << ","
              << "\"ordinary_rng_seed\":" << ordinary_seed << ",\"ordinary_rng_calls\":" << ordinary_calls << ","
              << "\"ordinary_rng_end_seed\":" << ordinary_end_seed << ","
              << "\"gold_rng_seed\":" << gold_seed << ",\"gold_rng_calls\":" << gold_calls << ","
              << "\"gold_rng_end_seed\":" << gold_end_seed << ",\"gold_creation_rng_calls\":" << gold_creation_rng.calls << ","
              << "\"gold_item_id\":" << gold_item_id << ","
              << "\"gold_item_identifier\":\"GoldStack01\","
                << "\"gold_pickup_type_word3\":" << gold_row.record.words[3] << ","
              << "\"gold_value_min_word27\":" << gold_row.record.words[27] << ","
              << "\"gold_value_max_word28\":" << gold_row.record.words[28] << ","
                << "\"gold_test_source_value\":" << gold_created_value << ",\"gold_pickup_rng_unchanged\":true,"
                << "\"gold_pickup_limit\":" << gold_limit << ",\"gold_pickup_credited\":" << gold_owner.gold << ","
                << "\"gold_pickup_atomic\":true,\"gold_valuation_fail_closed\":true,"
              << "\"exact_icon_available\":" << (render[0].has_exact_icon ? "true" : "false") << "}\n";
    return 0;
} catch (const std::exception& exception) {
    std::cerr << "runtime world-item adapter test failed: " << exception.what() << '\n';
    return 1;
}
