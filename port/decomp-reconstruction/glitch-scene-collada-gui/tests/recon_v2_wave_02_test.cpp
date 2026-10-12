// Test for the wave 02 (recon v2) reconstruction: CBatchMesh and CBatchSceneNode accessors,
// CBatchMesh constructors, the CGUIToolBar updateAbsolutePosition and IGUIElement tab order.
#include "glitch-scene-collada-gui/recon/v2/wave_02.hpp"


#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(__linux__)  // MAP_32BIT guest memory is Linux-only
#include <sys/mman.h>

// Test doubles for the externs the wave declares but does not own.
namespace {
int g_baseUpdateCalls = 0;
int g_nextFoundCalls = 0;
glitch::gui::IGUIElement* g_nextFound = nullptr;  // what getNextElement reports as found
glitch::gui::IGUIElement* g_lastStart = nullptr;  // start element seen by getNextElement
int g_lastTabGroup = -1;
}  // namespace

int glitch::collada::recon_v2_02::IGUIElement_updateAbsolutePosition(glitch::gui::IGUIElement*) {
    ++g_baseUpdateCalls;
    return 7;
}

int glitch::collada::recon_v2_02::IGUIElement_getNextElement(glitch::gui::IGUIElement* self,
                                                             int tabGroup,
                                                             glitch::gui::IGUIElement*& found,
                                                             glitch::gui::IGUIElement*& other,
                                                             bool recurse) {
    (void)recurse;
    ++g_nextFoundCalls;
    g_lastStart = self;
    g_lastTabGroup = tabGroup;
    found = g_nextFound;
    other = nullptr;
    return found != nullptr ? 1 : 0;
}

namespace {

using namespace glitch::collada::recon_v2_02;
namespace scene = glitch::scene;
namespace gui = glitch::gui;

int failures = 0;

void check(bool cond, const char* what) {
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", what);
    }
}

// Scene targets are read through 32-bit words, so they must sit below 4 GiB.
void* allocTarget(std::size_t size) {
    void* p = mmap(nullptr, size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    if (p == MAP_FAILED) {
        std::printf("FAIL: MAP_32BIT allocation\n");
        std::exit(1);
    }
    std::memset(p, 0, size);
    return p;
}

std::uint32_t wordOf(const void* p) { return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(p)); }

void testGetMeshBuffer() {
    auto* batches = static_cast<scene::CBatchMesh::SBatch*>(allocTarget(2 * sizeof(scene::CBatchMesh::SBatch)));
    auto* buffer = static_cast<scene::CMeshBuffer*>(allocTarget(sizeof(scene::CMeshBuffer)));
    buffer->referenceCount = 3;
    batches[0].meshBuffer = wordOf(buffer);
    batches[1].meshBuffer = 0;

    scene::CBatchMesh mesh{};
    mesh.batchesBegin = wordOf(batches);
    mesh.batchesEnd = wordOf(batches + 2);

    scene::CMeshBuffer* out = reinterpret_cast<scene::CMeshBuffer*>(1);
    getMeshBuffer(&out, &mesh, 0);
    check(out == buffer, "getMeshBuffer returns the batch buffer");
    check(buffer->referenceCount == 4, "getMeshBuffer increments the buffer refcount");

    getMeshBuffer(&out, &mesh, 1);
    check(out == nullptr, "getMeshBuffer returns null for an empty slot");
}

void testGetMesh() {
    auto* mesh = static_cast<scene::IMesh*>(allocTarget(sizeof(scene::IMesh)));
    mesh->referenceCount = 1;
    CBatchSceneNodeView node{};
    node.mesh = wordOf(mesh);

    scene::IMesh* out = nullptr;
    getMesh(&out, &node);
    check(out == mesh, "getMesh returns the node mesh");
    check(mesh->referenceCount == 2, "getMesh increments the mesh refcount");

    CBatchSceneNodeView empty{};
    getMesh(&out, &empty);
    check(out == nullptr, "getMesh returns null without a mesh");
}

