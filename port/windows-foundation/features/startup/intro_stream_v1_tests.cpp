#include "inflate_raw_v1.hpp"
#include "intro_stream_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using namespace dh::foundation::startup;

static int failures = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                          \
        }                                                                        \
    } while (0)

using Bytes = std::vector<std::uint8_t>;

// LSB-first bit writer for hand-built deflate streams.
struct BitWriter {
    Bytes out;
    unsigned bit = 0;
    void put(unsigned value, unsigned count) {  // value bits, LSB first
        for (unsigned i = 0; i < count; ++i) {
            if (bit == 0) out.push_back(0);
            if ((value >> i) & 1u) out.back() |= std::uint8_t(1u << bit);
            bit = (bit + 1) & 7u;
        }
    }
    void put_code(unsigned code, unsigned len) {  // Huffman codes are MSB first
        for (unsigned i = len; i-- > 0;) put((code >> i) & 1u, 1);
    }
    // Fixed Huffman literal/length symbol.
    void put_fixed_sym(unsigned sym) {
        if (sym <= 143) put_code(0x30 + sym, 8);
        else if (sym <= 255) put_code(0x190 + sym - 144, 9);
        else if (sym <= 279) put_code(sym - 256, 7);
        else put_code(0xC0 + sym - 280, 8);
    }
};

static Bytes stored_block(const Bytes& data) {
    Bytes out{0x01, std::uint8_t(data.size() & 0xff), std::uint8_t(data.size() >> 8),
              std::uint8_t(~data.size() & 0xff), std::uint8_t((~data.size() >> 8) & 0xff)};
    out.insert(out.end(), data.begin(), data.end());
    return out;
}

static bool inflate(const Bytes& in, std::size_t expected, Bytes& out, std::string& error) {
    return inflate_raw_v1(in.data(), in.size(), out, expected, error);
}

static void test_inflate() {
    std::string e;
    Bytes out;
    // Stored block "abc".
    CHECK(inflate(stored_block({'a', 'b', 'c'}), 3, out, e) && std::string(out.begin(), out.end()) == "abc");
    // Raw deflate of "a" (fixed Huffman, produced by zlib).
    CHECK(inflate({0x4b, 0x04, 0x00}, 1, out, e) && out.size() == 1 && out[0] == 'a');
    // Literal 'a' then match length 3 distance 1 -> "aaaa" (fixed Huffman).
    {
        BitWriter w;
        w.put(1, 1);  // final
        w.put(1, 2);  // fixed
        w.put_fixed_sym('a');
        w.put_fixed_sym(257);  // length 3
        w.put_code(0, 5);      // distance code 0 -> distance 1
        w.put_fixed_sym(256);
        CHECK(inflate(w.out, 4, out, e) && std::string(out.begin(), out.end()) == "aaaa");
    }
    // Match length 258 (symbol 285, no extra bits) with distance 1.
    {
        BitWriter w;
        w.put(1, 1);
        w.put(1, 2);
        w.put_fixed_sym('z');
        w.put_fixed_sym(285);
        w.put_code(0, 5);
        w.put_fixed_sym(256);
        CHECK(inflate(w.out, 259, out, e) && out.size() == 259 && out.back() == 'z');
    }
    // Failure: distance before start of output.
    {
        BitWriter w;
        w.put(1, 1);
        w.put(1, 2);
        w.put_fixed_sym('a');
        w.put_fixed_sym(257);
        w.put_code(4, 5);  // distance code 4 -> base 5, beyond the 1 byte written
        w.put(0, 1);
        w.put_fixed_sym(256);
        CHECK(!inflate(w.out, 10, out, e) && e.find("distance") != std::string::npos);
    }
    // Failure: output would exceed the expected size.
    CHECK(!inflate({0x4b, 0x04, 0x00}, 0, out, e) && e.find("exceeds") != std::string::npos);
    // Failure: truncated stream (dropping the final byte of 'a' encoding).
    CHECK(!inflate({0x4b}, 1, out, e));
    // Failure: stored block length check.
    CHECK(!inflate({0x01, 0x03, 0x00, 0x00, 0x00, 'a', 'b', 'c'}, 3, out, e) &&
          e.find("length") != std::string::npos);
    // Failure: reserved block type 3.
    CHECK(!inflate({0x07}, 1, out, e));
    // Failure: empty input.
    CHECK(!inflate({}, 1, out, e));
}

