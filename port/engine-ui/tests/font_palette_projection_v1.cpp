#include "../font_palette_projection_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

static unsigned checks;
static void check(bool ok, const char* why) {
    ++checks;
    if (!ok) throw std::runtime_error(std::string(why) + " #" + std::to_string(checks));
}
static std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    check(bool(f), "fixture open");
    return {std::istreambuf_iterator<char>(f), {}};
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "asset directory argument");
        const std::string root = argv[1];
        auto palette = read(root + "/fonts_pyarray.bin");
        auto power = read(root + "/loot_audiovisual_pycst.bin");
        dh2_pycst_view constants{};
        check(dh2_pycst_open(&constants, power.data(), std::uint32_t(power.size())) == 0,
              "actual ItemPowerColor constants parse");

        const std::uint32_t indices[] = {6, 4, 1, 5, 3, 2, 2};
        for (int powers = 0; powers <= 6; ++powers) {
            std::uint32_t actual = 0;
            std::string error;
            check(dh2::ui::font_palette_text_color_v1(palette.data(), palette.size(), constants,
                                                       powers, actual, error), "exact palette projection");
            const auto at = 4u + std::size_t(indices[powers]) * 8u + 4u;
            const auto* b = palette.data() + at;
            const std::uint32_t expected = std::uint32_t(b[0]) | (std::uint32_t(b[1]) << 8) |
                (std::uint32_t(b[2]) << 16) | (std::uint32_t(b[3]) << 24);
            check(actual == expected, "source power-count palette index");
        }
        std::string formatted;
        check(dh2::ui::format_item_name_v1("Blade", 0x01a2b3, formatted) &&
              formatted == "<font color='#01A2B3'>Blade</font>", "source uppercase %06X format");
        check(dh2::ui::format_item_name_v1("Blade", 0x1234567, formatted) &&
              formatted == "<font color='#1234567'>Blade</font>", "printf width is minimum");

        std::uint32_t retained = 0xdeadbeef;
        std::string error;
        check(!dh2::ui::font_palette_text_color_v1(nullptr, palette.size(), constants, 1, retained, error) &&
              retained == 0xdeadbeef && !error.empty(), "null input fails closed");
        auto truncated = palette;
        truncated.pop_back();
        check(!dh2::ui::font_palette_text_color_v1(truncated.data(), truncated.size(), constants, 1,
                                                    retained, error) && retained == 0xdeadbeef,
              "truncated exact array rejected");
        check(!dh2::ui::font_palette_text_color_v1(palette.data(), palette.size(), constants, -1,
                                                    retained, error) && retained == 0xdeadbeef,
              "negative power count rejected");
        std::cout << "{\"validation\":\"PASS\",\"palette_entries\":7,\"source_power_cases\":7,\"checks\":"
                  << checks << ",\"mismatches\":0}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
