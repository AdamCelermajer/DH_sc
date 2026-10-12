#include "glitch-scene-collada-gui/scene_node_animator.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CSceneNodeAnimator a;
    CHECK(a.getTargetCount() == 0);
    SAnimatorTarget t;
    t.track = 7;
    t.trackEx = 9;
    a.targets.push_back(t);
    CHECK(a.getTargetCount() == 1);
    CHECK(a.getAnimationTrack(0) == 7);
    CHECK(a.getAnimationTrackEx(0) == 9);
    CHECK(a.createClone() == nullptr);
    std::printf("%s\n", failures ? "scene_node_animator: FAILED" : "scene_node_animator: ok");
    return failures != 0;
}
