#include "net-transport/bluetooth_transport.hpp"

#include "net-transport/deps/transport_manager.hpp"

bool CBluetoothReliable::s_initialized_ = false;
bool CBluetoothUnreliable::s_initialized_ = false;

// ---- CBluetoothReliable ----

// 0x828cb4 _ZN18CBluetoothReliable7GetTypeEv
int CBluetoothReliable::GetType() const {
    return 4;
}

// 0x828cbc _ZN18CBluetoothReliable17CanDisconnectFromER10CNetworkId
bool CBluetoothReliable::CanDisconnectFrom(CNetworkId& peer) {
    (void)peer;
    return false;
}

// 0x828cc4 _ZN18CBluetoothReliable10InitializeEv
// Sets the initialized flag; -1 when it is already set.
int CBluetoothReliable::Initialize() {
    if (initialized_) {
        return -1;
    }
    initialized_ = true;
    return 0;
}

// 0x828ce0 _ZN18CBluetoothReliable10DisconnectEv
// Clears the initialized flag; -1 when it was not set.
int CBluetoothReliable::Disconnect() {
    if (!initialized_) {
        return -1;
    }
    initialized_ = false;
    return 0;
}

// 0x828cfc _ZN18CBluetoothReliable11IsConnectedEv
bool CBluetoothReliable::IsConnected() const {
    return false;
}

// 0x828d04 _ZN18CBluetoothReliable13IsConnectedToER10CNetworkId
bool CBluetoothReliable::IsConnectedTo(const CNetworkId& peer) const {
    (void)peer;
    return false;
}

// 0x828d0c _ZN18CBluetoothReliable6IsPeerEv
bool CBluetoothReliable::IsPeer() const {
    return false;
}

// 0x828d14 _ZN18CBluetoothReliable8IsClientEv
bool CBluetoothReliable::IsClient() const {
    return false;
}

// 0x828d1c _ZN18CBluetoothReliable11IsListeningEv
// Listening only when initialized and the socket reports a server role.
bool CBluetoothReliable::IsListening() const {
    return initialized_ && socket_.IsServer() && (flags_ & 1) != 0;
}

// 0x828d5c _ZN18CBluetoothReliable14StartBroadcastEv
void CBluetoothReliable::StartBroadcast() {}

// 0x828d60 _ZN18CBluetoothReliable13StopBroadcastEv
void CBluetoothReliable::StopBroadcast() {}

// 0x828d64 _ZN18CBluetoothReliable4SendER10CNetworkIdPvi
int CBluetoothReliable::Send(CNetworkId& peer, const void* data, int len) {
    (void)peer;
    (void)data;
    (void)len;
    return -1;
}

// 0x828d6c _ZN18CBluetoothReliable7ReceiveEv
int CBluetoothReliable::Receive() {
    return -1;
}

// 0x828d74 _ZN18CBluetoothReliable7SetModeE19_tBLUETOOTH_SESSION
void CBluetoothReliable::SetMode(int session) {
    (void)session;
}

// 0x828d78 _ZN18CBluetoothReliable18TerminateTransportEv
int CBluetoothReliable::TerminateTransport() {
    s_initialized_ = false;
    return 0;
}

// 0x82900c _ZN18CBluetoothReliableC2Ev
// 0x829330 _ZN18CBluetoothReliableC1Ev is the same body (skipped at that address).
// Reconstructed: the transport takes the default member values. The two std::string members of
// the original (address at +0x34, name at +0x50) are not modelled, as in CBluetoothUnreliable.
CBluetoothReliable::CBluetoothReliable() {
    peer_.Reset();
}

// 0x829260 _ZN18CBluetoothReliableC2Ejb
// 0x8295c4 _ZN18CBluetoothReliableC1Ejb is the same body (skipped at that address).
// Partial in the same way as CBluetoothUnreliable's constructor: the channel and listen arguments
// are not used by the pseudocode, so they are ignored.
CBluetoothReliable::CBluetoothReliable(unsigned int channel, bool listen) : CBluetoothReliable() {
    (void)channel;
    (void)listen;
}

