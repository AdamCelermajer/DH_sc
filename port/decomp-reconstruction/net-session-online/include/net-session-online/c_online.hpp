// COnline: the online session object. It tracks whether the session is an online game
// and whether the clock is synced with the server. The matching layer (CMatching, a
// stand-in in deps/) answers the member questions when the game is online. Only the
// small accessors are reconstructed here. The packet loop, the sync and the ping are
// queued.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x05 online-game flag (byte)
//   +0x30 time-synced flag (byte), read by IsTimeSynced when this is not the server
//
// The methods are non-virtual in the original (direct calls). The vtable and the dtors
// are queued, so the class is not polymorphic here.
#pragma once

#include <cstdint>

class COnline {
public:
    // 0x7fd798 _ZN7COnlineC1Ev and 0x7fd838 _ZN7COnlineC2Ev. Partial: the flags are set to
    // their defaults. The two CNetMutex members and the two list heads (+0x0c..+0x3c) are not
    // modelled here, so the sentinels and mutexes are not initialised.
    COnline() = default;
    // 0x7fda38 _ZN7COnlineD2Ev and 0x7fda98 _ZN7COnlineD1Ev. Partial: the list clears and the
    // CNetMutex destructors are not modelled (see the constructor).
    ~COnline() = default;

    // 0x7fd508 _ZN7COnline16DetectDisconnectEv: no-op.
    void DetectDisconnect() {}
    // 0x7fd50c _ZN7COnline15SetIsOnlineGameEb
    void SetIsOnlineGame(bool value) { isOnlineGame_ = value; }
    // 0x7fd514 _ZN7COnline15IsAgeRestrictedEv: returns 0.
    std::int32_t IsAgeRestricted() const { return 0; }
    // 0x7fd588 _ZN7COnline16GetLocalMemberIdEv: 1 when offline, else the matching local id.
    std::int32_t GetLocalMemberId() const;
    // 0x7fd5b4 _ZN7COnline8IsServerEv: 1 when offline, else the matching server flag.
    bool IsServer() const;
    // 0x7fd5d8 _ZN7COnline12IsTimeSyncedEv: 1 on the server, else the synced flag.
    bool IsTimeSynced() const;

    // Test seam. Not in the original.
    void SetTimeSyncedForTest(bool value) { timeSynced_ = value; }

private:
    bool isOnlineGame_ = false;  // +0x05
    bool timeSynced_ = false;    // +0x30
};
