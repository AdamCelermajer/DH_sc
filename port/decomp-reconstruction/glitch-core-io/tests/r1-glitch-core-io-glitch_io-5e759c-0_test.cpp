// Test for batch r1-glitch-core-io-glitch_io-5e759c-0: stream stride, primitive stream save and load.
// Build and run with:
//   bash tools/trackb/check_tu.sh --test <this file> <r1 src> <stream_itr_writer.cpp> <reference_counted.cpp>
// getPrimitiveCount is glitch-video's definition (the primitive-type rules of 0x5a0300). Buffers are the
// test fakes of io_video_fakes.hpp; loadPS gets a CNullDriver, which creates the index buffer.
#include <cstdint>
#include <cstdio>
#include <utility>
#include <vector>

#include "glitch-core-io/tb/glitch_io.hpp"
#include "io_stub_files.hpp"
#include "io_video_fakes.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {

// Read file over a byte vector (little-endian data is handed out as stored).
class MemReadFile : public glitch::io::testing::StubReadFile {
public:
    explicit MemReadFile(std::vector<std::uint8_t> bytes) : data(std::move(bytes)) {}
    std::int32_t read(void* buffer, std::uint32_t count) override {
        for (unsigned int i = 0; i < count; ++i) {
            static_cast<std::uint8_t*>(buffer)[i] = pos < data.size() ? data[pos] : 0;
            ++pos;
        }
        return static_cast<std::int32_t>(count);
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
class MemWriteFile : public glitch::io::testing::StubWriteFile {
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

// A stream entry that owns its buffer (a fake buffer; the stride does not read it).
void setEntry(glitch::video::CVertexStreams& table, std::size_t index, bool bound, std::uint16_t valueType,
              std::uint16_t count) {
    if (table.m_streams.size() <= index) table.m_streams.resize(index + 1);
    auto& entry = table.m_streams[index];
    entry.buffer = bound ? boost::intrusive_ptr<glitch::video::IBuffer>(new io_test_fakes::FakeBuffer(), true)
                         : boost::intrusive_ptr<glitch::video::IBuffer>();
    entry.field0a = valueType;
    entry.field0c = count;
    entry.field0e = 0;
}

// Heap tables, so the intrusive pointer that wraps them releases them with delete. The table is born with
// count 0, so the (table, true) form takes the first use.
boost::intrusive_ptr<const glitch::video::CVertexStreams> makeTable() {
    return boost::intrusive_ptr<const glitch::video::CVertexStreams>(new glitch::video::CVertexStreams(), true);
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
    // The index bytes stay in this vector; the buffer does not own them.
    std::vector<std::uint8_t> storage = {1, 0, 2, 0, 3, 0};
    glitch::video::CPrimitiveStream stream;
    stream.m_indexBuffer = boost::intrusive_ptr<glitch::video::IBuffer>(
        new io_test_fakes::FakeBuffer(static_cast<std::uint32_t>(storage.size()), storage.data(), false), true);
    stream.m_primitiveType = 5;  // triangle fan: count 3 gives 1 primitive
    stream.m_indexType = 1;
    stream.m_count = 3;
    stream.m_vertexStart = 7;
    stream.m_vertexEnd = 9;

    MemWriteFile file;
    const int returned = glitch::io::save(stream, &file, false);
    CHECK(returned == 24);

    std::vector<std::uint8_t> expected;
    for (std::uint32_t w : {5u, 1u, 3u, 1u, 7u, 9u}) appendLE32(expected, w);
    for (std::uint8_t b : {1, 0, 2, 0, 3, 0}) expected.push_back(b);
    CHECK(file.out == expected);
}

void testSavePrimitiveBE() {
    std::vector<std::uint8_t> storage = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    glitch::video::CPrimitiveStream stream;
    stream.m_indexBuffer = boost::intrusive_ptr<glitch::video::IBuffer>(
        new io_test_fakes::FakeBuffer(static_cast<std::uint32_t>(storage.size()), storage.data(), false), true);
    stream.m_primitiveType = 4;  // triangle strip: count 2 gives 0 primitives
    stream.m_indexType = 2;
    stream.m_count = 2;

    MemWriteFile file;
    glitch::io::save(stream, &file, true);

    std::vector<std::uint8_t> expected;
    for (std::uint32_t w : {4u, 2u, 2u, 0u, 0u, 0u}) appendBE32(expected, w);
    // Each 32-bit index is written byte-reversed: 01 02 03 04 becomes 04 03 02 01.
    for (std::uint8_t b : {0x04, 0x03, 0x02, 0x01, 0x08, 0x07, 0x06, 0x05}) expected.push_back(b);
    CHECK(file.out == expected);
}

void testLoadPrimitive() {
    std::vector<std::uint8_t> bytes;
    for (std::uint32_t w : {5u, 1u, 2u, 2u, 7u, 9u}) appendLE32(bytes, w);
    for (std::uint8_t b : {0xAA, 0xBB, 0xCC, 0xDD}) bytes.push_back(b);
    MemReadFile file(bytes);
    io_test_fakes::TestNullDriver driver;
    glitch::video::CPrimitiveStream stream = glitch::io::loadPS(&file, false, &driver.get());
    CHECK(stream.m_primitiveType == 5);
    CHECK(stream.m_indexType == 1);
    CHECK(stream.m_count == 2);
    CHECK(stream.m_vertexStart == 7);
    CHECK(stream.m_vertexEnd == 9);
    CHECK(stream.m_offset == 0);
    CHECK(stream.m_indexBuffer.get() != nullptr);
    if (stream.m_indexBuffer) {
        CHECK(stream.m_indexBuffer->m_size == 4);
        CHECK(stream.m_indexBuffer->m_data[0] == 0xAA && stream.m_indexBuffer->m_data[3] == 0xDD);
    }
    CHECK(file.pos == 28);
}

// PARTIAL path: with no driver the index bytes are skipped, so the stream has no buffer and the file
// position still moves past the indices.
void testLoadPrimitiveWithoutDriver() {
    std::vector<std::uint8_t> bytes;
    for (std::uint32_t w : {5u, 1u, 2u, 2u, 7u, 9u}) appendLE32(bytes, w);
    for (std::uint8_t b : {0xAA, 0xBB, 0xCC, 0xDD}) bytes.push_back(b);
    MemReadFile file(bytes);
    glitch::video::CPrimitiveStream stream = glitch::io::loadPS(&file, false, nullptr);
    CHECK(stream.m_indexBuffer.get() == nullptr);
    CHECK(stream.m_count == 2 && stream.m_indexType == 1);
    CHECK(file.pos == 28);
}

void testLoadPrimitiveEmpty() {
    std::vector<std::uint8_t> bytes;
    for (std::uint32_t w : {5u, 0xffu, 0u, 0u, 4u, 10u}) appendLE32(bytes, w);
    MemReadFile file(bytes);
    glitch::video::CPrimitiveStream stream = glitch::io::loadPS(&file, false, nullptr);
    CHECK(stream.m_indexBuffer.get() == nullptr);
    CHECK(stream.m_indexType == 0xff);
    CHECK(file.pos == 24);  // the empty stream reads only its header
}

}  // namespace

int main() {
    testStride();
    testSavePrimitiveLE();
    testSavePrimitiveBE();
    testLoadPrimitive();
    testLoadPrimitiveWithoutDriver();
    testLoadPrimitiveEmpty();
    if (failures == 0) std::printf("all r1-glitch-core-io-glitch_io-5e759c-0 tests passed\n");
    return failures == 0 ? 0 : 1;
}