// 0x82917c _ZN18CBluetoothReliableD1Ev
// 0x8291fc _ZN18CBluetoothReliableD2Ev is the same body (skipped at that address); 0x8291e0
// _ZN18CBluetoothReliableD0Ev is the deleting variant (skipped).
// Reconstructed: RestartBluetooth runs first, then the members are destroyed by the compiler.
CBluetoothReliable::~CBluetoothReliable() {
    CBluetoothSocket::RestartBluetooth();
}

// 0x828da0 _ZN18CBluetoothReliable17AcceptConnectionsEv
// Partial: accept_enabled_ (+0x30) is not set by any recovered code. The socket check is vtable
// +0x5c of CBluetoothSocket, which is IsServer(). AcceptCallback is the transport-manager
// stand-in (a no-op). The pseudocode's first argument is the global `byte_4` (an unidentified
// address, not a value), so the transport type passed is GetType(); that choice is a guess.
int CBluetoothReliable::AcceptConnections() {
    if (accept_enabled_ && socket_.IsServer() && (flags_ & 1) != 0) {
        GetTransportMgr().AcceptCallback(GetType(), this);
    }
    return 0;
}

// 0x828df0 _ZN18CBluetoothReliable7ConnectER10CNetworkId
// Partial: the pseudocode calls vtable slot +0x28 with `this` only. The vtable of this class puts
// IsConnectedTo(CNetworkId&) in that slot, so the call is IsConnectedTo(peer) here. The decompile
// shows no peer argument; the Bluetooth stub answers false whatever it is given. Fails with -1
// when the transport is not initialized; the stored peer itself returns 0 without a copy,
// otherwise the peer is stored.
int CBluetoothReliable::Connect(CNetworkId& peer) {
    if (!initialized_) {
        return -1;
    }
    if (IsConnectedTo(peer) || &peer == &peer_) {
        return 0;
    }
    peer_.Set(peer);
    return 0;
}

// 0x828e58 _ZN18CBluetoothReliable4OpenESsSsi
// Partial, as CBluetoothUnreliable::Open: the pseudocode stores one unnamed field (+0x04) and
// copies the name into a temporary that is freed at once. The port keeps neither and returns 0.
int CBluetoothReliable::Open(std::string address, std::string name, int channel) {
    (void)address;
    (void)name;
    (void)channel;
    return 0;
}

// 0x828ec8 _ZN18CBluetoothReliable6ListenESsSs
// Partial: forwards to Open. The channel argument of the forwarded call is not recovered (the
// pseudocode leaves it unset), so 0 is passed.
int CBluetoothReliable::Listen(std::string address, std::string name) {
    return Open(address, name, 0);
}

// 0x8290dc _ZN18CBluetoothReliable16GetPeerNetworkIdEv
// Partial: the pseudocode returns an id whose field3 is 0 with its presence bit (flags 0x8) set.
// CNetworkId has no accessor for field3, so the port returns a reset id (no presence bit).
CNetworkId CBluetoothReliable::GetPeerNetworkId() {
    CNetworkId id;
    id.Reset();
    return id;
}

// 0x829694 _ZN18CBluetoothReliable17GetLocalNetworkIdEv
// Partial: same as GetPeerNetworkId (field3 presence bit not set; see there).
CNetworkId CBluetoothReliable::GetLocalNetworkId() {
    CNetworkId id;
    id.Reset();
    return id;
}

// 0x829400 _ZN18CBluetoothReliable6CreateESsSsi
// Partial, as CBluetoothUnreliable::Create: allocates and initialises a transport, then calls Open
// with the second argument first. The pseudocode's string copies are garbled, so the argument
// order and the channel value are not recovered; the int argument is passed through as given.
CBluetoothReliable* CBluetoothReliable::Create(std::string first, std::string second, int channel) {
    auto* transport = new CBluetoothReliable();
    transport->Initialize();
    transport->Open(second, first, channel);
    return transport;
}

// 0x8294cc _ZN18CBluetoothReliable19InitializeTransportEv
// Partial: the pseudocode asks the transport manager (vtable +0x1c) for the session name of the
// "Default Bluetooth Session" transport, creates it through Create, initialises it, registers it
// and sets s_initialized. The manager call and its arguments are not recovered, so the port
// returns 0 and leaves the flag unset.
int CBluetoothReliable::InitializeTransport() {
    return 0;
}

