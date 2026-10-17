#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../player_gear_save_writer_v1.hpp"
#include "../player_saved_inventory_v1.hpp"
#include "../../game-data/player_gear_effects_v5.hpp"
#include "../../game-data/player_add_loot_v1.hpp"
#include "../../game-data/class_tables.hpp"

extern "C" {
#include "../../random/random.h"
}

#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;
namespace data = dh2::data;
namespace writer = dh2::player_gear_save_writer_v1;
namespace reader = dh2::player_saved_inventory_v1;
using Raw = std::vector<std::uint8_t>;

namespace {
unsigned checks = 0;
void require(bool condition, const char* message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}
Raw read_file(const fs::path& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    require(bool(in), "input file missing");
    const auto size = in.tellg();
    require(size >= 0, "input file size invalid");
    Raw result(static_cast<std::size_t>(size));
    in.seekg(0);
    require(bool(in.read(reinterpret_cast<char*>(result.data()),
                         std::streamsize(result.size()))), "input truncated");
    return result;
}
void write_file(const fs::path& path, const Raw& bytes) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    require(bool(out.write(reinterpret_cast<const char*>(bytes.data()),
                           std::streamsize(bytes.size()))), "output write failed");
}
data::Bytes bytes(const Raw& v) { return {v.data(), v.size()}; }

struct RandomOwner {
    dh2_random_state state{{1, 0x98765432u}, {0, 37u}};
    static bool next(void* raw, std::int32_t bound, std::uint32_t stream,
                     std::int32_t& value, std::string& error) {
        auto& self = *static_cast<RandomOwner*>(raw);
        if (stream != 0 || bound <= 0) {
            error = "unexpected source RNG request";
            return false;
        }
        value = dh2_random_next(&self.state, std::uint32_t(bound), stream);
        return true;
    }
    data::InventoryRandomServiceV4 service() { return {this, next}; }
};

struct InventoryServices {
    const data::ItemTable* item_table{};
    data::ItemPresentationOwnerV5* presentation{};
    data::PlayerGearEffectsV5* gear{};
    std::uintptr_t visual{};

    static const data::Item* metadata(void* raw, const data::ItemInstanceV1& item,
                                      std::string& error) {
        auto& self = *static_cast<InventoryServices*>(raw);
        const auto* row = data::item(*self.item_table, item.id);
        if (!row) error = "missing Item row";
        return row;
    }
    static bool text(void*, data::ItemInstanceV1&,
                     const data::ItemTextRequestV5& request,
                     data::ItemTextResponseV5& response, std::string& output,
                     std::string& error) {
        switch (request.operation) {
        case data::ItemTextOperationV5::constant:
            response.value = 17; return true;
        case data::ItemTextOperationV5::integer_string:
            if (request.value < 0) { error = "missing integer text"; return false; }
            response.text = "OID" + std::to_string(request.value); return true;
        case data::ItemTextOperationV5::class_name:
            response.value = request.value; return true;
        case data::ItemTextOperationV5::parse_varargs:
        case data::ItemTextOperationV5::parse_ex:
            if (request.input) output += request.input;
            for (std::uint32_t i = 0; i < request.count; ++i)
                output += ":" + std::to_string(request.arguments[i].integer);
            return true;
        }
        error = "unsupported Item text request";
        return false;
    }
    data::ItemTextServicesV5 text_services() { return {this, metadata, text}; }
    static bool invoke(void* raw, data::FreshInventoryOwnedV4&,
                       const data::OwnedInventoryRequestV4& request,
                       data::OwnedInventoryResponseV4& response,
                       std::string& error) {
        auto& self = *static_cast<InventoryServices*>(raw);
        const auto text = self.text_services();
        switch (request.operation) {
        case data::OwnedInventoryOperationV4::update_name:
            return data::item_update_name_v5(*request.item, text, error);
        case data::OwnedInventoryOperationV4::update_stats:
            return data::item_update_stats_v5(*request.item, text, error);
        case data::OwnedInventoryOperationV4::update_requirements:
            return data::item_update_requirements_v5(*request.item, text, error);
        case data::OwnedInventoryOperationV4::add_power:
            return self.presentation->add_power(*request.item, request.argument,
                std::int32_t(request.index), text, error);
        case data::OwnedInventoryOperationV4::debug_load:
        case data::OwnedInventoryOperationV4::full_notifications:
        case data::OwnedInventoryOperationV4::gold_notifications:
            return true;
        case data::OwnedInventoryOperationV4::debug_query:
        case data::OwnedInventoryOperationV4::inventory_full:
            response.value = 0; return true;
        case data::OwnedInventoryOperationV4::player_count:
            response.value = 1; return true;
        case data::OwnedInventoryOperationV4::update_gear_properties:
            return self.gear && self.gear->update_properties(error);
        case data::OwnedInventoryOperationV4::validate_hp_mp:
            return self.gear && self.gear->validate_hp_mp(error);
        case data::OwnedInventoryOperationV4::skin:
            return self.gear && self.gear->update_skin(&self.visual, {}, error);
        default:
            error = "unexpected V4 owner callback " +
                std::to_string(std::uint32_t(request.operation));
            return false;
        }
    }
    static void observe(void* raw, data::FreshInventoryOwnedV4&,
                        const data::OwnedInventoryRequestV4& request) {
        if (request.operation != data::OwnedInventoryOperationV4::destroy_item)
            return;
        auto& self = *static_cast<InventoryServices*>(raw);
        std::string error;
        if (!self.presentation->forget(*request.item, error))
            throw std::runtime_error(error);
    }
    data::OwnedInventoryServicesV4 binding() {
        return {this, invoke, observe, false};
    }
};

