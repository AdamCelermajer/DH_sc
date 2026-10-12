#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

#include <cstdio>
#include <cstring>

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
    // Category and module names through the table records.
    CModularSkinnedMesh named;
    CModularSkinnedMesh::SModuleHolder head{0, "head"};
    CModularSkinnedMesh::SModuleHolder torso{0, "torso"};
    CModularSkinnedMesh::SModuleHolder sword{0, "sword"};
    CModularSkinnedMesh::SModuleRecord bodyModules[2];
    bodyModules[0].holder = &head;
    bodyModules[1].holder = &torso;
    CModularSkinnedMesh::SModuleRecord weaponModules[1];
    weaponModules[0].holder = &sword;
    CModularSkinnedMesh::SCategoryEntry categoryEntries[2];
    categoryEntries[0].name = "body";
    categoryEntries[0].moduleCount = 2;
    categoryEntries[0].modules = bodyModules;
    categoryEntries[1].name = "weapon";
    categoryEntries[1].moduleCount = 1;
    categoryEntries[1].modules = weaponModules;
    CModularSkinnedMesh::SCategoryTable namedTable;
    namedTable.count = 2;
    namedTable.entries = categoryEntries;
    named.categories = &namedTable;
    named.currentModules.resize(2);
    named.currentModules[0].moduleId = 1;
    CHECK(std::strcmp(named.getCategoryName(1), "weapon") == 0);
    CHECK(named.getCategoryName(2) == nullptr);
    CHECK(std::strcmp(named.getModuleName(0, 0), "head") == 0);
    CHECK(named.getModuleName(1, 1) == nullptr);
    CHECK(std::strcmp(named.getCurrentModuleName(0), "torso") == 0);
    CHECK(named.getCurrentModuleName(1) == nullptr);
    CHECK(named.getCategoryId("weapon") == 1);
    CHECK(named.getCategoryId("missing") == -1);
    CHECK(named.getModuleId("torso") == 1);
    CHECK(named.getModuleId("sword") == 0);
    CHECK(named.getModuleId("missing") == -1);
    CHECK(!named.getBoundingBox().MinEdge.X);

    std::printf("%s\n", failures ? "modular_skinned_mesh: FAILED" : "modular_skinned_mesh: ok");
    return failures != 0;
}
