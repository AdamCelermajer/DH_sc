// Stand-ins for engine services used by the session layer. Each one names the
// original symbol it replaces.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>

#include "core-platform/memory.hpp"            // _Z10CustomFreePv (CustomFree, out of line in core-platform/src/memory.cpp)
#include "dh2/global/runtime_support.hpp"      // _Znaj15MemoryHintState: operator new[](size, MemoryHintState)
#include "net-transport/net_network_id.hpp"

// _Z9GetOnlinev()->+0x28: the online clock in milliseconds. The owning
// COnline object lives in another category; tests drive the clock directly.
inline std::int32_t& OnlineClockStorage() {
    static std::int32_t clockMs = 0;
    return clockMs;
}
inline std::int32_t OnlineNowMs() { return OnlineClockStorage(); }

// The transport manager calls that used to be stand-ins here (the listen id and the host names) are the real
// CTransportManager::GetListenNetworkId, GetHostname and GetLocalhostname (net-transport/transport_manager.hpp).
