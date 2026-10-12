// Checks the glitch::scene particle affector settings: constructor arguments
// land in the members the accessors read, setters write them back, and the
// spin time conversion matches the original's float-to-unsigned truncation.
#include "glitch-scene-collada-gui/particle_affectors.hpp"

#include <cstdio>

using namespace glitch;
using namespace glitch::scene;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

static bool sameVec(const core::vector3d<float>& a, const core::vector3d<float>& b) {
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

int main() {
    const core::vector3d<float> point{1.f, 2.f, 3.f};
    const core::vector3d<float> other{-1.f, 0.f, 4.f};
    const video::SColor color{0xAABBCCDDu};

    {
        CParticleAttractionAffector a(point, 2.5f, true, false, true, false);
        CHECK(sameVec(a.getPoint(), point));
        CHECK(a.getSpeed() == 2.5f);
        CHECK(a.getAttract());
        CHECK(!a.getAffectX());
        CHECK(a.getAffectY());
        CHECK(!a.getAffectZ());
        a.setPoint(other);
        CHECK(sameVec(a.getPoint(), other));
        a.setAffectZ(true);
        CHECK(a.getAffectZ());
        a.setAttract(false);
        CHECK(!a.getAttract());
        SParticle* none = nullptr;
        a.affect(100, none, 0);
        a.affect(350, none, 0);  // partial: only the time step is computed
    }

    {
        CParticleGravityAffector g(point, 750);
        CHECK(sameVec(g.getGravity(), point));
        CHECK(g.getTimeForceLost() == 750.f);
        g.setTimeForceLost(12.5f);
        CHECK(g.getTimeForceLost() == 12.5f);
        g.setGravity(other);
        CHECK(sameVec(g.getGravity(), other));
    }

    {
        CParticleRotationAffector r(point, other);
        CHECK(sameVec(r.getPivotPoint(), point));
        CHECK(sameVec(r.getSpeed(), other));
        r.setPivotPoint(other);
        CHECK(sameVec(r.getPivotPoint(), other));
        r.setSpeed(point);
        CHECK(sameVec(r.getSpeed(), point));
        SParticle* none = nullptr;
        r.affect(10, none, 0);
        r.affect(20, none, 0);
    }

    {
        CParticleSizeAffector s(4.f, 0.5f, 300, 900);
        CHECK(s.getTargetSize() == 4.f);
        CHECK(s.getVariation() == 0.5f);
        CHECK(s.getGrowForTime() == 300.f);
        CHECK(s.getFadeForTime() == 900.f);
        s.setTargetSize(8.f);
        CHECK(s.getTargetSize() == 8.f);
        s.setVariation(1.f);
        CHECK(s.getVariation() == 1.f);
        s.setGrowForTime(2.f);
        CHECK(s.getGrowForTime() == 2.f);
        s.setFadeForTime(3.f);
        CHECK(s.getFadeForTime() == 3.f);
    }

    {
        CParticleSpinAffector sp(40, 0.25f);
        CHECK(sp.getSpinTime() == 40.f);
        CHECK(sp.getVariation() == 0.25f);
        sp.setSpinTime(12.9f);
        CHECK(sp.getSpinTime() == 12.f);  // truncates toward zero
        sp.setSpinTime(-3.f);
        CHECK(sp.getSpinTime() == 0.f);  // negative input saturates to zero
        sp.setVariation(0.75f);
        CHECK(sp.getVariation() == 0.75f);
    }

    {
        CParticleFadeOutAffector f(color, 500);
        CHECK(f.getTargetColor().color == color.color);
        CHECK(f.getFadeOutTime() == 500.f);
        f.setFadeOutTime(6.f);
        CHECK(f.getFadeOutTime() == 6.f);
        f.setTargetColor(video::SColor{0x01020304u});
        CHECK(f.getTargetColor().color == 0x01020304u);
        SParticle* none = nullptr;
        f.affect(0, none, 0);
    }

    std::printf("%s\n", failures ? "particle_affectors: FAILED" : "particle_affectors: ok");
    return failures != 0;
}
