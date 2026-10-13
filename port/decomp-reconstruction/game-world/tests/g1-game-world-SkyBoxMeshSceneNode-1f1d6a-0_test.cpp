// Test for batch g1-game-world-SkyBoxMeshSceneNode-1f1d6a-0.
//
// Covers what the batch binds on the host without an engine: getType's constant, the constructor's render gate and
// null node, onRegisterSceneNode's return value, the render gate, and a destructor with no node. Reference counting on a
// real node needs a complete glitch::scene::IMeshSceneNode and is not exercised. The partial bodies (the mesh, the scene
// manager and the absolute transform) are not checked.

#include <cstdio>

#include "dh2/SkyBoxMeshSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/IMesh.hpp"

int main() {
    int failures = 0;

    {
        // A null mesh and a null node are enough: the constructor sets the gate and keeps the node pointer.
        SkyBoxMeshSceneNode node(boost::intrusive_ptr<glitch::collada::IMesh>(nullptr), nullptr);

        if (node.getType() != 1601792883) {
            std::printf("FAIL: getType() is %d, expected 1601792883\n", node.getType());
            ++failures;
        }
        if (!node.m_renderEnabled) {
            std::printf("FAIL: m_renderEnabled is false after construction\n");
            ++failures;
        }
        if (node.unk_13c != nullptr) {
            std::printf("FAIL: node pointer is not null after construction with a null node\n");
            ++failures;
        }
        if (node.onRegisterSceneNode() != 1) {
            std::printf("FAIL: onRegisterSceneNode() did not return 1\n");
            ++failures;
        }

        // With the gate clear, render must return at once. The partial body draws nothing either way.
        node.m_renderEnabled = false;
        node.render(nullptr);
        if (node.m_renderEnabled) {
            std::printf("FAIL: render() changed m_renderEnabled\n");
            ++failures;
        }
    }

    if (failures == 0) {
        std::printf("SkyBoxMeshSceneNode test: all checks passed\n");
        return 0;
    }
    std::printf("SkyBoxMeshSceneNode test: %d failure(s)\n", failures);
    return 1;
}