void testConstructor(std::uint32_t word70, const char* label) {
    scene::CBatchMesh mesh{};
    auto* raw = reinterpret_cast<unsigned char*>(&mesh);
    for (std::size_t i = 0; i < sizeof mesh; ++i) raw[i] = 0xAB;  // poison before the ctor
    constructBatchMesh(&mesh, word70);
    check(mesh.referenceCount == 0, label);
    check(mesh.batchesBegin == 0 && mesh.batchesEnd == 0 && mesh.batchesCapacity == 0, label);
    check(mesh.localBox.MinEdge.X == -1.0f && mesh.localBox.MaxEdge.Z == 1.0f, label);
    check(mesh.boundingBox.MinEdge.Z == -1.0f && mesh.boundingBox.MaxEdge.X == 1.0f, label);
    check(mesh.boundingBoxSource == 0, label);
    check(mesh.boundingBoxStamp == 0xFFFFFFFFu, label);
    check(mesh.reserved0x70 == word70, label);
    check(mesh.boundingBoxDirty == 1, label);
    check(mesh.reserved0x78 == 0, label);
}

void testUpdateAbsolutePosition() {
    gui::IGUIElement parent;
    parent.absoluteRect.UpperLeftCorner.X = 10;
    parent.absoluteRect.LowerRightCorner.X = 110;

    gui::IGUIElement toolbar;
    toolbar.parent = &parent;
    toolbar.desiredRect.UpperLeftCorner.X = 55;
    toolbar.desiredRect.LowerRightCorner.X = 66;
    g_baseUpdateCalls = 0;
    int r = CGUIToolBar_updateAbsolutePosition(&toolbar);
    check(toolbar.desiredRect.UpperLeftCorner.X == 0, "updateAbsolutePosition zeroes desired left");
    check(toolbar.desiredRect.LowerRightCorner.X == 100, "updateAbsolutePosition sets desired width");
    check(r == 7 && g_baseUpdateCalls == 1, "updateAbsolutePosition tail-calls the base once");

    gui::IGUIElement orphan;
    orphan.desiredRect.UpperLeftCorner.X = 5;
    CGUIToolBar_updateAbsolutePosition(&orphan);
    check(orphan.desiredRect.UpperLeftCorner.X == 5, "updateAbsolutePosition leaves a parentless element");
}

void testSetTabOrderGroup() {
    gui::IGUIElement root;
    gui::IGUIElement child;
    child.parent = &root;
    child.tabGroup = 1;
    gui::IGUIElement next;
    next.tabOrder = 4;

    g_nextFound = &next;
    g_nextFoundCalls = 0;
    IGUIElement_setTabOrder(&child, 0);
    check(g_nextFoundCalls == 1, "setTabOrder calls getNextElement once in a tab group");
    check(g_lastStart == &root, "setTabOrder starts at the topmost ancestor");
    check(g_lastTabGroup == 1, "setTabOrder passes the group flag");
    check(child.tabOrder == 5, "setTabOrder sets tabOrder to found + 1");

    g_nextFound = nullptr;
    child.tabOrder = 9;
    IGUIElement_setTabOrder(&child, 0);
    check(child.tabOrder == 0, "setTabOrder clears tabOrder when nothing is found");
    g_nextFound = nullptr;
}

void testSetTabOrderNoGroup() {
    gui::IGUIElement top;
    top.tabGroup = 1;
    gui::IGUIElement mid;
    mid.parent = &top;
    gui::IGUIElement leaf;
    leaf.parent = &mid;
    leaf.tabOrder = 3;

    g_nextFoundCalls = 0;
    g_nextFound = nullptr;
    IGUIElement_setTabOrder(&leaf, 0);
    check(g_nextFoundCalls == 1, "setTabOrder walks to the nearest tab-group ancestor");
    check(g_lastStart == &top, "setTabOrder starts at the tab-group ancestor");
    check(g_lastTabGroup == 0, "setTabOrder passes group flag 0 outside a group");
    check(leaf.tabOrder == 0, "setTabOrder zeroes tabOrder before the search");

    gui::IGUIElement lone;
    g_nextFoundCalls = 0;
    IGUIElement_setTabOrder(&lone, 0);
    check(g_nextFoundCalls == 0, "setTabOrder does nothing without a tab-group ancestor");
}

}  // namespace

int main() {
    testGetMeshBuffer();
    testGetMesh();
    testConstructor(44, "constructBatchMesh() default word70");
    testConstructor(0x1234, "constructBatchMesh(n) word70");
    testUpdateAbsolutePosition();
    testSetTabOrderGroup();
    testSetTabOrderNoGroup();
    if (failures == 0) {
        std::printf("wave_02 tests: all passed\n");
        return 0;
    }
    std::printf("wave_02 tests: %d failure(s)\n", failures);
    return 1;
}

#else
int main() {
    std::printf("skipped: MAP_32BIT guest memory is Linux-only\n");
    return 0;
}
#endif
