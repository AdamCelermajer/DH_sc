// Thin lock wrapper (vox::Mutex over pthread_mutex_t in the original).
#pragma once

#include <mutex>

namespace vox {

class Mutex {
public:
    Mutex() = default;
    ~Mutex() = default;

    // The original copy constructor memberwise-copies the pthread object; a
    // mutex is not meaningfully copyable, so copying is removed in the port.
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    void Lock() { mutex_.lock(); }
    bool TryLock() { return mutex_.try_lock(); }
    void Unlock() { mutex_.unlock(); }

private:
    std::mutex mutex_;
};

}  // namespace vox
