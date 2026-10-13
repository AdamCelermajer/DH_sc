// Test support for the real CConnectionManager (net-transport/connection_manager.hpp). The binary
// initializes it from the matching layer (CMatching::Initialize reaches CConnectionManager::Initialize
// through CSignIn); a test binary does it here. The send paths record into SentPacketLog()
// (net-transport/deps/connection_env.hpp), because the transport stand-ins send nothing.
#pragma once

#include <cstdint>

#include "net-session-online/deps/matching.hpp"
#include "net-transport/connection.hpp"
#include "net-transport/connection_manager.hpp"
#include "net-transport/deps/connection_env.hpp"

namespace connection_manager_test {

// The matching object used when a test has not installed one: InitializeInternal reads the transport
// types through CMatching::Get().
class NullMatching : public CMatching {
public:
    std::uint32_t MemberBit(int memberId) const override {
        return (memberId >= 0 && memberId < 32) ? (1u << memberId) : 0u;
    }
    int LocalMemberId() const override { return 0; }
    int ServerMemberId() const override { return 0; }
    bool IsServer() const override { return false; }
    std::uint32_t GetRoomMask() const override { return 0; }
};

// Initializes the connection manager once (the binary's CMatching::Initialize does this).
inline void Initialize() {
    if (CMatching::s_Matching == nullptr) {
        static NullMatching fallback;
        CMatching::Set(&fallback);
    }
    if (!CConnectionManager::IsInitialized()) {
        CConnectionManager::Initialize();
    }
}

// A connected peer for a member id: Connect, then the handshake result (state 4, kStateConnected), which
// the test sets through the CConnection test hook. IsConnected needs that state.
inline int ConnectPeer(int memberId) {
    CNetworkId peer;
    const int result = CConnectionManager::GetInstance()->Connect(memberId, peer);
    if (CConnection* conn = CConnectionManager::GetInstance()->GetConnectionByMemberId(memberId)) {
        conn->SetState(4);
    }
    return result;
}

// Whether a connection was requested for the member (Connect ran); it may not be established yet.
inline bool ConnectionRequested(int memberId) {
    return CConnectionManager::GetInstance()->GetConnectionByMemberId(memberId) != nullptr;
}

// Replaces ResetForTest: connections, packet registrations and the send log start empty.
inline void Reset() {
    CConnectionManager::Terminate();
    // The packet registrations live in the static table (s_recvCBMap), which Terminate does not clear.
    for (int type = 0; type < 10; ++type) {
        CConnectionManager::UnregisterPacketType(type);
    }
    SentPacketLog().clear();
    Initialize();
}

}  // namespace connection_manager_test
