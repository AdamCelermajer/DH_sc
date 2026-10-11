#pragma once
// Preview 17 CONDITIONS: event-driven condition flips. A watch registers the authored names a loaded
// level uses. Quest state changes (QuestRuntimeV1 services.state_changed) and event changes mark the
// dependent names dirty. flush() evaluates only the dirty names and returns the transitions. Nothing
// is evaluated per frame when no quest state changed.
#include "authored_conditions_v1.hpp"
#include "../quest_runtime/quest_runtime_v1.hpp"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::conditions {

struct ConditionFlipV1 {
    std::string name;
    bool before{};
    bool after{};
};

class ConditionWatchV1 {
public:
    explicit ConditionWatchV1(std::shared_ptr<const AuthoredConditionsV1> conditions);

    // Registers a name and its dependencies. Empty/"Invalid" and unknown names are not watched (false).
    bool watch(const std::string& name);
    bool watching(const std::string& name) const;
    std::size_t size() const noexcept { return entries_.size(); }

    // Dependency events. They only mark names dirty; they do not evaluate.
    void quest_changed(std::int32_t quest);
    void event_changed(std::int32_t event);

    // Baseline values for every watched name (fresh level, reload, rebind). No transitions are emitted.
    bool prime(const ConditionWorldV1& world, std::string& error);
    // Re-evaluates dirty names and returns the transitions since the last value. Clears the dirty set.
    std::vector<ConditionFlipV1> flush(const ConditionWorldV1& world, std::string& error);

    bool current(const std::string& name, bool& value) const;
    // Every watched name with its last value (name order).
    std::vector<std::pair<std::string,bool>> snapshot() const;
    std::size_t dirty_count() const noexcept { return dirty_.size(); }

private:
    struct Entry {
        bool value{};
        bool primed{};
        std::vector<std::int32_t> quests;
        std::vector<std::int32_t> events;
    };
    std::shared_ptr<const AuthoredConditionsV1> conditions_;
    std::map<std::string, Entry> entries_;
    std::multimap<std::int32_t, std::string> by_quest_;
    std::multimap<std::int32_t, std::string> by_event_;
    std::set<std::string> dirty_;
};

// World over a live quest runtime (the authored Quest state of this character). The runtime
// must outlive the returned world. `level_row` answers IsPlayerInLevel.
ConditionWorldV1 world_from_quest_runtime(const quest_runtime::QuestRuntimeV1& runtime,
                                          std::function<std::int32_t()> level_row);

} // namespace dh::foundation::conditions
