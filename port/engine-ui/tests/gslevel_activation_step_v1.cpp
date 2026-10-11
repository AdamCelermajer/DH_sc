#include "../gslevel_activation_step_v1.hpp"

#include <cassert>
#include <cstdint>

using namespace dh2::ui;

int main() {
    GsLevelActivationSnapshotV1 state{0x1000, 0x2000, 1, 0, false};
    auto step = plan_gslevel_activation_step_v1(state);
    assert(step.status == GsLevelActivationStatusV1::ready);
    assert(step.action == GsLevelActivationActionV1::set_phase);
    std::uint32_t phase = 0;
    assert(finish_gslevel_activation_step_v1(step, state, 0, &phase) ==
           GsLevelActivationStatusV1::ready && phase == 2);

    state.phase = 2;
    step = plan_gslevel_activation_step_v1(state);
    assert(step.action == GsLevelActivationActionV1::load_level);
    assert(finish_gslevel_activation_step_v1(step, state, 0, &phase) ==
           GsLevelActivationStatusV1::ready && phase == 3);

    state.phase = 3;
    step = plan_gslevel_activation_step_v1(state);
    assert(step.action == GsLevelActivationActionV1::update_level);
    assert(finish_gslevel_activation_step_v1(step, state, 37, &phase) ==
           GsLevelActivationStatusV1::ready && phase == 3);
    assert(finish_gslevel_activation_step_v1(step, state, 38, &phase) ==
           GsLevelActivationStatusV1::ready && phase == 4);

    state.phase = 4;
    step = plan_gslevel_activation_step_v1(state);
    assert(step.action == GsLevelActivationActionV1::update_level);
    assert(finish_gslevel_activation_step_v1(step, state, 0, &phase) ==
           GsLevelActivationStatusV1::ready && phase == 4);

    state.globally_blocked = true;
    step = plan_gslevel_activation_step_v1(state);
    assert(step.status == GsLevelActivationStatusV1::deferred);
    state.globally_blocked = false;
    state.level = 0x3000;
    assert(finish_gslevel_activation_step_v1(step, state, 38, &phase) ==
           GsLevelActivationStatusV1::stale_completion);
}
