// Instantiates the CVirtualEx stand-in for two applicator tags, so the defaulted
// destructor that covers the empty D1 instantiations is compiled and run.
#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

#include <cstdio>

using glitch::collada::animation_track::CVirtualEx;

namespace {
struct TagQuaternionFloat {};
struct TagVector3dShort {};
}  // namespace

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    {
        CVirtualEx<TagQuaternionFloat> quaternionTrack;
        CVirtualEx<TagVector3dShort>* heapTrack = new CVirtualEx<TagVector3dShort>();
        delete heapTrack;
        CHECK(sizeof(quaternionTrack) == 1);
    }
    {
        // Quaternion identity: three zero words and w = 1.0f.
        std::uint32_t quaternion[4] = {9u, 9u, 9u, 9u};
        glitch::collada::animation_track::CIdentityValueWriter<4, true>::getIdentityValue(quaternion);
        CHECK(quaternion[0] == 0u && quaternion[1] == 0u && quaternion[2] == 0u);
        CHECK(quaternion[3] == 0x3f800000u);
        // Plain identity: every word of the value is zeroed.
        std::uint32_t wide[12];
        for (auto& w : wide) w = 0xffffffffu;
        glitch::collada::animation_track::CIdentityValueWriter<12, false>::getIdentityValue(wide);
        for (auto w : wide) CHECK(w == 0u);
    }
    std::printf("%s\n", failures ? "animation_track_virtual_ex: FAILED" : "animation_track_virtual_ex: ok");
    return failures != 0;
}
