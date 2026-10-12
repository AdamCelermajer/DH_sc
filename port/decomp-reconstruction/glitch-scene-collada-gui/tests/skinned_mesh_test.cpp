#include "glitch-scene-collada-gui/skinned_mesh.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    SMeshBufferRef mb;
    SCountedRef mat, map;
    SSkinnedBufferEntry entries[2];
    entries[1].meshBuffer = &mb;
    entries[1].material = &mat;
    entries[1].vertexAttributeMap = &map;
    CSkinnedMesh m;
    m.buffers = entries;
    CHECK(m.getType() == 2);
    CHECK(m.clone() == nullptr);
    SMeshBufferRef* outMb = nullptr;
    m.getMeshBuffer(outMb, 1);
    CHECK(outMb == &mb && mb.count == 1);
    SCountedRef* outMat = nullptr;
    m.getMaterial(outMat, 1);
    CHECK(outMat == &mat && mat.count == 1);
    SCountedRef* outMap = nullptr;
    m.getMaterialVertexAttributeMap(outMap, 1);
    CHECK(outMap == &map && map.count == 1);
    std::printf("%s\n", failures ? "skinned_mesh: FAILED" : "skinned_mesh: ok");
    return failures != 0;
}
