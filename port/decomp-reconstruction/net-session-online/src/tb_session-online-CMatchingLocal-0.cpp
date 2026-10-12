// Batch e1-net-session-online-CMatchingLocal-0: CMatchingLocal accessors, the room-state reset,
// IsInRoom() and the packet entry points. The entry points whose bodies run through the NetStruct
// member layer and the CMatching base class are partial: they compile, their unreproduced steps
// are named in each comment, and they return 0 (see the ledger notes).
#include "net-session-online/c_matching_local.hpp"

#include "net-session-online/deps/matching_stand_ins.hpp"

// 0x801b30 _ZNK14CMatchingLocal11GetMemberIdEv
std::int32_t CMatchingLocal::GetMemberId() const {
    return memberId_;  // +0x3638
}

// 0x801b48 _ZNK14CMatchingLocal17GetServerMemberIdEv
std::int32_t CMatchingLocal::GetServerMemberId() const {
    return serverMemberId_;  // +0x363c
}

// 0x805680 _ZN14CMatchingLocal14GetFriendsListEb
// Partial: the original zeroes the three-pointer vector it returns through the hidden result,
// which is an empty list. The element type is not recoverable from the symbol. tFriendInfo is
// assumed here, from the friend-entry record in the port, and is not confirmed by the body.
std::vector<tFriendInfo> CMatchingLocal::GetFriendsList(bool /*flag*/) {
    return {};
}

// 0x8057f0 _ZN14CMatchingLocal17ProcessLostPacketEii
// Partial: the original calls the base CMatching::ProcessLostPacket, NetStruct::ProcessLostPacket
// on the room-info record (+0x4A20), and vtable slot +0x44 of each of the 32 member records
// (408 bytes each, from +0x4BE0). None of those layers is in this port, so nothing runs and the
// result is 0 (the original returns the last slot call's result).
std::int32_t CMatchingLocal::ProcessLostPacket(std::int32_t /*a2*/, std::int32_t /*a3*/) {
    return 0;
}

// 0x80585c _ZN14CMatchingLocal25ProcessAcknowledgedPacketEii
// Partial: same shape as ProcessLostPacket, with vtable slot +0x40 of each member record and the
// base CMatching::ProcessAcknowledgedPacket and NetStruct::ProcessAcknowledgedPacket not in port.
std::int32_t CMatchingLocal::ProcessAcknowledgedPacket(std::int32_t /*a2*/, std::int32_t /*a3*/) {
    return 0;
}

// 0x805b14 _ZN14CMatchingLocal14ReadPacketDataEiiR12NetBitStream
// Partial: the original calls the base CMatching::ReadPacketData, then
// NetStruct::ConditionalLoad on the room-info record (+0x4A20) with a flag computed from the
// member ids, the server-backup array load or erase (+0x7EE0), and vtable slot +0x14 of each member
// record. None of those layers is in this port, so nothing is read from the stream and the result is 0.
std::int32_t CMatchingLocal::ReadPacketData(std::int32_t /*a2*/, std::int32_t /*a3*/,
                                            class NetBitStream& /*stream*/) {
    return 0;
}

// 0x8063c4 _ZN14CMatchingLocal15WritePacketDataEiiR12NetBitStream
// Partial: the original ORs the base CMatching::WritePacketData result, NetStruct::Serialize of the
// room-info record, NetStructArray::Serialize of the server-backup array and the member records'
// vtable slot +0x0c (which also resets the member's network id and its dirty flags). None of
// those layers is in this port, so nothing is written to the stream and the result is 0.
std::int32_t CMatchingLocal::WritePacketData(std::int32_t /*a2*/, std::int32_t /*a3*/,
                                             class NetBitStream& /*stream*/) {
    return 0;
}

// 0x805cc8 _ZN14CMatchingLocal8IsInRoomEv
// Partial. Exact: the local member is the server (both ids valid and equal), or the server id is
// connected. The final step reads CConnectionManager::IsInitialized(nullptr), a static of
// net-transport that is not in the stand-ins; it is taken as false here, so the connected-member
// count is not consulted and the result is false.
bool CMatchingLocal::IsInRoom() {
    if (memberId_ >= 0 && memberId_ == serverMemberId_) {
        return true;
    }
    if (GetConnectionMgr().IsConnected(serverMemberId_)) {
        return true;
    }
    // CConnectionManager::IsInitialized(nullptr) not in port: treated as not initialized.
    return false;
}

// 0x8074a4 _ZN14CMatchingLocal5ResetEv
// Partial. Reproduced: serverPollTime_ = 0, roomOperationPending_ = false, serverMemberId_ = -2,
// memberId_ = -1, and under serversMutex_ the server map is cleared (the original's tree erase and
// header reset). Not reproduced: the NetStruct member writes and dirty flags (room info at +0x4A20,
// the server-backup array at +0x7EE8, and the 32 member records from +0x4D00), the NetStructMember
// vtable slot calls on them, and the CNetworkId resets they use. Tests in this category define
// their own no-op Reset; do not link this file with them.
void CMatchingLocal::Reset() {
    serverPollTime_ = 0;             // +0x465c
    roomOperationPending_ = false;   // +0x3630
    serverMemberId_ = -2;            // +0x363c
    memberId_ = -1;                  // +0x3638
    serversMutex_.Lock();
    servers_.clear();                // +0x4644 map, count at +0x4654
    serversMutex_.Unlock();
}
