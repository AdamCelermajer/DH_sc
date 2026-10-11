#include "character_faery_coordinator_v1.hpp"

namespace dh2::character_faery_coordinator_v1 {

Status bind(factory::Record& record, const Bindings& bindings,
            std::string& error) {
    error.clear();
    auto* const coordinator = record.coordinator.get();
    auto* const ai = record.canonical_ai();
    const auto identity = record.character.identity;
    using Component = factory::ctor::Component;
    if (!identity || !coordinator || !ai ||
        record.constructor.identity != identity ||
        !record.constructor.constructor_complete ||
        record.game_object.identity != identity ||
        !record.object_registered || !record.character_listed ||
        coordinator->owner() != identity ||
        record.components.find(Component::timers) != coordinator ||
        record.components.find(Component::state_machine) != coordinator) {
        error = "Faery Coordinator binding requires the completed canonical Character Record and timer/state slots";
        return Status::invalid_record;
    }
    const auto& controller = record.faery_controller;
    if (controller.character != identity || !controller.controller ||
        !controller.controllable ||
        record.components.find(Component::controller) !=
            reinterpret_cast<void*>(controller.controller) ||
        record.components.find(Component::controllable) !=
            reinterpret_cast<void*>(controller.controllable) ||
        ai->owner_04 != identity || !ai->identity) {
        error = "Faery Coordinator AI/controller projections do not match the same Record";
        return Status::owner_mismatch;
    }
    const bool has_frame_binding = bindings.ai_identity ||
        bindings.ai_owner_projection || bindings.controller_identity;
    if (has_frame_binding &&
        (bindings.ai_identity != ai->identity ||
         bindings.controller_identity != controller.controller ||
         !bindings.ai_owner_projection ||
         bindings.ai_owner_projection->owner != identity ||
         bindings.ai_owner_projection->controller != controller.controller ||
         bindings.ai_owner_projection->flags520 != coordinator->state.flags ||
         bindings.ai_owner_projection->locked != coordinator->state.controller_locked)) {
        error = "Faery CharAI frame projection does not match the canonical Record, AI, and controller";
        return Status::owner_mismatch;
    }
    if (!bindings.context || !bindings.facts || !bindings.services.context ||
        !bindings.services.invoke || !bindings.route_timer_event) {
        error = "Faery Coordinator requires live Character facts, state services, and retained AI timer routing";
        return Status::service_unavailable;
    }
    if (coordinator->bound()) {
        error = "Faery Record Coordinator was already bound; refusing to replace its callback graph";
        return Status::already_bound;
    }
    try {
        coordinator->bind(bindings);
    } catch (...) {
        error = "Faery Record Coordinator rejected its live provider bindings";
        return Status::bind_failed;
    }
    if (!coordinator->bound()) {
        error = "Faery Record Coordinator did not retain the supplied providers";
        return Status::bind_failed;
    }
    return Status::complete;
}

} // namespace dh2::character_faery_coordinator_v1
