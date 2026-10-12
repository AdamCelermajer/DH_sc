// Checks for the glitch::collada synchronized-set, skinned-mesh, timeline and
// animator-length accessors.
#include "glitch-scene-collada-gui/collada_misc.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    static_assert(sizeof(SSyncSegment) == 0x20, "sync segments are 32 bytes");

    SSyncSegment segments[2] = {};
    segments[0].timestamp = 100;
    segments[0].length = 25;
    segments[1].timestamp = 125;
    segments[1].length = 40;

    CSceneNodeAnimatorSynchronizedSet set;
    CHECK(set.getNumSegments() == 0);
    CHECK(!set.hasReachedEnd());
    set.segments = segments;
    set.segmentCount = 2;
    set.reachedEnd = true;
    CHECK(set.getNumSegments() == 2);
    CHECK(set.hasReachedEnd());
    CHECK(set.getSynchronizationTimestamp(1) == 125);
    CHECK(set.getSynchronizationLength(0) == 25);
    CHECK(set.getSynchronizationLength(1) == 40);

    ISkinnedMesh mesh;
    CHECK(!mesh.skinningEnabled);
    mesh.setIsSkinningEnabled(true);
    CHECK(mesh.skinningEnabled);

    CTimelineController timeline;
    CHECK(timeline.getScale() == 1.0f);
    timeline.scale = 0.5f;
    CHECK(timeline.getScale() == 0.5f);

    ISceneNodeAnimator animator;
    CHECK(animator.getLength() == 0);
    animator.length = 900;
    CHECK(animator.getLength() == 900);

    std::printf("%s\n", failures ? "collada_misc: FAILED" : "collada_misc: ok");
    return failures != 0;
}