struct Inputs {
    Raw loot_records, loot_names, loot_schema;
    Raw power_records, power_names, power_schema;
    Raw monopoly_records, monopoly_names, monopoly_schema;
    Raw character_records, character_names, character_schema;
    Raw class_records, class_names, class_schema;
    data::LootTablesV2 loot;
    data::ItemPowerTablesV5 powers;
    data::LootPowerResourcesV7 loot_power;
    data::CharacterTable characters;
    data::PropertyRules rules;
    data::ClassTables classes;
    std::vector<data::ClassRow> class_rows;

    explicit Inputs(const fs::path& cache, const fs::path& character_data) {
        auto get = [](const fs::path& dir, const char* stem, const char* suffix) {
            return read_file(dir / (std::string(stem) + suffix + ".bin"));
        };
        loot_records = get(cache, "loot_table", "_pyarray");
        loot_names = get(cache, "loot_table", "_pyarraynames");
        loot_schema = get(cache, "loot_table", "_pystructnames");
        power_records = get(cache, "item_powers", "_pyarray");
        power_names = get(cache, "item_powers", "_pyarraynames");
        power_schema = get(cache, "item_powers", "_pystructnames");
        monopoly_records = get(cache, "item_powers_monopoly", "_pyarray");
        monopoly_names = get(cache, "item_powers_monopoly", "_pyarraynames");
        monopoly_schema = get(cache, "item_powers_monopoly", "_pystructnames");
        character_records = get(character_data, "character_properties", "_pyarray");
        character_names = get(character_data, "character_properties", "_pyarraynames");
        character_schema = get(character_data, "character_properties", "_pystructnames");
        class_records = get(character_data, "character_classes", "_pyarray");
        class_names = get(character_data, "character_classes", "_pyarraynames");
        class_schema = get(character_data, "character_classes", "_pystructnames");
        std::string error;
        require(loot.load(bytes(loot_records), bytes(loot_names),
                          bytes(loot_schema), error), "LootTable cache load failed");
        require(powers.load(bytes(power_records), bytes(power_names),
                            bytes(power_schema), error), "ItemPower cache load failed");
        data::Bytes quantities;
        const auto loot_view = loot.borrow();
        require(data::select_source_quantity_array_v7(bytes(loot_records),
                    bytes(loot_names), loot_view.consumed(), quantities, error),
                "source NumProbArray selection failed");
        const data::LootPowerInputsV7 power_input{
            bytes(power_records), bytes(power_names), bytes(power_schema),
            bytes(monopoly_records), bytes(monopoly_names), bytes(monopoly_schema),
            quantities, bytes(loot_names), bytes(loot_schema)};
        require(loot_power.load(power_input, powers.borrow(), error),
                "LootPower resources load failed");
        require(data::load_characters(bytes(character_records), bytes(character_names),
                    bytes(character_schema), characters, error),
                "Character table load failed");
        require(data::load_property_rules(characters, rules, error),
                "Character property rules load failed");
        require(data::load_classes(bytes(class_records), bytes(class_names),
                    bytes(class_schema), classes, error), "class table load failed");
        for (const auto& row : classes.rows)
            class_rows.push_back({row.data(), std::uint32_t(row.size())});
    }
};

constexpr std::uintptr_t character_id = UINT64_C(0x100000007);
constexpr std::uint32_t warrior_row = 263;

