// Batch r1-glitch-core-io-glitch_io-5e759c-0 (glitch::io free functions, redo pass).
// Bodies follow the Hex-Rays pseudocode in recovered/native/hexrays-all. Byte order is handled
// explicitly, so the code does not depend on the host's endianness, except where a raw block
// is copied (the original's native copy, which is little-endian on the target).

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "glitch-core-io/io/irr_xml_file_read_callback.hpp"
#include "glitch-core-io/io/stream_itr_writer.hpp"
#include "glitch-core-io/tb/glitch_io.hpp"

namespace glitch::io {
namespace {

std::uint32_t byteSwap32(std::uint32_t value) {
    return (value >> 24) | ((value >> 8) & 0x0000ff00u) | ((value << 8) & 0x00ff0000u) | (value << 24);
}

// glitch::video::detail::SVertexAttributeTypeInspection::ValueTypeSize: 7 bytes at 0x8e4ca4 in the
// ELF, element size of each vertex value type (glitch-video/src/vertex_streams.cpp has the same table).
constexpr std::uint8_t kValueTypeSize[7] = {1, 1, 2, 2, 4, 4, 4};

// CSWTCH_4544: element size of index types 1 and 2 (DCD 2, 4 in the ARM listing of save).
// The getIndexTypeSize table used by loadPS and loadVS is not read; these two entries are the same
// values, and other index types have size 0 here.
std::uint32_t indexSize(std::uint32_t indexType) {
    if (indexType == 1) return 2;
    if (indexType == 2) return 4;
    return 0;
}

constexpr int kPrimitiveHeaderBytes = 24;  // six 32-bit words

// Reads one 32-bit word. swap selects big-endian file order; otherwise little-endian (the target).
std::uint32_t readWord(IReadFile* file, bool swap) {
    std::uint8_t b[4] = {0, 0, 0, 0};
    file->read(b, 4);
    if (swap) return (std::uint32_t(b[0]) << 24) | (std::uint32_t(b[1]) << 16) | (std::uint32_t(b[2]) << 8) | b[3];
    return (std::uint32_t(b[3]) << 24) | (std::uint32_t(b[2]) << 16) | (std::uint32_t(b[1]) << 8) | b[0];
}

// Reads one 16-bit value, with the same byte order rule as readWord.
std::uint16_t readHalf(IReadFile* file, bool swap) {
    std::uint8_t b[2] = {0, 0};
    file->read(b, 2);
    if (swap) return static_cast<std::uint16_t>((b[0] << 8) | b[1]);
    return static_cast<std::uint16_t>((b[1] << 8) | b[0]);
}

// Reads a 12-byte vector3d<float> block. Under swap every 32-bit word is reversed (the original's
// byteswap<float> per component). The values are not kept: see the notes of loadVS.
void skipVector3Pair(IReadFile* file, bool swap) {
    std::uint8_t raw[12];
    file->read(raw, 12);
    (void)swap;
}

// Stores a 16-bit value at element i of a buffer in little-endian order (the original's host order).
void storeHalfLE(std::uint8_t* dst, std::uint16_t value) {
    dst[0] = static_cast<std::uint8_t>(value & 0xff);
    dst[1] = static_cast<std::uint8_t>(value >> 8);
}

void storeWordLE(std::uint8_t* dst, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) dst[i] = static_cast<std::uint8_t>(value >> (8 * i));
}

// Reads the vertex stream headers (count entries of 12 bytes). mask gets the bits of the attributes
// (bit = low byte of the attribute), as the original's loop ORs them in.
std::vector<SVertexStreamHeader> readStreamHeaders(IReadFile* file, std::uint32_t count, bool swap,
                                                   std::uint32_t& mask) {
    std::vector<SVertexStreamHeader> headers;
    mask = 0;
    for (std::uint32_t i = 0; i < count; ++i) {
        SVertexStreamHeader header;
        header.offset = readWord(file, swap);
        header.attribute = readHalf(file, swap);
        header.valueType = readHalf(file, swap);
        header.count = readHalf(file, swap);
        header.stride = readHalf(file, swap);
        mask |= 1u << (header.attribute & 0xff);  // PARTIAL: the original shifts by a char; 32 bits here
        headers.push_back(header);
    }
    return headers;
}

// Number of set bits in attribute bits 1..16 of mask: the count0c rule recorded for allocate(mask)
// in glitch-video/include/glitch-video/vertex_streams.hpp.
std::uint8_t count0cForMask(std::uint32_t mask) {
    std::uint8_t count = 0;
    for (int bit = 1; bit <= 16; ++bit) {
        if (mask & (1u << bit)) ++count;
    }
    return count;
}

// Driver buffers of the index and vertex data: type 1 (index) or 0 (vertex), usage 4, and the bytes in
// zeroed storage that the buffer owns (IVideoDriver::createBuffer with flag true, vtable +0x78). The call
// is the one the listings make in loadPS (0x6b61fc, createBuffer(1, 4, bytes, new[](bytes), 1)) and loadVS
// (0x6b73d8, createBuffer(0, 4, bytes, new[](bytes), 1)). The result already holds one use.
constexpr glitch::video::E_BUFFER_TYPE kVertexBufferType = 0;
constexpr glitch::video::E_BUFFER_TYPE kIndexBufferType = 1;
constexpr glitch::video::E_BUFFER_USAGE kDataBufferUsage = 4;

boost::intrusive_ptr<glitch::video::IBuffer> createDataBuffer(glitch::video::IVideoDriver* driver,
                                                              glitch::video::E_BUFFER_TYPE type,
                                                              std::uint32_t bytes) {
    std::uint8_t* storage = new std::uint8_t[bytes]();
    return driver->createBuffer(type, kDataBufferUsage, bytes, storage, true);
}

}  // namespace

