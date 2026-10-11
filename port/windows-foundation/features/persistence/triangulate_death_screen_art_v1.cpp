#include "death_screen_source_paths_v1.hpp"

#include "../../../engine-ui/vendor/gameswf1714/base/ear_clip_triangulate.h"

#include <array>
#include <iostream>
#include <vector>

namespace {
template <std::size_t N>
void emit_fill(int fill_id, const std::array<const float*, N>& data,
               const std::array<std::size_t, N>& sizes) {
    std::array<array<float>, N> paths;
    for (std::size_t contour = 0; contour < N; ++contour) {
        for (std::size_t point = 0; point < sizes[contour]; ++point) {
            paths[contour].push_back(data[contour][point * 2]);
            paths[contour].push_back(data[contour][point * 2 + 1]);
        }
    }
    array<float> triangles;
    ear_clip_triangulate::compute(&triangles, static_cast<int>(N), &paths[0]);
    if (triangles.size() % 6 != 0) return;
    const std::array<int, 4> rgba = fill_id == 1
        ? std::array<int, 4>{0, 0, 0, 255}
        : std::array<int, 4>{255, 0, 0, 255};
    std::cout << "FILL " << fill_id << ' ' << triangles.size() / 2 << ' '
              << rgba[0] << ' ' << rgba[1] << ' ' << rgba[2] << ' ' << rgba[3] << '\n';
    for (int i = 0; i < triangles.size(); i += 2)
        std::cout << triangles[i] << ' ' << triangles[i + 1] << '\n';
}
} // namespace

int main() {
    using namespace death_screen_source_paths_v1;
    emit_fill(1, kFill1Data, kFill1Sizes);
    emit_fill(2, kFill2Data, kFill2Sizes);
    return 0;
}
