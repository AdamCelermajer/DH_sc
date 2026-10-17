#include "../design_settings_xp_provider_v1.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace xp = dh2::design_settings_xp_provider_v1;

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("expected design_pyarray.bin path");
        std::ifstream input(argv[1], std::ios::binary);
        if (!input) throw std::runtime_error("cannot read DesignSettings cache");
        const std::vector<std::uint8_t> bytes{
            std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
        xp::View view{bytes.data(), bytes.size()};
        const std::array<std::pair<std::uint32_t, float>, 7> expected{{
            {160, 900.0f}, {156, 5.0f}, {140, 10.0f}, {144, 20.0f},
            {152, 0.0f}, {148, 200.0f}, {164, 0.025f}}};
        for (const auto& item : expected) {
            float value = -1.0f;
            std::string error;
            check(xp::read(&view, item.first, &value, error) == 0,
                  "source XP DesignSettings field rejected");
            check(std::fabs(value - item.second) < 1e-6f,
                  "source XP DesignSettings field changed");
        }
        float untouched = 123.0f;
        std::string error;
        check(xp::read(&view, 168, &untouched, error) != 0 &&
              untouched == 123.0f, "unsupported XP field mutated output");
        xp::View short_view{bytes.data(), 8};
        check(xp::read(&short_view, 160, &untouched, error) != 0 &&
              untouched == 123.0f, "truncated DesignSettings table mutated output");
        std::int32_t combat_music_trigger=-1;
        check(xp::read_combat_music_trigger(&view,&combat_music_trigger,error)==0&&
              combat_music_trigger==4,
              "source DesignSettings CombatMusicTrigger at +0x14 changed");
        combat_music_trigger=123;
        check(xp::read_combat_music_trigger(&short_view,&combat_music_trigger,error)!=0&&
              combat_music_trigger==123,
              "truncated combat-music DesignSettings table mutated output");
        std::puts("PASS: seven XP DesignSettings source fields and fail-closed bounds");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "FAIL: %s\n", error.what());
        return 1;
    }
}
