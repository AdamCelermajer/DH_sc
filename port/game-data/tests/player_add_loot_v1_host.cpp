#include "../player_add_loot_v1.hpp"
#include "../player_gear_effects_v5.hpp"
#include "../player_equipment_queries_live_v1.hpp"
#include "../class_tables.hpp"

extern "C" {
#include "../../random/random.h"
}

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace dh2::data;
using namespace dh2::data::player_add_loot_v1;
using Raw = std::vector<std::uint8_t>;

static unsigned checks;

static void ck(bool ok, const char* message) {
    ++checks;
    if (!ok) throw std::runtime_error(message);
}

static Raw file(const char* path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error(std::string("Cannot read ") + path);
    return {std::istreambuf_iterator<char>(input), {}};
}

static Bytes bytes(const Raw& raw) { return {raw.data(), raw.size()}; }

static Damage attack_damage(const std::int32_t* attacker_properties,
                            const std::int32_t* defender_properties,
                            std::int32_t main_category) {
    CombatantView attacker{attacker_properties, main_category, -1, 0, 0, 0, 0, 0};
    CombatantView defender{defender_properties, -1, -1, 0, 0, 0, 0, 0};
    CombatRandom random{0x12345, 0};
    Damage damage{};
    DamageRequest request{&attacker, &defender, &random, 0, 0, 0, 0};
    if (dh2_combat_damage(&damage, &request))
        throw std::runtime_error("Ring combat PropertyView query rejected");
    return damage;
}

struct Reader {
    const Raw& raw;
    std::size_t at{};

    std::uint32_t u32() {
        ck(at <= raw.size() && raw.size() - at >= 4, "truncated fixture word");
        std::uint32_t value{};
        std::memcpy(&value, raw.data() + at, sizeof(value));
        at += sizeof(value);
        return value;
    }

    Raw block() {
        const auto size = u32();
        ck(size <= raw.size() - at, "truncated fixture block");
        Raw value(raw.begin() + std::ptrdiff_t(at),
                  raw.begin() + std::ptrdiff_t(at + size));
        at += size;
        return value;
    }
};

struct RandomOwner {
    dh2_random_state state{};
    std::uint32_t draws{};

    RandomOwner(std::uint32_t seed, std::uint32_t calls)
        : state{{seed, 0x98765432u}, {calls, 37u}} {}

    static bool next(void* opaque, std::int32_t bound, std::uint32_t stream,
                     std::int32_t& value, std::string& error) {
        auto& self = *static_cast<RandomOwner*>(opaque);
        if (stream != 0) {
            error = "unexpected source RNG stream";
            return false;
        }
        value = dh2_random_next(&self.state, std::uint32_t(bound), stream);
        ++self.draws;
        return true;
    }

    InventoryRandomServiceV4 service() { return {this, next}; }
};

struct Services {
    const ItemTable* items{};
    ItemPresentationOwnerV5* presentation{};
    PlayerGearEffectsV5* gear{};
    std::uintptr_t visual{};
    std::uint32_t debug_queries{};
    std::uint32_t power_adds{};
    bool fail_add_power{};

    static const Item* metadata(void* opaque, const ItemInstanceV1& instance,
                                std::string& error) {
        const auto& self = *static_cast<Services*>(opaque);
        const auto* row = item(*self.items, instance.id);
        if (!row) error = "fixture item metadata missing";
        return row;
    }

    static bool text(void*, ItemInstanceV1&, const ItemTextRequestV5& request,
                     ItemTextResponseV5& response, std::string& output,
                     std::string& error) {
        switch (request.operation) {
        case ItemTextOperationV5::constant:
            response.value = 17;
            return true;
        case ItemTextOperationV5::integer_string:
            if (request.value == -1) {
                error = "missing description fixture";
                return false;
            }
            response.text = "OID" + std::to_string(request.value);
            return true;
        case ItemTextOperationV5::class_name:
            response.value = request.value;
            return true;
        case ItemTextOperationV5::parse_varargs:
        case ItemTextOperationV5::parse_ex:
            if (request.input) output += request.input;
            for (std::uint32_t i = 0; i < request.count; ++i)
                output += ":" + std::to_string(request.arguments[i].integer);
            return true;
        }
        error = "unsupported text fixture operation";
        return false;
    }

