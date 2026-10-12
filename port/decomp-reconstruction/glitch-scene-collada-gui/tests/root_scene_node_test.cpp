#include "glitch-scene-collada-gui/root_scene_node.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CRootSceneNode root;
    CHECK(root.getUID() == 0);
    CHECK(root.getMaterialCount() == 0);
    CHECK(root.getType() == 0x72656164u);
    int a = 1, b = 2;
    root.addCamera(&a);
    root.addCamera(&b);
    CHECK(root.cameras.count() == 2);
    CHECK(root.cameras.head.next->value == &a);
    CHECK(root.cameras.head.prev->value == &b);
    root.addLight(&a);
    CHECK(root.lights.count() == 1);
    root.addMorphingMesh(&b);
    CHECK(root.morphingMeshes.count() == 1);
    root.addParticleSystem(&a);
    CHECK(root.particleSystems.count() == 1);
    root.removeMaterial(&a);
    CHECK(root.getMaterialCount() == 0);
    std::printf("%s\n", failures ? "root_scene_node: FAILED" : "root_scene_node: ok");
    return failures != 0;
}
