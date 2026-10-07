#pragma once
#include "vox_source_fields_v38.hpp"
namespace dh2::audio {
// Exact VoxSoundManager C1 instructions36c838..36c850. Called only when the
// genuine source manager is constructed, before its later Level listener update.
// This does not construct a listener, soundpack, World or output-ready gate.
inline void audio_constructor_distance_fields_v38(VoxListenerAuthorityV38&fields)noexcept{
 fields.reference_distance=1000;fields.maximum_distance=1000;fields.rolloff=1.f;
}
// Original Initialize36c3cc..36c3d8 invokes general integer parameter2,value4.
// Only that reached model field is updated; factor/speed/velocity are separate
// actual initialization authorities and must not be synthesized here.
inline void audio_initialize_distance_model_v38(AudioSpatialSourceV34&fields)noexcept{
 fields.distance_model=4;
}
}