// ---- CBluetoothUnreliable ----

// 0x8296bc _ZN20CBluetoothUnreliable7GetTypeEv
int CBluetoothUnreliable::GetType() const {
    return 3;
}

// 0x8296c4 _ZN20CBluetoothUnreliable17CanDisconnectFromER10CNetworkId
bool CBluetoothUnreliable::CanDisconnectFrom(CNetworkId& peer) {
    (void)peer;
    return false;
}

// 0x8296cc _ZN20CBluetoothUnreliable10InitializeEv
int CBluetoothUnreliable::Initialize() {
    if (initialized_) {
        return -1;
    }
    initialized_ = true;
    return 0;
}

// 0x8296e8 _ZN20CBluetoothUnreliable10DisconnectEv
int CBluetoothUnreliable::Disconnect() {
    if (!initialized_) {
        return -1;
    }
    initialized_ = false;
    return 0;
}

// 0x829704 _ZN20CBluetoothUnreliable11IsConnectedEv
bool CBluetoothUnreliable::IsConnected() const {
    return false;
}

// 0x82970c _ZN20CBluetoothUnreliable13IsConnectedToER10CNetworkId
bool CBluetoothUnreliable::IsConnectedTo(const CNetworkId& peer) const {
    (void)peer;
    return false;
}

// 0x829714 _ZN20CBluetoothUnreliable6IsPeerEv
bool CBluetoothUnreliable::IsPeer() const {
    return false;
}

// 0x82971c _ZN20CBluetoothUnreliable8IsClientEv
bool CBluetoothUnreliable::IsClient() const {
    return false;
}

// 0x829724 _ZN20CBluetoothUnreliable11IsListeningEv
bool CBluetoothUnreliable::IsListening() const {
    return initialized_ && socket_.IsServer() && (flags_ & 1) != 0;
}

// 0x829764 _ZN20CBluetoothUnreliable14StartBroadcastEv
void CBluetoothUnreliable::StartBroadcast() {}

// 0x829768 _ZN20CBluetoothUnreliable13StopBroadcastEv
void CBluetoothUnreliable::StopBroadcast() {}

// 0x82976c _ZN20CBluetoothUnreliable4SendER10CNetworkIdPvi
int CBluetoothUnreliable::Send(CNetworkId& peer, const void* data, int len) {
    (void)peer;
    (void)data;
    (void)len;
    return -1;
}

// 0x829774 _ZN20CBluetoothUnreliable7ReceiveEv
int CBluetoothUnreliable::Receive() {
    return -1;
}

// 0x82977c _ZN20CBluetoothUnreliable7SetModeE19_tBLUETOOTH_SESSION
void CBluetoothUnreliable::SetMode(int session) {
    (void)session;
}

// 0x829780 _ZN20CBluetoothUnreliable18TerminateTransportEv
int CBluetoothUnreliable::TerminateTransport() {
    s_initialized_ = false;
    return 0;
}

// 0x829980 _ZN20CBluetoothUnreliableC2Ev
// 0x829c2c _ZN20CBluetoothUnreliableC1Ev is the same body (skipped at that address).
// Reconstructed: the peer id is reset and the other members take their defaults (flags 0, the
// initialized and accept flags false). The two std::string members of the original (device
// name copies at +0x34 and +0x50) are not modelled: they are always empty here.
CBluetoothUnreliable::CBluetoothUnreliable() {
    peer_.Reset();
}

// 0x829b5c _ZN20CBluetoothUnreliableC2Ejb
// 0x829ecc _ZN20CBluetoothUnreliableC1Ejb is the same body (skipped at that address).
// Partial: the channel and listen arguments are not used by the pseudocode, so they are ignored.
// The original does not set the initialized flag here; the default value is used instead.
CBluetoothUnreliable::CBluetoothUnreliable(unsigned int channel, bool listen) : CBluetoothUnreliable() {
    (void)channel;
    (void)listen;
}

