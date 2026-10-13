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
    // Mouse (1) and key (2) input events are never forwarded.
    if (event.EventType == glitch::EET_MOUSE_INPUT_EVENT || event.EventType == glitch::EET_KEY_INPUT_EVENT) {
        return false;
    }
    // A GUI event (type 0) whose caller is the environment's own list head is dropped.
    if (event.EventType == glitch::EET_GUI_EVENT && event.GUIEvent.Caller == static_cast<const void*>(&childrenSentinel)) {
        return false;
    }
    // Forward through the receiver's vtable +0x08 slot (glitch::IEventReceiver::onEvent), passing the event unchanged.
    return userEventReceiver->onEvent(event);
}

}  // namespace glitch::gui
