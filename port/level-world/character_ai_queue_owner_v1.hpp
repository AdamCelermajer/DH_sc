#pragma once

#include "character_ai_queue.hpp"
#include "character_ai_turn.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace dh2::character_ai_queue_owner_v1 {

using OwnerProjection = character_ai_queue::Owner;
using Request = character_ai_queue::Request;
using Entry = character_ai_queue::Entry;
using Result = character_ai_queue::Result;

struct Services {
    void* context;
    std::int32_t (*invoke)(void*, character_ai_queue::State*, Entry*,
                           const Request*, std::uint32_t*);
};

enum class Status : std::int32_t {
    complete, invalid_argument, duplicate_ai, missing_ai, busy, capacity,
    service_failed,
};

// Stable process/world owner for the one source CharAI queue. The source
// 180 ms timer stays in character_ai_queue::State. Registration/removal retain
// source order and never replace or reset that timer. Owner projections are
// copied into stable records and refreshed from live Character fields before
// each Level-update advance.
class Runtime final {
public:
    Runtime();
    ~Runtime();
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;
    Runtime(Runtime&&) = delete;
    Runtime& operator=(Runtime&&) = delete;

    Status register_ai(std::uintptr_t ai, const OwnerProjection&);
    Status refresh_owner(std::uintptr_t ai, const OwnerProjection&);
    Status remove_ai(std::uintptr_t ai) noexcept;
    Status advance(std::uint32_t frame_delta_ms, bool global_blocked,
                   const Services*, Result*);

    character_ai_turn::Globals turn_globals() const noexcept;
    std::uint32_t count() const noexcept { return state_.count; }
    std::int32_t timer() const noexcept { return state_.timer; }

private:
    struct Record;
    struct Bridge {
        Runtime* owner;
        Services upstream;
        std::uint32_t frame_delta_ms;
    };
    static std::int32_t dispatch(void*, character_ai_queue::State*, Entry*,
                                 const Request*, std::uint32_t*);

    Record* find(std::uintptr_t ai) noexcept;
    const Record* find(std::uintptr_t ai) const noexcept;
    void bind_entries() noexcept;

    std::vector<std::unique_ptr<Record>> records_;
    std::vector<Entry*> entries_;
    character_ai_queue::State state_{};
    bool busy_ = false;
};

} // namespace dh2::character_ai_queue_owner_v1
