// v21.8 es-runtime: unit coverage for RuntimeSkillTimerServiceV1 (StartSkillTimerCB / StopTimerCB semantics).
// Pure logic, no assets: one-shot expiry, repeating catch-up, cancellation from inside a callback, owner cancellation,
// timers started from a callback, error propagation and rejected intervals.
#include "runtime_skill_timer_service_v1.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
} // namespace

int main() {
    try {
        // Rejected intervals never create a timer (native TMR_Start takes integer milliseconds; 0 is no timer).
        {
            RuntimeSkillTimerServiceV1 timers;
            check(timers.start(0.0, false, nullptr) == 0, "A 0 ms timer was created");
            check(timers.start(-5.0, true, nullptr) == 0, "A negative timer was created");
            check(timers.start(std::numeric_limits<double>::quiet_NaN(), false, nullptr) == 0, "A NaN timer was created");
            check(timers.start(0.5, false, nullptr) == 0, "A sub-millisecond timer was created");
            check(timers.size() == 0, "Rejected timers were stored");
            std::cout << "rejected intervals: 0, negative, NaN, 0.5 ms -> no timer\n";
        }

        // One-shot: nothing before its interval, one call at the interval, then removed. A stop after expiry is a no-op.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::vector<double> fired_at;
            std::string error;
            const auto id = timers.start(2200.0, false, [&](RuntimeSkillTimerIdV1, std::string&) {
                fired_at.push_back(timers.now_ms());
                return true;
            });
            check(id != 0 && timers.active(id), "One-shot timer was not started");
            check(timers.advance(2000.0, error), error);
            check(fired_at.empty() && timers.active(id), "One-shot fired before its interval");
            check(timers.advance(300.0, error), error);
            check(fired_at.size() == 1 && std::fabs(fired_at[0] - 2200.0) < 1e-9, "One-shot did not fire exactly once at 2200 ms");
            check(!timers.active(id) && timers.size() == 0, "Expired one-shot was not removed");
            check(!timers.stop(id), "StopTimerCB on an expired one-shot reported a stop");
            check(timers.advance(5000.0, error) && fired_at.size() == 1, "Expired one-shot fired again");
            std::cout << "one-shot: 2200 ms timer fired once at t=2200, removed, stop-after-expiry no-op\n";
        }

        // Repeating catch-up: a 500 ms loop advanced by 1200 ms fires at 500 and 1000 (the Mage tornado tick loop did
        // the same), then keeps its phase at 1500.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::vector<double> fired_at;
            std::string error;
            const auto id = timers.start(500.0, true, [&](RuntimeSkillTimerIdV1, std::string&) {
                fired_at.push_back(timers.now_ms());
                return true;
            });
            check(timers.advance(1200.0, error), error);
            check(fired_at.size() == 2 && std::fabs(fired_at[0] - 500.0) < 1e-9 && std::fabs(fired_at[1] - 1000.0) < 1e-9,
                  "Repeating timer did not catch up 500 and 1000 ms");
            check(timers.advance(300.0, error), error);
            check(fired_at.size() == 3 && std::fabs(fired_at[2] - 1500.0) < 1e-9, "Repeating timer lost its phase");
            check(timers.active(id), "Repeating timer expired by itself");
            std::cout << "repeating: 500 ms loop fires at 500, 1000, 1500 (phase kept)\n";
        }

        // FireSkullRain shape: a 1000 ms loop that stops itself after 3 ticks (MAX_TICKS 2: ticks 0,1,2 then StopTimerCB).
        {
            RuntimeSkillTimerServiceV1 timers;
            unsigned ticks = 0;
            std::string error;
            RuntimeSkillTimerIdV1 id = 0;
            id = timers.start(1000.0, true, [&](RuntimeSkillTimerIdV1 self, std::string&) {
                ++ticks;
                if (ticks >= 3) check(timers.stop(self), "Self-stop of the tick loop failed");
                return true;
            });
            check(timers.advance(10000.0, error), error);
            check(ticks == 3 && !timers.active(id), "Tick loop did not stop after 3 ticks");
            std::cout << "self-stopping loop: 3 ticks then StopTimerCB, no further ticks in 10 s\n";
        }

        // Stopping another timer from a callback, and two timers due at the same instant fire in start order.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::vector<std::string> order;
            std::string error;
            RuntimeSkillTimerIdV1 victim = 0;
            timers.start(100.0, false, [&](RuntimeSkillTimerIdV1, std::string&) {
                order.push_back("first");
                check(timers.stop(victim), "Cancelling a pending timer from a callback failed");
                return true;
            });
            victim = timers.start(100.0, false, [&](RuntimeSkillTimerIdV1, std::string&) {
                order.push_back("victim");
                return true;
            });
            timers.start(100.0, false, [&](RuntimeSkillTimerIdV1, std::string&) {
                order.push_back("second");
                return true;
            });
            check(timers.advance(200.0, error), error);
            check(order.size() == 2 && order[0] == "first" && order[1] == "second",
                  "Same-instant timers did not fire in start order or the cancelled one ran");
            std::cout << "same-instant: start order kept; cancelled sibling did not run\n";
        }

        // A timer started inside a callback is due one interval after that callback's time.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::vector<double> nested_at;
            std::string error;
            timers.start(300.0, false, [&](RuntimeSkillTimerIdV1, std::string&) {
                timers.start(200.0, false, [&](RuntimeSkillTimerIdV1, std::string&) {
                    nested_at.push_back(timers.now_ms());
                    return true;
                });
                return true;
            });
            check(timers.advance(1000.0, error), error);
            check(nested_at.size() == 1 && std::fabs(nested_at[0] - 500.0) < 1e-9,
                  "Timer started from a callback was not due 200 ms after t=300");
            std::cout << "nested: timer started at t=300 with 200 ms fired at t=500\n";
        }

        // stop_owner cancels only the owner's timers.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::string error;
            unsigned a = 0, b = 0;
            timers.start(100.0, true, [&](RuntimeSkillTimerIdV1, std::string&) { ++a; return true; }, 7);
            timers.start(100.0, true, [&](RuntimeSkillTimerIdV1, std::string&) { ++b; return true; }, 9);
            check(timers.advance(150.0, error) && a == 1 && b == 1, "Owner timers did not fire before cancellation");
            check(timers.stop_owner(7) == 1, "stop_owner did not cancel exactly the owner's timer");
            check(timers.advance(300.0, error) && a == 1 && b == 4, "stop_owner stopped the wrong owner or left a live timer");
            std::cout << "stop_owner: owner 7 cancelled, owner 9 kept ticking\n";
        }

        // An error from a callback stops the advance and is reported.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::string error;
            timers.start(100.0, false, [&](RuntimeSkillTimerIdV1, std::string& e) {
                e = "owner failed";
                return false;
            });
            check(!timers.advance(100.0, error) && error == "owner failed", "Callback error was not reported");
            std::cout << "callback error propagates: \"" << error << "\"\n";
        }

        // Invalid dt is rejected without moving the clock.
        {
            RuntimeSkillTimerServiceV1 timers;
            std::string error;
            check(!timers.advance(-1.0, error) && !error.empty(), "Negative dt was accepted");
            check(!timers.advance(std::numeric_limits<double>::infinity(), error), "Infinite dt was accepted");
            check(timers.now_ms() == 0.0, "Rejected dt moved the clock");
        }

        std::cout << "runtime_skill_timer_service_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime_skill_timer_service_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
