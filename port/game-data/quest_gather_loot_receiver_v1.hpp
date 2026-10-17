#pragma once

#include "quest_objective_factory_v1.hpp"
#include "../level-world/current_level_quest_event_v1.hpp"

namespace dh2::data::quest_gather_loot_receiver_v1 {
using Event = level_world::current_level_quest_event_v1::Event;
using EventRuntime = level_world::current_level_quest_event_v1::Runtime;
using Record = quest_objective_factory_v1::Record;
using StartScript = bool (*)(void*, std::int32_t, std::string&);
using ItemQuantity = bool (*)(void*, std::int32_t, bool&, std::int32_t&, std::string&);

// Receiver context is metadata only. Objective progress/completion remain in
// the canonical factory Record; the quantity callback must query its owner's
// one Character inventory. The script callback is required when script_id>=0.
struct Binding {
    Record* objective{};
    std::int32_t event_type{}, item_id{}, target_quantity{}, script_id{-1};
    void* quantity_context{};
    void* script_context{};
    ItemQuantity item_quantity{};
    StartScript start_script{};
    bool attached{};
    bool detach_pending{};
    bool inventory_registered{};

    static std::int32_t receive(void*, EventRuntime&, Event&, std::string&);
};

// Source Objective_GatherLoot::Compile projection after its pydata, live
// Level ID, and same-inventory FindItem inputs have been resolved.
bool compile(Record&, bool level_matches, std::int32_t item_id,
             std::int32_t target_quantity, std::int32_t script_id,
             bool item_found, std::int32_t item_quantity,
             void* script_context, StartScript, std::string& error);
}  // namespace dh2::data::quest_gather_loot_receiver_v1
