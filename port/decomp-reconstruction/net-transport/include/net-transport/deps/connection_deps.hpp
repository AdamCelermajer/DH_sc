// Stand-ins for the classes CConnectionManager calls into. CMatching, CConnection,
// CEventQueueBase and CTransportManager are owned by other slices; only the members the
// connection manager uses are declared here. Slot numbers are the byte offsets of the
// virtual calls in the original vtables. Bodies are no-op or simple test doubles and must
// be replaced by the real classes when the categories are merged.
#pragma once

#include <cstdint>

#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_network_id.hpp"

class CTransportManager;  // full class: net-transport/deps/transport_manager.hpp

// CMatching: the matching/session state. Only the calls below are used.
class CMatching {
public:
    // 0x... _ZN9CMatching3GetEv (the Hex-Rays call sites pass a dropped argument)
    static CMatching* Get();
    // _ZN9CMatching8IsServerEv
    bool IsServer() const { return server_; }
    // Maps a network member id to its connection slot index (0..31), -1 when there is none.
    int GetMemberIndex(int member_id) const;

    // Virtual slots. The names are provisional; the byte offsets are authoritative.
    int LocalMemberId() const { return 0; }                 // vtable +0x6c
    void SetLocalMemberId(int value) { (void)value; }       // vtable +0x70 (provisional)
    int ServerMemberId() const { return 0; }                // vtable +0x74
    int NewMemberId() const { return 0; }                   // vtable +0xac
    int TransportHandle(int protocol) const { (void)protocol; return 0; }  // vtable +0x34

    // Test hook for the stand-in: the server flag returned by IsServer().
    static void SetServerForTest(bool server);

private:
    bool server_ = false;
};

// CConnection: one peer connection. Members used by the connection manager.
// Layout (original, 32-bit): +0x18 state, +0x1c member id, +0x20 network id,
// +0x50 timeout, +0x1b8 key (two words). The vtable is +0x00/+0x04 dtors, +0x08 Connect,
// +0x0c Accept, +0x10 Disconnect, +0x14 Update, +0x18 Send.
class CConnection {
public:
    CConnection() = default;
    virtual ~CConnection() = default;

    virtual int Connect(int member_id, CNetworkId& id);                       // +0x08
    virtual int Accept(int member_id, CNetworkId& id, std::uint64_t key);     // +0x0c
    virtual int Disconnect();                                                 // +0x10
    virtual void Update();                                                    // +0x14
    virtual int Send(int packet_type, const void* data, int size);            // +0x18

    void SendKeepAlive(std::uint32_t now) { (void)now; }                       // _ZN11CConnection13SendKeepAliveEj
    void SetLastRecvTime() {}                                                  // _ZN11CConnection15SetLastRecvTimeEv
    void ProcessConnectFinalize(CNetworkId& id) { (void)id; }                  // _ZN11CConnection22ProcessConnectFinalizeER10CNetworkId
    void ProcessConnectResponse(CNetworkId& id) { (void)id; }                  // _ZN11CConnection22ProcessConnectResponseER10CNetworkId
    void ProcessPing(const void* packet) { (void)packet; }                     // _ZN11CConnection11ProcessPingEPv (provisional)

    int State() const { return state_; }                 // +0x18
    int MemberId() const { return member_id_; }          // +0x1c
    CNetworkId& NetworkId() { return network_id_; }      // +0x20
    const CNetworkId& NetworkId() const { return network_id_; }
    std::uint64_t Key() const { return key_; }           // +0x1b8
    void SetTimeout(std::uint32_t ms) { timeout_ = ms; } // +0x50
    void SetState(int state) { state_ = state; }         // test hook
    void SetMemberId(int member_id) { member_id_ = member_id; }  // test hook

private:
    int state_ = 0;
    int member_id_ = -1;
    CNetworkId network_id_{};
    std::uint32_t timeout_ = 0;
    std::uint64_t key_ = 0;
};

// CEventQueueBase: the event queue at +0x08 of the connection manager.
// Vtable: +0x00 AddEvent, +0x0c Clear.
class CEventQueueBase {
public:
    virtual ~CEventQueueBase() = default;
    virtual int AddEvent(int event, void* data, int size);  // +0x00
    virtual int Clear();                                    // +0x0c
};

// Calls CConnectionManager makes on CTransportManager. The class itself is in the
// transport-manager slice; until it is merged these are no-op stand-ins that return 0.
namespace transport_manager_calls {
void Update(CTransportManager& tm);
void Initialize(CTransportManager& tm);
void Terminate(CTransportManager& tm);
void InitializeTransport(CTransportManager& tm, std::intptr_t transport);
void Disconnect(CTransportManager& tm, CNetworkId& peer);
// CTransportManager::Send with a network id: (transport, to, packet type, data, size).
int Send(CTransportManager& tm, std::intptr_t transport, const CNetworkId& to,
         int packet_type, const void* data, int size);
// CTransportManager::Send without a network id: (transport, packet type, data, size).
int Send(CTransportManager& tm, std::intptr_t transport, int packet_type,
         const void* data, int size);
}  // namespace transport_manager_calls