struct Graph {
    data::PropertyState properties{};
    data::PropertyView view{};
    std::unique_ptr<data::PlayerSavegameV1> save;
    std::unique_ptr<data::FreshInventoryOwnedV4> inventory;
    std::unique_ptr<data::ItemPresentationOwnerV5> presentation;
    std::unique_ptr<data::PlayerGearEffectsV5> gear;
    std::unique_ptr<InventoryServices> services;
    data::OwnedInventoryServicesV4 callbacks{};

    Graph(Inputs& in, std::uintptr_t identity,
          data::InventoryRandomServiceV4 rng, std::int8_t capacity) {
        require(in.characters.rows.size() > warrior_row,
                "Warrior property row missing");
        data::reset_properties(in.rules, properties,
                               &in.characters.rows[warrior_row]);
        view = data::property_view(in.rules, properties);
        require(dh2_class_recalc_base(in.class_rows.data(),
                    std::uint32_t(in.class_rows.size()), properties.base.data(),
                    &view) == 0, "base Warrior properties failed");
        save = std::make_unique<data::PlayerSavegameV1>();
        save->set_character(identity);
        inventory = std::make_unique<data::FreshInventoryOwnedV4>(identity,
            in.loot.borrow(), rng, capacity, properties);
        presentation = std::make_unique<data::ItemPresentationOwnerV5>(in.powers.borrow());
        gear = std::make_unique<data::PlayerGearEffectsV5>(*inventory, view,
            in.class_rows.data(), std::uint32_t(in.class_rows.size()), in.powers.borrow());
        services = std::make_unique<InventoryServices>();
        services->item_table = &inventory->table();
        services->presentation = presentation.get();
        services->gear = gear.get();
        callbacks = services->binding();
    }
};

void verify_source_fixture(const fs::path& fixture, std::uint32_t& seed,
                           std::uint32_t& calls, std::int32_t& loot_id,
                           std::int32_t& value_bonus, std::int32_t& power_bonus,
                           std::int32_t& requested, std::int32_t& difficulty) {
    const auto b = read_file(fixture);
    require(b.size() >= 44, "powered AddLoot fixture header truncated");
    auto word = [&](std::size_t at) {
        return std::uint32_t(b[at]) | std::uint32_t(b[at + 1]) << 8 |
            std::uint32_t(b[at + 2]) << 16 | std::uint32_t(b[at + 3]) << 24;
    };
    require(word(0) == 0x37564c41u, "powered AddLoot fixture magic mismatch");
    loot_id = std::int32_t(word(4)); seed = word(8); calls = word(12);
    value_bonus = std::int32_t(word(20)); power_bonus = std::int32_t(word(24));
    requested = std::int32_t(word(28)); difficulty = std::int32_t(word(32));
    require(loot_id == 5 && seed == 1 && calls == 0 && value_bonus == 0 &&
            power_bonus == 0 && requested == 1 && difficulty == 0,
            "fixture is not the source row-5 powered AddLoot case");
}

Raw generate(const fs::path& cache, const fs::path& character_data,
             const fs::path& fixture, const fs::path& payload_path) {
    Inputs in(cache, character_data);
    std::uint32_t seed{}, calls{};
    std::int32_t loot_id{}, value_bonus{}, power_bonus{}, requested{}, difficulty{};
    verify_source_fixture(fixture, seed, calls, loot_id, value_bonus,
                          power_bonus, requested, difficulty);
    RandomOwner random; random.state.seeds[0] = seed; random.state.counters[0] = calls;
    const auto rng = random.service();
    Graph graph(in, character_id, rng, 12);
    graph.properties.resolved[9] = loot_id;
    data::LootPowerCreationV7 creation(in.loot_power.borrow(), rng);
    data::LootEntrySelectionContextV1 selection{};
    const data::OwnedLootEffectsV7 effects{&creation, in.powers.borrow(),
        graph.services->text_services(), value_bonus, power_bonus, requested, difficulty};
    std::unique_ptr<data::ItemInstanceV1> pending;
    std::string error;
    require(graph.inventory->add_world_loot_table(loot_id, selection, {&pending},
                graph.callbacks, effects, error),
            "source world AddLoot failed");
    require(!pending && graph.inventory->world_items().size() == 1,
            "AddLoot did not stage exactly one powered world Item");
    auto* generated_item = graph.inventory->world_items()[0]->item.get();
    require(generated_item && generated_item->id == 841 &&
            generated_item->powers == std::vector<std::int32_t>{193},
            "source row-5 AddLoot result differs from Item 841/power 193");
    std::int32_t index = -1;
    require(graph.inventory->pickup_world_item(0, index, graph.callbacks, error) &&
            index == 0 && graph.inventory->world_items().empty() &&
            graph.inventory->items().size() == 1 &&
            graph.inventory->items()[0]->item.get() == generated_item,
            "V4 pickup did not transfer the exact generated Item");
    std::int32_t equipped = 0;
    require(graph.inventory->character_auto_equip(0, equipped, graph.callbacks, error) &&
            equipped == 1 && graph.inventory->equipment()[0][3] ==
                graph.inventory->items()[0].get(),
            "V4 AutoEquip did not equip powered boots in slot 3");
    const auto power = in.powers.borrow().rows().at(193);
    require(power.properties.size() == 1 && power.properties[0].type == 2 &&
            graph.properties.gear[150] == power.properties[0].value,
            "source AddLoot power did not reach live gear property 150");

    Raw payload(4096);
    writer::Result written{};
    require(writer::save({graph.save.get(), graph.inventory.get(), in.powers.borrow()},
                {payload.data(), payload.size()}, &written, error) == writer::Status::complete,
            "canonical GEAR serializer rejected live V4 inventory");
    payload.resize(written.written);
    write_file(payload_path, payload);
    require(random.state.seeds[0] == 1302343 && random.state.counters[0] == 2,
            "AddLoot RNG receipt differs from original row-5 fixture");
    std::cout << "{\"validation\":\"PASS\",\"phase\":\"generate\","
              << "\"item_id\":841,\"power_id\":193,\"gear_property_150\":"
              << graph.properties.gear[150] << ",\"gear_bytes\":" << payload.size()
              << ",\"save_owner_count\":1}\n";
    return payload;
}

