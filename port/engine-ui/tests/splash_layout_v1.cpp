#include "splash_layout_v1.hpp"

#include <cassert>

int main() {
    constexpr auto sixteen_nine = dh2::ui::splash_full_viewport_v1(1920, 1080);
    static_assert(sixteen_nine.x == 0 && sixteen_nine.y == 0);
    static_assert(sixteen_nine.width == 1920 && sixteen_nine.height == 1080);
    constexpr float sixteen_nine_scale_x = float(sixteen_nine.width) / 1280.0f;
    constexpr float sixteen_nine_scale_y = float(sixteen_nine.height) / 752.0f;
    static_assert(sixteen_nine_scale_x > sixteen_nine_scale_y);

    constexpr auto twenty_nine = dh2::ui::splash_full_viewport_v1(2400, 1080);
    static_assert(twenty_nine.x == 0 && twenty_nine.y == 0);
    static_assert(twenty_nine.width == 2400 && twenty_nine.height == 1080);
    constexpr float twenty_nine_scale_x = float(twenty_nine.width) / 1280.0f;
    constexpr float twenty_nine_scale_y = float(twenty_nine.height) / 752.0f;
    static_assert(twenty_nine_scale_x > twenty_nine_scale_y);

    // The authored image rectangle maps to all four display edges in both
    // cases; independent axis scales deliberately avoid bars and cropping.
    assert(sixteen_nine.x + sixteen_nine.width == 1920);
    assert(sixteen_nine.y + sixteen_nine.height == 1080);
    assert(twenty_nine.x + twenty_nine.width == 2400);
    assert(twenty_nine.y + twenty_nine.height == 1080);
}
