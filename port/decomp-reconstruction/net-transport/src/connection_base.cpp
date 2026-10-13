#include "net-transport/connection_base.hpp"

#include <cstring>
#include <string>
#include <utility>

#include "net-transport/default_data_packet.hpp"
#include "net-transport/deps/connection_env.hpp"
#include "net-transport/deps/data_packet_payload.hpp"

namespace {

// A fresh message packet, as the original's operator new(0x101C) + DefaultDataPacket ctor.
std::unique_ptr<DefaultDataPacket> NewPacket() {
    return std::make_unique<DefaultDataPacket>();
}

// The length byte of a string field: the original's XP_API_STRLEN, cast to a byte.
unsigned char ByteLength(const char* text) {
    return static_cast<unsigned char>(platform_calls::StrLen(text));
}

}  // namespace

// 0x846ca4 _ZN10ConnectionC1EPci
// 0x846dac _ZN10ConnectionC2EPci is the same body (skipped at that address).
// The host is copied (a null host is stored as an empty string) and the socket is created for it.
Connection::Connection(char* host, int port)
    : host_(host ? host : ""), port_(port) {
    // The factory's observer argument is 0 in the original, which is nullptr here.
    socket_.reset(GLXPlayerSocketFactory::GetSocket(host_.c_str(), port_, nullptr));
}

// 0x846b14 _ZN10ConnectionD1Ev
// 0x846bcc _ZN10ConnectionD2Ev is the same body (skipped at that address); 0x846bb0
// _ZN10ConnectionD0Ev is the deleting variant (skipped).
// Partial: the original also frees an owned object at +0x2018 whose type is not recovered, so it
// is not freed here. The queues, retry copy, socket and name are released by the members.
Connection::~Connection() {
    disconnect();
}

// 0x84699c _ZN10Connection8sendDataEv
// Partial: the two debug prints (select error and "PPP not open") are dropped. The body bytes
// come from the DataPacket stand-in (deps/data_packet_payload.hpp), so an empty body is sent until
// DataPacket declares its body accessors. The queue, the send and the resend steps are as in the
// original.
void Connection::sendData() {
    if (outgoing_.empty()) {
        return;
    }
    if (!isConnected() || !socket_) {
        disconnect();
        return;
    }
    DataPacket& packet = *outgoing_.front();
    const std::size_t length = PayloadLength(packet);
    send_buffer_[0] = static_cast<std::uint8_t>(length);
    if (length != 0) {
        std::memcpy(send_buffer_ + 1, PayloadBody(packet), length);
    }
    if (socket_->Poll(true) < 0) {  // vtable +0x40, write readiness
        return;
    }
    if (socket_->SendBytes(reinterpret_cast<const char*>(send_buffer_), static_cast<int>(length) + 1) < 0) {  // +0x44

        disconnect();
        return;
    }
    last_send_time_ = platform_calls::GetTime();
    outgoing_.pop_front();  // the sent packet is destroyed
    if (!outgoing_.empty()) {
        sendData();
    }
}

