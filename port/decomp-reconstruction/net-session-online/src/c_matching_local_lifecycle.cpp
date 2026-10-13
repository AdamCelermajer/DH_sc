#include "net-session-online/c_matching_local.hpp"
#include "net-session-online/deps/matching_queue.hpp"

// 0x807c70 _ZN14CMatchingLocal10InitializeEi
// Partial: the base CMatching::Initialize(arg) runs first, as in the original, but it is itself partial (the
// CSignIn and packet-manager steps are not reproduced). The registration and the event are reproduced.
// Nothing in this port sets initialized_ (+0x0c) to true yet.
std::int32_t CMatchingLocal::Initialize(std::int32_t arg) {
    CMatching::Initialize(arg);
    if (!initialized_) {
        CConnectionManager::GetInstance()->RegisterPacketType(2, 2, &CMatchingLocal::sBroadcastPacketReceiverCallback);
        CConnectionManager::GetInstance()->RegisterPacketType(3, 1, &CMatchingLocal::sBroadcastPacketReceiverCallback);
        CConnectionManager::GetInstance()->RegisterPacketType(4, 1, &CMatchingLocal::sPacketReceiverCallback);
        Reset();
        MatchingInternalEventQueue().AddEvent(8388609, nullptr, 0);  // 0x800001
    }
    return 0;
}

// 0x80605c _ZN14CMatchingLocal9TerminateEv
// Partial: the base CMatching::Terminate() runs as in the original, but the base has no body in the port
// (0x7ff5a0 is partial), so it does nothing here.
std::int32_t CMatchingLocal::Terminate() {
    if (initialized_) {
        initialized_ = false;
        CMatching::Terminate();
        CConnectionManager::GetInstance()->UnregisterPacketType(2);
        CConnectionManager::GetInstance()->UnregisterPacketType(3);
        CConnectionManager::GetInstance()->UnregisterPacketType(4);
    }
    return 0;
}

// 0x806098 _ZN14CMatchingLocal9PingRoomsEv
std::int32_t CMatchingLocal::PingRooms() {
    MatchingInternalEventQueue().AddEvent(8388624, nullptr, 0);  // 0x800010
    return 0;
}
