// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CLightSceneNode-0.
// Owner class: glitch::scene::CLightSceneNode (declared in scene_nodes.hpp).
//
// All four functions are partial. No bodies are written yet because each one reads a
// value whose type is not reconstructed in this category (see the notes on each
// declaration in scene_nodes.hpp). Nothing here is guessed.

#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x583c18 _ZN6glitch5scene15CLightSceneNode13doLightRecalcEv
// Partial. Control flow (from Hex-Rays, not yet written as code):
//   light = this->light; count = light->+0x58 (uint16).
//   count > 1: when count == 2 zero the six box words (+0x13c..+0x154) and call
//              setAutomaticCulling(0), then re-read the count. Store count to recalcCount.
//   count <= 1: when light->+0x40 (float range) == FLT_MAX call setAutomaticCulling(0);
//              otherwise r = range*range*0.5, min = (-r, -r, -r), max = (r, r, r) and
//              setAutomaticCulling(1). Then store light->+0x58 to recalcCount.
// Missing: CLight fields at +0x40 and +0x58 (CLight is declared, not defined, here),
// and ISceneNode::setAutomaticCulling is not visible from CLightSceneNode because the
// class has no base yet. The original return value is an unused register leftover.
// No definition is written; calling it links to nothing until the body exists.

// 0x583cf0 _ZNK6glitch5scene15CLightSceneNode14getBoundingBoxEv
// Partial. Control flow: if recalcCount != light->+0x58, call doLightRecalc(); then
// return the box at +0x13c. Missing: CLight field at +0x58 (see doLightRecalc).

// 0x583d18 _ZN6glitch5scene15CLightSceneNode6renderEPv
// Partial. Control flow: d = (this->+0x110 object)->+0x14; if d is non-null call
// IVideoDriver::addDynamicLight(d, &light) (light at +0x134 passed by address).
// Missing: the type of the object at +0x110 (scene manager), and an IVideoDriver
// class definition in this category (only a forward declaration exists).

// 0x583d34 _ZN6glitch5scene15CLightSceneNode19onRegisterSceneNodeEv
// Partial. Control flow: m = this->+0x110 object; call vtable slot +36 of m with
// (m, this, &material, 0, 1, 0, 0x7FFFFFFF), release the intrusive_ptr to material,
// return 1. Missing: the class of the object at +0x110 and the function behind its
// vtable slot +36.

}  // namespace glitch::scene
