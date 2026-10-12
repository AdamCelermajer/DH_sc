// Track B batch e1-net-session-online-GLXPlayerPing-0: RecvEchoReply and Kernel of the ICMP
// echo probe. The rest of the class is in src/glx_player_ping.cpp.
#include <chrono>
#include <cstring>
#include <thread>

#include "net-session-online/deps/glx_stand_ins.hpp"
#include "net-session-online/deps/xp_api.hpp"
#include "net-session-online/glx_player_ping.hpp"
#include "net-session-online/xp_api.hpp"

namespace {

// The ICMP receive hook of the probe socket (GLXPlayerSocket vtable +0x5c, called with a
// 324-byte buffer). The port's GLXPlayerSocket declares no such hook, and the socket header
// is owned by another batch, so this stand-in reports -1, the failure the base socket gives
// for its other probe hooks. Layout request: GLXPlayerSocket needs
// `virtual std::int32_t RecvFrom(void* buffer, std::int32_t length)` at vtable +0x5c.
std::int32_t ReceiveIcmpPacket(GLXPlayerSocket* socket, std::uint8_t* buffer, std::int32_t length) {
    (void)socket;
    (void)buffer;
    (void)length;
    return -1;
}

}  // namespace

// 0x8402f8 GLXPlayerPing::RecvEchoReply()
// Partial: the receive step is the stand-in above, so the port always takes the failure
// branch. The parsing below follows the original's byte offsets in the 324-byte packet:
// +0x08 TTL, +0x0c source address, +0x14 ICMP type, +0x15 ICMP code, +0x18 ICMP identifier,
// +0x1a ICMP sequence and +0x20 the echoed send timestamp (32-bit, host order as written by
// SendEchoRequest).
std::int32_t GLXPlayerPing::RecvEchoReply() {
    std::uint8_t packet[324];
    XP_API_MEMSET(packet, 0, sizeof(packet));
    const std::int32_t count = ReceiveIcmpPacket(socket_.get(), packet, sizeof(packet));
    if (count < 0) {
        result_ = -1;
        return -1;
    }

    std::uint16_t replyId = 0;
    std::uint16_t replySeq = 0;
    std::uint32_t sentStamp = 0;
    std::memcpy(&replyId, &packet[24], sizeof(replyId));
    std::memcpy(&replySeq, &packet[26], sizeof(replySeq));
    std::memcpy(&sentStamp, &packet[32], sizeof(sentStamp));
    const std::uint8_t icmpType = packet[20];
    const std::uint8_t icmpCode = packet[21];

    if (replyId != echoId_ || icmpType != 0) {
        // The original passes in_addr to inet_ntoa; the packed form (low byte first) is the
        // in_addr memory order of the four source bytes, so XP_API_IP_INT2STR prints the same text.
        const std::uint32_t source = static_cast<std::uint32_t>(packet[12]) |
                                     (static_cast<std::uint32_t>(packet[13]) << 8) |
                                     (static_cast<std::uint32_t>(packet[14]) << 16) |
                                     (static_cast<std::uint32_t>(packet[15]) << 24);
        char sourceText[32] = {};
        XP_API_IP_INT2STR(source, sourceText);
        XP_DEBUG_OUT("recv icmp packet from %s type=%d code=%d\n", sourceText,
                     static_cast<int>(icmpType), static_cast<int>(icmpCode));
        return -2;
    }

    const std::int32_t elapsed = static_cast<std::int32_t>(
        XP_API_GET_TIME() - sentStamp);
    result_ = elapsed;
    if (elapsed <= 0) {
        XP_DEBUG_OUT("\nReply from: %s: bytes=%d seq:%d time<1ms\n", address_.get(), 32,
                     static_cast<int>(replySeq));
    } else {
        XP_DEBUG_OUT("\nReply from: %s: bytes=%d seq:%d TTL=%d time=%dms\n", address_.get(), 32,
                     static_cast<int>(replySeq), static_cast<int>(packet[8]), elapsed);
    }
    return count;
}

// 0x84069c GLXPlayerPing::Kernel()
// The probe state machine, run once per call: state 2 resolves the host, 3 sends an echo
// request, 4 waits for the reply, 5 holds the result for the receive timeout. Every path ends
// with a 1 ms sleep (usleep(0x3e8) in the original) and returns 0.
// Partial, two gaps:
//   - State 2: the original calls the socket's vtable +0x20 with the host, which returns a
//     hostent*. The port's OpenConnection returns an int handle, so the address copy cannot be
//     reproduced and the state stays 2. Layout request: a host-resolve hook returning a
//     hostent-shaped result on GLXPlayerSocket.
//   - State 4: the original passes the socket handle (GLXPlayerSocket +0x08) to
//     WaitForEchoReply. The port's socket has no handle getter, so -1 is passed; the port's
//     WaitForEchoReply ignores the value.
// The timeouts use the original's two-int pair: tv_sec holds the start time and tv_usec the
// timeout in the same unit as XP_API_GET_TIME.
std::int32_t GLXPlayerPing::Kernel() {
    // LABEL_2 in the original: a failed send or wait. Result -1, receive timer starts, state 5.
    auto failRound = [this]() {
        result_ = -1;
        receiveTimeout_.tv_sec = static_cast<std::int32_t>(XP_API_GET_TIME());
        state_ = 5;
    };
    // LABEL_8 in the original: send one echo request, start the send timer, state 4.
    auto startRound = [this, &failRound]() {
        if (SendEchoRequest() < 0) {
            failRound();
            return;
        }
        state_ = 4;
        sendTimeout_.tv_sec = static_cast<std::int32_t>(XP_API_GET_TIME());
    };

    switch (state_) {
    case 2:
        // The host resolve is partial (see above). Without an address the original leaves the
        // state at 2 when the resolve fails, which is the path taken here.
        break;
    case 3:
        startRound();
        break;
    case 4: {
        const std::int32_t waited = WaitForEchoReply(-1);
        if (waited < 0) {
            failRound();
        } else if (waited != 0) {
            if (RecvEchoReply() >= 0) {
                receiveTimeout_.tv_sec = static_cast<std::int32_t>(XP_API_GET_TIME());
                state_ = 5;
            }
        } else {
            const std::int32_t sinceSend = static_cast<std::int32_t>(
                XP_API_GET_TIME() - static_cast<std::uint32_t>(sendTimeout_.tv_sec));
            if (sinceSend >= sendTimeout_.tv_usec) {
                XP_DEBUG_OUT("TimeOut\n");
                receiveTimeout_.tv_sec = static_cast<std::int32_t>(XP_API_GET_TIME());
                result_ = -2;
                state_ = 3;
            }
        }
        break;
    }
    case 5: {
        const std::int32_t sinceReceive = static_cast<std::int32_t>(
            XP_API_GET_TIME() - static_cast<std::uint32_t>(receiveTimeout_.tv_sec));
        if (sinceReceive >= receiveTimeout_.tv_usec) {
            state_ = 3;
        }
        break;
    }
    default:
        break;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(0x3e8));
    return 0;
}
