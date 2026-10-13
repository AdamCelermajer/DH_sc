#include "net-transport/packet_manager.hpp"

#include <chrono>
#include <iterator>

#include "net-transport/connection_manager.hpp"
#include "net-transport/net_network_emulation.hpp"  // CNetworkEmulation::Initialize / Terminate (static)
#include "net-session-online/deps/matching.hpp"  // CMatching (one definition, net-session-online)

namespace {

// Original time source: GetOnline() vtable slot 0, a 32-bit millisecond clock. Here steady_clock
// in ms, truncated to 32 bits (as in connection_manager.cpp).
std::uint32_t PacketNowMs() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<std::uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

// Static `validFlag` of SendPackets (mark-and-sweep generation of member_info_ entries).
std::uint8_t send_packets_valid_flag = 0;

// Key of a sent packet: (member << 16) + sequence. The member id is kept to 16 bits, as the original.
std::uint32_t SentKey(std::uint32_t member, std::uint32_t sequence) {
    return sequence + (static_cast<std::uint32_t>(static_cast<std::uint16_t>(member)) << 16);
}

}  // namespace

CPacketManager* CPacketManager::s_packetManager = nullptr;
tPacketSlot CPacketManager::s_PacketSlots[CPacketManager::kPacketSlotCount];
std::uint8_t CPacketManager::s_packetSlotBitMap = 0;

// 0x8162a4 _ZN14CPacketManagerC1Ev, 0x816540 _ZN14CPacketManagerC2Ev
// Registers the receive callback for packet type 6 with CConnectionManager, then under the mutex
// initialises CNetworkEmulation (static, so the Hex-Rays argument is not used), sets the initialized
// flag and unlocks. The original's GetConnectionMgr() call is a pure getter whose result is not used
// here. Before the registration the original also clears the queue, the sorted containers and the
// send-side maps; the members are constructed empty, so those clears do nothing and are not repeated.
// The members are default-initialised, including the four CDataStats<int> (see the header).
// sPacketReceiverCallback returns int, while the registry takes a void callback: the lambda discards
// the result, which CConnectionManager's dispatch ignores too.
CPacketManager::CPacketManager() {
    CConnectionManager::RegisterPacketType(6, 0, [](int member, char* data, int size) {
        (void)CPacketManager::sPacketReceiverCallback(member, data, size);
    });
    mutex_.Lock();
    CNetworkEmulation::Initialize();
    initialized_ = true;
    mutex_.Unlock();
}

