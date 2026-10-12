// Track B batch e1-glitch-scene-collada-gui-glitch_gui_CGUIEnvironment-0
// (owner glitch::gui::CGUIEnvironment). Layout and declarations: gui_elements.hpp.
#include "glitch-scene-collada-gui/gui_elements.hpp"

namespace glitch::gui {

// 0x535710 _ZN6glitch3gui15CGUIEnvironment7onEventERKNS_6SEventE
// Partial: see the note on the declaration in gui_elements.hpp.
bool CGUIEnvironment::onEvent(const glitch::SEvent& event) {
    // +0x1c4: without a user receiver nothing is forwarded.
    if (userEventReceiver == nullptr) {
        return false;
    }
    // Categories 1 and 2 are never forwarded.
    if (event.category == 1 || event.category == 2) {
        return false;
    }
    // A GUI event (category 0) whose caller is the environment's own list head is dropped.
    if (event.category == 0 && event.caller == static_cast<const void*>(&childrenSentinel)) {
        return false;
    }
    // Forward through the receiver's vtable +0x08 slot, passing the event unchanged.
    return userEventReceiver->forwardEvent(event);
}

}  // namespace glitch::gui
