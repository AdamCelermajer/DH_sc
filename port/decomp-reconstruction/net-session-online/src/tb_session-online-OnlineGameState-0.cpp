// Batch e1-net-session-online-OnlineGameState-0.
//   processEvents: not rebuilt (see the ledger row; skipped).
//   processMessages and Init(int): partial, see each function.
#include "net-session-online/online_game_state.hpp"

#include "net-session-online/c_messaging.hpp"
#include "net-session-online/multiplayer_callback.hpp"
#include "net-transport/tb/c_net_player_manager.hpp"

namespace {

// The MultiplayerCallback handlers are non-static members in multiplayer_callback.hpp. The
// original symbols take no this pointer, so each entry below calls the member through a
// temporary. The handler bodies are partial and read no member state, so this is the same call.
// The state callbacks idleState and ingameState are already static and are used directly.
using StateCallback = std::int32_t (*)(void*);

struct StateEntryInit {
    std::int32_t stateId;
    StateCallback callback;
};

// Init adds these entries in this order, each with skip = 0.
const StateEntryInit kStateEntries[] = {
    {100, &MultiplayerCallback::idleState},
    {300, &MultiplayerCallback::ingameState},
    {101, [](void* c) -> std::int32_t { return MultiplayerCallback{}.createRoom(c); }},
    {102, [](void* c) -> std::int32_t { return MultiplayerCallback{}.searchRoom(c); }},
    {104, [](void* c) -> std::int32_t { return MultiplayerCallback{}.searchRoomPushNotif(c); }},
    {103, [](void* c) -> std::int32_t { return MultiplayerCallback{}.joinRoom(c); }},
    {200, [](void* c) -> std::int32_t { return MultiplayerCallback{}.lobbyUpdate(c); }},
    {201, [](void* c) -> std::int32_t { return MultiplayerCallback{}.roomLobbyUpdate(c); }},
};

struct HandlerInit {
    std::int32_t key;
    std::int32_t (*handler)(void*);
};

// Init registers these handlers in this order (only when the handler list is enabled).
const HandlerInit kHandlers[] = {
    {0, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleStartGameMessage(m); }},
    {6, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleMenuReadyMessage(m); }},
    {7, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleScriptCmd(m); }},
    {8, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleInitialMessage(m); }},
    {9, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleQuestsSyncMessage(m); }},
    {2, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleAttackMessage(m); }},
    {3, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleRaisedEventMessage(m); }},
    {4, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleLootMessage(m); }},
    {5, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleSpawnObjectMessage(m); }},
    {10, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleIsHostMessage(m); }},
    {11, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleGlobalDeathMessage(m); }},
    {12, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleAskResendAttributesMessage(m); }},
    {13, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandlePlayerJoined(m); }},
    {14, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandlePlayerLeft(m); }},
    {15, [](void* m) -> std::int32_t { return MultiplayerCallback{}.HandleMenuBackButton(m); }},
};

}  // namespace

// Inlined into Init in the original once per handler (no symbol of its own).
// Finds the node with this key and replaces its handler, or appends a node at the tail.
void OnlineGameState::setHandler(std::int32_t key, MessageHandler handler) {
    for (HandlerNode* node = handlerHead_; node != nullptr; node = node->next) {
        if (node->key == key) {
            node->handler = handler;
            return;
        }
    }
    // The original mallocs 0x10 bytes and checks for null. This port uses new, which does not return null.
    auto* node = new HandlerNode{key, handler, nullptr, handlerTail_};
    *handlerTail_ = node;
    handlerTail_ = &node->next;
}

// Part of the handler dispatch used by processMessages (inlined in the original, no symbol of its own).
void OnlineGameState::runHandlers(std::int32_t key) {
    for (HandlerNode* node = handlerHead_; node != nullptr; node = node->next) {
        if (node->key == key) {
            node->handler(handlerContext_);
        }
    }
}

