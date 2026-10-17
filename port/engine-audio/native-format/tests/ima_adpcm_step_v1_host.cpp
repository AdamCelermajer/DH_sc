#include "../ima_adpcm_step_v1.hpp"

#include <cstdlib>
#include <iostream>

namespace a = dh2::engine_audio::native_format;

void require(bool ok, const char* message) {
    if (!ok) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

int main() {
    a::ImaAdpcmStateV1 s{0, 0};
    require(a::ima_adpcm_decode_nibble_v1(&s, 0) == 0 && s.step_index == 0,
            "zero nibble source arithmetic");
    require(a::ima_adpcm_decode_nibble_v1(&s, 7) == 11 && s.step_index == 8,
            "positive delta and index increase");
    require(a::ima_adpcm_decode_nibble_v1(&s, 8) == 9 && s.step_index == 7,
            "negative delta and index decrease");

    s = {32760, 88};
    require(a::ima_adpcm_decode_nibble_v1(&s, 7) == 32767 && s.step_index == 88,
            "positive predictor and index saturation");
    s = {-32760, 88};
    require(a::ima_adpcm_decode_nibble_v1(&s, 15) == -32768 && s.step_index == 88,
            "negative predictor saturation");
    s = {123, 0};
    require(a::ima_adpcm_decode_nibble_v1(&s, 16) == 0 && s.predictor == 123 && s.step_index == 0,
            "invalid nibble leaves state unchanged");
    require(a::ima_adpcm_decode_nibble_v1(nullptr, 0) == 0, "null state rejected");
    s = {123, 89};
    require(a::ima_adpcm_decode_nibble_v1(&s, 0) == 0 && s.predictor == 123 && s.step_index == 89,
            "invalid index leaves state unchanged");
    std::cout << "IMA ADPCM step v1: 8 cases passed; block/container decoding not tested\n";
}
