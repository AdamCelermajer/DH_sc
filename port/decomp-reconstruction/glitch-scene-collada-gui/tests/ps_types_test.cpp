// Checks for the glitch::ps shape tags, the particle-system world-matrix
// accessor and the empty model initialisers.
#include "glitch-scene-collada-gui/ps_types.hpp"

#include <cstdio>

using namespace glitch::ps;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

struct GNPSParticle {};
struct SParticle {};

int main() {
    CHECK(PDPoint().getType() == 3);
    CHECK(PDLine().getType() == 4);
    CHECK(PDBox().getType() == 0);
    CHECK(PDCylinder().getType() == 2);
    CHECK(PDCone().getType() == 5);
    CHECK(PDSphere().getType() == 1);
    CHECK(PDBlob().getType() == 6);

    CHECK(PDPoint().size() == 1.0f);
    CHECK(PDBlob().size() == 1.0f);

    float matrix[16] = {};
    IParticleSystem<GNPSParticle> gnps;
    CHECK(gnps.getWorldMatrixPtr() == nullptr);
    gnps.worldMatrixPtr = matrix;
    CHECK(gnps.getWorldMatrixPtr() == matrix);

    IParticleSystem<SParticle> sps;
    sps.worldMatrixPtr = matrix + 4;
    CHECK(sps.getWorldMatrixPtr() == matrix + 4);

    GNPSSizeModel<GNPSParticle>().initPSizeModel();
    PSizeModel<SParticle>().initPSizeModel();
    GNPSColorModel<GNPSParticle>().initPColorModel();
    PColorModel<SParticle>().initPColorModel();
    GNPSMotionModel<GNPSParticle>().initPMotionModel();
    PMotionModel<SParticle>().initPMotionModel();
    SParticle a, b;
    PForcesModel<GNPSParticle>().initPForces(nullptr, nullptr);
    PForcesModel<SParticle>().initPForces(&a, &b);
    GNPSSpinModel<GNPSParticle>().initPSpinModel();
    PSpinModel<SParticle>().initPSpinModel();
    GNPSLifeModel<GNPSParticle>().initPLifeModel();
    PLifeModel<SParticle>().initPLifeModel();

    // Colour animation track presence.
    GNPSColorModel<GNPSParticle> gnpsColor;
    CHECK(gnpsColor.hasColorAnimationTrack() == 0);
    int trackTag = 0;
    gnpsColor.colorAnimationTrack = &trackTag;
    CHECK(gnpsColor.hasColorAnimationTrack() == 1);
    PColorModel<SParticle> pColor;
    CHECK(pColor.hasColorAnimationTrack() == 0);
    pColor.colorAnimationTrack = &trackTag;
    CHECK(pColor.hasColorAnimationTrack() == 1);

    std::printf("%s\n", failures ? "ps_types: FAILED" : "ps_types: ok");
    return failures != 0;
}
