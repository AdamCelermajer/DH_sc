// CMessaging: process-wide singleton that owns the receive and send queues and
// the acknowledgement bookkeeping. Offsets of the original 32-bit layout are
// given as comments. Reconstructed: the singleton accessors, the acknowledge and
// resend helpers, queue clearing, and the lost/acknowledged packet callbacks.
// Partial: the constructor and destructor (packet slot not registered yet) and
// ResetQueues (CMessage::m_messageSerial is not modelled). The rest of the
// slice is blocked on CMessage fields and NetBitStream internals (see NOTES.md).
#pragma once

#include <cstdint>
#include <list>
#include <map>
#include <set>

#include "net-session-online/c_message.hpp"
#include "net-session-online/deps/net_mutex.hpp"

class CMessaging {
public:
    // 0x80a710 _ZN10CMessaging13IsInitializedEv: 0 when no instance; otherwise the initialized flag at +0x04.
    static std::uint32_t IsInitialized();
    // 0x80a738 _ZN10CMessaging9TerminateEv: deletes the instance through its virtual destructor.
    static void Terminate();
    // 0x80a77c _ZN10CMessaging6UpdateEv: the original returns 0 with no work.
    static int Update();
    // 0x80b114 _ZN10CMessaging10InitializeEv: creates the singleton once. 0 on success, -1 on failure.
    static int Initialize();
    // 0x80b16c _ZN10CMessaging11GetInstanceEv: returns the singleton, creating it on first use.
    static CMessaging* GetInstance();

    // 0x80afe4 _ZN10CMessagingC1Ev (partial: packet slot 1 is not registered).
    CMessaging();
    // 0x80b438 _ZN10CMessagingD1Ev (partial: packet slot 1 is not unregistered).
    virtual ~CMessaging();

    // 0x80a8d4 _ZN10CMessaging18ResendLostMessagesEiRSt3setItSt4lessItESaItEE
    void ResendLostMessages(int memberId, std::set<std::uint16_t>& hashes);
    // 0x80a98c _ZN10CMessaging18AcknowledgeMessageEiP8CMessage
    void AcknowledgeMessage(int memberId, CMessage* message);
    // 0x80a994 _ZN10CMessaging22AcknowledgeSentMessagesEiRSt3setItSt4lessItESaItEE
    void AcknowledgeSentMessages(int memberId, std::set<std::uint16_t>& hashes);
    // 0x80ac34 _ZN10CMessaging18ClearMessageQueueEPSt4listIP8CMessageSaIS2_EEP9CNetMutex
    void ClearMessageQueue(std::list<CMessage*>* queue, CNetMutex* mutex);
    // 0x80ae90 _ZN10CMessaging11ResetQueuesEb (partial: no CMessage::m_messageSerial reset).
    void ResetQueues(bool resetMaps);
    // 0x80cbbc _ZN10CMessaging17ProcessLostPacketEii
    void ProcessLostPacket(int outerKey, int innerKey);
    // 0x80cd18 _ZN10CMessaging23ProcessAcknowledgedPacketEii
    void ProcessAcknowledgedPacket(int outerKey, int innerKey);
    // 0x80ccec _ZN10CMessaging18sProcessLostPacketEii: packet-manager callback, forwards to the singleton.
    static void sProcessLostPacket(int outerKey, int innerKey);
    // 0x80ce48 _ZN10CMessaging24sProcessAcknowledgedPacketEii: packet-manager callback, forwards to the singleton.
    static void sProcessAcknowledgedPacket(int outerKey, int innerKey);

    // Test hook: installs an instance for IsInitialized/Terminate tests.
    static void SetInstanceForTest(CMessaging* instance, bool initialized);

private:
    static CMessaging* s_messaging;  // CMessaging::s_messaging

    bool initialized_ = false;       // +0x04
    CNetMutex recvMutex_;            // +0x08 guards recvQueue_
    CNetMutex sendMutex_;            // +0x0c guards sendQueue_
    CNetMutex unusedMutex_;          // +0x10 not used by the reconstructed code
    CNetMutex ackMutex_;             // +0x14 taken by ResetQueues
    CNetMutex seqMutex_;             // +0x18 taken by ResetQueues
    std::list<CMessage*> recvQueue_;  // +0x1c
    std::list<CMessage*> sendQueue_;  // +0x24
    std::set<std::uint16_t> hashSet_;  // +0x2c message hashes; role inferred
    std::map<int, int> seqMapA_;     // +0x44; role inferred
    std::map<int, int> seqMapB_;     // +0x5c; role inferred
    // +0x74: packet id -> (sequence -> message hashes). Outer key first, then inner key.
    std::map<int, std::map<int, std::set<std::uint16_t>>> packetMap_;
};
