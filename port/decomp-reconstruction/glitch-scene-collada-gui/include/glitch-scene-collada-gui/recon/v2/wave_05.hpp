// Wave 05 reconstruction (recon v2). Free-function forms of the
// glitch::collada::animation_track::CVirtualEx<...>::getIdentityValue bodies
// whose pseudocode is a single zeroing call. No layout header is needed.
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_05_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_05_HPP

namespace glitch::collada::recon_v2_05 {

// 0x618e68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE16getIdentityValueEPv
// 0x618e78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE16getIdentityValueEPv
// Both are the same body: zero 16 bytes at out and return out.
void* get_identity_float4_zero(void* out);

}  // namespace glitch::collada::recon_v2_05

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_05_HPP
