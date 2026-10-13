// OnlineGameState: the online game state. The original object is an
// OnlineSingleton<OnlineGameState> with the state machine at +0x14. The state machine,
// the timeout checks, Destroy/Update, the message-handler list, processMessages and Init
// are modelled here (partial, see each row). processEvents and the singleton template are not.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x04 handler-list enabled flag (byte)   +0x08 handler-list head   +0x0c handler-list tail link
//   +0x10 handler context pointer (passed to every handler)
//   +0x14 state machine (MultiplayerStateMachine)
//   +0x24..+0x28 five flag bytes (see the members below)   +0x30 and +0x38 menu-name pointers   +0x34 mode word
//   +0x3c start-game timeval (sec, usec)   +0x44 room-creation timeval (sec, usec)
//   +0x4c, +0x4d, +0x4e flag bytes
//   +0x50 std::vector<tRoomInfo> (begin, end, capacity; zeroed by the ctor, freed by the dtor)
#pragma once

#include <cstdint>
#include <vector>

#include "net-session-online/deps/time_of_day.hpp"
#include "net-session-online/multiplayer_state_machine.hpp"
#include "net-session-online/t_room_info.hpp"

class OnlineGameState {
public:
    // 0x49ffe0 / 0x4a0040 _ZN15OnlineGameStateC1Ev, _ZN15OnlineGameStateC2Ev. Partial. The handler list
    // at +0x04..+0x10 starts empty with the enabled flag at +0x04 set to 1 (member default below). The
    // tRoomInfo vector at +0x50 starts empty. The flag bytes (+0x24..+0x28, +0x4c..+0x4e) start at zero. The state machine at +0x14 is default-constructed here, while the
    // original leaves it to Init (initStateMachine).
    OnlineGameState() = default;
    // 0x43f9c4 _ZN15OnlineGameStateD1Ev. Partial. Frees the handler list (in list order, unlinking each
    // node as the original does), clears the enabled flag at +0x04 and destroys the tRoomInfo vector at
    // +0x50 (member). Deviation: the state machine member frees its entries in its own destructor. The
    // original's D1 does not do that (the entries are freed by Destroy), so the port frees them earlier.
    ~OnlineGameState();

    // 0x4a0388 _ZN15OnlineGameState7DestroyEv. Partial. The original ignores this and works on the
    // OnlineSingleton<OnlineGameState> instance, which this port does too. The original returns the
    // state machine's return value, which the void destroyStateMachine here does not produce.
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

    // The state machine's current state (+0x14, the word at its +0x00).
    std::int32_t GetState() const { return stateMachine_.CurrentState(); }

    // Fields that other code reads and writes inline, at the original offsets (no symbols, so the names are
    // positions; the roles are those seen in the readers). The constructor zeroes only +0x28, +0x4d and +0x4e; the
    // port zeroes all of them. Public, as Track B decision 9 has it.
    bool flag24_ = false;                  // +0x24 online-game gate: createRoom returns 101 when clear; gates the
                                           // CNetPlayerManager block of processMessages; read by PlayerManager
    bool flag25_ = false;                  // +0x25 set to 1 after the join call (createRoom, searchRoom)
    bool flag26_ = false;                  // +0x26 read by HandleMenuBackButton
    bool flag27_ = false;                  // +0x27 read by HandleMenuBackButton
    bool flag28_ = false;                  // +0x28 zeroed by the constructor (roomLobbyUpdate, lobbyUpdate)
    const char* menuName30_ = nullptr;     // +0x30 menu name HandleMenuBackButton dispatches on
    std::int32_t mode_ = 0;                // +0x34 the mode word Init's switch stores; 2 and 3 select the attribute branch
    const char* menuName38_ = nullptr;     // +0x38
    bool flag4c_ = false;                  // +0x4c
    bool flag4d_ = false;                  // +0x4d zeroed by the constructor; set on the GL Live path of createRoom
    bool flag4e_ = false;                  // +0x4e zeroed by the constructor

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

    bool handlersEnabled_ = true;                           // +0x04 (the ctor stores 1; the dtor stores 0)
    HandlerNode* handlerHead_ = nullptr;                    // +0x08
    HandlerNode** handlerTail_ = &handlerHead_;             // +0x0c
    void* handlerContext_ = nullptr;                        // +0x10
    MultiplayerStateMachine stateMachine_;                  // +0x14 (16 bytes, through +0x23)
    Timeval startGame_{0, 0};                               // +0x3c
    Timeval roomCreation_{0, 0};                            // +0x44
    std::vector<tRoomInfo> roomInfos_;                      // +0x50 (12-byte vector in the original)
};
