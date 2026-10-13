// Wave 11 test: checks the layout members this wave relies on and the row counts.
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <type_traits>

#include "glitch-scene-collada-gui/recon/v2/wave_11.hpp"

namespace ac = glitch::collada;
namespace rv = glitch::collada::recon_v2_11;

int main() {
    int failures = 0;
    auto check = [&](bool ok, const char* what) {
        if (!ok) {
            std::printf("FAIL: %s\n", what);
            ++failures;
        }
    };
    // The layouts keep the 32-bit file/guest offsets with fixed-width guest words, so the checks are on the
    // member offsets and member types, not on sizeof (the build is 64-bit, see TRACK-B.md). The binary reads
    // the accessor at +0x00 (track), +0x04 (outputs) and +0x08 (key-search cache, SAnimationAccessor::findKeyFrameNo
    // 0x66b65c), so the object is longer than 8 bytes.
    check(offsetof(ac::SAnimationAccessor, track) == 0, "track at +0x00");
    check(offsetof(ac::SAnimationAccessor, outputs) == 4, "outputs at +0x04");
    check(offsetof(ac::SAnimationAccessor, cache) == 8, "cache at +0x08");
    check(std::is_same_v<decltype(ac::SAnimationAccessor::track), std::uint32_t>, "track is a guest word");
    check(std::is_same_v<decltype(ac::SAnimationAccessor::outputs), std::uint32_t>, "outputs is a guest word");
    check(std::is_same_v<decltype(ac::SAnimationAccessor::cache), std::uint32_t>, "cache is a guest word");
    check(offsetof(ac::animation_track::CApplicatorInfo, parameterId) == 8, "parameterId at +0x08");
    check(std::is_same_v<decltype(ac::animation_track::CApplicatorInfo::parameterId), std::uint16_t>,
          "parameterId is a 16-bit field");
    // CMaterialApplicatorInfo::clone (0x667ed0) allocates 0x0c bytes and copies the words at +0x04 and +0x08.
    check(offsetof(ac::animation_track::CMaterialApplicatorInfo, word_0x04) == 4, "word_0x04 at +0x04");
    check(offsetof(ac::animation_track::CMaterialApplicatorInfo, word_0x08) == 8, "word_0x08 at +0x08");
    check(rv::kThunkRows + rv::kPartialRows == 100, "wave 11 has 100 rows");
    std::printf("%s: %u thunks skipped, %u partial\n", failures ? "FAILED" : "PASSED",
                rv::kThunkRows, rv::kPartialRows);
    return failures ? 1 : 0;
}
