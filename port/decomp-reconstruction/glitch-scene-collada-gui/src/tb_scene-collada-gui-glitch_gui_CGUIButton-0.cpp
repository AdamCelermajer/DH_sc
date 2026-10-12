// Track B batch e1-glitch-scene-collada-gui-glitch_gui_CGUIButton-0
// (owner glitch::gui::CGUIButton). Declarations: gui_elements.hpp.
#include "glitch-scene-collada-gui/gui_elements.hpp"

namespace glitch::gui {

// 0x6a6eac _ZN6glitch3gui10CGUIButton7onEventERKNS_6SEventE
// Partial: stub body. The original (888 bytes) is not reconstructed yet. Its structure:
// a disabled button forwards the event to the receiver at +0x24; mouse, key and GUI events
// are dispatched on the event category, and a button click sends GUI event type 5 to that
// receiver. Setting the pressed state goes through the virtual setPressed (vtable +0x9c).
// Missing from the layout, so the body cannot be finished yet:
//   1. The IGUIElement base fields it reads: receiver pointer +0x24 and enabled +0x99.
//      CGUIButton does not derive from IGUIElement in gui_elements.hpp.
//   2. glitch::SEvent words 3 to 5 (+0x0c, +0x10, +0x14) and the key byte at +0x10.
//      deps/event_stand_ins.hpp maps words 0 and 2 only.
//   3. The host object at +0x150 and its vtable slots +0x10, +0x18 and +0x1c (type not mapped).
//   4. setPressed (vtable +0x9c) is not declared on CGUIButton in gui_elements.hpp.
bool CGUIButton::onEvent(const glitch::SEvent& event) {
    (void)event;
    return false;
}

}  // namespace glitch::gui
