#include "net-transport/net_bit_stream.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    {
        // Single bits, MSB first, then read them back.
        NetBitStream w(4);
        w.WriteBit(1);
        w.WriteBit(0);
        w.WriteBit(1);
        CHECK(w.WritePosition() == 3);
        CHECK(static_cast<std::uint8_t>(w.Size()) == 4);
        NetBitStream r(4);
        r.SetBuffer(reinterpret_cast<const char*>("\xA0\0\0\0"), 4);  // 1010 0000
        CHECK(r.ReadBit() == 1);
        CHECK(r.ReadBit() == 0);
        CHECK(r.ReadBit() == 1);
        CHECK(r.ReadBit() == 0);
    }
    {
        // Mixed-width integers across byte boundaries.
        NetBitStream w(32);
        w.WriteU32(0x1ABC, 13);
        w.WriteU32(0x5, 3);
        w.WriteU32(0xDEADBEEF, 32);
        CHECK(w.WritePosition() == 48);
        NetBitStream r(32);
        r.WriteStream(w);  // reads are bounded by the write position, so copy it in
        CHECK(r.ReadU32(13) == 0x1ABC);
        CHECK(r.ReadU32(3) == 0x5);
        CHECK(r.ReadU32(32) == 0xDEADBEEF);
    }
    {
        // Byte arrays and strings, with revert back to a saved point.
        NetBitStream w(64);
        w.WriteBit(1);
        w.SetRevertPoint();
        w.WriteString("hello", 0);
        w.WriteByteArray("abc", 3);
        NetBitStream r(64);
        r.WriteStream(w);
        CHECK(r.ReadBit() == 1);
        CHECK(r.ReadString() == "hello");
        char tail[4] = {};
        r.ReadByteArray(tail, 3);
        CHECK(std::strcmp(tail, "abc") == 0);
        w.Revert();
        CHECK(w.WritePosition() == 1);
        w.Reset();
        CHECK(w.WritePosition() == 0 && w.ReadPosition() == 0);
    }
    {
        // Reading past the written region sets an overflow and returns zero.
        NetBitStream w(8);
        w.WriteU32(0x7, 3);
        CHECK(w.ReadU32(8) == 0);
    }
    {
        // Copy constructor and operator= copy the size and both positions (Hex-Rays).
        NetBitStream a(8);
        a.WriteU32(0x12, 8);
        NetBitStream copy(a);
        CHECK(copy.Size() == 8 && copy.WritePosition() == 8);
        CHECK(copy == a);
        NetBitStream assigned(4);
        assigned = a;
        CHECK(assigned.Size() == 8 && assigned.WritePosition() == 8);
        CHECK(assigned == a);
    }
    std::printf("%s\n", failures ? "net_bit_stream: FAILED" : "net_bit_stream: ok");
    return failures != 0;
}
