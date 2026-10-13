// Slice 30 reconstruction (namespace glitch::collada::recon_30).
// Covers glitch::scene::CMeshConnectivity (edge list, triangle adjacency and
// the file save/load pair) and its nested SEdge.
//
// Layout: the CMeshConnectivity member offsets from the original are kept as
// comments (byte offsets). No layout header declares CMeshConnectivity, so the
// class is declared here only. Nothing is attached to an existing header.
// The mesh buffer, vertex stream and index buffer words it reads are not in
// any layout header; they are reached through the stub functions below, and
// the rows that use them are marked partial in the ledger.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-core-io/io/interfaces.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"

namespace glitch::collada::recon_30 {

// glitch::io::IWriteFile and glitch::io::IReadFile, the canonical interfaces (glitch-core-io/io/interfaces.hpp).
// save calls write at vtable +0x0c and load calls read at +0x0c, as the original does.
using glitch::io::IReadFile;
using glitch::io::IWriteFile;

// glitch::scene::CMeshBuffer (the canonical scene class, scene/CMeshBuffer.hpp).
using glitch::scene::CMeshBuffer;

// Stand-in for glitch::os::Printer::log(message, level).
void logMessage(const char* message, int level);

// Unattached stand-ins for the mesh buffer words used by this slice.
// unattached: needs member declarations in layouts/scene.hpp for CMeshBuffer
// (+0x14 vertex streams, +0x18 index buffer, +0x20 index count) and
// CVertexStreams (+0x08 key). Each returns zero or null until those land.
std::uint32_t meshBufferVertexStreamKey(const CMeshBuffer* buffer);  // *(buffer+0x14)+0x08
std::uint32_t meshBufferIndexCount(const CMeshBuffer* buffer);       // buffer+0x20
const std::uint16_t* meshBufferIndices(const CMeshBuffer* buffer);   // mapped index buffer
// Stand-in for IReferenceCounted::drop on the mesh buffer (+0x00 of the connectivity).
void releaseMeshBuffer(CMeshBuffer* buffer);

class CMeshConnectivity {
public:
    // glitch::scene::CMeshConnectivity::SEdge (16 bytes). Two vertex indices
    // (the pair is the edge key), then up to two face indices and a face count.
    struct SEdge {
        std::uint16_t vertex0;   // +0x00 smaller vertex index
        std::uint16_t vertex1;   // +0x02 larger vertex index
        std::uint32_t faces[2];  // +0x04, +0x08 face indices, filled by addFace
        std::uint16_t faceCount; // +0x0c number of faces stored
        std::uint16_t reserved;  // +0x0e padding
        SEdge* addFace(std::uint32_t face);
    };
    static_assert(sizeof(SEdge) == 16, "CMeshConnectivity::SEdge is 16 bytes");

    CMeshConnectivity() = default;
    ~CMeshConnectivity();
    CMeshConnectivity(const CMeshConnectivity&) = delete;
    CMeshConnectivity& operator=(const CMeshConnectivity&) = delete;

    int save(IWriteFile* file);
    int load(IReadFile* file);
    void creatEdgeList(bool weldVertices);
    void addEdgeWithFace(SEdge& edge, std::uint32_t face);

    CMeshBuffer* meshBuffer = nullptr;      // +0x00 refcounted mesh buffer
    std::vector<SEdge> edges;               // +0x04 edge vector (begin, end, capacity in the original)
    std::uint16_t* triangleVertices = nullptr;  // +0x10 three vertex indices per triangle
    std::uint32_t triangleCount = 0;        // +0x14 triangle count (index count / 3)
};

bool operator==(const CMeshConnectivity::SEdge& a, const CMeshConnectivity::SEdge& b);

}  // namespace glitch::collada::recon_30