// 0x578670 _ZN6glitch2io10fromStringERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEEPNS_5video12IVideoDriverE
// PARTIAL: the original takes the text before the first ';' as a texture name and calls the driver's
// texture manager (driver +0xe0) with it. The texture manager is not in the port, so the result is
// null. The text after ';' is built by the original and then dropped (the listing never reads it).
boost::intrusive_ptr<glitch::video::ITexture> fromString(const std::string& text,
                                                         glitch::video::IVideoDriver* driver) {
    boost::intrusive_ptr<glitch::video::ITexture> result;
    if (driver == nullptr || text.empty()) return result;
    const std::size_t semicolon = text.find(';');
    const std::string name = text.substr(0, semicolon);  // npos: the whole text
    (void)name;  // the texture manager lookup by this name is not reconstructed
    return result;
}

// 0x5787cc _ZN6glitch2io8toStringERKN5boost13intrusive_ptrINS_5video8ITextureEEEPNS3_12IVideoDriverE
// PARTIAL: the original writes the texture's name (the texture manager node's name, or "<null>"),
// then ';', then a string held by the texture. The texture manager node and the texture string are
// not laid out in the port, so only the empty result for a null texture or driver is reproduced.
std::string toString(const boost::intrusive_ptr<glitch::video::ITexture>& texture,
                     glitch::video::IVideoDriver* driver) {
    std::string result;
    if (texture.get() == nullptr || driver == nullptr) return result;
    return result;
}

// 0x575268 _ZN6glitch2io16createIXMLReaderEPNS0_9IReadFileE
CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>* createIXMLReader(IReadFile* file) {
    if (file == nullptr) return nullptr;
    // The callback takes a reference on the file (the original increments IReadFile's count
    // at +0x04 here); the callback's destructor releases it.
    ++file->referenceCounter;
    auto* callback = new CIrrXMLFileReadCallBack(file);
    auto* reader = new CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>();
    reader->parserFormat = 5;  // +0x20
    reader->readFile(callback);
    delete callback;  // the original calls the deleting destructor through vtable +0x04
    reader->createSpecialCharacterList();
    reader->current = reader->textData;  // +0x0c = +0x10
    return reader;
}

