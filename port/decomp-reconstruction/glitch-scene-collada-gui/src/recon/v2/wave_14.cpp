// Wave 14 (recon_v2): reconstructions of empty / constant-return attribute hooks.
#include "glitch-scene-collada-gui/recon/v2/wave_14.hpp"

namespace glitch {
namespace scene {
namespace recon_v2_14 {

// 0x6f76d0 glitch::scene::IParticleEmitter::serializeAttributes
// Original body is empty.
void IParticleEmitter_serializeAttributes(const void* /*self*/, void* /*attributes*/, const void* /*options*/) {}

// 0x6f76e4 glitch::scene::IParticleEmitter::deserializeAttributes
// Original body returns 0.
int IParticleEmitter_deserializeAttributes(void* /*self*/, int /*index*/, void* /*attributes*/, void* /*options*/) {
    return 0;
}

// 0x6f837c glitch::scene::IParticleAffector::serializeAttributes
// Original body is empty.
void IParticleAffector_serializeAttributes(const void* /*self*/, void* /*attributes*/, const void* /*options*/) {}

// 0x6f8390 glitch::scene::IParticleAffector::deserializeAttributes
// Original body returns 0.
int IParticleAffector_deserializeAttributes(void* /*self*/, int /*index*/, void* /*attributes*/, void* /*options*/) {
    return 0;
}

}  // namespace recon_v2_14
}  // namespace scene
}  // namespace glitch
