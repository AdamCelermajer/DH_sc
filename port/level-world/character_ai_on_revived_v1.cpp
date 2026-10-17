#include "character_ai_on_revived_v1.hpp"

#include <limits>

namespace dh2::character_ai_on_revived_v1 { namespace {

bool identity(const character_ai_initialization::State* ai,
              const character::Coordinator* coordinator,
              std::uintptr_t character, std::uintptr_t ai_identity,
              std::uintptr_t active_ais, std::uintptr_t character_expected,
              std::uintptr_t ai_expected, std::uintptr_t active_expected) {
    return ai && coordinator && character && ai_identity &&
        character == character_expected && ai_identity == ai_expected &&
        active_ais == active_expected && ai->identity == ai_identity &&
        ai->owner_04 == character && coordinator->bound() &&
        coordinator->owner() == character;
}

Status invoke(const Services& services, const Request& request, Reply& reply,
              Result& result, std::string& error) {
    result.last_operation = static_cast<std::uint32_t>(request.operation);
    ++result.calls;
    if (!services.invoke) {
        error = "CharAI::OnRevived reached provider is unavailable";
        return Status::service_unavailable;
    }
    reply = {};
    reply.timer_id = -1;
    try {
        if (services.invoke(services.context, &request, &reply, error)) {
            if (error.empty()) error = "CharAI::OnRevived provider failed";
            return Status::service_failed;
        }
    } catch (...) {
        error = "CharAI::OnRevived provider threw";
        return Status::service_failed;
    }
    return Status::complete;
}

} // namespace

Status execute(character_ai_initialization::State* ai,
    character::Coordinator* coordinator, std::uintptr_t expected_character,
    std::uintptr_t expected_ai, std::uintptr_t expected_active_ais,
    const Services& services, Result* out, std::string& error) {
    error.clear();
    if (!ai || !coordinator || !out || !expected_character || !expected_ai ||
        ai->identity != expected_ai || ai->owner_04 != expected_character ||
        coordinator->owner() != expected_character || !coordinator->bound() ||
        ai->active_ais_1c != expected_active_ais) {
        error = "CharAI::OnRevived requires the same active Character/AIS/Coordinator";
        return Status::invalid_argument;
    }

    const auto character = expected_character;
    const auto ai_identity = expected_ai;
    const auto active_ais = expected_active_ais;
    *out = {};
    out->character = character;
    out->ai = ai_identity;
    out->active_ais = active_ais;

    const auto coherent = [&] {
        return identity(ai, coordinator, character, ai->identity,
            ai->active_ais_1c, character, ai_identity, active_ais);
    };
    auto call = [&](Operation operation, std::uint32_t duration = 0,
                    std::int32_t repeat = 0, std::int32_t event = 0,
                    Reply* received = nullptr) {
        if (!coherent()) {
            error = "CharAI::OnRevived borrowed owner identity changed";
            return Status::invalid_argument;
        }
        Request request{operation, character, ai_identity, active_ais,
                        duration, repeat, event};
        Reply reply{};
        const auto status = invoke(services, request, reply, *out, error);
        if (status != Status::complete) return status;
        if (!coherent()) {
            error = "CharAI::OnRevived provider changed the active owner";
            return Status::invalid_argument;
        }
        if (received) *received = reply;
        return Status::complete;
    };

    Reply reply{};
    auto status = call(Operation::stop_all_timers, 0, 0, 0, &reply);
    if (status != Status::complete) return status;
    out->timers_stopped = 1;

    status = call(Operation::get_ai_tick, 0, 0, 0, &reply);
    if (status != Status::complete) return status;
    const auto ai_tick_ms = reply.duration_ms;

    status = call(Operation::start_ai_tick, ai_tick_ms, -1, 51, &reply);
    if (status != Status::complete) return status;
    if (reply.timer_id < 0) {
        error = "CharAI::OnRevived AI_Tick timer start returned an invalid source id";
        return Status::service_failed;
    }
    ai->word_10 = static_cast<std::uint32_t>(reply.timer_id);
    out->ai_tick_timer = reply.timer_id;
    out->ai_tick_started = 1;

    status = call(Operation::get_dot_tick, 0, 0, 0, &reply);
    if (status != Status::complete) return status;
    const auto dot_tick_ms = reply.duration_ms;

    status = call(Operation::start_dot_tick, dot_tick_ms, -1, 52, &reply);
    if (status != Status::complete) return status;
    if (reply.timer_id < 0) {
        error = "CharAI::OnRevived DoT_Tick timer start returned an invalid source id";
        return Status::service_failed;
    }
    ai->word_14 = static_cast<std::uint32_t>(reply.timer_id);
    out->dot_tick_timer = reply.timer_id;
    out->dot_tick_started = 1;

    if (active_ais) {
        status = call(Operation::active_ais_on_revived, 0, 0, 0, &reply);
        if (status != Status::complete) return status;
        out->active_ais_called = 1;
    }
    error.clear();
    return Status::complete;
}

} // namespace dh2::character_ai_on_revived_v1