// 0x576270 _ZN6glitch2io20createIXMLReaderUTF8EPNS0_9IReadFileE
CXMLReaderImpl<char, glitch::IReferenceCounted>* createIXMLReaderUTF8(IReadFile* file) {
    if (file == nullptr) return nullptr;
    ++file->referenceCounter;
    auto* callback = new CIrrXMLFileReadCallBack(file);
    auto* reader = new CXMLReaderImpl<char, glitch::IReferenceCounted>();
    reader->parserFormat = 1;  // +0x20
    reader->readFile(callback);
    delete callback;
    reader->createSpecialCharacterList();
    reader->current = reader->textData;  // +0x0c = +0x10
    return reader;
}

// 0x6b58a8 _ZN6glitch2io22getVertexStreamsStrideERKN5boost13intrusive_ptrIKNS_5video14CVertexStreamsEEE
// PARTIAL: an entry with a bound buffer whose value type is past the 7-entry ValueTypeSize table
// reads past the table in the original (the value is undefined there). The port uses an element
// size of 1 for such an entry; valid streams never reach it. The original does not test the
// table pointer for null, and neither does this port.
unsigned int getVertexStreamsStride(const boost::intrusive_ptr<const glitch::video::CVertexStreams>& streams) {
    const auto& entries = streams->m_streams;
    if (entries.empty()) return 0;

    auto elementSize = [](std::uint16_t valueType) -> unsigned int {
        return valueType < 7 ? kValueTypeSize[valueType] : 1u;  // see PARTIAL above
    };

    // Each bound stream: pad the running total to its element size, then add its components.
    unsigned int stride = 0;
    for (const auto& entry : entries) {
        if (entry.buffer == nullptr) continue;
        const unsigned int size = elementSize(entry.field0a);
        const unsigned int pad = (size - stride % size) % size;
        stride += entry.field0c * size + pad;
    }
    // The first bound stream aligns the total to its element size.
    for (const auto& entry : entries) {
        if (entry.buffer == nullptr) continue;
        const unsigned int size = elementSize(entry.field0a);
        stride += (size - stride % size) % size;
        break;
    }
    return stride;
}

// 0x6b5dd4 _ZN6glitch2io4saveERKNS_5video16CPrimitiveStreamEPNS0_10IWriteFileEb
// The header is six words: primitive type, index type, count, primitive count, unk_0c, unk_10.
// With bigEndian each word is byte-swapped. Without it the indices are one raw write of
// count * element size bytes; with it they are written element by element, byte-swapped.
// Index types other than 1 and 2 write nothing in the swapped path, and 0 bytes in the raw path.
int save(const glitch::video::CPrimitiveStream& stream, IWriteFile* file, bool bigEndian) {
    auto writeWord = [&](std::uint32_t value) {
        const std::uint32_t word = bigEndian ? byteSwap32(value) : value;
        file->write(&word, 4);
    };
    writeWord(stream.m_primitiveType);
    writeWord(stream.m_indexType);
    writeWord(stream.m_count);
    writeWord(stream.getPrimitiveCount());
    writeWord(stream.m_vertexStart);
    writeWord(stream.m_vertexEnd);

    if (!stream.m_indexBuffer) return kPrimitiveHeaderBytes;
    const std::uint8_t* base = stream.m_indexBuffer->m_data + stream.m_offset;
    const std::uint32_t count = stream.m_count;

    if (!bigEndian) {
        const std::uint32_t bytes = count * indexSize(stream.m_indexType);
        file->write(base, bytes);
        return kPrimitiveHeaderBytes;
    }
    if (stream.m_indexType == 0) {
        file->write(base, 0);
        return kPrimitiveHeaderBytes;
    }
    if (stream.m_indexType == 1) {
        for (std::uint32_t i = 0; i < count; ++i) {
            const std::uint8_t* element = base + 2 * i;
            const std::uint8_t out[2] = {element[1], element[0]};
            file->write(out, 2);
        }
    } else if (stream.m_indexType == 2) {
        for (std::uint32_t i = 0; i < count; ++i) {
            const std::uint8_t* element = base + 4 * i;
            const std::uint8_t out[4] = {element[3], element[2], element[1], element[0]};
            file->write(out, 4);
        }
    }
    return kPrimitiveHeaderBytes;
}

