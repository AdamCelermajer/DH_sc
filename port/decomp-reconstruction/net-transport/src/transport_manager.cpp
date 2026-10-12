// CTransportManager: transport table, receive pass, packet framing and the singleton.
// Reconstructed from libDungeonHunter2.so pseudocode.
#include "net-transport/transport_manager.hpp"

#include <cstring>
#include <new>

#include "net-transport/connection_manager.hpp"
#include "net-transport/deps/connection_env.hpp"
#include "net-transport/deps/transport_slots.hpp"
#include "net-transport/transport_factory.hpp"
#include "net-transport/transport_manager_impl.hpp"

namespace {
// Packet header (8 bytes): magic u32 at +0, total length u16 at +4 (header included),
// packet type byte at +6 (bit 7 must be clear), one byte pad at +7, payload from +8.
constexpr std::size_t kHeaderBytes = 8;
constexpr std::size_t kMaxPayload = 0xFF8;
// Port and address the broadcast ids use (6776 = 0x1A78, address word -1).
constexpr std::uint16_t kBroadcastPort = 0x1A78;
constexpr std::uint32_t kBroadcastHi = 0xFFFFFFFFu;
// clock() ticks in one second as the original uses them (1000000 per second).
constexpr std::int64_t kWindowTicksPerSecond = 1000000;

std::uint16_t ReadU16(const char* p) {
    return static_cast<std::uint16_t>(static_cast<std::uint8_t>(p[0]) |
                                      (static_cast<std::uint8_t>(p[1]) << 8));
}

std::uint32_t ReadU32(const char* p) {
    return static_cast<std::uint32_t>(static_cast<std::uint8_t>(p[0])) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(p[1])) << 8) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(p[2])) << 16) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(p[3])) << 24);
}
}  // namespace

CTransportManager* CTransportManager::s_transportManager = nullptr;

// 0x81b758 _ZN17CTransportManagerC1Ev
// 0x81b7ac _ZN17CTransportManagerC2Ev
// Both variants have the same body: initialized and terminating cleared, timestamp zero.
CTransportManager::CTransportManager() = default;

// 0x81b6d4 _ZN17CTransportManagerD2Ev
// 0x81b724 _ZN17CTransportManagerD1Ev
// The original resets the vtable and destroys the mutex. The table entries are not freed
// here (the transports are owned by their own users, as in the original).
CTransportManager::~CTransportManager() = default;

// 0x81a9dc _ZN17CTransportManager12AbortNetworkEv
// Empty in the original.
void CTransportManager::AbortNetwork() {}

// 0x81a9e0 _ZN17CTransportManager11GetInstanceEv
CTransportManager* CTransportManager::GetInstance() {
    return s_transportManager;
}

// 0x81aa04 _ZN17CTransportManager13IsInitializedEv
bool CTransportManager::IsInitialized() {
    return s_transportManager != nullptr && s_transportManager->initialized_;
}

// 0x81b800 _ZN17CTransportManager10InitializeEv
// Partial: the original calls one virtual slot (vtable +0x00) on the instance after it is
// created. The slot is not named in the pseudocode, so that call is not reproduced.
int CTransportManager::Initialize() {
    if (s_transportManager == nullptr) {
        s_transportManager = new (std::nothrow) CTransportManagerImpl();
    }
    return s_transportManager != nullptr ? 0 : -1;
}

// 0x81aa2c _ZN17CTransportManager9TerminateEv
// Partial: the original calls two virtual slots (+0x04, +0x18) on the instance before it
// clears the pointer. Their meaning is not recovered; the port deletes the instance, and
// the deleting destructor (+0x04 in the original's Impl vtable) does the same job.
void CTransportManager::Terminate() {
    CTransportManager* manager = s_transportManager;
    if (manager != nullptr) {
        delete manager;
        s_transportManager = nullptr;
    }
}

