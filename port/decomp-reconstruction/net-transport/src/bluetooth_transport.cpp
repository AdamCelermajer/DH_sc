#include "net-transport/bluetooth_transport.hpp"

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
