// Test for the wave 08 animation_track helpers. Guest memory is mapped below
// 4 GiB (MAP_32BIT) so 32-bit guest words can hold host addresses.

#include <cstdio>
#include <cstring>

#include "glitch-scene-collada-gui/recon/v2/wave_08.hpp"
#if defined(__linux__)  // MAP_32BIT guest memory is Linux-only
#include <sys/mman.h>

using namespace glitch::collada;
using namespace glitch::collada::recon_v2_08;

static int failures = 0;
#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                \
        }                                                              \
    } while (0)

int main() {
    const std::size_t kSize = 4096;
    void* mem = mmap(nullptr, kSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    if (mem == MAP_FAILED) {
        std::printf("mmap MAP_32BIT failed\n");
        return 1;
    }
    auto* base = static_cast<std::uint8_t*>(mem);
    const std::uint32_t g = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(base));
    std::memset(base, 0, kSize);

    // Layout within the arena (guest addresses = g + offset).
    const std::uint32_t kTrackOff = 0x000;     // SAnimationAccessorTrack
    const std::uint32_t kKeyframeOff = 0x100;  // SAnimationKeyframe[2], stride 28
    const std::uint32_t kOutputOff = 0x200;    // output slots, stride 8
    const std::uint32_t kRowsOff = 0x300;      // SLightColorRow[3], stride 3

    // Track: keyframes pointer at +0x08.
    auto* track = reinterpret_cast<SAnimationAccessorTrack*>(base + kTrackOff);
    track->keyframes = g + kKeyframeOff;

    // Keyframes: outputIndex at +0x18 of each 28-byte entry. Both use output 0.
    auto* kf = reinterpret_cast<SAnimationKeyframe*>(base + kKeyframeOff);
    kf[0].outputIndex = 0;
    kf[1].outputIndex = 0;

    // Output slot 0 sits at outputs + 8*0 + 4; its data word is at +4 of that address.
    // getOutput(a, 0) therefore returns outputs + 4 and the rows pointer is at outputs + 8.
    const std::uint32_t kOutputs = g + kOutputOff;
    auto* words = reinterpret_cast<std::uint32_t*>(base + kOutputOff);
    words[2] = g + kRowsOff;  // outputs + 8

    // Rows: row0 = (10,20,30), row1 = (50,60,70), row2 = (100,100,100).
    auto* rows = reinterpret_cast<SLightColorRow*>(base + kRowsOff);
    rows[0] = {{10, 20, 30}};
    rows[1] = {{50, 60, 70}};
    rows[2] = {{100, 100, 100}};

    SAnimationAccessor acc{};
    acc.track = g + kTrackOff;
    acc.outputs = kOutputs;

    // getOutput returns outputs + 8*idx + 4.
    CHECK(getOutput(acc, 0) == reinterpret_cast<const SAnimationOutputSlot*>(base + kOutputOff + 4));

    // 0x6126c8: blend rows 0 and 1 at t = 0.5 -> (30, 40, 50); returns component 2.
    std::uint8_t out1[3] = {0, 0, 0};
    std::uint32_t r1 = CInterpreter_SLightColor_getKeyBasedValueEx(acc, 0, 0, 0.5f, out1);
    CHECK(out1[0] == 30 && out1[1] == 40 && out1[2] == 50);
    CHECK(r1 == 50);

    // 0x6126c8 at t = 0 returns row 0 exactly.
    std::uint8_t out1b[3] = {0, 0, 0};
    CInterpreter_SLightColor_getKeyBasedValueEx(acc, 0, 0, 0.0f, out1b);
    CHECK(out1b[0] == 10 && out1b[1] == 20 && out1b[2] == 30);

    // 0x612820: row a4 minus row a3 with a3 = 0, a4 = 1 -> (40, 40, 40); returns slot address.
    std::uint8_t out2[3] = {0, 0, 0};
    std::uint32_t r2 = CVirtualEx_CLightColor_getKeyBasedValue_delta(acc, 0, 1, out2);
    CHECK(out2[0] == 40 && out2[1] == 40 && out2[2] == 40);
    CHECK(r2 == kOutputs + 4);

    // 0x612874: lerp row a3 (0) toward row a4 (1) at t = 0.5 -> (30, 40, 50), minus row a2 (0) -> (20, 20, 20).
    std::uint8_t out3[3] = {0, 0, 0};
    std::uint32_t r3 = CInterpreter_SLightColor_getKeyBasedValueEx_offset(acc, 0, 0, 1, 0.5f, out3);
    CHECK(out3[0] == 20 && out3[1] == 20 && out3[2] == 20);
    CHECK(r3 == 50);

    // 0x612874 with a2 = 1 (row 1 = (50,60,70)) subtracted from lerp (row 0 -> row 2 at t = 0.5 -> (55,60,65)).
    std::uint8_t out4[3] = {0, 0, 0};
    CInterpreter_SLightColor_getKeyBasedValueEx_offset(acc, 1, 0, 2, 0.5f, out4);
    CHECK(out4[0] == 5 && out4[1] == 0 && out4[2] == 0xFB);  // 65 - 70 wraps to 0xFB

    munmap(mem, kSize);
    if (failures == 0) {
        std::printf("wave_08 test: all checks passed\n");
        return 0;
    }
    std::printf("wave_08 test: %d failure(s)\n", failures);
    return 1;
}

#else
int main() {
    std::printf("skipped: MAP_32BIT guest memory is Linux-only\n");
    return 0;
}
#endif
