// OnlineGameState: the online game state. The original object is an
// OnlineSingleton<OnlineGameState> with the state machine at +0x14. The state machine,
// the timeout checks, Destroy/Update, the message-handler list, processMessages and Init
// are modelled here (partial, see each row). processEvents and the singleton template are not.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 handler-list enabled flag (byte)   +0x08 handler-list head   +0x0c handler-list tail link
//   +0x10 handler context pointer (passed to every handler)
//   +0x14 state machine (MultiplayerStateMachine)
//   +0x3c start-game timeval (sec, usec)   +0x44 room-creation timeval (sec, usec)
#pragma once

#include <cstdint>

#include "net-session-online/deps/time_of_day.hpp"
#include "net-session-online/multiplayer_state_machine.hpp"

class OnlineGameState {
public:
    // 0x49ffe0 / 0x4a0040 _ZN15OnlineGameStateC1Ev, _ZN15OnlineGameStateC2Ev. Partial: only the
    // members this port models are initialised. The handler list at +0x04..+0x10 is set to empty
    // (not enabled). The flags at +0x24 and the fields from +0x28 to +0x58 are not modelled.
    OnlineGameState() = default;
    // 0x43f9c4 _ZN15OnlineGameStateD1Ev. Partial: the tRoomInfo vector at +0x50 is not modelled.
    ~OnlineGameState() = default;

    // 0x4a0388 _ZN15OnlineGameState7DestroyEv. Partial: the original looks the object up through
    // OnlineSingleton<OnlineGameState>::GetInstance(); this port acts on the object it is called on.
    void Destroy();
    // 0x4a039c _ZN15OnlineGameState6UpdateEv. Partial: runs the state machine only. The
    // processEvents() and processMessages() calls, gated on the online-game flag, are not ported.
    void Update();
    // 0x49fb0c _ZN15OnlineGameState15processMessagesEv. Partial: the message-type dispatch over the
    // handler list is reconstructed. The block that the original runs while the byte at +0x24 is set
    // (NetPlayerMgr, event 3145728, PlayerJoined handler) is not. The original returns the last handler
    // result; no caller in the Hex-Rays corpus reads it, so this port returns nothing.
    void processMessages();
    // 0x4a04c0 _ZN15OnlineGameState4InitEi. Partial: see the .cpp. Builds the state table and the
    // handler list, then sets the state to 100. The matching-provider switch on the mode and the
    // GameCenter/PlayerSavegame branches are not reconstructed. Returns this (the original returns the
    // singleton).
    OnlineGameState* Init(int mode);

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
    // Message handler: takes the handler context and returns an int (the original's handler return).
    using MessageHandler = std::int32_t (*)(void*);
    // One entry of the handler list. Malloc'd 0x10 bytes in the original.
    struct HandlerNode {
        std::int32_t key;             // +0x00 message id (0 start game, 2 attack, ... 15 back button)
        MessageHandler handler;       // +0x04
        HandlerNode* next;            // +0x08
        HandlerNode** tailBefore;     // +0x0c the tail link that was current when this node was added
    };

    // Adds the handler for this key, or replaces the handler of the first node with this key.
    void setHandler(std::int32_t key, MessageHandler handler);
    // Calls every handler whose key matches, in list order, with the handler context.
    void runHandlers(std::int32_t key);

    bool handlersEnabled_ = false;                          // +0x04
    HandlerNode* handlerHead_ = nullptr;                    // +0x08
    HandlerNode** handlerTail_ = &handlerHead_;             // +0x0c
    void* handlerContext_ = nullptr;                        // +0x10
    MultiplayerStateMachine stateMachine_;                  // +0x14 (16 bytes, through +0x24)
    Timeval startGame_{0, 0};                               // +0x3c
    Timeval roomCreation_{0, 0};                            // +0x44
};
