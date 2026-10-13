// CNetPlayerManager: the table of network players of the session, from libDungeonHunter2.so.
//
// Layout (32-bit offsets from the original, kept as comments, never used for pointer arithmetic):
//   +0x000..+0x01f  base part, not recovered (the class is not declared with its base here).
//   +0x020          net_struct_ (NetStructPlayerManager: NetStruct plus one member, the host member id at
//                   +0x150; ResendManagerInfo and the packet callbacks forward to it).
//   +0x178          initialized_ (byte). Its name follows the method IsInitialized.
//   +0x17c          fallback_player_ (CNetPlayerInfo*). Returned when a lookup finds nothing.
//                   Its name is not recovered.
//   +0x194          player_slot_count_ (int): the number of player slots (limit of GetPlayerInternal).
//   +0x198, +0x19c  players_ (begin, end): one CNetPlayerInfo* per slot.
//   +0x1a4, +0x1a8  secondary_players_ (begin, end): second list, same element type. Its name is
//                   not recovered.
//
// Statics: s_playerManager (the singleton), CreateNetPlayerInfo and DeleteNetPlayerInfo (the
// player factory hooks set by RegisterPlayerInfoFunctions).
#pragma once

#include <cstddef>
#include <map>
#include <vector>

#include "net-session-online/deps/matching.hpp"  // CMatching (one definition, net-session-online)
#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_struct.hpp"
#include "net-transport/tb/cnet_player_info.hpp"
#include "net-transport/tb/net_player_manager_deps.hpp"
#include "net-transport/tb/net_struct_int.hpp"

class CNetPlayerManager {
public:
    // The replicated manager info (CNetPlayerManager::NetStructPlayerManager). NetStruct (0x130 bytes) plus one
    // NetStructInt<6u> block at +0x130 that holds the host's member id: its constructor declares the block and
    // sets it to -1, and CNetPlayerInfo::IsHost compares a player's member id with its value (manager +0x170).
    class NetStructPlayerManager : public NetStruct {
    public:
        NetStructPlayerManager();                        // 0x810d90
        ~NetStructPlayerManager() override;              // 0x810ec8 (D1); the deleting form 0x810f3c is the compiler's

        NetStructInt<6u> host_member_id_;                // +0x130 (value at +0x150)
    };

    virtual ~CNetPlayerManager() = default;

    // Singleton access.
    static CNetPlayerManager* GetInstance();  // 0x8100bc
    static bool IsInitialized();              // 0x8100e0
    static void Terminate();                  // 0x810108

    // Lookups.
    CNetPlayerInfo* GetPlayerInternal(int index) const;                 // 0x81014c
    CNetPlayerInfo* GetPlayerInternal(int first_key, int second_key) const;  // 0x810180
    CNetPlayerInfo* GetPlayer(int index, bool any_state) const;         // 0x8101e0
    CNetPlayerInfo* GetPlayerByMemberId(int first_key, int second_key,  // 0x810228
                                        bool allow_inactive) const;
    int GetFreePlayerId() const;                                        // 0x810270
    CNetPlayerInfo* GetLocalPlayer(int second_key) const;               // 0x8105dc

    // Player factory hooks.
    static void RegisterPlayerInfoFunctions(CNetPlayerInfo* (*create)(),
                                            void (*destroy)(CNetPlayerInfo*));  // 0x8102c0
    static void UnregisterPlayerInfoFunctions();                        // 0x8102f0

    // Packet bookkeeping over the NetStruct and every player.
    bool AreAllPlayersAcknowledged() const;                             // 0x810338
    void ResendManagerInfo();                                           // 0x8103f4
    void ProcessLostPacket(int a2, int a3);                             // 0x8103fc
    static void sProcessLostPacket(int a2, int a3);                     // 0x8104c0
    void ProcessAcknowledgedPacket(int a2, int a3);                     // 0x8104ec
    static void sProcessAcknowledgedPacket(int a2, int a3);             // 0x8105b0

    // Packet serialisation.
    int WritePacketData(int a2, int a3, NetBitStream& stream);          // 0x810610
    static int sWritePacketData(int a2, int a3, NetBitStream& stream);  // 0x81086c

    // Removal.
    int RemovePlayer(int index);                                        // 0x8108a8

    // Player list maintenance (batch e2-net-transport-CNetPlayerManager-0e46d6-0).
    int DisconnectPlayer(int a2);                                       // 0x8108c0
    int SetPlayerGamepadId(int a2, int a3);                             // 0x810b7c
    int ReassignPlayer(int a2, int a3, int a4);                         // 0x810bd4
    void ClearPlayers();                                                // 0x810c88
    void InitPlayers();                                                 // 0x8111ec
    void ResetPlayers();                                                // 0x811308
    std::vector<int>& GetPlayerIdList();                                // 0x8113fc
    int GetPlayerCount();                                               // 0x8114ec
    void ClearPlayerListCaches(int a2);                                 // 0x812608

    // Id lists by member (batch e2-net-transport-CNetPlayerManager-0e46d6-1).
    std::vector<int>& GetPlayerIdListByMemberId(int a2);                // 0x812d50
    int GetMemberPlayerCount(int a2);                                   // 0x812e74
    bool MemberHasActivePlayers(int a2);                                // 0x812e90
    std::vector<int>& GetLocalPlayerIdList();                           // 0x812ea8
    int GetLocalPlayerCount();                                          // 0x812ed0

    // Packet reads. AddPlayer(NetBitStream&, ...) is declared for ReadPacketData (not in this batch's
    // reconstructed set; see the ledger).
    void AddPlayer(NetBitStream& stream, int a2, int a3);               // 0x811be8
    NetBitStream* ReadPacketData(int a2, int a3, NetBitStream& stream); // 0x811d74
    static NetBitStream* sReadPacketData(int a2, int a3, NetBitStream& stream);  // 0x811e94

    // Singleton and factory state. The definitions are in src/tb_transport-CNetPlayerManager-0.cpp.
    static CNetPlayerManager* s_playerManager;
    static CNetPlayerInfo* (*CreateNetPlayerInfo)();
    static void (*DeleteNetPlayerInfo)(CNetPlayerInfo*);

private:
    friend class CNetPlayerInfo;  // IsHost reads the host member id at +0x170 (net_struct_.host_member_id_)

    NetStructPlayerManager net_struct_;             // +0x20
    bool initialized_ = false;                      // +0x178
    CNetPlayerInfo* fallback_player_ = nullptr;     // +0x17c
    int player_slot_count_ = 0;                     // +0x194
    std::vector<CNetPlayerInfo*> players_;          // +0x198, +0x19c
    std::vector<CNetPlayerInfo*> secondary_players_;  // +0x1a4, +0x1a8
    std::int32_t unk_1b0_ = 0;                        // +0x1b0. Set to -1 by ResetPlayers; name not recovered.
    bool id_list_dirty_ = false;                      // +0x1b4 (byte). Set by the id-list cache.
    std::vector<int> player_id_list_;                 // +0x1b8, +0x1bc (begin, end). Cached id list.
    std::map<int, std::vector<int>> player_list_caches_;  // +0x1c4. Per-key lists (name not recovered).
};
