#include "player_script_timer_dispatch_v1.hpp"
#include "character_ai_events.hpp"

namespace dh2::player_script_timer_dispatch_v1 {
namespace {
struct Call {
    const Bindings* bindings = nullptr;
    std::uint32_t timer_id = 0;
    unsigned calls = 0;
    bool foreign_ais = false;
    std::string error;
};

std::uint8_t ais_timer_slot_token;

std::int32_t invoke(void* raw, character::AIEventState64* state,
                    const character::AIEventRequest40* request,
                    std::uint32_t*) {
    auto& call = *static_cast<Call*>(raw);
    if (!state || !request || !call.bindings ||
        request->service != character::ai_event_ais_virtual ||
        request->operation != 0x90 || request->event != 0x35 ||
        request->argument != call.timer_id ||
        request->callee != reinterpret_cast<std::uintptr_t>(&ais_timer_slot_token) ||
        request->payload != 0 || state->ai != call.bindings->ai_identity)
        return 1;
    if (request->subject != call.bindings->retained_ais ||
        state->active != call.bindings->retained_ais) {
        call.foreign_ais = true;
        call.error = "active AIS is not the retained Player AISDefault";
        return 1;
    }
    ++call.calls;
    const auto status = dh2_script_game_on_timer(call.bindings->vm, call.timer_id);
    if (status) {
        call.error = dh2_script_vm_error(call.bindings->vm);
        if (call.error.empty()) call.error = "retained Player OnTimer callback failed";
        return 1;
    }
    return 0;
}
}

Status dispatch(const Bindings& bindings, std::uint32_t timer_id,
                std::string& error) {
    error.clear();
    if (!bindings.character || !bindings.ai_identity ||
        !bindings.retained_ais || !bindings.ai ||
        bindings.ai->identity != bindings.ai_identity ||
        bindings.ai->owner_04 != bindings.character ||
        !bindings.coordinator || bindings.coordinator->owner() != bindings.character ||
        !bindings.controller || !bindings.properties ||
        !bindings.coordinator->bound() || !bindings.vm) {
        error = "Player OnScriptTimer Character, CharAI, AIS, Coordinator, or VM identity differs";
        return Status::invalid_owner;
    }

    const auto& timers = bindings.coordinator->timers();
    if (!timers.slots || timer_id >= timers.count ||
        timers.slots[timer_id].id != timer_id ||
        timers.slots[timer_id].event != 0x35 ||
        timers.slots[timer_id].user_ref != 0) {
        error = "Player OnScriptTimer timer is not the retained event-0x35 slot";
        return Status::invalid_owner;
    }

    std::uintptr_t ais_methods[51]{};
    ais_methods[0x90/4] =
        reinterpret_cast<std::uintptr_t>(&ais_timer_slot_token);
    character::AIEventOwner48 owner{
        bindings.character, bindings.controller,
        reinterpret_cast<std::uintptr_t>(&bindings.coordinator->state),
        bindings.properties,0,0,0,0};
    character::AIEventState64 state{
        bindings.ai_identity,&owner,nullptr,bindings.ai->active_ais_1c,
        ais_methods,bindings.ai->paused_18,0,0,0,0,0};
    character::AIEventServices24 services{
        nullptr,nullptr,0,0};
    Call call{&bindings,timer_id,0,false,{}};
    services={&call,invoke,1u<<character::ai_event_ais_virtual,0};
    character::AIEventResult16 result{};
    const auto status=dh2_character_ai_event_script_timer(
        &result,&state,timer_id,&services);
    if (status) {
        error=call.error.empty()?"CharAI OnScriptTimer/AIS OnScriptTimer dispatch failed":call.error;
        return call.foreign_ais?Status::unavailable:Status::failed;
    }
    if (!call.calls) return Status::inactive_ais;
    return Status::delivered;
}

} // namespace dh2::player_script_timer_dispatch_v1
