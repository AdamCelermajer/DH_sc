#pragma once

#include <string>
#include <utility>

namespace dh2::ui {

// A character-menu action can change the selected Faery while the retained
// HUD movie is still inside its own input callback. Re-run the authored HUD
// onPush at the next safe SWF boundary so its skill buttons read the same
// canonical Player state after ChangeFaery completes.
struct DeferredHudSkillsRefreshV1 {
    bool pending{};

    void request() noexcept { pending = true; }

    template<class Refresh>
    bool flush(Refresh&& refresh, std::string& error) {
        if (!pending) {
            error.clear();
            return true;
        }
        if (!std::forward<Refresh>(refresh)(error)) {
            if (error == "SWF core busy") {
                error.clear();
                return true;
            }
            return false;
        }
        pending = false;
        error.clear();
        return true;
    }
};

} // namespace dh2::ui