    static bool inventory(void* opaque, FreshInventoryOwnedV4&,
                          const OwnedInventoryRequestV4& request,
                          OwnedInventoryResponseV4& response,
                          std::string& error) {
        auto& self = *static_cast<Services*>(opaque);
        const auto text_service = ItemTextServicesV5{self.context(), metadata, text};
        switch (request.operation) {
        case OwnedInventoryOperationV4::update_name:
            return item_update_name_v5(*request.item, text_service, error);
        case OwnedInventoryOperationV4::update_stats:
            return item_update_stats_v5(*request.item, text_service, error);
        case OwnedInventoryOperationV4::update_requirements:
            return item_update_requirements_v5(*request.item, text_service, error);
        case OwnedInventoryOperationV4::add_power:
            ++self.power_adds;
            if (self.fail_add_power) {
                error = "declared AddPower fixture failure";
                return false;
            }
            return self.presentation->add_power(*request.item, request.argument,
                                                std::int32_t(request.index),
                                                text_service, error);
        case OwnedInventoryOperationV4::player_count:
            response.value = 1;
            return true;
        case OwnedInventoryOperationV4::inventory_full:
            response.value = 0;
            return true;
        case OwnedInventoryOperationV4::full_notifications:
        case OwnedInventoryOperationV4::gold_notifications:
            return true;
        case OwnedInventoryOperationV4::update_gear_properties:
            if (!self.gear) { error = "missing canonical gear owner"; return false; }
            return self.gear->update_properties(error);
        case OwnedInventoryOperationV4::validate_hp_mp:
            if (!self.gear) { error = "missing canonical gear owner"; return false; }
            return self.gear->validate_hp_mp(error);
        case OwnedInventoryOperationV4::skin:
            if (!self.gear) { error = "missing canonical gear owner"; return false; }
            return self.gear->update_skin(&self.visual, {}, error);
        case OwnedInventoryOperationV4::current_player:
            response.identity = 0;
            response.value = 0;
            return true;
        case OwnedInventoryOperationV4::debug_load:
        case OwnedInventoryOperationV4::debug_query:
            ++self.debug_queries;
            response.value = 0;
            return true;
        default:
            error = "required inventory continuation unavailable in host fixture";
            return false;
        }
    }

    static void observe(void* opaque, FreshInventoryOwnedV4&,
                        const OwnedInventoryRequestV4& request) {
        if (request.operation != OwnedInventoryOperationV4::destroy_item) return;
        auto& self = *static_cast<Services*>(opaque);
        std::string error;
        if (!self.presentation->forget(*request.item, error))
            throw std::runtime_error(error);
    }

    void* context() { return this; }
    ItemTextServicesV5 text_service() { return {this, metadata, text}; }
};

