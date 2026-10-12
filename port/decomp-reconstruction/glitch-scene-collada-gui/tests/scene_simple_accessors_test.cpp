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

    SAnimatedMesh mesh;
    CHECK(mesh.getMeshType() == 0);
    mesh.meshType = 7;
    CHECK(mesh.getMeshType() == 7);
    CHECK(IAnimatedMesh().getMeshType() == 0);

    std::printf("%s\n", failures ? "scene_simple_accessors: FAILED" : "scene_simple_accessors: ok");
    return failures != 0;
}
