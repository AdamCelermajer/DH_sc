#include "net-transport/packet_manager.hpp"

CPacketManager* CPacketManager::s_packetManager = nullptr;
tPacketSlot CPacketManager::s_PacketSlots[CPacketManager::kPacketSlotCount];
std::uint8_t CPacketManager::s_packetSlotBitMap = 0;

// 0x8162a4 _ZN14CPacketManagerC1Ev, 0x816540 _ZN14CPacketManagerC2Ev
// Partial: sets up the state (queue, mutex, initialized flag). The original also registers the
// receive callback with CConnectionManager (packet type 6), initialises CNetworkEmulation and
// creates the CDataStats members, none of which are in this port yet.
CPacketManager::CPacketManager() {
    initialized_ = true;
}

// 0x816ab8 _ZN14CPacketManagerD2Ev, 0x81704c _ZN14CPacketManagerD1Ev
// Partial: releases the queue. The original also unregisters packet type 6 and tears down the
// CNetworkEmulation and CDataStats members, which this port does not have yet.
CPacketManager::~CPacketManager() {
    initialized_ = false;
    packets_.clear();
}

// 0x8151c8 _ZN14CPacketManager11GetInstanceEv
CPacketManager* CPacketManager::GetInstance() {
    return s_packetManager;
}

// 0x8151ec _ZN14CPacketManager13IsInitializedEv
bool CPacketManager::IsInitialized() {
    return s_packetManager != nullptr && s_packetManager->initialized_;
}

// 0x8164e8 _ZN14CPacketManager10InitializeEv
// Partial: the constructor is partial (see above), so the object is initialised without the
// CConnectionManager registration.
int CPacketManager::Initialize() {
    if (s_packetManager != nullptr) {
        return 0;
    }
    s_packetManager = new CPacketManager();
    return s_packetManager != nullptr ? 0 : -1;
}

// 0x815214 _ZN14CPacketManager9TerminateEv
// The original calls the deleting destructor through vtable slot +4, then clears the pointer.
int CPacketManager::Terminate() {
    if (s_packetManager != nullptr) {
        delete s_packetManager;
        s_packetManager = nullptr;
    }
    return 0;
}

// 0x815258 _ZN14CPacketManager18RegisterPacketSlotE12PACKET_SLOTSPFbiiR12NetBitStreamEPFviiS2_EPFviiES8_
// A slot that is already in use is left unchanged, as in the original.
int CPacketManager::RegisterPacketSlot(PACKET_SLOTS slot, bool (*write)(int, int, NetBitStream&),
                                       void (*read)(int, int, NetBitStream&),
                                       void (*ack)(int, int), void (*lost)(int, int)) {
    tPacketSlot& entry = s_PacketSlots[slot];
    if (!entry.used) {
        entry.used = true;
        entry.write = write;
        entry.read = read;
        entry.ack = ack;
        entry.lost = lost;
        s_packetSlotBitMap = static_cast<std::uint8_t>(s_packetSlotBitMap | (1u << slot));
    }
    return 0;
}

// 0x8152c0 _ZN14CPacketManager20UnregisterPacketSlotE12PACKET_SLOTS
int CPacketManager::UnregisterPacketSlot(PACKET_SLOTS slot) {
    s_PacketSlots[slot] = tPacketSlot{};
    s_packetSlotBitMap = static_cast<std::uint8_t>(s_packetSlotBitMap & ~(1u << slot));
    return s_packetSlotBitMap;
}

// 0x815324 _ZN14CPacketManager16IsPacketSlotUsedE12PACKET_SLOTS
bool CPacketManager::IsPacketSlotUsed(PACKET_SLOTS slot) {
    return s_PacketSlots[slot].used;
}

// 0x81534c _ZN14CPacketManager14GetPacketCountEv
int CPacketManager::GetPacketCount() const {
    return static_cast<int>(packets_.size());
}

// 0x815378 _ZN14CPacketManager19ArePacketsAvailableEv
bool CPacketManager::ArePacketsAvailable() const {
    return !packets_.empty();
}

// 0x8153a8 _ZN14CPacketManager18SequenceMoreRecentEjj
// The original compares its first argument (passed as `this`) with the second; the third
// argument is unused.
bool CPacketManager::SequenceMoreRecent(std::uint32_t a, std::uint32_t b) {
    if (a > b && a - b <= 0x8000u) {
        return true;
    }
    if (a < b) {
        return b - a > 0x8000u;
    }
    return false;
}

// 0x8153e0 _ZN14CPacketManager17GetSequenceOffsetEjj
// The difference is folded into the signed 16-bit range.
int CPacketManager::GetSequenceOffset(std::uint32_t a, std::uint32_t b) {
    std::int32_t result = static_cast<std::int32_t>(a - b);
    if (static_cast<std::int32_t>(a - b) >= -32768) {
        if (result > 0x8000) {
            result -= 0x10000;
        }
    } else {
        result += 0x10000;
    }
    return result;
}

// 0x8153fc _ZN14CPacketManager15PrintStatisticsEv
void CPacketManager::PrintStatistics() const {}

// 0x815400 _ZN14CPacketManager10ReadHeaderER12NetBitStream
CPacketManager::tPacketMgrHeader CPacketManager::ReadHeader(NetBitStream& stream) {
    tPacketMgrHeader header;
    header.sequence = stream.ReadU32(16);
    header.ack = stream.ReadU32(16);
    header.ack_bits = stream.ReadU32(32);
    header.matching_id = stream.ReadU32(16);
    header.connection_id = stream.ReadU32(16);
    header.slot_mask = static_cast<std::uint8_t>(stream.ReadU32(8));
    return header;
}

// 0x815474 _ZN14CPacketManager11WriteHeaderERNS_16tPacketMgrHeaderER12NetBitStream
void CPacketManager::WriteHeader(const tPacketMgrHeader& header, NetBitStream& stream) {
    stream.WriteU32(header.sequence, 16);
    stream.WriteU32(header.ack, 16);
    stream.WriteU32(header.ack_bits, 32);
    stream.WriteU32(header.matching_id, 16);
    stream.WriteU32(header.connection_id, 16);
    stream.WriteU32(header.slot_mask, 8);
}

// 0x816c68 _ZN14CPacketManager13GetNextPacketEv
NetBitStream CPacketManager::GetNextPacket() {
    NetBitStream packet(packets_.front());
    packets_.pop_front();
    return packet;
}

// 0x817c9c _ZN14CPacketManager22PacketReceiverCallbackEiPci
// The stream buffer is at least 4088 bytes (as in the original). The source id is unused.
int CPacketManager::PacketReceiverCallback(int source, char* data, int size) {
    (void)source;
    mutex_.Lock();
    std::uint32_t capacity = size >= 4088 ? static_cast<std::uint32_t>(size) : 4088u;
    NetBitStream stream(capacity);
    stream.SetBuffer(data, static_cast<std::size_t>(size));
    packets_.push_back(stream);
    return mutex_.Unlock();
}
