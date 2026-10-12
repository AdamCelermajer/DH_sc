// OnlineGameState: the online game state. The original object is an
// OnlineSingleton<OnlineGameState> with the state machine at +0x14. The state machine,
// the timeout checks and Destroy/Update are modelled here. The processEvents/processMessages
// bodies and the singleton template are not.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x3c start-game timeval (sec, usec)   +0x44 room-creation timeval (sec, usec)
#pragma once

#include <cstdint>

#include "net-session-online/deps/time_of_day.hpp"
#include "net-session-online/multiplayer_state_machine.hpp"

class OnlineGameState {
public:
    // 0x49ffe0 / 0x4a0040 _ZN15OnlineGameStateC1Ev, _ZN15OnlineGameStateC2Ev. Partial: only the
    // members this port models are initialised. The list and flags at +0x04..+0x58 are not.
    OnlineGameState() = default;
    // 0x43f9c4 _ZN15OnlineGameStateD1Ev. Partial: the tRoomInfo vector at +0x50 is not modelled.
    ~OnlineGameState() = default;

    // 0x4a0388 _ZN15OnlineGameState7DestroyEv. Partial: the original looks the object up through
    // OnlineSingleton<OnlineGameState>::GetInstance(); this port acts on the object it is called on.
    void Destroy();
    // 0x4a039c _ZN15OnlineGameState6UpdateEv. Partial: runs the state machine only. The
    // processEvents() and processMessages() calls, gated on the online-game flag, are not ported.
    void Update();

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
    MultiplayerStateMachine stateMachine_;  // +0x14 (16 bytes, through +0x24)
    Timeval startGame_{0, 0};               // +0x3c
    Timeval roomCreation_{0, 0};            // +0x44
};
