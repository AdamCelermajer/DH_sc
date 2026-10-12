// CBluetoothReliable and CBluetoothUnreliable: Bluetooth transports. There is no
// Bluetooth stack on Windows, and the original Android build's leaf bodies return fixed
// values, so the port keeps those. Only the leaf functions are reconstructed here; Connect,
// Open, Listen, Create, InitializeTransport, the constructors and the destructors are not yet.
//
// Original layout shared by both (32-bit offsets):
//   +0x00 vptr (CTransport base)
//   +0x08 CBluetoothSocket
//   +0x2c flags (bit 0: listening)
//   +0x68 initialized flag (set by Initialize, cleared by Disconnect)
#pragma once

#include <cstdint>

#include "net-transport/bluetooth_socket.hpp"
#include "net-transport/net_network_id.hpp"
#include "net-transport/transport.hpp"

class CBluetoothReliable : public CTransport {
public:
    int GetType() const;                             // 0x828cb4 _ZN18CBluetoothReliable7GetTypeEv (returns 4)
    bool CanDisconnectFrom(CNetworkId& peer);        // 0x828cbc _ZN18CBluetoothReliable17CanDisconnectFromER10CNetworkId
    int Initialize();                                // 0x828cc4 _ZN18CBluetoothReliable10InitializeEv
    int Disconnect() override;                       // 0x828ce0 _ZN18CBluetoothReliable10DisconnectEv
    bool IsConnected() const;                        // 0x828cfc _ZN18CBluetoothReliable11IsConnectedEv
    bool IsConnectedTo(const CNetworkId& peer) const;  // 0x828d04 _ZN18CBluetoothReliable13IsConnectedToER10CNetworkId
    bool IsPeer() const;                             // 0x828d0c _ZN18CBluetoothReliable6IsPeerEv
    bool IsClient() const;                           // 0x828d14 _ZN18CBluetoothReliable8IsClientEv
    bool IsListening() const;                        // 0x828d1c _ZN18CBluetoothReliable11IsListeningEv
    void StartBroadcast() override;                  // 0x828d5c _ZN18CBluetoothReliable14StartBroadcastEv
    void StopBroadcast() override;                   // 0x828d60 _ZN18CBluetoothReliable13StopBroadcastEv
    int Send(CNetworkId& peer, const void* data, int len);  // 0x828d64 _ZN18CBluetoothReliable4SendER10CNetworkIdPvi (returns -1)
    int Receive();                                   // 0x828d6c _ZN18CBluetoothReliable7ReceiveEv (returns -1)
    void SetMode(int session);                       // 0x828d74 _ZN18CBluetoothReliable7SetModeE19_tBLUETOOTH_SESSION
    static int TerminateTransport();                 // 0x828d78 _ZN18CBluetoothReliable18TerminateTransportEv

private:
    CBluetoothSocket socket_;                        // +0x08
    std::uint32_t flags_ = 0;                        // +0x2c
    bool initialized_ = false;                       // +0x68
    static bool s_initialized_;                      // _ZN18CBluetoothReliable13s_initializedE
};

class CBluetoothUnreliable : public CTransport {
public:
    int GetType() const;                             // 0x8296bc _ZN20CBluetoothUnreliable7GetTypeEv (returns 3)
    bool CanDisconnectFrom(CNetworkId& peer);        // 0x8296c4 _ZN20CBluetoothUnreliable17CanDisconnectFromER10CNetworkId
    int Initialize();                                // 0x8296cc _ZN20CBluetoothUnreliable10InitializeEv
    int Disconnect() override;                       // 0x8296e8 _ZN20CBluetoothUnreliable10DisconnectEv
    bool IsConnected() const;                        // 0x829704 _ZN20CBluetoothUnreliable11IsConnectedEv
    bool IsConnectedTo(const CNetworkId& peer) const;  // 0x82970c _ZN20CBluetoothUnreliable13IsConnectedToER10CNetworkId
    bool IsPeer() const;                             // 0x829714 _ZN20CBluetoothUnreliable6IsPeerEv
    bool IsClient() const;                           // 0x82971c _ZN20CBluetoothUnreliable8IsClientEv
    bool IsListening() const;                        // 0x829724 _ZN20CBluetoothUnreliable11IsListeningEv
    void StartBroadcast() override;                  // 0x829764 _ZN20CBluetoothUnreliable14StartBroadcastEv
    void StopBroadcast() override;                   // 0x829768 _ZN20CBluetoothUnreliable13StopBroadcastEv
    int Send(CNetworkId& peer, const void* data, int len);  // 0x82976c _ZN20CBluetoothUnreliable4SendER10CNetworkIdPvi (returns -1)
    int Receive();                                   // 0x829774 _ZN20CBluetoothUnreliable7ReceiveEv (returns -1)
    void SetMode(int session);                       // 0x82977c _ZN20CBluetoothUnreliable7SetModeE19_tBLUETOOTH_SESSION
    static int TerminateTransport();                 // 0x829780 _ZN20CBluetoothUnreliable18TerminateTransportEv

private:
    CBluetoothSocket socket_;                        // +0x08
    std::uint32_t flags_ = 0;                        // +0x2c
    bool initialized_ = false;                       // +0x68
    static bool s_initialized_;                      // _ZN20CBluetoothUnreliable13s_initializedE
};
