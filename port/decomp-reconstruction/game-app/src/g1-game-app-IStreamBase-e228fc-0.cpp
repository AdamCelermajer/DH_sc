// Batch g1-game-app-IStreamBase-e228fc-0 (IStreamBase, part 1 of 2).
// Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so. The assembly was checked for the vtable slots and the
// 64-bit transfer count (readAs<unsigned int>, 0x313b48), seekWrite (0x316408), the string writeAs (0x461668) and the
// deleting destructor (0x316798, skipped).
//
// Each typed helper is an explicit specialization with its own address, as in the other part of this class
// (g1-game-app-IStreamBase-e228fc-1.cpp). The primary templates in dh2/IStreamBase.hpp have no body.
//
// The original moves sizeof(T) bytes straight into the object. That is exact for the scalar types and for
// Door::DoorStates, whose host and file sizes match. Point3D<float> and std::bitset<128> are decoded field by field from
// their 32-bit file layout instead, so no host struct is copied (TRACK-B decision 2).
// Structs::CharacterProperties is the 900-byte sheet: the vptr word (not stored), then 224 int32 words, encoded and
// decoded one word at a time like the other records.
#include "dh2/IStreamBase.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "core-platform/deps/game_types.hpp"
#include "dh2/Door.hpp"
#include "pydata-accessors/structs_records.hpp"  // Structs::CharacterProperties

// Global assert level (libDungeonHunter2.so gAssertLevel). core-platform/memory.hpp declares it too, but that header is
// not included here, as in the other sources of this class.
extern int gAssertLevel;

namespace {

// The source path that the original passes to its IStream.h ASSERT.
constexpr const char* kIStreamSource = "..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/IStream.h";

// The lines of the two ASSERTs in the template: 69 for a read, 77 for a write.
constexpr int kReadAssertLine = 69;
constexpr int kWriteAssertLine = 77;

// The original ASSERT. gAssertLevel 2 stops the program (the original stores to address 0 to crash). gAssertLevel 1
// prints the message to stderr, and execution continues. Any other level does nothing.
void ReportIStreamAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kIStreamSource, line);
    }
}

// File fields are little-endian. Each field is encoded and decoded on its own; no host struct is copied.
void PutU32(std::uint8_t* p, std::uint32_t v) {
    p[0] = static_cast<std::uint8_t>(v);
    p[1] = static_cast<std::uint8_t>(v >> 8);
    p[2] = static_cast<std::uint8_t>(v >> 16);
    p[3] = static_cast<std::uint8_t>(v >> 24);
}

std::uint32_t GetU32(const std::uint8_t* p) {
    return static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8) |
           (static_cast<std::uint32_t>(p[2]) << 16) | (static_cast<std::uint32_t>(p[3]) << 24);
}

// A float32 field is its IEEE bit pattern.
void PutFloat(std::uint8_t* p, float v) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &v, sizeof bits);
    PutU32(p, bits);
}

float GetFloat(const std::uint8_t* p) {
    const std::uint32_t bits = GetU32(p);
    float v = 0.0f;
    std::memcpy(&v, &bits, sizeof v);
    return v;
}

// Point3D<float>: X, Y and Z as float32 at +0x0, +0x4 and +0x8. 12 bytes.
constexpr std::size_t kPoint3DFileSize = 12;

void EncodePoint3D(const Point3D<float>& point, std::uint8_t* out) {
    PutFloat(out + 0, point.X);
    PutFloat(out + 4, point.Y);
    PutFloat(out + 8, point.Z);
}

void DecodePoint3D(const std::uint8_t* in, Point3D<float>& point) {
    point.X = GetFloat(in + 0);
    point.Y = GetFloat(in + 4);
    point.Z = GetFloat(in + 8);
}

// std::bitset<128>: 16 bytes. Bit i is bit i % 8 of byte i / 8. That is the order the original's raw copy gives on a
// little-endian target, whatever the width of the bitset's internal words.
constexpr std::size_t kBitsetFileSize = 16;
constexpr std::size_t kBitsetBits = 128;

