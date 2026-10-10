// B066: static level geometry (GPU buffers, cached range scans, frustum culling) must be pixel-identical to the plain
// client-array path for every camera pose, and must actually take the fast paths (vbo draws, culled ranges).
#include "../frame_perf.hpp"
#include "../platform_win32.hpp"
#include "../render_queue.hpp"
#include "../renderer.hpp"
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;

static void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

static Mesh makeGrid(int cells, float spacing) {
    Mesh mesh;
    for (int gz = 0; gz < cells; ++gz) for (int gx = 0; gx < cells; ++gx) {
        const float x0 = (gx - cells / 2) * spacing, z0 = (gz - cells / 2) * spacing, size = spacing * 0.8f;
        const std::size_t base = mesh.vertices.size();
        const float shade = 0.4f + 0.6f * float((gx * 7 + gz * 3) % 10) / 10.0f;
        const bool alpha = (gx + gz) % 5 == 0;
        for (int corner = 0; corner < 4; ++corner) {
            Vertex v;
            v.position = {x0 + ((corner & 1) ? size : 0.f), (gx % 3) * 0.2f, z0 + ((corner & 2) ? size : 0.f)};
            v.normal = {0, 1, 0};
            v.u = float(corner & 1); v.v = float((corner >> 1) & 1);
            v.color = {shade, 1.0f - shade * 0.5f, 0.5f + 0.1f * corner, alpha ? 0.5f : 1.0f};
            mesh.vertices.push_back(v);
        }
        DrawRange range;
        range.firstIndex = mesh.indices.size();
        for (std::uint32_t index : {0u, 2u, 1u, 1u, 2u, 3u}) mesh.indices.push_back(std::uint32_t(base) + index);
        range.indexCount = 6;
        if ((gx + gz) % 4 == 1) range.material.color = {0.5f, 0.9f, 1.0f, 1.0f};   // tinted: client colour path
        mesh.ranges.push_back(range);
    }
    return mesh;
}

static std::vector<unsigned char> render(Window& window, Renderer& renderer, const Mesh& mesh, const Camera& camera) {
    renderer.beginFrame(camera);
    RenderQueue queue;
    queue.submit(mesh);
    queue.flush(renderer, camera);
    renderer.endFrame();
    std::vector<unsigned char> pixels(std::size_t(window.width()) * window.height() * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, window.width(), window.height(), GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    window.swap();
    return pixels;
}

int main() { try {
    Window window; check(window.open("B066 static geometry", 320, 240), window.error());
    Renderer renderer; check(renderer.initialize(window.width(), window.height()), "Renderer initialize failed");
    renderer.setGlLoader(&Window::gl_proc);
    auto& counters = perf::FramePerf::get().counters();

    Mesh dynamicMesh = makeGrid(14, 10.0f);
    Mesh staticMesh = dynamicMesh; staticMesh.staticGeometry = true;

    struct Pose { Vec3 eye, target; };
    const std::vector<Pose> poses = {
        {{0, 60, 70}, {0, 0, 0}}, {{-50, 25, 10}, {-40, 0, -10}}, {{30, 10, 30}, {60, 0, 60}},
        {{0, 40, 0}, {0, 0, 1}}, {{0, 5, 0}, {0, 5, -100}}, {{0, 5, 0}, {0, 5, 100}},   // along the grid and away
        {{200, 30, 200}, {300, 0, 300}},                                              // everything behind or beside
        {{-20, 15, -20}, {20, 0, 20}}, {{65, 12, -64}, {0, 0, 0}},
    };
    std::uint64_t culledTotal = 0, vboTotal = 0;
    bool fullyCulledPose = false;
    for (std::size_t i = 0; i < poses.size(); ++i) {
        Camera camera; camera.eye = poses[i].eye; camera.target = poses[i].target; camera.verticalFovDegrees = 50.0f;
        camera.farPlane = 400.0f;
        const auto before = counters;
        const auto expected = render(window, renderer, dynamicMesh, camera);
        const auto mid = counters;
        check(mid.vboDraws == before.vboDraws, "Non-static mesh used buffer objects");
        const auto actual = render(window, renderer, staticMesh, camera);
        check(expected == actual, "Static (VBO/culled) render differs from client-array render at pose " + std::to_string(i));
        culledTotal += counters.culledRanges - mid.culledRanges;
        vboTotal += counters.vboDraws - mid.vboDraws;
        if (counters.culledRanges - mid.culledRanges == staticMesh.ranges.size()) fullyCulledPose = true;
        if (i == 0) { bool any = false; for (std::size_t p = 0; p < expected.size(); p += 3) if (expected[p] > 30 || expected[p + 1] > 30) any = true; check(any, "Pose 0 rendered nothing (test is not exercising draws)"); }
    }
    check(culledTotal > 0, "No range was ever frustum-culled");
    check(fullyCulledPose, "A pose with the whole level outside the view did not cull every range");
    if (Window::gl_proc("glGenBuffers")) check(vboTotal > 0, "Static mesh never used buffer objects although the driver exposes them");
    else std::cout << "NOTE driver without buffer objects: client-array fallback verified only\n";

    // Replaced level geometry: invalidate, mutate, and the cached copy must follow.
    for (auto& v : staticMesh.vertices) v.color[0] = 1.0f - v.color[0];
    dynamicMesh = staticMesh; dynamicMesh.staticGeometry = false;
    renderer.invalidateStaticGeometry();
    Camera camera; camera.eye = poses[0].eye; camera.target = poses[0].target; camera.verticalFovDegrees = 50.0f; camera.farPlane = 400.0f;
    check(render(window, renderer, dynamicMesh, camera) == render(window, renderer, staticMesh, camera), "Invalidated static mesh kept stale buffers");
    std::cout << "PASS static geometry pixel-identical to client arrays over " << poses.size() << " poses; culled=" << culledTotal << " vboDraws=" << vboTotal << "\n";
    return 0;
} catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; } }
