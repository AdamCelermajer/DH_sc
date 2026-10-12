// Checks for the glitch::scene animator, camera, batch, mesh and timeline
// accessors.
#include "glitch-scene-collada-gui/scene_nodes.hpp"

#include <cstdio>

using namespace glitch::scene;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    ISceneNodeAnimator animator;
    CHECK(animator.getType() == 10);
    CHECK(!animator.isEventReceiverEnabled());
    CHECK(animator.getLength() == 0);
    animator.updateTime(5);
    animator.computeAnimationValues(5);
    animator.applyAnimationValues(5);

    CEmptySceneNode empty;
    CHECK(empty.getType() == 0x79746d65u);
    empty.render(nullptr);

    ICameraSceneNode base;
    CHECK(base.getTargetNode() == nullptr);

    CCameraSceneNode camera;
    CHECK(camera.getType() == 0x5f6d6163u);
    CHECK(!camera.isInputReceiverEnabled());
    camera.setInputReceiverEnabled(true);
    CHECK(camera.isInputReceiverEnabled());
    camera.boundingBox.MinEdge.X = 1.0f;
    CHECK(camera.getBoundingBox().MinEdge.X == 1.0f);

    CLightSceneNode light;
    CMeshSceneNode mesh;
    CSceneManagerRootNode root;
    CHECK(light.getType() == 0x7468676cu);
    CHECK(mesh.getType() == 0x6873656du);
    CHECK(root.getType() == 0x72676d73u);

    CSceneManager manager;
    CHECK(!manager.hierarchyChanged && !manager.visibilityChanged);
    manager.notifyVisibilityChanged();
    CHECK(manager.visibilityChanged && !manager.hierarchyChanged);
    manager.visibilityChanged = false;
    manager.notifyHierarchyChanged();
    CHECK(manager.hierarchyChanged && manager.visibilityChanged);
    CHECK(manager.createMeshWriter(0) == nullptr);

    CBillboardSceneNode billboard;
    CHECK(billboard.getType() == 0x6c6c6962u);
    CHECK(billboard.getMaterialCount() == 1);
    billboard.size.Width = 3.0f;
    billboard.size.Height = 4.0f;
    CHECK(billboard.getSize().Width == 3.0f && billboard.getSize().Height == 4.0f);

    CBatchMesh::SSegment segment;
    segment.indexBegin = 12;
    segment.indexEnd = 34;
    segment.visible = 1;
    CBatchMesh batch;
    std::uint32_t first = 0, second = 0;
    batch.getIndexRange(first, second, &segment);
    CHECK(first == 12 && second == 34);
    batch.init();
    batch.saveSegmentExtraData(nullptr, nullptr, false);

    CBatchSceneNode batchNode;
    CHECK(batchNode.getType() == 0x6e736162u);
    CHECK(batchNode.isSegmentVisible(segment));
    segment.visible = 0;
    CHECK(!batchNode.isSegmentVisible(segment));
    batchNode.preRegisterSceneNode();

    IMesh meshBase;
    CHECK(meshBase.getUserProperty() == nullptr);

    ITimelineController timeline;
    timeline.jumpTo(42);
    CHECK(timeline.currentFrame == 42);
    CHECK(!timeline.getLoop());
    CHECK(timeline.getScale() == 1.0f);
    timeline.setScale(2.0f);
    CHECK(timeline.getScale() == 1.0f);
    timeline.update(7);

    std::printf("%s\n", failures ? "scene_nodes: FAILED" : "scene_nodes: ok");
    return failures != 0;
}
