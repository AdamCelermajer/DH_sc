#include "net-session-online/glx_player_ping.hpp"

#include <cstring>

#include "net-session-online/deps/glx_stand_ins.hpp"

namespace {

// The original's STRNEW: a copy of the text, or null for a null argument.
std::unique_ptr<char[]> CopyText(const char* text) {
    if (text == nullptr) {
        return nullptr;
    }
    const std::size_t length = std::strlen(text);
    std::unique_ptr<char[]> copy(new char[length + 1]);
    std::memcpy(copy.get(), text, length + 1);
    return copy;
}

// Stand-in for the BSD in_cksum over a byte buffer: the ones-complement sum of 16-bit words
// in host order. It is not in the queue, so it is not ledgered.
std::uint16_t InCksum(const unsigned char* data, std::size_t length) {
    std::uint32_t sum = 0;
    while (length > 1) {
        std::uint16_t word;
        std::memcpy(&word, data, sizeof(word));
        sum += word;
        data += 2;
        length -= 2;
    }
    if (length == 1) {
        sum += *data;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return static_cast<std::uint16_t>(~sum & 0xffff);
}

}  // namespace

// 0x840564 and 0x840600 _ZN13GLXPlayerPingC1EPKci
// Partial: the socket comes from the factory, which builds the base socket class in the
// port (the original builds CAndroidSocket).
GLXPlayerPing::GLXPlayerPing(const char* host, std::int32_t port)
    : host_(CopyText(host)), port_(port) {
    socket_.reset(GLXPlayerSocketFactory::GetSocket(host, port, nullptr));
}

// 0x8400fc _ZN13GLXPlayerPingD1Ev, 0x8401cc _ZN13GLXPlayerPingD2Ev
// The original frees the host, the address, the two timeouts and the socket. The members
// do that here.
GLXPlayerPing::~GLXPlayerPing() = default;

// 0x84008c _ZN13GLXPlayerPing4InitEv
std::int32_t GLXPlayerPing::Init() {
    if (host_ != nullptr && port_ >= 0) {
        return socket_->Connect();
    }
    return 0;
}

// 0x8400c8 _ZN13GLXPlayerPing7PrepareEv
std::int32_t GLXPlayerPing::Prepare() {
    const std::int32_t result = Init();
    if (result != 0) {
        state_ = 2;
        return 0;
    }
    return -1;
}

// 0x840280 _ZN13GLXPlayerPing16WaitForEchoReplyEi
// The original calls select() on the socket handle with a zero timeout. Partial: no
// select stand-in exists, so the port reports that no reply has arrived.
std::int32_t GLXPlayerPing::WaitForEchoReply(std::int32_t fd) {
    (void)fd;
    return 0;
}

// 0x840454 _ZN13GLXPlayerPing15SendEchoRequestEv
// The request and the identifier and sequence statics are shared by every probe, as in the
// original. Fields are written in host byte order, as the original does. The timestamp is
// XP_API_GET_TIME, which the stand-in returns as 0.
std::int32_t GLXPlayerPing::SendEchoRequest() {
    static bool idCaptured = false;
    static std::uint16_t echoId = 0;
    static std::uint16_t echoSeq = 0;
    static unsigned char echoRequest[48] = {};

    if (!idCaptured) {
        echoId = echoId_;
        idCaptured = true;
    }
    const std::uint16_t seq = echoSeq;
    ++echoSeq;

    echoRequest[0] = 8;   // ICMP echo request
    echoRequest[1] = 0;
    std::memset(&echoRequest[2], 0, 2);
    std::memcpy(&echoRequest[4], &echoId, sizeof(echoId));
    std::memcpy(&echoRequest[6], &seq, sizeof(seq));
    const std::uint32_t stamp = XP_API_GET_TIME();
    std::memcpy(&echoRequest[12], &stamp, sizeof(stamp));
    for (int i = 0; i < 32; ++i) {
        echoRequest[16 + i] = static_cast<unsigned char>(32 + i);
    }
    const std::uint16_t checksum = InCksum(echoRequest, sizeof(echoRequest));
    std::memcpy(&echoRequest[2], &checksum, sizeof(checksum));

    const std::int32_t result =
        socket_->SendTo(echoRequest, sizeof(echoRequest), address_.get(), port_);
    if (result < 0) {
        result_ = -1;
    }
    return result;
}
