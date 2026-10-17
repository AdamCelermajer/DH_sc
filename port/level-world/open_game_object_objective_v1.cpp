#include "open_game_object_objective_v1.hpp"

namespace dh2::level_world::open_game_object_objective_v1 {
namespace of = data::quest_objective_factory_v1;

namespace {
bool coherent(const Objective& objective) {
    return objective.ref.action.identity &&
           objective.ref.fields == &objective.fields &&
           objective.ref.action.character_10 == &objective.fields.character_10 &&
           objective.dispatch_0 == of::Dispatch::open_game_object;
}

}

bool compile(Objective& objective, const dh2_quest_objective& definition,
             std::int32_t event_type, const Services& services,
             Binding& binding, std::string& error) {
    if (!coherent(objective) || definition.common[0] != objective.fields.type_4) {
        error = "OpenGameObject Compile requires its canonical factory Record and retained pydata selector";
        return false;
    }
    if (!services.complete || !services.events || event_type < 0) {
        error = "OpenGameObject Compile requires current-Level completion and EventManager providers";
        return false;
    }

    // IDA 0x47ec30: pydata+0x20 is copied to Objective+0x24; pydata+0x28
    // supplies the required event count. The common Objective compile state
    // is set only when the target is not -1.
    const auto target = definition.args[0];
    objective.derived_24 = target;
    binding.objective = &objective;
    binding.event_type = event_type;
    binding.target_id = target;
    binding.required_count = definition.args[2];
    binding.services = services;

    if (target != -1) {
        objective.compiled_8 = 1;
        if (static_cast<std::int32_t>(objective.quantity_20) >= binding.required_count) {
            // Objective::SetIsCompleted (IDA 0x47ba10) sets done_14 before
            // running the source script; this callback operates on this same
            // canonical Objective and preserves that ordering.
            if (!services.complete(services.context, objective, error)) {
                if (error.empty()) error = "OpenGameObject SetIsCompleted provider failed";
                return false;
            }
            // Compile's vtable+0x1c call is Objective_EventReceiver::Unregister
            // (IDA 0x47adbc), not Register.
            if (!unregister_objective(objective, binding, error)) return false;
        }
    }
    error.clear();
    return true;
}

bool register_objective(Objective& objective, Binding& binding,
                        std::string& error) {
    if (!coherent(objective) || !objective.compiled_8 ||
        binding.objective != &objective || !binding.services.events ||
        binding.event_type < 0 || binding.target_id == -1) {
        error = "OpenGameObject Register requires its compiled canonical Objective and current-Level EventManager";
        return false;
    }
    bool attached = false;
    const auto status = binding.services.events->attach(
        binding.event_type, objective.ref.action.identity, 0, &binding,
        receive, attached, error);
    if (status != current_level_quest_event_v1::Status::complete) {
        if (error.empty()) error = "OpenGameObject EventManager::Attach failed";
        return false;
    }
    // The source stores its registered byte after Attach, even when Attach
    // returns zero for an already attached receiver.
    binding.registered = true;
    error.clear();
    return true;
}

bool unregister_objective(Objective& objective, Binding& binding,
                          std::string& error) {
    if (!coherent(objective) || binding.objective != &objective ||
        !binding.services.events || binding.event_type < 0) {
        error = "OpenGameObject Unregister requires its current-Level EventManager binding";
        return false;
    }
    bool scheduled = false;
    const auto status = binding.services.events->delayed_detach(
        binding.event_type, objective.ref.action.identity, scheduled, error);
    if (status != current_level_quest_event_v1::Status::complete) {
        if (error.empty()) error = "OpenGameObject EventManager::DelayedDetach failed";
        return false;
    }
    // Objective_EventReceiver clears its registered byte after the call;
    // delayed delivery remains queued in EventManager until its normal flush.
    binding.registered = false;
    binding.detach_pending = scheduled;
    error.clear();
    return true;
}

std::int32_t receive(void* raw, EventRuntime&, Event& event,
                     std::string& error) {
    if (!raw) {
        error = "OpenGameObject receiver context is null";
        return 1;
    }
    auto& binding = *static_cast<Binding*>(raw);
    auto* objective = binding.objective;
    if (!objective || !coherent(*objective) || !objective->compiled_8 ||
        objective->derived_24.index() != 0 ||
        std::get<std::int32_t>(objective->derived_24) != binding.target_id) {
        error = "OpenGameObject receiver lost its canonical compiled Objective/target";
        return 1;
    }

    // The current-level event carrier's network_id/flag1/subject_id/flag0
    // project the source IEvent words +0x18/+0x11/+0x14/+0x10 respectively.
    if (event.objective_type != binding.event_type ||
        event.network_id != binding.target_id) return 0;
    if (event.flag1) {
        if (static_cast<std::int32_t>(objective->quantity_20) >= event.subject_id)
            return 0;
        objective->quantity_20 = static_cast<std::uint32_t>(event.subject_id);
    } else {
        ++objective->quantity_20;
        event.flag0 = 1;
        event.subject_id = static_cast<std::int32_t>(objective->quantity_20);
    }
    if (static_cast<std::int32_t>(objective->quantity_20) < binding.required_count) {
        error.clear();
        return 0;
    }
    if (!binding.services.complete ||
        !binding.services.complete(binding.services.context, *objective, error)) {
        if (error.empty()) error = "OpenGameObject SetIsCompleted provider failed";
        return 1;
    }
    // The source handler calls its vtable+0x1c member after completion.
    if (!unregister_objective(*objective, binding, error)) return 1;
    error.clear();
    return 0;
}

bool marker(Objective& objective, MarkerOperation, std::string& error) {
    if (!coherent(objective)) {
        error = "OpenGameObject marker call requires its canonical Objective";
        return false;
    }
    // Objective::InstallObjectiveMarker (0x479f80) and
    // Objective::RemoveObjectiveMarker (0x479f84) are both bx lr.
    error.clear();
    return true;
}

}  // namespace dh2::level_world::open_game_object_objective_v1
