#include "net-session-online/online_game_state.hpp"

#include "net-session-online/tb/online_singleton.hpp"

namespace {
// The original compares the elapsed time with 90000 ms.
constexpr std::int32_t kTimeOutMs = 90000;
}  // namespace

// 0x43f9c4 _ZN15OnlineGameStateD1Ev
// Partial, see the header. Order as in the original: the handler list is freed node by node. Removing
// a node first links its successor (or the tail) to the node's own tail link, then stores the successor
// into that link. The enabled flag is cleared last. The vector at +0x50 and the state machine are
// destroyed by their member destructors after this body.
OnlineGameState::~OnlineGameState() {
    while (handlerHead_ != nullptr) {
        HandlerNode* node = handlerHead_;
        HandlerNode* next = node->next;
        if (next != nullptr) {
            next->tailBefore = node->tailBefore;
        } else {
            handlerTail_ = node->tailBefore;
        }
        *node->tailBefore = next;
        delete node;
    }
    handlersEnabled_ = false;
}

// 0x4a00a0 _ZN15OnlineGameState11timevaldiffEP7timevalS1_
std::int32_t OnlineGameState::timevaldiff(const Timeval* from, const Timeval* to) const {
    return (to->tv_sec - from->tv_sec) * 1000 + (to->tv_usec - from->tv_usec) / 1000;
}

// 0x4a0388 _ZN15OnlineGameState7DestroyEv
// Partial: see the header. The original ignores this and acts on OnlineSingleton<OnlineGameState>::GetInstance(),
// which creates the instance when it does not exist. The original's return value (the state machine's
// return) is not produced by the void destroyStateMachine here.
void OnlineGameState::Destroy() {
    OnlineSingleton<OnlineGameState>::GetInstance()->stateMachine_.destroyStateMachine();
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
