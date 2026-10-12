// Slice 05 test: checks the one reconstructed body (billboardSetSize) and that
// the stub entry points link and run without touching state.
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_05.hpp"

namespace {

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++failures;
    }
}

}  // namespace

int main() {
    using glitch::core::dimension2d;
    glitch::scene::CBillboardSceneNode node;

    // Zero size: the stored size becomes 1x1 and the half extent is 0.
    glitch::scene::recon_05::billboardSetSize(node, dimension2d<float>{0.0f, 0.0f});
    check(node.size.Width == 1.0f, "zero width becomes 1");
    check(node.size.Height == 1.0f, "zero height becomes 1");
    check(node.boundingBox.MinEdge.X == 0.0f && node.boundingBox.MaxEdge.Z == 0.0f, "zero extent");

    // (6, 12): half extent is (6 + 12) / 6 = 3, min is -3 and max is 3 on each axis.
    glitch::scene::recon_05::billboardSetSize(node, dimension2d<float>{6.0f, 12.0f});
    check(node.size.Width == 6.0f && node.size.Height == 12.0f, "size stored");
    check(node.boundingBox.MinEdge.X == -3.0f && node.boundingBox.MinEdge.Y == -3.0f &&
              node.boundingBox.MinEdge.Z == -3.0f,
          "min edge is -3");
    check(node.boundingBox.MaxEdge.X == 3.0f && node.boundingBox.MaxEdge.Y == 3.0f &&
              node.boundingBox.MaxEdge.Z == 3.0f,
          "max edge is 3");

    // Stubs must link and do nothing observable.
    glitch::scene::CSceneManager manager;
    glitch::scene::recon_05::sceneManagerClear(manager);

    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::puts("recon_slice_05_test: ok");
    return 0;
}
