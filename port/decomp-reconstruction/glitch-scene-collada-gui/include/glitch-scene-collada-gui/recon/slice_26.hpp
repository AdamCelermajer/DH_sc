// Slice 26 reconstruction header (glitch::scene mesh cache, particle system,
// scene collision manager and collision/delete animators).
// Only free functions whose body needs no class member are declared here.
// Class members of the slice are not in any layout header yet, so their rows
// are ledgered partial with an unattached note.
#pragma once

namespace glitch::collada::recon_26 {

// 0x6c4170 _ZN6glitch5scene22CSceneCollisionManager13getLowestRootEffffPf
// Solves a*t^2 + b*t + c = 0 for the smallest root in (0, maxT). Writes it to
// *out and returns 1, or returns 0 when no root lies in range. The original is
// a CSceneCollisionManager member that never reads `this`.
int getLowestRoot(float a, float b, float c, float maxT, float* out);

}  // namespace glitch::collada::recon_26
