#pragma once

#include <cstdint>
#include <string_view>

namespace dh2::ui {

struct SplashViewportV1 {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t width{};
    std::int32_t height{};
};

// Startup art uses its authored 1280x752 source rectangle and scales across
// the complete physical display. Keeping this rectangle shared by both GSInit
// and the loading movie also keeps the Touch-to-Continue clip on the same
// screen transform as its background.
constexpr SplashViewportV1 splash_full_viewport_v1(std::int32_t width,
                                                   std::int32_t height) noexcept {
    return {0, 0, width, height};
}

// GSInit::Update stage 7 (0x3851b4..0x385250), including its language
// precedence over the native phone-width cell.
constexpr const char* splash_source_uri_v1(std::int32_t width,
                                           std::int32_t language) noexcept {
    return language == 4 ? "data/3d/textures/splash_final_jp.tga" :
           language == 5 ? "data/3d/textures/splash_final_kor.tga" :
           width == 800 ? "data/3d/textures/splash_final_i9000.tga" :
           width == 854 ? "data/3d/textures/splash_final_droid.tga" :
                          "data/3d/textures/splash_final.tga";
}

constexpr bool is_splash_source_uri_v1(std::string_view uri) noexcept {
    return uri == "data/3d/textures/splash_final.tga" ||
           uri == "data/3d/textures/splash_final_droid.tga" ||
           uri == "data/3d/textures/splash_final_i9000.tga" ||
           uri == "data/3d/textures/splash_final_jp.tga" ||
           uri == "data/3d/textures/splash_final_kor.tga";
}

// The supplied droid/i9000 menu_splash background has bounds
// [-4838,4780,-3284,3141] and a 9.9993896484375 bitmap matrix. Against
// the shipping atlas this samples only 962x643 of GSInit's 1280x752 art.
// Correct that one fill's sampling; its geometry, animated world transform,
// prompt, and every other atlas sprite retain their authored values.
inline bool splash_android_background_uv_v1(const float input[6],
                                             float output[6]) noexcept {
    constexpr float scale = 9.9993896484375f;
    constexpr float authored[6]{1.f/scale, 0.f, 4838.f/scale,
                                0.f, 1.f/scale, 3284.f/scale};
    for (unsigned i = 0; i < 6; ++i) {
        const float difference = input[i] - authored[i];
        const float tolerance = i == 2 || i == 5 ? .001f : .000001f;
        if (!(difference >= -tolerance && difference <= tolerance)) return false;
    }
    constexpr float x = 1280.f / (4780.f + 4838.f);
    constexpr float y = 752.f / (3141.f + 3284.f);
    const float corrected[6]{x, 0.f, 4838.f*x, 0.f, y, 3284.f*y};
    for (unsigned i = 0; i < 6; ++i) output[i] = corrected[i];
    return true;
}

} // namespace dh2::ui
