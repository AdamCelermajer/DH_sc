// Stand-ins for the event record and the user event receiver interface, as read by
// glitch::gui::CGUIEnvironment::onEvent. Only the words that the pseudocode reads are
// mapped; the rest of each record is not reconstructed.
#pragma once

#include <cstdint>

namespace glitch {

// glitch::SEvent. Word 0 is the category: 0 for GUI events, 1 and 2 are rejected by
// CGUIEnvironment::onEvent. Word 2 is the caller pointer, compared for GUI events.
struct SEvent {
    std::int32_t category = 0;     // +0x00, word 0
    std::int32_t word1 = 0;        // +0x04, not read by onEvent
    const void* caller = nullptr;  // +0x08, word 2
};

// glitch::IEventReceiver. Under the Itanium-style ARM vtable, slots +0x00 and +0x04 are
// the destructors, so the call made by onEvent through slot +0x08 is the first
// non-destructor virtual. The original name of that method is not mapped, so it is kept
// as forwardEvent.
class IEventReceiver {
public:
    virtual ~IEventReceiver() = default;
    // vtable +0x08 (name not mapped in the original)
    virtual bool forwardEvent(const SEvent& event) = 0;
};

}  // namespace glitch
