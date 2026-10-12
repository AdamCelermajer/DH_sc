// CPacketManager: packet slots, sequence numbers and the incoming packet queue.
// Reconstructed from libDungeonHunter2.so pseudocode (CPacketManager.cpp).
//
// Only the self-contained part is here: slot registry, sequence arithmetic, header
// (de)serialisation, the incoming queue and the singleton accessors. The send and
// acknowledge paths depend on CConnectionManager and CMatching, which are not in this
// port yet; they are not reconstructed in this file.
#pragma once

#include <cstdint>
#include <list>

#include "net-transport/net_bit_stream.hpp"
#include "net-transport/sync.hpp"

// PACKET_SLOTS: slot index 0..7, names not recovered (kept numeric).
using PACKET_SLOTS = int;

// One registered packet type. Original stride 92 bytes (32-bit build), fields:
//   +0x00 used flag, +0x04 write callback, +0x08 read callback, +0x0c ack callback, +0x10 lost callback.
struct tPacketSlot {
    bool used = false;                                    // +0x00
    bool (*write)(int, int, NetBitStream&) = nullptr;     // +0x04
    void (*read)(int, int, NetBitStream&) = nullptr;      // +0x08
    void (*ack)(int, int) = nullptr;                      // +0x0c
    void (*lost)(int, int) = nullptr;                     // +0x10
};

class CPacketManager {
public:
    // Packet header, written in this order: sequence (16 bits), ack (16), ack_bits (32),
    // matching_id (16), connection_id (16), slot_mask (8). The original ReadHeader and
    // WriteHeader use the same order.
    struct tPacketMgrHeader {
        std::uint32_t sequence = 0;        // +0x00
        std::uint32_t ack = 0;             // +0x04
        std::uint32_t ack_bits = 0;        // +0x08
        std::uint32_t connection_id = 0;   // +0x0c
        std::uint32_t matching_id = 0;     // +0x10
        std::uint8_t slot_mask = 0;        // +0x14
    };

    CPacketManager();                                             // 0x8162a4 C1, 0x816540 C2 (partial)
    virtual ~CPacketManager();                                    // 0x816ab8 D1, 0x81704c D2 (partial)
    CPacketManager(const CPacketManager&) = delete;
    CPacketManager& operator=(const CPacketManager&) = delete;

    static CPacketManager* GetInstance();                         // 0x8151c8 _ZN14CPacketManager11GetInstanceEv
    static bool IsInitialized();                                  // 0x8151ec _ZN14CPacketManager13IsInitializedEv
    static int Initialize();                                      // 0x8164e8 _ZN14CPacketManager10InitializeEv (partial)
    static int Terminate();                                       // 0x815214 _ZN14CPacketManager9TerminateEv

    static int RegisterPacketSlot(PACKET_SLOTS slot, bool (*write)(int, int, NetBitStream&),
                                  void (*read)(int, int, NetBitStream&),
                                  void (*ack)(int, int), void (*lost)(int, int));  // 0x815258
    static int UnregisterPacketSlot(PACKET_SLOTS slot);           // 0x8152c0
    static bool IsPacketSlotUsed(PACKET_SLOTS slot);              // 0x815324

    int GetPacketCount() const;                                   // 0x81534c
    bool ArePacketsAvailable() const;                             // 0x815378

    // Sequence arithmetic on 16-bit sequence numbers (wrap-around at 0x8000).
    static bool SequenceMoreRecent(std::uint32_t a, std::uint32_t b);   // 0x8153a8
    static int GetSequenceOffset(std::uint32_t a, std::uint32_t b);     // 0x8153e0

    void PrintStatistics() const;                                 // 0x8153fc (empty in the original)

    static tPacketMgrHeader ReadHeader(NetBitStream& stream);     // 0x815400
    static void WriteHeader(const tPacketMgrHeader& header, NetBitStream& stream);  // 0x815474

    NetBitStream GetNextPacket();                                 // 0x816c68 (caller checks ArePacketsAvailable)
    int PacketReceiverCallback(int source, char* data, int size); // 0x817c9c

    static constexpr int kPacketSlotCount = 8;                    // 8 * 92 bytes = 736 (0x2e0) in the original

    static CPacketManager* s_packetManager;                       // _ZN14CPacketManager16s_packetManagerE
    static tPacketSlot s_PacketSlots[kPacketSlotCount];           // _ZN14CPacketManager13s_PacketSlotsE
    static std::uint8_t s_packetSlotBitMap;                       // _ZN14CPacketManager18s_packetSlotBitMapE

private:
    bool initialized_ = false;                                    // +0x04 (set true by the constructor)
    CNetMutex mutex_;                                             // +0x08
    std::list<NetBitStream> packets_;                             // +0x24 incoming packets, in arrival order
};
