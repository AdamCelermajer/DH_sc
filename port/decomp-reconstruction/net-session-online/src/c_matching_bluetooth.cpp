#include "net-session-online/c_matching_bluetooth.hpp"

// 0x801b60 _ZN18CMatchingBluetooth16GetTransportTypeE14tPROTOCOL_TYPE
// The original takes the object pointer in r0 and never reads it, so this is static.
std::int32_t CMatchingBluetooth::GetTransportType(std::uint32_t protocolType) {
    static const std::int32_t kTransportTypes[3] = {3, 4, 4};  // CSWTCH.653
    if (protocolType > 2) {
        return 0;
    }
    return kTransportTypes[protocolType];
}
