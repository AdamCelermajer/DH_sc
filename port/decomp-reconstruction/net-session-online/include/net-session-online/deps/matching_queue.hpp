// Stand-in for CMatching::m_eventQueueInternal, the matching-layer event queue that
// CMatchingGLLive posts to. CMatching is owned by the room layer (another category),
// so the queue is a function-local static here.
#pragma once

#include "net-session-online/c_event_queue_base.hpp"

inline CEventQueueBase& MatchingInternalEventQueue() {
    static CEventQueueBase queue;
    return queue;
}

// Stand-in for CMatching::m_eventQueue, the outgoing queue that CMatching::ProcessEvents posts to
// (codes 0x400000 and up). Same convention as MatchingInternalEventQueue.
inline CEventQueueBase& MatchingOutputEventQueue() {
    static CEventQueueBase queue;
    return queue;
}