void EncodeBits(const std::bitset<kBitsetBits>& bits, std::uint8_t* out) {
    for (std::size_t i = 0; i < kBitsetFileSize; ++i) {
        out[i] = 0;
    }
    for (std::size_t bit = 0; bit < kBitsetBits; ++bit) {
        if (bits[bit]) {
            out[bit / 8] = static_cast<std::uint8_t>(out[bit / 8] | (1u << (bit % 8)));
        }
    }
}

void DecodeBits(const std::uint8_t* in, std::bitset<kBitsetBits>& bits) {
    for (std::size_t bit = 0; bit < kBitsetBits; ++bit) {
        bits.set(bit, ((in[bit / 8] >> (bit % 8)) & 1u) != 0);
    }
}

// Structs::CharacterProperties: 900 bytes in the original. +0x00 is the vtable pointer, then the 224 int32 properties
// m_w1..m_w224 at +0x04 .. +0x383. The original copies the whole object, vptr included; a host vptr is not file data,
// so file bytes +0x00..+0x03 are written as zero and ignored on read, and each word is moved on its own.
constexpr std::size_t kSheetFileSize = 900;
constexpr std::size_t kSheetWords = 224;

using Sheet = Structs::CharacterProperties;
using SheetWord = std::int32_t Sheet::*;