// 0x81aa8c _ZN17CTransportManager15EnableBroadcastE10CNetworkId
// Partial: the original's loop ends with the found transport's StartBroadcast (+0x30) call.
// When no transport matches, the original returns an uninitialised pointer; the port
// returns without calling anything.
void CTransportManager::EnableBroadcast(const CNetworkId& id) {
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport == nullptr) {
            continue;
        }
        CNetworkId local;
        transport_slots::GetLocalId(transport, local);
        if (local.IsEqual(id)) {
            transport->StartBroadcast();
            return;
        }
    }
}

// 0x81ab0c _ZN17CTransportManager16DisableBroadcastE10CNetworkId
// Partial: same shape as EnableBroadcast; the found transport gets StopBroadcast (+0x34).
// The no-match return value is not recovered.
void CTransportManager::DisableBroadcast(const CNetworkId& id) {
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport == nullptr) {
            continue;
        }
        CNetworkId local;
        transport_slots::GetLocalId(transport, local);
        if (local.IsEqual(id)) {
            transport->StopBroadcast();
            return;
        }
    }
}

// 0x81ab8c _ZN17CTransportManager20GetTransportByPeerE15tTRANSPORT_TYPE10CNetworkId
// Partial: the type and peer tests go through transport_slots (vtable +0x20 and +0x28),
// which are stand-ins until CTransport declares them.
CTransport* CTransportManager::GetTransportByPeer(tTRANSPORT_TYPE type, const CNetworkId& peer) const {
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport != nullptr && transport_slots::Type(transport) == type &&
            transport_slots::IsConnectedTo(transport, peer)) {
            return transport;
        }
    }
    return nullptr;
}

// 0x81ac0c _ZN17CTransportManager13IsConnectedToER10CNetworkId
// Partial: the result is the transport's state slot (vtable +0x24), which is not named in
// transport.hpp; transport_slots::ConnectedState stands in for it.
bool CTransportManager::IsConnectedTo(const CNetworkId& peer) const {
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport != nullptr && transport_slots::IsConnectedTo(transport, peer)) {
            return transport_slots::ConnectedState(transport) != 0;
        }
    }
    return false;
}

// 0x81ac78 _ZN17CTransportManager18IsMainThreadBlockedEv
// The original returns constant 0.
bool CTransportManager::IsMainThreadBlocked() const {
    return false;
}

// 0x81ac80 _ZN17CTransportManager18SetThreadTimestampEv
// Partial: the original reads the 64-bit clock from GetOnline() (vtable +0x00). The port
// uses the millisecond clock of online_calls::Now().
std::int64_t CTransportManager::SetThreadTimestamp() {
    thread_timestamp_ = static_cast<std::int64_t>(online_calls::Now());
    return thread_timestamp_;
}

// 0x81ad30 _ZN17CTransportManager18GetListenNetworkIdE15tTRANSPORT_TYPE
// Partial: the fields of type 3 and 4 (field2 and field3, presence bits 2 and 3) have no
// public setter in CNetworkId, so those transports are skipped here. The listening flag
// (object flag at +0x04) and the local id are read through transport_slots.
CNetworkId CTransportManager::GetListenNetworkId(tTRANSPORT_TYPE type) const {
    CNetworkId result;
    result.Reset();
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport == nullptr) {
            continue;
        }
        if (type != 0 && transport_slots::Type(transport) != type) {
            continue;
        }
        if (!transport_slots::IsListening(transport)) {
            continue;
        }
        CNetworkId local;
        transport_slots::GetLocalId(transport, local);
        std::uint16_t lo = 0;
        std::uint32_t hi = 0;
        switch (transport_slots::Type(transport) - 1) {
            case 0:  // transport 1 -> address1 (presence bit 0)
                local.GetAddress1(lo, hi);
                result.SetAddress1(lo, hi);
                break;
            case 1:  // transport 2 -> address0 (presence bit 1)
                local.GetAddress0(lo, hi);
                result.SetAddress0(lo, hi);
                break;
            default:
                // Types 3 and 4 (field2, field3): not settable through CNetworkId's public API.
                break;
        }
    }
    return result;
}

