#include "character_ai_queue_owner_v1.hpp"
#include <algorithm>

namespace dh2::character_ai_queue_owner_v1 {

struct Runtime::Record {
    OwnerProjection owner;
    Entry entry;
    Record(std::uintptr_t ai, const OwnerProjection& projection)
        : owner(projection), entry{ai, &owner} {}
};

Runtime::Runtime() {
    // Keep a non-null bounded queue view even before the first CharAI registers.
    entries_.reserve(8);
    bind_entries();
}

Runtime::~Runtime() = default;

Runtime::Record* Runtime::find(std::uintptr_t ai) noexcept {
    const auto found = std::find_if(records_.begin(), records_.end(),
        [ai](const auto& record) { return record->entry.ai == ai; });
    return found == records_.end() ? nullptr : found->get();
}

const Runtime::Record* Runtime::find(std::uintptr_t ai) const noexcept {
    const auto found = std::find_if(records_.begin(), records_.end(),
        [ai](const auto& record) { return record->entry.ai == ai; });
    return found == records_.end() ? nullptr : found->get();
}

void Runtime::bind_entries() noexcept {
    state_.entries = entries_.data();
    state_.count = static_cast<std::uint32_t>(entries_.size());
    state_.capacity = static_cast<std::uint32_t>(entries_.capacity());
}

Status Runtime::register_ai(std::uintptr_t ai, const OwnerProjection& projection) {
    if (busy_) return Status::busy;
    if (!ai || !projection.identity) return Status::invalid_argument;
    if (find(ai)) return Status::duplicate_ai;
    if (entries_.size() >= 65536) return Status::capacity;
    if (entries_.size() == entries_.capacity()) {
        const auto next = std::min<std::size_t>(65536,
            std::max<std::size_t>(8, entries_.capacity() * 2));
        if (next <= entries_.capacity()) return Status::capacity;
        entries_.reserve(next);
    }
    auto record = std::make_unique<Record>(ai, projection);
    auto* entry = &record->entry;
    records_.push_back(std::move(record));
    entries_.push_back(entry);
    bind_entries();
    return Status::complete;
}

Status Runtime::refresh_owner(std::uintptr_t ai, const OwnerProjection& projection) {
    if (busy_) return Status::busy;
    auto* record = find(ai);
    if (!record) return Status::missing_ai;
    if (!projection.identity || projection.identity != record->owner.identity)
        return Status::invalid_argument;
    record->owner = projection;
    return Status::complete;
}

Status Runtime::remove_ai(std::uintptr_t ai) noexcept {
    if (busy_) return Status::busy;
    const auto record = std::find_if(records_.begin(), records_.end(),
        [ai](const auto& value) { return value->entry.ai == ai; });
    if (record == records_.end()) return Status::missing_ai;
    auto* entry = &(*record)->entry;
    const auto queued = std::find(entries_.begin(), entries_.end(), entry);
    if (queued == entries_.end()) return Status::invalid_argument;
    entries_.erase(queued);
    records_.erase(record);
    bind_entries();
    return Status::complete;
}

std::int32_t Runtime::dispatch(void* raw, character_ai_queue::State* state,
        Entry* entry, const Request* request, std::uint32_t* output) {
    if (!raw || !state || !request || !output) return 1;
    auto& bridge = *static_cast<Bridge*>(raw);
    if (!bridge.owner || !bridge.owner->busy_) return 1;
    if (request->operation == character_ai_queue::Operation::frame_delta) {
        *output = bridge.frame_delta_ms;
        return 0;
    }
    if (!bridge.upstream.invoke) return 1;
    try {
        return bridge.upstream.invoke(bridge.upstream.context, state, entry,
                                      request, output) ? 1 : 0;
    } catch (...) {
        return 1;
    }
}

Status Runtime::advance(std::uint32_t frame_delta_ms, bool global_blocked,
                        const Services* services, Result* output) {
    if (busy_) return Status::busy;
    if (!output || !services) return Status::invalid_argument;
    if (entries_.capacity() == 0 || entries_.capacity() > 65536 ||
        entries_.size() > entries_.capacity()) return Status::invalid_argument;
    const auto upstream = *services;
    state_.global_blocked = global_blocked ? 1 : 0;
    bind_entries();
    Bridge bridge{this, upstream, frame_delta_ms};
    const character_ai_queue::Services bound{&bridge, dispatch};
    busy_ = true;
    const auto status = character_ai_queue::advance(&state_, &bound, output);
    busy_ = false;
    return status == character_ai_queue::Status::complete ?
        Status::complete : Status::service_failed;
}

character_ai_turn::Globals Runtime::turn_globals() const noexcept {
    return character_ai_queue::turn_globals(state_);
}

} // namespace dh2::character_ai_queue_owner_v1
