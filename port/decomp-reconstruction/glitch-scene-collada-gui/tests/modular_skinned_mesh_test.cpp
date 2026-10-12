#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CModularSkinnedMesh m;
    CModularSkinnedMesh::SCategoryTable table;
    table.count = 3;
    m.categories = &table;
    m.currentModules.resize(3);
    m.currentModules[1].moduleId = 4;
    CHECK(m.getType() == 3);
    CHECK(m.clone() == nullptr);
    CHECK(m.getCategoryCount() == 3);
    CHECK(m.getCurrentModuleId(1) == 4);
    CHECK(m.getCurrentModuleId(0) == -1);
    CHECK(m.reallocate(100) == 0);
    CHECK(m.getMeshBufferCount() == 0);
    CModularSkinnedMesh::SModularBuffer b;
    CHECK(b.needsOutputBuffer);
    SCountedRef mat;
    b.material = &mat;
    m.buffers.push_back(b);
    m.needOutputBufferFlag = true;
    CHECK(m.needOutputBuffer());
    CHECK(m.getMeshBufferCount() == 1);
    SCountedRef* out = nullptr;
    m.getMaterial(out, 0);
    CHECK(out == &mat && mat.count == 1);
    std::printf("%s\n", failures ? "modular_skinned_mesh: FAILED" : "modular_skinned_mesh: ok");
    return failures != 0;
}