// 0x81ae94 _ZN17CTransportManager20IsTransportAvailableE15tTRANSPORT_TYPE
// Partial: types 3 and 4 test field2 for a nonzero value in the original; the port tests the
// field2 presence bit (CNetworkId::Flags() bit 2) instead, as no value accessor exists.
// Types outside 1..4 are available (constant 1).
bool CTransportManager::IsTransportAvailable(tTRANSPORT_TYPE type) {
    CTransportManager& manager = GetTransportMgr();
    switch (static_cast<unsigned>(type - 1)) {
        case 0:
        case 1: {
            CNetworkId id = manager.GetListenNetworkId(1);
            std::uint16_t lo = 0;
            std::uint32_t hi = 0;
            id.GetAddress1(lo, hi);
            return hi != 0;
        }
        case 2:
        case 3: {
            CNetworkId id = manager.GetListenNetworkId(3);
            return (id.Flags() & 4u) != 0;
        }
        default:
            return true;
    }
}

// 0x81af88 _ZN17CTransportManager27PrepareConnectionNetworkIdER10CNetworkId
// Partial: the argument roles are mixed in the pseudocode; the port copies the peer id and,
// for each transport that is connected to it, resets the id to that transport's type.
CNetworkId CTransportManager::PrepareConnectionNetworkId(const CNetworkId& peer) const {
    CNetworkId result = peer;
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport != nullptr && transport_slots::IsConnectedTo(transport, peer)) {
            result.Reset(transport_slots::Type(transport));
        }
    }
    return result;
}

// 0x81b010 _ZN17CTransportManager4SendEP10CTransportR10CNetworkIdiPvi
// Packet layout (from ReceiverCallback): magic u32 at +0, length u16 at +4 (header
// included), type byte at +6, payload at +8. The original's stack slots overlap, so the
// layout is inferred from the receiver.
// Partial: the original reports statistics (CConnectionManager::ReportStatistic) before the
// send; that call is not reproduced. The transport send is a shim until CTransport declares it.
int CTransportManager::Send(CTransport* transport, const CNetworkId& to, tPACKET_TYPE type,
                            const void* data, std::size_t size) {
    if (size > kMaxPayload) {
        return -1;
    }
    const std::size_t total = size + kHeaderBytes;
    char packet[kMaxPayload + kHeaderBytes];
    const std::uint32_t magic = kPacketMagic;
    packet[0] = static_cast<char>(magic & 0xFF);
    packet[1] = static_cast<char>((magic >> 8) & 0xFF);
    packet[2] = static_cast<char>((magic >> 16) & 0xFF);
    packet[3] = static_cast<char>((magic >> 24) & 0xFF);
    packet[4] = static_cast<char>(total & 0xFF);
    packet[5] = static_cast<char>((total >> 8) & 0xFF);
    packet[6] = static_cast<char>(type & 0x7F);
    packet[7] = 0;
    if (size != 0) {
        std::memcpy(packet + kHeaderBytes, data, size);
    }
    if (transport == nullptr) {
        return -1;
    }
    return transport_slots::Send(transport, to, packet, static_cast<int>(total));
}

