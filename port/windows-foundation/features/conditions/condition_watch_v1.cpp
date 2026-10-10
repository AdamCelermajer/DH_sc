#include "condition_watch_v1.hpp"

namespace dh::foundation::conditions {

ConditionWatchV1::ConditionWatchV1(std::shared_ptr<const AuthoredConditionsV1> conditions)
    : conditions_(std::move(conditions)) {}

bool ConditionWatchV1::watch(const std::string& name) {
    if (is_no_condition(name) || !conditions_ || !conditions_->contains(name)) return false;
    if (entries_.count(name)) return true;
    Entry entry;
    if (!conditions_->dependencies(name, entry.quests, entry.events)) return false;
    for (const auto quest : entry.quests) by_quest_.emplace(quest, name);
    for (const auto event : entry.events) by_event_.emplace(event, name);
    entries_.emplace(name, std::move(entry));
    dirty_.insert(name); // the first flush after a watch evaluates it (no transition is reported for it)
    return true;
}

bool ConditionWatchV1::watching(const std::string& name) const {
    return entries_.count(name) != 0;
}

void ConditionWatchV1::quest_changed(std::int32_t quest) {
    const auto range = by_quest_.equal_range(quest);
    for (auto it = range.first; it != range.second; ++it) dirty_.insert(it->second);
}

void ConditionWatchV1::event_changed(std::int32_t event) {
    const auto range = by_event_.equal_range(event);
    for (auto it = range.first; it != range.second; ++it) dirty_.insert(it->second);
}

bool ConditionWatchV1::prime(const ConditionWorldV1& world, std::string& error) {
    error.clear();
    for (auto& pair : entries_) {
        bool value = false;
        std::string e;
        if (!conditions_->evaluate(pair.first, world, value, e)) {
            error = pair.first + ": " + e;
            return false;
        }
        pair.second.value = value;
        pair.second.primed = true;
        dirty_.erase(pair.first);
    }
    return true;
}

std::vector<ConditionFlipV1> ConditionWatchV1::flush(const ConditionWorldV1& world, std::string& error) {
    error.clear();
    std::vector<ConditionFlipV1> flips;
    if (dirty_.empty()) return flips;
    const auto pending = dirty_; // std::set: deterministic (name) order
    dirty_.clear();
    for (const auto& name : pending) {
        const auto found = entries_.find(name);
        if (found == entries_.end()) continue;
        bool value = false;
        std::string e;
        if (!conditions_->evaluate(name, world, value, e)) {
            if (error.empty()) error = name + ": " + e;
            dirty_.insert(name); // retried on the next dependency event
            continue;
        }
        // A name that was never primed takes its first value as the baseline (no transition).
        if (!found->second.primed) {
            found->second.primed = true;
            found->second.value = value;
        } else if (value != found->second.value) {
            flips.push_back({name, found->second.value, value});
            found->second.value = value;
        }
    }
    return flips;
}

bool ConditionWatchV1::current(const std::string& name, bool& value) const {
    const auto found = entries_.find(name);
    if (found == entries_.end()) return false;
    value = found->second.value;
    return true;
}

ConditionWorldV1 world_from_quest_runtime(const quest_runtime::QuestRuntimeV1& runtime,
                                          std::function<std::int32_t()> level_row) {
    ConditionWorldV1 world;
    world.quest_state = [&runtime](std::int32_t quest, std::int32_t& state) {
        quest_runtime::QuestStateV1 authored{};
        if (quest < 0 || !runtime.state_of(quest, authored)) return false;
        state = std::int32_t(authored);
        return true;
    };
    world.current_level_row = std::move(level_row);
    return world;
}

} // namespace dh::foundation::conditions
