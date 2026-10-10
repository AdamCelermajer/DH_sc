// Consistency checks of the exported original menu_Loading geometry (loading_art_data.cpp).
#include "loading_art_v1.hpp"

#include <cstdio>

using namespace dh::foundation::startup;

static int failures = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                          \
        }                                                                        \
    } while (0)

static void check_span(const LayerSpan& span) {
    CHECK(span.count > 0);
    for (int i = 0; i < span.count; ++i) {
        const Layer& layer = span.layers[i];
        CHECK(layer.verts.size() % 3 == 0 && !layer.verts.empty());
        CHECK(layer.texture >= 0 && layer.texture <= 6);
        if (layer.texture)
            for (const auto& v : layer.verts) CHECK(v.u >= -0.01f && v.u <= 1.01f && v.v >= -0.01f && v.v <= 1.01f);
    }
}

int main() {
    const LoadingArt& art = loading_art();
    check_span(art.bg);
    check_span(art.frame);
    check_span(art.track);
    check_span(art.fill);
    check_span(art.spark);
    // The progress mask grows with the frame: empty-ish at 0 %, the whole bar at 100 %, and the spark
    // leaves the bar on the last frame (original colour transform alpha 0).
    CHECK(art.frames[0].maskX1 - art.frames[0].maskX0 < 20.0f);
    for (int i = 1; i <= 100; ++i) CHECK(art.frames[i].maskX1 >= art.frames[i - 1].maskX1);
    CHECK(art.frames[100].maskX1 - art.frames[100].maskX0 > 550.0f);
    CHECK(art.frames[50].sparkAlpha > 0.0f && art.frames[100].sparkAlpha == 0.0f);
    CHECK(art.heading.w > 0 && art.tip.w > 0 && art.heading.size > art.tip.size);
    CHECK(art.stageW == 1024.0f && art.stageH == 768.0f);
    if (failures) return 1;
    std::puts("loading art v1 tests passed");
    return 0;
}
