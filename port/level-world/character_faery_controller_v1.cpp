#include "character_faery_controller_v1.hpp"

#include "character_runtime_factory_v1.hpp"

namespace dh2::character_faery_controller_v1 {
namespace {

bool valid_record(const factory::Record& record) noexcept {
    const auto identity = record.character.identity;
    return identity && record.constructor.identity == identity &&
        record.game_object.identity == identity && record.object_registered &&
        record.character_listed;
}

} // namespace

Status bind(factory::Record& record, std::uintptr_t controller,
            std::uintptr_t controllable, std::string& error) {
    error.clear();
    if (!valid_record(record) || !controller || !controllable) {
        error = "Faery controller requires a registered canonical Character Record";
        return Status::invalid_record;
    }
    using Component = factory::ctor::Component;
    if (record.faery_controller.character) {
        error = "Faery Character Record already has a controller binding";
        return Status::owner_mismatch;
    }
    if (record.components.find(Component::controller) !=
            reinterpret_cast<void*>(controller) ||
        record.components.find(Component::controllable) !=
            reinterpret_cast<void*>(controllable)) {
        error = "Faery controller binding differs from canonical component slots";
        return Status::owner_mismatch;
    }
    record.faery_controller = {record.character.identity, controller,
                               controllable, 0, 0, 0};
    return Status::complete;
}

Status point_command(factory::Record& record, PointCommand command,
                     const float point[3], std::uint32_t global_blocked,
                     std::uint32_t locked, std::uint32_t forced,
                     const PointServices& services, Result* output,
                     std::string& error) {
    error.clear();
    if (output) *output = {};
    const auto& state = record.faery_controller;
    if (!output || !valid_record(record) || !state.character ||
        state.character != record.character.identity || !state.controller ||
        !state.controllable || !point || command > character::controller_head_to_point) {
        error = "Faery point command requires the canonical bound controller and Character";
        return Status::invalid_record;
    }
    if (!services.invoke) {
        error = "Faery point-controllable provider is unavailable";
        return Status::service_unavailable;
    }
    auto& live = record.faery_controller;
    live.global_blocked = global_blocked;
    live.locked = locked;
    live.forced = forced;
    output->command = static_cast<std::uint32_t>(command);
    output->character = live.character;
    output->controller = live.controller;
    output->controllable = live.controllable;
    const character::ControllerCommandState32 gate{live.controller,
        live.character, live.global_blocked, live.locked, live.forced, 0};
    const int status = dh2_character_controller_point_command(
        &gate, static_cast<std::uint32_t>(command), point, &services);
    output->source_return = static_cast<std::uint32_t>(status);
    output->dispatched = status == 1 && (live.forced ||
        (!live.global_blocked && !live.locked));
    if (status != 1) {
        error = status < 0 ? "Faery source point-controllable dispatch failed"
                           : "Faery source controller command was rejected";
        return Status::command_failed;
    }
    return Status::complete;
}

void retire(State& state) noexcept { state = {}; }

} // namespace dh2::character_faery_controller_v1