// 0x8466fc _ZN10Connection14receiveDataLenEv
// Partial: the debug prints of the error paths are dropped. The receive is a two-step read:
// step 0 reads the one-byte body length, step 1 reads the body (as the original). A packet is
// queued when the body is complete. Also a guard for a missing socket (the original dereferences
// it): the call returns without reading.
void Connection::receiveDataLen() {
    if (recv_step_ != 0 && recv_step_ != 1) {
        return;  // "receiveDataLen-- error m_iRecvStep" (debug print dropped)
    }
    if (!socket_) {
        return;
    }
    const bool body_step = recv_step_ == 1;
    if (!body_step) {
        need_recv_data_len_ = 1;
    }
    if (recv_buf_len_ == 0) {
        std::memset(recv_buffer_, 0, kBufferBytes);
    }
    if (socket_->Poll(false) < 0) {  // vtable +0x40, read readiness
        return;
    }
    if (!socket_->HasPendingData()) {  // vtable +0x4c
        return;
    }
    const int result = socket_->RecvBytes(reinterpret_cast<char*>(recv_buffer_ + recv_buf_len_),  // vtable +0x48
                                          need_recv_data_len_ - recv_buf_len_);
    if (result < 0) {
        return;
    }
    if (result == 0) {
        disconnect();  // the peer closed the link
        return;
    }
    if (recv_buf_len_ < need_recv_data_len_) {
        recv_buf_len_ += result;
    }
    if (!body_step) {
        if (need_recv_data_len_ == recv_buf_len_) {
            // The length byte is in. The body length is that byte.
            recv_step_ = 1;
            need_recv_data_len_ = recv_buffer_[0];
            recv_buf_len_ = 0;
        } else if (need_recv_data_len_ < recv_buf_len_) {
            disconnect();  // "error,more data" (debug print dropped)
        }
        return;
    }
    if (need_recv_data_len_ == recv_buf_len_) {
        auto packet = NewPacket();
        packet->setMessageBody(recv_buffer_, recv_buf_len_);
        addIncomingPacket(packet.release());
        std::memset(recv_buffer_, 0, 0x80);
        need_recv_data_len_ = 0;
        recv_buf_len_ = 0;
        recv_step_ = 0;
    } else if (need_recv_data_len_ < recv_buf_len_) {
        disconnect();  // "more data" (debug print dropped)
    }
}

// 0x8456fc _ZN10Connection21getNextIncomingPacketEv
// Pops the first incoming packet. The caller owns the returned packet; nullptr when none.
DataPacket* Connection::getNextIncomingPacket() {
    if (incoming_.empty()) {
        return nullptr;
    }
    std::unique_ptr<DataPacket> packet = std::move(incoming_.front());
    incoming_.pop_front();
    return packet.release();
}

// 0x845754 _ZN10Connection11isConnectedEv
bool Connection::isConnected() const {
    return connected_;
}

// 0x84575c _ZN10Connection13set_connectedEb
void Connection::set_connected(bool connected) {
    connected_ = connected;
}

// 0x845764 _ZN10Connection14cleanRetryDataEv
void Connection::cleanRetryData() {
    retry_packet_.reset();
}

// 0x845798 _ZN10Connection19getTimeOfLastReqestEv
int Connection::getTimeOfLastReqest() const {
    return last_request_time_;
}

// 0x8457a4 _ZN10Connection13saveRetryDataEP10DataPacket
// Partial: the copy of the body comes from the DataPacket stand-in (see sendData), so the retry
// copy has an empty body until DataPacket declares its body accessors.
void Connection::saveRetryData(DataPacket* packet) {
    if (!packet) {
        return;
    }
    cleanRetryData();
    auto copy = NewPacket();
    copy->setMessageBody(PayloadBody(*packet), static_cast<int>(PayloadLength(*packet)));
    retry_packet_ = std::move(copy);
    last_request_time_ = platform_calls::GetTime();
}

// 0x84581c _ZN10Connection7connectEv
// Partial: a guard is added for a missing socket (the original dereferences it after a
// disconnect). The connect attempt count lives in the socket (+0x85c, ConnectAttempts).
int Connection::connect() {
    if (state_ == 1) {
        if (!socket_) {
            state_ = 3;
            return 0;
        }
        const int result = socket_->Connect(host_.c_str(), 0);  // vtable +0x38
        if (result) {
            connected_ = true;
            sendEstablishConnectionPackage();
            receiveDataLen();
            last_send_time_ = platform_calls::GetTime();
            state_ = 2;
            socket_->SetConnectAttempts(0);
            return 1;
        }
        if (socket_->ConnectAttempts() >= 2) {
            state_ = 3;
            socket_->SetConnectAttempts(0);
        }
        return 0;
    }
    if (state_ != 0) {
        return 0;
    }
    if (!host_.empty() && port_ != 0 && socket_) {
        state_ = socket_->BeginConnect() ? 1 : 3;  // vtable +0x28
        return 0;
    }
    state_ = 3;
    return 0;
}

