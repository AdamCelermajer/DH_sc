#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/deps/matching_stand_ins.hpp"

// Packet type ids used by these bodies: 1 server message, 2 client message/request,
// 4 change-server message (see the registrations in Initialize).

// 0x805d58 _ZN14CMatchingLocal23SendChangeServerMessageEv
std::int32_t CMatchingLocal::SendChangeServerMessage() {
    NetBitStream stream(0x800);
    const std::uint8_t type = 4;
    stream.WriteByteArray(&type, 1);
    const std::int32_t id = memberId_;
    stream.WriteByteArray(&id, sizeof id);
    return GetConnectionMgr().SendToAll(4, stream);
}

// 0x805de0 _ZN14CMatchingLocal18SendServerResponseEb
// Partial: the room attributes are written through the stand-in, which writes no bytes. The
// original's inet_ntoa() debug calls are not reproduced.
std::int32_t CMatchingLocal::SendServerResponse(bool flag) {
    NetBitStream stream(0x800);
    const std::uint8_t type = 1;
    stream.WriteByteArray(&type, 1);
    RoomServerHeader header{};
    header.listenId = GetListenNetworkId();
    header.memberId = memberId_;
    header.memberCount = GetMemberCount();
    header.flag = flag ? 1 : 0;
    stream.WriteByteArray(&header, sizeof header);
    roomAttributes_.Serialize(stream, 7);
    return GetConnectionMgr().SendBroadcast(2, stream);
}

// 0x805f2c _ZN14CMatchingLocal17SendClientRequestEv
// The listen address is fetched and then not written, as in the original.
std::int32_t CMatchingLocal::SendClientRequest() {
    NetBitStream stream(0x800);
    const std::uint8_t type = 2;
    stream.WriteByteArray(&type, 1);
    (void)GetListenNetworkId();
    return GetConnectionMgr().SendBroadcast(2, stream);
}

// Vtable slot +0x58 of ProcessClientMessage is not named in the vtable dump, and its body is
// not in this port. Until it is, the request is treated as not accepted.
bool CMatchingLocal::ClientRequestAccepted(const CNetworkId& /*from*/) const {
    return false;
}

// 0x805ee0 _ZN14CMatchingLocal20ProcessClientMessageER10CNetworkIdR12NetBitStream
// Partial: see ClientRequestAccepted. The original's stream argument is not read here.
void CMatchingLocal::ProcessClientMessage(const CNetworkId& from, NetBitStream& /*stream*/) {
    if (memberId_ >= 0 && memberId_ == serverMemberId_) {
        if (!ClientRequestAccepted(from)) {
            SendServerResponse(false);
        }
    }
}

// 0x805fa8 _ZN14CMatchingLocal22PacketReceiverCallbackEiPci
void CMatchingLocal::PacketReceiverCallback(std::int32_t from, char* data, std::uint32_t size) {
    NetBitStream stream(size);
    stream.SetBuffer(data, size);
    std::uint8_t type = 0;
    stream.ReadByteArray(&type, 1);
    if (type == 4) {
        ProcessChangeServerMessage(from, stream);
    }
}

// 0x80601c _ZN14CMatchingLocal23sPacketReceiverCallbackEiPci
// Routes to the matching object. The original does not check for a missing object either.
void CMatchingLocal::sPacketReceiverCallback(std::int32_t from, char* data, int size) {
    s_Matching->PacketReceiverCallback(from, data, static_cast<std::uint32_t>(size));
}

// 0x8098cc _ZN14CMatchingLocal31BroadcastPacketReceiverCallbackER10CNetworkIdPci
void CMatchingLocal::BroadcastPacketReceiverCallback(CNetworkId& from, char* data, std::uint32_t size) {
    NetBitStream stream(size);
    stream.SetBuffer(data, size);
    std::uint8_t type = 0;
    stream.ReadByteArray(&type, 1);
    if (type == 1) {
        ProcessServerMessage(from, stream);
    } else if (type == 2) {
        ProcessClientMessage(from, stream);
    }
}

// 0x80995c _ZN14CMatchingLocal32sBroadcastPacketReceiverCallbackER10CNetworkIdPci
void CMatchingLocal::sBroadcastPacketReceiverCallback(CNetworkId& from, char* data, int size) {
    s_Matching->BroadcastPacketReceiverCallback(from, data, static_cast<std::uint32_t>(size));
}
