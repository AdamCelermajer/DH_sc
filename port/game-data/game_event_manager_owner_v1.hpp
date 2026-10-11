#pragma once

#include "quest_objective_list_v1.hpp"
#include "v2_event_table_v1.hpp"
#include "v2_event_update_kernel_v1.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh2::data::game_event_manager_owner_v1 {

enum class Operation : std::uint32_t {
    none, construct_list, assign_event, initial_reset, set_inactive,
    reset_objectives, invalidate_compile, compile, register_objectives,
    close_list, complete
};
enum class Status : std::uint32_t {
    complete, invalid_argument, owner_unavailable, service_failed,
    source_fault, reentrant
};

struct Result {
    Status status = Status::complete;
    Operation failed_operation = Operation::none;
    std::int32_t failed_row = -1;
    std::uint32_t rows_visited = 0;
    std::uint32_t objectives_created = 0;
    std::uint32_t rows_reset = 0;
    std::uint32_t rows_compiled = 0;
    std::uint32_t rows_registered = 0;
};

struct Row {
    std::uintptr_t identity = 0;
    std::uint32_t ordinal = 0;
    std::int32_t state = -1;
    bool registered = false;
    const v2_event_table_v1::Event* definition = nullptr;
    quest_objective_list_v1::List objectives;
};

// The reached leaf provider. The ObjectiveList Runtime owns source-order
// LoopOnAll traversal and passes the exact Objective member dispatch.
struct Services {
    void* context = nullptr;
    bool (*objective_member)(void*, std::uintptr_t level_identity,
                             std::uintptr_t manager_identity, Row&,
                             const quest_objective_list_v1::MemberCall&,
                             std::string&) = nullptr;
};

// Per-frame providers reached by the source GameEventManager::Update loop.
// Row state remains owned by Owner; objective callbacks inspect the existing
// Save factory records and script execution uses the current Level VM.
struct UpdateServices {
    void* context = nullptr;
    bool (*objective_list_valid)(void*, const Row&, bool&, std::string&) = nullptr;
    bool (*objective_list_eval)(void*, const Row&, bool&, std::string&) = nullptr;
    bool (*start_script)(void*, const v2_event_table_v1::Event&,
                         const std::string&, std::int32_t argument,
                         bool check_running, bool online, bool& started,
                         std::string&) = nullptr;
};

// One retained Level's GameEventManager. It owns event state and each
// ObjectiveList while borrowing the parsed table and existing Objective
// factory/array services. It does not own Quest receiver registrations or a VM.
class Owner {
    struct RetainedRow;
    std::uintptr_t level_identity_ = 0;
    std::uint32_t level_generation_ = 0;
    std::uintptr_t manager_identity_ = 0;
    v2_event_table_v1::Table table_;
    quest_objective_list_v1::Services objective_services_{};
    Services services_{};
    std::vector<std::unique_ptr<RetainedRow>> rows_;
    bool initialized_ = false;
    bool busy_ = false;
    static std::int32_t dispatch_objective_member(
        void*, quest_objective_list_v1::List&,
        const quest_objective_list_v1::MemberCall&);

public:
    Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    ~Owner();

    Status initialize(std::uintptr_t level_identity, std::uint32_t level_generation,
                      std::uintptr_t manager_identity,
                      const v2_event_table_v1::Table&,
                      quest_objective_list_v1::Services,
                      Services, Result*, std::string& error);
    // Source GameEventManager::ReInit: each retained GameEvent performs
    // ReInit (state=0 then ObjectiveList::_resetData), Compile, then Register
    // only if its original registered byte was false.
    // Operation-scoped replacement for current-Level Objective providers.
    // The callback/context is borrowed only until this synchronous ReInit
    // returns; retained row/list state remains owned here.
    Status reinit(std::uintptr_t expected_level_identity,
                  std::uint32_t expected_level_generation,
                  std::uintptr_t expected_manager_identity,
                  const Services& operation_services, Result*, std::string& error);
    // Source GameEventManager::Compile after Load: no reset, then compile and
    // register rows whose original registered byte is clear.
    Status compile(std::uintptr_t expected_level_identity,
                   std::uint32_t expected_level_generation,
                   std::uintptr_t expected_manager_identity,
                   const Services& operation_services, Result*, std::string& error);
    // Uses the original stateless Update kernel over this owner's retained
    // table. Event callbacks can resolve their canonical state/list with row_for.
    v2_event_update_kernel_v1::Status update(
        std::uintptr_t expected_level_identity,
        std::uint32_t expected_level_generation,
        std::uintptr_t expected_manager_identity,
        const UpdateServices&,
        v2_event_update_kernel_v1::Result*, std::string& error);
    // Call before the borrowed Native Quest ObjectiveFactory owner retires.
    // The destructor intentionally performs no virtual cleanup implicitly.
    Status close(Result*, std::string& error);

    std::uintptr_t level_identity() const noexcept { return level_identity_; }
    std::uint32_t level_generation() const noexcept { return level_generation_; }
    std::uintptr_t manager_identity() const noexcept { return manager_identity_; }
    std::size_t row_count() const noexcept { return rows_.size(); }
    Row* row(std::size_t ordinal) noexcept;
    const Row* row(std::size_t ordinal) const noexcept;
    Row* row_for(const v2_event_table_v1::Event&) noexcept;
    const Row* row_for(const v2_event_table_v1::Event&) const noexcept;
    const v2_event_table_v1::Table* table() const noexcept {
        return initialized_ ? &table_ : nullptr;
    }
};

} // namespace dh2::data::game_event_manager_owner_v1
