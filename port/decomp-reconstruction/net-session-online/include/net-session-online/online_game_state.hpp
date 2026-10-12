// OnlineGameState: the online game state. This port reconstructs only the timeout
// checks. The original object is an OnlineSingleton<OnlineGameState> with the state
// machine at +0x14, and it is not modelled here.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x3c start-game timeval (sec, usec)   +0x44 room-creation timeval (sec, usec)
#pragma once

#include <cstdint>

#include "net-session-online/deps/time_of_day.hpp"

class OnlineGameState {
public:
    // 0x4a00a0 _ZN15OnlineGameState11timevaldiffEP7timevalS1_
    // Milliseconds from 'from' to 'to'. The original divides the usec difference by 1000
    // in 32-bit arithmetic.
    std::int32_t timevaldiff(const Timeval* from, const Timeval* to) const;
    // 0x4a02f8 _ZN15OnlineGameState32GlliveRoomCreationTimeOutExpiredEv
    // True when more than 90000 ms have passed since the room was created.
    bool GlliveRoomCreationTimeOutExpired() const;
    // 0x4a0340 _ZN15OnlineGameState29GlliveStartGameTimeOutExpiredEv
    // True when more than 90000 ms have passed since the game started.
    bool GlliveStartGameTimeOutExpired() const;

    // Test seams. Not in the original.
    void SetStartGameTimeForTest(const Timeval& when) { startGame_ = when; }
    void SetRoomCreationTimeForTest(const Timeval& when) { roomCreation_ = when; }

private:
    Timeval startGame_{0, 0};      // +0x3c
    Timeval roomCreation_{0, 0};   // +0x44
};