void cold_open(const fs::path& cache, const fs::path& character_data,
               const fs::path& payload_path) {
    Inputs in(cache, character_data);
    const auto payload = read_file(payload_path);
    Graph graph(in, character_id, {nullptr, nullptr}, 12);
    const auto before = graph.properties.resolved;
    std::unique_ptr<data::ItemInstanceV1> incoming;
    reader::Bindings bindings{graph.save.get(), graph.inventory.get(), &graph.view,
        &graph.callbacks, in.powers.borrow(), &incoming};
    reader::Runtime runtime(bindings);
    reader::Result loaded{};
    std::string error;
    require(runtime.load(bytes(payload), &loaded, error) == reader::Status::complete,
            "cold GEAR reader failed");
    require(loaded.completed_items == 1 && loaded.completed_powers == 1 &&
            loaded.equips == 1 && loaded.item_id == 841 && loaded.power_id == 193 &&
            !incoming && graph.inventory->items().size() == 1 &&
            graph.inventory->items()[0]->item->powers ==
                std::vector<std::int32_t>{193} && graph.inventory->equipment()[0][3] ==
                graph.inventory->items()[0].get(),
            "cold GEAR reader did not restore the equipped powered V4 Item");
    const auto* presentation = graph.presentation->powers(
        *graph.inventory->items()[0]->item);
    require(presentation && presentation->size() == 1 &&
            presentation->at(0).id == 193,
            "cold GEAR AddPower did not rebuild the presentation owner");
    const auto power = in.powers.borrow().rows().at(193);
    require(graph.gear->update_properties(error),
            "cold equipment/property recalculation failed");
    require(power.properties.size() == 1 && power.properties[0].type == 2 &&
            graph.properties.gear[150] == power.properties[0].value &&
            graph.properties.resolved[150] != before[150],
            "cold GEAR reopen lost power 193's property 150 equipment effect");
    require(graph.save->character() == graph.inventory->character() &&
            graph.save->character() == character_id,
            "cold GEAR reader crossed the one canonical Save/Character owner");
    std::cout << "{\"validation\":\"PASS\",\"phase\":\"cold_open\","
              << "\"item_id\":841,\"power_id\":193,\"equipped_slot\":3,"
              << "\"gear_property_150\":" << graph.properties.gear[150]
              << ",\"save_owner_count\":1}\n";
}
} // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 6, "expected mode, cache, character data, fixture, and payload path");
        const std::string mode = argv[1];
        const fs::path cache(argv[2]), character_data(argv[3]), fixture(argv[4]),
            payload(argv[5]);
        if (mode == "generate") generate(cache, character_data, fixture, payload);
        else if (mode == "cold-open") cold_open(cache, character_data, payload);
        else throw std::runtime_error("unknown phase");
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "{\"validation\":\"FAIL\",\"error\":\"" << error.what()
                  << "\",\"checks\":" << checks << "}\n";
        return 1;
    }
}
