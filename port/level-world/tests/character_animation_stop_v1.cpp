#include "../character_animation_stop_v1.hpp"

#include <cassert>
#include <cstdint>
#include <string>

namespace {
using namespace dh2;
using namespace dh2::character_animation_stop_v1;

struct Capture {
    unsigned calls{};
    Identity character{};
    std::uint32_t event{};
    std::int32_t sequence{-1};
    std::uintptr_t payload{};
    bool accept{true};
};

bool raise_event(void* opaque, Identity character, std::uint32_t event,
                 std::int32_t sequence, std::uintptr_t payload,
                 std::string& error) {
    auto& capture = *static_cast<Capture*>(opaque);
    ++capture.calls;
    capture.character = character;
    capture.event = event;
    capture.sequence = sequence;
    capture.payload = payload;
    if (!capture.accept) error = "fixture coordinator rejected event";
    return capture.accept;
}

data::AnimationTables table() {
    data::AnimationTables result;
    data::AnimationSequence root, child;
    root.type = 0;
    data::AnimationStep redirect;
    redirect.redir = 1;
    redirect.anim = 1;
    root.steps.push_back(redirect);
    child.type = 1;
    data::AnimationStep clip;
    clip.anim = 14;
    child.steps.push_back(clip);
    result.sequences = {root, child};
    return result;
}
}

int main() {
    constexpr Identity character = 0x12345678;
    const auto animations = table();
    data::AnimationRandom random{};
    data::AnimationScheduler scheduler;
    std::string error;
    assert(scheduler.start(animations, 0, random, error));
    assert(scheduler.active() && scheduler.frames().size() == 2);
    const auto selected_clip = scheduler.clip().anim;

    std::uint8_t latch = 0;
    Capture capture;
    const Services services{&capture, raise_event};
    assert(stop(scheduler, character, character + 1, latch, true,
                services, error) == Status::identity_mismatch);
    assert(scheduler.active() && latch == 0 && capture.calls == 0);

    const Services unavailable{};
    assert(stop(scheduler, character, character, latch, true,
                unavailable, error) == Status::service_unavailable);
    assert(!scheduler.active() && scheduler.frames().size() == 1 && latch == 1);

    assert(scheduler.start(animations, 0, random, error));
    latch = 0;
    assert(stop(scheduler, character, character, latch, true,
                services, error) == Status::complete);
    assert(error.empty() && !scheduler.active() && scheduler.frames().size() == 1);
    assert(scheduler.frames().front().sequence == 0);
    assert(scheduler.clip().anim == selected_clip);
    assert(latch == 1 && capture.calls == 1);
    assert(capture.character == character && capture.event == 0x22);
    assert(capture.sequence == 0 && capture.payload == 0);
    assert(latch == 1); // the source latch is internal, not the event payload

    assert(stop(scheduler, character, character, latch, true,
                services, error) == Status::complete);
    assert(capture.calls == 1); // the source +0x48 latch suppresses duplicates

    // With no VisualObject controller, ANIM_Stop still resets the scheduler
    // depth but CharAnimator does not raise Character event 34.
    data::AnimationScheduler headless;
    assert(headless.start(animations, 0, random, error));
    std::uint8_t no_controller_latch = 0;
    assert(stop(headless, character, character, no_controller_latch, false,
                unavailable, error) == Status::complete);
    assert(!headless.active() && headless.frames().size() == 1);
    assert(no_controller_latch == 0 && capture.calls == 1);

    // Source side effects precede Coordinator delivery; rejection preserves
    // the stop and one-shot latch rather than rolling them back.
    data::AnimationScheduler rejected;
    assert(rejected.start(animations, 0, random, error));
    std::uint8_t rejected_latch = 0;
    capture.accept = false;
    assert(stop(rejected, character, character, rejected_latch, true,
                services, error) == Status::event_rejected);
    assert(!rejected.active() && rejected_latch == 1 && capture.calls == 2);
    assert(error == "fixture coordinator rejected event");
}
