#include "combat_result_event_bus_v1.hpp"

#include <algorithm>

namespace dh::foundation::generic_skills {

std::uint64_t CombatResultEventBusV1::subscribe(ActorId receiver, std::uint32_t callback_flag, std::string key,
                                                Listener listener) {
    if (receiver == invalid_actor_id || !listener) return 0;
    if (callback_flag != kCombatCallbackTargetHitV1 && callback_flag != kCombatCallbackTargetMissedV1) return 0;
    Entry entry;
    entry.token = next_token_++;
    entry.receiver = receiver;
    entry.callback_flag = callback_flag;
    entry.key = std::move(key);
    entry.listener = std::move(listener);
    const auto token = entry.token;
    entries_.push_back(std::move(entry));
    return token;
}

bool CombatResultEventBusV1::unsubscribe(std::uint64_t token) {
    const auto it = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& e) { return e.token == token; });
    if (it == entries_.end()) return false;
    entries_.erase(it);
    return true;
}

std::size_t CombatResultEventBusV1::unsubscribe_key(ActorId receiver, const std::string& key) {
    const auto before = entries_.size();
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
                                  [&](const Entry& e) { return e.receiver == receiver && e.key == key; }),
                   entries_.end());
    return before - entries_.size();
}

std::uint32_t CombatResultEventBusV1::flags(ActorId receiver) const {
    std::uint32_t result = 0;
    for (const auto& entry : entries_) {
        if (entry.receiver != receiver) continue;
        result |= entry.callback_flag;
    }
    return result;
}

std::size_t CombatResultEventBusV1::listener_count(ActorId receiver) const {
    return static_cast<std::size_t>(std::count_if(entries_.begin(), entries_.end(),
                                                  [&](const Entry& e) { return e.receiver == receiver; }));
}

std::size_t CombatResultEventBusV1::dispatch(const CombatResultEventV1& event) {
    std::size_t invoked = 0;
    const ActorId receivers[2] = {event.attacker, event.target};
    const bool missed = (event.outcomes & 3u) != 0;
    const std::uint32_t wanted = missed ? kCombatCallbackTargetMissedV1 : kCombatCallbackTargetHitV1;
    for (std::size_t r = 0; r < 2; ++r) {
        const ActorId receiver = receivers[r];
        if (receiver == invalid_actor_id) continue;
        // Native: each receiver's own flag word selects the callback; the Lua proxy runs its listeners in order.
        // Copy the matching listeners first so a listener may unsubscribe while the list is walked.
        std::vector<Listener> matching;
        for (const auto& entry : entries_)
            if (entry.receiver == receiver && entry.callback_flag == wanted) matching.push_back(entry.listener);
        for (auto& listener : matching) {
            listener(event, receiver);
            ++invoked;
        }
    }
    return invoked;
}

} // namespace dh::foundation::generic_skills
