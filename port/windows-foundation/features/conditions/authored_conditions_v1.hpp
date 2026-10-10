#pragma once
// Preview 17 CONDITIONS: one evaluator for the authored v2Conditions table (IsAfter_*, IsBefore_*,
// IsBetween_*, IsDuring_*, RENE_FOLLOW, ...). Used by population (activate_cond/deactivate_cond),
// gated objects (unlock_cond) and any future owner that reads a condition name.
//
// Table: original-cache/data/pydata/v2conditions_pyarray.bin (rows), _pyarraynames.bin (names),
// _pystructnames.bin (schema). Row = count, count x (op, argument8, argument_c), type. Decoded by
// NativeConditionTableV69 (the original Arrays.v2Conditions reader).
//
// Evaluation (IDA libDungeonHunter2.so, pseudocode-all.c):
//   Condition_IsQuestInStateGeneric::Eval 0x478fe8: quest = SG_GetQuestByID(local character,
//     argument8, -1); no character or no quest -> false. op 0: argument_c == state;
//     op 1: argument_c > state; op 2: argument_c < state.
//   factory 3 (IsPlayerInLevel): argument8 == current level row.
//   factories 4/5: literal TRUE in the captured ELF (native_conditions_runtime_v69.cpp).
//   factory 6 (IsEventInState): GameEvents.GetEventByID(argument8) state == argument_c.
//   ConditionList::Eval 0x478754: stubs in order, first false short-circuits; empty list is true.
//   "Invalid" and the empty name mean no condition (ConditionData with no compiled list is true).
#include "../../../level-world/native_conditions_v69.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::conditions {

// Services the evaluator reads. Each is the single owner of its state; absent owners make the
// dependent stub fail (error set, value false) instead of silently answering.
struct ConditionWorldV1 {
    // SG_GetQuestByID(quest, -1): state of an authored v2Quest row. False when the quest has no state.
    std::function<bool(std::int32_t quest, std::int32_t& state)> quest_state;
    // GameEvents.GetEventByID(event): authored event state value. False when the event is absent.
    std::function<bool(std::int32_t event, std::int32_t& value)> event_state;
    // Application current level row (Level::m_row, IsPlayerInLevel key). -1 when unknown.
    std::function<std::int32_t()> current_level_row;
};

// "" and "Invalid" are the authored no-condition values.
bool is_no_condition(const std::string& name) noexcept;

class AuthoredConditionsV1 {
public:
    // Decodes the three original-cache blobs. Atomic: malformed input returns no table.
    static bool decode(const std::vector<std::uint8_t>& records, const std::vector<std::uint8_t>& names,
                       const std::vector<std::uint8_t>& schema,
                       std::shared_ptr<const AuthoredConditionsV1>& out, std::string& error);

    bool contains(const std::string& name) const;
    std::size_t size() const noexcept { return index_.size(); }

    // Quest rows and event ids a named condition reads (watch dependencies). False when unknown.
    bool dependencies(const std::string& name, std::vector<std::int32_t>& quests,
                      std::vector<std::int32_t>& events) const;

    // Evaluates a name. "" and "Invalid" are true. An unknown name returns false with `error` set and
    // is logged once per process. A failed service also returns false with `error` set.
    bool evaluate(const std::string& name, const ConditionWorldV1& world, bool& value,
                  std::string& error) const;

private:
    std::shared_ptr<const dh2::world::NativeConditionTableV69> table_;
    std::map<std::string, std::size_t> index_;
};

// Logs an unknown authored condition name once per process (stderr, one line).
void report_unknown_condition_once(const std::string& name);

} // namespace dh::foundation::conditions
