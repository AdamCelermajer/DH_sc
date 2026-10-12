// Batch e1-net-session-online-OnlineGameState-0.
//   processEvents: not rebuilt (see the ledger row; skipped).
//   processMessages and Init(int): partial, see each function.
#include "net-session-online/online_game_state.hpp"

#include "net-session-online/c_messaging.hpp"
#include "net-session-online/multiplayer_callback.hpp"

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
// Partial: the message-type dispatch is reconstructed. Not reconstructed: the block that the
// original runs while the byte at +0x24 is set (NetPlayerMgr, event 3145728, PlayerJoined
// handler 13). The original's int return value is not kept (see the header).
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
    // Not reconstructed: the block gated on the byte at +0x24 (see the header comment).
    if (messaging->IsMessageReceived("CMsgGlobalDeath")) {
        runHandlers(11);
    }
    if (messaging->IsMessageReceived("CMsgAskResendAttributes")) {
        runHandlers(12);
    }
}

// 0x4a04c0 _ZN15OnlineGameState4InitEi
// Partial: the state table and the handler list are reconstructed. The matching-provider switch
// on mode (cases 1 to 4, which set the COnline flag, change the CMatching provider and read the
// DebugSwitches and GameCenter values) is not, and neither is the tail that follows it. The mode
// is therefore unused here.
OnlineGameState* OnlineGameState::Init(int mode) {
    (void)mode;  // used only by the switch that is not reconstructed

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
