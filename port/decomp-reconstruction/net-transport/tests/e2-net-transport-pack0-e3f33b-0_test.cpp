// Test for batch e2-net-transport-pack0-e3f33b-0: NetStructInt<6u> TestValue, Write and Read.
#include <cstdint>
#include <cstdio>

#include "net-transport/net_bit_stream.hpp"
#include "net-transport/tb/cnet_player_info.hpp"
#include "net-transport/tb/net_struct_int.hpp"

namespace {

int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

}  // namespace

int main() {
    NetStructInt<6u> member;

    // TestValue: magnitudes up to 0x1f pass, 0x20 and above fail (5 magnitude bits).
    CHECK(member.TestValue(0));
    CHECK(member.TestValue(31));
    CHECK(member.TestValue(-31));
    CHECK(!member.TestValue(32));
    CHECK(!member.TestValue(-32));

    // Write then Read round trip. The stream is written, copied into a reader, and read back.
    const std::int32_t values[] = {0, 1, -1, 5, -5, 31, -31};
    for (std::int32_t v : values) {
        NetBitStream out(64);
        member.SetValue(v);
        member.Write(out);

        NetBitStream in(64);
        in.WriteStream(out);

        NetStructInt<6u> reader;
        reader.Read(in);
        CHECK(reader.GetValue() == v);
    }

    // Write layout: 5 magnitude bits, then one sign bit (1 for negative). Value -3 gives the
    // bits 00011 (magnitude) then 1 (sign): six bits in all.
    {
        NetBitStream out(16);
        member.SetValue(-3);
        member.Write(out);
        NetBitStream in(16);
        in.WriteStream(out);
        CHECK(in.ReadU32(5) == 3u);
        CHECK(in.ReadByte(1) == 1u);
    }

    // CNetPlayerInfo constructor (partial): the fields the class declares end at the values the
    // original leaves after its constructor and Reset().
    {
        CNetPlayerInfo info;
        CHECK(info.member_id_ == 0);
        CHECK(info.conn_index_ == -1);
        CHECK(info.unk_1c8 == -1);
        CHECK(info.state_ == 0);
        CHECK(info.applied_state_ == 0);
        CHECK(!info.IsActive());
    }

    // Create and Delete (partial): Create returns a fresh object, Delete accepts null and frees one.
    {
        CNetPlayerInfo* created = CNetPlayerInfo::Create();
        CHECK(created != nullptr);
        CHECK(created->state_ == 0);
        CNetPlayerInfo::Delete(created);
        CNetPlayerInfo::Delete(nullptr);
    }

    if (failures == 0) {
        std::printf("e2-net-transport-pack0-e3f33b-0 test OK\n");
        return 0;
    }
    std::printf("%d failure(s)\n", failures);
    return 1;
}