static void compare_source_snapshot(const Raw& source,
                                   const FreshInventoryOwnedV4& inventory) {
    Reader expected{source};
    ck(expected.u32() == std::uint32_t(inventory.gold()), "gold differs from source AddLoot");
    ck(expected.u32() == std::uint32_t(inventory.current_equipment()),
       "equipment mode differs from source AddLoot");
    const auto count = expected.u32();
    ck(count == inventory.items().size(), "item count differs from source AddLoot");

    for (std::uint32_t i = 0; i < count; ++i) {
        const auto& slot = *inventory.items()[i];
        const auto& instance = *slot.item;
        ck(expected.u32() == std::uint32_t(instance.id), "item id differs from source AddLoot");
        ck(expected.u32() == instance.quantity, "item quantity differs from source AddLoot");
        ck(expected.u32() == std::uint32_t(instance.value), "item value differs from source AddLoot");
        ck(expected.u32() == instance.identified, "identified state differs from source AddLoot");
        ck(expected.u32() == std::uint8_t(slot.slots[0]), "left slot differs from source AddLoot");
        ck(expected.u32() == std::uint8_t(slot.slots[1]), "right slot differs from source AddLoot");
        ck(expected.u32() == instance.powers.size(), "power count differs from source AddLoot");
        for (const auto power : instance.powers)
            ck(expected.u32() == std::uint32_t(power), "power id differs from source AddLoot");
    }

    std::uint32_t potion = std::numeric_limits<std::uint32_t>::max();
    for (std::uint32_t i = 0; i < inventory.items().size(); ++i)
        if (inventory.items()[i]->item.get() == inventory.potion()) potion = i;
    ck(expected.u32() == potion, "potion index differs from source AddLoot");
    for (unsigned set = 0; set < 2; ++set) {
        for (unsigned equip_slot = 0; equip_slot < 9; ++equip_slot) {
            std::uint32_t index = std::numeric_limits<std::uint32_t>::max();
            const auto* equipped = inventory.equipment()[set][equip_slot];
            for (std::uint32_t i = 0; i < inventory.items().size(); ++i)
                if (equipped == inventory.items()[i].get()) index = i;
            ck(expected.u32() == index, "equipment mapping differs from source AddLoot");
        }
    }
    ck(expected.at == source.size(), "trailing original AddLoot snapshot bytes");
}

