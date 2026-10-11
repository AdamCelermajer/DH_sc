#include "authored_conditions_v1.hpp"

#include <iostream>
#include <mutex>
#include <set>

namespace dh::foundation::conditions {

bool is_no_condition(const std::string& name) noexcept {
    return name.empty() || name == "Invalid";
}

void report_unknown_condition_once(const std::string& name) {
    static std::mutex guard;
    static std::set<std::string> logged;
    std::lock_guard<std::mutex> lock(guard);
    if (logged.insert(name).second)
        std::cerr << "Unknown authored condition " << name << " (not in v2Conditions; logged once)\n";
}

bool AuthoredConditionsV1::decode(const std::vector<std::uint8_t>& records, const std::vector<std::uint8_t>& names,
                                  const std::vector<std::uint8_t>& schema,
                                  std::shared_ptr<const AuthoredConditionsV1>& out, std::string& error) {
    out.reset();
    auto table = std::make_shared<dh2::world::NativeConditionTableV69>();
    const dh2::world::NativeConditionDecodeBudgetV69 budget;
    if (!table->decode({records.data(), records.size()}, {names.data(), names.size()},
                       {schema.data(), schema.size()}, budget, error))
        return false;
    if (!table->ready() || table->rows().empty()) {
        error = "v2Conditions table decoded no rows";
        return false;
    }
    auto result = std::make_shared<AuthoredConditionsV1>();
    result->table_ = std::move(table);
    for (std::size_t i = 0; i < result->table_->rows().size(); ++i) {
        const auto& name = result->table_->rows()[i].name;
        if (name.empty()) {
            error = "v2Conditions row has no name";
            return false;
        }
        if (!result->index_.emplace(name, i).second) {
            error = "v2Conditions name is duplicated: " + name;
            return false;
        }
    }
    out = std::move(result);
    error.clear();
    return true;
}

bool AuthoredConditionsV1::contains(const std::string& name) const {
    return index_.count(name) != 0;
}

bool AuthoredConditionsV1::dependencies(const std::string& name, std::vector<std::int32_t>& quests,
                                        std::vector<std::int32_t>& events) const {
    quests.clear();
    events.clear();
    const auto found = index_.find(name);
    if (found == index_.end()) return false;
    const auto& row = table_->rows()[found->second];
    for (std::int32_t i = 0; i < row.count4; ++i) {
        const auto& stub = row.stubs8[i];
        if (stub.op4 >= 0 && stub.op4 <= 2) quests.push_back(stub.argument8);
        else if (stub.op4 == 6) events.push_back(stub.argument8);
    }
    return true;
}

bool AuthoredConditionsV1::evaluate(const std::string& name, const ConditionWorldV1& world, bool& value,
                                    std::string& error) const {
    error.clear();
    value = false;
    if (is_no_condition(name)) {
        value = true;
        return true;
    }
    const auto found = index_.find(name);
    if (found == index_.end()) {
        error = "unknown authored condition " + name;
        report_unknown_condition_once(name);
        return false;
    }
    const auto& row = table_->rows()[found->second];
    bool all = true;
    for (std::int32_t i = 0; i < row.count4 && all; ++i) {
        const auto& stub = row.stubs8[i];
        bool truth = false;
        switch (stub.op4) {
        case 0:
        case 1:
        case 2: {
            if (!world.quest_state) {
                error = "quest state service is not bound (" + name + ")";
                return false;
            }
            std::int32_t state = 0;
            if (!world.quest_state(stub.argument8, state)) {
                truth = false; // SG_GetQuestByID returned NULL: the source returns false.
                break;
            }
            if (stub.op4 == 0) truth = stub.argument_c == state;
            else if (stub.op4 == 1) truth = stub.argument_c > state;
            else truth = stub.argument_c < state;
            break;
        }
        case 3: {
            if (!world.current_level_row) {
                error = "current level service is not bound (" + name + ")";
                return false;
            }
            const auto level = world.current_level_row();
            if (level < 0) {
                error = "current level is unknown for IsPlayerInLevel (" + name + ")";
                return false;
            }
            truth = stub.argument8 == level;
            break;
        }
        case 4:
        case 5:
            truth = true; // literal TRUE in the captured ELF (no state read).
            break;
        case 6: {
            if (!world.event_state) {
                error = "event state service is not bound (" + name + ")";
                return false;
            }
            std::int32_t state = 0;
            if (!world.event_state(stub.argument8, state)) {
                truth = false;
                break;
            }
            truth = stub.argument_c == state;
            break;
        }
        default:
            error = "unsupported condition factory " + std::to_string(stub.op4) + " in " + name;
            return false;
        }
        all = truth; // ConditionList::Eval short-circuits on the first false stub.
    }
    value = all;
    return true;
}

} // namespace dh::foundation::conditions
