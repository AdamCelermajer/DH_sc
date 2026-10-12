// Checks the glitch::scene particle emitter settings: constructor arguments
// land in the members the accessors read, and the setters write them back.
#include "glitch-scene-collada-gui/particle_emitters.hpp"

#include <cstdio>

using namespace glitch;
using namespace glitch::scene;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool sameVec(const core::vector3d<float>& a, const core::vector3d<float>& b) {
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

int main() {
    const core::vector3d<float> center{1.f, 2.f, 3.f};
    const core::vector3d<float> normal{0.f, 1.f, 0.f};
    const core::vector3d<float> direction{0.f, 0.03f, 0.f};
    const video::SColor minColor{0x11223344u};
    const video::SColor maxColor{0x55667788u};

    {
        CParticleCylinderEmitter e(center, 5.f, normal, 9.f, true, direction, 20, 40, minColor,
                                   maxColor, 2000, 4000, 15);
        CHECK(sameVec(e.getCenter(), center));
        CHECK(sameVec(e.getNormal(), normal));
        CHECK(e.getRadius() == 5.f);
        CHECK(e.getLength() == 9.f);
        CHECK(e.getOutlineOnly());
        CHECK(sameVec(e.getDirection(), direction));
        CHECK(e.getMinParticlesPerSecond() == 20);
        CHECK(e.getMaxParticlesPerSecond() == 40);
        CHECK(e.getMinStartColor().color == minColor.color);
        CHECK(e.getMaxStartColor().color == maxColor.color);
        CHECK(e.getMinLifeTime() == 2000);
        CHECK(e.getMaxLifeTime() == 4000);
        CHECK(e.getMaxAngleDegrees() == 15);

        e.setCenter({4.f, 5.f, 6.f});
        CHECK(e.getCenter().X == 4.f && e.getCenter().Z == 6.f);
        e.setRadius(0.5f);
        CHECK(e.getRadius() == 0.5f);
        e.setOutlineOnly(false);
        CHECK(!e.getOutlineOnly());
        e.setMaxParticlesPerSecond(7);
        CHECK(e.getMaxParticlesPerSecond() == 7);
        e.setMinStartColor(maxColor);
        CHECK(e.getMinStartColor().color == maxColor.color);
        SParticle* out = nullptr;
        CHECK(e.emitt(0, 100, out) == 0);  // partial: no particles produced
    }

    {
        CParticlePointEmitter p(direction, 3, 9, minColor, maxColor, 100, 300, -4);
        CHECK(sameVec(p.getDirection(), direction));
        CHECK(p.getMinParticlesPerSecond() == 3);
        CHECK(p.getMaxParticlesPerSecond() == 9);
        CHECK(p.getMinStartColor().color == minColor.color);
        CHECK(p.getMaxStartColor().color == maxColor.color);
        CHECK(p.getMinLifeTime() == 100);
        CHECK(p.getMaxLifeTime() == 300);
        CHECK(p.getMaxAngleDegrees() == -4);
        p.setDirection(normal);
        CHECK(sameVec(p.getDirection(), normal));
        p.setMinParticlesPerSecond(1);
        CHECK(p.getMinParticlesPerSecond() == 1);
    }

    {
        core::aabbox3d<float> box;
        box.MinEdge = {-1.f, -1.f, -1.f};
        box.MaxEdge = {1.f, 2.f, 3.f};
        CParticleBoxEmitter b(box, direction, 5, 6, minColor, maxColor, 10, 20, 90);
        CHECK(b.getBox().MinEdge.X == -1.f);
        CHECK(b.getBox().MaxEdge.Z == 3.f);
        CHECK(sameVec(b.getDirection(), direction));
        CHECK(b.getMinParticlesPerSecond() == 5);
        CHECK(b.getMaxParticlesPerSecond() == 6);
        CHECK(b.getMinLifeTime() == 10);
        CHECK(b.getMaxLifeTime() == 20);
        CHECK(b.getMaxAngleDegrees() == 90);
        core::aabbox3d<float> other;
        other.MinEdge = {0.f, 0.f, 0.f};
        other.MaxEdge = {8.f, 8.f, 8.f};
        b.setBox(other);
        CHECK(b.getBox().MaxEdge.X == 8.f);
        b.setMaxStartColor(minColor);
        CHECK(b.getMaxStartColor().color == minColor.color);
    }

    {
        CParticleRingEmitter r(center, 2.f, 0.25f, direction, 1, 2, minColor, maxColor, 7, 8, 0);
        CHECK(sameVec(r.getCenter(), center));
        CHECK(r.getRadius() == 2.f);
        CHECK(r.getRingThickness() == 0.25f);
        CHECK(sameVec(r.getDirection(), direction));
        CHECK(r.getMinParticlesPerSecond() == 1);
        CHECK(r.getMaxParticlesPerSecond() == 2);
        CHECK(r.getMinLifeTime() == 7);
        CHECK(r.getMaxLifeTime() == 8);
        CHECK(r.getMaxAngleDegrees() == 0);
        r.setRingThickness(1.5f);
        CHECK(r.getRingThickness() == 1.5f);
        r.setCenter(normal);
        CHECK(sameVec(r.getCenter(), normal));
    }

    {
        CParticleSphereEmitter s(center, 3.f, direction, 4, 8, minColor, maxColor, 50, 60, 45);
        CHECK(sameVec(s.getCenter(), center));
        CHECK(s.getRadius() == 3.f);
        CHECK(sameVec(s.getDirection(), direction));
        CHECK(s.getMinParticlesPerSecond() == 4);
        CHECK(s.getMaxParticlesPerSecond() == 8);
        CHECK(s.getMinStartColor().color == minColor.color);
        CHECK(s.getMaxStartColor().color == maxColor.color);
        CHECK(s.getMinLifeTime() == 50);
        CHECK(s.getMaxLifeTime() == 60);
        CHECK(s.getMaxAngleDegrees() == 45);
        s.setRadius(6.f);
        CHECK(s.getRadius() == 6.f);
        s.setDirection(normal);
        CHECK(sameVec(s.getDirection(), normal));
    }

    {
        boost::intrusive_ptr<const IMesh> none;
        CParticleMeshEmitter m(none, true, direction, 0.25f, 2, true, 9, 10, minColor, maxColor, 300,
                               600, 30);
        CHECK(m.getMesh().get() == nullptr);
        CHECK(m.isUsingNormalDirection());
        CHECK(m.getUseNormalDirection());
        CHECK(sameVec(m.getDirection(), direction));
        CHECK(m.getNormalDirectionModifier() == 0.25f);
        CHECK(m.getEveryMeshVertex());
        CHECK(m.getMBNumber() == 2);
        CHECK(m.getMinParticlesPerSecond() == 9);
        CHECK(m.getMaxParticlesPerSecond() == 10);
        CHECK(m.getMinStartColor().color == minColor.color);
        CHECK(m.getMaxStartColor().color == maxColor.color);
        CHECK(m.getMinLifeTime() == 300);
        CHECK(m.getMaxLifeTime() == 600);
        CHECK(m.getMaxAngleDegrees() == 30);
        m.setUseNormalDirection(false);
        CHECK(!m.isUsingNormalDirection());
        m.setEveryMeshVertex(false);
        CHECK(!m.getEveryMeshVertex());
        m.setNormalDirectionModifier(2.f);
        CHECK(m.getNormalDirectionModifier() == 2.f);
        m.setMaxParticlesPerSecond(12);
        CHECK(m.getMaxParticlesPerSecond() == 12);
        SParticle* out = nullptr;
        CHECK(m.emitt(0, 10, out) == 0);
    }

    {
        CParticleAnimatedMeshSceneNodeEmitter a(nullptr, false, normal, 1.5f, 4, true, 2, 3, minColor,
                                                maxColor, 11, 12, -7);
        CHECK(a.getAnimatedMeshSceneNode() == nullptr);
        CHECK(!a.isUsingNormalDirection());
        CHECK(sameVec(a.getDirection(), normal));
        CHECK(a.getNormalDirectionModifier() == 1.5f);
        CHECK(a.getEveryMeshVertex());
        CHECK(a.getMBNumber() == 4);
        CHECK(a.getMinParticlesPerSecond() == 2);
        CHECK(a.getMaxParticlesPerSecond() == 3);
        CHECK(a.getMinStartColor().color == minColor.color);
        CHECK(a.getMaxStartColor().color == maxColor.color);
        CHECK(a.getMinLifeTime() == 11);
        CHECK(a.getMaxLifeTime() == 12);
        CHECK(a.getMaxAngleDegrees() == -7);
        a.setMinStartColor(maxColor);
        CHECK(a.getMinStartColor().color == maxColor.color);
        SParticle* out = nullptr;
        CHECK(a.emitt(0, 10, out) == 0);
    }

    std::printf("%s\n", failures ? "particle_emitters: FAILED" : "particle_emitters: ok");
    return failures != 0;
}
