// Test for batch r1-glitch-core-io-glitch_io-5e759c-0: stream stride, primitive stream save and load.
// Build and run with:
//   bash tools/trackb/check_tu.sh --test <this file> <r1 src> <stream_itr_writer.cpp> <reference_counted.cpp>
// getPrimitiveCount is defined here for the test only (the real one is in glitch-video).
#include <cstdint>
#include <cstdio>
#include <utility>
#include <vector>

#include "glitch-core-io/tb/glitch_io.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace glitch::video {
// Test stand-in: the real count depends on the primitive type (glitch-video, 0x5a0300).
std::uint32_t CPrimitiveStream::getPrimitiveCount() const { return count; }
}  // namespace glitch::video

namespace {

// Read file over a byte vector (little-endian data is handed out as stored).
class MemReadFile : public glitch::io::IReadFile {
public:
    explicit MemReadFile(std::vector<std::uint8_t> bytes) : data(std::move(bytes)) {}
    void read(void* buffer, unsigned int count) override {
        for (unsigned int i = 0; i < count; ++i) {
            static_cast<std::uint8_t*>(buffer)[i] = pos < data.size() ? data[pos] : 0;
            ++pos;
        }
    }
    bool seek(long offset, bool relative) override {
        pos = static_cast<std::size_t>(relative ? static_cast<long>(pos) + offset : offset);
        return true;
    }
    long getPos() const override { return static_cast<long>(pos); }

