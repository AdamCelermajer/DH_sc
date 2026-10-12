// Track B batch e1-glitch-scene-collada-gui-glitch_gui_CGUICheckBox-0
// (owner glitch::gui::CGUICheckBox). Declaration: gui_simple_accessors.hpp.
#include "glitch-scene-collada-gui/gui_simple_accessors.hpp"

namespace glitch::gui {

// 0x6a7ed8 _ZN6glitch3gui12CGUICheckBox7onEventERKNS_6SEventE
// Partial: stub. The control flow below is confirmed from the Hex-Rays pseudocode and the
// assembly (decomp_asm.py), but no member it touches is declared on CGUICheckBox yet, so
// nothing is reconstructed and the function reports "not handled" to its caller.
//
// Flow to rebuild once the layout requests are applied (offsets in the original):
//   1. enabled byte at +0x99 is zero: forward to the parent (+0x24) through its onEvent
//      virtual (vtable +0x08), or return null when there is no parent.
//   2. event type word 0 selects the branch: 1 (mouse), 2 (keyboard), 0 (GUI, which clears
//      the pressed flag at +0x158 when word 2 is this and word 4 is zero); other types forward.
//   3. mouse, word 5 == 0 (left button down): pressed = 1, +0x15c = Timer::getTime(),
//      call the object at +0x150 through vtable +0x10 with (object, this) (setFocus in
//      the CGUIEnvironment vtable order), return true.
//   4. mouse, word 5 == 3 (left button up): call the object at +0x150 through vtable +0x18
//      (removeFocus order) with (object, this); clear pressed; if it was set, the parent is
//      present and (word 2, word 3) lies inside the rectangle +0x48..+0x54 (both edges
//      included), toggle the checked byte at +0x159 and send the parent a GUI event
//      {category 0, caller this, word 3 0, word 4 7}; return true.
//   5. keyboard, key down (byte +0x10 set): Return (13) or Space (32) sets pressed and
//      returns true; with pressed set, Escape (27) clears pressed and returns true; other
//      keys forward.
//   6. keyboard, key up with pressed set: Return or Space clears pressed; if a parent is
//      present, toggles +0x159 and sends it the same GUI event as step 4, then returns true;
//      without a parent it still returns true. Other keys forward.
bool CGUICheckBox::onEvent(const glitch::SEvent& event) {
    (void)event;
    return false;
}

}  // namespace glitch::gui
