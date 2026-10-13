#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"
#include "net-session-online/c_room_attributes.hpp"  // CRoomAttributes (real)
#include "net-session-online/c_room_search_filter.hpp"  // CRoomSearchFilter (real)
#include "net-session-online/deps/platform_stand_ins.hpp"
#include "net-session-online/deps/stream_bytes.hpp"
#include "net-transport/connection_manager.hpp"  // CConnectionManager (real)

// 0x8094fc _ZN14CMatchingLocal9AddServerEiR10CNetworkIdiR15CRoomAttributes
// Partial: the room filter always accepts (CRoomSearchFilter is a stand-in), and the transport
// name comes from the transport stand-in. The rest follows the original: an existing address is
// refreshed; a new one is added under the next key when the filter accepts it.
std::int32_t CMatchingLocal::AddServer(std::int32_t memberId, const CNetworkId& address,
                                       std::int32_t rank, const CRoomAttributes& attributes) {
    serversMutex_.Lock();
    for (auto& entry : servers_) {
        if (entry.second.address.IsEqual(address)) {
            entry.second.lastSeenMs = OnlineNowMs();
            entry.second.rank = rank;
            serversMutex_.Unlock();
            return 0;
        }
    }
    if (roomSearchFilter_.TestRoomAttributes(attributes)) {
        const std::int32_t now = OnlineNowMs();
        tMatchingPeer peer;
        peer.memberId = memberId;
        peer.address = address;
        peer.lastSeenMs = now;
        peer.name = GetTransportNameOf(address);
        peer.rank = rank;
        peer.timeOffset = now - serverPollTime_;
        peer.roomAttributes = attributes;
        const std::uint64_t key = static_cast<std::uint64_t>(static_cast<std::int64_t>(nextPeerKey_));
        ++nextPeerKey_;
        servers_[key] = peer;
        if (roomOperationPending_) {
            MatchingInternalEventQueue().AddEvent(8388622, nullptr, 0);  // 0x80000e
        }
    }
    serversMutex_.Unlock();
    return 0;
}

// 0x8072d8 _ZN14CMatchingLocal12RemoveServerER10CNetworkId
// The original walks the tree in key order and erases the first entry whose address matches.
void CMatchingLocal::RemoveServer(const CNetworkId& address) {
    serversMutex_.Lock();
    for (auto it = servers_.begin(); it != servers_.end(); ++it) {
        if (it->second.address.IsEqual(address)) {
            servers_.erase(it);
            break;
        }
    }
    serversMutex_.Unlock();
}

// 0x8073c4 _ZN14CMatchingLocal12PurgeServersEv
// Erases every server not seen for more than 20000 ms. The clock is read per entry, as in the original.
std::int32_t CMatchingLocal::PurgeServers() {
    serversMutex_.Lock();
    for (auto it = servers_.begin(); it != servers_.end();) {
        const std::uint32_t age = static_cast<std::uint32_t>(OnlineNowMs()) -
                                  static_cast<std::uint32_t>(it->second.lastSeenMs);
        if (age > 0x4E20) {
            it = servers_.erase(it);
        } else {
            ++it;
        }
    }
    serversMutex_.Unlock();
    return 0;
}

// 0x8097c4 _ZN14CMatchingLocal20ProcessServerMessageER10CNetworkIdR12NetBitStream
// The sender's id (from) is not used by the original body; the record carries the address.
void CMatchingLocal::ProcessServerMessage(const CNetworkId& /*from*/, NetBitStream& stream) {
    if (memberId_ < 0 || memberId_ != serverMemberId_) {
        RoomServerHeader header{};
        stream.ReadByteArray(&header, sizeof header);
        CRoomAttributes attributes;
        attributes.Unserialize(stream);
        if (CConnectionManager::GetInstance()->IsConnected(header.memberId) && header.memberCount > joinThreshold_) {
            CConnectionManager::GetInstance()->Disconnect(header.memberId, false);
            MatchingInternalEventQueue().AddEvent(8388621, nullptr, 0);  // 0x80000d
        }
        if (header.flag != 0) {
            RemoveServer(header.listenId);
        } else {
            AddServer(header.memberId, header.listenId, header.memberCount, attributes);
        }
    }
}

// 0x808538 _ZN14CMatchingLocal13GetMemberNameEi
// Partial: the transport names come from the stand-ins. The original returns the local name
// whole, and for a remote member the part of the transport name before the first '.'.
std::string CMatchingLocal::GetMemberName(std::int32_t memberId) const {
    if (memberId == memberId_) {
        return GetLocalTransportName();
    }
    const std::string full = GetTransportNameOf(CConnectionManager::GetInstance()->GetMemberNetworkId(memberId));
    const std::size_t dot = full.find('.');
    return dot == std::string::npos ? full : full.substr(0, dot);
}