// 0x49fb0c _ZN15OnlineGameState15processMessagesEv
// The message-type dispatch over the handler list, in the original order, with the joined-player block between
// CMsgQuestsSync and CMsgGlobalDeath. The original's int return value (the last handler result) is not kept (see the
// header).
void OnlineGameState::processMessages() {
    // CMessaging::Get() is a thunk to CMessaging::GetInstance(). Each block fetches the same singleton.
    CMessaging* messaging = CMessaging::GetInstance();
    if (messaging->IsMessageReceived("CMsgStartGame")) {
        runHandlers(0);
    }
    if (messaging->IsMessageReceived("CMsgIsHost")) {
        runHandlers(10);
    }
    if (messaging->IsMessageReceived("CMsgSpawnObject")) {
        runHandlers(5);
    }
    if (messaging->IsMessageReceived("CMsgMenuReady")) {
        runHandlers(6);
    }
    if (messaging->IsMessageReceived("CMsgAttackResult")) {
        runHandlers(2);
    }
    if (messaging->IsMessageReceived("CMsgRaisedEvent")) {
        runHandlers(3);
    }
    if (messaging->IsMessageReceived("CMsgDropLoot")) {
        runHandlers(4);
    }
    if (messaging->IsMessageReceived("CMsgScriptCmd")) {
        runHandlers(7);
    }
    if (messaging->IsMessageReceived("CMsgInitialSetup")) {
        runHandlers(8);
    }
    if (messaging->IsMessageReceived("CMsgQuestsSync")) {
        runHandlers(9);
    }
    // While the online-game byte (+0x24) is set and the player manager is initialised, a joined-player event
    // (3145728) in the manager's internal queue (+0x180) runs the handlers of key 13 (HandlePlayerJoined). GetNetPlayerMgr
    // is the thunk to CNetPlayerManager::GetInstance.
    if (flag24_ && CNetPlayerManager::IsInitialized() &&
        CNetPlayerManager::GetInstance()->internal_events_.HasEventOccured(3145728, false)) {
        runHandlers(13);
    }
    if (messaging->IsMessageReceived("CMsgGlobalDeath")) {
        runHandlers(11);
    }
    if (messaging->IsMessageReceived("CMsgAskResendAttributes")) {
        runHandlers(12);
    }
}

// 0x4a04c0 _ZN15OnlineGameState4InitEi
// Partial. The state table (8 entries, skip 0, in the binary's order: 100 idleState, 300 ingameState,
// 101 createRoom, 102 searchRoom, 104 searchRoomPushNotif, 103 joinRoom, 200 lobbyUpdate, 201
// roomLobbyUpdate) and the 15-handler list are reconstructed and match the listing, as is the state-100
// store at the end.
//
// MISSING, in binary order (asm 0x4a0a10 on, 852 instructions total):
//   a) `GetOnline()->COnlineImpl::Initialize()` (0x824f40, 4a0a10..4a0a14) runs before the mode switch.
//   b) The switch on mode (4a0a18..4a0a24 first tests GetOnline()+5, the online-game byte, and skips the
//      whole switch when it is already set), cases 1 and 2 at 0x4a1170 / 0x4a13e4, plus the 3 and 4 paths
//      around 0x4a136c and 0x4a11c8. Each case does CMatching::Destroy, CMatching::SetMatchingProvider
//      (0x7fe4c0), DebugSwitches::load + GetSwitch("ConnectToAlphaServer"/"ConnectToBetaServer"),
//      Application::GetVersionString, the online version assign, CGameCenter user/host UID and player
//      count, COnline::SetIsOnlineGame, and for modes 1/2 CMatchingGLLive::SetPlayerParam; the mode is then
//      stored at +0x34.
//   c) The host block after the switch (0x4a15c0): builds a CRoomAttributes, PlayerSavegame::SG_Load(16),
//      SG_GetCurrentPrimaryQuest, six SetAttributeInt calls, one SetAttributeBin, then assigns into
//      CMatching::Get()->roomAttributes_.
//   d) COnlineImpl::SignIn at 0x4a1554.
// The state machine entries are the only part that runs today, so a mode-1..4 Init leaves the matching
// provider, the online-game flag and the host room attributes unset.
OnlineGameState* OnlineGameState::Init(int mode) {
    (void)mode;  // consumed only by the mode switch that is not reconstructed (see above)

    stateMachine_.initStateMachine();
    for (const StateEntryInit& entry : kStateEntries) {
        stateMachine_.addStateEntry(entry.stateId, entry.callback, false);
    }

    // The original registers the handlers only when the list is enabled at +0x04.
    if (handlersEnabled_) {
        for (const HandlerInit& entry : kHandlers) {
            setHandler(entry.key, entry.handler);
        }
    }

    // The state set after the switch in the original.
    stateMachine_.SetStateForTest(100);
    return this;
}
