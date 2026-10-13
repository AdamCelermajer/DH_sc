// Stand-ins for the connection and transport services that the matching classes use
// (CConnectionManager and CTransportManager in net-transport, the online clock in the platform
// layer). Split out of matching_stand_ins.hpp so that a translation unit that uses the real
// CRoomAttributes (c_room_attributes.hpp) can still reach the connection manager: the other
// stand-ins in matching_stand_ins.hpp redefine CRoomAttributes and CRoomSearchFilter, which
// conflict with the real headers. matching_stand_ins.hpp includes this file, so existing users
// are unchanged. Each one names the original symbol it replaces. The fakes record what they are
// asked to do so the tests can check the callers. Nothing here is the real behaviour.
#pragma once

#include <cstdint>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "net-session-online/deps/net_bit_stream.hpp"
#include "net-session-online/deps/network_id.hpp"
#include "net-session-online/deps/platform_stand_ins.hpp"

// CConnectionManager (net-transport). Keeps the set of connected member ids and a log
// of the packets sent and the packet types registered.
class CConnectionManager {
public:
    struct SentPacket {
        int packetType;
        bool broadcast;
        std::vector<std::uint8_t> bytes;
    };

    // CConnectionManager::GetConnectionCount(bool) (stand-in: connected peers)
    int GetConnectionCount(bool /*includeLocal*/) const { return static_cast<int>(connected_.size()); }
    // CConnectionManager::IsConnected(int)
    bool IsConnected(int memberId) const { return connected_.count(memberId) != 0; }
    // CConnectionManager::Connect(int, CNetworkId&)
    void Connect(int memberId, const CNetworkId& /*address*/) { connected_.insert(memberId); }
    // CConnectionManager::Disconnect(int, bool)
    void Disconnect(int memberId, bool /*notify*/) { connected_.erase(memberId); }
    // CConnectionManager::DisconnectAll()
    void DisconnectAll() { connected_.clear(); }
    // CConnectionManager::GetConnMemberIdList(bool)
    std::vector<int> GetConnMemberIdList(bool /*includeLocal*/) const {
        return std::vector<int>(connected_.begin(), connected_.end());
    }
    // CConnectionManager::GetMemberNetworkId(int) (stand-in: an empty id)
    CNetworkId GetMemberNetworkId(int /*memberId*/) const { return CNetworkId{}; }
    // CConnectionManager::SendToAll(int type, buffer, size)
    int SendToAll(int packetType, const NetBitStream& stream) {
        sent_.push_back({packetType, false, {stream.Data(), stream.Data() + stream.Size()}});
        return 0;
    }
    // CConnectionManager::SendBroadcast(int type, buffer, size)
    int SendBroadcast(int packetType, const NetBitStream& stream) {
        sent_.push_back({packetType, true, {stream.Data(), stream.Data() + stream.Size()}});
        return 0;
    }
    // CConnectionManager::RegisterPacketType(int, int, callback), point-to-point form
    void RegisterPacketType(int packetType, int /*channel*/, void (*callback)(int, char*, int)) {
        pointCallbacks_[packetType] = callback;
    }
    // CConnectionManager::RegisterPacketType(int, int, callback), broadcast form
    void RegisterPacketType(int packetType, int /*channel*/, void (*callback)(CNetworkId&, char*, int)) {
        broadcastCallbacks_[packetType] = callback;
    }
    // CConnectionManager::UnregisterPacketType(int)
    void UnregisterPacketType(int packetType) {
        pointCallbacks_.erase(packetType);
        broadcastCallbacks_.erase(packetType);
    }

    // Test access.
    const std::vector<SentPacket>& Sent() const { return sent_; }
    void ClearSent() { sent_.clear(); }
    bool HasCallback(int packetType) const {
        return pointCallbacks_.count(packetType) != 0 || broadcastCallbacks_.count(packetType) != 0;
    }
    void ResetForTest() {
        connected_.clear();
        sent_.clear();
        pointCallbacks_.clear();
        broadcastCallbacks_.clear();
    }

private:
    std::set<int> connected_;
    std::vector<SentPacket> sent_;
    std::map<int, void (*)(int, char*, int)> pointCallbacks_;
    std::map<int, void (*)(CNetworkId&, char*, int)> broadcastCallbacks_;
};

// GetConnectionMgr() (net-transport)
inline CConnectionManager& GetConnectionMgr() {
    static CConnectionManager manager;
    return manager;
}

// CTransportManager::GetListenNetworkId(out, GetTransportMgr(), 0) (stand-in: an empty id)
inline CNetworkId GetListenNetworkId() { return CNetworkId{}; }

// The transport name of a peer, as the transport manager reports it (stand-in: a
// table that tests can fill). The original splits the name at the first '.'.
inline std::map<std::uint64_t, std::string>& TransportNamesForTest() {
    static std::map<std::uint64_t, std::string> names;
    return names;
}
// Transport name for a peer address (stand-in: looks up the first 8 bytes of the id).
inline std::string GetTransportNameOf(const CNetworkId& id) {
    std::uint64_t key = 0;
    std::memcpy(&key, id.Bytes(), sizeof key);
    auto it = TransportNamesForTest().find(key);
    return it == TransportNamesForTest().end() ? std::string() : it->second;
}
// Transport name of this device (stand-in: empty)
inline std::string GetLocalTransportName() { return std::string(); }
