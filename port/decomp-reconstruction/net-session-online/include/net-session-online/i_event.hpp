// IEvent: base of the session events. The event id is stored at +0x04 after the
// vtable pointer; the derived events set it.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x00 vptr  +0x04 event id
#pragma once

#include <cstdint>

class IEvent {
public:
    virtual ~IEvent() = default;  // 0x31d8d8 D1 (the deleting destructor 0x31fc20 is skipped)
    // 0x31d8dc _ZNK6IEvent10getEventIDEv
    std::uint32_t getEventID() const { return eventId_; }

protected:
    std::uint32_t eventId_ = 0;  // +0x04
};
