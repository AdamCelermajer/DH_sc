#include "../loot_pickup_event_bridge_v1.hpp"
#include "../current_level_quest_event_v1.hpp"
#include "../source_level_owner_v1.hpp"
#include "../../game-data/fresh_inventory_owned_v4.hpp"
#include "../../game-data/item_presentation_v5.hpp"
#include "../../game-data/loot_power_resources_v7.hpp"
#include "../../game-data/quest_gather_loot_receiver_v1.hpp"
#include "../../android-native/app/src/main/cpp/player_inventory_save_commit_v1.hpp"

extern "C" {
#include "../../random/random.h"
}

#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2;
using namespace dh2::data;
using namespace dh2::character::loot_pickup_event_bridge_v1;
namespace qevent = dh2::level_world::current_level_quest_event_v1;

namespace {
unsigned checks{};
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}
using Raw = std::vector<std::uint8_t>;
Raw file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("missing input: " + path);
    return {std::istreambuf_iterator<char>(input), {}};
}
Bytes bytes(const Raw& value) { return {value.data(), value.size()}; }
std::uint32_t word(const Raw& value, std::size_t offset) {
    if (offset > value.size() || value.size() - offset < 4)
        throw std::runtime_error("truncated source AddLoot fixture");
    std::uint32_t result{};
    std::memcpy(&result, value.data() + offset, 4);
    return result;
}
struct RandomOwner {
    dh2_random_state state{};
    explicit RandomOwner(const Raw& fixture)
        : state{{word(fixture, 8), 0x98765432u}, {word(fixture, 12), 37u}} {}
    static bool next(void* raw, std::int32_t bound, std::uint32_t stream,
                     std::int32_t& result, std::string& error) {
        auto& self = *static_cast<RandomOwner*>(raw);
        if (stream != 0) { error = "unexpected source RNG stream"; return false; }
        result = dh2_random_next(&self.state, std::uint32_t(bound), stream);
        return true;
    }
    InventoryRandomServiceV4 service() { return {this, next}; }
};
struct ItemServices {
    const ItemTable* items{};
    ItemPresentationOwnerV5* presentation{};
    static const Item* metadata(void* raw, const ItemInstanceV1& instance,
                                std::string& error) {
        auto& self = *static_cast<ItemServices*>(raw);
        const auto* row = item(*self.items, instance.id);
        if (!row) error = "source ItemTable row missing";
        return row;
    }
    static bool text(void*, ItemInstanceV1&, const ItemTextRequestV5& request,
                     ItemTextResponseV5& response, std::string& output,
                     std::string& error) {
        switch (request.operation) {
        case ItemTextOperationV5::constant: response.value = 17; return true;
        case ItemTextOperationV5::integer_string:
            if (request.value == -1) { error = "missing source text fixture"; return false; }
            response.text = "OID" + std::to_string(request.value); return true;
        case ItemTextOperationV5::class_name: response.value = request.value; return true;
        case ItemTextOperationV5::parse_varargs:
        case ItemTextOperationV5::parse_ex:
            if (request.input) output += request.input;
            for (std::uint32_t i = 0; i < request.count; ++i)
                output += ":" + std::to_string(request.arguments[i].integer);
            return true;
        }
        error = "unsupported Item text operation";
        return false;
    }
    static bool inventory(void* raw, FreshInventoryOwnedV4&,
                          const OwnedInventoryRequestV4& request,
                          OwnedInventoryResponseV4& response, std::string& error) {
        auto& self = *static_cast<ItemServices*>(raw);
        const ItemTextServicesV5 text{raw, metadata, ItemServices::text};
        switch (request.operation) {
        case OwnedInventoryOperationV4::update_name:
            return item_update_name_v5(*request.item, text, error);
        case OwnedInventoryOperationV4::update_stats:
            return item_update_stats_v5(*request.item, text, error);
        case OwnedInventoryOperationV4::update_requirements:
            return item_update_requirements_v5(*request.item, text, error);
        case OwnedInventoryOperationV4::add_power:
            return self.presentation->add_power(*request.item, request.argument,
                                                std::int32_t(request.index), text, error);
        case OwnedInventoryOperationV4::player_count: response.value = 1; return true;
        case OwnedInventoryOperationV4::debug_load:
        case OwnedInventoryOperationV4::debug_query: response.value = 0; return true;
        case OwnedInventoryOperationV4::current_player:
            response.identity = 0; response.value = 0; return true;
        case OwnedInventoryOperationV4::destroy_item:
            return self.presentation->forget(*request.item, error);
        default: error = "unbound AddLoot inventory continuation"; return false;
        }
    }
    static void observe(void* raw, FreshInventoryOwnedV4&,
                        const OwnedInventoryRequestV4& request) {
        if (request.operation == OwnedInventoryOperationV4::destroy_item) {
            auto& self = *static_cast<ItemServices*>(raw);
            std::string error;
            if (!self.presentation->forget(*request.item, error))
                throw std::runtime_error(error);
        }
    }
    ItemTextServicesV5 text_service() { return {this, metadata, text}; }
};
struct QuestContext {
    std::int32_t script_id{-1};
    unsigned script_calls{};
    static bool item_quantity(void* raw, std::int32_t id, bool& found,
                              std::int32_t& quantity, std::string& error) {
        auto& self = *static_cast<QuestContext*>(raw);
        if (!self.inventory->quest_gathering_item_quantity(id, found, quantity, error))
            return false;
        return true;
    }
    static bool start_script(void* raw, std::int32_t id, std::string&) {
        auto& self = *static_cast<QuestContext*>(raw);
        self.script_id = id;
        ++self.script_calls;
        return true;
    }
    FreshInventoryOwnedV4* inventory{};
};
struct PickupContext {
    const source_level_owner_v1::Owner* level{};
    FreshInventoryOwnedV4* inventory_owner{};
    ItemServices* item_services{};
    Services* services{};
    Result result{};
    unsigned calls{};
    std::vector<std::string>* order{};
    ItemInstanceV1* transferred_item{};
    static bool is_player(void* raw, std::uintptr_t character, bool& player,
                          std::string&) {
        const auto& self = *static_cast<PickupContext*>(raw);
        player = self.inventory_owner && self.inventory_owner->character() == character;
        return true;
    }
    static bool inventory_callback(void* raw, FreshInventoryOwnedV4& owner,
                                   const OwnedInventoryRequestV4& request,
                                   OwnedInventoryResponseV4& response, std::string& error) {
        auto& self = *static_cast<PickupContext*>(raw);
        return ItemServices::inventory(self.item_services, owner, request, response, error);
    }
    static void observe(void* raw, FreshInventoryOwnedV4& owner,
                        const OwnedInventoryRequestV4& request) {
        auto& self = *static_cast<PickupContext*>(raw);
        ItemServices::observe(self.item_services, owner, request);
    }
    static bool after_world_pickup(void* raw, std::uintptr_t character,
                                   std::int32_t item_id, std::string& error) {
        auto& self = *static_cast<PickupContext*>(raw);
        const auto& world = self.inventory_owner->world_items();
        const auto& carried = self.inventory_owner->items();
        const bool transferred = self.transferred_item && world.empty() &&
            std::any_of(carried.begin(), carried.end(), [&](const auto& slot) {
                return slot && slot->item.get() == self.transferred_item;
            });
        if (!transferred) {
            error = "GatherLoot tail ran before canonical V4 pickup committed";
            return false;
        }
        self.order->push_back("pickup-committed");
        return dh2::character::loot_pickup_event_bridge_v1::after_transfer(
            *self.level, *self.inventory_owner, character, item_id, *self.services,
            &self.result, error) == Status::raised;
    }
};
struct EventContext {
    qevent::Runtime* runtime{};
    std::uintptr_t owner{};
    std::vector<std::string>* order{};
    FreshInventoryOwnedV4* inventory{};
    unsigned raise_calls{};
    static bool is_player(void*, std::uintptr_t, bool& player, std::string&) {
        player = true;
        return true;
    }
    static bool raise(void* raw, std::uintptr_t owner,
                      const character::LootPickupQuestEventV10& source,
                      std::string& error) {
        auto& self = *static_cast<EventContext*>(raw);
        if (owner != self.owner) { error = "RaiseAsync wrong owner"; return false; }
        ++self.raise_calls;
        if (!self.inventory || self.inventory->items().empty() ||
            !self.inventory->world_items().empty()) {
            error = "GatherLoot RaiseAsync ran before pickup ownership transfer";
            return false;
        }
        self.order->push_back("qest-raised");
        auto event = source;
        qevent::Result result{};
        return self.runtime->raise(event, result, error) == qevent::Status::complete;
    }
};
}