// The words in file order: word i is at file offset 4 + 4 * i.
constexpr SheetWord kSheetWordMembers[kSheetWords] = {
    &Sheet::m_w1, &Sheet::m_w2, &Sheet::m_w3, &Sheet::m_w4, &Sheet::m_w5, &Sheet::m_w6,
    &Sheet::m_w7, &Sheet::m_w8, &Sheet::m_w9, &Sheet::m_w10, &Sheet::m_w11, &Sheet::m_w12,
    &Sheet::m_w13, &Sheet::m_w14, &Sheet::m_w15, &Sheet::m_w16, &Sheet::m_w17, &Sheet::m_w18,
    &Sheet::m_w19, &Sheet::m_w20, &Sheet::m_w21, &Sheet::m_w22, &Sheet::m_w23, &Sheet::m_w24,
    &Sheet::m_w25, &Sheet::m_w26, &Sheet::m_w27, &Sheet::m_w28, &Sheet::m_w29, &Sheet::m_w30,
    &Sheet::m_w31, &Sheet::m_w32, &Sheet::m_w33, &Sheet::m_w34, &Sheet::m_w35, &Sheet::m_w36,
    &Sheet::m_w37, &Sheet::m_w38, &Sheet::m_w39, &Sheet::m_w40, &Sheet::m_w41, &Sheet::m_w42,
    &Sheet::m_w43, &Sheet::m_w44, &Sheet::m_w45, &Sheet::m_w46, &Sheet::m_w47, &Sheet::m_w48,
    &Sheet::m_w49, &Sheet::m_w50, &Sheet::m_w51, &Sheet::m_w52, &Sheet::m_w53, &Sheet::m_w54,
    &Sheet::m_w55, &Sheet::m_w56, &Sheet::m_w57, &Sheet::m_w58, &Sheet::m_w59, &Sheet::m_w60,
    &Sheet::m_w61, &Sheet::m_w62, &Sheet::m_w63, &Sheet::m_w64, &Sheet::m_w65, &Sheet::m_w66,
    &Sheet::m_w67, &Sheet::m_w68, &Sheet::m_w69, &Sheet::m_w70, &Sheet::m_w71, &Sheet::m_w72,
    &Sheet::m_w73, &Sheet::m_w74, &Sheet::m_w75, &Sheet::m_w76, &Sheet::m_w77, &Sheet::m_w78,
    &Sheet::m_w79, &Sheet::m_w80, &Sheet::m_w81, &Sheet::m_w82, &Sheet::m_w83, &Sheet::m_w84,
    &Sheet::m_w85, &Sheet::m_w86, &Sheet::m_w87, &Sheet::m_w88, &Sheet::m_w89, &Sheet::m_w90,
    &Sheet::m_w91, &Sheet::m_w92, &Sheet::m_w93, &Sheet::m_w94, &Sheet::m_w95, &Sheet::m_w96,
    &Sheet::m_w97, &Sheet::m_w98, &Sheet::m_w99, &Sheet::m_w100, &Sheet::m_w101, &Sheet::m_w102,
    &Sheet::m_w103, &Sheet::m_w104, &Sheet::m_w105, &Sheet::m_w106, &Sheet::m_w107, &Sheet::m_w108,
    &Sheet::m_w109, &Sheet::m_w110, &Sheet::m_w111, &Sheet::m_w112, &Sheet::m_w113, &Sheet::m_w114,
    &Sheet::m_w115, &Sheet::m_w116, &Sheet::m_w117, &Sheet::m_w118, &Sheet::m_w119, &Sheet::m_w120,
    &Sheet::m_w121, &Sheet::m_w122, &Sheet::m_w123, &Sheet::m_w124, &Sheet::m_w125, &Sheet::m_w126,
    &Sheet::m_w127, &Sheet::m_w128, &Sheet::m_w129, &Sheet::m_w130, &Sheet::m_w131, &Sheet::m_w132,
    &Sheet::m_w133, &Sheet::m_w134, &Sheet::m_w135, &Sheet::m_w136, &Sheet::m_w137, &Sheet::m_w138,
    &Sheet::m_w139, &Sheet::m_w140, &Sheet::m_w141, &Sheet::m_w142, &Sheet::m_w143, &Sheet::m_w144,
    &Sheet::m_w145, &Sheet::m_w146, &Sheet::m_w147, &Sheet::m_w148, &Sheet::m_w149, &Sheet::m_w150,
    &Sheet::m_w151, &Sheet::m_w152, &Sheet::m_w153, &Sheet::m_w154, &Sheet::m_w155, &Sheet::m_w156,
    &Sheet::m_w157, &Sheet::m_w158, &Sheet::m_w159, &Sheet::m_w160, &Sheet::m_w161, &Sheet::m_w162,
    &Sheet::m_w163, &Sheet::m_w164, &Sheet::m_w165, &Sheet::m_w166, &Sheet::m_w167, &Sheet::m_w168,
    &Sheet::m_w169, &Sheet::m_w170, &Sheet::m_w171, &Sheet::m_w172, &Sheet::m_w173, &Sheet::m_w174,
    &Sheet::m_w175, &Sheet::m_w176, &Sheet::m_w177, &Sheet::m_w178, &Sheet::m_w179, &Sheet::m_w180,
    &Sheet::m_w181, &Sheet::m_w182, &Sheet::m_w183, &Sheet::m_w184, &Sheet::m_w185, &Sheet::m_w186,
    &Sheet::m_w187, &Sheet::m_w188, &Sheet::m_w189, &Sheet::m_w190, &Sheet::m_w191, &Sheet::m_w192,
    &Sheet::m_w193, &Sheet::m_w194, &Sheet::m_w195, &Sheet::m_w196, &Sheet::m_w197, &Sheet::m_w198,
    &Sheet::m_w199, &Sheet::m_w200, &Sheet::m_w201, &Sheet::m_w202, &Sheet::m_w203, &Sheet::m_w204,
    &Sheet::m_w205, &Sheet::m_w206, &Sheet::m_w207, &Sheet::m_w208, &Sheet::m_w209, &Sheet::m_w210,
    &Sheet::m_w211, &Sheet::m_w212, &Sheet::m_w213, &Sheet::m_w214, &Sheet::m_w215, &Sheet::m_w216,
    &Sheet::m_w217, &Sheet::m_w218, &Sheet::m_w219, &Sheet::m_w220, &Sheet::m_w221, &Sheet::m_w222,
    &Sheet::m_w223, &Sheet::m_w224,
};

void EncodeSheet(const Structs::CharacterProperties& sheet, std::uint8_t* out) {
    PutU32(out, 0);
    for (std::size_t i = 0; i < kSheetWords; ++i) {
        PutU32(out + 4 + 4 * i, static_cast<std::uint32_t>(sheet.*kSheetWordMembers[i]));
    }
}

void DecodeSheet(const std::uint8_t* in, Structs::CharacterProperties& sheet) {
    for (std::size_t i = 0; i < kSheetWords; ++i) {
        sheet.*kSheetWordMembers[i] = static_cast<std::int32_t>(GetU32(in + 4 + 4 * i));
    }
}

}  // namespace