// 0x845944 _ZN10Connection17addIncomingPacketEP10DataPacket
// Partial: the original's error branch (a head without a tail, debug print) cannot occur with the
// queue, so it is not reproduced. The packet is taken by the queue.
int Connection::addIncomingPacket(DataPacket* packet) {
    if (packet) {
        incoming_.emplace_back(packet);
    }
    return 0;
}

// 0x8459b4 _ZN10Connection17addOutgoingPacketEP10DataPacket
// Partial: the return value of the original is the result of sendData (not reproduced, 0 is
// returned). The packet is taken by the queue; when the link is down it is destroyed.
// Partial too in the same error branch as addIncomingPacket (debug print dropped).
int Connection::addOutgoingPacket(DataPacket* packet) {
    if (!packet) {
        return 0;
    }
    std::unique_ptr<DataPacket> owned(packet);
    if (!connected_) {
        return 0;  // owned is destroyed (the original calls the packet's deleting destructor)
    }
    const bool was_empty = outgoing_.empty();
    outgoing_.push_back(std::move(owned));
    if (was_empty) {
        sendData();
    }
    return 0;
}

// 0x845a50 _ZN10Connection13sendRetryDataEv
// Partial: the body copy comes from the DataPacket stand-in (see sendData).
void Connection::sendRetryData() {
    if (!retry_packet_) {
        return;
    }
    auto copy = NewPacket();
    copy->setMessageBody(PayloadBody(*retry_packet_), static_cast<int>(PayloadLength(*retry_packet_)));
    addOutgoingPacket(copy.release());
    last_request_time_ = platform_calls::GetTime();
}

// 0x845ad8 _ZN10Connection19keepConnectionAliveEv
// Sends a keep-alive ("ga") when the last send is more than 10 seconds old.
bool Connection::keepConnectionAlive() {
    if (platform_calls::GetTime() - last_send_time_ <= 10000) {
        return false;
    }
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('a');
    packet->packMessage();
    addOutgoingPacket(packet.release());
    return true;
}

