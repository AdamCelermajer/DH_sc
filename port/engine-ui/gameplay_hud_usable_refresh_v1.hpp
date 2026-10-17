#pragma once

#include <chrono>
#include <cstdint>
#include <utility>

namespace dh2::ui {

// The authored HUD can poll skill usability less often than cooldown frames.
// A successful skill-button action invalidates this cache so the next HUD
// projection rechecks the same retained Player script immediately.
class GameplayHudUsableRefreshV1 {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    bool due(TimePoint now) const noexcept {
        return last_refresh_.time_since_epoch().count() == 0 ||
               now - last_refresh_ >= std::chrono::milliseconds(500);
    }
    void refreshed(TimePoint now) noexcept { last_refresh_ = now; }
    void invalidate() noexcept { last_refresh_ = {}; }

private:
    TimePoint last_refresh_{};
};

// Cast actions can change the source OnSkillCheck/OnSpellCheck result before
// the normal HUD polling interval expires. Preserve a fresh cache when an
// action fails; invalidate only after the bound source action completes.
template<class Action>
bool run_hud_action_and_refresh_usable_v1(
        GameplayHudUsableRefreshV1& refresh, Action&& action) {
    if (!std::forward<Action>(action)()) return false;
    refresh.invalidate();
    return true;
}

inline void refresh_usable_after_skill_train_v1(
        GameplayHudUsableRefreshV1& refresh, bool test_only,
        std::uint32_t source_result) noexcept {
    if (!test_only && source_result != 0) refresh.invalidate();
}

} // namespace dh2::ui
