// CEventQueueBase: mutex-guarded FIFO of CEvent values (a std::list in the
// original STL private layout). Template instances CEventQueue<T, N> derive from
// it; their Clear() is a branch to CEventQueueBase::Clear.
// Offsets are the original 32-bit layout: +0x00 vptr, +0x04 mutex, +0x08 list,
// +0x10 expiry-check flag (see NOTES.md for the unnamed +0x0c word).
#pragma once

#include <cstdint>
#include <list>

#include "net-session-online/c_event.hpp"
#include "net-session-online/deps/net_mutex.hpp"

class CEventQueueBase {
public:
    CEventQueueBase() = default;
    virtual ~CEventQueueBase();                                   // D1 / D2 (D0 skipped)

    void AddEvent(int type, void* data, int size);                // 8AddEventEiPvi
    bool HasEvents() const;                                       // 9HasEventsEv (no lock)
    int GetEventData(int type, void* data, int size);             // 12GetEventDataEiPvi
    void Clear(int type);                                         // 5ClearEi
    void Clear();                                                 // 5ClearEv
    bool FindEvent(int type, bool consume);                       // 9FindEventEib
    bool ConsumeEvent(int type) { return FindEvent(type, true); }       // 12ConsumeEventEi
    bool HasEventOccured(int type, bool consume) {                       // 15HasEventOccuredEib
        return FindEvent(type, consume);
    }

private:
    CNetMutex mutex_;                  // +0x04
    std::list<CEvent> events_;         // +0x08
    std::uint32_t expiryCheck_ = 0;    // +0x10 passed as the enable flag to CEvent::IsExpired
};
