// Wave 03 reconstructions (partial-row rerun). Only the rows that could be
// written against the layout headers are declared here. Everything else in the
// wave stays in the ledger as partial or skipped (see ledger_parts/wave_03.csv).
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_03_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_03_HPP

#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::video {
class IVideoDriver;
}  // namespace glitch::video

namespace glitch::scene::recon_v2_03 {

// 0x59a188 _ZN6glitch5scene26createMeshUniquePrimitivesERKN5boost13intrusive_ptrINS0_5IMeshEEE
// Handle copy of an intrusive_ptr<IMesh>. Identical body to 0x59a1a4 (ICF).
IMesh** createMeshUniquePrimitives(IMesh** result, IMesh* const* a2);

// 0x59a1a4 _ZN6glitch5scene16createMeshWeldedERKN5boost13intrusive_ptrINS0_5IMeshEEEf
// Pseudocode takes two arguments (no float); the body is the same handle copy
// as 0x59a188 (ICF).
IMesh** createMeshWelded(IMesh** result, IMesh* const* a2);

// 0x59ba3c _ZN6glitch5scene22createMeshWith2TCoordsERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS_5video12IVideoDriverE
// Calls createMeshCopy (unmapped, extern) and returns the owned handle.
IMesh** createMeshWith2TCoords(IMesh** result, IMesh* const* a2, video::IVideoDriver* a3);

}  // namespace glitch::scene::recon_v2_03

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_03_HPP
