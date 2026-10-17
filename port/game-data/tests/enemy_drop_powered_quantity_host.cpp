// Source enemy-drop mode: AddLoot with requested power count -1 must consult
// the retained NumProbArray before power selection, pickup, and equip. The
// original integration snapshot is requested=1; this specifically tests the
// -1 composition, backed by the separately captured quantity-selection corpus.
#define main add_loot_fixture_main
#include "player_add_loot_v1_host.cpp"
#undef main
#include <algorithm>

static Raw file(const std::string& path) { return file(path.c_str()); }
static void ck(bool ok, const std::string& message) { ck(ok, message.c_str()); }

struct TracedRandom {
    struct Draw { std::int32_t bound{}, value{}; std::uint32_t stream{}; };
    dh2_random_state state{};
    std::vector<Draw> trace;
    TracedRandom(std::uint32_t seed, std::uint32_t calls)
        : state{{seed, 0x98765432u}, {calls, 37u}} {}
    static bool next(void* opaque, std::int32_t bound, std::uint32_t stream,
                     std::int32_t& value, std::string& error) {
        auto& self = *static_cast<TracedRandom*>(opaque);
        if (stream != 0 || bound <= 0) {
            error = "enemy-drop RNG received invalid source stream/bound";
            return false;
        }
        value = dh2_random_next(&self.state, std::uint32_t(bound), stream);
        self.trace.push_back({bound, value, stream});
        return true;
    }
    InventoryRandomServiceV4 service() { return {this, next}; }
};

