// MultiplayerCallback: the multiplayer state callbacks. The state machine calls them
// with a context pointer. Only the two state-id callbacks are reconstructed here. The
// message handlers (HandleStartGameMessage and the rest) are queued.
#pragma once

#include <cstdint>

class MultiplayerCallback {
public:
    // 0x49a0c4 _ZN19MultiplayerCallback9idleStateEPv: the idle state id, 100.
    static std::int32_t idleState(void* context);
    // 0x49a0cc _ZN19MultiplayerCallback11ingameStateEPv: the in-game state id, 300.
    static std::int32_t ingameState(void* context);
};
