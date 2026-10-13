// Checks the glitch::scene type tags, constant returns and one-member accessors
// in scene_simple_accessors.hpp: the tags are the four-character words, the
// speed setter applies the original 0.001 scale, and the animator factory clamps
// indices above 8 to 10.
#include "glitch-scene-collada-gui/scene_simple_accessors.hpp"

#include <cstdio>
#include <cstring>

using namespace glitch::scene;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool tagIs(std::uint32_t tag, const char* four) {
    return std::memcmp(&tag, four, 4) == 0;  // little-endian host order
}

int main() {
    CAnimatedMeshSceneNode amsh;
    CHECK(tagIs(amsh.getType(), "amsh"));
    amsh.startFrame = 3;
    amsh.endFrame = 90;
    amsh.frameNr = 12.5f;
    CHECK(amsh.getStartFrame() == 3);
    CHECK(amsh.getEndFrame() == 90);
    CHECK(amsh.getFrameNr() == 12.5f);
    amsh.setAnimationSpeed(2000.0f);
    CHECK(amsh.animationSpeed == 2000.0f * 0.001f);
    amsh.setLoopMode(true);
    amsh.setRenderFromIdentity(true);
    CHECK(amsh.loopMode && amsh.renderFromIdentity);

    CTextSceneNode text;
    CHECK(tagIs(text.getType(), "text"));
    CHECK(&text.getBoundingBox() == &text.boundingBox);

    CBillboardTextSceneNode bt;
    CHECK(bt.onRegisterSceneNode() == 1);
    CHECK(&bt.getSize() == &bt.size);
    CHECK(&bt.getBoundingBox() == &bt.boundingBox);

    CDummyTransformationSceneNode dummy;
    CHECK(tagIs(dummy.getType(), "dmmy"));

    CSkyBoxSceneNode sky;
    CHECK(tagIs(sky.getType(), "sky_"));
    CHECK(sky.getMaterialCount() == 6);

    CTerrainSceneNode terrain;
    CHECK(tagIs(terrain.getType(), "terr"));
    terrain.position.X = 1.0f;
    CHECK(terrain.getPosition().X == 1.0f);
    CHECK(&terrain.getScale() == &terrain.scale);

    CShadowVolumeSceneNode shadow;
    CHECK(tagIs(shadow.getType(), "shdw"));
    shadow.SetBiasFactor(0.25f);
    CHECK(shadow.biasFactor == 0.25f);

    CParticleSystemSceneNode ps;
    CHECK(tagIs(ps.getType(), "ptcl"));
    CHECK(ps.getEmitter() == nullptr);
    CHECK(ps.getMaterialCount() == 1);
    ps.setTransparency(0.5f);
    CHECK(ps.transparency == 0.5f);
    ps.setParticlesAreGlobal(true);
    CHECK(ps.particlesAreGlobal);

    CSceneNodeAnimatorCameraFPS fps;
    fps.setMoveSpeed(4.0f);
    fps.setRotateSpeed(1.5f);
    CHECK(fps.getMoveSpeed() == 4.0f);
    CHECK(fps.getRotateSpeed() == 1.5f);
    CHECK(fps.getType() == 7);
    CHECK(fps.isEventReceiverEnabled());

    CSceneNodeAnimatorCameraMaya maya;
    maya.setZoomSpeed(0.75f);
    CHECK(maya.getZoomSpeed() == 0.75f);
    CHECK(maya.getType() == 8);

    CSceneNodeAnimatorCollisionResponse cr;
    cr.setSlidingSpeed(2.0f);
    CHECK(cr.getSlidingSpeed() == 2.0f);
    CHECK(!cr.isFalling());
    CHECK(cr.getWorld() == nullptr);
    CHECK(cr.getType() == 6);

    CDefaultSceneNodeAnimatorFactory factory;
    CHECK(factory.getCreatableSceneNodeAnimatorTypeCount() == 9);
    CHECK(factory.getCreateableSceneNodeAnimatorType(4) == 4);
    CHECK(factory.getCreateableSceneNodeAnimatorType(9) == 10);

    CHECK(IParticleCylinderEmitter().getType() == 3);
    CHECK(IParticleSpinAffector().getType() == 6);

    SAnimatedMesh mesh(boost::intrusive_ptr<IMesh>(), 0);
    CHECK(mesh.getMeshType() == 0);
    mesh.meshType = 7;
    CHECK(mesh.getMeshType() == 7);

    // Text, camera and particle accessors.
    CTextSceneNode textNode;
    textNode.setTextColor(glitch::video::SColor{0x11u});
    CHECK(textNode.textColor.color == 0x11u);
    CBillboardTextSceneNode billboardText;
    billboardText.setTextColor(glitch::video::SColor{0x22u});
    CHECK(billboardText.textColor.color == 0x22u);

    CSceneNodeAnimatorCameraFPS fpsAnim;
    fpsAnim.keyDown[2] = true;
    fpsAnim.allKeysUp();
    CHECK(!fpsAnim.keyDown[2]);
    fpsAnim.setVerticalMovement(true);
    CHECK(!fpsAnim.noVerticalMovement);
    fpsAnim.setVerticalMovement(false);
    CHECK(fpsAnim.noVerticalMovement);

    CSceneNodeAnimatorCameraMaya mayaAnim;
    mayaAnim.mouseKeyDown[1] = true;
    CHECK(mayaAnim.isMouseKeyDown(1));
    CHECK(!mayaAnim.isMouseKeyDown(0));
    mayaAnim.allKeysUp();
    CHECK(!mayaAnim.isMouseKeyDown(1));

    CParticleSystemSceneNode particleNode;
    particleNode.particles.resize(5);
    CHECK(particleNode.getNumberOfParticles() == 5);
    glitch::core::dimension2d<float> psSize{2.0f, 3.0f};
    particleNode.setParticleSize(psSize);
    CHECK(particleNode.particleSize.Width == 2.0f && particleNode.particleSize.Height == 3.0f);
    particleNode.setBillboardType(2);
    CHECK(particleNode.billboardType == 2);

    // Terrain patches are a row-major grid of patchesPerSide squared entries.
    CTerrainSceneNode terrainNode;
    terrainNode.patchesPerSide = 2;
    terrainNode.patches.resize(4);
    terrainNode.setLODOfPatch(1, 0, 7);
    CHECK(terrainNode.patches[2].lod == 7);
    CHECK(&terrainNode.getBoundingBox(1, 0) == &terrainNode.patches[2].boundingBox);
    terrainNode.setCurrentLODOfPatches(3);
    CHECK(terrainNode.patches[0].lod == 3 && terrainNode.patches[3].lod == 3);
    terrainNode.rotationPivotFlag = 1;
    terrainNode.setRotationPivot(glitch::core::vector3d<float>{1.0f, 2.0f, 3.0f});
    CHECK(terrainNode.rotationPivotFlag == 0);
    CHECK(terrainNode.rotationPivot.Y == 2.0f);

    // Collision response vectors and particle affector flag.
    CSceneNodeAnimatorCollisionResponse collision;
    collision.setEllipsoidRadius(glitch::core::vector3d<float>{4.0f, 5.0f, 6.0f});
    CHECK(collision.getEllipsoidRadius().Z == 6.0f);
    collision.setGravity(glitch::core::vector3d<float>{0.0f, -9.0f, 0.0f});
    CHECK(collision.getGravity().Y == -9.0f);
    collision.setEllipsoidTranslation(glitch::core::vector3d<float>{7.0f, 8.0f, 9.0f});
    CHECK(collision.getEllipsoidTranslation().X == 7.0f);
    IParticleAffector affector;
    affector.setEnabled(true);
    CHECK(affector.getEnabled());
    affector.setEnabled(false);
    CHECK(!affector.getEnabled());

    // Animated mesh frames and bounding box; mesh cache lookups by pointer and index.
    SAnimatedMesh animated(boost::intrusive_ptr<IMesh>(), 0);
    animated.meshes.resize(3);
    CHECK(animated.getFrameCount() == 3);
    glitch::core::aabbox3d<float> animBox{{0.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 3.0f}};
    animated.setBoundingBox(animBox);
    CHECK(animated.getBoundingBox().MaxEdge.Z == 3.0f);

    CMeshCache cache;
    CHECK(cache.getMeshCount() == 0);
    CHECK(cache.getMeshFilename(0) == nullptr);
    SAnimatedMesh meshA(boost::intrusive_ptr<IMesh>(), 0);
    SAnimatedMesh meshB(boost::intrusive_ptr<IMesh>(), 0);
    cache.entries.resize(2);
    cache.entries[0].filename = "a.x";
    cache.entries[0].mesh = &meshA;
    cache.entries[1].filename = "b.x";
    cache.entries[1].mesh = &meshB;
    CHECK(cache.getMeshCount() == 2);
    CHECK(std::strcmp(cache.getMeshFilename(1), "b.x") == 0);
    CHECK(cache.getMeshFilename(2) == nullptr);
    boost::intrusive_ptr<const IAnimatedMesh> keyB(&meshB);
    CHECK(std::strcmp(cache.getMeshFilename(keyB), "b.x") == 0);
    boost::intrusive_ptr<const IAnimatedMesh> keyNone;
    CHECK(cache.getMeshFilename(keyNone) == nullptr);

    // Animator name table and the creatable scene-node factory records.
    CDefaultSceneNodeAnimatorFactory animNames;
    CHECK(std::strcmp(animNames.getCreateableSceneNodeAnimatorTypeName(4), "texture") == 0);
    CHECK(std::strcmp(animNames.getCreateableSceneNodeAnimatorTypeName(ESNAT_CAMERA_MAYA), "cameraMaya") == 0);
    CHECK(animNames.getCreateableSceneNodeAnimatorTypeName(9u) == nullptr);
    CDefaultSceneNodeFactory nodeFactory;
    CHECK(nodeFactory.getCreatableSceneNodeTypeCount() == 0);
    CHECK(nodeFactory.getCreateableSceneNodeType(0) == 0x6e6b6e75u);
    CDefaultSceneNodeFactory::SCreatableType kinds[2];
    kinds[0].type = 0x6d657368u;
    kinds[0].name = "mesh";
    kinds[1].type = 0x74787420u;
    kinds[1].name = "text";
    nodeFactory.types.assign(kinds, kinds + 2);
    CHECK(nodeFactory.getCreatableSceneNodeTypeCount() == 2);
    CHECK(nodeFactory.getCreateableSceneNodeType(1) == 0x74787420u);
    CHECK(std::strcmp(nodeFactory.getCreateableSceneNodeTypeName(0), "mesh") == 0);
    CHECK(nodeFactory.getCreateableSceneNodeTypeName(2) == nullptr);
    CHECK(nodeFactory.getCreateableSceneNodeType(2) == 0x6e6b6e75u);

    {
        // Mesh cache: index lookup, clear and the driver constructor.
        int meshTagA = 0, meshTagB = 0;
        const IAnimatedMesh* meshA = reinterpret_cast<const IAnimatedMesh*>(&meshTagA);
        const IAnimatedMesh* meshB = reinterpret_cast<const IAnimatedMesh*>(&meshTagB);
        CMeshCache indexCache;
        CMeshCache::SMeshEntry entryA;
        entryA.mesh = meshA;
        CMeshCache::SMeshEntry entryB;
        entryB.mesh = meshB;
        indexCache.entries.push_back(entryA);
        indexCache.entries.push_back(entryB);
        CHECK(indexCache.getMeshIndex(boost::intrusive_ptr<const IAnimatedMesh>(meshB)) == 1);
        CHECK(indexCache.getMeshIndex(boost::intrusive_ptr<const IAnimatedMesh>(meshA)) == 0);
        CHECK(indexCache.getMeshIndex(boost::intrusive_ptr<const IAnimatedMesh>()) == -1);
        indexCache.clear();
        CHECK(indexCache.getMeshCount() == 0);
        CMeshCache driverCache(nullptr);
        CHECK(driverCache.referenceCount == 1 && driverCache.getMeshCount() == 0);

        // Mesh: clear drops the buffers; the box setter stores the box.
        CMesh clearedMesh;
        clearedMesh.buffers.push_back(CMesh::SBuffer{});
        clearedMesh.clear();
        CHECK(clearedMesh.buffers.empty());
        glitch::core::aabbox3d<float> sceneBox{{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}};
        clearedMesh.setBoundingBox(sceneBox);
        CHECK(clearedMesh.getBoundingBox().MaxEdge.Z == 6.0f);

        // Shadow volume load returns zero.
        CShadowVolumeSceneNode shadow;
        CHECK(shadow.load(nullptr) == 0);

        // The binary loader's extension test (the append buffer is checked in scene_append_mesh_buffer_test.cpp).
        CColladaBinaryFileLoader binaryLoader;
        CHECK(binaryLoader.isALoadableFileExtension("scene.bdae"));
        CHECK(!binaryLoader.isALoadableFileExtension("scene.dae"));

        // Camera Maya difference and fly-straight difference.
        CSceneNodeAnimatorCameraMaya maya;
        maya.stateA = 10.0f;
        maya.stateB = 4.0f;
        CHECK(maya.updateAnimationState() == 6.0f);
        CSceneNodeAnimatorFlyStraight fly;
        fly.end.X = 2.0f;
        fly.start.X = 0.5f;
        CHECK(fly.recalculateImidiateValues() == 1.5f);
    }

    std::printf("%s\n", failures ? "scene_simple_accessors: FAILED" : "scene_simple_accessors: ok");
    return failures != 0;
}
