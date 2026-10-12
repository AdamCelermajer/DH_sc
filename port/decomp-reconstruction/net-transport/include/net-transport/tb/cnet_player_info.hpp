// CNetPlayerInfo: per-player record of the net player manager, from libDungeonHunter2.so.
//
// Layout (32-bit offsets from the original, taken from the Hex-Rays pseudocode of the methods
// in batch e1-net-transport-CNetPlayerInfo-0):
//   +0x000..+0x12f  base NetStruct (vptr off_9924D8 / off_992370). NetStruct is not in the tree
//                   yet, so the base range is not declared here.
//   +0x130..+0x24f  NetStructMember-derived blocks, 0x28 bytes each (vptr at +0, the value
//                   word at +0x20 of the block). The blocks are not declared as objects here.
//   +0x178  member_id_    value of the block at +0x158: network member id (compared with the
//                         host member id in IsHost, passed to the net player manager events).
//   +0x1a0  conn_index_   value of the block at +0x180: connection slot index, -1 when none.
//   +0x1c8  unk_1c8       value of the block at +0x1a8: -1 when none (its name is not known).
//   +0x1f0  state_       value of the block at +0x1d0. Values seen: 1 and 2 are the activating
//                         states (IsActivating), 3 is active (IsActive), 4 and 5 are the
//                         disconnect states (4 on the server, 5 on a client), 6 is removed
//                         (client side). 0 is the reset state (Reset, Remove on the server).
//   +0x280  applied_state_  copy of state_ made by Update (the original's +0x280 word).
//
// Layout request (for the integrator): the base NetStruct (0x130 bytes, with a std::map of
// NetStruct::tPacketHistory at +0x10c and the byte flags at +0x124 and +0x125), the
// NetStructMember blocks at +0x130..+0x248 and the NetStructString<32> member at +0x248 must be
// declared before the field offsets above can hold in a single class. Until then the class
// declares only the fields the methods in this batch use, and the offsets are comments.
//
// Virtual slots (original vtable, from tools/decomp_vtable.py): +0x48 Update, +0x4c Reset,
// +0x50 IsLocal, +0x54 IsLocalTo, +0x58 IsInRoom, +0x5c IsActive, +0x60 IsActivating. The NetStruct
// virtuals in slots +0x00..+0x44 are not declared yet.
#pragma once

#include <cstdint>

class CNetPlayerInfo {
public:
    // Declared for the callers in the batch. Their bodies are not in this batch
    // (see the ledger notes): SetState, IsHost, IsInRoom, the constructors, operator= and the
    // destructors.
    void SetState(char state);  // 0x36f40c, skipped
    bool IsHost() const;        // 0x80f23c, skipped
    bool IsInRoom() const;      // 0x80f180, skipped (vtable +0x58)

    // Reconstructed or partial bodies.
    virtual void Update();                  // 0x80f3d0, partial (vtable +0x48)
    virtual void Reset();                   // 0x80f27c, partial (vtable +0x4c)
    virtual bool IsLocal() const;           // 0x80f1ec, reconstructed (vtable +0x50)
    virtual bool IsLocalTo(int member_index) const;  // 0x80f1a0, reconstructed (vtable +0x54)
    virtual bool IsActive() const;          // 0x80f124, reconstructed (vtable +0x5c)
    virtual bool IsActivating() const;      // 0x36d208, reconstructed (vtable +0x60)

    bool IsRemote() const;                  // 0x80f164, reconstructed
    void Add();                             // 0x80f260, partial
    void Remove();                          // 0x80f2e0, partial
    void Disconnect();                      // 0x80f374, partial

    // Fields (32-bit offsets in the comments; see the layout notes above).
    std::int32_t member_id_ = -1;     // +0x178
    std::int32_t conn_index_ = -1;    // +0x1a0
    std::int32_t unk_1c8 = -1;        // +0x1c8
    std::int32_t state_ = 0;          // +0x1f0
    std::int32_t applied_state_ = 0;  // +0x280
};
