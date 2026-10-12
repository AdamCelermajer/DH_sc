#include "net-session-online/online_game_state.hpp"

namespace {
// The original compares the elapsed time with 90000 ms.
constexpr std::int32_t kTimeOutMs = 90000;
}  // namespace

// 0x4a00a0 _ZN15OnlineGameState11timevaldiffEP7timevalS1_
std::int32_t OnlineGameState::timevaldiff(const Timeval* from, const Timeval* to) const {
    return (to->tv_sec - from->tv_sec) * 1000 + (to->tv_usec - from->tv_usec) / 1000;
}

// 0x4a0388 _ZN15OnlineGameState7DestroyEv
// Partial: see the header. The original's return value is the state machine's return, which
// the port's runStateMachine does not produce, so Destroy returns nothing.
void OnlineGameState::Destroy() {
    stateMachine_.destroyStateMachine();
}

// 0x4a039c _ZN15OnlineGameState6UpdateEv
// Partial: processEvents() and processMessages() are not ported (see the header).
void OnlineGameState::Update() {
    stateMachine_.runStateMachine();
}

// 0x4a02f8 _ZN15OnlineGameState32GlliveRoomCreationTimeOutExpiredEv
bool OnlineGameState::GlliveRoomCreationTimeOutExpired() const {
    Timeval now{};
    GetTimeOfDay(&now);
    return timevaldiff(&roomCreation_, &now) > kTimeOutMs;
}

// 0x4a0340 _ZN15OnlineGameState29GlliveStartGameTimeOutExpiredEv
bool OnlineGameState::GlliveStartGameTimeOutExpired() const {
    Timeval now{};
    GetTimeOfDay(&now);
    return timevaldiff(&startGame_, &now) > kTimeOutMs;
}
