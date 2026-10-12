// Stand-in for CConnectionManager (owned by another category). The event queue that the
// connection code writes to sits at +0x08 of the original object.
#pragma once

#include "net-transport/deps/c_event_queue_base.hpp"

class CConnectionManager {
public:
    // Transport type for a packet type (tTRANSPORT_TYPE). Stand-in: always transport 0.
    int GetPacketTransportType(int packet_type) const {
        (void)packet_type;
        return 0;
    }

    CEventQueueBase& EventQueue() { return events_; }

private:
    CEventQueueBase events_;  // +0x08
};

// _Z16GetConnectionMgrv. Stand-in: a static default manager.
inline CConnectionManager* GetConnectionMgr() {
    static CConnectionManager instance;
    return &instance;
}