// 0x845b60 _ZN10Connection24sendKickOutPlayerPackageEPc
// Partial: the original returns `this` on the null-name path; 0 is returned here.
int Connection::sendKickOutPlayerPackage(char* name) {
    if (!name) {
        return 0;
    }
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('k');
    packet->addByteLenString(name, ByteLength(name));
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x845c0c _ZN10Connection24sendUpdateSessionPackageEi
int Connection::sendUpdateSessionPackage(int state) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('e');
    packet->addByte(static_cast<unsigned char>(state));
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x845ca4 _ZN10Connection21sendFinishGamePackageEv
int Connection::sendFinishGamePackage() {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('f');
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x845d24 _ZN10Connection20sendStartGamePackageEv
int Connection::sendStartGamePackage() {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('s');
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x845da4 _ZN10Connection29sendListSessionsByDataPackageEhiPci
int Connection::sendListSessionsByDataPackage(unsigned char type, int a3, char* data, int length) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('l');
    packet->addByte(type);
    packet->addInt(a3);
    packet->addByte('d');
    packet->addByteLenString(data, static_cast<unsigned char>(length));
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x845e88 _ZN10Connection29sendListSessionsByNamePackageEhiPc
int Connection::sendListSessionsByNamePackage(unsigned char type, int a3, char* name) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('l');
    packet->addByte(type);
    packet->addInt(a3);
    packet->addByte('n');
    packet->addByteLenString(name, ByteLength(name));
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x845f70 _ZN10Connection26sendListSessionsAllPackageEhi
int Connection::sendListSessionsAllPackage(unsigned char type, int a3) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('l');
    packet->addByte(type);
    packet->addInt(a3);
    packet->addByte('a');
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x846034 _ZN10Connection23sendListSessionsPackageEhi
int Connection::sendListSessionsPackage(unsigned char type, int a3) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('l');
    packet->addByte(type);
    packet->addInt(a3);
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x8460e4 _ZN10Connection23sendLeaveSessionPackageEv
int Connection::sendLeaveSessionPackage() {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('q');
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x846164 _ZN10Connection22sendJoinSessionPackageEPc
int Connection::sendJoinSessionPackage(char* name) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('j');
    packet->addByteLenString(name, ByteLength(name));
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x846208 _ZN10Connection20sendQuickGamePackageEv
int Connection::sendQuickGamePackage() {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('u');
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x846288 _ZN10Connection24sendCreateSessionPackageEPcS0_i
// Partial: a null password is sent as the original's one-space string, but with the original's
// byte count, which reads past that literal. The port sends that many spaces instead.
int Connection::sendCreateSessionPackage(char* name, char* password, int max_players) {
    const auto count = static_cast<unsigned char>(max_players);
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('c');
    packet->addByteLenString(name, ByteLength(name));
    if (password) {
        packet->addByteLenString(password, count);
    } else {
        const std::string spaces(count, ' ');
        packet->addByteLenString(spaces.c_str(), count);
    }
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x846364 _ZN10Connection27sendFinishConnectionPackageEv
// Queued without a retry copy. Sets the last request time to now and returns it.
int Connection::sendFinishConnectionPackage() {
    auto packet = NewPacket();
    packet->addByte('s');
    packet->addByte('x');
    packet->packMessage();
    addOutgoingPacket(packet.release());
    last_request_time_ = platform_calls::GetTime();
    return last_request_time_;
}

// 0x8463d0 _ZN10Connection16sendLoginPackageEPc
// Partial: the debug print of the user name ("sendLoginPackage") is dropped.
int Connection::sendLoginPackage(char* user) {
    auto packet = NewPacket();
    packet->addByte('g');
    packet->addByte('r');
    packet->addByte('i');
    packet->addByteLenString(user, ByteLength(user));
    packet->packMessage();
    saveRetryData(packet.get());
    return addOutgoingPacket(packet.release());
}

// 0x846488 _ZN10Connection30sendEstablishConnectionPackageEv
// Partial: the debug print of the random value is dropped. Two handshake packets ("sw" and "sr",
// each with the random value) are queued without a retry copy. The random value is the platform
// stand-in (the original passes the connection name, which the stand-in ignores).
int Connection::sendEstablishConnectionPackage() {
    need_recv_data_len_ = 0;
    recv_buf_len_ = 0;
    recv_step_ = 0;
    const int random = platform_calls::GetRandom();
    auto first = NewPacket();
    first->addByte('s');
    first->addByte('w');
    first->addInt(random);
    first->packMessage();
    addOutgoingPacket(first.release());
    auto second = NewPacket();
    second->addByte('s');
    second->addByte('r');
    second->addInt(random);
    second->packMessage();
    addOutgoingPacket(second.release());
    last_request_time_ = platform_calls::GetTime();
    handshake_time_ = platform_calls::GetTime();
    return handshake_time_;
}

// 0x8465b8 _ZN10Connection10disconnectEv
// Closes the socket and drops it, clears the retry copy and both queues (their packets are
// destroyed), and zeroes both buffers and the timers. The state and the receive step are kept, as
// in the original.
void Connection::disconnect() {
    connected_ = false;
    if (socket_) {
        socket_->OnCancelled();  // vtable +0x3c, the close hook (Connection::disconnect calls it, then deletes)
        socket_.reset();
    }
    cleanRetryData();
    incoming_.clear();
    outgoing_.clear();
    std::memset(recv_buffer_, 0, kBufferBytes);
    std::memset(send_buffer_, 0, kBufferBytes);
    handshake_time_ = 0;
    last_send_time_ = 0;
    last_request_time_ = 0;
}

// 0x846c68 _ZN10Connection7setNameEPc
// A null name is stored as an empty string.
void Connection::setName(char* name) {
    name_ = name ? name : "";
}
