// Wave 01 (recon v2) reconstructions. Free functions in glitch::collada::recon_v2_01
// that take the IGUIElement as `self`, the way the Hex-Rays pseudocode passes `this`.
#pragma once

#include "glitch-scene-collada-gui/layouts/gui.hpp"

namespace glitch::collada::recon_v2_01 {

// Child-list node, the 12-byte record the original allocates with GlitchAlloc(12, 0)
// for each child (addChild, bringToFront). Words: next, prev, element.
// The list head is the IGUIElement's own childrenNext / childrenPrev pair (the
// sentinel at +0x04 / +0x08).
struct ChildNode {
    void* next;
    void* prev;
    glitch::gui::IGUIElement* element;
};

// 0x535d70 _ZN6glitch3gui11IGUIElement12bringToFrontEPS1_
// Moves the child's node to the end of the child list. Returns 1 when the child was
// found and moved, 0 when it is not in the list.
int bringToFront(glitch::gui::IGUIElement* self, glitch::gui::IGUIElement* child);

}  // namespace glitch::collada::recon_v2_01
