#include "v2_event_update_kernel_v1.hpp"

#include <exception>

namespace dh2::data::v2_event_update_kernel_v1 {
namespace {
bool output_valid(const Result* out) noexcept {
    return out && reinterpret_cast<std::uintptr_t>(out) % alignof(Result) == 0;
}
Status fail(Result& result, Stage stage, std::uint32_t row, bool committed,
            Status status, std::string& error, const char* fallback) {
    result.status = status;
    result.failed_stage = stage;
    result.failed_row = static_cast<std::int32_t>(row);
    result.state_committed_before_failure = committed;
    if (error.empty()) error = fallback;
    return status;
}
}

Status update(const v2_event_table_v1::Table& table, const Services& services,
              Result* out, std::string& error) {
    if (!output_valid(out)) return Status::invalid_argument;
    *out = {};
    if (table.rows.empty() || table.states.active != 1 ||
        table.states.completed != 2 || table.states.count != 3 ||
        table.states.inactive != 0 ||
        !services.trigger_supported || !services.state ||
        !services.objective_list_valid || !services.objective_list_eval ||
        !services.set_state || !services.start_script) {
        out->status = services.trigger_supported && services.state &&
                      services.objective_list_valid && services.objective_list_eval &&
                      services.set_state && services.start_script
                    ? Status::invalid_argument : Status::service_unavailable;
        error = "v2Event update requires a nonempty parsed table and every borrowed owner callback";
        return out->status;
    }

    error.clear();
    // Do not mutate any GameEvent until every ObjectiveList type can be served
    // by the existing canonical Objective/Quest runtime.
    for (std::uint32_t i = 0; i < table.rows.size(); ++i) {
        for (const auto& trigger : table.rows[i].triggers) {
            bool supported = false;
            try {
                if (!services.trigger_supported(services.context, table.rows[i],
                                                trigger, supported, error))
                    return fail(*out, Stage::trigger_preflight, i, false,
                                Status::service_failed, error,
                                "v2Event trigger support provider failed");
            } catch (...) {
                return fail(*out, Stage::trigger_preflight, i, false,
                            Status::service_failed, error,
                            "v2Event trigger support provider threw");
            }
            if (!error.empty())
                return fail(*out, Stage::trigger_preflight, i, false,
                            Status::service_failed, error,
                            "v2Event trigger support provider failed");
            if (!supported) {
                error = "v2Event table contains a trigger without a bound canonical Objective implementation";
                return fail(*out, Stage::trigger_preflight, i, false,
                            Status::unsupported_trigger, error,
                            "unsupported v2Event trigger");
            }
        }
    }

    for (std::uint32_t i = 0; i < table.rows.size(); ++i) {
        const auto& row = table.rows[i];
        ++out->rows_visited;
        std::int32_t state = 0;
        try {
            if (!services.state(services.context, row, state, error))
                return fail(*out, Stage::read_state, i, false,
                            Status::service_failed, error,
                            "v2Event state provider failed");
        } catch (...) {
            return fail(*out, Stage::read_state, i, false,
                        Status::service_failed, error,
                        "v2Event state provider threw");
        }
        if (!error.empty())
            return fail(*out, Stage::read_state, i, false,
                        Status::service_failed, error,
                        "v2Event state provider failed");

        if (state == table.states.inactive) {
            bool valid = false;
            try {
                if (!services.objective_list_valid(services.context, row, valid, error))
                    return fail(*out, Stage::objective_valid, i, false,
                                Status::service_failed, error,
                                "v2Event ObjectiveList::IsValid provider failed");
            } catch (...) {
                return fail(*out, Stage::objective_valid, i, false,
                            Status::service_failed, error,
                            "v2Event ObjectiveList::IsValid provider threw");
            }
            if (!error.empty())
                return fail(*out, Stage::objective_valid, i, false,
                            Status::service_failed, error,
                            "v2Event ObjectiveList::IsValid provider failed");
            if (!valid) continue;
            try {
                if (!services.set_state(services.context, row, table.states.active, error))
                    return fail(*out, Stage::set_active, i, false,
                                Status::service_failed, error,
                                "v2Event SetState(Active) provider failed");
            } catch (...) {
                return fail(*out, Stage::set_active, i, false,
                            Status::service_failed, error,
                            "v2Event SetState(Active) provider threw");
            }
            if (!error.empty())
                return fail(*out, Stage::set_active, i, false,
                            Status::service_failed, error,
                            "v2Event SetState(Active) provider failed");
            ++out->rows_activated;
            out->state_committed_before_failure = false;
            continue;
        }

        if (state != table.states.active) continue;
        bool complete = false;
        try {
            if (!services.objective_list_eval(services.context, row, complete, error))
                return fail(*out, Stage::objective_eval, i, false,
                            Status::service_failed, error,
                            "v2Event ObjectiveList::Eval provider failed");
        } catch (...) {
            return fail(*out, Stage::objective_eval, i, false,
                        Status::service_failed, error,
                        "v2Event ObjectiveList::Eval provider threw");
        }
        if (!error.empty())
            return fail(*out, Stage::objective_eval, i, false,
                        Status::service_failed, error,
                        "v2Event ObjectiveList::Eval provider failed");
        if (!complete) continue;

        try {
            if (!services.set_state(services.context, row, table.states.completed, error))
                return fail(*out, Stage::set_completed, i, false,
                            Status::service_failed, error,
                            "v2Event SetState(Completed) provider failed");
        } catch (...) {
            return fail(*out, Stage::set_completed, i, false,
                        Status::service_failed, error,
                        "v2Event SetState(Completed) provider threw");
        }
        if (!error.empty())
            return fail(*out, Stage::set_completed, i, false,
                        Status::service_failed, error,
                        "v2Event SetState(Completed) provider failed");
        ++out->rows_completed;
        out->state_committed_before_failure = true;

        bool started = false;
        try {
            // Source GameEvent::ExecScript uses arg=-1 and check-running=false.
            if (!services.start_script(services.context, row, row.script, -1,
                                       false, false, started, error))
                return fail(*out, Stage::start_script, i, true,
                            Status::service_failed, error,
                            "v2Event completion script provider failed after state commit");
        } catch (...) {
            return fail(*out, Stage::start_script, i, true,
                        Status::service_failed, error,
                        "v2Event completion script provider threw after state commit");
        }
        if (!error.empty())
            return fail(*out, Stage::start_script, i, true,
                        Status::service_failed, error,
                        "v2Event completion script provider failed after state commit");
        out->scripts_started += started ? 1u : 0u;
        out->state_committed_before_failure = false;
    }
    out->status = Status::complete;
    out->failed_stage = Stage::none;
    out->failed_row = -1;
    out->state_committed_before_failure = false;
    error.clear();
    return Status::complete;
}

} // namespace dh2::data::v2_event_update_kernel_v1
