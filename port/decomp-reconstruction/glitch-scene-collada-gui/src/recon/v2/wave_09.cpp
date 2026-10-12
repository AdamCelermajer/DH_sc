// Wave 09 (recon v2). Pseudocode signatures kept; offsets not involved (memset only).
#include "glitch-scene-collada-gui/recon/v2/wave_09.hpp"

#include <cstring>

namespace glitch::collada::recon_v2_09 {

// 0x618E88 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE16getIdentityValueEPv
void *identity_618e88(int a1, void *a2) {
    (void)a1;
    std::memset(a2, 0, 0x10u);
    return a2;
}

// 0x618E98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE16getIdentityValueEPv
void *identity_618e98(int a1, void *a2) {
    (void)a1;
    std::memset(a2, 0, 0x10u);
    return a2;
}

// 0x618F18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getIdentityValueEPv
void *identity_618f18(int a1, void *a2) {
    (void)a1;
    std::memset(a2, 0, 0x10u);
    return a2;
}

}  // namespace glitch::collada::recon_v2_09