// 0x6b61fc _ZN6glitch2io6loadPSEPNS0_9IReadFileEbPNS_5video12IVideoDriverE
// Reads the 24-byte header, then the indices. An index type of 0xff marks an empty stream (no
// buffer). The index bytes go into a driver buffer (createBuffer(1, 4, bytes, storage, true), as the
// listing does). PARTIAL: with no driver (the original would call through a null pointer) the index
// bytes are skipped and the stream has no buffer; the buffer is written through m_data, not through
// a driver map call. The empty-stream count (word 5 minus word 4) is taken from the pseudocode and
// not checked against the ARM listing.
glitch::video::CPrimitiveStream loadPS(IReadFile* file, bool bigEndian, glitch::video::IVideoDriver* driver) {
    glitch::video::CPrimitiveStream stream;

    std::uint8_t raw[kPrimitiveHeaderBytes];
    file->read(raw, kPrimitiveHeaderBytes);
    auto word = [&](int index) -> std::uint32_t {
        const std::uint8_t* b = raw + 4 * index;
        if (bigEndian) return (std::uint32_t(b[0]) << 24) | (std::uint32_t(b[1]) << 16) | (std::uint32_t(b[2]) << 8) | b[3];
        return (std::uint32_t(b[3]) << 24) | (std::uint32_t(b[2]) << 16) | (std::uint32_t(b[1]) << 8) | b[0];
    };
    const std::uint32_t primitiveType = word(0);
    const std::uint32_t indexType = word(1);
    const std::uint32_t count = word(2);
    const std::uint32_t word4 = word(4);
    const std::uint32_t word5 = word(5);

    stream.m_primitiveType = static_cast<std::uint16_t>(primitiveType);
    stream.m_offset = 0;
    stream.m_vertexStart = word4;
    stream.m_vertexEnd = word5;
    if (indexType == 0xff) {
        // Empty stream: no buffer. The original stores word5 - word4 as the count.
        stream.m_count = word5 - word4;
        stream.m_indexType = static_cast<std::uint16_t>(indexType);
        return stream;
    }
    stream.m_count = count;
    stream.m_indexType = static_cast<std::uint16_t>(indexType);

    const std::uint32_t bytes = count * indexSize(indexType);
    if (driver == nullptr) {
        // PARTIAL: no driver to create the buffer; the index bytes are skipped so the file position stays right.
        file->seek(static_cast<long>(bytes), true);
        return stream;
    }
    stream.m_indexBuffer = createDataBuffer(driver, kIndexBufferType, bytes);

    std::uint8_t* data = stream.m_indexBuffer->m_data;
    if (bigEndian && indexType == 1) {
        for (std::uint32_t i = 0; i < count; ++i) {
            std::uint8_t in[2];
            file->read(in, 2);
            storeHalfLE(data + 2 * i, static_cast<std::uint16_t>((in[0] << 8) | in[1]));
        }
    } else if (bigEndian && indexType == 2) {
        for (std::uint32_t i = 0; i < count; ++i) {
            std::uint8_t in[4];
            file->read(in, 4);
            storeWordLE(data + 4 * i, (std::uint32_t(in[0]) << 24) | (std::uint32_t(in[1]) << 16) |
                                          (std::uint32_t(in[2]) << 8) | in[3]);
        }
    } else if (!bigEndian || indexType == 0) {
        file->read(data, bytes);
    }
    // bigEndian with another index type reads nothing (as the original).
    return stream;
}

