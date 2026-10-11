#include "game_event_manager_owner_v1.hpp"

#include <limits>

namespace dh2::data::game_event_manager_owner_v1 {
namespace {
namespace OL = dh2::data::quest_objective_list_v1;

Status fail(Result& result, Operation op, std::uint32_t row, Status status,
            std::string& error, const char* fallback) {
    result.status = status;
    result.failed_operation = op;
    result.failed_row = static_cast<std::int32_t>(row);
    if (error.empty()) error = fallback;
    return status;
}

} // namespace

struct Owner::RetainedRow {
    Row row;
    std::unique_ptr<OL::Runtime> runtime;
};

Owner::Owner() = default;
Owner::~Owner() = default;

std::int32_t Owner::dispatch_objective_member(
    void* raw, OL::List& list, const OL::MemberCall& call) {
    auto& owner = *static_cast<Owner*>(raw);
    if (!call.objective || !call.error) return 1;
    RetainedRow* retained = nullptr;
    for (const auto& candidate : owner.rows_) {
        if (&candidate->row.objectives == &list) {
            retained = candidate.get();
            break;
        }
    }
    if (!retained || !owner.services_.objective_member) {
        *call.error = "GameEvent ObjectiveList has no canonical retained-row member provider";
        return 1;
    }
    try {
        if (!owner.services_.objective_member(owner.services_.context,
                owner.level_identity_, owner.manager_identity_, retained->row,
                call, *call.error)) {
            if (call.error->empty()) *call.error = "GameEvent Objective member provider failed";
            return 1;
        }
    } catch (...) {
        *call.error = "GameEvent Objective member provider threw";
        return 1;
    }
    return 0;
}

Status Owner::initialize(std::uintptr_t level_identity,
                         std::uint32_t level_generation,
                         std::uintptr_t manager_identity,
                         const v2_event_table_v1::Table& table,
                         OL::Services objective_services, Services services,
                         Result* output, std::string& error) {
    if (!output || reinterpret_cast<std::uintptr_t>(output) % alignof(Result)) {
        error = "GameEventManager owner requires an aligned result destination";
        return Status::invalid_argument;
    }
    *output = {};
    error.clear();
    if (initialized_ || !rows_.empty() || busy_ || !level_identity || !manager_identity ||
        !objective_services.allocate || !objective_services.factory ||
        !objective_services.delete_virtual4 || !objective_services.deallocate ||
        !services.objective_member || table.rows.empty() ||
        table.rows.size() > static_cast<std::size_t>(INT32_MAX)) {
        output->status = initialized_ || busy_ ? Status::source_fault :
                         Status::owner_unavailable;
        error = "GameEventManager owner requires a new Level identity, parsed rows, Objective factory, and all source operation providers";
        return output->status;
    }

    level_identity_ = level_identity;
    level_generation_ = level_generation;
    manager_identity_ = manager_identity;
    objective_services_ = objective_services;
    const auto previous_services = services_;
    services_ = services;
    struct ServiceGuard {
        Services& target;
        Services previous;
        ~ServiceGuard() { target = previous; }
    } service_guard{services_, previous_services};
    try {
        table_ = table;
        rows_.reserve(table_.rows.size());
        for (std::uint32_t i = 0; i < table_.rows.size(); ++i) {
            auto retained = std::make_unique<RetainedRow>();
            retained->row.ordinal = i;
            retained->row.definition = &table_.rows[i];
            retained->row.identity = reinterpret_cast<std::uintptr_t>(&retained->row);
            // Retain the row before invoking providers so a partial source
            // prefix remains inspectable and can be explicitly closed.
            rows_.push_back(std::move(retained));
            auto& row = *rows_.back();
            auto list_services = objective_services_;
            list_services.member_context = this;
            list_services.member = dispatch_objective_member;
            row.runtime = std::make_unique<OL::Runtime>(
                row.row.objectives, list_services);
            OL::Result list_result{};
            if (row.runtime->construct(&list_result) != OL::Status::complete)
                return fail(*output, Operation::construct_list, i,
                            Status::service_failed, error,
                            "GameEvent ObjectiveList constructor failed");
            OL::Definition definition{};
            definition.event = row.row.definition;
            if (row.runtime->assign_pydata(
                    definition,
                    static_cast<std::int32_t>(row.row.definition->triggers.size()),
                    &list_result) != OL::Status::complete)
                return fail(*output, Operation::assign_event, i,
                            Status::service_failed, error,
                            "GameEvent ObjectiveList could not build from v2Event triggers");
            output->objectives_created += list_result.published;

            // GameEventManager::Load calls GameEvent::ReInit immediately
            // after AssignPyData for each newly allocated row.
            row.row.state = 0;
            if (row.runtime->reset_data(&list_result, error) != OL::Status::complete)
                return fail(*output, Operation::initial_reset, i,
                            Status::service_failed, error,
                            "GameEvent initial ObjectiveList reset failed");
            ++output->rows_reset;
        }
    } catch (...) {
        output->status = Status::service_failed;
        output->failed_operation = Operation::construct_list;
        output->failed_row = static_cast<std::int32_t>(rows_.size());
        error = "GameEventManager row retention allocation failed";
        return output->status;
    }
    initialized_ = true;
    output->status = Status::complete;
    output->failed_operation = Operation::complete;
    error.clear();
    return output->status;
}

Status Owner::reinit(std::uintptr_t expected_level_identity,
                     std::uint32_t expected_level_generation,
                     std::uintptr_t expected_manager_identity,
                     const Services& operation_services,
                     Result* output, std::string& error) {
    if (!output || reinterpret_cast<std::uintptr_t>(output) % alignof(Result)) {
        error = "GameEventManager::ReInit requires an aligned result destination";
        return Status::invalid_argument;
    }
    *output = {};
    error.clear();
    if (!initialized_ || busy_ || !level_identity_ || !manager_identity_ ||
        expected_level_identity != level_identity_ ||
        expected_level_generation != level_generation_ ||
        expected_manager_identity != manager_identity_) {
        output->status = busy_ ? Status::reentrant : Status::owner_unavailable;
        error = "GameEventManager::ReInit rejected a stale or mismatched Level generation/manager owner";
        return output->status;
    }
    if (!operation_services.objective_member) {
        output->status = Status::owner_unavailable;
        error = "GameEventManager::ReInit requires operation-scoped current-Level Objective providers";
        return output->status;
    }
    const auto previous_services = services_;
    services_ = operation_services;
    struct ServiceGuard {
        Services& target;
        Services previous;
        ~ServiceGuard() { target = previous; }
    } service_guard{services_, previous_services};
    busy_ = true;
    struct Guard { bool& busy; ~Guard() { busy = false; } } guard{busy_};
    for (std::uint32_t i = 0; i < rows_.size(); ++i) {
        auto& row = rows_[i]->row;
        auto& runtime = *rows_[i]->runtime;
        OL::Result list_result{};
        ++output->rows_visited;
        // GameEvent::ReInit stores state before dispatching ObjectiveList reset.
        row.state = 0;
        if (runtime.reset_data(&list_result, error) != OL::Status::complete)
            return fail(*output, Operation::reset_objectives, i,
                        Status::service_failed, error,
                        "GameEvent ObjectiveList reset failed");
        ++output->rows_reset;
        error.clear();
        if (runtime.invalidate_compile(&list_result, error) != OL::Status::complete)
            return fail(*output, Operation::invalidate_compile, i,
                        Status::service_failed, error,
                        "GameEvent ObjectiveList invalidate-compile failed");
        error.clear();
        if (runtime.compile(&list_result, error) != OL::Status::complete)
            return fail(*output, Operation::compile, i,
                        Status::service_failed, error,
                        "GameEvent ObjectiveList compile failed");
        ++output->rows_compiled;
        if (!row.registered) {
            error.clear();
            if (runtime.register_objectives(&list_result, error) != OL::Status::complete)
                return fail(*output, Operation::register_objectives, i,
                            Status::service_failed, error,
                            "GameEvent ObjectiveList register failed");
            row.registered = true;
            ++output->rows_registered;
        }
    }
    output->status = Status::complete;
    output->failed_operation = Operation::complete;
    error.clear();
    return output->status;
}

Status Owner::compile(std::uintptr_t expected_level_identity,
                      std::uint32_t expected_level_generation,
                      std::uintptr_t expected_manager_identity,
                      const Services& operation_services,
                      Result* output, std::string& error) {
    if (!output || reinterpret_cast<std::uintptr_t>(output) % alignof(Result)) {
        error = "GameEventManager::Compile requires an aligned result destination";
        return Status::invalid_argument;
    }
    *output = {};
    error.clear();
    if (!initialized_ || busy_ || expected_level_identity != level_identity_ ||
        expected_level_generation != level_generation_ ||
        expected_manager_identity != manager_identity_) {
        output->status = busy_ ? Status::reentrant : Status::owner_unavailable;
        error = "GameEventManager::Compile rejected a stale or mismatched Level generation/manager owner";
        return output->status;
    }
    if (!operation_services.objective_member) {
        output->status = Status::owner_unavailable;
        error = "GameEventManager::Compile requires operation-scoped current-Level Objective providers";
        return output->status;
    }
    const auto previous_services = services_;
    services_ = operation_services;
    struct ServiceGuard {
        Services& target;
        Services previous;
        ~ServiceGuard() { target = previous; }
    } service_guard{services_, previous_services};
    busy_ = true;
    struct Guard { bool& busy; ~Guard() { busy = false; } } guard{busy_};
    for (std::uint32_t i = 0; i < rows_.size(); ++i) {
        auto& row = rows_[i]->row;
        auto& runtime = *rows_[i]->runtime;
        OL::Result list_result{};
        ++output->rows_visited;
        if (runtime.invalidate_compile(&list_result, error) != OL::Status::complete)
            return fail(*output, Operation::invalidate_compile, i,
                        Status::service_failed, error,
                        "GameEvent ObjectiveList invalidate-compile failed");
        error.clear();
        if (runtime.compile(&list_result, error) != OL::Status::complete)
            return fail(*output, Operation::compile, i,
                        Status::service_failed, error,
                        "GameEvent ObjectiveList compile failed");
        ++output->rows_compiled;
        if (!row.registered) {
            error.clear();
            if (runtime.register_objectives(&list_result, error) != OL::Status::complete)
                return fail(*output, Operation::register_objectives, i,
                            Status::service_failed, error,
                            "GameEvent ObjectiveList register failed");
            row.registered = true;
            ++output->rows_registered;
        }
    }
    output->status = Status::complete;
    output->failed_operation = Operation::complete;
    error.clear();
    return output->status;
}

namespace {
struct UpdateCall {
    Owner* owner = nullptr;
    const UpdateServices* services = nullptr;
};
Row* update_row(UpdateCall& call, const v2_event_table_v1::Event& event,
                std::string& error) {
    if (!call.owner || !call.services) {
        error = "GameEvent update lost its retained owner or operation providers";
        return nullptr;
    }
    auto* row = call.owner->row_for(event);
    if (!row || row->definition != &event) {
        error = "GameEvent update row is not retained by the active Level owner";
        return nullptr;
    }
    return row;
}
bool update_trigger_supported(void* raw, const v2_event_table_v1::Event& event,
                              const v2_event_table_v1::Trigger& trigger,
                              bool& supported, std::string& error) {
    auto& call = *static_cast<UpdateCall*>(raw);
    supported = false;
    if (!update_row(call, event, error)) return false;
    switch (static_cast<v2_event_table_v1::ObjectiveType>(trigger.type)) {
    case v2_event_table_v1::ObjectiveType::kill_x_enemies:
    case v2_event_table_v1::ObjectiveType::move_in_zone:
    case v2_event_table_v1::ObjectiveType::talk_to_npc:
        supported = true;
        error.clear();
        return true;
    default:
        error.clear();
        return true;
    }
}
bool update_state(void* raw, const v2_event_table_v1::Event& event,
                  std::int32_t& state, std::string& error) {
    auto& call = *static_cast<UpdateCall*>(raw);
    auto* row = update_row(call, event, error);
    if (!row) return false;
    state = row->state;
    error.clear();
    return true;
}
bool update_valid(void* raw, const v2_event_table_v1::Event& event,
                  bool& valid, std::string& error) {
    auto& call = *static_cast<UpdateCall*>(raw);
    auto* row = update_row(call, event, error);
    return row && call.services->objective_list_valid(
        call.services->context, *row, valid, error);
}
bool update_eval(void* raw, const v2_event_table_v1::Event& event,
                 bool& complete, std::string& error) {
    auto& call = *static_cast<UpdateCall*>(raw);
    auto* row = update_row(call, event, error);
    return row && call.services->objective_list_eval(
        call.services->context, *row, complete, error);
}
bool update_set_state(void* raw, const v2_event_table_v1::Event& event,
                      std::int32_t state, std::string& error) {
    auto& call = *static_cast<UpdateCall*>(raw);
    auto* row = update_row(call, event, error);
    if (!row) return false;
    const auto* table = call.owner->table();
    if (!table || (state != table->states.inactive &&
                   state != table->states.active &&
                   state != table->states.completed)) {
        error = "GameEvent::SetState received an unknown retained v2EventState";
        return false;
    }
    row->state = state;
    error.clear();
    return true;
}
bool update_start_script(void* raw, const v2_event_table_v1::Event& event,
                         const std::string& script, std::int32_t argument,
                         bool check_running, bool online, bool& started,
                         std::string& error) {
    auto& call = *static_cast<UpdateCall*>(raw);
    if (!update_row(call, event, error)) return false;
    return call.services->start_script(call.services->context, event, script,
        argument, check_running, online, started, error);
}
} // namespace

v2_event_update_kernel_v1::Status Owner::update(
    std::uintptr_t expected_level_identity,
    std::uint32_t expected_level_generation,
    std::uintptr_t expected_manager_identity,
    const UpdateServices& services,
    v2_event_update_kernel_v1::Result* output, std::string& error) {
    if (!output || reinterpret_cast<std::uintptr_t>(output) %
                       alignof(v2_event_update_kernel_v1::Result)) {
        error = "GameEventManager::Update requires an aligned result destination";
        return v2_event_update_kernel_v1::Status::invalid_argument;
    }
    error.clear();
    if (!initialized_ || busy_ || expected_level_identity != level_identity_ ||
        expected_level_generation != level_generation_ ||
        expected_manager_identity != manager_identity_) {
        *output = {};
        output->status = v2_event_update_kernel_v1::Status::service_unavailable;
        error = "GameEventManager::Update rejected a stale or mismatched Level generation/manager owner";
        return output->status;
    }
    if (!services.objective_list_valid || !services.objective_list_eval ||
        !services.start_script) {
        *output = {};
        output->status = v2_event_update_kernel_v1::Status::service_unavailable;
        error = "GameEventManager::Update requires same-Level ObjectiveList and script providers";
        return output->status;
    }
    busy_ = true;
    struct Guard { bool& busy; ~Guard() { busy = false; } } guard{busy_};
    UpdateCall call{this, &services};
    v2_event_update_kernel_v1::Services kernel{};
    kernel.context = &call;
    kernel.trigger_supported = update_trigger_supported;
    kernel.state = update_state;
    kernel.objective_list_valid = update_valid;
    kernel.objective_list_eval = update_eval;
    kernel.set_state = update_set_state;
    kernel.start_script = update_start_script;
    return v2_event_update_kernel_v1::update(table_, kernel, output, error);
}

Status Owner::close(Result* output, std::string& error) {
    if (!output || reinterpret_cast<std::uintptr_t>(output) % alignof(Result)) {
        error = "GameEventManager close requires an aligned result destination";
        return Status::invalid_argument;
    }
    *output = {};
    error.clear();
    if (busy_) {
        output->status = Status::reentrant;
        error = "GameEventManager owner is busy";
        return output->status;
    }
    busy_ = true;
    struct Guard { bool& busy; ~Guard() { busy = false; } } guard{busy_};
    for (std::uint32_t i = 0; i < rows_.size(); ++i) {
        OL::Result list_result{};
        if (rows_[i]->runtime->destroy(&list_result) != OL::Status::complete) {
            output->status = Status::service_failed;
            output->failed_operation = Operation::close_list;
            output->failed_row = static_cast<std::int32_t>(i);
            error = "GameEvent ObjectiveList destruction failed";
            return output->status;
        }
    }
    rows_.clear();
    table_ = {};
    level_identity_ = manager_identity_ = 0;
    level_generation_ = 0;
    initialized_ = false;
    output->status = Status::complete;
    output->failed_operation = Operation::complete;
    error.clear();
    return output->status;
}

Row* Owner::row(std::size_t ordinal) noexcept {
    return ordinal < rows_.size() ? &rows_[ordinal]->row : nullptr;
}
const Row* Owner::row(std::size_t ordinal) const noexcept {
    return ordinal < rows_.size() ? &rows_[ordinal]->row : nullptr;
}
Row* Owner::row_for(const v2_event_table_v1::Event& event) noexcept {
    for (const auto& retained : rows_)
        if (retained->row.definition == &event) return &retained->row;
    return nullptr;
}
const Row* Owner::row_for(const v2_event_table_v1::Event& event) const noexcept {
    for (const auto& retained : rows_)
        if (retained->row.definition == &event) return &retained->row;
    return nullptr;
}

} // namespace dh2::data::game_event_manager_owner_v1
