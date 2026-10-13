// Checks for the glitch::collada skinned-mesh, timeline, skin-technique and streaming-manager members of collada_misc.hpp. The
// animators, the synchronized sets and the animation-set templates are canonical classes now (collada_animators_test.cpp).
#include "glitch-scene-collada-gui/collada_misc.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    ISkinnedMesh mesh;
    CHECK(!mesh.skinningEnabled);
    mesh.setIsSkinningEnabled(true);
    CHECK(mesh.skinningEnabled);

    CTimelineController timeline;
    CHECK(timeline.getScale() == 1.0f);
    timeline.scale = 0.5f;
    CHECK(timeline.getScale() == 0.5f);

    int nodeTag = 0;
    glitch::scene::ISceneNode* node = reinterpret_cast<glitch::scene::ISceneNode*>(&nodeTag);
    detail::CColladaHardwareMatrixSkinTechnique matrixSkin;
    matrixSkin.attach(node);
    CHECK(matrixSkin.attachedNode == node);
    detail::CColladaHardwareQuatSkinTechnique quatSkin;
    quatSkin.attach(node);
    CHECK(quatSkin.attachedNode == node);
    detail::CColladaHardwareTextureSkinTechnique textureSkin;
    textureSkin.attach(node);
    CHECK(textureSkin.attachedNode == node);

    CAnimationStreamingManager streaming;
    CHECK(CAnimationStreamingManager::Instance == &streaming);
    CHECK(streaming.cacheUsage == 0 && streaming.registryBegin == nullptr);
    CAnimationStreamingManager::Instance = nullptr;

    std::printf("%s\n", failures ? "collada_misc: FAILED" : "collada_misc: ok");
    return failures != 0;
}
