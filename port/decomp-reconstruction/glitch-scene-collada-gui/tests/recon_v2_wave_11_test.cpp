// Wave 11 test: checks the layout members this wave relies on and the row counts.
#include <cstddef>
#include <cstdio>

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
    check(sizeof(ac::SAnimationAccessor) == 8, "SAnimationAccessor is 8 bytes");
    check(offsetof(ac::SAnimationAccessor, outputs) == 4, "outputs at +0x04");
    check(offsetof(ac::animation_track::CApplicatorInfo, parameterId) == 8, "parameterId at +0x08");
    check(sizeof(ac::animation_track::CMaterialApplicatorInfo) == 0x0c, "CMaterialApplicatorInfo is 0x0c");
    check(rv::kThunkRows + rv::kPartialRows == 100, "wave 11 has 100 rows");
    std::printf("%s: %u thunks skipped, %u partial\n", failures ? "FAILED" : "PASSED",
                rv::kThunkRows, rv::kPartialRows);
    return failures ? 1 : 0;
}
