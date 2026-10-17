#include "ais_combat_result_dispatch_v1.hpp"

namespace dh2::ais_combat_result_dispatch_v1 {
namespace {
constexpr std::uint32_t on_target_hit = 0x800;
constexpr std::uint32_t on_target_missed = 0x1000;
constexpr std::uint32_t suppress_ai_callbacks = 0x20000000;

bool valid(const Actor* actor) {
    return actor && actor->character && (!actor->ais || actor->flags_b8);
}
}

Status dispatch(const Arguments* args, const Services* services, Report* report) {
    if (!args || !services || !report || !args->attacker || !args->defender ||
        !args->result || args->attacker == args->defender ||
        !valid(args->attacker) || !valid(args->defender) ||
        args->attacker->character == args->defender->character)
        return Status::invalid_argument;

    *report = {};
    if (args->result->mask & suppress_ai_callbacks) return Status::complete;

    const bool missed = (args->result->outcomes & 3u) != 0;
    report->selected = missed ? Callback::target_missed : Callback::target_hit;
    const std::uint32_t membership = missed ? on_target_missed : on_target_hit;

    // Native F_ApplyResult calls CharAI::OnCombatResults first for the
    // attacker, then for the defender. Read each owner's flags at its turn.
    // AISDefault ignores LuaScript::Call's status, so preserve the second
    // source callback even after a failed or unavailable first dispatch.
    Actor* actors[] = {args->attacker, args->defender};
    bool failed = false;
    for (Actor* actor : actors) {
        if (!actor->ais || !(*actor->flags_b8 & membership)) continue;
        ++report->attempts;
        if (!services->dispatch) {
            ++report->failures;
            report->last_status = static_cast<std::int32_t>(Status::service_unavailable);
            failed = true;
            continue;
        }
        std::int32_t status = 0;
        try {
            status = services->dispatch(services->context,
                actor->ais, report->selected, args->attacker->character,
                args->defender->character);
        } catch (...) {
            status = -1;
        }
        if (status) {
            report->last_status = status;
            ++report->failures;
            failed = true;
        } else {
            ++report->callbacks;
        }
    }
    return failed ? Status::service_failed : Status::complete;
}
} // namespace dh2::ais_combat_result_dispatch_v1
