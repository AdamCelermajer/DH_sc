// Checks the read forwarders of the generated record classes. A forwarder (a thunk to
// CharacterProperties::read) owns no fields: its read goes through the base read, which
// stores the 224 words at +0x4 .. +0x380. For the same stream, a forwarder must leave every
// word the same as the base does.
#include "pydata-accessors/structs_records.hpp"
#include "read_bytes_stream.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Hands out a fixed byte sequence to StreamReader::readAs and reports how far it was read.
class ByteStream : public ReadBytesStream {
public:
    ByteStream(const unsigned char* data, std::size_t n) : data_(data), n_(n) {}
    bool ReadBytes(void* dst, std::size_t n) override {
        if (pos_ + n > n_) return false;
        std::memcpy(dst, data_ + pos_, n);
        pos_ += n;
        return true;
    }
    std::size_t pos() const { return pos_; }

private:
    const unsigned char* data_;
    std::size_t n_;
    std::size_t pos_ = 0;
};

// Word k (0-based) is the base's member at offset +0x4 * (k + 1).
using Word = std::int32_t Structs::CharacterProperties::*;
static const Word kWords[] = {
    &Structs::CharacterProperties::m_w1, &Structs::CharacterProperties::m_w2, &Structs::CharacterProperties::m_w3, &Structs::CharacterProperties::m_w4, &Structs::CharacterProperties::m_w5, &Structs::CharacterProperties::m_w6,
    &Structs::CharacterProperties::m_w7, &Structs::CharacterProperties::m_w8, &Structs::CharacterProperties::m_w9, &Structs::CharacterProperties::m_w10, &Structs::CharacterProperties::m_w11, &Structs::CharacterProperties::m_w12,
    &Structs::CharacterProperties::m_w13, &Structs::CharacterProperties::m_w14, &Structs::CharacterProperties::m_w15, &Structs::CharacterProperties::m_w16, &Structs::CharacterProperties::m_w17, &Structs::CharacterProperties::m_w18,
    &Structs::CharacterProperties::m_w19, &Structs::CharacterProperties::m_w20, &Structs::CharacterProperties::m_w21, &Structs::CharacterProperties::m_w22, &Structs::CharacterProperties::m_w23, &Structs::CharacterProperties::m_w24,
    &Structs::CharacterProperties::m_w25, &Structs::CharacterProperties::m_w26, &Structs::CharacterProperties::m_w27, &Structs::CharacterProperties::m_w28, &Structs::CharacterProperties::m_w29, &Structs::CharacterProperties::m_w30,
    &Structs::CharacterProperties::m_w31, &Structs::CharacterProperties::m_w32, &Structs::CharacterProperties::m_w33, &Structs::CharacterProperties::m_w34, &Structs::CharacterProperties::m_w35, &Structs::CharacterProperties::m_w36,
    &Structs::CharacterProperties::m_w37, &Structs::CharacterProperties::m_w38, &Structs::CharacterProperties::m_w39, &Structs::CharacterProperties::m_w40, &Structs::CharacterProperties::m_w41, &Structs::CharacterProperties::m_w42,
    &Structs::CharacterProperties::m_w43, &Structs::CharacterProperties::m_w44, &Structs::CharacterProperties::m_w45, &Structs::CharacterProperties::m_w46, &Structs::CharacterProperties::m_w47, &Structs::CharacterProperties::m_w48,
    &Structs::CharacterProperties::m_w49, &Structs::CharacterProperties::m_w50, &Structs::CharacterProperties::m_w51, &Structs::CharacterProperties::m_w52, &Structs::CharacterProperties::m_w53, &Structs::CharacterProperties::m_w54,
    &Structs::CharacterProperties::m_w55, &Structs::CharacterProperties::m_w56, &Structs::CharacterProperties::m_w57, &Structs::CharacterProperties::m_w58, &Structs::CharacterProperties::m_w59, &Structs::CharacterProperties::m_w60,
    &Structs::CharacterProperties::m_w61, &Structs::CharacterProperties::m_w62, &Structs::CharacterProperties::m_w63, &Structs::CharacterProperties::m_w64, &Structs::CharacterProperties::m_w65, &Structs::CharacterProperties::m_w66,
    &Structs::CharacterProperties::m_w67, &Structs::CharacterProperties::m_w68, &Structs::CharacterProperties::m_w69, &Structs::CharacterProperties::m_w70, &Structs::CharacterProperties::m_w71, &Structs::CharacterProperties::m_w72,
    &Structs::CharacterProperties::m_w73, &Structs::CharacterProperties::m_w74, &Structs::CharacterProperties::m_w75, &Structs::CharacterProperties::m_w76, &Structs::CharacterProperties::m_w77, &Structs::CharacterProperties::m_w78,
    &Structs::CharacterProperties::m_w79, &Structs::CharacterProperties::m_w80, &Structs::CharacterProperties::m_w81, &Structs::CharacterProperties::m_w82, &Structs::CharacterProperties::m_w83, &Structs::CharacterProperties::m_w84,
    &Structs::CharacterProperties::m_w85, &Structs::CharacterProperties::m_w86, &Structs::CharacterProperties::m_w87, &Structs::CharacterProperties::m_w88, &Structs::CharacterProperties::m_w89, &Structs::CharacterProperties::m_w90,
    &Structs::CharacterProperties::m_w91, &Structs::CharacterProperties::m_w92, &Structs::CharacterProperties::m_w93, &Structs::CharacterProperties::m_w94, &Structs::CharacterProperties::m_w95, &Structs::CharacterProperties::m_w96,
    &Structs::CharacterProperties::m_w97, &Structs::CharacterProperties::m_w98, &Structs::CharacterProperties::m_w99, &Structs::CharacterProperties::m_w100, &Structs::CharacterProperties::m_w101, &Structs::CharacterProperties::m_w102,
    &Structs::CharacterProperties::m_w103, &Structs::CharacterProperties::m_w104, &Structs::CharacterProperties::m_w105, &Structs::CharacterProperties::m_w106, &Structs::CharacterProperties::m_w107, &Structs::CharacterProperties::m_w108,
    &Structs::CharacterProperties::m_w109, &Structs::CharacterProperties::m_w110, &Structs::CharacterProperties::m_w111, &Structs::CharacterProperties::m_w112, &Structs::CharacterProperties::m_w113, &Structs::CharacterProperties::m_w114,
    &Structs::CharacterProperties::m_w115, &Structs::CharacterProperties::m_w116, &Structs::CharacterProperties::m_w117, &Structs::CharacterProperties::m_w118, &Structs::CharacterProperties::m_w119, &Structs::CharacterProperties::m_w120,
    &Structs::CharacterProperties::m_w121, &Structs::CharacterProperties::m_w122, &Structs::CharacterProperties::m_w123, &Structs::CharacterProperties::m_w124, &Structs::CharacterProperties::m_w125, &Structs::CharacterProperties::m_w126,
    &Structs::CharacterProperties::m_w127, &Structs::CharacterProperties::m_w128, &Structs::CharacterProperties::m_w129, &Structs::CharacterProperties::m_w130, &Structs::CharacterProperties::m_w131, &Structs::CharacterProperties::m_w132,
    &Structs::CharacterProperties::m_w133, &Structs::CharacterProperties::m_w134, &Structs::CharacterProperties::m_w135, &Structs::CharacterProperties::m_w136, &Structs::CharacterProperties::m_w137, &Structs::CharacterProperties::m_w138,
    &Structs::CharacterProperties::m_w139, &Structs::CharacterProperties::m_w140, &Structs::CharacterProperties::m_w141, &Structs::CharacterProperties::m_w142, &Structs::CharacterProperties::m_w143, &Structs::CharacterProperties::m_w144,
    &Structs::CharacterProperties::m_w145, &Structs::CharacterProperties::m_w146, &Structs::CharacterProperties::m_w147, &Structs::CharacterProperties::m_w148, &Structs::CharacterProperties::m_w149, &Structs::CharacterProperties::m_w150,
    &Structs::CharacterProperties::m_w151, &Structs::CharacterProperties::m_w152, &Structs::CharacterProperties::m_w153, &Structs::CharacterProperties::m_w154, &Structs::CharacterProperties::m_w155, &Structs::CharacterProperties::m_w156,
    &Structs::CharacterProperties::m_w157, &Structs::CharacterProperties::m_w158, &Structs::CharacterProperties::m_w159, &Structs::CharacterProperties::m_w160, &Structs::CharacterProperties::m_w161, &Structs::CharacterProperties::m_w162,
    &Structs::CharacterProperties::m_w163, &Structs::CharacterProperties::m_w164, &Structs::CharacterProperties::m_w165, &Structs::CharacterProperties::m_w166, &Structs::CharacterProperties::m_w167, &Structs::CharacterProperties::m_w168,
    &Structs::CharacterProperties::m_w169, &Structs::CharacterProperties::m_w170, &Structs::CharacterProperties::m_w171, &Structs::CharacterProperties::m_w172, &Structs::CharacterProperties::m_w173, &Structs::CharacterProperties::m_w174,
    &Structs::CharacterProperties::m_w175, &Structs::CharacterProperties::m_w176, &Structs::CharacterProperties::m_w177, &Structs::CharacterProperties::m_w178, &Structs::CharacterProperties::m_w179, &Structs::CharacterProperties::m_w180,
    &Structs::CharacterProperties::m_w181, &Structs::CharacterProperties::m_w182, &Structs::CharacterProperties::m_w183, &Structs::CharacterProperties::m_w184, &Structs::CharacterProperties::m_w185, &Structs::CharacterProperties::m_w186,
    &Structs::CharacterProperties::m_w187, &Structs::CharacterProperties::m_w188, &Structs::CharacterProperties::m_w189, &Structs::CharacterProperties::m_w190, &Structs::CharacterProperties::m_w191, &Structs::CharacterProperties::m_w192,
    &Structs::CharacterProperties::m_w193, &Structs::CharacterProperties::m_w194, &Structs::CharacterProperties::m_w195, &Structs::CharacterProperties::m_w196, &Structs::CharacterProperties::m_w197, &Structs::CharacterProperties::m_w198,
    &Structs::CharacterProperties::m_w199, &Structs::CharacterProperties::m_w200, &Structs::CharacterProperties::m_w201, &Structs::CharacterProperties::m_w202, &Structs::CharacterProperties::m_w203, &Structs::CharacterProperties::m_w204,
    &Structs::CharacterProperties::m_w205, &Structs::CharacterProperties::m_w206, &Structs::CharacterProperties::m_w207, &Structs::CharacterProperties::m_w208, &Structs::CharacterProperties::m_w209, &Structs::CharacterProperties::m_w210,
    &Structs::CharacterProperties::m_w211, &Structs::CharacterProperties::m_w212, &Structs::CharacterProperties::m_w213, &Structs::CharacterProperties::m_w214, &Structs::CharacterProperties::m_w215, &Structs::CharacterProperties::m_w216,
    &Structs::CharacterProperties::m_w217, &Structs::CharacterProperties::m_w218, &Structs::CharacterProperties::m_w219, &Structs::CharacterProperties::m_w220, &Structs::CharacterProperties::m_w221, &Structs::CharacterProperties::m_w222,
    &Structs::CharacterProperties::m_w223, &Structs::CharacterProperties::m_w224,
};
static const std::size_t kWordCount = sizeof kWords / sizeof kWords[0];