// 0x313b48 void IStreamBase::readAs<unsigned int>(unsigned int&)
// read() is vtable +0x18 and returns a 64-bit count (r0:r1). The original checks both halves; comparing the whole
// count does the same.
template <>
void IStreamBase::readAs<unsigned int>(unsigned int& value) {
    const std::uint64_t got = read(&value, sizeof(unsigned int));
    if (got != sizeof(unsigned int)) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
}

// 0x316404 IStreamBase::~IStreamBase() (D1, base-object destructor)
// The original body is empty (4 bytes). The compiler sets the vtable pointer.
IStreamBase::~IStreamBase() {}

// 0x316408 void IStreamBase::seekWrite(unsigned long long) (virtual, vtable +0x2c)
// Forwards the 64-bit position to seek() (vtable +0x20), a tail call in the original. The original also passes the
// object's vtable pointer as a second argument, a register left over for the 64-bit position. seek() takes no such
// argument, so it is not passed here.
void IStreamBase::seekWrite(std::uint64_t pos) {
    seek(pos);
}

// 0x31641c std::uint64_t IStreamBase::tellWrite() const (virtual, vtable +0x30)
// Returns tell() (vtable +0x24), a tail call in the original. The 64-bit result passes through in r0:r1.
std::uint64_t IStreamBase::tellWrite() const {
    return tell();
}

// 0x316798 IStreamBase::~IStreamBase() (D0, deleting destructor)
// Skipped: compiler-generated. It is the deleting form of the virtual destructor declared in dh2/IStreamBase.hpp. The
// original stores the class vtable pointer (_ZTV11IStreamBase + 8) and calls CustomFree on the object (asm checked).

// 0x33e040 void IStreamBase::readAs<bool>(bool&)
template <>
void IStreamBase::readAs<bool>(bool& value) {
    const std::uint64_t got = read(&value, sizeof(bool));
    if (got != sizeof(bool)) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
}

