#include "../loot_pickup_event_bridge_v1.hpp"
#include "../current_level_quest_event_v1.hpp"
#include "../../game-data/quest_gather_loot_receiver_v1.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2;
using namespace dh2::character::loot_pickup_event_bridge_v1;

namespace {
unsigned checks{};
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}
std::vector<std::uint8_t> file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    check(bool(input), "cache input unavailable");
    return {std::istreambuf_iterator<char>(input), {}};
}
struct GatherContext {
    std::int32_t quantity{1}, script_id{-1};
    unsigned script_calls{};
    static bool item_quantity(void* raw, std::int32_t id, bool& found,
                              std::int32_t& quantity, std::string&) {
        auto& self = *static_cast<GatherContext*>(raw);
        if (id != 456) return false;
        found = true;
        quantity = self.quantity;
        return true;
    }
    static bool start_script(void* raw, std::int32_t id, std::string&) {
        auto& self = *static_cast<GatherContext*>(raw);
        self.script_id = id;
        ++self.script_calls;
        return true;
    }
};
struct RaiseContext {
    level_world::current_level_quest_event_v1::Runtime* events{};
    std::uintptr_t event_owner{};
    unsigned calls{}, player_queries{};
    character::LootPickupQuestEventV10 delivered{};
    static bool is_player(void* raw, std::uintptr_t character, bool& result,
                          std::string&) {
        auto& self = *static_cast<RaiseContext*>(raw);
        ++self.player_queries;
        result = character == 0x9001;
        return true;
    }
    static bool raise(void* raw, std::uintptr_t event_owner,
                      const character::LootPickupQuestEventV10& source,
                      std::string& error) {
        auto& self = *static_cast<RaiseContext*>(raw);
        if (!self.events || event_owner != self.event_owner) {
            error = "wrong Quest EventManager owner";
            return false;
        }
        auto event = source;
        level_world::current_level_quest_event_v1::Result result{};
        ++self.calls;
        if (self.events->raise(event, result, error) !=
            level_world::current_level_quest_event_v1::Status::complete) return false;
        self.delivered = event;
        return true;
    }
};
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "expected cache directory");
        const auto root = std::string(argv[1]) + "/";
        auto records = file(root + "loot_table_pyarray.bin");
        auto names = file(root + "loot_table_pyarraynames.bin");
        auto schema = file(root + "loot_table_pystructnames.bin");
        data::LootTablesV2 table;
        std::string error;
        check(table.load({records.data(), records.size()}, {names.data(), names.size()},
                         {schema.data(), schema.size()}, error), "load actual ItemTable");

        data::LootRandom8V2 rng{1, 0};
        data::FreshInventoryOwnedV4 inventory(0x9001, table.borrow(), rng, 12,
                                               std::make_shared<data::PropertyState>());
        check(inventory.register_quest_gathering_item_id(456, error),
              "register source GatherLoot item in canonical V4");

        source_level_owner_v1::Owner level;
        int projection{}, fields{}, quest_owner{};
        check(level.begin_load() && level.request_level(1, 0, 0) &&
              level.begin_source_load() && level.publish_projection(&projection),
              "construct active source-Level lifecycle");
        check(level.bind_native_level({&fields, &quest_owner, 0x5001, 0x9001,
                                       1, 0, 0, 38}),
              "bind canonical player Save and Quest owner to Level");
        check(level.snapshot().player_character == 0x9001 &&
              level.snapshot().quest_owner == &quest_owner,
              "Level snapshot retains canonical Character and Quest owner identities");

        level_world::current_level_quest_event_v1::Runtime events;
        using Gather = data::quest_gather_loot_receiver_v1::Binding;
        data::quest_objective_factory_v1::Record objective(0x8001);
        objective.fields.character_10 = 0x9001;
        GatherContext gather_context;
        check(data::quest_gather_loot_receiver_v1::compile(
                  objective, true, 456, 1, 42, true, 0, &gather_context,
                  GatherContext::start_script, error),
              "compile canonical GatherLoot receiver");
        Gather gather{&objective, 37, 456, 1, 42, &gather_context,
                      &gather_context, GatherContext::item_quantity,
                      GatherContext::start_script};
        bool attached{};
        check(events.attach(37, 0x8001, 0, &gather, Gather::receive,
                            attached, error) ==
                  level_world::current_level_quest_event_v1::Status::complete && attached,
              "attach GatherLoot receiver to the existing EventManager");
        const auto event_owner = reinterpret_cast<std::uintptr_t>(&quest_owner);
        check(reinterpret_cast<std::uintptr_t>(level.snapshot().quest_owner) == event_owner,
              "snapshot Quest owner identity matches RaiseAsync owner");
        RaiseContext raise_context{&events, event_owner};
        Services services{&raise_context, RaiseContext::is_player,
                          event_owner, 37, RaiseContext::raise};
        check(services.event_owner_identity == event_owner,
              "RaiseAsync service identity matches Level Quest owner");

        Result result{};
        check(after_transfer(level, inventory, 0x9001, 457, services, &result, error) ==
                  Status::ignored && raise_context.calls == 0,
              "unregistered pickup must not raise quest event");
        check(after_transfer(level, inventory, 0x9002, 456, services, &result, error) ==
                  Status::ignored && raise_context.calls == 0,
              "different Character must not reach canonical inventory or EventManager");
        const auto committed_status = after_transfer(
            level, inventory, 0x9001, 456, services, &result, error);
        if (committed_status != Status::raised)
            throw std::runtime_error("committed pickup status " +
                std::to_string(static_cast<unsigned>(committed_status)) + ": " + error);
        check(result.registered_item &&
                  result.level_identity == level.snapshot().source_level &&
                  result.event_owner_identity == event_owner &&
                  objective.done_14 && objective.quantity_20 == 1 &&
                  gather_context.script_calls == 1 && gather_context.script_id == 42 &&
                  raise_context.delivered.flag0 == 1 &&
                  raise_context.delivered.subject_id == 1,
              "committed pickup raises into the active Level's same Quest EventManager");

        // ItemManager despawns the shell after the first committed transfer;
        // the objective's source done guard keeps repeated delivery from
        // restarting its completion script if a caller retries the tail.
        check(after_transfer(level, inventory, 0x9001, 456, services, &result, error) ==
                  Status::raised && gather_context.script_calls == 1 &&
                  raise_context.calls == 2,
              "repeated pickup notification cannot re-run objective completion");

        services.event_owner_identity = event_owner + 1;
        check(after_transfer(level, inventory, 0x9001, 456, services, &result, error) ==
                  Status::failed && raise_context.calls == 2,
              "stale Quest EventManager identity is rejected before dispatch");
        level.begin_teardown();
        services.event_owner_identity = event_owner;
        check(after_transfer(level, inventory, 0x9001, 456, services, &result, error) ==
                  Status::ignored && raise_context.calls == 2,
              "pickup after Level teardown is a no-op");

        std::cout << "{\"validation\":\"PASS\",\"active_level_and_v4_owner_binding\":true,"
                     "\"gather_registered_list_gate\":true,\"raise_async_order\":true,"
                     "\"duplicate_completion_guard\":true,\"stale_owner_rejected\":true,"
                     "\"teardown_noop\":true,\"checks\":" << checks << "}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
