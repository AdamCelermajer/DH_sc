// Wave 05 reconstruction (recon v2).
#include "glitch-scene-collada-gui/recon/v2/wave_05.hpp"

#include <cstddef>
#include <cstring>

namespace glitch::collada::recon_v2_05 {

// 0x618e68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getIdentityValueEPv
// 0x618e78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getIdentityValueEPv
// Pseudocode: return LC_API_MEMSET_0(a2, 0, 0x10u);  (a2 is the out pointer, 16 bytes = float[4])
void* get_identity_float4_zero(void* out) {
    std::memset(out, 0, 0x10);
    return out;
}

}  // namespace glitch::collada::recon_v2_05