int main(int argc, char** argv) {
    try {
        ck(argc == 4, "expected original AddLoot fixture, canonical pydata, and character data directories");
        const auto fixture = file(argv[1]);
        Reader input{fixture};
        ck(input.u32() == 0x37564c41u, "invalid original AddLoot fixture magic");
        const auto loot_id = std::int32_t(input.u32());
        const auto seed = input.u32();
        const auto calls = input.u32();
        const auto capacity = std::int8_t(input.u32());
        const auto value_bonus = std::int32_t(input.u32());
        const auto power_bonus = std::int32_t(input.u32());
        const auto requested = std::int32_t(input.u32());
        const auto difficulty = std::int32_t(input.u32());
        const auto seed_after = input.u32();
        const auto calls_after = input.u32();
        const auto source_snapshot = input.block();
        ck(input.at == fixture.size(), "trailing original AddLoot fixture bytes");

        const std::array<const char*, 9> cache_names{{
            "item_powers_pyarray.bin", "item_powers_pyarraynames.bin",
            "item_powers_pystructnames.bin", "item_powers_monopoly_pyarray.bin",
            "item_powers_monopoly_pyarraynames.bin", "item_powers_monopoly_pystructnames.bin",
            "loot_table_pyarray.bin", "loot_table_pyarraynames.bin",
            "loot_table_pystructnames.bin"}};
        std::array<Raw, 9> cache;
        for (std::size_t i = 0; i < cache.size(); ++i)
            cache[i] = file((std::string(argv[2]) + "/" + cache_names[i]).c_str());

        ck(cache[6].size() == 284104, "unexpected canonical loot/NumProbArray cache size");
        Raw quantities(cache[6].begin() + 283108, cache[6].end());
        std::string error;
        ItemPowerTablesV5 power_tables;
        ck(power_tables.load(bytes(cache[0]), bytes(cache[1]), bytes(cache[2]), error),
           "could not load existing V5 item power owner");
        const auto power_view = power_tables.borrow();
        LootPowerInputsV7 power_input{
            bytes(cache[0]), bytes(cache[1]), bytes(cache[2]),
            bytes(cache[3]), bytes(cache[4]), bytes(cache[5]), bytes(quantities),
            bytes(cache[7]), bytes(cache[8])};
        LootPowerResourcesV7 resources;
        ck(resources.load(power_input, power_view, error),
           "could not load existing V7 loot power resources");

        LootTablesV2 loot_tables;
        ck(loot_tables.load(bytes(cache[6]), bytes(cache[7]), bytes(cache[8]), error),
           "could not load existing LootTablesV2 owner");
        const auto loot_view = loot_tables.borrow();
        ck(loot_id >= 0 && std::size_t(loot_id) < loot_view.loots().size(),
           "source AddLoot fixture row is out of range");

        RandomOwner random(seed, calls);
        const auto rng = random.service();
        LootPowerCreationV7 creation(resources.borrow(), rng);
        ItemPresentationOwnerV5 presentation(power_view);
        Services service_context{&loot_view.items(), &presentation};
        const auto text = service_context.text_service();
        const std::string character_data = argv[3];
        auto character_bytes = file((character_data + "/character_properties_pyarray.bin").c_str());
        auto character_names = file((character_data + "/character_properties_pyarraynames.bin").c_str());
        auto character_schema = file((character_data + "/character_properties_pystructnames.bin").c_str());
        CharacterTable characters;
        ck(load_characters(bytes(character_bytes), bytes(character_names),
                          bytes(character_schema), characters, error),
           "could not load canonical Character property table");
        PropertyRules rules;
        ck(load_property_rules(characters, rules, error),
           "could not load canonical Character property rules");
        auto class_bytes = file((character_data + "/character_classes_pyarray.bin").c_str());
        auto class_names = file((character_data + "/character_classes_pyarraynames.bin").c_str());
        auto class_schema = file((character_data + "/character_classes_pystructnames.bin").c_str());
        ClassTables class_tables;
        ck(load_classes(bytes(class_bytes), bytes(class_names), bytes(class_schema),
                        class_tables, error), "could not load canonical class tables");
        std::vector<ClassRow> class_rows;
        for (const auto& row : class_tables.rows)
            class_rows.push_back({row.data(), std::uint32_t(row.size())});
        constexpr std::uint32_t warrior = 263;
        ck(characters.rows.size() > warrior, "Warrior Character row is missing");
        PropertyState properties{};
        reset_properties(rules, properties, &characters.rows[warrior]);
        auto view = property_view(rules, properties);
        ck(dh2_class_recalc_base(class_rows.data(), std::uint32_t(class_rows.size()),
                                 properties.base.data(), &view) == 0,
           "could not initialize the same Warrior Character PropertyView");
        const auto unequipped_resolved = properties.resolved;
        properties.resolved[9] = loot_id;
        constexpr std::uintptr_t character = UINT64_C(0x100000007);
        FreshInventoryOwnedV4 inventory(character, loot_view, rng, capacity, properties);
        PlayerGearEffectsV5 gear(inventory, view, class_rows.data(),
                                 std::uint32_t(class_rows.size()), power_view);
        service_context.gear = &gear;
        OwnedInventoryServicesV4 inventory_services{
            &service_context, Services::inventory, Services::observe, false};
        OwnedLootEffectsV7 effects{
            &creation, power_view, text, value_bonus, power_bonus, requested, difficulty};
        LootEntrySelectionContextV1 selection{};
        std::unique_ptr<ItemInstanceV1> pending;

        Bindings bindings{};
        bindings.character = character;
        bindings.inventory = &inventory;
        bindings.resolved_loot_property = properties.resolved.data() + 9;
        bindings.selection = &selection;
        bindings.difficulty = difficulty;
        bindings.pending = {&pending};
        bindings.inventory_services = &inventory_services;
        bindings.creation = &creation;
        bindings.powers = power_view;
        bindings.text = text;
        const Request request{character, {loot_id, value_bonus, power_bonus, requested, 0}};
        ck(value_bonus == 0 && power_bonus == 0,
           "fixture is outside the source _InitEquipment bonus arguments");
        ck(request.arguments[4] == 0, "fixture is outside the source _InitEquipment boolean argument");

        Result result{};
        const auto add_status = invoke(bindings, request, &result, error);
        ck(add_status == Status::complete, error.empty()
               ? "selected AddLoot adapter did not complete"
               : error.c_str());
        ck(result.attempted && result.loot_table == loot_id,
           "AddLoot adapter did not report the invoked table");
        ck(result.items_before == 0 && result.items_after == inventory.items().size(),
           "AddLoot adapter item-count result differs from its live owner");
        ck(result.pending_item == bool(pending), "AddLoot adapter pending-slot result is stale");
        ck(!pending, "player AddLoot left an item in the caller's pending slot");
        ck(inventory.properties() == &properties && inventory.character() == character,
           "AddLoot adapter replaced the live Character inventory/property owner");
        ck(service_context.debug_queries > 0,
           "AddLoot did not reuse the existing Debug service provider");
        ck(service_context.power_adds > 0,
           "powered AddLoot fixture did not reach the existing AddPower provider");
        compare_source_snapshot(source_snapshot, inventory);
        ck(random.state.seeds[0] == seed_after && random.state.counters[0] == calls_after,
           "shared inventory RNG differs from the original AddLoot call");
        ck(random.state.seeds[1] == 0x98765432u && random.state.counters[1] == 37u,
           "AddLoot changed the unrelated RNG stream");

        // A stale Character Loot property must reject before touching either
        // inventory state or the caller's result object.
        const auto item_count = inventory.items().size();
        const auto old_result = result;
        auto wrong_property = request;
        ++wrong_property.arguments[0];
        ck(invoke(bindings, wrong_property, &result, error) == Status::invalid_argument,
           "mismatched live Loot property was accepted");
        ck(!error.empty() && result.loot_table == old_result.loot_table &&
               inventory.items().size() == item_count,
           "invalid AddLoot call changed output or inventory state");

        auto wrong_call_shape = request;
        ++wrong_call_shape.arguments[1];
        ck(invoke(bindings, wrong_call_shape, &result, error) == Status::invalid_argument,
           "unsupported AddLoot value bonus was accepted");
        ck(error.find("outside the recovered") != std::string::npos &&
               result.loot_table == old_result.loot_table &&
               inventory.items().size() == item_count,
           "unsupported AddLoot call shape changed output or inventory state");

        // Missing reached providers remain explicit failures. The owned loot
        // implementation keeps the actual prefix, while this adapter reports
        // the required-service error and its current inventory count.
        auto failed_services = inventory_services;
        failed_services.invoke = nullptr;
        auto failed_bindings = bindings;
        failed_bindings.inventory_services = &failed_services;
        Result failed_result{};
        ck(invoke(failed_bindings, request, &failed_result, error) == Status::invalid_argument,
           "missing required AddLoot provider was accepted");
        ck(error.find("provider unavailable") != std::string::npos,
           "missing AddLoot provider was not reported explicitly");
        ck(inventory.items().size() == item_count,
           "invalid provider binding mutated the inventory");

        // A reached provider failure remains a failed source continuation and
        // exposes its real pending Item prefix through the caller's slot.
        RandomOwner failure_random(seed, calls);
        const auto failure_rng = failure_random.service();
        LootPowerCreationV7 failure_creation(resources.borrow(), failure_rng);
        ItemPresentationOwnerV5 failure_presentation(power_view);
        Services failure_context{&loot_view.items(), &failure_presentation};
        failure_context.fail_add_power = true;
        const auto failure_text = failure_context.text_service();
        PropertyState failure_properties{};
        failure_properties.resolved[9] = loot_id;
        FreshInventoryOwnedV4 failure_inventory(
            character, loot_view, failure_rng, capacity, failure_properties);
        OwnedInventoryServicesV4 failure_services{
            &failure_context, Services::inventory, Services::observe, false};
        OwnedLootEffectsV7 failure_effects{
            &failure_creation, power_view, failure_text,
            value_bonus, power_bonus, requested, difficulty};
        std::unique_ptr<ItemInstanceV1> failure_pending;
        auto failure_bindings = bindings;
        failure_bindings.inventory = &failure_inventory;
        failure_bindings.resolved_loot_property = failure_properties.resolved.data() + 9;
        failure_bindings.pending = {&failure_pending};
        failure_bindings.inventory_services = &failure_services;
        failure_bindings.creation = &failure_creation;
        failure_bindings.text = failure_text;
        Result failure_result{};
        ck(invoke(failure_bindings, request, &failure_result, error) == Status::failed,
           "reached AddPower failure was hidden by the adapter");
        ck(error == "declared AddPower fixture failure" && failure_result.attempted &&
               failure_result.pending_item && failure_pending &&
               failure_result.items_after == failure_inventory.items().size() &&
               failure_context.power_adds == 1,
           "failed AddLoot did not expose its required provider error and Item prefix");
        ck(failure_random.draws > 0,
           "failed powered AddLoot did not preserve its preceding shared RNG draws");
        ck(failure_presentation.forget(*failure_pending, error),
           "could not retire failed pending Item presentation state");
        failure_pending.reset();

        ck(inventory.items().size() == 1, "source AddLoot fixture no longer creates one item");
        auto& generated = *inventory.items()[0]->item;
        ck(generated.id == 841 && generated.powers.size() == 1 && generated.powers[0] == 193,
           "original random AddPower result differs from the checked-in source fixture");
        const auto* generated_metadata = item(loot_view.items(), generated.id);
        ck(generated_metadata && generated_metadata->record.words[26] == 3,
           "source-generated powered item is not the expected equippable boots row");
        const auto generated_power = power_view.rows().at(std::size_t(generated.powers[0]));
        ck(generated_power.properties.size() == 1 && generated_power.properties[0].type == 2,
           "generated power does not carry the original gear-property row");
        std::int32_t equip_result{};
        ck(inventory.character_auto_equip(0, equip_result, inventory_services, error),
           error.empty() ? "source-generated power item failed Character::AutoEquip" : error.c_str());
        ck(equip_result == 1 && inventory.equipment()[0][3] == inventory.items()[0].get(),
           "generated powered boots did not reach the canonical V4 equipment slot");
        ck(inventory.properties() == &properties && view.gear == properties.gear.data() &&
               properties.gear[150] == generated_power.properties[0].value &&
               properties.resolved[150] != unequipped_resolved[150],
           "generated power did not recalculate the same Character PropertyView");

        for (const auto& slot : inventory.items())
            ck(presentation.forget(*slot->item, error), "could not retire presentation test state");

        // QuestReward is an original fixed ring row with ItemPowerList 56.
        // Generate its random affix from the source table, carry that same
        // Item through V4 world pickup and equip, then query the live combat
        // projection from the Character PropertyView.
        PropertyState ring_properties{};
        reset_properties(rules, ring_properties, &characters.rows[warrior]);
        auto ring_view = property_view(rules, ring_properties);
        ck(dh2_class_recalc_base(class_rows.data(), std::uint32_t(class_rows.size()),
                                 ring_properties.base.data(), &ring_view) == 0,
           "could not initialize the ring fixture's Warrior PropertyView");
        const auto ring_before = ring_properties;
        RandomOwner ring_random(seed, calls);
        const auto ring_rng = ring_random.service();
        LootPowerCreationV7 ring_creation(resources.borrow(), ring_rng);
        FreshInventoryOwnedV4 ring_inventory(character, loot_view, ring_rng, capacity,
                                              ring_properties);
        ItemPresentationOwnerV5 ring_presentation(power_view);
        Services ring_context{&loot_view.items(), &ring_presentation};
        PlayerGearEffectsV5 ring_gear(ring_inventory, ring_view, class_rows.data(),
                                      std::uint32_t(class_rows.size()), power_view);
        ring_context.gear = &ring_gear;
        const auto ring_services = OwnedInventoryServicesV4{
            &ring_context, Services::inventory, Services::observe, false};
        const ItemTextServicesV5 ring_text = ring_context.text_service();
        const OwnedLootEffectsV7 ring_effects{
            &ring_creation, power_view, ring_text, 0, 0, -1, 0};
        LootEntrySelectionContextV1 ring_selection{};
        std::unique_ptr<ItemInstanceV1> ring_pending;
        ck(ring_inventory.add_world_loot_table(193, ring_selection,
                {&ring_pending}, ring_services, ring_effects, error),
           error.empty() ? "source QuestReward ring loot generation failed" : error.c_str());
        ck(!ring_pending && ring_inventory.world_items().size() == 1 &&
               ring_inventory.world_items()[0]->item->id == 929 &&
               !ring_inventory.world_items()[0]->item->powers.empty(),
           "source QuestReward did not create its powered ring in the canonical world-item owner");
        const auto ring_power_id = ring_inventory.world_items()[0]->item->powers[0];
        ck(ring_power_id >= 0 && std::size_t(ring_power_id) < power_view.rows().size(),
           "generated ring power is outside the retained source power table");
        std::int32_t ring_index = -1;
        ck(ring_inventory.pickup_world_item(0, ring_index, ring_services, error) &&
               ring_index == 0 && ring_inventory.world_items().empty() &&
               ring_inventory.items().size() == 1,
           error.empty() ? "source-generated ring failed V4 world pickup" : error.c_str());
        std::int32_t ring_equip_result{};
        ck(ring_inventory.character_auto_equip(0, ring_equip_result,
                ring_services, error),
           error.empty() ? "source-generated ring failed Character::AutoEquip" : error.c_str());
        ck(ring_equip_result == 1 &&
               (ring_inventory.equipment()[0][5] || ring_inventory.equipment()[0][6]),
           "source-generated ring did not reach one canonical jewelry slot");
        bool ring_gear_changed = false, ring_resolved_changed = false;
        for (std::size_t p = 0; p < ring_properties.gear.size(); ++p) {
            ring_gear_changed |= ring_properties.gear[p] != ring_before.gear[p];
            ring_resolved_changed |= ring_properties.resolved[p] != ring_before.resolved[p];
        }
        ck(ring_inventory.properties() == &ring_properties &&
               ring_view.gear == ring_properties.gear.data() &&
               ring_gear_changed && ring_resolved_changed,
           "generated ring power did not update the canonical Character gear/resolved sheets");
        PlayerEquipmentQueriesLiveV1 ring_queries(ring_inventory, ring_view);
        CombatantView ring_attacker{};
        ck(ring_queries.combat_view(ring_attacker, error) &&
               ring_attacker.properties == ring_properties.resolved.data(),
           "combat view did not borrow the ring-updated Character PropertyView");
        PropertySheet ring_defender = rules.defaults;
        const auto ring_damage = attack_damage(ring_attacker.properties,
                                               ring_defender.data(),
                                               ring_attacker.main_damage_class);
        ck(ring_damage.amount >= 0,
           "source-generated ring combat projection returned invalid damage");
        const auto& ring_power = power_view.rows()[std::size_t(ring_power_id)];
        for (const auto& slot : ring_inventory.items())
            ck(ring_presentation.forget(*slot->item, error),
               "could not retire generated ring presentation state");

        // Repeat the original powered AddLoot row through Character::DropLoot's
        // world staging path. The generated 841/193 Item must retain identity
        // across pickup, then the same V4 gear owner recalculates Warrior stats.
        PropertyState world_properties{};
        reset_properties(rules, world_properties, &characters.rows[warrior]);
        auto world_view = property_view(rules, world_properties);
        ck(dh2_class_recalc_base(class_rows.data(), std::uint32_t(class_rows.size()),
                                 world_properties.base.data(), &world_view) == 0,
           "could not initialize the powered world-drop Warrior PropertyView");
        const auto world_before = world_properties;
        RandomOwner world_random(seed, calls);
        const auto world_rng = world_random.service();
        LootPowerCreationV7 world_creation(resources.borrow(), world_rng);
        FreshInventoryOwnedV4 world_inventory(character, loot_view, world_rng, capacity,
                                               world_properties);
        ItemPresentationOwnerV5 world_presentation(power_view);
        Services world_context{&loot_view.items(), &world_presentation};
        PlayerGearEffectsV5 world_gear(world_inventory, world_view, class_rows.data(),
                                       std::uint32_t(class_rows.size()), power_view);
        world_context.gear = &world_gear;
        const OwnedInventoryServicesV4 world_services{
            &world_context, Services::inventory, Services::observe, false};
        const auto world_text = world_context.text_service();
        const OwnedLootEffectsV7 world_effects{
            &world_creation, power_view, world_text, value_bonus, power_bonus,
            requested, difficulty};
        LootEntrySelectionContextV1 world_selection{};
        std::unique_ptr<ItemInstanceV1> world_pending;
        ck(world_inventory.add_world_loot_table(loot_id, world_selection,
                {&world_pending}, world_services, world_effects, error),
           error.empty() ? "source AddLoot table failed world-drop generation" : error.c_str());
        ck(!world_pending && world_inventory.items().empty() &&
               world_inventory.world_items().size() == 1 &&
               world_inventory.world_items()[0]->item->id == 841 &&
               world_inventory.world_items()[0]->item->powers.size() == 1 &&
               world_inventory.world_items()[0]->item->powers[0] == 193,
           "source AddLoot world path did not preserve powered Item 841/193");
        auto* world_generated = world_inventory.world_items()[0]->item.get();
        std::int32_t world_index = -1;
        ck(world_inventory.pickup_world_item(0, world_index, world_services, error) &&
               world_index == 0 && world_inventory.world_items().empty() &&
               world_inventory.items().size() == 1 &&
               world_inventory.items()[0]->item.get() == world_generated,
           error.empty() ? "powered world Item failed canonical V4 pickup" : error.c_str());
        std::int32_t world_equip_result{};
        ck(world_inventory.character_auto_equip(0, world_equip_result,
                world_services, error),
           error.empty() ? "picked-up powered world Item failed Character::AutoEquip" : error.c_str());
        const auto world_power = power_view.rows().at(193);
        ck(world_equip_result == 1 && world_inventory.equipment()[0][3] ==
               world_inventory.items()[0].get() && world_inventory.properties() ==
               &world_properties && world_view.gear == world_properties.gear.data() &&
               world_properties.gear[150] == world_power.properties[0].value &&
               world_properties.resolved[150] != world_before.resolved[150],
           "world-picked Item 841/193 failed canonical gear/property recalculation");
        ck(world_random.state.seeds[0] == seed_after &&
               world_random.state.counters[0] == calls_after,
           "world AddLoot RNG differs from the original powered AddLoot fixture");
        for (const auto& slot : world_inventory.items())
            ck(world_presentation.forget(*slot->item, error),
               "could not retire powered world-drop presentation state");
        std::cout << "{\"validation\":\"PASS\",\"source_host_parity\":true"
                  << ",\"loot_table_row\":" << loot_id
                  << ",\"items\":" << result.items_after
                  << ",\"power_adds\":" << service_context.power_adds
                  << ",\"debug_queries\":" << service_context.debug_queries
                  << ",\"source_rng_after\":[" << seed_after << ',' << calls_after << ']'
                  << ",\"host_rng_after\":[" << random.state.seeds[0] << ','
                  << random.state.counters[0] << ']'
                  << ",\"ring_generated\":{\"item_id\":929,\"power_id\":"
                  << ring_power_id << ",\"property_rows\":"
                  << ring_power.properties.size() << ",\"property_type\":"
                  << (ring_power.properties.empty() ? -1 : ring_power.properties[0].type)
                  << ",\"property_value\":"
                  << (ring_power.properties.empty() ? 0 : ring_power.properties[0].value)
                  << ",\"gear_slot\":"
                  << (ring_inventory.equipment()[0][5] ? 5 : 6) << "} "
                  << ",\"adapter_cases\":[\"original powered AddLoot parity\","
                     "\"generated power to V4 equip to canonical Character gear PropertyView\","
                     "\"source QuestReward ring random power to V4 pickup/equip and combat view\","
                     "\"source powered AddLoot 841/193 through V4 world pickup/equip/property recalc\","
                     "\"live Loot property mismatch rejected\","
                     "\"unsupported caller arguments rejected\","
                     "\"missing required provider rejected\","
                     "\"reached provider failure preserves pending Item prefix\"]"
                  << ",\"source_scope\":\"original powered AddLoot and QuestReward ring generation followed through canonical V4 pickup/equip, Warrior PropertyView, and combat query; no PlayerSavegame lifecycle\""
                  << ",\"shared_rng_verified\":true"
                  << ",\"checks\":" << checks << "}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
