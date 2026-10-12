// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CRootSceneNode-0: glitch::collada::CRootSceneNode.
// Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/root_scene_node.hpp.

#include "glitch-scene-collada-gui/root_scene_node.hpp"

namespace glitch::collada {

// 0x65b3dc _ZN6glitch7collada14CRootSceneNode10onPostLoadEv
// Partial. Reconstructed: the three direct calls to the attach helpers in order (BL targets 0x65b380,
// 0x65b350, 0x65ae70), the byte store of 1 at +0x1a8, and the OR of 0x100 into the word at +0x11c.
// Not reconstructed, so the row stays partial:
//   1. The virtual call through the vptr at +0xb8 with (this, true). Address-point-relative slot is
//      ISceneNode::updateAbsolutePosition(bool) (vtable address point 0x1c, checked against the raw
//      vtable entries). CRootSceneNode has no virtual methods in this tree, and declaring them would
//      give it a vptr with no definitions behind it (tests construct it by value and would fail to
//      link), so the dispatch is left out here.
//   2. The virtual call through the vptr at +0xf4 with (this). Slot is CSceneNode::computeBoundingBox()
//      (returns int, Hex-Rays body at 0x65cda8). Its int result is the original's return value, so the
//      return below is a placeholder 0.
//   3. The three attach helpers are direct calls and are declared, but their bodies are not in the
//      source tree yet (their ledger rows are partial); onPostLoad links only once they exist.
int CRootSceneNode::onPostLoad() {
    attachSkin();              // BL 0x65b380
    attachCameras();           // BL 0x65b350
    attachParticleSystems();   // BL 0x65ae70

    // TODO(partial): vptr+0xb8 call, ISceneNode::updateAbsolutePosition(true) on this. Not dispatched.
    int result = 0;            // TODO(partial): replace with the int returned by the vptr+0xf4 call.

    unk_01a8 = 1;              // +0x1a8 (byte store)
    unk_011c |= 0x100u;        // +0x11c (flags word, OR 0x100)
    return result;
}

}  // namespace glitch::collada