    std::vector<std::uint8_t> data;
    std::size_t pos = 0;
};

// Write file that keeps every byte written.
class MemWriteFile : public glitch::io::IWriteFile {
public:
    std::size_t write(const void* bytes, unsigned int count) override {
        const auto* p = static_cast<const std::uint8_t*>(bytes);
        out.insert(out.end(), p, p + count);
        return count;
    }
    std::vector<std::uint8_t> out;
};

// Four little-endian bytes of a 32-bit value.
void appendLE32(std::vector<std::uint8_t>& v, std::uint32_t x) {
    for (int i = 0; i < 4; ++i) v.push_back(static_cast<std::uint8_t>(x >> (8 * i)));
}

// Four big-endian bytes of a 32-bit value.
void appendBE32(std::vector<std::uint8_t>& v, std::uint32_t x) {
    for (int i = 3; i >= 0; --i) v.push_back(static_cast<std::uint8_t>(x >> (8 * i)));
}

// A stream entry that owns its buffer.
void setEntry(glitch::video::CVertexStreams& table, std::size_t index, bool bound, std::uint16_t valueType,
              std::uint16_t count) {
    if (table.streams.size() <= index) table.streams.resize(index + 1);
    auto& entry = table.streams[index];
    entry.buffer = bound ? std::make_shared<glitch::video::IBuffer>() : nullptr;
    entry.field0a = valueType;
    entry.field0c = count;
    entry.field0e = 0;
}

// Heap tables, so the intrusive pointer that wraps them releases them with delete.
boost::intrusive_ptr<const glitch::video::CVertexStreams> makeTable() {
    return boost::intrusive_ptr<const glitch::video::CVertexStreams>(new glitch::video::CVertexStreams());
}

void testStride() {
    auto empty = makeTable();
    CHECK(glitch::io::getVertexStreamsStride(empty) == 0);

    // One bound stream of three 4-byte values: 12 bytes. An unbound entry adds nothing.
    auto one = makeTable();
    auto* oneMut = const_cast<glitch::video::CVertexStreams*>(one.get());
    setEntry(*oneMut, 0, true, 4, 3);
    setEntry(*oneMut, 1, false, 4, 5);
    CHECK(glitch::io::getVertexStreamsStride(one) == 12);

    // 1-byte x3 (total 3), then 4-byte x2: pad to 4, total 12. The first bound size (1) aligns nothing more.
    auto two = makeTable();
    auto* twoMut = const_cast<glitch::video::CVertexStreams*>(two.get());
    setEntry(*twoMut, 0, true, 0, 3);
    setEntry(*twoMut, 1, true, 4, 2);
    CHECK(glitch::io::getVertexStreamsStride(two) == 12);

    // 2-byte x1 (total 2), 1-byte x1 (total 3), then the final pad to the first bound size (2): 4.
    auto three = makeTable();
    auto* threeMut = const_cast<glitch::video::CVertexStreams*>(three.get());
    setEntry(*threeMut, 0, true, 2, 1);
    setEntry(*threeMut, 1, true, 0, 1);
    CHECK(glitch::io::getVertexStreamsStride(three) == 4);
}

void testSavePrimitiveLE() {
    glitch::video::CPrimitiveStream stream;
    stream.buffer = boost::intrusive_ptr<glitch::video::IBuffer>(new glitch::video::IBuffer());
    stream.buffer->storage = {1, 0, 2, 0, 3, 0};
    stream.buffer->data = stream.buffer->storage.data();
    stream.primitiveType = 5;
    stream.indexType = 1;
    stream.count = 3;
    stream.unk_0c = 7;
    stream.unk_10 = 9;

    MemWriteFile file;
    const int returned = glitch::io::save(stream, &file, false);
    CHECK(returned == 24);

    std::vector<std::uint8_t> expected;
    for (std::uint32_t w : {5u, 1u, 3u, 3u, 7u, 9u}) appendLE32(expected, w);
    for (std::uint8_t b : {1, 0, 2, 0, 3, 0}) expected.push_back(b);
    CHECK(file.out == expected);
}

void testSavePrimitiveBE() {
    glitch::video::CPrimitiveStream stream;
    stream.buffer = boost::intrusive_ptr<glitch::video::IBuffer>(new glitch::video::IBuffer());
    stream.buffer->storage = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    stream.buffer->data = stream.buffer->storage.data();
    stream.primitiveType = 4;
    stream.indexType = 2;
    stream.count = 2;

    MemWriteFile file;
    glitch::io::save(stream, &file, true);

    std::vector<std::uint8_t> expected;
    for (std::uint32_t w : {4u, 2u, 2u, 2u, 0u, 0u}) appendBE32(expected, w);
    // Each 32-bit index is written byte-reversed: 01 02 03 04 becomes 04 03 02 01.
    for (std::uint8_t b : {0x04, 0x03, 0x02, 0x01, 0x08, 0x07, 0x06, 0x05}) expected.push_back(b);
    CHECK(file.out == expected);
}

void testLoadPrimitive() {
    std::vector<std::uint8_t> bytes;
    for (std::uint32_t w : {5u, 1u, 2u, 2u, 7u, 9u}) appendLE32(bytes, w);
    for (std::uint8_t b : {0xAA, 0xBB, 0xCC, 0xDD}) bytes.push_back(b);
    MemReadFile file(bytes);
    glitch::video::CPrimitiveStream stream = glitch::io::loadPS(&file, false, nullptr);
    CHECK(stream.primitiveType == 5);
    CHECK(stream.indexType == 1);
    CHECK(stream.count == 2);
    CHECK(stream.unk_0c == 7);
    CHECK(stream.unk_10 == 9);
    CHECK(stream.offset == 0);
    CHECK(stream.buffer.get() != nullptr);
    if (stream.buffer) {
        CHECK(stream.buffer->size == 4);
        CHECK(stream.buffer->data[0] == 0xAA && stream.buffer->data[3] == 0xDD);
    }
}

void testLoadPrimitiveEmpty() {
    std::vector<std::uint8_t> bytes;
    for (std::uint32_t w : {5u, 0xffu, 0u, 0u, 4u, 10u}) appendLE32(bytes, w);
    MemReadFile file(bytes);
    glitch::video::CPrimitiveStream stream = glitch::io::loadPS(&file, false, nullptr);
    CHECK(stream.buffer.get() == nullptr);
    CHECK(stream.indexType == 0xff);
    CHECK(file.pos == 24);  // the empty stream reads only its header
}

}  // namespace

int main() {
    testStride();
    testSavePrimitiveLE();
    testSavePrimitiveBE();
    testLoadPrimitive();
    testLoadPrimitiveEmpty();
    if (failures == 0) std::printf("all r1-glitch-core-io-glitch_io-5e759c-0 tests passed\n");
    return failures == 0 ? 0 : 1;
}
