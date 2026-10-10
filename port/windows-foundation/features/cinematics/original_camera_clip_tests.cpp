// P16 CINE2: PlayCamera clip owner. Real cs_swamp_intro scene01 (dictionary id 359) from the staged package.
#include "../../original_camera_clip.hpp"
#include "../../asset_catalog.hpp"
#include <cmath>
#include <cstdio>
#include <string>

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    std::printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++failures;
}
bool same(const dh::foundation::CameraVec3& a, const dh::foundation::CameraVec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("usage: original_camera_clip_tests <assets dir>\n");
        return 2;
    }
    using namespace dh::foundation;
    AssetCatalog assets(argv[1]);
    CameraClipLibrary library;
    std::vector<std::uint8_t> bytes;
    std::string path, error;

    std::vector<std::uint8_t> scene;
    check(!library.read(assets, -1, bytes, scene, path, error) && !error.empty(), "negative dictionary id is rejected");
    check(!library.read(assets, 999999, bytes, scene, path, error) && error.find("absent") != std::string::npos,
          "out-of-range dictionary id is rejected");

    const bool read = library.read(assets, 359, bytes, scene, path, error);
    check(read && path.find("cs_swamp_intro_camera_scene01.bdae") != std::string::npos, "PlayCamera 359 resolves to cs_swamp_intro_camera_scene01");
    if (!read) {
        std::printf("read error: %s\n", error.c_str());
        return 1;
    }

    OriginalCameraClip clip;
    check(clip.load(scene, bytes, error), "scene01 binds to the level camera scene and loads its Player");
    if (!clip.loaded()) std::printf("load error: %s\n", error.c_str());
    check(clip.duration_ms() > 0, "scene01 has a positive authored duration");
    std::printf("range start=%d end=%d duration=%d ms\n", clip.start_ms(), clip.end_ms(), clip.duration_ms());

    CameraVec3 e0{}, t0{}, e1{}, t1{}, em{}, tm{};
    check(clip.sample(0, e0, t0, error), "sample at clip start");
    check(clip.sample(clip.duration_ms(), e1, t1, error), "sample at clip end");
    check(clip.sample(clip.duration_ms() / 2, em, tm, error), "sample at clip middle");
    std::printf("start eye=(%.2f,%.2f,%.2f) target=(%.2f,%.2f,%.2f)\n", e0.x, e0.y, e0.z, t0.x, t0.y, t0.z);
    std::printf("mid   eye=(%.2f,%.2f,%.2f) target=(%.2f,%.2f,%.2f)\n", em.x, em.y, em.z, tm.x, tm.y, tm.z);
    std::printf("end   eye=(%.2f,%.2f,%.2f) target=(%.2f,%.2f,%.2f)\n", e1.x, e1.y, e1.z, t1.x, t1.y, t1.z);
    check(!same(e0, e1) || !same(t0, t1), "the authored camera moves over the clip");
    check(std::isfinite(em.x) && std::isfinite(tm.y), "middle sample is finite");
    check(!same(e0, t0), "eye and target differ (camera has a look direction)");

    CameraVec3 past{}, pastT{};
    check(clip.sample(clip.duration_ms() + 5000, past, pastT, error) && same(past, e1) && same(pastT, t1),
          "samples past the end clamp to the clip end");

    OriginalCameraClip unloaded;
    CameraVec3 e{}, t{};
    check(!unloaded.sample(0, e, t, error) && !error.empty(), "sampling an unloaded clip is rejected");
    check(!clip.load(scene, std::vector<std::uint8_t>{1, 2, 3}, error) && !clip.loaded(), "a non-BRES clip is rejected and clears the clip");

    std::printf("%s (%d failure(s))\n", failures ? "FAILED" : "ALL PASS", failures);
    return failures ? 1 : 0;
}
