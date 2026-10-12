// Tests for the wave 03 reconstructions. createMeshCopy and referenceDrop are
// unmapped externs, so this file supplies test doubles for them.
#include <cstdio>

#include "glitch-scene-collada-gui/recon/v2/wave_03.hpp"

namespace glitch::scene {

static int g_copyCalls = 0;
static unsigned int g_lastFlagsA = 0;
static unsigned int g_lastFlagsB = 0;
static int g_dropCalls = 0;

void createMeshCopy(IMesh** out, IMesh* const* mesh, video::IVideoDriver*, unsigned int flagsA,
                    unsigned int flagsB) {
    ++g_copyCalls;
    g_lastFlagsA = flagsA;
    g_lastFlagsB = flagsB;
    (void)mesh;
    static IMesh copied{0, 1};  // owned reference, count 1
    *out = &copied;
}

void referenceDrop(IMesh* object) {
    ++g_dropCalls;
    --object->referenceCount;
}

}  // namespace glitch::scene

using namespace glitch::scene;

static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

static void testHandleCopy() {
    IMesh mesh{0, 1};
    IMesh* src = &mesh;
    IMesh* result = nullptr;
    recon_v2_03::createMeshUniquePrimitives(&result, &src);
    CHECK(result == &mesh);
    CHECK(mesh.referenceCount == 2);

    IMesh* nullSrc = nullptr;
    IMesh* nullResult = &mesh;
    recon_v2_03::createMeshWelded(&nullResult, &nullSrc);
    CHECK(nullResult == nullptr);
    CHECK(mesh.referenceCount == 2);

    IMesh* weldedResult = nullptr;
    recon_v2_03::createMeshWelded(&weldedResult, &src);
    CHECK(weldedResult == &mesh);
    CHECK(mesh.referenceCount == 3);
}

static void testWith2TCoords() {
    IMesh mesh{0, 1};
    IMesh* src = &mesh;
    IMesh* result = nullptr;
    g_copyCalls = 0;
    g_dropCalls = 0;
    IMesh** ret = recon_v2_03::createMeshWith2TCoords(&result, &src, nullptr);
    CHECK(ret == &result);
    CHECK(g_copyCalls == 1);
    CHECK(g_lastFlagsA == 0xFFFFFFFFu);
    CHECK(g_lastFlagsB == 6u);
    CHECK(g_dropCalls == 1);
    CHECK(result != nullptr);
    // Copy owns one reference: count 1 after the extra ++ and the temp drop.
    CHECK(result != nullptr && result->referenceCount == 1);
}

int main() {
    testHandleCopy();
    testWith2TCoords();
    if (failures == 0) {
        std::printf("recon_v2_wave_03: all tests passed\n");
        return 0;
    }
    std::printf("recon_v2_wave_03: %d failure(s)\n", failures);
    return 1;
}
