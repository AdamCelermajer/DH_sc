#include "gslevel_activation_step_v1.hpp"

namespace dh2::ui {
namespace {
constexpr std::uint32_t kConstructed = 1;
constexpr std::uint32_t kLoadRequested = 2;
constexpr std::uint32_t kLoading = 3;
constexpr std::uint32_t kActive = 4;
constexpr std::int32_t kLoaded = 38;
}

GsLevelActivationStepV1 plan_gslevel_activation_step_v1(
    const GsLevelActivationSnapshotV1& source) noexcept {
    GsLevelActivationStepV1 out{};
    out.gslevel = source.gslevel;
    out.level = source.level;
    out.source_phase = source.phase;
    out.next_phase = source.phase;
    if (!source.gslevel || !source.level) {
        out.status = GsLevelActivationStatusV1::invalid_owner;
        return out;
    }
    if (source.phase < kConstructed || source.phase > kActive) {
        out.status = GsLevelActivationStatusV1::invalid_phase;
        return out;
    }
    if (source.globally_blocked) {
        out.status = GsLevelActivationStatusV1::deferred;
        return out;
    }

    out.status = GsLevelActivationStatusV1::ready;
    switch (source.phase) {
    case kConstructed:
        out.action = GsLevelActivationActionV1::set_phase;
        out.next_phase = kLoadRequested;
        break;
    case kLoadRequested:
        out.action = GsLevelActivationActionV1::load_level;
        out.next_phase = kLoading;
        break;
    case kLoading:
    case kActive:
        out.action = GsLevelActivationActionV1::update_level;
        break;
    default:
        out.status = GsLevelActivationStatusV1::invalid_phase;
        out.action = GsLevelActivationActionV1::none;
        break;
    }
    return out;
}

GsLevelActivationStatusV1 finish_gslevel_activation_step_v1(
    const GsLevelActivationStepV1& step,
    const GsLevelActivationSnapshotV1& current,
    std::int32_t level_state_after_update,
    std::uint32_t* resulting_phase) noexcept {
    if (!resulting_phase || step.status != GsLevelActivationStatusV1::ready ||
        !step.gslevel || !step.level || step.gslevel != current.gslevel ||
        step.level != current.level || step.source_phase != current.phase ||
        current.globally_blocked) {
        return GsLevelActivationStatusV1::stale_completion;
    }

    switch (step.action) {
    case GsLevelActivationActionV1::set_phase:
    case GsLevelActivationActionV1::load_level:
        *resulting_phase = step.next_phase;
        return GsLevelActivationStatusV1::ready;
    case GsLevelActivationActionV1::update_level:
        *resulting_phase = current.phase;
        if (current.phase == kLoading && level_state_after_update == kLoaded)
            *resulting_phase = kActive;
        return GsLevelActivationStatusV1::ready;
    case GsLevelActivationActionV1::none:
    default:
        return GsLevelActivationStatusV1::invalid_phase;
    }
}

} // namespace dh2::ui