// 0x6b6cac _ZN6glitch2io22loadHeadersAndSkipDataEPNS0_9IReadFileERNS0_20SPrimitiveStreamDescEb
// Reads the vertex stream block without its vertex data: the stream headers, the trailing vector
// pairs, the vertex count, then skips the vertex bytes, reads the primitive stream header into
// desc and skips its indices. PARTIAL: the table is sized by the header count, not by allocate's
// mask rule; the stream flags word (+0x0e) and the trailing block are read and discarded; the
// stand-in has no storage for them.
boost::intrusive_ptr<glitch::video::CVertexStreams> loadHeadersAndSkipData(IReadFile* file,
                                                                           SPrimitiveStreamDesc& desc,
                                                                           bool bigEndian) {
    std::uint8_t flagByte = 0;
    file->read(&flagByte, 1);
    const std::uint32_t headerCount = readWord(file, bigEndian);
    std::uint32_t mask = 0;
    const std::vector<SVertexStreamHeader> headers = readStreamHeaders(file, headerCount, bigEndian, mask);

    auto* table = new glitch::video::CVertexStreams();
    // The canonical table is born with count 0: the (table, true) form takes the first use.
    boost::intrusive_ptr<glitch::video::CVertexStreams> result(table, true);
    table->m_count0c = count0cForMask(mask);
    table->m_streams.resize(headers.size());
    for (std::size_t i = 0; i < headers.size(); ++i) {
        auto& entry = table->m_streams[i];
        entry.buffer = nullptr;
        entry.offset = headers[i].offset;
        entry.attribute = headers[i].attribute;
        entry.field0a = headers[i].valueType;
        entry.field0c = headers[i].count;
        entry.field0e = headers[i].stride;
    }

    // Trailing block: two vector3d pairs, then count0c more pairs (read and discarded here).
    skipVector3Pair(file, bigEndian);
    skipVector3Pair(file, bigEndian);
    for (std::uint8_t k = 0; k < table->m_count0c; ++k) {
        skipVector3Pair(file, bigEndian);
        skipVector3Pair(file, bigEndian);
    }

    const std::uint32_t vertexCount = readWord(file, false);  // not byte-swapped, as the original
    table->m_field08 = vertexCount;
    const std::uint32_t stride = headers.empty() ? 0 : headers[0].stride;
    file->seek(static_cast<long>(vertexCount * stride), true);

    // Primitive stream header (24 bytes, swapped per word under bigEndian), then its indices.
    std::uint8_t raw[kPrimitiveHeaderBytes];
    file->read(raw, kPrimitiveHeaderBytes);
    auto word = [&](int index) -> std::uint32_t {
        const std::uint8_t* b = raw + 4 * index;
        if (bigEndian) return (std::uint32_t(b[0]) << 24) | (std::uint32_t(b[1]) << 16) | (std::uint32_t(b[2]) << 8) | b[3];
        return (std::uint32_t(b[3]) << 24) | (std::uint32_t(b[2]) << 16) | (std::uint32_t(b[1]) << 8) | b[0];
    };
    desc.primitiveType = word(0);
    desc.indexType = word(1);
    desc.count = word(2);
    desc.primitiveCount = word(3);
    desc.unk_10 = word(4);
    desc.unk_14 = word(5);
    file->seek(static_cast<long>(desc.count * indexSize(desc.indexType)), true);
    (void)flagByte;
    return result;
}

