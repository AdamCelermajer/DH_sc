// Slice 30 reconstruction (see include/glitch-scene-collada-gui/recon/slice_30.hpp).
// Each function keeps the original address and mangled symbol in its comment.

#include "glitch-scene-collada-gui/recon/slice_30.hpp"

#include <algorithm>
#include <cstdio>

namespace glitch::collada::recon_30 {

namespace {

// Tag written after the connectivity data (pseudocode constant); load checks for it.
constexpr std::int32_t kFileTagValue = -1056970744;
// load checks the two header words against these constants.
constexpr std::uint32_t kHeaderCheck1 = 12395596;

constexpr std::uint16_t kMaxFacesPerEdge = 2;

}  // namespace

// Stand-in logger: writes the message to stderr.
void logMessage(const char* message, int level) {
    std::fprintf(stderr, "[glitch level %d] %s\n", level, message);
}

// unattached stand-ins (see header). Return zero or null until the layouts land.
std::uint32_t meshBufferVertexStreamKey(const CMeshBuffer*) { return 0; }
std::uint32_t meshBufferIndexCount(const CMeshBuffer*) { return 0; }
const std::uint16_t* meshBufferIndices(const CMeshBuffer*) { return nullptr; }
void releaseMeshBuffer(CMeshBuffer*) {}

// ---------------------------------------------------------------------------
// glitch::scene::CMeshConnectivity::SEdge
// ---------------------------------------------------------------------------

// 0x7034d4 _ZN6glitch5scene17CMeshConnectivity5SEdge7addFaceEj
CMeshConnectivity::SEdge* CMeshConnectivity::SEdge::addFace(std::uint32_t face) {
    if (faceCount == kMaxFacesPerEdge) {
        logMessage("Edge Warning : An edge have more than 2 faces, it might gives unexpected results.", 3);
        return this;
    }
    faces[faceCount] = face;
    faceCount = static_cast<std::uint16_t>(faceCount + 1);
    return this;
}

// The edge key is the vertex pair. The original's equality operator is not in
// the pseudocode of this slice; the pair comparison here is an assumption.
bool operator==(const CMeshConnectivity::SEdge& a, const CMeshConnectivity::SEdge& b) {
    return a.vertex0 == b.vertex0 && a.vertex1 == b.vertex1;
}

// ---------------------------------------------------------------------------
// glitch::scene::CMeshConnectivity
// ---------------------------------------------------------------------------

// 0x70336c _ZN6glitch5scene17CMeshConnectivity4saveEPNS_2io10IWriteFileE
// partial: the vertex stream key and the index count come from the unattached
// stubs above (see header). The write sequence and tag follow the pseudocode.
int CMeshConnectivity::save(IWriteFile* file) {
    int total = 0;

    const std::uint32_t streamKey = meshBufferVertexStreamKey(meshBuffer);
    total += static_cast<int>(file->write(&streamKey, 4));

    const std::uint32_t indexCount = meshBufferIndexCount(meshBuffer);
    total += static_cast<int>(file->write(&indexCount, 4));

    const std::uint32_t edgeCount = static_cast<std::uint32_t>(edges.size());
    total += static_cast<int>(file->write(&edgeCount, 4));

    for (const SEdge& edge : edges) {
        total += static_cast<int>(file->write(&edge, sizeof(SEdge)));
    }

    total += static_cast<int>(file->write(&triangleCount, 4));
    total += static_cast<int>(file->write(triangleVertices, 6 * triangleCount));

    const std::int32_t tag = kFileTagValue;
    total += static_cast<int>(file->write(&tag, 4));
    return total;
}

// 0x7037d4 _ZN6glitch5scene17CMeshConnectivity15addEdgeWithFaceERNS1_5SEdgeEj
// partial: SEdge equality is assumed to compare the vertex pair (see above).
void CMeshConnectivity::addEdgeWithFace(SEdge& edge, std::uint32_t face) {
    auto it = std::find(edges.begin(), edges.end(), edge);
    if (it == edges.end()) {
        edge.addFace(face);
        edges.push_back(edge);
    } else {
        it->addFace(face);
    }
}

// 0x703864 _ZN6glitch5scene17CMeshConnectivity4loadEPNS_2io9IReadFileE
// partial: the pseudocode has an unnamed one-argument call at vtable offset 12
// after the adjacency reset. It is not reproduced because it has no size or
// buffer argument. The header check uses the constants from the pseudocode, and
// the adjacency is reset to 0xFFFF rather than read, as in the pseudocode.
int CMeshConnectivity::load(IReadFile* file) {
    int total = 0;

    std::uint32_t header[2] = {0, 0};
    total += static_cast<int>(file->read(header, 8));
    if (header[0] != 0 || header[1] != kHeaderCheck1) {
        logMessage("Mesh Connectivity load : The vertex or index count in the file does not match the current mesh", 3);
    }

    std::uint32_t edgeCount = 0;
    total += static_cast<int>(file->read(&edgeCount, 4));

    // The original resets the end pointer to the begin pointer here.
    edges.clear();
    edges.reserve(edgeCount);

    for (std::uint32_t i = 0; i < edgeCount; ++i) {
        SEdge edge{};
        total += static_cast<int>(file->read(&edge, sizeof(SEdge)));
        edges.push_back(edge);
    }

    total += static_cast<int>(file->read(&triangleCount, 4));

    delete[] triangleVertices;
    triangleVertices = new std::uint16_t[3 * static_cast<std::size_t>(triangleCount)];
    for (std::uint32_t i = 0; i < 3 * triangleCount; ++i) {
        triangleVertices[i] = 0xFFFFu;
    }

    std::uint32_t tag = 0;
    total += static_cast<int>(file->read(&tag, 4));
    if (tag != static_cast<std::uint32_t>(kFileTagValue)) {
        logMessage("Mesh Connectivity load : error loading the file...", 3);
    }
    return total;
}

// 0x703acc _ZN6glitch5scene17CMeshConnectivity13creatEdgeListEb
// partial: welding (weldVertices) is not reconstructed. It needs the vertex
// stream positions, which are not in a layout header. The index data comes from
// the unattached stubs, so the function is a no-op until those are mapped. The
// triangle and edge build below follows the pseudocode tail.
void CMeshConnectivity::creatEdgeList(bool weldVertices) {
    (void)weldVertices;

    const std::uint32_t indexCount = meshBufferIndexCount(meshBuffer);
    const std::uint16_t* indices = meshBufferIndices(meshBuffer);

    const std::uint32_t newTriangleCount = indexCount / 3;
    std::vector<std::uint16_t> remapped(indices ? indexCount : 0);
    for (std::uint32_t i = 0; i < remapped.size(); ++i) {
        remapped[i] = indices[i];
    }

    delete[] triangleVertices;
    triangleVertices = new std::uint16_t[3 * static_cast<std::size_t>(newTriangleCount)];
    for (std::uint32_t i = 0; i < 3 * newTriangleCount; ++i) {
        triangleVertices[i] = 0xFFFFu;
    }
    triangleCount = newTriangleCount;

    for (std::uint32_t t = 0; t < newTriangleCount; ++t) {
        const std::uint16_t a = remapped[3 * t];
        const std::uint16_t b = remapped[3 * t + 1];
        const std::uint16_t c = remapped[3 * t + 2];
        triangleVertices[3 * t] = a;
        triangleVertices[3 * t + 1] = b;
        triangleVertices[3 * t + 2] = c;

        SEdge ab{};
        ab.vertex0 = std::min(a, b);
        ab.vertex1 = std::max(a, b);
        addEdgeWithFace(ab, t);

        SEdge bc{};
        bc.vertex0 = std::min(b, c);
        bc.vertex1 = std::max(b, c);
        addEdgeWithFace(bc, t);

        SEdge ac{};
        ac.vertex0 = std::min(a, c);
        ac.vertex1 = std::max(a, c);
        addEdgeWithFace(ac, t);
    }
}

// 0x70355c _ZN6glitch5scene17CMeshConnectivityD1Ev
// 0x70359c _ZN6glitch5scene17CMeshConnectivityD2Ev
// Both symbols are the same body (the complete and base destructors).
// partial: releaseMeshBuffer is an unattached stub, so the refcount drop of
// the mesh buffer is not reproduced. The edge vector is freed by std::vector.
CMeshConnectivity::~CMeshConnectivity() {
    delete[] triangleVertices;
    if (meshBuffer) {
        releaseMeshBuffer(meshBuffer);
    }
}

}  // namespace glitch::collada::recon_30
