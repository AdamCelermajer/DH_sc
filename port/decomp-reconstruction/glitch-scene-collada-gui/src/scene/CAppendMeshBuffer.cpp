// Bodies of glitch::scene::CAppendMeshBuffer, SBatchConfig and SDefaultEndOfBatchCallback (canonical classes:
// scene/CAppendMeshBuffer.hpp). This file replaces the stand-in functions of recon/slice_06 (copyVertexData, copyIndexData,
// SBatchConfig::operator=, the end-of-batch destructor), recon/slice_07 (hasEnoughSpace, beginAppend, setup), recon/slice_25
// (the void* functions of the append buffer) and scene_simple_accessors.cpp (clear).
#include "glitch-scene-collada-gui/scene/CAppendMeshBuffer.hpp"

#include <cstring>

#include "glitch-video/video_driver.hpp"

namespace glitch::scene {

namespace {

// The CPU copy a freshly created buffer gets when the driver gave it none (ctor 0x6b8cb4): the original calls the unbind slot
// (vtable +0x0c) before and after replacing the data. Returns true when a copy was made.
bool giveCpuCopy(video::IBuffer& buffer, std::uint32_t capacity) {
    std::uint8_t flags = buffer.m_flags12;
    bool copied = false;
    if ((flags & video::IBuffer::kFlagDriverMapped) == 0 || (flags & video::IBuffer::kFlagHasData) != 0) {
        if (buffer.m_usage != 4) buffer.bindImpl(false);
        flags = buffer.m_flags12;
        if ((flags & video::IBuffer::kFlagDriverMapped) == 0) {
            buffer.reset(capacity, new std::uint8_t[capacity], true);
            flags = buffer.m_flags12;
            if (((flags & video::IBuffer::kFlagDriverMapped) == 0 || (flags & video::IBuffer::kFlagHasData) != 0) &&
                buffer.m_usage != 4) {
                buffer.bindImpl(false);
            }
            copied = true;
        }
    }
    return copied;
}

video::CPrimitiveStream triangleStream() {
    video::CPrimitiveStream stream;  // no index buffer: index type 0xff
    stream.m_primitiveType = 6;      // triangles
    return stream;
}

}  // namespace

// Not part of the original: an unallocated buffer (no driver, no buffers) for tests.
CAppendMeshBuffer::CAppendMeshBuffer() = default;

// 0x6b8cb4 _ZN6glitch5scene17CAppendMeshBufferC1EjjPNS_5video12IVideoDriverENS2_14E_BUFFER_USAGEEj (complete object)
// 0x6b8fe0 _ZN6glitch5scene17CAppendMeshBufferC2EjjPNS_5video12IVideoDriverENS2_14E_BUFFER_USAGEEj (base object)
// Reconstructed. The base class is built from an empty triangle stream, then both IBuffers are created through the driver
// (vtable +0x78) with no data. A buffer that has no driver-side mapping gets a CPU copy of its capacity; `cpuCopy` (+0x60) is
// set when either did. The index buffer becomes the primitive stream's buffer, and the stream starts with no elements,
// vertices 0 .. 0 and 16-bit indices.
CAppendMeshBuffer::CAppendMeshBuffer(std::uint32_t vertexCapacity, std::uint32_t indexCapacity, video::IVideoDriver* driver,
                                     video::E_BUFFER_USAGE usage, std::uint32_t stride)
    : CMeshBuffer(stride, triangleStream(), true) {
    vertexCapacityBytes = vertexCapacity;
    indexCapacityBytes = indexCapacity;
    vertexBuffer = driver->createBuffer(0, usage, vertexCapacity, nullptr, true);
    indexBuffer = driver->createBuffer(1, usage, indexCapacity, nullptr, true);
    if (giveCpuCopy(*vertexBuffer, vertexCapacity)) cpuCopy = 1;
    if (giveCpuCopy(*indexBuffer, indexCapacity)) cpuCopy = 1;
    primitiveStream.m_indexBuffer = indexBuffer;
    primitiveStream.m_vertexEnd = 0;
    primitiveStream.m_indexType = 1;
    primitiveStream.m_count = 0;
    primitiveStream.m_vertexStart = 0;
}

// 0x6b8c2c _ZN6glitch5scene17CAppendMeshBufferD1Ev
// 0x6b8c98 _ZN6glitch5scene17CAppendMeshBufferD0Ev (skipped)
// Reconstructed: endAppend, then the stream-index vector, the index buffer and the vertex buffer go (the members), then the
// base class.
CAppendMeshBuffer::~CAppendMeshBuffer() {
    endAppend();
}

// 0x6b8878 _ZN6glitch5scene17CAppendMeshBuffer5clearEv
void CAppendMeshBuffer::clear() {
    indexUsedBytes = 0;
    vertexUsedBytes = 0;
}

// 0x5a2040 _ZNK6glitch5scene17CAppendMeshBuffer13hasEnoughSpaceEjj
bool CAppendMeshBuffer::hasEnoughSpace(std::uint32_t vertexCount, std::uint32_t indexCount) const {
    const std::uint32_t usedVertexBytes = vertexUsedBytes;
    return vertexCount <= (vertexCapacityBytes - usedVertexBytes) / vertexStride &&
           indexCount <= (indexCapacityBytes - indexUsedBytes) / indexStride &&
           vertexCount + usedVertexBytes / vertexStride < 0x10000u;
}

// 0x5a26bc _ZN6glitch5scene17CAppendMeshBuffer11beginAppendEv
void CAppendMeshBuffer::beginAppend() {
    if (mappedVertices == nullptr) {
        mappedVertices = vertexBuffer->map(4);
        mappedIndices = indexBuffer->map(4);
    }
}

// 0x6b8aa0 _ZN6glitch5scene17CAppendMeshBuffer9endAppendEv
void CAppendMeshBuffer::endAppend() {
    if (mappedVertices != nullptr) {
        vertexBuffer->unmap();
        indexBuffer->unmap();
        mappedIndices = nullptr;
        mappedVertices = nullptr;
    }
}

// 0x5a8a88 _ZN6glitch5scene17CAppendMeshBuffer5setupEv
// Publishes the appended counts: the primitive stream gets the index count, vertices 0 .. vertexCount and 16-bit indices, and
// the vertex stream table gets the vertex count in its +0x08 word.
void CAppendMeshBuffer::setup() {
    const std::uint32_t indexCount = indexUsedBytes / indexStride;
    const std::uint32_t vertexCount = vertexUsedBytes / vertexStride;
    primitiveStream.m_vertexStart = 0;
    primitiveStream.m_count = indexCount;
    primitiveStream.m_indexType = 1;
    primitiveStream.m_vertexEnd = vertexCount;
    vertexStreams->m_field08 = vertexCount;
}

// 0x6b8b74 _ZN6glitch5scene17CAppendMeshBuffer6resizeEjj
void CAppendMeshBuffer::resize(std::uint32_t vertexCapacity, std::uint32_t indexCapacity) {
    endAppend();
    vertexBuffer->reset(vertexCapacity, nullptr, true);
    indexBuffer->reset(indexCapacity, nullptr, true);
}

// 0x6b8bb4 _ZN6glitch5scene17CAppendMeshBuffer5resetEv
// The loop of the original indexes the stream table with its own counter (0 .. n-1), not with the recorded stream indices;
// that is kept: the first n entries lose their buffers.
void CAppendMeshBuffer::reset() {
    endAppend();
    clear();
    for (std::size_t i = 0; i < streamIndices.size(); ++i) {
        vertexStreams->m_streams[i].buffer.reset();
        vertexStreams->updateHomogeneityInternal(false);
    }
    streamIndices.clear();
}

// 0x6b8950 _ZN6glitch5scene17CAppendMeshBuffer15configureStreamEhiNS_5video29E_VERTEX_ATTRIBUTE_VALUE_TYPEEt
void CAppendMeshBuffer::configureStream(std::uint8_t streamIndex, std::int32_t offset, std::int32_t valueType,
                                        std::uint16_t componentCount) {
    video::SVertexStream& stream = vertexStreams->m_streams[streamIndex];
    stream.buffer = vertexBuffer;
    stream.field0e = 0;
    stream.offset = static_cast<std::uint32_t>(offset);
    stream.field0a = static_cast<std::uint16_t>(valueType);
    stream.field0c = componentCount;
    vertexStreams->updateHomogeneityInternal(false);
    streamIndices.push_back(streamIndex);
}

// 0x6b8834 _ZN6glitch5scene17CAppendMeshBuffer12adjustStrideEt
void CAppendMeshBuffer::adjustStride(std::uint16_t stride) {
    for (std::uint8_t index : streamIndices) {
        vertexStreams->m_streams[index].field0e = stride;
    }
    vertexStride = stride;
}

// 0x58c2c4 _ZNK6glitch5scene17CAppendMeshBuffer14copyVertexDataEPv
void CAppendMeshBuffer::copyVertexData(void* destination) const {
    const video::IBuffer& buffer = *vertexBuffer;
    std::memcpy(destination, buffer.map(1), vertexUsedBytes);
    buffer.unmap();
}

// 0x58c348 _ZNK6glitch5scene17CAppendMeshBuffer13copyIndexDataEPv
void CAppendMeshBuffer::copyIndexData(void* destination) const {
    const video::IBuffer& buffer = *indexBuffer;
    std::memcpy(destination, buffer.map(1), indexUsedBytes);
    buffer.unmap();
}

// 0x58cc74 _ZNK6glitch5scene17CAppendMeshBuffer31allocateConfiguredVertexStreamsERKN5boost13intrusive_ptrINS_5video7IBufferEEE
// Reconstructed. The table is allocated for the mask of the configured stream indices; each configured stream is copied (offset,
// type, component count, stride) into the table's entry for the same attribute, now reading from `buffer`; the table's count word
// is the vertex count. (The original also builds and destroys an unused vector of stream descriptions.)
boost::intrusive_ptr<video::CVertexStreams> CAppendMeshBuffer::allocateConfiguredVertexStreams(
    const boost::intrusive_ptr<video::IBuffer>& buffer) const {
    std::uint32_t mask = 0;
    for (std::uint8_t index : streamIndices) {
        mask |= 1u << index;
    }
    boost::intrusive_ptr<video::CVertexStreams> streams = video::CVertexStreams::allocate(mask);
    for (std::uint8_t index : streamIndices) {
        const video::SVertexStream& source = vertexStreams->m_streams[index];
        video::SVertexStream* target = streams->getStream(source.attribute, streams->m_streams.data(),
                                                          streams->m_streams.data() + streams->m_streams.size());
        target->buffer = buffer;
        target->offset = source.offset;
        target->field0e = source.field0e;
        target->field0a = source.field0a;
        target->field0c = source.field0c;
        streams->updateHomogeneityInternal(false);
    }
    streams->m_field08 = vertexUsedBytes / vertexStride;
    return streams;
}

// 0x58f2a8 _ZN6glitch5scene12SBatchConfigaSERKS1_
SBatchConfig& SBatchConfig::operator=(const SBatchConfig& other) {
    material = other.material;
    for (int i = 0; i < 8; ++i) {
        renderState[i] = other.renderState[i];
    }
    meshBuffer = other.meshBuffer;
    return *this;
}

// 0x58fa04 _ZN6glitch5scene26SDefaultEndOfBatchCallbackD1Ev
// 0x58fa64 _ZN6glitch5scene26SDefaultEndOfBatchCallbackD0Ev (skipped)
// Clears the map (the members release every configuration).
SDefaultEndOfBatchCallback::~SDefaultEndOfBatchCallback() = default;

// 0x59013c _ZN6glitch5scene26SDefaultEndOfBatchCallbackclERKNS0_17CAppendMeshBufferERKN5boost13intrusive_ptrINS_5video9CMaterialEEE
// SKIPPED, not reconstructed (7208 bytes; the member is defined empty so the vtable links): the original raises the largest-buffer counters g_bigV / g_bigI and prints them,
// creates a vertex and an index buffer through the driver (type 0 with the usage member, type 1 with usage 4) and copies the
// appended bytes into them (copyVertexData / copyIndexData), builds the vertex stream table with
// allocateConfiguredVertexStreams and wraps both in a new mesh buffer (index count, vertex count, 16-bit indices, triangles,
// flag byte 0), clones the material under a random name, copies the first eight render-state words of the material's first pass
// into an SBatchConfig, and stores it in the map under the key at compileState + 0x10 (then sets that key to -1). The material
// clone, the render-pass state and the compile-state type are video / compiler internals that are not in this tree.
void SDefaultEndOfBatchCallback::operator()(const CAppendMeshBuffer& /*buffer*/,
                                            const boost::intrusive_ptr<video::CMaterial>& /*material*/) {}

// 0x58d3d4 _ZN6glitch5scene26SDefaultEndOfBatchCallback8finalizeEv
// SKIPPED, not reconstructed (5560 bytes; defined empty): for every stored configuration the original registers a material renderer with the
// driver's CMaterialRendererManager (begin/endMaterialRenderer, beginTechnique("ZeTechnique"), addRenderPass, endTechnique),
// builds a material instance from it, copies the old material's parameters into it and hands the finished mesh buffer to
// CBatchMesh::setBuffer. The renderer manager's builder interface is not exercised here.
void SDefaultEndOfBatchCallback::finalize() {}

}  // namespace glitch::scene