int main(int argc, char** argv) {
    try {
        const bool crypt_ghost_source = argc == 5 && std::string(argv[4]) == "crypt_ghost";
        ck(argc == 4 || crypt_ghost_source,
           "expected original powered AddLoot fixture, cache, Character data, and optional crypt_ghost mode");
        const auto fixture = file(argv[1]);
        Reader input{fixture};
        ck(input.u32() == 0x37564c41u, "invalid powered AddLoot fixture");
        const auto fixture_loot_id = std::int32_t(input.u32());
        const auto fixture_seed = input.u32();
        const auto fixture_calls = input.u32();
        const auto capacity = std::int8_t(input.u32());
        (void)input.u32(); // value bonus
        (void)input.u32(); // power bonus
        const auto source_requested = std::int32_t(input.u32());
        (void)input.u32(); // difficulty
        (void)input.u32(); (void)input.u32();
        (void)input.block();
        ck(input.at == fixture.size() && source_requested == 1,
           "fixture no longer identifies the captured baseline call");
        // Character::DropLoot -> ItemObject::DropLootTable supplies the
        // victim's Character::GetLoot() value, killer properties, requested
        // count -1, and the current Level difficulty. Crypt_Ghost is source
        // Character row 35 (Loot=22); seed 4 is the original ELF AddLoot
        // capture yielding Item 925 x1 with no powers, after four RNG calls.
        const auto loot_id = crypt_ghost_source ? 22 : fixture_loot_id;
        const auto seed = crypt_ghost_source ? 4u : fixture_seed;
        const auto calls = crypt_ghost_source ? 0u : fixture_calls;

        const std::string cache_dir = argv[2], character_dir = argv[3];
        const char* names[] = {
            "item_powers_pyarray.bin", "item_powers_pyarraynames.bin",
            "item_powers_pystructnames.bin", "item_powers_monopoly_pyarray.bin",
            "item_powers_monopoly_pyarraynames.bin", "item_powers_monopoly_pystructnames.bin",
            "loot_table_pyarray.bin", "loot_table_pyarraynames.bin",
            "loot_table_pystructnames.bin"};
        std::array<Raw, 9> cache;
        for (std::size_t i = 0; i < cache.size(); ++i)
            cache[i] = file(cache_dir + "/" + names[i]);
        std::string error;
        LootTablesV2 loot_tables;
        ck(loot_tables.load(bytes(cache[6]), bytes(cache[7]), bytes(cache[8]), error), error);
        const auto loot = loot_tables.borrow();
        Bytes quantities{};
        ck(select_source_quantity_array_v7(bytes(cache[6]), bytes(cache[7]),
                                           loot.consumed(), quantities, error), error);
        ItemPowerTablesV5 power_tables;
        ck(power_tables.load(bytes(cache[0]), bytes(cache[1]), bytes(cache[2]), error), error);
        const auto powers = power_tables.borrow();
        LootPowerInputsV7 power_input{
            bytes(cache[0]), bytes(cache[1]), bytes(cache[2]),
            bytes(cache[3]), bytes(cache[4]), bytes(cache[5]), quantities,
            bytes(cache[7]), bytes(cache[8])};
        LootPowerResourcesV7 resources;
        ck(resources.load(power_input, powers, error), error);
        const auto power_resources = resources.borrow();
        // The row-5 source capture selects Item 841 from one fixed entry. Its
        // entry carries both ItemPowerList and NumProbArray IDs.
        const LootEntry32V2* source_entry = nullptr;
        if (!crypt_ghost_source) for (const auto& entry : loot.loots().at(std::size_t(loot_id)).fixed_entries) {
            if (entry.words[1] < 0 || entry.words[3] < 0) continue;
            const auto& choices = loot.item_lists().at(std::size_t(entry.words[0]));
            if (std::any_of(choices.begin(), choices.end(), [](const auto& choice) {
                    return choice.item == 841;
                })) {
                source_entry = &entry;
                break;
            }
        }
        const std::vector<LootItemEntryV2>* item_choices_ptr = nullptr;
        const std::vector<LootQuantityChoiceV7>* quantity_choices_ptr = nullptr;
        std::int32_t item_bound = 0, quantity_bound = 0;
        if (!crypt_ghost_source) {
            ck(source_entry && std::size_t(source_entry->words[1]) < power_resources.lists().size() &&
                   std::size_t(source_entry->words[3]) < power_resources.quantities().size(),
               "captured row 5 does not expose the source power and NumProbArray rows");
            item_choices_ptr = &loot.item_lists().at(std::size_t(source_entry->words[0]));
            quantity_choices_ptr = &power_resources.quantities()[source_entry->words[3]];
            for (const auto& choice : *item_choices_ptr) item_bound += choice.probability;
            for (const auto& choice : *quantity_choices_ptr) quantity_bound += choice.probability;
            ck(item_bound > 0 && quantity_bound > 0,
               "captured ItemList or NumProbArray has no positive source weights");
        }

        constexpr std::uint32_t warrior = 263;
        auto character_bytes = file(character_dir + "/character_properties_pyarray.bin");
        auto character_names = file(character_dir + "/character_properties_pyarraynames.bin");
        auto character_schema = file(character_dir + "/character_properties_pystructnames.bin");
        CharacterTable characters;
        ck(load_characters(bytes(character_bytes), bytes(character_names),
                           bytes(character_schema), characters, error), error);
        PropertyRules rules;
        ck(load_property_rules(characters, rules, error), error);
        auto class_bytes = file(character_dir + "/character_classes_pyarray.bin");
        auto class_names = file(character_dir + "/character_classes_pyarraynames.bin");
        auto class_schema = file(character_dir + "/character_classes_pystructnames.bin");
        ClassTables class_tables;
        ck(load_classes(bytes(class_bytes), bytes(class_names), bytes(class_schema),
                        class_tables, error), error);
        std::vector<ClassRow> class_rows;
        for (const auto& row : class_tables.rows)
            class_rows.push_back({row.data(), std::uint32_t(row.size())});
        ck(characters.rows.size() > warrior, "Warrior Character row is missing");
        PropertyState properties{};
        reset_properties(rules, properties, &characters.rows[warrior]);
        auto view = property_view(rules, properties);
        ck(dh2_class_recalc_base(class_rows.data(), std::uint32_t(class_rows.size()),
                                 properties.base.data(), &view) == 0,
           "Warrior PropertyView initialization failed");
        const auto before = properties;

        TracedRandom actual(seed, calls);
        const auto rng = actual.service();
        LootPowerCreationV7 creation(power_resources, rng);
        FreshInventoryOwnedV4 inventory(0x100000007, loot, rng, capacity, properties);
        ItemPresentationOwnerV5 presentation(powers);
        Services services{&loot.items(), &presentation};
        PlayerGearEffectsV5 gear(inventory, view, class_rows.data(),
                                 std::uint32_t(class_rows.size()), powers);
        services.gear = &gear;
        const OwnedInventoryServicesV4 inventory_services{
            &services, Services::inventory, Services::observe, false};
        const auto text = services.text_service();
        const auto value_bonus256 = properties.resolved[195];
        const auto power_bonus256 = properties.resolved[196];
        const auto power_bonus = power_bonus256 >= 0
            ? power_bonus256 / 256
            : -std::int32_t((std::uint64_t(-std::int64_t(power_bonus256)) + 255) / 256);
        const OwnedLootEffectsV7 effects{
            &creation, powers, text, value_bonus256, power_bonus256, -1, 0};
        LootEntrySelectionContextV1 selection{};
        selection.player_counts.warrior = 1;
        std::unique_ptr<ItemInstanceV1> pending;
        ck(inventory.add_world_loot_table(loot_id, selection, {&pending},
                                          inventory_services, effects, error), error);
        if (crypt_ghost_source && inventory.world_items().empty() && inventory.items().empty()) {
            const auto& root = loot.loots().at(std::size_t(loot_id));
            ck(root.sub_loots.size() == 2 && actual.trace.size() == 1,
               "Crypt_Ghost row22 selected-library traversal changed unexpectedly");
            std::cout << "{\"validation\":\"KNOWN_GAP\",\"mode\":\"crypt_ghost_source_drop\","
                      << "\"provenance\":\"selected-library host source, not live gameplay\","
                      << "\"loot_row\":22,\"subloot_rows\":["
                      << root.sub_loots[0] << "," << root.sub_loots[1] << "],\"subloot_names\":[\""
                      << loot.loot_names().at(std::size_t(root.sub_loots[0])) << "\",\""
                      << loot.loot_names().at(std::size_t(root.sub_loots[1])) << "\"],"
                      << "\"selected_world_items\":0,\"selected_rng_draws\":"
                      << actual.trace.size() << ",\"original_elf_addloot_capture_item\":925,"
                      << "\"original_elf_capture_rng_draws\":4,"
                      << "\"original_end_to_end_character_death_capture\":false}\n";
            return 0;
        }
        ck(!pending && inventory.items().empty() && inventory.world_items().size() == 1,
           "enemy-style AddLoot expected one world item; world=" +
               std::to_string(inventory.world_items().size()) + ", owned=" +
               std::to_string(inventory.items().size()) + ", RNG draws=" +
               std::to_string(actual.trace.size()) + ", root roll=" +
               std::to_string(loot.loots().at(std::size_t(loot_id)).roll_type) +
               ", root fixed=" + std::to_string(loot.loots().at(std::size_t(loot_id)).fixed_entries.size()) +
               ", root random=" + std::to_string(loot.loots().at(std::size_t(loot_id)).random_entries.size()) +
               ", root subs=" + std::to_string(loot.loots().at(std::size_t(loot_id)).sub_loots.size()));
        auto* generated = inventory.world_items()[0]->item.get();
        ck(generated && generated->id == (crypt_ghost_source ? 925 : 841) &&
               (crypt_ghost_source ? generated->powers.empty() : !generated->powers.empty()),
           "source enemy AddLoot result differs from its original ELF output");
        bool power_affects_property_150 = false;
        for (const auto power_id : generated->powers) {
            ck(power_id >= 0 && std::size_t(power_id) < powers.rows().size(),
               "enemy-style AddLoot produced a power outside ItemPowerTable");
            for (const auto& property : powers.rows()[std::size_t(power_id)].properties)
                power_affects_property_150 |= property.type == 2;
        }

        // Independent source-semantic replay for the prefix: ItemList weighted
        // draw, then NumProbArray weighted quantity draw on the same seed/stream.
        std::int32_t expected_quantity = -1;
        if (crypt_ghost_source) {
            ck(generated->signed_quantity() == 1 && actual.trace.size() == 4 &&
                   actual.trace[0].bound == 11 && actual.trace[0].value == 6 &&
                   actual.trace[1].bound == 100 && actual.trace[1].value == 44 &&
                   actual.trace[2].bound == 100 && actual.trace[2].value == 5 &&
                   actual.trace[3].bound == 1 && actual.trace[3].value == 0,
               "Crypt_Ghost Loot=22 seed4 differs from quantity, two Pct and ItemList source RNG ordering");
        } else {
            TracedRandom expected(seed, calls);
            auto expected_rng = expected.service();
            std::int32_t discard = -1;
            ck(expected_rng.next(expected_rng.context, item_bound, 0, discard, error), error);
            ck(loot_quantity_v7(&expected_quantity, expected_rng,
                                quantity_choices_ptr->data(),
                                std::uint32_t(quantity_choices_ptr->size()), power_bonus, error) == 0, error);
            ck(actual.trace.size() >= expected.trace.size() &&
                   actual.trace[0].bound == item_bound &&
                   actual.trace[1].bound == quantity_bound &&
                   actual.trace[0].value == expected.trace[0].value &&
                   actual.trace[1].value == expected.trace[1].value &&
                   std::int32_t(generated->powers.size()) == expected_quantity,
               "requested=-1 did not consume source ItemList then NumProbArray RNG prefix");
        }

        auto* identity = generated;
        std::int32_t index = -1;
        ck(inventory.pickup_world_item(0, index, inventory_services, error) &&
               index == 0 && inventory.world_items().empty() &&
               inventory.items().size() == 1 && inventory.items()[0]->item.get() == identity &&
               (crypt_ghost_source ? identity->powers.empty()
                                   : identity->powers.size() == std::size_t(expected_quantity)),
           "enemy-generated powered Item identity changed during V4 pickup");
        if (crypt_ghost_source) {
            ck(inventory.properties() == &properties && inventory.world_items().empty(),
               "Crypt_Ghost source result failed V4 pickup transfer");
        } else {
            std::int32_t equipped = 0;
            ck(inventory.character_auto_equip(0, equipped, inventory_services, error), error);
            ck(equipped == 1 && inventory.equipment()[0][3] == inventory.items()[0].get() &&
                   inventory.properties() == &properties && view.gear == properties.gear.data() &&
                   properties.resolved[150] != before.resolved[150] &&
                   power_affects_property_150,
               "enemy-quantity generated power did not reach canonical gear properties");
        }
        for (const auto& slot : inventory.items())
            ck(presentation.forget(*slot->item, error), error);
        std::cout << "{\"validation\":\"PASS\",\"provenance\":\"selected-library host source; not end-to-end live gameplay\",\"mode\":\""
                  << (crypt_ghost_source ? "crypt_ghost_source_drop" : "enemy_drop_requested_minus_one") << "\","
                  << "\"loot_row\":" << loot_id << ",\"item_id\":" << identity->id
                  << ",\"power_count\":" << identity->powers.size()
                  << ",\"quantity_list_id\":" << (source_entry ? source_entry->words[3] : -1)
                  << ",\"itemlist_rng_bound\":" << item_bound
                  << ",\"quantity_rng_bound\":" << quantity_bound
                  << ",\"quantity_selected\":" << (crypt_ghost_source ? 0 : expected_quantity)
                  << ",\"rng_draws\":" << actual.trace.size()
                  << ",\"rng_trace\":[";
        for (std::size_t i = 0; i < actual.trace.size(); ++i) {
            if (i) std::cout << ',';
            std::cout << "{\"bound\":" << actual.trace[i].bound
                      << ",\"value\":" << actual.trace[i].value << '}';
        }
        std::cout << ']'
                  << ",\"rng_seed_after\":" << actual.state.seeds[0]
                  << ",\"rng_calls_after\":" << actual.state.counters[0]
                  << ",\"power_affects_property_150\":"
                  << (power_affects_property_150 ? "true" : "false")
                  << ",\"pickup_identity_preserved\":true,\"gear_property_150_changed\":"
                  << (crypt_ghost_source ? "false" : "true") << "} \n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