// 0x816ab8 _ZN14CPacketManagerD2Ev, 0x81704c _ZN14CPacketManagerD1Ev
// Unregisters packet type 6, tears down CNetworkEmulation (static) and clears the initialized flag.
// The members are destroyed after this body, in reverse declaration order: the four CDataStats<int>
// first, as in the original, then the +0x70 map, the acknowledged list and the sent-time map. The
// order of the last containers (member map, waiting map, incoming list) differs from the original only
// among containers with no observable side effect.
CPacketManager::~CPacketManager() {
    CConnectionManager::UnregisterPacketType(6);
    CNetworkEmulation::Terminate();
    initialized_ = false;
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
// partial: the original allocates the object with operator new(0x118, 2) and constructs it before the
// null check. The second argument (allocator tag) is not recovered. The port uses the default
// operator new, which throws instead of returning null, so the -1 result is not reachable.
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

// 0x815504 _ZN14CPacketManager26ProcessAcknowledgedPacketsEv
// Runs the ack callback of every used slot for each acknowledged key, then empties the list. The list
// is read by index and its size is taken again after every key, so a callback that adds a key is
// served in the same pass, as in the original. The original's CConnectionManager::ReportStatistic call
// after each key goes to an empty hook (0x7fc094 is a bare return), so it is omitted.
void CPacketManager::ProcessAcknowledgedPackets() {
    for (std::size_t i = 0; i < acked_.size(); ++i) {
        const std::uint32_t key = acked_[i];
        const int member = static_cast<int>(key >> 16);
        const int sequence = static_cast<int>(key & 0xFFFFu);
        for (int slot = 0; slot < kPacketSlotCount; ++slot) {
            const tPacketSlot& entry = s_PacketSlots[slot];
            if (entry.used && entry.ack != nullptr) {
                entry.ack(member, sequence);
            }
        }
    }
    acked_.clear();
}

// 0x815cac _ZN14CPacketManager17AcknowledgePacketERNS_16tPacketMgrHeaderE
// Records a received packet in the sender's bit field. The original returns the member entry,
// which no caller uses. The shift follows ARM LSL semantics: a shift of 32 or more gives 0.
void CPacketManager::AcknowledgePacket(tPacketMgrHeader& header) {
    tPacketMemberInfo& info = member_info_[static_cast<int>(header.matching_id)];
    const int offset = GetSequenceOffset(info.last_recv_sequence, header.sequence);
    const std::int8_t narrow = static_cast<std::int8_t>(offset);
    if (offset < 0) {
        info.last_recv_sequence = header.sequence;
        const std::uint8_t shift = static_cast<std::uint8_t>(-narrow);
        info.ack_bits = shift < 32 ? (info.ack_bits << shift) : 0u;
        info.ack_bits |= 1u;
    } else if (offset <= 31) {
        info.ack_bits |= 1u << narrow;
    }
    info.ack_bits &= 0xFFFFu;  // the original stores the low 16 bits back
}

// 0x815d80 _ZN14CPacketManager13IsPacketValidERNS_16tPacketMgrHeaderE
// A packet is new when it is addressed to this member and its sequence is not yet in the bit field.
bool CPacketManager::IsPacketValid(const tPacketMgrHeader& header) {
    if (static_cast<int>(header.connection_id) != CMatching::Get()->LocalMemberId()) {
        return false;
    }
    const tPacketMemberInfo& info = member_info_[static_cast<int>(header.matching_id)];
    const int offset = GetSequenceOffset(info.last_recv_sequence, header.sequence);
    if (offset < 0) {
        return true;
    }
    if (offset > 31) {
        return false;
    }
    return (info.ack_bits & (1u << offset)) == 0;
}

// 0x815e10 _ZN14CPacketManager19PreparePacketHeaderEiR12NetBitStream
// Writes the header for a packet to `member`. Returns the sequence number that was written.
std::uint32_t CPacketManager::PreparePacketHeader(int member, NetBitStream& stream) {
    const tPacketMemberInfo& info = member_info_[member];
    tPacketMgrHeader header;
    header.sequence = static_cast<std::uint16_t>(info.last_sent_sequence + 1);
    header.ack = info.last_recv_sequence;
    header.ack_bits = info.ack_bits;
    header.connection_id = static_cast<std::uint32_t>(member);
    header.matching_id = static_cast<std::uint32_t>(CMatching::Get()->LocalMemberId());
    header.slot_mask = s_packetSlotBitMap;
    WriteHeader(header, stream);
    return header.sequence;
}

// 0x815ec8 _ZN14CPacketManager17PreparePacketDataEijR12NetBitStream
// Each slot writes one flag bit. A registered slot that the member has also registered writes
// [flag 1][tag 32 bits][payload][block length 16 bits], where the tag is the slot index times
// 0x01020304 (the original's running counter) and the length is the bit count of the block. If the
// payload fails, the stream reports a write overflow (flags at +0x1c are non-zero, which the original
// tests as a whole word) or the packet passes 0x578 bytes, the block is reverted and the flag is 0.
// The member's slot mask is read only when the slot is used and has a write callback, as the original
// short-circuits its tests.
void CPacketManager::PreparePacketData(int member, std::uint32_t sequence, NetBitStream& stream) {
    std::uint32_t tag = 0;
    for (int slot = 0; slot < kPacketSlotCount; ++slot, tag += 0x01020304u) {
        const tPacketSlot& entry = s_PacketSlots[slot];
        if (!entry.used || entry.write == nullptr ||
            ((member_info_[member].slot_mask >> slot) & 1u) == 0) {
            stream.WriteByte(0, 1);
            continue;
        }
        const std::uint32_t saved_size = stream.Size();
        const std::uint32_t saved_write = stream.WritePosition();
        stream.SetRevertPoint();
        stream.WriteByte(1, 1);
        stream.WriteU32(tag, 32);
        if (!entry.write(member, static_cast<int>(sequence), stream) || stream.Flags() != 0) {
            stream.Revert();
            stream.WriteByte(0, 1);
            continue;
        }
        const std::uint32_t write_pos = stream.WritePosition();
        const std::uint32_t bytes = (write_pos >> 3) + ((write_pos & 7u) != 0 ? 1u : 0u);
        if (bytes > 0x578u) {
            stream.Revert();
            stream.WriteByte(0, 1);
            continue;
        }
        const std::int64_t block_bits = 8 * static_cast<std::int64_t>(saved_size) - saved_write -
                                        (8 * static_cast<std::int64_t>(stream.Size()) - write_pos);
        stream.WriteU32(static_cast<std::uint32_t>(block_bits), 16);
    }
}

// 0x81684c _ZN14CPacketManager18SendWaitingPacketsEv
// Sends the queued packets in key order and removes each one after it is sent.
// partial: when the CConnectionManager singleton is missing the queue is left as it is; the
// original dereferences the singleton. tSendPacket keeps only member, data and bit count.
void CPacketManager::SendWaitingPackets() {
    if (waiting_.empty()) {
        return;
    }
    CConnectionManager* connections = CConnectionManager::GetInstance();
    if (connections == nullptr) {
        return;
    }
    do {
        auto first = waiting_.begin();
        tSendPacket& packet = first->second;
        const std::uint32_t bytes = (packet.bits >> 3) + ((packet.bits & 7u) != 0 ? 1u : 0u);
        connections->SendTo(6, packet.member, packet.data.data(), static_cast<int>(bytes));
        waiting_.erase(first);
    } while (!waiting_.empty());
}

// 0x8168fc _ZN14CPacketManager18ProcessLostPacketsEv
// A sent packet older than 1000 ms is lost: every used slot's lost callback runs and the entry is
// removed. An entry whose member is no longer known is removed and the scan stops, as the original does.
// partial: the clock is steady_clock. The original reads the time through the GetOnline() object's
// vtable slot 0, which is not recovered. The CConnectionManager::ReportStatistic call after each lost
// packet goes to an empty hook (0x7fc094 is a bare return), so it is omitted.
void CPacketManager::ProcessLostPackets() {
    auto it = sent_times_.begin();
    while (it != sent_times_.end()) {
        const auto next = std::next(it);
        const int member = static_cast<int>(it->first >> 16);
        const int sequence = static_cast<int>(it->first & 0xFFFFu);
        if (member_info_.find(member) == member_info_.end()) {
            sent_times_.erase(it);
            return;
        }
        if (static_cast<std::int32_t>(PacketNowMs() - it->second) > 1000) {
            for (int slot = 0; slot < kPacketSlotCount; ++slot) {
                const tPacketSlot& entry = s_PacketSlots[slot];
                if (entry.used && entry.lost != nullptr) {
                    entry.lost(member, sequence);
                }
            }
            sent_times_.erase(it);
        }
        it = next;
    }
}

// 0x816ca8 _ZN14CPacketManager29ProcessPacketAcknowledgementsERNS_16tPacketMgrHeaderE
// Bit i of ack_bits acknowledges sequence (ack - i) of the sender. A sent packet found for that
// sequence moves to acked_ and leaves sent_times_.
void CPacketManager::ProcessPacketAcknowledgements(const tPacketMgrHeader& header) {
    const std::uint32_t member_part = static_cast<std::uint32_t>(static_cast<std::uint16_t>(header.matching_id)) << 16;
    std::uint16_t sequence = static_cast<std::uint16_t>(header.ack);
    for (std::uint32_t bits = header.ack_bits; bits != 0; bits >>= 1) {
        if ((bits & 1u) != 0) {
            const std::uint32_t key = sequence + member_part;
            const auto found = sent_times_.find(key);
            if (found != sent_times_.end()) {
                acked_.push_back(key);
                sent_times_.erase(found);
            }
        }
        sequence = static_cast<std::uint16_t>(sequence - 1);
    }
}

// 0x816eac _ZN14CPacketManager14ReceivePacketsEv
// Reads queued packets under the mutex. The sender's slot mask is stored before the validity test.
// A packet that is new for this member is read slot by slot and then acknowledged. A slot that is
// unknown here ends the call without an acknowledgement. Returns the result of the mutex unlock.
// The CConnectionManager::ReportStatistic call after each header read goes to an empty hook
// (0x7fc094 is a bare return), so it is omitted.
int CPacketManager::ReceivePackets() {
    mutex_.Lock();
    if (!ArePacketsAvailable()) {
        return mutex_.Unlock();
    }
    while (true) {
        NetBitStream stream = GetNextPacket();
        tPacketMgrHeader header = ReadHeader(stream);
        member_info_[static_cast<int>(header.matching_id)].slot_mask = header.slot_mask;
        if (IsPacketValid(header)) {
            ProcessPacketAcknowledgements(header);
            int slot = 0;
            while (slot < kPacketSlotCount) {
                if (stream.ReadByte(1) == 0) {
                    ++slot;
                    continue;
                }
                stream.ReadU32(32);  // slot tag: read, not checked (as the original)
                const tPacketSlot& entry = s_PacketSlots[slot];
                if (!entry.used || entry.read == nullptr) {
                    return mutex_.Unlock();
                }
                entry.read(static_cast<int>(header.matching_id), static_cast<int>(header.sequence), stream);
                ++slot;
                stream.ReadU32(16);  // block length: read, not checked (as the original)
            }
            AcknowledgePacket(header);
        }
        if (!ArePacketsAvailable()) {
            return mutex_.Unlock();
        }
    }
}

// 0x817814 _ZN14CPacketManager10SendPacketEi
// Sends one packet to a connected member. Returns 1 when the packet was built (even when it is too
// large to send), 0 when the member is not connected. The sent time is recorded per (member, sequence).
// partial: the sent time is steady_clock in ms. The original stores the GetOnline() vtable slot 0 time,
// which is not recovered. A missing CConnectionManager singleton returns 0 here; the original
// dereferences it. The ReportStatistic call after the sent-time insert goes to an empty hook
// (0x7fc094 is a bare return), so it is omitted.
int CPacketManager::SendPacket(int member) {
    CConnectionManager* connections = CConnectionManager::GetInstance();
    if (connections == nullptr || !connections->IsConnected(member)) {
        return 0;
    }
    NetBitStream stream(0xFF8u);
    const std::uint32_t sequence = PreparePacketHeader(member, stream);
    PreparePacketData(member, sequence, stream);
    const std::uint32_t now = PacketNowMs();
    member_info_[member].last_sent_sequence = sequence;
    sent_times_[SentKey(static_cast<std::uint32_t>(member), sequence)] = now;
    const std::uint32_t bits = stream.WritePosition();
    const std::uint32_t bytes = (bits >> 3) + ((bits & 7u) != 0 ? 1u : 0u);
    if (bytes <= 0x578u) {
        std::vector<char> buffer(bytes);
        stream.GetBuffer(buffer.data(), bytes);
        connections->SendTo(6, member, buffer.data(), static_cast<int>(bytes));
    }
    return 1;
}

// 0x8179d0 _ZN14CPacketManager11SendPacketsEv
// Runs the acknowledgement and loss passes, then, when forced or after 50 ms, sends one packet to
// each member in the list and sweeps the member map for members that were not sent to. The sweep
// flag toggles on every sweep. Returns -1 when the manager is not initialised, 0 otherwise.
// The member sweep is the original's: every member that was not sent to is removed, and the flag
// toggles on every sweep (the original's running flag is always the current generation).
// partial: the send interval uses the steady clock, while the original reads the COnline time field at
// +0x1c, which COnline does not model. A missing CConnectionManager singleton sends nothing, where the
// original dereferences it.
int CPacketManager::SendPackets() {
    if (!initialized_) {
        return -1;
    }
    ProcessAcknowledgedPackets();
    ProcessLostPackets();
    const std::uint32_t now = PacketNowMs();
    if (force_send_ || static_cast<std::uint32_t>(now - last_send_time_) > 50u) {
        bool sent_any = false;
        CConnectionManager* connections = CConnectionManager::GetInstance();
        if (connections != nullptr) {
            for (int member : connections->GetConnMemberIdList(false)) {
                if (SendPacket(member) != 0) {
                    sent_any = true;
                }
                const auto info = member_info_.find(member);
                if (info != member_info_.end()) {
                    info->second.valid_flag = send_packets_valid_flag;
                }
            }
        }
        for (auto it = member_info_.begin(); it != member_info_.end();) {
            if (it->second.valid_flag != send_packets_valid_flag) {
                it = member_info_.erase(it);
            } else {
                ++it;
            }
        }
        send_packets_valid_flag = static_cast<std::uint8_t>(send_packets_valid_flag ^ 1u);
        if (sent_any) {
            last_send_time_ = now;
        }
    }
    force_send_ = false;
    SendWaitingPackets();
    PrintStatistics();
    return 0;
}

// 0x817d1c _ZN14CPacketManager23sPacketReceiverCallbackEiPci
// Passes data from a connected member to the packet manager. Returns 0 when the member is not
// connected.
// partial: a missing CConnectionManager or CPacketManager singleton returns 0 here; the original
// dereferences them.
int CPacketManager::sPacketReceiverCallback(int member, char* data, int size) {
    CConnectionManager* connections = CConnectionManager::GetInstance();
    if (connections == nullptr || !connections->IsConnected(member) || s_packetManager == nullptr) {
        return 0;
    }
    return s_packetManager->PacketReceiverCallback(member, data, size);
}
