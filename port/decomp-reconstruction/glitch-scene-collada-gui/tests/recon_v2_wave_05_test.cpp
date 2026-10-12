// Wave 05 test: checks the identity getter body (zero 16 bytes, return out).
#include "glitch-scene-collada-gui/recon/v2/wave_05.hpp"

#include <cstdio>
#include <cstring>

int main() {
    float buf[4] = {1.f, 2.f, 3.f, 4.f};
    void* r = glitch::collada::recon_v2_05::get_identity_float4_zero(buf);
    bool ok = (r == buf);
    for (int i = 0; i < 4; ++i) ok = ok && buf[i] == 0.f;
    unsigned char raw[24];
    std::memset(raw, 0xAB, sizeof raw);
    glitch::collada::recon_v2_05::get_identity_float4_zero(raw);
    for (int i = 0; i < 16; ++i) ok = ok && raw[i] == 0;
    for (int i = 16; i < 24; ++i) ok = ok && raw[i] == 0xAB;
    std::printf("%s\n", ok ? "wave_05 test: PASS" : "wave_05 test: FAIL");
    return ok ? 0 : 1;
}
