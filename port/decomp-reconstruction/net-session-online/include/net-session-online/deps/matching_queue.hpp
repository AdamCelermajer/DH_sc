// Stand-in for CMatching::m_eventQueueInternal, the matching-layer event queue that
// CMatchingGLLive posts to. CMatching is owned by the room layer (another category),
// so the queue is a function-local static here.
#pragma once

#include "net-session-online/c_event_queue_base.hpp"

inline CEventQueueBase& MatchingInternalEventQueue() {
    static CEventQueueBase queue;
    return queue;
}
