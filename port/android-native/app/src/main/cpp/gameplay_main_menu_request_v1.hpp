#pragma once

namespace dh2::native {
// A one-shot handoff from the authored HUD callback to the Android lifecycle
// owner. The caller consumes it only after SWF input dispatch has returned.
class GameplayMainMenuRequestV1 {
public:
    void request() noexcept { pending_ = true; }
    bool consume() noexcept {
        if (!pending_) return false;
        pending_ = false;
        return true;
    }
    void reset() noexcept { pending_ = false; }
private:
    bool pending_{};
};
}
