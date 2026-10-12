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

    // Declared for the session loop (bodies in src/tb_session-online-COnline-0.cpp). Partial where noted there.
    enum tDisconnectCause : std::int32_t {};
    // 0x7fd744 _ZN7COnline11GetInstanceEv: the singleton, created on first use as a COnlineImpl.
    static COnline* GetInstance();
    // 0x7fd51c _ZN7COnline16ReportDisconnectENS_15tDisconnectCauseEi. Partial: the event queue at +0x08 is not declared.
    void ReportDisconnect(tDisconnectCause cause, std::int32_t detail);
    // 0x7fd568 _ZN7COnline7GetPingEv. Partial: the server ping is not in the stand-in (returns 0).
    std::int32_t GetPing() const;
    // 0x7fd634 _ZN7COnline11SendPacketsEv. Partial: CPacketManager and CMessaging send are not available.
    void SendPackets();
    // 0x7fd66c _ZN7COnline7DestroyEv. Partial: CMatching::Destroy is not in the stand-in.
    static void Destroy();
    // 0x7fd6d0 _ZN7COnline9TerminateEv. Partial: the initialized flag (+0x04) and event queue (+0x08) are not declared.
    void Terminate();
    // 0x7fd6f4 _ZN7COnline10InitializeEv. Partial: the initialized guard and the start-time store are not declared.
    std::int32_t Initialize();
    // 0x7fd914 _ZN7COnline14ReceivePacketsEv. Partial: start-time and counter fields and the packet manager are not declared.
    std::int32_t ReceivePackets();
    // 0x7fd974 _ZN7COnline12SendSyncTimeEv. Partial: the point-to-point send and sync-time field are not declared.
    void SendSyncTime();
    // 0x7fd9ec _ZN7COnline8SyncTimeEv. Partial: the list mutex and time-sync list are not declared.
    void SyncTime();
    // 0x7fdb58 _ZN7COnline6UpdateEf. Partial: the resend check needs the clock slot and sync fields.
    void Update(float dt);
    // 0x7fdbe4 _ZN7COnline22PacketReceiverCallbackEiPci. Partial: the server echo and the client sync list are not declared.
    void PacketReceiverCallback(std::int32_t memberId, char* data, std::int32_t size);
    // 0x7fdde8 _ZN7COnline23sPacketReceiverCallbackEiPci: forwards to the singleton.
    static void sPacketReceiverCallback(std::int32_t memberId, char* data, std::int32_t size);

    // The singleton (COnline::s_Online). Defined in the batch source.
    static COnline* s_Online;

    // Test seam. Not in the original.
    void SetTimeSyncedForTest(bool value) { timeSynced_ = value; }

private:
    bool isOnlineGame_ = false;  // +0x05
    bool timeSynced_ = false;    // +0x30
};
