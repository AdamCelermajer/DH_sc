// v21.8 w2-assassin: DoT owner (PROPS_AddDot capacity, HandleDots ticks on the DoT_Tick boundary, expiry, dead target).
#include "dot_tick_owner_v1.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::status_effects;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
bool near(double a, double b) { return std::fabs(a - b) < 1e-6; }
} // namespace

int main() {
    try {
        const ActorId attacker = 7, target = 9;

        // Invalid inputs are ignored (F_ApplyResult only calls PROPS_AddDot for duration > 0 and tick > 0).
        {
            DotTickOwnerV1 dots;
            dots.add(attacker, target, kDotNormalElement, 0.0, 1.0);
            dots.add(attacker, target, kDotNormalElement, 3000.0, 0.0);
            dots.add(attacker, invalid_actor_id, kDotNormalElement, 3000.0, 1.0);
            check(dots.empty(), "non-positive duration/tick or missing target must not add a DoT");
        }

        // One DoT with duration 3000 ms and a 1000 ms DOT_Tick: ticks at the 1 s and 2 s boundaries, expires at 3 s.
        {
            DotTickOwnerV1 dots;
            dots.add(attacker, target, kDotNormalElement, 3000.0, 4.5);
            check(dots.size() == 1, "one DoT stored");
            auto t1 = dots.advance(500.0);
            check(t1.empty(), "no tick before the first 1000 ms boundary");
            auto t2 = dots.advance(600.0); // crosses 1000 ms
            check(t2.size() == 1 && t2[0].target == target && t2[0].attacker == attacker && near(t2[0].tick_hp, 4.5),
                  "one tick of 4.5 HP at the first boundary");
            auto t3 = dots.advance(1000.0); // crosses 2000 ms
            check(t3.size() == 1, "second tick at the second boundary");
            // 3000 ms boundary: the DoT still has 900 ms left at the start of this frame, so it ticks once and then expires.
            auto t4 = dots.advance(1000.0);
            check(t4.size() == 1, "third tick at the 3 s boundary while the DoT is still active");
            check(dots.empty(), "DoT is gone once its duration has elapsed");
            auto t5 = dots.advance(1000.0);
            check(t5.empty(), "no tick after expiry");
        }

        // Capacity 1 per element: a second add replaces the first (new tick and duration).
        {
            DotTickOwnerV1 dots;
            dots.add(attacker, target, kDotNormalElement, 2000.0, 1.0);
            dots.add(7, target, kDotNormalElement, 5000.0, 3.0);
            check(dots.size() == 1, "same target and element replaces the DoT");
            auto ticks = dots.advance(1000.0);
            check(ticks.size() == 1 && near(ticks[0].tick_hp, 3.0), "replacement carries the new tick value");
            // A different element on the same target is a separate DoT.
            dots.add(attacker, target, 2, 2000.0, 1.5);
            check(dots.size() == 2, "a different element stacks as a second DoT");
        }

        // Several targets are independent; forget_target drops only that target.
        {
            DotTickOwnerV1 dots;
            dots.add(attacker, 1, kDotNormalElement, 4000.0, 2.0);
            dots.add(attacker, 2, kDotNormalElement, 4000.0, 2.0);
            dots.forget_target(1);
            check(dots.size() == 1, "forget_target removes only the given target");
            auto ticks = dots.advance(1000.0);
            check(ticks.size() == 1 && ticks[0].target == 2, "remaining DoT ticks on its own target");
        }

        // A long frame (several boundaries) yields one tick per boundary while the DoT is active.
        {
            DotTickOwnerV1 dots;
            dots.add(attacker, target, kDotNormalElement, 10000.0, 1.0);
            auto ticks = dots.advance(3500.0);
            check(ticks.size() == 3, "three boundaries crossed in 3.5 s give three ticks");
        }

        std::cout << "dot_tick_owner_v1 tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "dot_tick_owner_v1 test failed: " << error.what() << "\n";
        return 1;
    }
}
