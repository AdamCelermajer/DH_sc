#pragma once
// v21.8 es-runtime: general skill timers (Lua StartSkillTimerCB / StartTimerCB / StopTimerCB).
//
// Original (IDA export + .local-inputs/assets-extra/android/data/scripts/skills/_commons.luac + ai/_commons.luac):
//   - StartSkillTimerCB(ms, loop, cb, ref) -> StartTimerCB(ms, loop, SkillTimerCB_, {cb, skill_id, ref}) -> native
//     Character::_StartTimer (IDA 0x3b7590): loop true -> CharTimers::TMR_Start(ms, -1, 53) (repeats until stopped),
//     loop false -> TMR_Start(ms, 0, 53) (once). Returns the timer id.
//   - Expiry: the native timer event calls the AI-script OnTimer(id) (ai/_commons.luac), which calls cb(id, ref);
//     a non-looping entry is removed after its callback.
//   - StopTimerCB(id) clears the callback entry and calls native Character::_StopTimer (IDA 0x3b7064, TMR_Stop).
//     Stopping a one-shot that already expired is a no-op.
//   - SkillTimerCB_ restores SKILL_ID to the skill that started the timer while cb runs (the owner's own timers).
//
// This service is the port of that clock for skill owners. Every owner that used to keep its own ad hoc clock
// (RuntimeMageSkillOwnerV1 statues and tornado ticks) runs its timers here. Times are gameplay milliseconds advanced by
// the host once per unpaused frame (the same clock the Mage owner used).
//
// Inferences (marked, not proven by a test against the original):
//   - A repeating timer that is due several times within one advance fires once per elapsed interval, in due-time
//     order (the Mage tornado tick loop already caught up this way).
//   - Intervals below 1 ms are refused (the native timers are integer milliseconds); 0 is not a timer.
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace dh::foundation::generic_skills {

using RuntimeSkillTimerIdV1 = std::uint64_t;  // 0 = no timer (Lua nil)

// Runs when the timer is due. Returning false (with `error`) stops the advance and reports the failure.
using RuntimeSkillTimerCallbackV1 = std::function<bool(RuntimeSkillTimerIdV1 timer, std::string& error)>;

class RuntimeSkillTimerServiceV1 {
public:
    // Starts a timer `interval_ms` after the current gameplay time. loop=false: fires once. loop=true: fires every
    // interval until stop(). `owner` is an opaque key (a skill id or an owner token) used by stop_owner(). Returns 0 when
    // the interval is not finite or below 1 ms.
    RuntimeSkillTimerIdV1 start(double interval_ms, bool loop, RuntimeSkillTimerCallbackV1 callback, std::uint64_t owner = 0);
    // StopTimerCB: false when the id is not active (never started, already stopped, or an expired one-shot).
    bool stop(RuntimeSkillTimerIdV1 timer);
    // Cancels every timer of one owner. Returns the number cancelled.
    std::size_t stop_owner(std::uint64_t owner);
    bool active(RuntimeSkillTimerIdV1 timer) const;
    std::size_t size() const noexcept { return timers_.size(); }
    // Current gameplay time of the clock (the sum of all advanced dt).
    double now_ms() const noexcept { return now_ms_; }
    // Advances the clock by dt_ms (finite, >= 0) and fires every timer that comes due, in due-time order (ties: start
    // order). A callback may stop or start timers; a timer started inside a callback is due `interval_ms` after the
    // time of the callback that started it.
    bool advance(double dt_ms, std::string& error);
    void clear();

private:
    struct Entry {
        double due_ms = 0.0;
        double interval_ms = 0.0;
        bool loop = false;
        std::uint64_t owner = 0;
        RuntimeSkillTimerCallbackV1 callback;
    };
    double now_ms_ = 0.0;
    RuntimeSkillTimerIdV1 next_id_ = 1;
    std::map<RuntimeSkillTimerIdV1, Entry> timers_;  // keyed by id = start order
};

} // namespace dh::foundation::generic_skills
