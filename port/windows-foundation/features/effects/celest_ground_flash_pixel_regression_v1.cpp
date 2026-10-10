#include "../../../engine-textures/textures.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "original ground-flash texture path required");
        std::ifstream input(argv[1], std::ios::binary);
        check(bool(input), "original ground-flash texture missing");
        std::vector<std::uint8_t> encoded((std::istreambuf_iterator<char>(input)), {});
        dh2::textures::View view{};
        check(dh2_texture_open(encoded.data(), encoded.size(), &view) == dh2::textures::Error::ok,
              "foundation texture decoder rejected original source bytes");
        check(view.width == 128 && view.height == 128 && view.alpha,
              "original source texture dimensions/alpha changed");
        std::vector<std::uint8_t> rgba(std::size_t(view.width) * view.height * 4);
        check(dh2_texture_decode(&view, rgba.data(), rgba.size()) == dh2::textures::Error::ok,
              "foundation texture decoder failed original source pixels");

        std::size_t zero_alpha = 0, perimeter = 0, perimeter_alpha = 0;
        std::size_t perimeter_rgb = 0, zero_alpha_rgb = 0;
        std::array<std::uint8_t, 4> representative{};
        bool found_representative = false;
        for (unsigned y = 0; y < view.height; ++y) for (unsigned x = 0; x < view.width; ++x) {
            const auto offset = (std::size_t(y) * view.width + x) * 4;
            const auto r = rgba[offset], g = rgba[offset + 1], b = rgba[offset + 2], a = rgba[offset + 3];
            const bool nonzero_rgb = r || g || b;
            if (!a) {
                ++zero_alpha;
                if (nonzero_rgb) ++zero_alpha_rgb;
            }
            if (x && y && x + 1 < view.width && y + 1 < view.height) continue;
            ++perimeter;
            if (a) ++perimeter_alpha;
            if (nonzero_rgb) ++perimeter_rgb;
            if (!found_representative && a == 0 && nonzero_rgb) {
                representative = {r, g, b, a};
                found_representative = true;
            }
        }
        check(zero_alpha == 3906 && perimeter == 508 && perimeter_alpha == 166 &&
              perimeter_rgb == 431 && found_representative,
              "decoded source texture edge statistics differ from the pinned source evidence");

        // Source pass: ONE,ONE + FUNC_ADD; GL_MODULATE; no alpha test.
        // Pick the first zero-alpha/nonzero-RGB perimeter texel at pixel center
        // over black, where nearest/linear sampling agree at the center.
        constexpr std::array<float, 3> material_color{0.588235f, 0.588235f, 0.588235f};
        std::array<unsigned, 3> additive{};
        std::array<unsigned, 3> alpha_blend{};
        for (unsigned c = 0; c < 3; ++c) {
            const float source = (float(representative[c]) / 255.0f) * material_color[c];
            additive[c] = unsigned(source * 255.0f + 0.5f); // dst=0, ONE/ONE
            alpha_blend[c] = 0; // src alpha=0, conventional SRC_ALPHA/ONE_MINUS_SRC_ALPHA
        }
        check(additive[0] || additive[1] || additive[2],
              "source additive pass unexpectedly suppresses the zero-alpha RGB edge");
        check(additive != alpha_blend,
              "regression no longer distinguishes authored ONE/ONE from alpha blending");
        std::cout << "PASS source-pixel/pass regression: decoded=128x128 zero_alpha=" << zero_alpha
                  << " perimeter_rgb=" << perimeter_rgb << "/" << perimeter
                  << " representative_rgba=" << unsigned(representative[0]) << ','
                  << unsigned(representative[1]) << ',' << unsigned(representative[2]) << ','
                  << unsigned(representative[3]) << " ONE/ONE_rgb=" << additive[0] << ','
                  << additive[1] << ',' << additive[2] << " alpha_blend_rgb=0,0,0\n";
    } catch (const std::exception& e) {
        std::cerr << "FAIL " << e.what() << '\n';
        return 1;
    }
    return 0;
}
