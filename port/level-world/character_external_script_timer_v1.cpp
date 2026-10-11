#include "character_external_script_timer_v1.hpp"

#include "character_ai_events.hpp"
#include "monster_external_script_session.hpp"

namespace dh2::character_external_script_timer_v1 {
namespace {

std::uint8_t ais_timer_slot_token;

int discard_returns(void*, const dh2_script_first_return_v1*, std::uint32_t,
                    char*, std::size_t) {
    return 0;
}

struct Call {
    const Bindings* bindings{};
    std::uint32_t timer_id{};
    Result* result{};
    std::string* error{};
};

std::int32_t invoke(void* raw, character::AIEventState64* state,
                    const character::AIEventRequest40* request,
                    std::uint32_t*) {
    auto& call = *static_cast<Call*>(raw);
    const auto& bindings = *call.bindings;
    if (!state || !request || !call.result || !call.error ||
        request->service != character::ai_event_ais_virtual ||
        request->operation != 0x90 || request->event != 0x35 ||
        request->argument != call.timer_id ||
        request->callee != reinterpret_cast<std::uintptr_t>(&ais_timer_slot_token) ||
        request->payload || state->ai != bindings.ai_identity ||
        request->subject != bindings.retained_ais ||
        state->active != bindings.retained_ais) {
        if (call.error) *call.error = "external AIS OnScriptTimer identity or slot differs";
        return 1;
    }

    dh2_script_value argument{};
    argument.type = DH2_SCRIPT_NUMBER;
    argument.number = static_cast<float>(call.timer_id);
    ++call.result->callback_calls;
    const int status = bindings.session->call_all("OnTimer", &argument, 1,
        discard_returns, nullptr, *call.error);
    return status == 0 ? 0 : 1;
}

} // namespace

Status dispatch(const Bindings& bindings, std::uint32_t timer_id,
                Result* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output ||
        (bindings.kind != AISKind::default_ais &&
         bindings.kind != AISKind::external_ais) ||
        !bindings.character || !bindings.ai_identity ||
        !bindings.retained_ais || !bindings.controller ||
        !bindings.properties || !bindings.ai ||
        bindings.ai->identity != bindings.ai_identity ||
        bindings.ai->owner_04 != bindings.character ||
        !bindings.coordinator || !bindings.coordinator->bound() ||
        bindings.coordinator->owner() != bindings.character ||
        !bindings.session || !bindings.session->ready() ||
        !bindings.session->vm_identity()) {
        error = "external OnScriptTimer Character, CharAI, AIS, Coordinator, or Session identity differs";
        return Status::invalid_owner;
    }
    if (bindings.ai->active_ais_1c != bindings.retained_ais) {
        if (!bindings.ai->active_ais_1c) {
            output->inactive_ais = true;
            return Status::inactive_ais;
        }
        error = "active AIS is not the retained external Session AIS";
        return Status::invalid_owner;
    }

    const auto& timers = bindings.coordinator->timers();
    if (!timers.slots || timer_id >= timers.count ||
        timers.slots[timer_id].id != timer_id ||
        timers.slots[timer_id].event != 0x35) {
        error = "external OnScriptTimer requires this Character's expired event-0x35 slot";
        return Status::invalid_owner;
    }

    character::AIEventOwner48 owner{
        bindings.character, bindings.controller,
        reinterpret_cast<std::uintptr_t>(&bindings.coordinator->state),
        bindings.properties, 0, 0, 0, 0};
    std::uintptr_t ais_methods[51]{};
    ais_methods[0x90 / 4] =
        reinterpret_cast<std::uintptr_t>(&ais_timer_slot_token);
    character::AIEventState64 state{
        bindings.ai_identity, &owner, nullptr, bindings.retained_ais,
        ais_methods, bindings.ai->paused_18, 0, 0, 0, 0, 0};
    Call call{&bindings, timer_id, output, &error};
    const character::AIEventServices24 services{
        &call, invoke, 1u << character::ai_event_ais_virtual, 0};
    character::AIEventResult16 event_result{};
    const int status = dh2_character_ai_event_script_timer(
        &event_result, &state, timer_id, &services);
    output->event_service_calls = event_result.service_calls;
    if (status) {
        if (error.empty()) error = "source CharAI OnScriptTimer/AIS OnScriptTimer dispatch failed";
        return status == 2 ? Status::unavailable : Status::failed;
    }
    if (!output->callback_calls) {
        output->inactive_ais = true;
        return Status::inactive_ais;
    }
    return Status::delivered;
}

} // namespace dh2::character_external_script_timer_v1
