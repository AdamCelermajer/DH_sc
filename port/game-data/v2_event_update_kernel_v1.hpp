#pragma once

#include "v2_event_table_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::data::v2_event_update_kernel_v1 {

enum class Stage : std::uint32_t {
    none,
    trigger_preflight,
    read_state,
    objective_valid,
    objective_eval,
    set_active,
    set_completed,
    start_script,
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    unsupported_trigger,
    service_unavailable,
    service_failed,
};

struct Services {
    // The production bridge context is borrowed from the canonical native
    // Quest owner and its already-retained ScriptManager session.
    void* context = nullptr;
    // Must verify that this trigger's Objective implementation is available
    // on the same canonical Level/Quest owner before any row state mutates.
    bool (*trigger_supported)(void*, const v2_event_table_v1::Event&,
                              const v2_event_table_v1::Trigger&, bool&,
                              std::string&) = nullptr;
    bool (*state)(void*, const v2_event_table_v1::Event&, std::int32_t&,
                  std::string&) = nullptr;
    bool (*objective_list_valid)(void*, const v2_event_table_v1::Event&, bool&,
                                 std::string&) = nullptr;
    bool (*objective_list_eval)(void*, const v2_event_table_v1::Event&, bool&,
                                std::string&) = nullptr;
    bool (*set_state)(void*, const v2_event_table_v1::Event&, std::int32_t,
                      std::string&) = nullptr;
    // Source GameEvent::SetState calls ExecScript only after committing
    // Completed; a missing script ID is a successful no-op in the adapter.
    bool (*start_script)(void*, const v2_event_table_v1::Event&,
                         const std::string& script, std::int32_t argument,
                         bool check_running, bool online, bool& started,
                         std::string&) = nullptr;
};

struct Result {
    Status status = Status::complete;
    Stage failed_stage = Stage::none;
    std::uint32_t rows_visited = 0;
    std::uint32_t rows_activated = 0;
    std::uint32_t rows_completed = 0;
    std::uint32_t scripts_started = 0;
    std::int32_t failed_row = -1;
    bool state_committed_before_failure = false;
};

// Stateless GameEventManager::Update ordering over caller-owned GameEvent
// records and ObjectiveLists. It never creates event rows, an event queue,
// objective records, or a ScriptManager/VM.
Status update(const v2_event_table_v1::Table&, const Services&, Result*,
              std::string& error);

} // namespace dh2::data::v2_event_update_kernel_v1
