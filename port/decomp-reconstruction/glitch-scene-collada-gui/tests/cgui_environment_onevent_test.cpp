// Behaviour test for glitch::gui::CGUIEnvironment::onEvent (0x535710).
#include <cstdio>

#include "glitch-core-io/io/interfaces.hpp"  // complete io::IFileSystem for the environment's counting handle
#include "glitch-scene-collada-gui/gui_elements.hpp"

namespace {

struct RecordingReceiver : glitch::IEventReceiver {
    int calls = 0;
    bool result = true;
    bool onEvent(const glitch::SEvent&) override {
        ++calls;
        return result;
    }
};

int failures = 0;

void check(bool condition, const char* what) {
    if (!condition) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

}  // namespace

int main() {
    glitch::gui::CGUIEnvironment env;

    // No receiver: nothing is forwarded.
    glitch::SEvent gui{};
    gui.EventType = glitch::EET_GUI_EVENT;
    check(env.onEvent(gui) == false, "no receiver returns false");

    RecordingReceiver receiver;
    env.userEventReceiver = &receiver;

    // Categories 1 and 2 are rejected without forwarding.
    glitch::SEvent cat1{};
    cat1.EventType = glitch::EET_MOUSE_INPUT_EVENT;
    glitch::SEvent cat2{};
    cat2.EventType = glitch::EET_KEY_INPUT_EVENT;
    check(env.onEvent(cat1) == false && env.onEvent(cat2) == false, "mouse and key events are rejected");
    check(receiver.calls == 0, "rejected categories do not reach the receiver");

    // GUI event whose caller is the list head: dropped.
    glitch::SEvent fromRoot{};
    fromRoot.EventType = glitch::EET_GUI_EVENT;
    fromRoot.GUIEvent.Caller = static_cast<const void*>(&env.childrenSentinel);
    check(env.onEvent(fromRoot) == false, "GUI event from the list head is dropped");
    check(receiver.calls == 0, "dropped GUI event does not reach the receiver");

    // GUI event from another caller: forwarded, return value passed through.
    glitch::SEvent fromElement{};
    fromElement.EventType = glitch::EET_GUI_EVENT;
    int otherElement = 0;
    fromElement.GUIEvent.Caller = &otherElement;
    check(env.onEvent(fromElement) == true, "GUI event from another caller is forwarded");
    check(receiver.calls == 1, "forwarded GUI event reaches the receiver once");

    receiver.result = false;
    check(env.onEvent(fromElement) == false, "receiver result is returned");

    // A category other than 0, 1 and 2 is forwarded even when its caller is the list head.
    glitch::SEvent other{};
    other.EventType = glitch::EET_LOG_TEXT_EVENT;
    other.GUIEvent.Caller = static_cast<const void*>(&env.childrenSentinel);
    const int before = receiver.calls;
    (void)env.onEvent(other);
    check(receiver.calls == before + 1, "a log event is forwarded regardless of caller");

    if (failures == 0) {
        std::printf("cgui_environment_onevent_test: all checks passed\n");
        return 0;
    }
    return 1;
}