// 0x6b73d8 _ZN6glitch2io6loadVSEPNS0_9IReadFileEbPNS_5video12IVideoDriverE
// Reads the vertex stream block: headers, trailing pairs, the vertex count and the vertex bytes.
// PARTIAL, as loadHeadersAndSkipData, plus: the vertex bytes are read as one raw block for both
// byte orders (the original swaps per element with SStreamItrLoader when bigEndian, and that
// iterator's 64-byte element layout is not the port's); the vertex buffer is a private IBuffer,
// not a driver buffer (vtable +0x78 is not in the port); every stream entry shares that buffer
// (as setStream is given the same buffer in the original).
boost::intrusive_ptr<glitch::video::CVertexStreams> loadVS(IReadFile* file, bool bigEndian,
                                                           glitch::video::IVideoDriver* driver) {
    const std::uint32_t headerCount = readWord(file, bigEndian);
    std::uint32_t mask = 0;
    const std::vector<SVertexStreamHeader> headers = readStreamHeaders(file, headerCount, bigEndian, mask);

    auto* table = new glitch::video::CVertexStreams();
    // The canonical table is born with count 0: the (table, true) form takes the first use.
    boost::intrusive_ptr<glitch::video::CVertexStreams> result(table, true);
    table->m_count0c = count0cForMask(mask);
    table->m_streams.resize(headers.size());

    skipVector3Pair(file, bigEndian);
    skipVector3Pair(file, bigEndian);
    for (std::uint8_t k = 0; k < table->m_count0c; ++k) {
        skipVector3Pair(file, bigEndian);
        skipVector3Pair(file, bigEndian);
    }

    const std::uint32_t vertexCount = readWord(file, false);
    table->m_field08 = vertexCount;
    const std::uint32_t stride = headers.empty() ? 0 : headers[0].stride;
    const std::uint32_t bytes = vertexCount * stride;

    boost::intrusive_ptr<glitch::video::IBuffer> buffer;
    if (driver != nullptr) {
        buffer = createDataBuffer(driver, kVertexBufferType, bytes);
        if (bytes != 0) file->read(buffer->m_data, bytes);
    } else {
        // PARTIAL: no driver to create the buffer; the vertex bytes are skipped and the streams have no buffer.
        file->seek(static_cast<long>(bytes), true);
    }

    for (std::size_t i = 0; i < headers.size(); ++i) {
        auto& entry = table->m_streams[i];
        entry.buffer = buffer;
        entry.offset = headers[i].offset;
        entry.attribute = headers[i].attribute;
        entry.field0a = headers[i].valueType;
        entry.field0c = headers[i].count;
        entry.field0e = headers[i].stride;
    }
    return result;
}

// 0x6b7c1c _ZN6glitch2io6loadMBEPNS0_9IReadFileEbPNS_5video12IVideoDriverE
// Reads a mesh buffer: one flag byte, the vertex streams and the index stream. PARTIAL: the
// CMeshBuffer object (its streams at +0x14, index stream at +0x18, flag at +0x34) has no layout in
// glitch-core-io's stand-ins, so the result is null and the parsed parts are discarded.
boost::intrusive_ptr<glitch::scene::CMeshBuffer> loadMB(IReadFile* file, bool bigEndian,
                                                        glitch::video::IVideoDriver* driver) {
    std::uint8_t flag = 0;
    file->read(&flag, 1);
    auto streams = loadVS(file, bigEndian, driver);
    auto indices = loadPS(file, bigEndian, driver);
    (void)streams;
    (void)indices;
    (void)flag;
    return boost::intrusive_ptr<glitch::scene::CMeshBuffer>();
}

