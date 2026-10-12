// CNetPlayerManager: the table of network players of the session, from libDungeonHunter2.so.
//
// Layout (32-bit offsets from the original, kept as comments, never used for pointer arithmetic):
//   +0x000..+0x01f  base part, not recovered (the class is not declared with its base here).
//   +0x020          net_struct_ (NetStruct, the replicated manager info; ResendManagerInfo and the
//                   packet callbacks forward to it).
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
#include <vector>

#include "net-transport/deps/connection_deps.hpp"  // CMatching (stand-in, src/connection_deps.cpp)
#include "net-transport/net_bit_stream.hpp"
#include "net-transport/tb/cnet_player_info.hpp"
#include "net-transport/tb/net_player_manager_deps.hpp"

class CNetPlayerManager {
public:
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
    int ResendManagerInfo();                                            // 0x8103f4
    void ProcessLostPacket(int a2, int a3);                             // 0x8103fc
    static void sProcessLostPacket(int a2, int a3);                     // 0x8104c0
    void ProcessAcknowledgedPacket(int a2, int a3);                     // 0x8104ec
    static void sProcessAcknowledgedPacket(int a2, int a3);             // 0x8105b0

    // Packet serialisation.
    int WritePacketData(int a2, int a3, NetBitStream& stream);          // 0x810610
    static int sWritePacketData(int a2, int a3, NetBitStream& stream);  // 0x81086c

    // Removal.
    int RemovePlayer(int index);                                        // 0x8108a8

    // Singleton and factory state. The definitions are in src/tb_transport-CNetPlayerManager-0.cpp.
    static CNetPlayerManager* s_playerManager;
    static CNetPlayerInfo* (*CreateNetPlayerInfo)();
    static void (*DeleteNetPlayerInfo)(CNetPlayerInfo*);

private:
    np_deps::NetStructStandIn net_struct_;          // +0x20 (NetStruct; stand-in until merged)
    bool initialized_ = false;                      // +0x178
    CNetPlayerInfo* fallback_player_ = nullptr;     // +0x17c
    int player_slot_count_ = 0;                     // +0x194
    std::vector<CNetPlayerInfo*> players_;          // +0x198, +0x19c
    std::vector<CNetPlayerInfo*> secondary_players_;  // +0x1a4, +0x1a8
};
