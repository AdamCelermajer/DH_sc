#include "platform_wifi_alive.hpp"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2ipdef.h>
#include <windows.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <ipifcons.h>

namespace dh2::platform {

bool source_wifi_alive(bool& alive, std::string& error) {
    alive = false;
    error.clear();

    // Select the interface Windows would use for an ordinary external route,
    // then compare its media type. This preserves the original Wi-Fi-only
    // distinction when Ethernet/VPN and Wi-Fi are both present.
    SOCKADDR_INET destination{};
    destination.Ipv4.sin_family = AF_INET;
    destination.Ipv4.sin_addr.s_addr = htonl(0x01010101u); // 1.1.1.1; route lookup only.
    MIB_IPFORWARD_ROW2 route{};
    SOCKADDR_INET source{};
    const NETIO_STATUS route_status = GetBestRoute2(
        nullptr, 0, nullptr, &destination, 0, &route, &source);
    if (route_status == ERROR_NOT_FOUND || route_status == ERROR_NETWORK_UNREACHABLE) {
        return true; // No active route is the source-equivalent no-active-network case.
    }
    if (route_status != NO_ERROR) {
        error = "GetBestRoute2 failed with status " + std::to_string(route_status);
        return false;
    }

    MIB_IF_ROW2 interface_row{};
    interface_row.InterfaceLuid = route.InterfaceLuid;
    const NETIO_STATUS interface_status = GetIfEntry2(&interface_row);
    if (interface_status != NO_ERROR) {
        error = "GetIfEntry2 failed with status " + std::to_string(interface_status);
        return false;
    }
    alive = interface_row.Type == IF_TYPE_IEEE80211;
    return true;
}

} // namespace dh2::platform