// 0x33e138 void IStreamBase::writeAs<bool>(bool const&)
// write() is vtable +0x1c.
template <>
void IStreamBase::writeAs<bool>(const bool& value) {
    const std::uint64_t written = write(&value, sizeof(bool));
    if (written != sizeof(bool)) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x37fb38 void IStreamBase::writeAs<std::bitset<128u> >(std::bitset<128u> const&)
// The 16-byte file layout comes from EncodeBits, not from a copy of the bitset's words.
template <>
void IStreamBase::writeAs<std::bitset<128u>>(const std::bitset<128u>& value) {
    std::uint8_t file[kBitsetFileSize];
    EncodeBits(value, file);
    const std::uint64_t written = write(file, kBitsetFileSize);
    if (written != kBitsetFileSize) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x37fbe8 void IStreamBase::readAs<std::bitset<128u> >(std::bitset<128u>&)
// The bits start from the bitset's current values, so bytes a short read does not reach keep them, as the original's
// direct read does. The 16 bytes are then decoded bit by bit.
template <>
void IStreamBase::readAs<std::bitset<128u>>(std::bitset<128u>& value) {
    std::uint8_t file[kBitsetFileSize];
    EncodeBits(value, file);
    const std::uint64_t got = read(file, kBitsetFileSize);
    if (got != kBitsetFileSize) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
    DecodeBits(file, value);
}

// 0x38b758 void IStreamBase::readAs<int>(int&)
// Also called by readAs(std::string&) for the length, in the other part of this class.
template <>
void IStreamBase::readAs<int>(int& value) {
    const std::uint64_t got = read(&value, sizeof(int));
    if (got != sizeof(int)) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
}

// 0x38b808 void IStreamBase::writeAs<int>(int const&)
// Also called by writeAs(std::string const&) for the length.
template <>
void IStreamBase::writeAs<int>(const int& value) {
    const std::uint64_t written = write(&value, sizeof(int));
    if (written != sizeof(int)) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x39f638 void IStreamBase::readAs<unsigned char>(unsigned char&)
template <>
void IStreamBase::readAs<unsigned char>(unsigned char& value) {
    const std::uint64_t got = read(&value, sizeof(unsigned char));
    if (got != sizeof(unsigned char)) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
}

// 0x39f828 void IStreamBase::writeAs<unsigned char>(unsigned char const&)
template <>
void IStreamBase::writeAs<unsigned char>(const unsigned char& value) {
    const std::uint64_t written = write(&value, sizeof(unsigned char));
    if (written != sizeof(unsigned char)) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x3a3970 void IStreamBase::readAs<Structs::CharacterProperties>(Structs::CharacterProperties&)
// read() is vtable +0x18 and gets (object, 900, 0). ASSERT bytesRead == sizeof(T) at line 69. The words start from the
// sheet's current values, so a short read keeps them, as the original's direct read does. Then the 224 words are
// decoded. The vptr word of the file image is skipped (see kSheetFileSize).
template <>
void IStreamBase::readAs<Structs::CharacterProperties>(Structs::CharacterProperties& value) {
    std::uint8_t file[kSheetFileSize];
    EncodeSheet(value, file);
    const std::uint64_t got = read(file, kSheetFileSize);
    if (got != kSheetFileSize) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
    DecodeSheet(file, value);
}

// 0x3a3ad0 void IStreamBase::writeAs<Structs::CharacterProperties>(Structs::CharacterProperties const&)
// write() is vtable +0x1c and gets (object, 900, 0). ASSERT bytesWritten == sizeof(T) at line 77. The file image is
// the vptr word (zero here) followed by the 224 words.
template <>
void IStreamBase::writeAs<Structs::CharacterProperties>(const Structs::CharacterProperties& value) {
    std::uint8_t file[kSheetFileSize];
    EncodeSheet(value, file);
    const std::uint64_t written = write(file, kSheetFileSize);
    if (written != kSheetFileSize) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x3a3a20 void IStreamBase::readAs<Point3D<float> >(Point3D<float>&)
// The 12 bytes start from the current values, so a short read keeps them, as the original's direct read does. Then X,
// Y and Z are decoded.
template <>
void IStreamBase::readAs<Point3D<float>>(Point3D<float>& value) {
    std::uint8_t file[kPoint3DFileSize];
    EncodePoint3D(value, file);
    const std::uint64_t got = read(file, kPoint3DFileSize);
    if (got != kPoint3DFileSize) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
    DecodePoint3D(file, value);
}

// 0x3a3b80 void IStreamBase::writeAs<Point3D<float> >(Point3D<float> const&)
template <>
void IStreamBase::writeAs<Point3D<float>>(const Point3D<float>& value) {
    std::uint8_t file[kPoint3DFileSize];
    EncodePoint3D(value, file);
    const std::uint64_t written = write(file, kPoint3DFileSize);
    if (written != kPoint3DFileSize) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x3e79b4 void IStreamBase::readAs<Door::DoorStates>(Door::DoorStates&)
// A 4-byte enum: the host and file sizes match, so the raw read is the original's read.
template <>
void IStreamBase::readAs<Door::DoorStates>(Door::DoorStates& value) {
    const std::uint64_t got = read(&value, sizeof(Door::DoorStates));
    if (got != sizeof(Door::DoorStates)) {
        ReportIStreamAssert("bytesRead == sizeof(T)", kReadAssertLine);
    }
}

// 0x3e7abc void IStreamBase::writeAs<Door::DoorStates>(Door::DoorStates const&)
template <>
void IStreamBase::writeAs<Door::DoorStates>(const Door::DoorStates& value) {
    const std::uint64_t written = write(&value, sizeof(Door::DoorStates));
    if (written != sizeof(Door::DoorStates)) {
        ReportIStreamAssert("bytesWritten == sizeof(T)", kWriteAssertLine);
    }
}

// 0x461668 void IStreamBase::writeAs(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)
// The length written first is size + 1, the terminating NUL included, as an int32 through writeAs<int>. Then size + 1
// bytes from the character buffer go through write(), so the NUL is written too. The original takes that count from its
// end and begin pointers; std::string gives the same count from size(). The count is sign-extended to 64 bits, as the
// original does (asm: MOV R3, R2, ASR #31). The result of write() is dropped: the original tail-calls it, and the
// header declares void.
void IStreamBase::writeAs(const std::string& value) {
    const int length = static_cast<int>(value.size()) + 1;
    writeAs<int>(length);
    write(value.c_str(), static_cast<std::uint64_t>(length));
}
