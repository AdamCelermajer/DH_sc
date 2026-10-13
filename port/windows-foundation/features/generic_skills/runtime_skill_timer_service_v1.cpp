#include "runtime_skill_timer_service_v1.hpp"

#include <cmath>
#include <vector>

namespace dh::foundation::generic_skills {

RuntimeSkillTimerIdV1 RuntimeSkillTimerServiceV1::start(double interval_ms, bool loop,
                                                        RuntimeSkillTimerCallbackV1 callback, std::uint64_t owner) {
    if (!std::isfinite(interval_ms) || interval_ms < 1.0) return 0;
    if (next_id_ == 0) return 0;
    const RuntimeSkillTimerIdV1 id = next_id_++;
    Entry entry;
    entry.due_ms = now_ms_ + interval_ms;
    entry.interval_ms = interval_ms;
    entry.loop = loop;
    entry.owner = owner;
    entry.callback = std::move(callback);
    timers_.emplace(id, std::move(entry));
    return id;
}

bool RuntimeSkillTimerServiceV1::stop(RuntimeSkillTimerIdV1 timer) {
    return timers_.erase(timer) != 0;
}

std::size_t RuntimeSkillTimerServiceV1::stop_owner(std::uint64_t owner) {
    std::size_t cancelled = 0;
    for (auto it = timers_.begin(); it != timers_.end();) {
        if (it->second.owner == owner) {
            it = timers_.erase(it);
            ++cancelled;
        } else {
            ++it;
        }
    }
    return cancelled;
}

bool RuntimeSkillTimerServiceV1::active(RuntimeSkillTimerIdV1 timer) const {
    return timers_.find(timer) != timers_.end();
}

void RuntimeSkillTimerServiceV1::clear() {
    timers_.clear();
}

bool RuntimeSkillTimerServiceV1::advance(double dt_ms, std::string& error) {
    error.clear();
    if (!std::isfinite(dt_ms) || dt_ms < 0.0) {
        error = "Skill timer clock requires a finite nonnegative dt";
        return false;
    }
    const double target = now_ms_ + dt_ms;
    for (;;) {
        // Earliest due timer at or before the target time; ties resolve to the earlier start (lower id).
        auto due = timers_.end();
        for (auto it = timers_.begin(); it != timers_.end(); ++it) {
            if (it->second.due_ms > target) continue;
            if (due == timers_.end() || it->second.due_ms < due->second.due_ms) due = it;
        }
        if (due == timers_.end()) break;
        now_ms_ = due->second.due_ms;
        const RuntimeSkillTimerIdV1 id = due->first;
        // Copy the callback and update the entry before running it: the callback may stop or start timers, which
        // invalidates `due`. A one-shot is removed before its callback (a StopTimerCB inside it is then a no-op, as in Lua).
        RuntimeSkillTimerCallbackV1 callback = due->second.callback;
        if (due->second.loop) due->second.due_ms += due->second.interval_ms;
        else timers_.erase(due);
        if (!callback) continue;
        std::string callback_error;
        if (!callback(id, callback_error)) {
            error = callback_error.empty() ? "Skill timer callback failed" : callback_error;
            return false;
        }
    }
    now_ms_ = target;
    return true;
}

} // namespace dh::foundation::generic_skills
