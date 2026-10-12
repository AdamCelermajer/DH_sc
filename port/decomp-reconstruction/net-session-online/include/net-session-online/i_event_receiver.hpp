// IEventReceiver: interface for objects that receive session events. The original has
// no members and no methods besides the destructors.
#pragma once

class IEventReceiver {
public:
    virtual ~IEventReceiver() = default;  // 0x32fdbc D1 (the deleting destructor 0x330674 is skipped)
};
