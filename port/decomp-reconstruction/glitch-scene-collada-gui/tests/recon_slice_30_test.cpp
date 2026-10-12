// Slice 30 test: CMeshConnectivity edge list and save/load round trip.
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>

#include "glitch-scene-collada-gui/recon/slice_30.hpp"

using namespace glitch::collada::recon_30;

namespace {

// One buffer serves as both write and read sink.
struct MemFile : IWriteFile, IReadFile {
    std::vector<unsigned char> data;
    std::size_t pos = 0;

    std::uint32_t write(const void* d, std::uint32_t size) override {
        const unsigned char* p = static_cast<const unsigned char*>(d);
        data.insert(data.end(), p, p + size);
        return size;
    }
    std::uint32_t read(void* d, std::uint32_t size) override {
        if (pos + size > data.size()) return 0;
        std::memcpy(d, data.data() + pos, size);
        pos += size;
        return size;
    }
};

int check(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        return 1;
    }
    return 0;
}

}  // namespace

int main() {
    int failures = 0;

    // addFace: two faces are kept, the third is dropped.
    {
        CMeshConnectivity::SEdge e{};
        e.vertex0 = 1;
        e.vertex1 = 2;
        e.addFace(10);
        e.addFace(11);
        e.addFace(12);
        failures += check(e.faceCount == 2, "addFace caps at two faces");
        failures += check(e.faces[0] == 10 && e.faces[1] == 11, "addFace stores faces in order");
    }

    // addEdgeWithFace: the same vertex pair merges into one edge.
    {
        CMeshConnectivity c;
        CMeshConnectivity::SEdge a{};
        a.vertex0 = 0;
        a.vertex1 = 1;
        c.addEdgeWithFace(a, 0);
        CMeshConnectivity::SEdge b{};
        b.vertex0 = 0;
        b.vertex1 = 1;
        c.addEdgeWithFace(b, 1);
        CMeshConnectivity::SEdge d{};
        d.vertex0 = 1;
        d.vertex1 = 2;
        c.addEdgeWithFace(d, 2);
        failures += check(c.edges.size() == 2, "addEdgeWithFace merges same vertex pair");
        failures += check(c.edges[0].faceCount == 2, "shared edge holds both faces");
    }

    // save then load restores the edges and the triangle count.
    {
        CMeshConnectivity src;
        CMeshConnectivity::SEdge e{};
        e.vertex0 = 3;
        e.vertex1 = 4;
        src.addEdgeWithFace(e, 7);
        src.triangleCount = 1;
        src.triangleVertices = new std::uint16_t[3]{3, 4, 5};

        MemFile file;
        const int written = src.save(&file);
        failures += check(written == static_cast<int>(file.data.size()), "save return matches bytes written");

        CMeshConnectivity dst;
        MemFile readBack;
        readBack.data = file.data;
        // load does not read the adjacency block yet (see ledger), so only the
        // edge list and triangle count are compared here.
        dst.load(&readBack);
        failures += check(dst.edges.size() == 1, "load restores edge count");
        failures += check(dst.edges.size() == 1 && dst.edges[0].vertex0 == 3 && dst.edges[0].vertex1 == 4,
                          "load restores edge vertices");
        failures += check(dst.triangleCount == 1, "load restores triangle count");
    }

    // creatEdgeList with no index data builds nothing and does not crash.
    {
        CMeshConnectivity c;
        c.creatEdgeList(false);
        failures += check(c.triangleCount == 0, "creatEdgeList with no indices has no triangles");
    }

    if (failures != 0) {
        std::fprintf(stderr, "%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("recon_slice_30 tests passed\n");
    return 0;
}
