#include "character_critical_camera_bridge_v1.hpp"

#include <cstddef>

namespace dh2::character_critical_camera_bridge_v1 {
namespace {
bool disjoint(const void* a, std::size_t as, const void* b, std::size_t bs) {
    if (!a || !b) return false;
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (x > UINTPTR_MAX - as || y > UINTPTR_MAX - bs) return false;
    return x + as <= y || y + bs <= x;
}

bool query(const Request& request, Query operation, std::int32_t& value,
           Report& report) {
    ++report.queries_attempted;
    const QueryRequest query_request{operation, request.attacker};
    try {
        if (request.invoke(request.context, query_request, &value) != 0)
            return false;
    } catch (...) {
        return false;
    }
    ++report.queries_completed;
    return true;
}
}

Status resolve(const Request* request,
               camera_animset_v1::PlaybackOwner& playback,
               camera_animset_v1::PlayRequest* output,
               Report* report) {
    if (!request || !output || !report ||
        reinterpret_cast<std::uintptr_t>(request) % alignof(Request) ||
        reinterpret_cast<std::uintptr_t>(output) %
            alignof(camera_animset_v1::PlayRequest) ||
        reinterpret_cast<std::uintptr_t>(report) % alignof(Report) ||
        !disjoint(request, sizeof(*request), output, sizeof(*output)) ||
        !disjoint(request, sizeof(*request), report, sizeof(*report)) ||
        !disjoint(output, sizeof(*output), report, sizeof(*report)) ||
        !request->result ||
        reinterpret_cast<std::uintptr_t>(request->result) %
            alignof(data::CombatResult) ||
        !request->attacker || !request->invoke ||
        !disjoint(request, sizeof(*request), request->result,
                  sizeof(*request->result)) ||
        !disjoint(output, sizeof(*output), request->result,
                  sizeof(*request->result)) ||
        !disjoint(report, sizeof(*report), request->result,
                  sizeof(*request->result)))
        return Status::invalid_argument;

    *output = {};
    *report = {};
    if ((request->result->outcomes & 8u) == 0) return Status::complete;

    std::int32_t is_player = 0;
    if (!query(*request, Query::requester_is_player, is_player, *report))
        return Status::provider_failed;
    if (is_player != 0 && is_player != 1) return Status::provider_failed;
    if (is_player != 1) return Status::complete;
    report->combat_result_player_gate = 1;

    // CanPlayShakeAnim queries IsPlayer again on the live Character.
    if (!query(*request, Query::requester_is_player, is_player, *report))
        return Status::provider_failed;
    if (is_player != 0 && is_player != 1) return Status::provider_failed;
    bool can_play = is_player == 0;
    if (is_player == 1) {
        std::int32_t local_player_count = 0;
        if (!query(*request, Query::local_player_count, local_player_count,
                   *report))
            return Status::provider_failed;
        if (local_player_count == 1) {
            std::int32_t is_local = 0;
            if (!query(*request, Query::requester_is_local, is_local, *report))
                return Status::provider_failed;
            if (is_local != 0 && is_local != 1) return Status::provider_failed;
            can_play = is_local != 0;
        }
    }
    report->camera_can_play = can_play ? 1u : 0u;

    std::string error;
    if (!playback.combat_shake(true, true, can_play, *output, error))
        return Status::request_failed;
    report->play_requested = output->present ? 1u : 0u;
    return Status::complete;
}

} // namespace dh2::character_critical_camera_bridge_v1
