// CXPlayerManager: the player-service manager state. Only the state switch and the
// ready check are reconstructed. The constructors and OnNetworkError are queued.
//
// Layout (original 32-bit offsets, kept as comments): +0x14 current state (int).
#pragma once

#include <cstdint>

// CXPlayerManagerState in the mangled names. The value is an int in the original.
using CXPlayerManagerState = std::int32_t;

class CXPlayerManager {
public:
    // 0x52e0d0 _ZN15CXPlayerManager11SwitchStateE20CXPlayerManagerState
    void SwitchState(CXPlayerManagerState state);
    // 0x52e0d8 _ZN15CXPlayerManager7IsReadyEv
    bool IsReady() const;
    // 0x52e2d4 _ZN15CXPlayerManager14OnNetworkErrorEv: prints the error, sets the error code
    // to 3 and switches to state 0x11. The original is also the first vtable slot.
    void OnNetworkError();

    // Not in the original: test seam.
    CXPlayerManagerState StateForTest() const { return state_; }
    std::int32_t LastErrorForTest() const { return lastError_; }

private:
    static constexpr CXPlayerManagerState kReadyState = 0x10;
    CXPlayerManagerState state_ = 0;  // +0x14
    std::int32_t lastError_ = 0;      // +0x7c (name inferred: set to 3 on a network error)
};
