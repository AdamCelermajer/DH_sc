// Wave 14 (recon_v2): empty or constant-return particle attribute hooks.
// Addresses 0x6f76d0, 0x6f76e4, 0x6f837c, 0x6f8390 (slice_33).
// The owning classes (IParticleEmitter, IParticleAffector) are not declared in
// the allowed layout headers, so the IAttributes and option pointers are opaque.
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_14_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_14_HPP

namespace glitch {
namespace scene {
namespace recon_v2_14 {

// 0x6f76d0 _ZNK6glitch5scene16IParticleEmitter19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void IParticleEmitter_serializeAttributes(const void* self, void* attributes, const void* options);

// 0x6f76e4 _ZN6glitch5scene16IParticleEmitter21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
int IParticleEmitter_deserializeAttributes(void* self, int index, void* attributes, void* options);

// 0x6f837c _ZNK6glitch5scene17IParticleAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void IParticleAffector_serializeAttributes(const void* self, void* attributes, const void* options);

// 0x6f8390 _ZN6glitch5scene17IParticleAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
int IParticleAffector_deserializeAttributes(void* self, int index, void* attributes, void* options);

}  // namespace recon_v2_14
}  // namespace scene
}  // namespace glitch

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_14_HPP