// 0x6b7d84 _ZN6glitch2io4saveERKN5boost13intrusive_ptrIKNS_5video14CVertexStreamsEEEPNS0_10IWriteFileEb
// Writes the vertex stream block. PARTIAL: the stream header table (count, then one 12-byte header
// per bound stream with its aligned offset, attribute, value type, count and the total stride),
// the vertex count and the per-vertex data are written; the trailing vector pairs are not (the
// stand-in has no storage for them). Returns 12 * headers + 24 * count0c + 32, the byte count of
// the header section as the listing computes it.
int save(const boost::intrusive_ptr<const glitch::video::CVertexStreams>& streams, IWriteFile* file,
         bool bigEndian) {
    const auto& entries = streams->m_streams;

    // Header table: aligned running offset for each bound stream (as getVertexStreamsStride).
    struct Header {
        std::uint32_t offset;
        std::uint16_t attribute;
        std::uint16_t valueType;
        std::uint16_t count;
    };
    std::vector<Header> headers;
    std::uint32_t total = 0;
    for (const auto& entry : entries) {
        if (entry.buffer == nullptr) continue;
        const std::uint32_t size = entry.field0a < 7 ? kValueTypeSize[entry.field0a] : 1u;
        const std::uint32_t pad = (size - total % size) % size;
        headers.push_back({total + pad, entry.attribute, entry.field0a, entry.field0c});
        total += pad + entry.field0c * size;
    }
    std::uint16_t totalStride = static_cast<std::uint16_t>(total);
    if (!headers.empty()) {
        const std::uint32_t size = headers.front().valueType < 7 ? kValueTypeSize[headers.front().valueType] : 1u;
        totalStride = static_cast<std::uint16_t>(total + (size - total % size) % size);
    }

    auto writeWord = [&](std::uint32_t value) {
        const std::uint32_t word = bigEndian ? byteSwap32(value) : value;
        file->write(&word, 4);
    };
    auto writeHalf = [&](std::uint16_t value) {
        const std::uint16_t half = bigEndian ? static_cast<std::uint16_t>((value >> 8) | (value << 8)) : value;
        file->write(&half, 2);
    };
    writeWord(static_cast<std::uint32_t>(headers.size()));
    for (const auto& header : headers) {
        writeWord(header.offset);
        writeHalf(header.attribute);
        writeHalf(header.valueType);
        writeHalf(header.count);
        writeHalf(totalStride);
    }
    // PARTIAL: the trailing block (two vector3d pairs and count0c pairs) is not written here.
    writeWord(streams->m_field08);

    // Vertex data: one SStreamItrWriter per bound stream. For each vertex, a stream is padded to its
    // element size with zero bytes, then its components are written and the source advances by the
    // stream stride (SStreamItrWriter::writeAndAdvance, which also does the byte swap).
    static const std::uint8_t kPadding[1] = {0};
    std::vector<SStreamItrWriter> writers;
    std::vector<std::uint32_t> elementSizes;
    for (const auto& entry : entries) {
        if (entry.buffer == nullptr) continue;
        SStreamItrWriter writer{};
        writer.data = entry.buffer->m_data + entry.offset;
        writer.count = entry.field0c;
        writer.elementSize = entry.field0a < 7 ? kValueTypeSize[entry.field0a] : 1u;
        writer.stride = entry.field0e;
        writers.push_back(writer);
        elementSizes.push_back(writer.elementSize);
    }
    std::uint32_t position = 0;
    for (std::uint32_t vertex = 0; vertex < streams->m_field08; ++vertex) {
        for (std::size_t i = 0; i < writers.size(); ++i) {
            const std::uint32_t size = elementSizes[i];
            const std::uint32_t pad = (size - position % size) % size;
            for (std::uint32_t p = 0; p < pad; ++p) file->write(kPadding, 1);
            writers[i].writeAndAdvance(file, bigEndian);
            position += pad + static_cast<std::uint32_t>(writers[i].count) * size;
        }
    }
    const std::uint32_t finalPad = totalStride == 0 ? 0 : (totalStride - position % totalStride) % totalStride;
    for (std::uint32_t p = 0; p < finalPad; ++p) file->write(kPadding, 1);

    return static_cast<int>(12 * headers.size() + 24 * streams->m_count0c + 32);
}

// 0x6b875c _ZN6glitch2io4saveERKN5boost13intrusive_ptrIKNS_5scene11CMeshBufferEEEPNS0_10IWriteFileEbPjSB_
// PARTIAL: writes the mesh buffer's flag byte, the vertex streams and the index stream. The
// CMeshBuffer layout is not in glitch-core-io's stand-ins (see loadMB), so nothing is written and
// the result is 0; the out parameters are left unchanged.
int save(const boost::intrusive_ptr<const glitch::scene::CMeshBuffer>& mesh, IWriteFile* file,
         bool bigEndian, unsigned int* vertexResult, unsigned int* indexResult) {
    (void)mesh;
    (void)file;
    (void)bigEndian;
    (void)vertexResult;
    (void)indexResult;
    return 0;
}

}  // namespace glitch::io
