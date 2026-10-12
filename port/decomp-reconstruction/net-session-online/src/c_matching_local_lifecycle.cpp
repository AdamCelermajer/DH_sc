#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"

// 0x807c70 _ZN14CMatchingLocal10InitializeEi
// Partial: the base CMatching::Initialize(this, arg) call is not in the port. The registration
// and the event are reproduced. Nothing in this port sets initialized_ (+0x0c) to true yet.
std::int32_t CMatchingLocal::Initialize(std::int32_t /*arg*/) {
    if (!initialized_) {
        GetConnectionMgr().RegisterPacketType(2, 2, &CMatchingLocal::sBroadcastPacketReceiverCallback);
        GetConnectionMgr().RegisterPacketType(3, 1, &CMatchingLocal::sBroadcastPacketReceiverCallback);
        GetConnectionMgr().RegisterPacketType(4, 1, &CMatchingLocal::sPacketReceiverCallback);
        Reset();
        MatchingInternalEventQueue().AddEvent(8388609, nullptr, 0);  // 0x800001
    }
    return 0;
}

// 0x80605c _ZN14CMatchingLocal9TerminateEv
// Partial: the base CMatching::Terminate(this) call is not in the port.
std::int32_t CMatchingLocal::Terminate() {
    if (initialized_) {
        initialized_ = false;
        GetConnectionMgr().UnregisterPacketType(2);
        GetConnectionMgr().UnregisterPacketType(3);
        GetConnectionMgr().UnregisterPacketType(4);
    }
    return 0;
}

// 0x806098 _ZN14CMatchingLocal9PingRoomsEv
std::int32_t CMatchingLocal::PingRooms() {
    MatchingInternalEventQueue().AddEvent(8388624, nullptr, 0);  // 0x800010
    return 0;
}
