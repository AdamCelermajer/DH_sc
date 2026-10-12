// Stand-in for CEventQueueBase and the CEventQueue<CConnection::EVENTS, 100> base of CConnection.
// Owned by another category. Only the calls the connection code makes are declared.
#pragma once

class CEventQueueBase {
public:
    virtual ~CEventQueueBase() = default;

    // Vtable +0x00 (CEventQueueBase::AddEvent). Stand-in: the event is not stored.
    virtual int AddEvent(int event, const void* data, int size) {
        (void)event;
        (void)data;
        (void)size;
        return 0;
    }

    // Vtable +0x0c (CEventQueueBase::Clear()). Stand-in: nothing is stored, nothing to clear.
    virtual void Clear() {}
};
