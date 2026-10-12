#include "net-session-online/multiplayer_callback.hpp"

// 0x49a0c4 _ZN19MultiplayerCallback9idleStateEPv
std::int32_t MultiplayerCallback::idleState(void* /*context*/) { return 100; }

// 0x49a0cc _ZN19MultiplayerCallback11ingameStateEPv
std::int32_t MultiplayerCallback::ingameState(void* /*context*/) { return 300; }