// Builds a 2x1 RGB565 container with two frames (stored blocks) for a delta test.
static Bytes build_container(std::uint32_t w, std::uint32_t h, std::uint32_t fps,
                             const std::vector<Bytes>& frames, bool badMagic = false) {
    Bytes file;
    const char magic[9] = "DH2INTR1";
    file.insert(file.end(), magic, magic + 8);
    if (badMagic) file[0] = 'X';
    auto u32 = [&](std::uint32_t v) {
        for (int i = 0; i < 4; ++i) file.push_back(std::uint8_t(v >> (8 * i)));
    };
    u32(1);
    u32(w);
    u32(h);
    u32(fps);
    u32(std::uint32_t(frames.size()));
    std::vector<Bytes> encoded;
    std::uint32_t offset = 0;
    for (const auto& f : frames) {
        encoded.push_back(stored_block(f));
    }
    for (const auto& e : encoded) {
        u32(offset);
        u32(std::uint32_t(e.size()));
        offset += std::uint32_t(e.size());
    }
    for (const auto& e : encoded) file.insert(file.end(), e.begin(), e.end());
    return file;
}

static void test_stream() {
    // Frame 0 delta = frame0 values (previous is zero): red 0xF800, green 0x07E0.
    // Frame 1 delta = frame1 ^ frame0: pixels become blue 0x001F and white 0xFFFF.
    const std::uint16_t f0[2] = {0xF800, 0x07E0};
    const std::uint16_t f1[2] = {0x001F, 0xFFFF};
    const std::uint16_t d1[2] = {std::uint16_t(f1[0] ^ f0[0]), std::uint16_t(f1[1] ^ f0[1])};
    auto le = [](const std::uint16_t* v) {
        Bytes b;
        for (int i = 0; i < 2; ++i) { b.push_back(std::uint8_t(v[i] & 0xff)); b.push_back(std::uint8_t(v[i] >> 8)); }
        return b;
    };
    const Bytes file = build_container(2, 1, 24, {le(f0), le(d1)});
    IntroStreamV1 stream;
    std::string e;
    CHECK(stream.open(file, e));
    CHECK(stream.info().width == 2 && stream.info().height == 1 && stream.info().fps == 24 &&
          stream.info().frame_count == 2);
    Bytes rgba;
    CHECK(stream.next_frame(rgba, e));
    CHECK(rgba.size() == 8);
    // Red 0xF800 -> (255,0,0); green 0x07E0 -> (0,255,0); alpha 255.
    CHECK(rgba[0] == 255 && rgba[1] == 0 && rgba[2] == 0 && rgba[3] == 255);
    CHECK(rgba[4] == 0 && rgba[5] == 255 && rgba[6] == 0 && rgba[7] == 255);
    CHECK(stream.next_frame(rgba, e));
    CHECK(rgba[0] == 0 && rgba[1] == 0 && rgba[2] == 255);     // blue
    CHECK(rgba[4] == 255 && rgba[5] == 255 && rgba[6] == 255);  // white
    CHECK(stream.at_end());
    CHECK(!stream.next_frame(rgba, e));  // end of stream is not an error the host must crash on
    stream.rewind();
    CHECK(!stream.at_end());
    CHECK(stream.next_frame(rgba, e) && rgba[0] == 255);  // rewind restarts from frame 0 with zero previous

    // Rejections.
    CHECK(!stream.open(build_container(2, 1, 24, {le(f0)}, true), e) && e.find("magic") != std::string::npos);
    CHECK(!stream.open(Bytes(10, 0), e));
    CHECK(!stream.open(build_container(0, 1, 24, {le(f0)}), e));
    CHECK(!stream.open(build_container(2, 1, 0, {le(f0)}), e));
    {
        Bytes truncated = file;
        truncated.resize(40);  // inside the frame table
        CHECK(!stream.open(truncated, e) && e.find("truncated") != std::string::npos);
    }
    {
        Bytes cut = file;
        cut.pop_back();  // last frame now extends past the end of the file
        CHECK(!stream.open(cut, e) && e.find("outside") != std::string::npos);
    }
    {
        // Corrupt frame: the stream ends with an error instead of showing garbage.
        // The frame's inflated payload is 6 bytes, but a 2x1 RGB565 frame expects 4.
        Bytes corruptFrame = build_container(2, 1, 24, {Bytes{0x01, 0x05, 0x00, 0xFA, 0xFF, 1}});
        CHECK(stream.open(corruptFrame, e));
        CHECK(!stream.next_frame(rgba, e) && e.find("frame 0") != std::string::npos);
        CHECK(stream.at_end());
    }
}

static void test_rgb565() {
    const std::uint16_t px[3] = {0x0000, 0xFFFF, 0xF800};
    std::vector<std::uint8_t> rgba;
    rgb565_to_rgba8(px, 3, rgba);
    CHECK(rgba.size() == 12);
    CHECK(rgba[0] == 0 && rgba[1] == 0 && rgba[2] == 0 && rgba[3] == 255);
    CHECK(rgba[4] == 255 && rgba[5] == 255 && rgba[6] == 255 && rgba[7] == 255);
    CHECK(rgba[8] == 255 && rgba[9] == 0 && rgba[10] == 0);
}

int main() {
    test_inflate();
    test_stream();
    test_rgb565();
    if (failures == 0) std::puts("intro_stream_v1 tests: all passed");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
