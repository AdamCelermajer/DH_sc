// Stand-ins for engine services used by the session layer. Each one names the
// original symbol it replaces.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>

#include "net-transport/deps/byte_array.hpp"  // CustomFree
#include "net-transport/net_network_id.hpp"

// _Znaj15MemoryHintState: operator new[] with an allocator hint.
inline void* NewArrayWithHint(std::size_t bytes, int /*hint*/) { return std::malloc(bytes); }

// _Z10CustomFreePv: defined once, in net-transport/deps/byte_array.hpp (included here).

// _Z9GetOnlinev()->+0x28: the online clock in milliseconds. The owning
// COnline object lives in another category; tests drive the clock directly.
inline std::int32_t& OnlineClockStorage() {
    static std::int32_t clockMs = 0;
    return clockMs;
}
inline std::int32_t OnlineNowMs() { return OnlineClockStorage(); }

// CTransportManager::GetListenNetworkId(out, GetTransportMgr(), 0) (stand-in: an empty id).
inline CNetworkId GetListenNetworkId() { return CNetworkId{}; }

// The transport name of a peer, as the transport manager reports it (stand-in: a table that tests can
// fill). The original splits the name at the first '.'.
inline std::map<std::uint64_t, std::string>& TransportNamesForTest() {
    static std::map<std::uint64_t, std::string> names;
    return names;
}

// The key of a peer in the name table: address0 (16-bit low part, 32-bit high part).
inline std::uint64_t TransportKeyOf(const CNetworkId& id) {
    std::uint16_t lo = 0;
    std::uint32_t hi = 0;
    if (!id.GetAddress0(lo, hi)) {
        return 0;
    }
    return static_cast<std::uint64_t>(lo) | (static_cast<std::uint64_t>(hi) << 32);
}

// Transport name for a peer address (stand-in: looks up the table by TransportKeyOf).
inline std::string GetTransportNameOf(const CNetworkId& id) {
    const std::uint64_t key = TransportKeyOf(id);
    auto it = TransportNamesForTest().find(key);
    return it == TransportNamesForTest().end() ? std::string() : it->second;
}

// Transport name of this device (stand-in: empty).
inline std::string GetLocalTransportName() { return std::string(); }