// 0x81b11c _ZN17CTransportManager16ReceiverCallbackEP10CTransportR10CNetworkIdPci
// Partial: the pseudocode reads the packet buffer from the id argument and the length from
// the buffer argument; the port follows the symbol order (data, size) and the framing. Each
// packet is checked the same way as the original: length within the remaining bytes, magic,
// and type bit 7 clear. Deviation: a length below the 8-byte header ends the pass. The original tests IsInitialized() and reports statistics per
// packet; the statistics call is not reproduced. Type 7 (keep-alive) is answered with a
// type 8 packet, as in the original. The port adds a bounds check so that a buffer shorter
// than a header is rejected.
void CTransportManager::ReceiverCallback(CTransport* transport, CNetworkId& from, char* data, int size) {
    if (size <= 0) {
        return;
    }
    int remaining = size;
    char* packet = data;
    while (true) {
        if (remaining < static_cast<int>(kHeaderBytes)) {
            return;
        }
        const int length = static_cast<std::int16_t>(ReadU16(packet + 4));
        // A length below the header size would never advance the loop (the original spins
        // here); such a packet ends the pass instead. Deviation, see the comment above.
        if (length < static_cast<int>(kHeaderBytes) || length > remaining ||
            ReadU32(packet) != kPacketMagic || (packet[6] & 0x80) != 0) {
            return;
        }
        const int type = packet[6] & 0x7F;
        if (IsInitialized() && type == 7 && !IsMainThreadBlocked()) {
            Send(transport, from, 8, nullptr, 0);
        }
        remaining -= length;
        CConnectionManager::sReceiverCallback(type, from, packet + kHeaderBytes, length - static_cast<int>(kHeaderBytes));
        if (remaining <= 0) {
            return;
        }
        packet += length;
    }
}

// 0x81b290 _ZN17CTransportManager4SendE15tTRANSPORT_TYPE10CNetworkIdiPvi
int CTransportManager::Send(tTRANSPORT_TYPE type, const CNetworkId& to, tPACKET_TYPE packet_type,
                            const void* data, std::size_t size) {
    CTransport* transport = GetTransportByPeer(type, to);
    return Send(transport, to, packet_type, data, size);
}

// 0x81b2fc _ZN17CTransportManager4SendE15tTRANSPORT_TYPEiPvi
// Partial: types 1 and 2 use the broadcast address (port 6776, address word -1) on
// address1 and address0. Types 3 and 4 need field2 and field3 set to -1, which CNetworkId
// cannot do through its public API, so no packet is sent for them here.
int CTransportManager::Send(tTRANSPORT_TYPE type, tPACKET_TYPE packet_type, const void* data,
                            std::size_t size) {
    CNetworkId id;
    id.Reset();
    switch (type) {
        case 1:
            id.SetAddress1(kBroadcastPort, kBroadcastHi);
            break;
        case 2:
            id.SetAddress0(kBroadcastPort, kBroadcastHi);
            break;
        default:
            return 0;
    }
    return Send(type, id, packet_type, data, size);
}

// 0x81b3c8 _ZN17CTransportManager7ReceiveEv
// Partial: lock, then one receive pass over all transports, skipped while terminating. The
// per-transport receive call (vtable +0x10) is a transport_slots stand-in.
void CTransportManager::Receive() {
    if (!initialized_ || terminating_) {
        return;
    }
    mutex_.Lock();
    for (int i = 0; i < kTransportSlots; ++i) {
        if (!terminating_ && transports_[i] != nullptr) {
            transport_slots::Receive(transports_[i]);
        }
    }
    mutex_.Unlock();
}

// 0x81b440 _ZN17CTransportManager10DisconnectER10CNetworkId
// Partial: the test uses CanDisconnectFrom (vtable +0x2c), a stand-in until CTransport
// declares it. The removed transport is deleted through its deleting destructor (+0x04).
void CTransportManager::Disconnect(const CNetworkId& peer) {
    mutex_.Lock();
    for (int i = 0; i < kTransportSlots; ++i) {
        CTransport* transport = transports_[i];
        if (transport != nullptr && transport_slots::CanDisconnectFrom(transport, peer)) {
            delete transport;
            transports_[i] = nullptr;
        }
    }
    mutex_.Unlock();
}

// 0x81b4c4 _ZN17CTransportManager12AddTransportEP10CTransport
// Stores the transport in the first free slot.
void CTransportManager::AddTransport(CTransport* transport) {
    if (transport == nullptr) {
        return;
    }
    mutex_.Lock();
    for (int i = 0; i < kTransportSlots; ++i) {
        if (transports_[i] == nullptr) {
            transports_[i] = transport;
            break;
        }
    }
    mutex_.Unlock();
}

