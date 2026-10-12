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

// 0x80aa50 _ZN10CMessaging23AreMessagesAcknowledgedEPKc
// With no name: 0 when the send queue is empty, otherwise 1. With a name the original
// searches the send queue for an unflagged message of that wire type and returns its
// +0x3c flag. Partial: the search needs the wire type id (+0x10) and the flag, which
// CMessage does not expose, so every name reports 1 (no match).
int CMessaging::AreMessagesAcknowledged(const char* typeName) {
    if (typeName == nullptr) {
        return sendQueue_.empty() ? 0 : 1;
    }
    sendMutex_.Lock();
    sendMutex_.Unlock();
    return 1;
}

// 0x80ab00 _ZN10CMessaging19GetMessageFromQueueEPKc
// Partial: the original returns the first unflagged receive-queue message whose wire type
// id matches FindTypeId(typeName). The type id and flag are not reachable, so no match.
CMessage* CMessaging::GetMessageFromQueue(const char* typeName) {
    (void)typeName;
    recvMutex_.Lock();
    recvMutex_.Unlock();
    return nullptr;
}

// 0x80ab80 _ZN10CMessaging17IsMessageReceivedEPKc
bool CMessaging::IsMessageReceived(const char* typeName) {
    return GetMessageFromQueue(typeName) != nullptr;
}

// 0x80acb8 _ZN10CMessaging22PurgeMessagesFromQueueEPSt4listIP8CMessageSaIS2_EEP9CNetMutexPKc
// Partial: the original also removes (and deletes) entries whose +0x3c flag is set. The
// flag is not reachable, so only null entries are removed. The original unlocks twice;
// one unlock is made here, as in ClearMessageQueue.
void CMessaging::PurgeMessagesFromQueue(std::list<CMessage*>* queue, CNetMutex* mutex) {
    mutex->Lock();
    for (auto it = queue->begin(); it != queue->end();) {
        if (*it == nullptr) {
            it = queue->erase(it);
        } else {
            ++it;
        }
    }
    mutex->Unlock();
}

// 0x80ad54 _ZN10CMessaging16ProcessRecvQueueEv
// Partial: the original sets the +0x3c flag of each expired receive-queue message, then
// purges. The flag write is not reconstructed, so the purge removes only null entries.
void CMessaging::ProcessRecvQueue() {
    recvMutex_.Lock();
    for (CMessage* message : recvQueue_) {
        if (message != nullptr && message->IsExpired()) {
            // Original: flag at +0x3c set to 1 (not reachable from CMessaging).
        }
    }
    recvMutex_.Unlock();
    PurgeMessagesFromQueue(&recvQueue_, &recvMutex_);
}

// 0x80b900 _ZN10CMessaging12PackMessagesEiR12NetBitStream
// Writes each eligible send-queue message as a 1 bit followed by its serialized form, then
// a 0 bit. Returns the hashes of the written messages. Partial: the original skips messages
// whose +0x3c flag is set, and stops before a message that would not fit in the buffer; neither
// is reconstructed (no flag accessor, and the stand-in stream has no bit-position API).
std::set<std::uint16_t> CMessaging::PackMessages(int memberId, NetBitStream& stream) {
    std::set<std::uint16_t> hashes;
    sendMutex_.Lock();
    for (CMessage* message : sendQueue_) {
        if (message == nullptr || message->IsAcknowledgedBy(memberId) || message->HasBeenSentTo(memberId)) {
            continue;
        }
        if (!message->IsFor(memberId) && CMatching::Get()->IsServer()) {
            continue;
        }
        stream.WriteByte(1, 1);
        message->Serialize(stream);
        hashes.insert(static_cast<std::uint16_t>(message->GetHash()));
        message->SetSentFlag(memberId, true);
    }
    stream.WriteByte(0, 1);
    sendMutex_.Unlock();
    return hashes;
}

// 0x80ba78 _ZN10CMessaging18AddMissingMessagesEiii
// Adds the hashes of the sequence numbers strictly between fromSequence and toSequence.
// The Hex-Rays third argument of CMessage::CreateHash is a leftover; CreateHash takes two.
void CMessaging::AddMissingMessages(int memberId, int fromSequence, int toSequence) {
    for (int sequence = fromSequence + 1; sequence < toSequence; ++sequence) {
        hashSet_.insert(static_cast<std::uint16_t>(CMessage::CreateHash(memberId, sequence)));
    }
}

// 0x80cf1c _ZN10CMessaging15WritePacketDataEiiR12NetBitStream
// Writes a 0 bit when the send queue is empty (returns 0). Otherwise writes a 1 bit, packs
// the messages into packetMap_[outerKey][innerKey] and returns 1. Inherits the partial
// PackMessages.
int CMessaging::WritePacketData(int outerKey, int innerKey, NetBitStream& stream) {
    if (sendQueue_.empty()) {
        stream.WriteByte(0, 1);
        return 0;
    }
    stream.WriteByte(1, 1);
    auto& packed = packetMap_[outerKey][innerKey];
    packed = PackMessages(outerKey, stream);
    return 1;
}

// 0x80cfdc _ZN10CMessaging16sWritePacketDataEiiR12NetBitStream: packet-manager callback, forwards to the singleton.
int CMessaging::sWritePacketData(int outerKey, int innerKey, NetBitStream& stream) {
    return s_messaging->WritePacketData(outerKey, innerKey, stream);
}

// 0x80d624 _ZN10CMessaging14AddToRecvQueueEP8CMessage
// Partial: the original calls ProcessMissingMessages(message) before taking the lock. That
// function is not reconstructed, so the call is omitted.
void CMessaging::AddToRecvQueue(CMessage* message) {
    recvMutex_.Lock();
    recvQueue_.push_back(message);
    recvMutex_.Unlock();
}

// 0x80db24 _ZN10CMessaging14AddToSendQueueEP8CMessage
// The message is acknowledged for the local member, appended to the send queue, and the queue
// is sorted by send priority (the Hex-Rays body is an inlined std::list::sort on
// CMessage::CompareMsgSendPriority). Partial: for a local message the original hands it to
// ReceiveMessage and returns before queueing. ReceiveMessage is not reconstructed, so local
// messages are queued here.
void CMessaging::AddToSendQueue(CMessage* message) {
    sendMutex_.Lock();
    message->AcknowledgeMember(CMatching::Get()->LocalMemberId());
    sendQueue_.push_back(message);
    sendQueue_.sort(&CMessage::CompareMsgSendPriority);
    sendMutex_.Unlock();
}

// 0x80e1e8 _ZN10CMessaging15SendMsgToServerEP8CMessage
void CMessaging::SendMsgToServer(CMessage* message) {
    CMatching* matching = CMatching::Get();
    message->SetDestinationMask(matching->MemberBit(matching->ServerMemberId()));
    AddToSendQueue(message);
}

// 0x80e23c _ZN10CMessaging9SendMsgToEP8CMessagei
void CMessaging::SendMsgTo(CMessage* message, int memberId) {
    if (CMatching::Get()->LocalMemberId() == memberId) {
        AddToRecvQueue(message);
        return;
    }
    message->SetDestinationMask(CMatching::Get()->MemberBit(memberId));
    AddToSendQueue(message);
}

// 0x80e2a4 _ZN10CMessaging7SendMsgEP8CMessage
void CMessaging::SendMsg(CMessage* message) {
    message->SetDestinationMask(0xFFFFFFFFu);
    AddToSendQueue(message);
}

void CMessaging::SetInstanceForTest(CMessaging* instance, bool initialized) {
    s_messaging = instance;
    if (instance != nullptr) {
        instance->initialized_ = initialized;
    }
}