// Little-endian value of the 4 bytes at p (the layout StreamReader::readAs decodes).
static std::int32_t le32(const unsigned char* p) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8) |
                                     (static_cast<std::uint32_t>(p[2]) << 16) | (static_cast<std::uint32_t>(p[3]) << 24));
}

// True when every stored word of obj equals the base's word.
template <class Obj>
static bool sameWords(const Structs::CharacterProperties& base, const Obj& obj) {
    for (std::size_t k = 0; k < kWordCount; ++k) {
        if (base.*kWords[k] != obj.*kWords[k]) return false;
    }
    return true;
}

int main() {
    CHECK(kWordCount == 224);

    unsigned char bytes[4 * 224];
    for (std::size_t i = 0; i < sizeof bytes; ++i) bytes[i] = static_cast<unsigned char>(i * 37 + 11);

    // Base read of the whole buffer: every word decodes from its own four bytes.
    Structs::CharacterProperties base;
    ByteStream baseIn(bytes, sizeof bytes);
    base.read(&baseIn);
    CHECK(baseIn.pos() == sizeof bytes);
    for (std::size_t k = 0; k < kWordCount; ++k) {
        CHECK(base.*kWords[k] == le32(bytes + 4 * k));
    }

    // Forwarders read the same buffer through the base and leave the same words.
    {
        Structs::MadrukProperties madruk;
        ByteStream in(bytes, sizeof bytes);
        madruk.read(&in);
        CHECK(in.pos() == sizeof bytes);
        CHECK(sameWords(base, madruk));
    }
    {
        Structs::AssassinProperties assassin;
        ByteStream in(bytes, sizeof bytes);
        assassin.read(&in);
        CHECK(in.pos() == sizeof bytes);
        CHECK(sameWords(base, assassin));
    }
    {
        Structs::FriendlyNPCProperties npc;
        ByteStream in(bytes, sizeof bytes);
        npc.read(&in);
        CHECK(in.pos() == sizeof bytes);
        CHECK(sameWords(base, npc));
    }
    {
        Structs::SlimeProperties slime;
        ByteStream in(bytes, sizeof bytes);
        slime.read(&in);
        CHECK(in.pos() == sizeof bytes);
        CHECK(sameWords(base, slime));
    }

    // A short stream stops both reads at the same word; the words after it stay zero.
    {
        Structs::CharacterProperties shortBase;
        ByteStream shortIn(bytes, 10);
        shortBase.read(&shortIn);
        CHECK(shortIn.pos() == 8);
        CHECK(shortBase.*kWords[0] == le32(bytes));
        CHECK(shortBase.*kWords[2] == 0);

        Structs::PlayerProperties player;
        ByteStream playerIn(bytes, 10);
        player.read(&playerIn);
        CHECK(playerIn.pos() == 8);
        CHECK(sameWords(shortBase, player));
    }

    std::printf("%s\n", failures ? "structs_forwarder: FAILED" : "structs_forwarder: ok");
    return failures != 0;
}