// 0x81b52c _ZN17CTransportManager14AcceptCallbackE15tTRANSPORT_TYPEP10CTransport
// The type is not used; the accepted transport is added to the singleton, as in the original.
void CTransportManager::AcceptCallback(tTRANSPORT_TYPE type, CTransport* transport) {
    (void)type;
    GetTransportMgr().AddTransport(transport);
}

// 0x81b544 _ZN17CTransportManager17InitializeInternalEv
// Partial: the original calls a virtual slot (vtable +0x08) with the high word of the
// timestamp after the table is cleared; that call is not reproduced.
int CTransportManager::InitializeInternal() {
    mutex_.Lock();
    for (auto& transport : transports_) {
        transport = nullptr;
    }
    SetThreadTimestamp();
    initialized_ = true;
    mutex_.Unlock();
    return 0;
}

// 0x81b5b4 _ZN17CTransportManager21ReceiverThreadExecuteEv
// Partial: the blocked branch (main thread blocked and this device is the server) calls
// a virtual slot (vtable +0x10) that is not named; it is not reproduced. Matching checks
// use matching_calls::IsServer (stand-in). The connection manager is null-checked here,
// where the original calls it unguarded.
void CTransportManager::ReceiverThreadExecute() {
    if (IsMainThreadBlocked()) {
        if (matching_calls::IsServer()) {
            // Virtual slot (vtable +0x10) not reconstructed.
        }
        return;
    }
    Receive();
    CConnectionManager* connections = CConnectionManager::GetInstance();
    if (connections != nullptr) {
        connections->KeepAliveConnections();
    }
}

// 0x81b604 _ZN17CTransportManager7ConnectER10CNetworkId
// Partial: the original passes a second register value to CTransportFactory::ConnectTransport
// that the pseudocode does not show. The port passes the prepared id only.
int CTransportManager::Connect(const CNetworkId& peer) {
    if (!initialized_) {
        return -1;
    }
    CNetworkId prepared = PrepareConnectionNetworkId(peer);
    CTransportFactory::ConnectTransport(prepared);
    return 0;
}

// 0x81b644 _ZN17CTransportManager19InitializeTransportE15tTRANSPORT_TYPE
int CTransportManager::InitializeTransport(tTRANSPORT_TYPE type) {
    return CTransportFactory::InitializeTransport(type);
}

// 0x81b64c _ZN17CTransportManager17TerminateInternalEv
// Partial: the original calls a virtual slot (vtable +0x0c) on the instance first; that
// call is not reproduced.
void CTransportManager::TerminateInternal() {
    terminating_ = true;
    initialized_ = false;
    mutex_.Lock();
    for (int i = 0; i < kTransportSlots; ++i) {
        if (transports_[i] != nullptr) {
            delete transports_[i];
            transports_[i] = nullptr;
        }
    }
    CTransportFactory::TerminateTransport();
    mutex_.Unlock();
}

// 0x81baec _ZN17CTransportManager17SendToAllTransportE10tPACKET_TYPER10CNetworkIdPvi
// Sends to every transport type listed in the id, in list order.
void CTransportManager::SendToAllTransport(tPACKET_TYPE type, const CNetworkId& to,
                                           const void* data, std::size_t size) {
    for (tTRANSPORT_TYPE transport_type : to.GetTransportTypeList()) {
        Send(transport_type, to, type, data, size);
    }
}

// 0x81bb88 _ZN17CTransportManager9KeepAliveER10CNetworkId
// Keep-alive is packet type 7 sent to every transport type of the peer id.
void CTransportManager::KeepAlive(const CNetworkId& peer) {
    SendToAllTransport(7, peer, nullptr, 0);
}
