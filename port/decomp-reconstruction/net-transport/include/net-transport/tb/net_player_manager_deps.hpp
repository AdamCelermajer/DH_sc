// Shims for the calls CNetPlayerManager makes into code that is not declared in the tree yet.
// Each shim keeps the original call's name and argument list. The bodies are inert and must be
// replaced by the real calls when the owning classes are merged. The functions that use a shim
// are ledgered as partial, and their notes name the shim.
#pragma once

#include "net-transport/net_bit_stream.hpp"
#include "net-transport/tb/cnet_player_info.hpp"

namespace np_deps {

// NetStruct, the replicated-info base at CNetPlayerManager+0x20. tb/netstruct.hpp was not in the
// tree when this batch was written (the NetStruct batch is still moving it), so this stand-in
// declares only the calls CNetPlayerManager makes. Replace it with NetStruct when it is merged.
class NetStructStandIn {
public:
    void SetEnabled(bool enabled) { (void)enabled; }  // NetStruct::SetEnabled (0x81366c)
    int Resend() { return 0; }                        // NetStruct::Resend() (0x81347c)
    int Load(NetBitStream& stream, int member_id, std::uint32_t a4) {  // NetStruct::Load (vt +0x14)
        (void)stream;
        (void)member_id;
        (void)a4;
        return 0;
    }
};

// GetOnline() and COnline::IsServer(). COnline is owned by net-session-online; this device is
// never the server in the stand-in.
struct OnlineStandIn {
    bool IsServer() const { return false; }
};
inline OnlineStandIn* GetOnline() {
    static OnlineStandIn online;
    return &online;
}

// NetStruct::ProcessLostPacket (0x814774) and NetStruct::ProcessAcknowledgedPacket (0x814d28).
// Not declared in the tree yet.
inline void NetStructProcessLostPacket(NetStructStandIn& net_struct, int a2, int a3) {
    (void)net_struct;
    (void)a2;
    (void)a3;
}
inline void NetStructProcessAcknowledgedPacket(NetStructStandIn& net_struct, int a2, int a3) {
    (void)net_struct;
    (void)a2;
    (void)a3;
}

// NetStruct::ConditionalSerialize (0x814cd4) is a tail jump to NetStruct::SerializeInternal,
// which is not declared in the tree yet.
inline int NetStructConditionalSerialize(NetStructStandIn& net_struct, bool is_server,
                                         NetBitStream& stream, int a2, int a3) {
    (void)net_struct;
    (void)is_server;
    (void)stream;
    (void)a2;
    (void)a3;
    return 0;
}

// CNetPlayerInfo virtual slots +0x08, +0x0c, +0x28, +0x2c, +0x40 and +0x44. They belong to the
// NetStruct base and are not declared in tb/cnet_player_info.hpp yet. The argument lists are the
// ones the Hex-Rays calls pass.
inline void PlayerSlot08(CNetPlayerInfo* player, NetBitStream* stream, int a2, int a3) {
    (void)player;
    (void)stream;
    (void)a2;
    (void)a3;
}
inline int PlayerSlot0C(CNetPlayerInfo* player, bool flag, NetBitStream* stream, int a2, int a3) {
    (void)player;
    (void)flag;
    (void)stream;
    (void)a2;
    (void)a3;
    return 0;
}
inline bool PlayerSlot28(CNetPlayerInfo* player, int a2) {
    (void)player;
    (void)a2;
    return false;
}
inline bool PlayerSlot2C(CNetPlayerInfo* player) {
    (void)player;
    return false;
}
inline void PlayerSlot40(CNetPlayerInfo* player, int a2, int a3) {
    (void)player;
    (void)a2;
    (void)a3;
}
inline void PlayerSlot44(CNetPlayerInfo* player, int a2, int a3) {
    (void)player;
    (void)a2;
    (void)a3;
}

// NetStruct::SetEnabled(player, flag) on a CNetPlayerInfo (the player is a NetStruct in the
// original). Inert until NetStruct is merged into CNetPlayerInfo.
inline void PlayerSetEnabled(CNetPlayerInfo* player, bool enabled) {
    (void)player;
    (void)enabled;
}

// CNetPlayerInfo virtual slot +0x18 (NetStruct vtable), called by ReadPacketData with
// (player, flag, stream, a2, 0). Result not used by the caller.
inline int PlayerSlot18(CNetPlayerInfo* player, bool flag, NetBitStream* stream, int a2, int a3) {
    (void)player;
    (void)flag;
    (void)stream;
    (void)a2;
    (void)a3;
    return 0;
}

// CNetPlayerInfo::Create() (0x8103d4) and CNetPlayerInfo::Delete(CNetPlayerInfo*) (0x81009c).
// Not declared on CNetPlayerInfo yet. The stand-ins create nothing and free nothing.
inline CNetPlayerInfo* CreatePlayerInfo() { return nullptr; }
inline void DeletePlayerInfo(CNetPlayerInfo* player) { (void)player; }

}  // namespace np_deps
