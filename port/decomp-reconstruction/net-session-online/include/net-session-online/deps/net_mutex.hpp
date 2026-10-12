// Stand-in for CNetMutex (net-transport category): a non-recursive lock. The
// original wraps pthread_mutex_t; Lock/Unlock/destroy map 1:1 onto it.
#pragma once

#include <mutex>

class CNetMutex {
public:
    void Lock() { mutex_.lock(); }
    void Unlock() { mutex_.unlock(); }

private:
    std::mutex mutex_;
};
