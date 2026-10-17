#pragma once

#include <cstdint>

namespace dh2::ui {

// One observed GSLevel::Update tick. The native state word is GSLevel+0x38;
// level_state is the same Level+0x130 word read after Level::Update.
struct GsLevelActivationSnapshotV1 {
    std::uintptr_t gslevel{};
    std::uintptr_t level{};
    std::uint32_t phase{};
    std::int32_t level_state{};
    bool globally_blocked{}; // Native byte_99F818 guard.
};

enum class GsLevelActivationActionV1 : std::uint8_t {
    none,
    set_phase,
    load_level,
    update_level
};

enum class GsLevelActivationStatusV1 : std::uint8_t {
    ready,
    deferred,
    invalid_owner,
    invalid_phase,
    stale_completion
};

struct GsLevelActivationStepV1 {
    GsLevelActivationStatusV1 status{GsLevelActivationStatusV1::invalid_owner};
    GsLevelActivationActionV1 action{GsLevelActivationActionV1::none};
    std::uintptr_t gslevel{};
    std::uintptr_t level{};
    std::uint32_t source_phase{};
    std::uint32_t next_phase{};
};

// Mirrors the phase dispatch in GSLevel::Update at ELF 0x386630. This is a
// pure adapter: it owns no GSLevel, Level, state queue, save, or VM. The caller
// executes the returned operation against the already-bound source owners.
GsLevelActivationStepV1 plan_gslevel_activation_step_v1(
    const GsLevelActivationSnapshotV1&) noexcept;

// Complete one returned operation against the same identities. A phase-3
// Level::Update enters phase 4 only when its post-update Level+0x130 is 38.
GsLevelActivationStatusV1 finish_gslevel_activation_step_v1(
    const GsLevelActivationStepV1&, const GsLevelActivationSnapshotV1& current,
    std::int32_t level_state_after_update, std::uint32_t* resulting_phase) noexcept;

} // namespace dh2::ui
