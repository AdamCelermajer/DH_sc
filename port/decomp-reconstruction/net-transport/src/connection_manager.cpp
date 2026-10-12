// CConnectionManager. Reconstructed from the Hex-Rays pseudocode, checked against the ARM
// listing where the pseudocode symbol ranges were noted as misaligned (KeepAliveConnections,
// sReceiverCallback, Update, TerminateInternal). Offsets in comments are the original 32-bit ones.
#include "net-transport/connection_manager.hpp"

#include <chrono>
#include <cstddef>
#include <cstring>
#include <new>

#include "net-transport/deps/transport_manager.hpp"  // GetTransportMgr()

namespace {

// CConnection::state values used by the original (+0x18).
constexpr int kStateConnected = 4;  // IsConnected, SendToAll, Disconnect checks
constexpr int kStateDropped = 1;    // Update removes a connection in this state

// Event id posted by Disconnect when notify is set (0x600004).
constexpr int kEventDisconnected = 6291460;

// Packet type 5 carries the connection-management subtypes (first byte of the packet).
constexpr tPACKET_TYPE kConnectionPacketType = 5;
constexpr tPACKET_TYPE kPingPacketType = 0;
constexpr int kDisconnectSubtype = 3;

// Layout of the 16-byte ping packet (memcpy'd from the wire, original v7[] array).
struct PingPacket {
    std::int32_t word0;        // +0x00
    std::int32_t member_id;    // +0x04 (v7[1])
    std::int32_t word2;        // +0x08
    std::uint8_t reply_pending;  // +0x0c (LOBYTE(v7[3]))
    std::uint8_t pad[3];       // +0x0d..+0x0f
};
static_assert(sizeof(PingPacket) == 16, "ping packet is 16 bytes");

// Payload of the disconnect packet: the member id, the notify flag and padding (8 bytes).
struct DisconnectPayload {
    std::int32_t member_id;    // +0x00
    std::uint8_t notify;       // +0x04
    std::uint8_t pad[3];       // +0x05..+0x07
};
static_assert(sizeof(DisconnectPayload) == 8, "disconnect payload is 8 bytes");

// Time source for the keep-alive. The original reads GetOnline() vtable slot 0 (a clock in ms,
// truncated); here it is steady_clock in ms, as in CNetworkEmulation.
std::uint32_t KeepAliveNowMs() {
    using namespace std::chrono;
    return static_cast<std::uint32_t>(
        duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

}  // namespace

// Static state. The original stores the singleton and the mutex as globals.
CConnectionManager* CConnectionManager::s_connectionManager = nullptr;
CConnectionManager::RecvCallbacks CConnectionManager::s_recvCBMap[CConnectionManager::kPacketTypes];
CNetMutex CConnectionManager::m_connections_mutex;

// 0x7fc0a8 _ZN18CConnectionManagerC1Ev (partial: the event queue sentinel is not reproduced)
// The C2 variant at 0x7fc140 has the same body and is skipped.
CConnectionManager::CConnectionManager() {
    // Original: vptr, CNetMutex at +0x0c, then the event queue at +0x08 with its list sentinel
    // at +0x10 and capacity 100 at +0x18. Those are set up by the CEventQueueBase stand-in.
    for (CConnection*& slot : connections_) {
        slot = nullptr;
    }
    initialized_ = false;
}

// 0x7fd400 _ZN18CConnectionManagerD1Ev (partial: the list clear is left to the event queue)
// The D2 variant at 0x7fd4b4 has the same body and is skipped; D0 at 0x7fd454 is skipped.
CConnectionManager::~CConnectionManager() {
    // Original: clears the CEvent list at +0x10, then destroys the CNetMutex at +0x0c.
    // Here the members are destroyed in reverse order; the event queue clears itself.
}

// 0x7fbd4c _ZN18CConnectionManager7GetImplEv
int CConnectionManager::GetImpl() {
    return 0;
}

// 0x7fcc30 _ZN18CConnectionManager6UpdateEv
int CConnectionManager::Update() {
    transport_manager_calls::Update(GetTransportMgr());
    for (CConnection*& slot : connections_) {
        if (!slot) {
            continue;
        }
        slot->Update();
        if (slot->State() != kStateDropped) {
            continue;
        }
        transport_manager_calls::Disconnect(GetTransportMgr(), slot->NetworkId());
        m_connections_mutex.Lock();
        if (slot) {
            delete slot;
            slot = nullptr;
        }
        m_connections_mutex.Unlock();
    }
    PingConnections();
    PrintStatistics();
    return 0;
}

// 0x7fbd54 _ZN18CConnectionManager11GetInstanceEv
CConnectionManager* CConnectionManager::GetInstance() {
    return s_connectionManager;
}

// 0x7fbd78 _ZN18CConnectionManager13IsInitializedEv
bool CConnectionManager::IsInitialized() {
    return s_connectionManager != nullptr && s_connectionManager->initialized_;
}

// 0x7fbda0 _ZN18CConnectionManager24GetConnectionByNetworkIdER10CNetworkId
CConnection* CConnectionManager::GetConnectionByNetworkId(CNetworkId& id) {
    for (CConnection* conn : connections_) {
        if (conn && conn->NetworkId().IsEqual(id)) {
            return conn;
        }
    }
    return nullptr;
}

// 0x7fbdf8 _ZN18CConnectionManager11GetMemberIdER10CNetworkId
int CConnectionManager::GetMemberId(CNetworkId& id) {
    for (CConnection* conn : connections_) {
        if (conn && conn->NetworkId().IsEqual(id)) {
            return conn->MemberId();
        }
    }
    return -1;
}

// 0x7fbe54 _ZN18CConnectionManager13DisconnectAllEv
int CConnectionManager::DisconnectAll() {
    for (CConnection* conn : connections_) {
        if (conn) {
            conn->Disconnect();
        }
    }
    return 0;
}

// 0x7fbe94 _ZN18CConnectionManager18GetConnectionCountEb
int CConnectionManager::GetConnectionCount(bool connected_only) {
    int count = 0;
    for (CConnection* conn : connections_) {
        if (conn && (!connected_only || conn->State() == kStateConnected)) {
            ++count;
        }
    }
    return count;
}

// 0x7fbeec _ZN18CConnectionManager9SendToAllE12tPACKET_TYPEPvi
int CConnectionManager::SendToAll(tPACKET_TYPE type, void* data, int size) {
    for (CConnection* conn : connections_) {
        if (conn && conn->State() == kStateConnected) {
            conn->Send(type, data, size);
        }
    }
    return 0;
}

// 0x7fbf5c _ZN18CConnectionManager18RegisterPacketTypeE12tPACKET_TYPE14tPROTOCOL_TYPEPFviPciE
int CConnectionManager::RegisterPacketType(tPACKET_TYPE type, tPROTOCOL_TYPE protocol, MemberCallback cb) {
    RecvCallbacks& entry = s_recvCBMap[type];
    entry.protocol = protocol;
    entry.by_member = cb;
    return 0;
}

// 0x7fbf98 _ZN18CConnectionManager18RegisterPacketTypeE12tPACKET_TYPE14tPROTOCOL_TYPEPFvR10CNetworkIdPciE
int CConnectionManager::RegisterPacketType(tPACKET_TYPE type, tPROTOCOL_TYPE protocol, NetIdCallback cb) {
    RecvCallbacks& entry = s_recvCBMap[type];
    entry.protocol = protocol;
    entry.by_network_id = cb;
    return 0;
}

// 0x7fbfd4 _ZN18CConnectionManager22IsPacketTypeRegisteredE12tPACKET_TYPE
bool CConnectionManager::IsPacketTypeRegistered(tPACKET_TYPE type) {
    const RecvCallbacks& entry = s_recvCBMap[type];
    return entry.by_network_id != nullptr || entry.by_member != nullptr;
}

// 0x7fc014 _ZN18CConnectionManager22GetPacketTransportTypeE12tPACKET_TYPE
std::intptr_t CConnectionManager::GetPacketTransportType(tPACKET_TYPE type) {
    if (!IsPacketTypeRegistered(type)) {
        return 0;
    }
    return GetTransportType(s_recvCBMap[type].protocol);
}

// 0x7fc060 _ZN18CConnectionManager16GetTransportTypeE14tPROTOCOL_TYPE
// partial: the original reads past the three-entry table for an out-of-range protocol;
// this returns 0 instead.
std::intptr_t CConnectionManager::GetTransportType(tPROTOCOL_TYPE protocol) {
    if (protocol < 0 || protocol >= kTransportSlots) {
        return 0;
    }
    return transports_[protocol];
}

// 0x7fc06c _ZN18CConnectionManager15PingConnectionsEv (empty in the original)
void CConnectionManager::PingConnections() {}

// 0x7fc070 _ZN18CConnectionManager21SetConnectionTimeoutsEj
// The pseudocode returns the loop index; the value is not used, so this returns void.
void CConnectionManager::SetConnectionTimeouts(std::uint32_t timeout) {
    for (CConnection* conn : connections_) {
        if (conn) {
            conn->SetTimeout(timeout);
        }
    }
}

// 0x7fc094 _ZN18CConnectionManager15ReportStatisticEi21tCONNECTION_STATISTICi (empty)
void CConnectionManager::ReportStatistic(int transport, tCONNECTION_STATISTIC stat, int value) {
    (void)transport;
    (void)stat;
    (void)value;
}

// 0x7fc098 _ZN18CConnectionManager15ReportStatisticE15tTRANSPORT_TYPER10CNetworkId21tCONNECTION_STATISTICi (empty)
void CConnectionManager::ReportStatistic(tTRANSPORT_TYPE transport, CNetworkId& id,
                                         tCONNECTION_STATISTIC stat, int value) {
    (void)transport;
    (void)id;
    (void)stat;
    (void)value;
}

// 0x7fc09c _ZN18CConnectionManager15PrintStatisticsEv (empty)
void CConnectionManager::PrintStatistics() {}

// 0x7fc0a0 _ZN18CConnectionManager21PrintMergedStatisticsEv (empty)
void CConnectionManager::PrintMergedStatistics() {}

// 0x7fc0a4 _ZN18CConnectionManager12GetBandwidthERiS0_ (empty: the outputs are not written)
void CConnectionManager::GetBandwidth(int& in, int& out) {
    (void)in;
    (void)out;
}

// 0x7fc1d8 _ZN18CConnectionManager20KeepAliveConnectionsEv (ARM checked)
// The time source (GetOnline() slot 0) is replaced by steady_clock in ms.
int CConnectionManager::KeepAliveConnections() {
    const std::uint32_t now = KeepAliveNowMs();
    m_connections_mutex.Lock();
    for (CConnection* conn : connections_) {
        if (conn) {
            conn->SendKeepAlive(now);
        }
    }
    return m_connections_mutex.Unlock();
}

// 0x7fc244 _ZN18CConnectionManager17sReceiverCallbackE12tPACKET_TYPER10CNetworkIdPci (ARM checked)
int CConnectionManager::sReceiverCallback(tPACKET_TYPE type, CNetworkId& from, char* data, int size) {
    m_connections_mutex.Lock();
    CConnectionManager* self = GetInstance();
    // Types 7 and 8 bypass the registration check (unsigned (type - 7) <= 1).
    if (static_cast<unsigned int>(type - 7) <= 1u || IsPacketTypeRegistered(type)) {
        NetIdCallback by_netid = nullptr;
        CConnection* conn = self->GetConnectionByNetworkId(from);
        if (conn) {
            const int member = conn->MemberId();
            conn->SetLastRecvTime();
            const RecvCallbacks& entry = s_recvCBMap[type];
            if (entry.by_member) {
                entry.by_member(member, data, size);
                return m_connections_mutex.Unlock();
            }
            by_netid = entry.by_network_id;
        } else {
            by_netid = s_recvCBMap[type].by_network_id;
        }
        if (by_netid) {
            by_netid(from, data, size);
        }
    }
    return m_connections_mutex.Unlock();
}

// 0x7fc33c _ZN18CConnectionManager20UnregisterPacketTypeE12tPACKET_TYPE
// The original returns -1 whatever the outcome.
int CConnectionManager::UnregisterPacketType(tPACKET_TYPE type) {
    RecvCallbacks& entry = s_recvCBMap[type];
    entry.protocol = 0;
    entry.by_member = nullptr;
    entry.by_network_id = nullptr;
    return -1;
}

// 0x7fc3ac _ZN18CConnectionManager6SendToE12tPACKET_TYPER10CNetworkIdPvi
// partial: CTransportManager::Send is not reconstructed (the stand-in returns 0 and sends nothing).
int CConnectionManager::SendTo(tPACKET_TYPE type, CNetworkId& to, void* data, int size) {
    const std::intptr_t transport = GetPacketTransportType(type);
    return transport_manager_calls::Send(GetTransportMgr(), transport, to, type, data, size);
}

// 0x7fc3ec _ZN18CConnectionManager13SendBroadcastE12tPACKET_TYPEPvi
// partial: CTransportManager::Send is not reconstructed (the stand-in sends nothing).
int CConnectionManager::SendBroadcast(tPACKET_TYPE type, void* data, int size) {
    const std::intptr_t transport = GetPacketTransportType(type);
    return transport_manager_calls::Send(GetTransportMgr(), transport, type, data, size);
}

// 0x7fc438 _ZN18CConnectionManager23GetConnectionByMemberIdEi
CConnection* CConnectionManager::GetConnectionByMemberId(int member_id) {
    const int index = CMatching::Get()->GetMemberIndex(member_id);
    if (index < 0 || index >= kConnectionSlots) {
        return nullptr;
    }
    return connections_[index];
}

// 0x7fc464 _ZN18CConnectionManager20PingReceiverCallbackER10CNetworkIdPci
// partial: the ping packet layout is inferred (flag byte at +0x0c, member id at +0x04) and
// CConnection::ProcessPing result is not used here.
void CConnectionManager::PingReceiverCallback(CNetworkId& from, char* data, int size) {
    (void)size;
    PingPacket packet{};
    std::memcpy(&packet, data, sizeof(packet));
    if (packet.reply_pending) {
        packet.reply_pending = 0;
        SendTo(kPingPacketType, from, &packet, static_cast<int>(sizeof(packet)));
        return;
    }
    if (CConnection* conn = GetConnectionByMemberId(packet.member_id)) {
        tMsgPing ping{packet.word0, 0, packet.word2};
        conn->ProcessPing(ping);
    }
}

// 0x7fc4e0 _ZN18CConnectionManager21sPingReceiverCallbackER10CNetworkIdPci
void CConnectionManager::sPingReceiverCallback(CNetworkId& from, char* data, int size) {
    GetInstance()->PingReceiverCallback(from, data, size);
}

// 0x7fc508 _ZN18CConnectionManager25ProcessConnectionFinalizeER10CNetworkIdR12NetBitStream
// partial: the 16-byte key is compared by its first word to the member id (as the pseudocode
// does); the layout of the rest of the key is not understood. The original return value is
// the member id and is not used by the caller, so this returns void.
void CConnectionManager::ProcessConnectionFinalize(CNetworkId& from, NetBitStream& stream) {
    std::uint8_t key[16] = {};
    stream.ReadByteArray(key, sizeof(key));
    std::int32_t key_word0 = 0;
    std::memcpy(&key_word0, key, sizeof(key_word0));
    const int member = GetMemberId(from);
    if (member == key_word0) {
        if (CConnection* conn = GetConnectionByMemberId(member)) {
            conn->ProcessConnectFinalize(from);
        }
    }
}

// 0x7fc568 _ZN18CConnectionManager25ProcessConnectionResponseER10CNetworkIdR12NetBitStream
// partial: the matching-state update after the response (the vtable calls at +0x6c/+0x70 on
// CMatching, provisional names) is applied with provisional slot names.
void CConnectionManager::ProcessConnectionResponse(CNetworkId& from, NetBitStream& stream) {
    std::int32_t key_words[4] = {};
    stream.ReadByteArray(key_words, sizeof(key_words));
    const int member = GetMemberId(from);
    if (member == key_words[0]) {
        if (CConnection* conn = GetConnectionByMemberId(member)) {
            conn->ProcessConnectResponse(from);
        }
    }
    // Key word 1 is compared with the local member id and stored when different.
    CMatching* matching = CMatching::Get();
    if (key_words[1] != matching->LocalMemberId()) {
        CMatching::Get()->SetLocalMemberId(key_words[1]);
    }
}

// 0x7fc5f8 _ZN18CConnectionManager6SendToE12tPACKET_TYPEiPvi
int CConnectionManager::SendTo(tPACKET_TYPE type, int member_id, void* data, int size) {
    CConnection* conn = GetConnectionByMemberId(member_id);
    if (!conn) {
        return 0;
    }
    return conn->Send(type, data, size);
}

// 0x7fc638 _ZN18CConnectionManager12SendToServerE12tPACKET_TYPEPvi
// partial: the server member id comes from CMatching vtable +0x74 (provisional name).
int CConnectionManager::SendToServer(tPACKET_TYPE type, void* data, int size) {
    const int server = CMatching::Get()->ServerMemberId();
    return SendTo(type, server, data, size);
}

// 0x7fc680 _ZN18CConnectionManager14DisconnectPeerEi
int CConnectionManager::DisconnectPeer(int member_id) {
    CConnection* conn = GetConnectionByMemberId(member_id);
    if (!conn) {
        return 0;
    }
    return conn->Disconnect();
}

// 0x7fc6a8 _ZN18CConnectionManager18GetMemberNetworkIdEi
CNetworkId CConnectionManager::GetMemberNetworkId(int member_id) {
    CNetworkId out;
    if (CConnection* conn = GetConnectionByMemberId(member_id)) {
        out = conn->NetworkId();
    } else {
        out.Reset();
    }
    return out;
}

// 0x7fc6f4 _ZN18CConnectionManager11IsConnectedEi
bool CConnectionManager::IsConnected(int member_id) {
    CConnection* conn = GetConnectionByMemberId(member_id);
    return conn != nullptr && conn->State() == kStateConnected;
}

// 0x7fc718 _ZN18CConnectionManager10DisconnectEib
// partial: the local member id is read from CMatching vtable +0x6c (provisional name) and
// decides between DisconnectPeer and DisconnectAll on the non-server path.
int CConnectionManager::Disconnect(int member_id, bool notify) {
    if (CMatching::Get()->IsServer() && IsConnected(member_id)) {
        DisconnectPayload payload{};
        payload.member_id = member_id;
        payload.notify = notify ? 1 : 0;
        NetBitStream stream(0x800);
        std::uint8_t subtype = kDisconnectSubtype;
        stream.WriteByteArray(&subtype, 1);
        stream.WriteByteArray(&payload, sizeof(payload));
        const std::uint32_t bits = stream.WritePosition();
        const std::uint32_t bytes = (bits >> 3) + ((bits & 7u) ? 1u : 0u);
        std::vector<char> data(bytes);
        stream.GetBuffer(data.data(), bytes);
        GetInstance()->SendToAll(kConnectionPacketType, data.data(), static_cast<int>(bytes));
    }

    if (CMatching::Get()->IsServer()) {
        DisconnectPeer(member_id);
    } else if (CMatching::Get()->LocalMemberId() != member_id) {
        DisconnectPeer(member_id);
    } else {
        DisconnectAll();
    }

    if (notify) {
        event_queue_.AddEvent(kEventDisconnected, &member_id, static_cast<int>(sizeof(member_id)));
    }
    return 0;
}

// 0x7fc830 _ZN18CConnectionManager17ProcessDisconnectER10CNetworkIdR12NetBitStream
int CConnectionManager::ProcessDisconnect(CNetworkId& from, NetBitStream& stream) {
    (void)from;
    DisconnectPayload payload{};
    stream.ReadByteArray(&payload, sizeof(payload));
    return Disconnect(payload.member_id, payload.notify != 0);
}

// 0x7fc864 _ZN18CConnectionManager21ProcessDisconnectPeerER10CNetworkIdR12NetBitStream
int CConnectionManager::ProcessDisconnectPeer(CNetworkId& from, NetBitStream& stream) {
    (void)from;
    std::int32_t member_id = 0;
    stream.ReadByteArray(&member_id, sizeof(member_id));
    if (!IsConnected(member_id)) {
        return 0;
    }
    if (CMatching::Get()->IsServer()) {
        return Disconnect(member_id, false);
    }
    return DisconnectPeer(member_id);
}

// 0x7fc8d0 _ZN18CConnectionManager13AddConnectionEiP11CConnection
int CConnectionManager::AddConnection(int member_id, CConnection* conn) {
    const int index = CMatching::Get()->GetMemberIndex(member_id);
    if (!conn || index < 0 || index > kConnectionSlots - 1) {
        return -1;
    }
    if (connections_[index]) {
        return -1;
    }
    connections_[index] = conn;
    return index;
}

// 0x7fc930 _ZN18CConnectionManager7ConnectEiR10CNetworkId
// The original allocates 0x1c0 bytes with a custom operator new; this uses nothrow new.
int CConnectionManager::Connect(int member_id, CNetworkId& id) {
    CConnection* conn = GetConnectionByMemberId(member_id);
    if (!conn) {
        conn = new (std::nothrow) CConnection();
        if (!conn) {
            return -1;
        }
        const int added = AddConnection(member_id, conn);
        if (added < 0) {
            delete conn;
            return added;
        }
    }
    return conn->Connect(member_id, id);
}

// 0x7fc9cc _ZN18CConnectionManager6AcceptEiR10CNetworkIdy
// partial: the original's vtable call at +0x0c passes one more argument than Accept's
// signature (the first word of the connection object); that argument is not reproduced.
int CConnectionManager::Accept(int member_id, CNetworkId& id, std::uint64_t key) {
    if (CConnection* existing = GetConnectionByMemberId(member_id)) {
        return existing->Accept(member_id, id, key);
    }
    CConnection* conn = new (std::nothrow) CConnection();
    const int added = AddConnection(member_id, conn);
    if (added < 0) {
        delete conn;
        return added;
    }
    if (!conn) {
        return -1;
    }
    return conn->Accept(member_id, id, key);
}

// 0x7fca74 _ZN18CConnectionManager24ProcessConnectionRequestER10CNetworkIdR12NetBitStream
// partial: a new member id (CMatching vtable +0xac, provisional name) is taken when the
// packet has no member id and no existing connection matches its key.
int CConnectionManager::ProcessConnectionRequest(CNetworkId& from, NetBitStream& stream) {
    // Words: [0] member id, [1] unused here, [2] key low, [3] key high.
    std::int32_t words[4] = {};
    stream.ReadByteArray(words, sizeof(words));
    std::int32_t member = words[0];
    const std::uint64_t key =
        (static_cast<std::uint64_t>(static_cast<std::uint32_t>(words[3])) << 32) |
        static_cast<std::uint32_t>(words[2]);

    if (member < 0) {
        CConnection* found = nullptr;
        for (CConnection* conn : connections_) {
            if (conn && conn->Key() == key) {
                found = conn;
                break;
            }
        }
        if (found && found->MemberId() >= 0) {
            return Accept(found->MemberId(), from, key);
        }
        member = CMatching::Get()->NewMemberId();
    }
    return Accept(member, from, key);
}

// 0x7fcb2c _ZN18CConnectionManager22PacketReceiverCallbackER10CNetworkIdPci
void CConnectionManager::PacketReceiverCallback(CNetworkId& from, char* data, int size) {
    NetBitStream stream(static_cast<std::uint32_t>(size));
    stream.SetBuffer(data, static_cast<std::size_t>(size));
    std::uint8_t subtype = 0;
    stream.ReadByteArray(&subtype, 1);
    switch (subtype) {
        case 0:
            ProcessConnectionRequest(from, stream);
            break;
        case 1:
            ProcessConnectionResponse(from, stream);
            break;
        case 2:
            ProcessConnectionFinalize(from, stream);
            break;
        case 3:
            ProcessDisconnect(from, stream);
            break;
        case 4:
            ProcessDisconnectPeer(from, stream);
            break;
        default:
            break;
    }
}

// 0x7fcc08 _ZN18CConnectionManager23sPacketReceiverCallbackER10CNetworkIdPci
void CConnectionManager::sPacketReceiverCallback(CNetworkId& from, char* data, int size) {
    GetInstance()->PacketReceiverCallback(from, data, size);
}

// 0x7fcd04 _ZN18CConnectionManager17TerminateInternalEv (ARM checked)
// partial: the ARM calls CTransportManager::Terminate with whatever r0 holds after the mutex
// unlock. This port calls it on GetTransportMgr().
int CConnectionManager::TerminateInternal() {
    initialized_ = false;
    DisconnectAll();
    for (CConnection*& slot : connections_) {
        m_connections_mutex.Lock();
        if (slot) {
            delete slot;
            slot = nullptr;
        }
        m_connections_mutex.Unlock();
    }
    transport_manager_calls::Terminate(GetTransportMgr());
    UnregisterPacketType(5);
    UnregisterPacketType(0);
    return event_queue_.Clear();
}

// 0x7fcd98 _ZN18CConnectionManager9TerminateEv
// The pseudocode returns the value left by the last call (the deleting destructor's r0); that
// value is not meaningful, so this returns void.
void CConnectionManager::Terminate() {
    if (!s_connectionManager) {
        return;
    }
    s_connectionManager->TerminateInternal();
    if (s_connectionManager) {
        delete s_connectionManager;
        s_connectionManager = nullptr;
    }
}

// 0x7fcdec _ZN18CConnectionManager18InitializeInternalEv (ARM checked)
// partial: the transport handles come from CMatching vtable +0x34 (name not recovered). The ARM
// calls CTransportManager::Initialize with this+0x1c (the memset return value); this port calls it
// on GetTransportMgr().
int CConnectionManager::InitializeInternal() {
    RegisterPacketType(kConnectionPacketType, 0, &CConnectionManager::sPacketReceiverCallback);
    RegisterPacketType(kPingPacketType, 0, &CConnectionManager::sPingReceiverCallback);
    for (int protocol = 0; protocol < kTransportSlots; ++protocol) {
        transports_[protocol] = CMatching::Get()->TransportHandle(protocol);
    }
    for (CConnection*& slot : connections_) {
        slot = nullptr;
    }
    transport_manager_calls::Initialize(GetTransportMgr());
    for (std::intptr_t transport : transports_) {
        transport_manager_calls::InitializeTransport(GetTransportMgr(), transport);
    }
    initialized_ = true;
    return 0;
}

// 0x7fcec0 _ZN18CConnectionManager10InitializeEv
int CConnectionManager::Initialize() {
    if (s_connectionManager) {
        return s_connectionManager->InitializeInternal();
    }
    CConnectionManager* self = new (std::nothrow) CConnectionManager();
    s_connectionManager = self;
    if (self) {
        return self->InitializeInternal();
    }
    return -1;
}

// 0x7fd284 _ZN18CConnectionManager19GetConnMemberIdListEb
// partial: the non-server path uses the server member id from CMatching vtable +0x74 (provisional).
std::vector<int> CConnectionManager::GetConnMemberIdList(bool all) {
    std::vector<int> ids;
    if (CMatching::Get()->IsServer() || all) {
        for (CConnection* conn : connections_) {
            if (conn) {
                ids.push_back(conn->MemberId());
            }
        }
    } else {
        ids.push_back(CMatching::Get()->ServerMemberId());
    }
    return ids;
}

// 0x7fd454 D0 and 0x7fd4b4 D2 are compiler-generated; see the destructor above.
