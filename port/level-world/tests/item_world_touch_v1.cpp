#include "../item_world_touch_v1.hpp"

#include <cassert>

using dh2::item_world_touch_v1::ContactEvent;
using dh2::item_world_touch_v1::queue_interact;

int main() {
    bool active = false;
    assert(queue_interact(ContactEvent::begin, true, true, active));
    assert(active);
    assert(!queue_interact(ContactEvent::persist, true, true, active));
    assert(!queue_interact(ContactEvent::begin, true, true, active));
    assert(!queue_interact(ContactEvent::end, true, true, active));
    assert(!active);
    assert(queue_interact(ContactEvent::begin, true, true, active));

    // A failed begin decision is not retried every physics persist frame.
    active = false;
    assert(!queue_interact(ContactEvent::begin, true, false, active));
    assert(active);
    assert(!queue_interact(ContactEvent::persist, true, true, active));
    assert(!queue_interact(ContactEvent::end, false, false, active));
    assert(queue_interact(ContactEvent::begin, true, true, active));

    active = false;
    assert(!queue_interact(ContactEvent::begin, false, true, active));
    assert(!active);
    assert(!queue_interact(ContactEvent::persist, true, true, active));
    assert(!queue_interact(ContactEvent::other, true, true, active));

    const float owner[3]{0.f,0.f,0.f};
    const float in_range[3]{8.f,0.f,0.f};
    const float out_of_range[3]{8.f,0.f,1.f};
    // distance 8 - owner melee 2 - item radius 1 == InteractRadius 5.
    assert(dh2::item_world_touch_v1::in_interaction_range(
        owner,in_range,2.f,1.f,5.f,-1));
    assert(!dh2::item_world_touch_v1::in_interaction_range(
        owner,out_of_range,2.f,1.f,5.f,-1));
    // Source type 8 uses the stricter zero threshold after both radii.
    const float touching[3]{3.f,0.f,0.f};
    assert(dh2::item_world_touch_v1::in_interaction_range(
        owner,touching,2.f,1.f,100.f,8));
    assert(!dh2::item_world_touch_v1::in_interaction_range(
        owner,in_range,2.f,1.f,100.f,8));
    using dh2::item_world_touch_v1::receiver_allows_interact;
    assert(receiver_allows_interact(0,0x1000));
    assert(receiver_allows_interact(0x1000,0x1000));
    assert(!receiver_allows_interact(0x2000,0x1000));
    assert(!receiver_allows_interact(0,0));
    return 0;
}
