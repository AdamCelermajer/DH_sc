#include "net-session-online/c_messaging.hpp"

#include "net-session-online/deps/matching.hpp"

CMessaging* CMessaging::s_messaging = nullptr;

// 0x80a710 _ZN10CMessaging13IsInitializedEv
std::uint32_t CMessaging::IsInitialized() {
    CMessaging* instance = s_messaging;
    if (instance == nullptr) {
        return 0;
    }
    return instance->initialized_ ? 1u : 0u;
}

// 0x80a738 _ZN10CMessaging9TerminateEv
void CMessaging::Terminate() {
    CMessaging* instance = s_messaging;
    if (instance != nullptr) {
        delete instance;
        s_messaging = nullptr;
    }
}

// 0x80a77c _ZN10CMessaging6UpdateEv
int CMessaging::Update() { return 0; }

// 0x80b114 _ZN10CMessaging10InitializeEv
int CMessaging::Initialize() {
    if (s_messaging != nullptr) {
        return 0;
    }
    s_messaging = new CMessaging();
    return s_messaging != nullptr ? 0 : -1;
}

// 0x80b16c _ZN10CMessaging11GetInstanceEv
CMessaging* CMessaging::GetInstance() {
    if (s_messaging == nullptr) {
        s_messaging = new CMessaging();
    }
    return s_messaging;
}

// 0x80afe4 _ZN10CMessagingC1Ev
// Partial: the original also registers packet slot 1 with the packet manager
// (sWritePacketData, sReadPacketData, sProcessAcknowledgedPacket,
// sProcessLostPacket). The write and read callbacks are not reconstructed yet,
// so no registration is made here.
CMessaging::CMessaging() {
    initialized_ = false;
    ResetQueues(true);
    initialized_ = true;
}

// 0x80b438 _ZN10CMessagingD1Ev
// Partial: the original also calls CPacketManager::UnregisterPacketSlot(1),
// which pairs with the registration omitted in the constructor.
CMessaging::~CMessaging() {
    initialized_ = false;
    ResetQueues(true);
}

// 0x80a8d4 _ZN10CMessaging18ResendLostMessagesEiRSt3setItSt4lessItESaItEE
// Every send-queue message whose hash is in the set is marked unsent for memberId.
void CMessaging::ResendLostMessages(int memberId, std::set<std::uint16_t>& hashes) {
    sendMutex_.Lock();
    for (CMessage* message : sendQueue_) {
        const auto hash = static_cast<std::uint16_t>(message->GetHash());
        if (hashes.find(hash) != hashes.end()) {
            message->SetSentFlag(memberId, false);
        }
    }
    sendMutex_.Unlock();
}

// 0x80a98c _ZN10CMessaging18AcknowledgeMessageEiP8CMessage
void CMessaging::AcknowledgeMessage(int memberId, CMessage* message) {
    message->AcknowledgeMember(memberId);
}

// 0x80a994 _ZN10CMessaging22AcknowledgeSentMessagesEiRSt3setItSt4lessItESaItEE
// Every send-queue message whose hash is in the set is acknowledged for memberId.
void CMessaging::AcknowledgeSentMessages(int memberId, std::set<std::uint16_t>& hashes) {
    sendMutex_.Lock();
    for (CMessage* message : sendQueue_) {
        const auto hash = static_cast<std::uint16_t>(message->GetHash());
        if (hashes.find(hash) != hashes.end()) {
            AcknowledgeMessage(memberId, message);
        }
    }
    sendMutex_.Unlock();
}

// 0x80ac34 _ZN10CMessaging18ClearMessageQueueEPSt4listIP8CMessageSaIS2_EEP9CNetMutex
// The original unlocks the mutex twice; a std::mutex must be unlocked once, so
// only one unlock is made here.
void CMessaging::ClearMessageQueue(std::list<CMessage*>* queue, CNetMutex* mutex) {
    mutex->Lock();
    for (CMessage* message : *queue) {
        delete message;
    }
    queue->clear();
    mutex->Unlock();
}

// 0x80ae90 _ZN10CMessaging11ResetQueuesEb
// Partial: the original also sets CMessage::m_messageSerial to 0 on this path.
// That static is not modelled in c_message.hpp, so it is not reset here.
void CMessaging::ResetQueues(bool resetMaps) {
    ClearMessageQueue(&sendQueue_, &sendMutex_);
    ClearMessageQueue(&recvQueue_, &recvMutex_);
    hashSet_.clear();

    ackMutex_.Lock();
    seqMutex_.Lock();
    packetMap_.clear();
    if (resetMaps) {
        seqMapA_.clear();
        seqMapB_.clear();
    }
    seqMutex_.Unlock();
    ackMutex_.Unlock();
}

// 0x80cbbc _ZN10CMessaging17ProcessLostPacketEii
// If packetMap_[outerKey] has innerKey, resend the hashes stored under it and
// remove that entry. Looking up outerKey inserts it when absent, as the original does.
void CMessaging::ProcessLostPacket(int outerKey, int innerKey) {
    if (packetMap_.empty()) {
        return;
    }
    auto& inner = packetMap_[outerKey];
    if (inner.find(innerKey) == inner.end()) {
        return;
    }
    ResendLostMessages(outerKey, inner[innerKey]);
    auto& again = packetMap_[outerKey];
    auto it = again.find(innerKey);
    if (it != again.end()) {
        again.erase(it);
    }
}

// 0x80cd18 _ZN10CMessaging23ProcessAcknowledgedPacketEii
// Same walk as ProcessLostPacket, but the stored hashes are acknowledged.
void CMessaging::ProcessAcknowledgedPacket(int outerKey, int innerKey) {
    if (packetMap_.empty()) {
        return;
    }
    auto& inner = packetMap_[outerKey];
    if (inner.find(innerKey) == inner.end()) {
        return;
    }
    AcknowledgeSentMessages(outerKey, inner[innerKey]);
    auto& again = packetMap_[outerKey];
    auto it = again.find(innerKey);
    if (it != again.end()) {
        again.erase(it);
    }
}

// 0x80ccec _ZN10CMessaging18sProcessLostPacketEii
void CMessaging::sProcessLostPacket(int outerKey, int innerKey) {
    s_messaging->ProcessLostPacket(outerKey, innerKey);
}

// 0x80ce48 _ZN10CMessaging24sProcessAcknowledgedPacketEii
void CMessaging::sProcessAcknowledgedPacket(int outerKey, int innerKey) {
    s_messaging->ProcessAcknowledgedPacket(outerKey, innerKey);
}

void CMessaging::SetInstanceForTest(CMessaging* instance, bool initialized) {
    s_messaging = instance;
    if (instance != nullptr) {
        instance->initialized_ = initialized;
    }
}
