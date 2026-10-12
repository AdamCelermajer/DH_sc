#include "glitch-scene-collada-gui/synchronized_blender.hpp"

#include <cstdio>

using namespace glitch::collada;
using SD = CSceneNodeAnimatorSynchronizedBlender::SynchronizationData;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    SD a, b;
    a.key = 1;
    b.key = 2;
    CHECK(CSceneNodeAnimatorSynchronizedBlender::orderingFunc(a, b));
    CHECK(!CSceneNodeAnimatorSynchronizedBlender::orderingFunc(b, a));
    CSceneNodeAnimatorSynchronizedBlender blender;
    CHECK(blender.getSynchronizationElapsedTime() == 0);
    std::printf("%s\n", failures ? "synchronized_blender: FAILED" : "synchronized_blender: ok");
    return failures != 0;
}
