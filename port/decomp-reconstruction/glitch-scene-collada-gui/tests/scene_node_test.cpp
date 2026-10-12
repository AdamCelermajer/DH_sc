#include "glitch-scene-collada-gui/scene_node.hpp"

#include <cstdio>
#include <cstring>

using namespace glitch::scene;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    ISceneNode node;
    CHECK(node.getID() == 0);
    node.setID(42);
    CHECK(node.getID() == 42);

    CHECK(std::strcmp(node.getUID(), "") == 0);
    CHECK(std::strcmp(node.getScopeID(), "") == 0);
    CHECK(node.getRenderVertexCount(nullptr) == 0xffffffffu);
    CHECK(node.getAbsoluteTransformation() == node.absoluteTransform);
    CHECK(node.getMaterialCount() == 0);
    CHECK(node.getMaterial(0) == nullptr);
    CHECK(node.getAnimators().empty());
    CHECK(node.getBindedAnimators().empty());
    CHECK(node.getChildren().empty());
    CHECK(node.getTriangleSelector() == nullptr);
    CHECK(node.getType() == 0x6e6b6e75u);
    CHECK(node.clone() == nullptr);

    // The debug flag is bit 7 of the flags word and does not touch bit 0 (visible).
    node.flags = 0x1;
    CHECK(!node.isDebugObject());
    node.setIsDebugObject(true);
    CHECK(node.isDebugObject());
    CHECK(node.flags == 0x81);
    node.setIsDebugObject(false);
    CHECK(!node.isDebugObject());
    CHECK(node.flags == 0x1);

    int data = 0;
    node.setGameData(&data);
    CHECK(node.getGameData() == &data);

    node.setCameraOffset(2.5f);
    CHECK(node.getCameraOffset() == 2.5f);
    node.setRenderingLayer(7);
    CHECK(node.getRenderingLayer() == 7);

    std::printf("%s\n", failures ? "scene_node: FAILED" : "scene_node: ok");
    return failures != 0;
}
