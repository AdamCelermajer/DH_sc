// Test for glitch::scene::CSceneCollisionManager::getCollisionPoint, Track B batch
// e1-glitch-scene-collada-gui-glitch_scene_CSceneCollisionManager-0. Uses a fake selector, so no engine code is needed.
// Exit status is non-zero when a check fails.
#include <cmath>
#include <cstdio>
#include <vector>

#include "glitch-scene-collada-gui/tb/glitch_scene_CSceneCollisionManager.hpp"

namespace {

using glitch::core::triangle3d;
using glitch::core::vector3d;

// Selector that returns every triangle it holds, ignoring the box and the transform.
class FakeSelector : public glitch::scene::ITriangleSelector {
public:
    explicit FakeSelector(std::vector<triangle3d<float>> tris) : triangles(std::move(tris)) {}

    int getTriangleCount() const override { return static_cast<int>(triangles.size()); }

    void getTriangles(triangle3d<float>* out, int arraySize, int& outCount,
                      const glitch::core::CMatrix4<float>*) const override {
        copy(out, arraySize, outCount);
    }
    void getTriangles(triangle3d<float>* out, int arraySize, int& outCount,
                      const glitch::core::aabbox3d<float>&, const glitch::core::CMatrix4<float>*) const override {
        copy(out, arraySize, outCount);
    }
    void getTriangles(triangle3d<float>* out, int arraySize, int& outCount,
                      const glitch::core::line3d<float>&, const glitch::core::CMatrix4<float>*) const override {
        copy(out, arraySize, outCount);
    }

private:
    void copy(triangle3d<float>* out, int arraySize, int& outCount) const {
        int n = 0;
        for (const auto& t : triangles) {
            if (n >= arraySize) break;
            out[n++] = t;
        }
        outCount = n;
    }

    std::vector<triangle3d<float>> triangles;
};

// Right triangle in the plane z, centred on the origin in x and y.
triangle3d<float> makeTriangle(float z) {
    triangle3d<float> t;
    t.A = vector3d<float>{-1.0f, -1.0f, z};
    t.B = vector3d<float>{1.0f, -1.0f, z};
    t.C = vector3d<float>{0.0f, 1.0f, z};
    return t;
}

glitch::core::line3d<float> makeRay(float fromZ, float toZ, float x = 0.0f) {
    glitch::core::line3d<float> r;
    r.start = vector3d<float>{x, 0.0f, fromZ};
    r.end = vector3d<float>{x, 0.0f, toZ};
    return r;
}

bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int failures = 0;

void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    } else {
        std::printf("ok:   %s\n", what);
    }
}

}  // namespace

int main() {
    using glitch::scene::CSceneCollisionManager;

    // 1. A single triangle on the ray: hit at the origin.
    {
        CSceneCollisionManager mgr;
        FakeSelector sel({makeTriangle(0.0f)});
        vector3d<float> point{};
        triangle3d<float> tri{};
        const bool hit = mgr.getCollisionPoint(makeRay(-10.0f, 10.0f), &sel, point, tri);
        check(hit, "single triangle is hit");
        check(near(point.X, 0.0f) && near(point.Y, 0.0f) && near(point.Z, 0.0f), "hit point is the origin");
        check(near(tri.A.Z, 0.0f), "out triangle is the hit triangle");
        check(mgr.triangles.size() == 1, "scratch buffer grew to the triangle count");
    }

    // 2. Two triangles on the ray: the one nearer to the start is returned.
    {
        CSceneCollisionManager mgr;
        FakeSelector sel({makeTriangle(5.0f), makeTriangle(2.0f)});
        vector3d<float> point{};
        triangle3d<float> tri{};
        const bool hit = mgr.getCollisionPoint(makeRay(-10.0f, 10.0f), &sel, point, tri);
        check(hit, "two triangles on the ray: a hit is reported");
        check(near(point.Z, 2.0f), "nearest triangle along the ray is chosen");
        check(near(tri.A.Z, 2.0f), "out triangle is the nearest one");
    }

    // 3. The ray misses: the triangle is off to the side.
    {
        CSceneCollisionManager mgr;
        triangle3d<float> off = makeTriangle(0.0f);
        off.A.X += 10.0f;
        off.B.X += 10.0f;
        off.C.X += 10.0f;
        FakeSelector sel({off});
        vector3d<float> point{vector3d<float>{7.0f, 7.0f, 7.0f}};
        triangle3d<float> tri{};
        const bool hit = mgr.getCollisionPoint(makeRay(-10.0f, 10.0f), &sel, point, tri);
        check(!hit, "missed triangle reports no hit");
        check(near(point.X, 7.0f), "outputs untouched on a miss");
    }

    // 4. The segment stops before the triangle: no hit.
    {
        CSceneCollisionManager mgr;
        FakeSelector sel({makeTriangle(5.0f)});
        vector3d<float> point{};
        triangle3d<float> tri{};
        const bool hit = mgr.getCollisionPoint(makeRay(-10.0f, 1.0f), &sel, point, tri);
        check(!hit, "triangle beyond the segment end is not hit");
    }

    // 5. Null selector and empty selector.
    {
        CSceneCollisionManager mgr;
        vector3d<float> point{};
        triangle3d<float> tri{};
        check(!mgr.getCollisionPoint(makeRay(-10.0f, 10.0f), nullptr, point, tri), "null selector returns false");
        FakeSelector empty({});
        check(!mgr.getCollisionPoint(makeRay(-10.0f, 10.0f), &empty, point, tri), "empty selector returns false");
    }

    std::printf("%s (%d failure%s)\n", failures ? "FAILED" : "PASSED", failures, failures == 1 ? "" : "s");
    return failures ? 1 : 0;
}
