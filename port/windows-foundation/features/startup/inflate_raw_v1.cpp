#include "inflate_raw_v1.hpp"

#include <array>

namespace dh::foundation::startup {
namespace {

constexpr int kMaxBits = 15;

struct BitReader {
    const std::uint8_t* data;
    std::size_t size;
    std::size_t pos = 0;   // byte position
    unsigned bit = 0;      // bit position inside data[pos]
    bool overrun = false;

    unsigned bits(unsigned count) {
        unsigned value = 0;
        for (unsigned i = 0; i < count; ++i) {
            if (pos >= size) { overrun = true; return 0; }
            value |= static_cast<unsigned>((data[pos] >> bit) & 1u) << i;
            if (++bit == 8) { bit = 0; ++pos; }
        }
        return value;
    }
    void align_to_byte() {
        if (bit != 0) { bit = 0; ++pos; }
    }
};

// Canonical Huffman table in the "count per length + sorted symbols" form.
struct Huffman {
    std::array<std::uint16_t, kMaxBits + 1> count{};
    std::vector<std::uint16_t> symbol;
};

// Returns 0 when complete, >0 when incomplete, <0 when over-subscribed.
int build(Huffman& h, const std::uint16_t* lengths, unsigned n) {
    h.count.fill(0);
    for (unsigned s = 0; s < n; ++s) h.count[lengths[s]]++;
    if (h.count[0] == n) return 0;  // no codes: decoding will fail if used
    int left = 1;
    for (int len = 1; len <= kMaxBits; ++len) {
        left <<= 1;
        left -= h.count[len];
        if (left < 0) return left;
    }
    std::array<std::uint16_t, kMaxBits + 2> offs{};
    for (int len = 1; len < kMaxBits; ++len) offs[len + 1] = static_cast<std::uint16_t>(offs[len] + h.count[len]);
    h.symbol.assign(n, 0);
    for (unsigned s = 0; s < n; ++s)
        if (lengths[s] != 0) h.symbol[offs[lengths[s]]++] = static_cast<std::uint16_t>(s);
    return left;
}

// Returns symbol, or -1 on an invalid code / end of input.
int decode(BitReader& r, const Huffman& h) {
    int code = 0, first = 0, index = 0;
    for (int len = 1; len <= kMaxBits; ++len) {
        code |= static_cast<int>(r.bits(1));
        if (r.overrun) return -1;
        const int count = h.count[len];
        if (code - count < first) return h.symbol[index + (code - first)];
        index += count;
        first += count;
        first <<= 1;
        code <<= 1;
    }
    return -1;
}

constexpr std::array<std::uint16_t, 29> kLengthBase{3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27,
                                                    31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
constexpr std::array<std::uint8_t, 29> kLengthExtra{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                                    2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
constexpr std::array<std::uint16_t, 30> kDistBase{1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129,
                                                  193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
                                                  8193, 12289, 16385, 24577};
constexpr std::array<std::uint8_t, 30> kDistExtra{0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
                                                  6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

bool codes(BitReader& r, const Huffman& lencode, const Huffman& distcode,
           std::vector<std::uint8_t>& out, std::size_t expected, std::string& error) {
    for (;;) {
        int sym = decode(r, lencode);
        if (sym < 0) { error = "invalid literal/length code"; return false; }
        if (sym < 256) {
            if (out.size() >= expected) { error = "output exceeds expected size"; return false; }
            out.push_back(static_cast<std::uint8_t>(sym));
        } else if (sym == 256) {
            return true;
        } else {
            sym -= 257;
            if (sym >= 29) { error = "invalid length symbol"; return false; }
            const unsigned length = kLengthBase[sym] + r.bits(kLengthExtra[sym]);
            const int dsym = decode(r, distcode);
            if (dsym < 0 || dsym >= 30) { error = "invalid distance code"; return false; }
            const unsigned dist = kDistBase[dsym] + r.bits(kDistExtra[dsym]);
            if (r.overrun) { error = "truncated stream"; return false; }
            if (dist > out.size()) { error = "distance before start of output"; return false; }
            if (out.size() + length > expected) { error = "output exceeds expected size"; return false; }
            std::size_t from = out.size() - dist;
            for (unsigned i = 0; i < length; ++i) out.push_back(out[from + i]);
        }
    }
}

bool fixed_tables(Huffman& lencode, Huffman& distcode) {
    std::array<std::uint16_t, 288> lengths{};
    unsigned s = 0;
    for (; s < 144; ++s) lengths[s] = 8;
    for (; s < 256; ++s) lengths[s] = 9;
    for (; s < 280; ++s) lengths[s] = 7;
    for (; s < 288; ++s) lengths[s] = 8;
    build(lencode, lengths.data(), 288);
    std::array<std::uint16_t, 30> dlengths{};
    dlengths.fill(5);
    build(distcode, dlengths.data(), 30);
    return true;
}

bool dynamic_tables(BitReader& r, Huffman& lencode, Huffman& distcode, std::string& error) {
    static constexpr std::array<std::uint8_t, 19> order{16, 17, 18, 0, 8, 7, 9, 6, 10, 5,
                                                        11, 4, 12, 3, 13, 2, 14, 1, 15};
    const unsigned nlen = r.bits(5) + 257;
    const unsigned ndist = r.bits(5) + 1;
    const unsigned ncode = r.bits(4) + 4;
    if (nlen > 286 || ndist > 30) { error = "bad dynamic block counts"; return false; }
    std::array<std::uint16_t, 19> lengths{};
    for (unsigned i = 0; i < ncode; ++i) lengths[order[i]] = static_cast<std::uint16_t>(r.bits(3));
    if (r.overrun) { error = "truncated stream"; return false; }
    Huffman codelen;
    if (build(codelen, lengths.data(), 19) != 0) { error = "invalid code-length code"; return false; }

    std::array<std::uint16_t, 286 + 30> all{};
    const unsigned total = nlen + ndist;
    unsigned index = 0;
    while (index < total) {
        const int sym = decode(r, codelen);
        if (sym < 0) { error = "invalid code-length symbol"; return false; }
        if (sym < 16) {
            all[index++] = static_cast<std::uint16_t>(sym);
            continue;
        }
        std::uint16_t value = 0;
        unsigned repeat = 0;
        if (sym == 16) {
            if (index == 0) { error = "repeat without previous length"; return false; }
            value = all[index - 1];
            repeat = 3 + r.bits(2);
        } else if (sym == 17) {
            repeat = 3 + r.bits(3);
        } else {
            repeat = 11 + r.bits(7);
        }
        if (r.overrun) { error = "truncated stream"; return false; }
        if (index + repeat > total) { error = "code lengths overflow"; return false; }
        while (repeat--) all[index++] = value;
    }
    if (all[256] == 0) { error = "missing end-of-block code"; return false; }
    if (build(lencode, all.data(), nlen) < 0) { error = "over-subscribed literal code"; return false; }
    if (build(distcode, all.data() + nlen, ndist) < 0) { error = "over-subscribed distance code"; return false; }
    return true;
}

} // namespace

bool inflate_raw_v1(const std::uint8_t* in, std::size_t size,
                    std::vector<std::uint8_t>& out, std::size_t expected,
                    std::string& error) {
    out.clear();
    out.reserve(expected);
    error.clear();
    if (!in && size != 0) { error = "null input"; return false; }
    BitReader r{in, size};
    for (;;) {
        const unsigned last = r.bits(1);
        const unsigned type = r.bits(2);
        if (r.overrun) { error = "truncated stream"; return false; }
        if (type == 0) {
            r.align_to_byte();
            if (r.pos + 4 > size) { error = "truncated stored block"; return false; }
            const unsigned len = in[r.pos] | (in[r.pos + 1] << 8);
            const unsigned nlen = in[r.pos + 2] | (in[r.pos + 3] << 8);
            if ((len ^ 0xffffu) != nlen) { error = "stored block length check failed"; return false; }
            r.pos += 4;
            if (r.pos + len > size) { error = "truncated stored block"; return false; }
            if (out.size() + len > expected) { error = "output exceeds expected size"; return false; }
            out.insert(out.end(), in + r.pos, in + r.pos + len);
            r.pos += len;
        } else if (type == 1) {
            Huffman lencode, distcode;
            fixed_tables(lencode, distcode);
            if (!codes(r, lencode, distcode, out, expected, error)) return false;
        } else if (type == 2) {
            Huffman lencode, distcode;
            if (!dynamic_tables(r, lencode, distcode, error)) return false;
            if (!codes(r, lencode, distcode, out, expected, error)) return false;
        } else {
            error = "invalid block type";
            return false;
        }
        if (last) break;
    }
    return true;
}

} // namespace dh::foundation::startup
