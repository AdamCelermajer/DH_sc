#include "net-transport/sync.hpp"

#include <cerrno>
#include <cstdio>

// 0x3106f4 _ZN6CMutex4LockEv
void CMutex::Lock() {
    ++lock_count_;
}

// 0x310704 _ZN6CMutex6UnlockEv
void CMutex::Unlock() {
    lock_count_ = lock_count_ < 2 ? 0 : lock_count_ - 1;
}

// 0x310720 _ZN6CMutex8IsLockedEv
// The original returns 1 only when the count is zero (1 - count, clamped to 0
// for count > 1); reproduced exactly even though the name suggests the inverse.
int CMutex::IsLocked() const {
    return lock_count_ == 0 ? 1 : 0;
}

// 0x80e2cc _ZN10CSemaphoreC2Ei
CSemaphore::CSemaphore(int initial) : count_(initial) {}

// 0x80e2e4 _ZN10CSemaphore4WaitEv
// Hex-Rays body is empty (bx lr); no blocking is performed. Kept partial: the
// no-op is the original behaviour, and real primitives need a design decision.
void CSemaphore::Wait() {}

// 0x80e2e8 _ZN10CSemaphore7ReleaseEi
// Hex-Rays body is empty; the count is not changed. Partial, as Wait.
void CSemaphore::Release(int) {}

// 0x80e33c _ZN14CReadWriteLock9LockWriteEv
// Hex-Rays body is empty: not a real lock. Partial, as CSemaphore::Wait.
void CReadWriteLock::LockWrite() {}

// 0x80e340 _ZN14CReadWriteLock11UnlockWriteEv
void CReadWriteLock::UnlockWrite() {}

// 0x80e344 _ZN14CReadWriteLock8LockReadEv
void CReadWriteLock::LockRead() {}

// 0x80e348 _ZN14CReadWriteLock10UnlockReadEv
void CReadWriteLock::UnlockRead() {}

// 0x80e398 _ZN9CNetMutexC1Ev
// 0x80e3d4 _ZN9CNetMutexC2Ev
CNetMutex::CNetMutex() = default;

// 0x80e370 _ZN9CNetMutexD1Ev
// 0x80e384 _ZN9CNetMutexD2Ev
CNetMutex::~CNetMutex() = default;

// 0x80e36c _ZN9CNetMutex4LockEv
void CNetMutex::Lock() {
    mutex_.lock();
}

// 0x80e368 _ZN9CNetMutex6UnlockEv
int CNetMutex::Unlock() {
    mutex_.unlock();
    return 0;
}

// 0x317e10 _ZN6ThreadD1Ev (out-of-line destructor body)
Thread::~Thread() {
    if (handle_.joinable()) {
        stop_ = true;
        handle_.join();
    }
}

// 0x84559c _ZN6Thread22IsCurrectThreadRunningEv
// Partial: the original probes the pthread with pthread_kill(tid, 0) and reports
// running unless the result is EINVAL (22) or ESRCH (3). std::thread has no probe;
// joinable() stands in for it.
bool Thread::IsCurrentThreadRunning() const {
    return handle_.joinable();
}

// 0x8455c0 _ZN6Thread3RunEv
// Partial: the original prints "PthredSelf" via XP_DEBUG_OUT first; that log is dropped.
// The loop itself matches the pseudocode: while (!stop) Step().
void Thread::Run() {
    while (!stop_.load()) {
        Step();
    }
}

// 0x855688 _ZN6Thread5StartEv
int Thread::Start() {
    if (Init() < 0) {
        return -1;
    }
    stop_ = false;
    try {
        handle_ = std::thread([this] { Run(); });
    } catch (...) {
        return -1;
    }
    return 0;
}

// 0x85560c _ZN6Thread4StopEv
int Thread::Stop() {
    if (stop_.load()) {
        return 0;
    }
    stop_ = true;
    if (IsCurrentThreadRunning()) {
        if (handle_.get_id() != std::this_thread::get_id()) {
            handle_.join();
        }
    }
    Cleanup();
    return 0;
}
