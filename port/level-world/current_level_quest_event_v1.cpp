#include "current_level_quest_event_v1.hpp"

#include <algorithm>
#include <exception>

namespace dh2::level_world::current_level_quest_event_v1 {
namespace {
Status missing(std::string& error, const char* message) {
    error = message;
    return Status::invalid_argument;
}
}

Status Runtime::attach(std::int32_t event_type, std::uintptr_t receiver,
                       std::int32_t priority, void* context, Receiver invoke,
                       bool& attached, std::string& error) {
    attached = false;
    if (!receiver || !invoke) return missing(error, "Invalid current-Level event receiver");
    auto& entries = receivers_[event_type];
    const auto existing = std::find_if(entries.begin(), entries.end(),
        [receiver](const Entry& entry) { return entry.receiver == receiver; });
    if (existing != entries.end()) {
        error.clear();
        return Status::complete;
    }
    try {
        entries.push_back({receiver, priority, context, invoke});
    } catch (const std::exception&) {
        error = "Current-Level event receiver allocation failed";
        return Status::receiver_failed;
    }
    attached = true;
    error.clear();
    return Status::complete;
}

Status Runtime::detach(std::int32_t event_type, std::uintptr_t receiver,
                       bool& detached, std::string& error) {
    detached = false;
    if (!receiver) return missing(error, "Invalid current-Level event receiver");
    const auto group = receivers_.find(event_type);
    if (group == receivers_.end()) {
        error.clear();
        return Status::complete;
    }
    auto& entries = group->second;
    const auto found = std::find_if(entries.begin(), entries.end(),
        [receiver](const Entry& entry) { return entry.receiver == receiver; });
    if (found != entries.end()) {
        entries.erase(found);
        detached = true;
        if (entries.empty()) receivers_.erase(group);
    }
    error.clear();
    return Status::complete;
}

Status Runtime::delayed_detach(std::int32_t event_type,
                               std::uintptr_t receiver,
                               bool& scheduled, std::string& error) {
    scheduled = false;
    if (!receiver) return missing(error, "Invalid current-Level event receiver");
    const auto group = receivers_.find(event_type);
    if (group == receivers_.end() ||
        std::none_of(group->second.begin(), group->second.end(),
            [receiver](const Entry& entry) { return entry.receiver == receiver; })) {
        error.clear();
        return Status::complete;
    }
    try {
        delayed_detach_.emplace_back(event_type, receiver);
    } catch (const std::exception&) {
        error = "Current-Level delayed-detach allocation failed";
        return Status::receiver_failed;
    }
    scheduled = true;
    error.clear();
    return Status::complete;
}

Status Runtime::raise(Event& event, Result& result, std::string& error) {
    result = {};
    const auto group = receivers_.find(event.objective_type);
    if (group == receivers_.end()) {
        error.clear();
        return Status::complete;
    }

    // EventManager::Raise copies ReceiverInfo nodes before synchronous calls;
    // callbacks may attach/detach without changing this delivery pass.
    std::vector<Entry> snapshot;
    try {
        snapshot = group->second;
    } catch (const std::exception&) {
        error = "Current-Level receiver snapshot allocation failed";
        return Status::receiver_failed;
    }
    for (const auto& entry : snapshot) {
        std::int32_t stop = 0;
        error.clear();
        try {
            ++result.delivered;
            stop = entry.invoke(entry.context, *this, event, error);
        } catch (const std::exception&) {
            error = "Current-Level event receiver threw";
            return Status::receiver_threw;
        } catch (...) {
            error = "Current-Level event receiver threw";
            return Status::receiver_threw;
        }
        // A source receiver may legitimately consume delivery with return 1.
        // Adapter failures use the separate diagnostic channel: never erase a
        // failed provider merely because its callback also returned 1.
        if (!error.empty()) return Status::receiver_failed;
        if (stop == 1) {
            result.stopped = true;
            error.clear();
            break;
        }
    }
    error.clear();
    return Status::complete;
}

Status Runtime::flush_delayed_detaches(Result& result, std::string& error) {
    result = {};
    if (flushing_) {
        error = "Current-Level delayed-detach flush is reentrant";
        return Status::reentrant_update;
    }
    flushing_ = true;
    struct Guard { bool& value; ~Guard() { value = false; } } guard{flushing_};
    auto pending = std::move(delayed_detach_);
    delayed_detach_.clear();
    for (const auto& request : pending) {
        bool removed = false;
        const auto status = detach(request.first, request.second, removed, error);
        if (status != Status::complete) return status;
        result.detached += removed ? 1u : 0u;
    }
    error.clear();
    return Status::complete;
}

void Runtime::clear() noexcept {
    receivers_.clear();
    delayed_detach_.clear();
    flushing_ = false;
}

}  // namespace dh2::level_world::current_level_quest_event_v1
