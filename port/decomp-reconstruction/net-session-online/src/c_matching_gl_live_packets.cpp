#include "net-session-online/c_matching_gl_live.hpp"
#include "net-session-online/deps/net_bit_stream.hpp"

// 0x821188 _ZN15CMatchingGLLive31BroadcastPacketReceiverCallbackER10CNetworkIdPci
// The first byte of the packet selects the handler: 1 is a server message and 2 is a client
// message. Partial through the callee: ProcessServerMessage (0x821110) is partial (it cannot unserialize the
// room attributes yet), so a server message is read by a handler that adds nothing.
void CMatchingGLLive::BroadcastPacketReceiverCallback(CNetworkId& from, char* data, int size) {
    NetBitStream stream(static_cast<std::uint32_t>(size));
    stream.SetBuffer(data, static_cast<std::uint32_t>(size));
    std::uint8_t messageType = 0;
    stream.ReadByteArray(&messageType, 1);
    if (messageType == 1) {
        ProcessServerMessage(from, stream);
    } else if (messageType == 2) {
        ProcessClientMessage(from, stream);
    }
}