int main(int argc, char** argv) {
    try {
        check(argc == 3, "expected powered AddLoot fixture and canonical cache directory");
        const auto fixture = file(argv[1]);
        const auto cache_dir = std::string(argv[2]) + "/";
        const char* names[] = {
            "item_powers_pyarray.bin", "item_powers_pyarraynames.bin",
            "item_powers_pystructnames.bin", "item_powers_monopoly_pyarray.bin",
            "item_powers_monopoly_pyarraynames.bin", "item_powers_monopoly_pystructnames.bin",
            "loot_table_pyarray.bin", "loot_table_pyarraynames.bin", "loot_table_pystructnames.bin"};
        std::array<Raw, 9> raw;
        for (std::size_t i = 0; i < raw.size(); ++i) raw[i] = file(cache_dir + names[i]);
        check(raw[6].size() == 284104, "unexpected source LootTable cache size");
        Raw quantities(raw[6].begin() + 283108, raw[6].end());
        std::string error;
        ItemPowerTablesV5 powers;
        check(powers.load(bytes(raw[0]), bytes(raw[1]), bytes(raw[2]), error), "load V5 powers");
        LootPowerInputsV7 inputs{bytes(raw[0]), bytes(raw[1]), bytes(raw[2]),
            bytes(raw[3]), bytes(raw[4]), bytes(raw[5]), bytes(quantities),
            bytes(raw[7]), bytes(raw[8])};
        LootPowerResourcesV7 resources;
        check(resources.load(inputs, powers.borrow(), error), "load V7 loot power resources");
        LootTablesV2 tables;
        check(tables.load(bytes(raw[6]), bytes(raw[7]), bytes(raw[8]), error), "load V2 loot table");
        constexpr std::uintptr_t character = 0x9001;
        RandomOwner random(fixture);
        const auto rng = random.service();
        LootPowerCreationV7 creation(resources.borrow(), rng);
        ItemPresentationOwnerV5 presentation(powers.borrow());
        const auto loot_view = tables.borrow();
        ItemServices item_services{&loot_view.items(), &presentation};
        const auto inventory_callbacks = OwnedInventoryServicesV4{
            &item_services, ItemServices::inventory, ItemServices::observe};
        const auto loot_effects = OwnedLootEffectsV7{
            &creation, powers.borrow(), item_services.text_service(),
            std::int32_t(word(fixture, 20)), std::int32_t(word(fixture, 24)),
            std::int32_t(word(fixture, 28)), std::int32_t(word(fixture, 32))};
        PropertyState properties{};
        FreshInventoryOwnedV4 inventory(character, tables.borrow(), rng,
                                        std::int8_t(word(fixture, 16)), properties);
        source_level_owner_v1::Owner level;
        int projection{}, fields{}, quest_owner{};
        check(level.begin_load() && level.request_level(1, 0, 0) &&
              level.begin_source_load() && level.publish_projection(&projection) &&
              level.bind_native_level({&fields, &quest_owner, 0x5001, character, 1, 0, 0, 38}),
              "bind active Level, canonical Save, Character and Quest owner");
        qevent::Runtime events;
        std::vector<std::string> order;
        const auto event_owner = reinterpret_cast<std::uintptr_t>(&quest_owner);
        EventContext event_context{&events, event_owner, &order, &inventory};
        Services bridge_services{&event_context, EventContext::is_player,
            event_owner, 37, EventContext::raise};

        // Runtime owner exists, but Objective_GatherLoot has not run Compile /
        // Register. The source Character list30 precondition must suppress the
        // event; a missing receiver must not mutate QEST or reach the save seam.
        Result unregistered_result{};
        check(after_transfer(level, inventory, character, 841, bridge_services,
              &unregistered_result, error) == Status::ignored &&
              !unregistered_result.registered_item && event_context.raise_calls == 0 &&
              order.empty(),
              "pickup emitted GatherLoot before canonical Objective Register");
        unsigned save_calls = 0;
        check(inventory.register_quest_gathering_item_id(841, error),
              "register objective item in same canonical V4 inventory");

        QuestContext quest_context{};
        quest_context.inventory = &inventory;
        bool found{};
        std::int32_t current_quantity{};
        check(inventory.quest_gathering_item_quantity(841, found, current_quantity, error),
              "query pre-pickup inventory quantity");
        data::quest_objective_factory_v1::Record objective(0x8001);
        objective.fields.character_10 = character;
        check(data::quest_gather_loot_receiver_v1::compile(
            objective, true, 841, 1, 42, found, current_quantity, &quest_context,
            QuestContext::start_script, error), "compile retained GatherLoot objective");
        auto gather = data::quest_gather_loot_receiver_v1::Binding{
            &objective, 37, 841, 1, 42, &quest_context, &quest_context,
            QuestContext::item_quantity, QuestContext::start_script};
        bool attached{};
        check(events.attach(37, 0x8001, 0, &gather,
              data::quest_gather_loot_receiver_v1::Binding::receive,
              attached, error) == qevent::Status::complete && attached,
              "attach GatherLoot receiver to the selected current-Level event runtime");
        PickupContext pickup{&level, &inventory, &item_services, &bridge_services};
        pickup.order = &order;
        auto pickup_services = inventory_callbacks;
        pickup_services.context = &pickup;
        pickup_services.invoke = PickupContext::inventory_callback;
        pickup_services.observe_storage = PickupContext::observe;
        pickup_services.after_world_pickup = PickupContext::after_world_pickup;

        std::unique_ptr<ItemInstanceV1> pending;
        LootEntrySelectionContextV1 selection{};
        check(inventory.add_world_loot_table(std::int32_t(word(fixture, 4)), selection, {&pending},
              inventory_callbacks, loot_effects, error), "generate enemy world loot through source LootTable");
        check(!pending && inventory.items().empty() && inventory.world_items().size() == 1,
              "generated drop must remain a world Item before popup pickup");
        auto* source_item = inventory.world_items()[0]->item.get();
        check(source_item && source_item->id == 841 && source_item->powers.size() == 1 &&
              source_item->powers[0] == 193, "source generated Item identity/power changed");
        pickup.transferred_item = source_item;
        std::int32_t inventory_index = -1;
        check(inventory.pickup_world_item(0, inventory_index, pickup_services, error),
              error.empty() ? "popup pickup and GatherLoot event failed" : error.c_str());
        check(inventory.world_items().empty() && inventory_index == 0 &&
              inventory.items().size() == 1 && inventory.items()[0]->item.get() == source_item,
              "popup pickup replaced or lost the generated source Item identity");
        if (!(pickup.result.status == Status::raised && objective.done_14 &&
              objective.quantity_20 == 1 && quest_context.script_calls == 1 &&
              quest_context.script_id == 42)) {
            throw std::runtime_error("GatherLoot state: status=" +
                std::to_string(static_cast<unsigned>(pickup.result.status)) +
                " done=" + std::to_string(objective.done_14) +
                " qty=" + std::to_string(objective.quantity_20) +
                " script_calls=" + std::to_string(quest_context.script_calls) +
                " script=" + std::to_string(quest_context.script_id));
        }
        ++checks;

        // The Android caller saves after V4 returns from its synchronous
        // GatherLoot tail. Exercise the selected commit adapter at that seam.
        check(dh2::native::player_inventory_save_commit_v1::after_successful_mutation(
            [&](std::string&) {
                if (!objective.done_14 || objective.quantity_20 != 1 ||
                    inventory.items().size() != 1 || !inventory.world_items().empty())
                    return false;
                ++save_calls;
                order.push_back("sg-save");
                return true;
            }, error), "post-pickup SG_Save commit boundary failed");
        check(event_context.raise_calls == 1 && save_calls == 1 &&
              order == std::vector<std::string>({"pickup-committed", "qest-raised", "sg-save"}),
              "pickup mutation, QEST dispatch, and SG_Save order changed");

        check(presentation.forget(*inventory.items()[0]->item, error),
              "retire generated Item presentation fixture");
        std::cout << "{\"validation\":\"PASS\",\"generated_item\":841,"
                     "\"generated_power\":193,\"same_item_after_popup_pickup\":true,"
                     "\"qest_gatherloot_completed\":true,"
                     "\"unregistered_objective_noop\":true,"
                     "\"order\":[\"pickup-committed\",\"qest-raised\",\"sg-save\"],"
                     "\"checks\":" << checks << "}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