// 0x829a78 _ZN20CBluetoothUnreliableD1Ev
// 0x829af8 _ZN20CBluetoothUnreliableD2Ev is the same body (skipped at that address).
// Reconstructed: RestartBluetooth runs first, then the members are destroyed by the compiler.
CBluetoothUnreliable::~CBluetoothUnreliable() {
    CBluetoothSocket::RestartBluetooth();
}

// 0x8297f8 _ZN20CBluetoothUnreliable7ConnectER10CNetworkId
// Reconstructed. Fails with -1 when the transport is not initialized. A peer that is already
// connected, or the stored peer itself, returns 0 without a copy. Otherwise the peer is stored.
// The original calls IsConnectedTo through vtable +0x28; the call is direct here.
int CBluetoothUnreliable::Connect(CNetworkId& peer) {
    if (!initialized_) {
        return -1;
    }
    if (IsConnectedTo(peer) || &peer == &peer_) {
        return 0;
    }
    peer_.Set(peer);
    return 0;
}

// 0x8297a8 _ZN20CBluetoothUnreliable17AcceptConnectionsEv
// Partial: accept_enabled_ (+0x30) is not set by any recovered code. The socket check is vtable
// +0x5c of CBluetoothSocket, which is IsServer(). AcceptCallback is the transport-manager
// stand-in (a no-op), called with transport type 3.
int CBluetoothUnreliable::AcceptConnections() {
    if (accept_enabled_ && socket_.IsServer() && (flags_ & 1) != 0) {
        GetTransportMgr().AcceptCallback(3, this);
    }
    return 0;
}

// 0x829860 _ZN20CBluetoothUnreliable4OpenESsSsi
// Partial: the pseudocode stores one unnamed field (+0x04, taken from the address string's
// internals, meaning not identified) and copies the name into a temporary that is freed at once.
// The port keeps neither and returns 0.
int CBluetoothUnreliable::Open(std::string address, std::string name, int channel) {
    (void)address;
    (void)name;
    (void)channel;
    return 0;
}

// 0x8298d0 _ZN20CBluetoothUnreliable6ListenESsSs
// Partial: forwards to Open. The channel argument of the forwarded call is not recovered (the
// pseudocode leaves it unset), so 0 is passed. Open has no observable effect here.
int CBluetoothUnreliable::Listen(std::string address, std::string name) {
    return Open(address, name, 0);
}

// 0x829a50 _ZN20CBluetoothUnreliable16GetPeerNetworkIdEv
// Partial: the pseudocode returns an id whose field2 is 0 with its presence bit (flags 0x4) set.
// CNetworkId has no accessor for field2, so the port returns a reset id (no presence bit).
CNetworkId CBluetoothUnreliable::GetPeerNetworkId() {
    CNetworkId id;
    id.Reset();
    return id;
}

// 0x829f9c _ZN20CBluetoothUnreliable17GetLocalNetworkIdEv
// Partial: same as GetPeerNetworkId (field2 presence bit not set; see there).
CNetworkId CBluetoothUnreliable::GetLocalNetworkId() {
    CNetworkId id;
    id.Reset();
    return id;
}

// 0x829cfc _ZN20CBluetoothUnreliable6CreateESsSsi
// Partial: allocates and initialises a transport, then calls Open with the arguments in the
// order the pseudocode uses (second, first, channel). The channel value is not recovered (the
// decompile never assigns it), so the int argument is passed through as given.
CBluetoothUnreliable* CBluetoothUnreliable::Create(std::string first, std::string second, int channel) {
    auto* transport = new CBluetoothUnreliable();
    transport->Initialize();
    transport->Open(second, first, channel);
    return transport;
}

// 0x829dc8 _ZN20CBluetoothUnreliable19InitializeTransportEv
// Partial: the pseudocode creates the "Default Bluetooth Session" transport through Create,
// with arguments that the decompile does not fully recover (a string from a transport-manager
// call and an unset channel), registers it with the transport manager and sets s_initialized.
// The port does not create the transport: it returns 0 and leaves the flag unset.
int CBluetoothUnreliable::InitializeTransport() {
    return 0;
}
