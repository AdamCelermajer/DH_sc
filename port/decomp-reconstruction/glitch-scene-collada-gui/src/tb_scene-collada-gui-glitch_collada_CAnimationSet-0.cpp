// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CAnimationSet-0.
// Owner: glitch::collada::CAnimationSet (declared in glitch-scene-collada-gui/animation_set.hpp).
//
// Reconstructed here: setMismatchBehavior, getAnimationCount.
// Declared only (partial, no body yet): addAnimationLibrary (both overloads), addAnimation,
// CompileInternal, compile. Their missing inputs are listed next to each declaration in the header.

#include "glitch-scene-collada-gui/animation_set.hpp"

namespace glitch::collada {

// 0x62db98 glitch::collada::CAnimationSet::setMismatchBehavior(glitch::collada::CAnimationSet::E_MISMATCH_BEHAVIOR)
// +0x08 holds the behavior word; the function returns this (r0 in the original).
CAnimationSet& CAnimationSet::setMismatchBehavior(E_MISMATCH_BEHAVIOR behavior) {
    mismatchBehavior = behavior;
    return *this;
}

// 0x65f044 glitch::collada::CAnimationSet::getAnimationCount() const
// Count of the +0x24 database vector. The original computes (end - begin) >> 3 over 8-byte
// entries; pointer subtraction gives the same element count on the host.
std::uint32_t CAnimationSet::getAnimationCount() const {
    return static_cast<std::uint32_t>(databasesEnd - databases);
}

}  // namespace glitch::collada
