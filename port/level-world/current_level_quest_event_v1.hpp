#pragma once

#include "loot_pickup_quest_tail_v10.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace dh2::level_world::current_level_quest_event_v1 {

using Event = character::LootPickupQuestEventV10;
class Runtime;
using Receiver = std::int32_t (*)(void*, Runtime&, Event&, std::string&);

struct Result {
    std::uint32_t delivered = 0;
    std::uint32_t detached = 0;
    bool stopped = false;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    receiver_failed,
    receiver_threw,
    reentrant_update
};

// One current-Level listener table. Quest/objective records stay owned by the
// canonical Quest owner; entries here are only source EventManager receiver
// registrations. RaiseAsync is synchronous in this ELF and snapshots the
// selected receiver list before callbacks run (IDA 0x339090 -> 0x338ebc).
class Runtime {
    struct Entry {
        std::uintptr_t receiver = 0;
        std::int32_t priority = 0;
        void* context = nullptr;
        Receiver invoke = nullptr;
    };

    std::map<std::int32_t, std::vector<Entry>> receivers_;
    std::vector<std::pair<std::int32_t, std::uintptr_t>> delayed_detach_;
    bool flushing_ = false;

public:
    Runtime() = default;
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;

    // Source Attach returns 1 for a new receiver and 0 for a duplicate. The
    // priority value is retained but does not reorder delivery.
    Status attach(std::int32_t event_type, std::uintptr_t receiver,
                  std::int32_t priority, void* context, Receiver,
                  bool& attached, std::string& error);
    Status detach(std::int32_t event_type, std::uintptr_t receiver,
                  bool& detached, std::string& error);
    Status delayed_detach(std::int32_t event_type, std::uintptr_t receiver,
                          bool& scheduled, std::string& error);

    // Event type is the source GetType() result represented by
    // Event::objective_type. Receivers run in attach order; return value 1
    // stops delivery. The semantic payload remains mutable because source
    // objective handlers update QE_GatherLoot's flag/quantity fields.
    Status raise(Event&, Result&, std::string& error);

    // Models EventManager::DropDelayedDetach after the level's queued-event
    // phase. RaiseAsync itself does not enqueue in this binary.
    Status flush_delayed_detaches(Result&, std::string& error);
    void clear() noexcept;
};

}  // namespace dh2::level_world::current_level_quest_event_v1
