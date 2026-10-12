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

    CCameraSceneNode camera(0, glitch::core::vector3d<float>(), glitch::core::vector3d<float>(), false);
    CHECK(camera.getType() == 0x5f6d6163u);
    CHECK(camera.isInputReceiverEnabled());  // the constructor sets inputReceiverEnabled (+0x165 = 1)
    camera.setInputReceiverEnabled(false);
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

    // Scene sceneMgr colour and rendered-node words; batch mesh records.
    CSceneManager sceneMgr;
    sceneMgr.setShadowColor(glitch::video::SColor{0x44u});
    CHECK(sceneMgr.shadowColors[3].color == 0x44u);
    CHECK(sceneMgr.setSceneNodeRenderPass(5) == 0);
    CHECK(sceneMgr.setSceneNodeRenderPass(6) == 5);
    glitch::video::SColorf ambient{0.1f, 0.2f, 0.3f, 1.0f};
    sceneMgr.setAmbientLight(ambient);
    CHECK(sceneMgr.ambientLight.b == 0.3f);
    int nodeMarker = 0;
    sceneMgr.currentRenderedNode = &nodeMarker;
    sceneMgr.currentRenderedSecond = 9;
    sceneMgr.currentRenderedWord = 4;
    void* renderedNode = nullptr;
    std::int32_t renderedSecond = 0;
    CHECK(sceneMgr.getCurrentRenderedNode(&renderedNode, &renderedSecond) == 4);
    CHECK(renderedNode == &nodeMarker && renderedSecond == 9);
    CHECK(sceneMgr.getCurrentRenderedNode(nullptr, nullptr) == 4);

    CBatchMesh batchMesh;
    CHECK(batchMesh.getMeshBufferCount() == 0);
    batchMesh.batches.resize(2);
    CHECK(batchMesh.getMeshBufferCount() == 2);
    CHECK(!batchMesh.clone());
    glitch::core::aabbox3d<float> batchBox{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}};
    batchMesh.setBoundingBox(batchBox);
    CHECK(batchMesh.getBoundingBox().MinEdge.X == -1.0f);
    CBatchMesh::SBatch zeroed;
    CHECK(zeroed.field0C == 0 && zeroed.field0E == 0 && zeroed.field10 == 0);
    CBatchMesh::SBatch filled(3);
    CHECK(filled.field0C == 3 && filled.field0E == 3 && filled.field10 == 0);

    std::printf("%s\n", failures ? "scene_nodes: FAILED" : "scene_nodes: ok");
    return failures != 0;
}
