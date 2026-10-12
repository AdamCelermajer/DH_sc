// Test for batch e2-net-transport-NetStruct-9cfb4c-0 (packet history and serialize forwarders).
// The test links only the batch source and net_bit_stream.cpp. Symbols outside this batch are
// stubbed below: the NetStruct constructor and destructor, the vtable's Load, Erase and GetSizeBits,
// NetStruct::SerializeInternal (not reconstructed), NetStruct::Resend() (partial), and the change
// counter, which is defined in tb_transport-NetStructMember-0.cpp.
#include "net-transport/net_struct.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

std::uint64_t NetStruct::s_changeCounter = 0;

static int resend_calls = 0;
static int serialize_calls = 0;
static bool last_conditional = false;
static int last_member = 0;
static int last_packet = 0;

NetStruct::NetStruct() {}
NetStruct::~NetStruct() {}
int NetStruct::Load(NetBitStream&, int, std::uint32_t) { return 0; }
int NetStruct::Erase(NetBitStream&) { return 0; }
int NetStruct::GetSizeBits() { return 0; }
int NetStruct::Resend() { ++resend_calls; return 1234; }
int NetStruct::SerializeInternal(bool conditional, NetBitStream&, int memberId, int packetId) {
    ++serialize_calls;
    last_conditional = conditional;
    last_member = memberId;
    last_packet = packetId;
    return 555;
}

// Exposes the protected packet history to the test.
struct Probe : NetStruct {
    std::size_t total() const { return packetHistory_.size(); }
    std::size_t count(int member) { return packetHistory_[member].size(); }
    bool has(int member, int packet) { return packetHistory_[member].count(packet) != 0; }
    tPacketHistory get(int member, int packet) { return packetHistory_[member][packet]; }
};

int main() {
    // AddPacketHistory stores the change counter and the mask; the return value is the mask as int.
    {
        Probe p;
        NetStruct::s_changeCounter = 42;
        const int r = p.AddPacketHistory(3, 7, 0, 0xABCDEF0123456789ull);
        CHECK(p.get(3, 7).changeStamp == 42);
        CHECK(p.get(3, 7).changeMask == 0xABCDEF0123456789ull);
        CHECK(r == 0x23456789);

        NetStruct::s_changeCounter = 50;
        p.AddPacketHistory(3, 8, 0, 5);
        CHECK(p.count(3) == 2);
        CHECK(p.get(3, 8).changeStamp == 50 && p.get(3, 8).changeMask == 5);

        // Adding the same packet id again replaces the mask and the stamp of that entry.
        p.AddPacketHistory(3, 7, 0, 9);
        CHECK(p.count(3) == 2);
        CHECK(p.get(3, 7).changeMask == 9);
        CHECK(p.get(3, 7).changeStamp == 50);
    }

    // ProcessLostPacket erases only the entry for that packet.
    {
        Probe p;
        p.AddPacketHistory(3, 7, 0, 1);
        p.AddPacketHistory(3, 8, 0, 2);
        p.ProcessLostPacket(3, 7);
        CHECK(!p.has(3, 7));
        CHECK(p.has(3, 8));
        p.ProcessLostPacket(3, 99);  // no such packet: nothing erased
        CHECK(p.count(3) == 1);
    }

    // With no history at all, ProcessLostPacket returns before creating an entry.
    {
        Probe p;
        p.ProcessLostPacket(5, 1);
        CHECK(p.total() == 0);
    }

    // An unknown member with existing history: operator[] creates an empty entry, nothing is erased.
    {
        Probe p;
        p.AddPacketHistory(3, 7, 0, 1);
        p.ProcessLostPacket(4, 7);
        CHECK(p.total() == 2);
        CHECK(p.count(4) == 0);
        CHECK(p.has(3, 7));
    }

    // ProcessAcknowledgedPacket erases the entry the same way.
    {
        Probe p;
        p.AddPacketHistory(2, 5, 0, 1);
        p.ProcessAcknowledgedPacket(2, 5);
        CHECK(!p.has(2, 5));
        CHECK(p.count(2) == 0);
    }

    // More than 64 packets for one member: the history is cleared and Resend() is returned.
    {
        Probe p;
        for (int i = 0; i < 65; ++i) {
            p.AddPacketHistory(1, i, 0, static_cast<std::uint64_t>(i));
        }
        CHECK(p.count(1) == 65);
        const int before = resend_calls;
        const int r = p.AddPacketHistory(1, 100, 0, 7);
        CHECK(resend_calls == before + 1);
        CHECK(r == 1234);
        CHECK(p.count(1) == 0);
    }

    // The serialize entry points forward to SerializeInternal with the original constant arguments.
    {
        Probe p;
        NetBitStream stream(8);
        serialize_calls = 0;

        int r = p.Serialize(stream, 4, 9);
        CHECK(r == 555 && serialize_calls == 1);
        CHECK(last_conditional && last_member == 4 && last_packet == 9);

        r = p.SerializeDisabled(stream);
        CHECK(r == 555);
        CHECK(!last_conditional && last_member == -1 && last_packet == -1);

        r = p.ConditionalSerialize(false, stream, 2, 3);
        CHECK(r == 555 && !last_conditional && last_member == 2 && last_packet == 3);

        r = p.ConditionalSerialize(true, stream, 6, 7);
        CHECK(last_conditional && last_member == 6 && last_packet == 7);
        CHECK(serialize_calls == 4);
    }

    if (failures == 0) {
        std::printf("e2-net-transport-NetStruct-9cfb4c-0 test: all checks passed\n");
        return 0;
    }
    std::printf("%d check(s) failed\n", failures);
    return 1;
}
