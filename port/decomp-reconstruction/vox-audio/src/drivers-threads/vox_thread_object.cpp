// vox::VoxThread object: constructors, destructor, Stop and the update loop.
#include <chrono>
#include <climits>
#include <cstring>

#include <thread>

#if defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#include <sched.h>
#define VOX_THREAD_HAS_PTHREAD_PRIORITY 1
#endif

#include "vox-audio/vox/mutex.hpp"
#include "vox-audio/vox/vox_thread.hpp"

namespace vox {

// Thread state kept on the heap: the mutex, the two flags, the previous pass time, the
// name and the std::thread.
struct VoxThread::State {
    Mutex mutex;                      // +0x0c
    bool running = true;              // +0x10
    bool looping = true;              // +0x11
    double last_time = 0.0;           // +0x18, seconds
    char name[64] = {};               // +0x20
    std::thread thread;               // +0x60
};

namespace {

// Seconds from a steady clock. The original reads its own clock (_GetTime); only the
// differences between two readings are used, so the epoch does not matter.
double NowSeconds() {
    using Clock = std::chrono::steady_clock;
    return std::chrono::duration<double>(Clock::now().time_since_epoch()).count();
}

// ARM float-to-int conversion saturates; this keeps the same result for large values.
int SaturateToInt(double value) {
    if (value >= static_cast<double>(INT_MAX)) {
        return INT_MAX;
    }
    if (value <= static_cast<double>(INT_MIN)) {
        return INT_MIN;
    }
    return static_cast<int>(value);
}

}  // namespace

// 0x893784 _ZN3vox9VoxThreadC1EPFvPvS1_ES1_S1_Pc (partial: the priority step is not run on Windows)
// 0x8938b4 _ZN3vox9VoxThreadC2EPFvPvS1_ES1_S1_Pc (same body)
// The priority step: the new thread's policy and its minimum and maximum priority are read;
// a maximum above 30 becomes max(min, 31) and the thread is given that priority. The step
// is ported on POSIX builds. Missing on Windows (one item): pthread_setschedparam has no
// std::thread or Win32 equivalent here, so the priority is left at the default.
VoxThread::VoxThread(Callback callback, void* first, void* second, const char* name)
    : callback_(callback), first_(first), second_(second), state_(new State()) {
    if (callback_ == nullptr) {
        // The original stores the low byte of the callback as the loop flag: 0.
        state_->looping = false;
        return;
    }
    if (name != nullptr) {
        std::strncpy(state_->name, name, 63);
        state_->name[63] = '\0';
    } else {
        std::strcpy(state_->name, "VoxThread");
    }
    state_->thread = std::thread([this] { Update(nullptr); });
#if defined(VOX_THREAD_HAS_PTHREAD_PRIORITY)
    pthread_t handle = state_->thread.native_handle();
    int policy = 0;
    sched_param param{};
    pthread_getschedparam(handle, &policy, &param);
    const int priority_min = sched_get_priority_min(policy);
    int priority_max = sched_get_priority_max(policy);
    if (priority_max > 30) {
        priority_max = priority_min < 31 ? 31 : priority_min;
    }
    sched_param set{};
    set.sched_priority = priority_max;
    pthread_setschedparam(handle, policy, &set);
#endif
}

// D1 0x89374c _ZN3vox9VoxThreadD1Ev and D2 0x893768 _ZN3vox9VoxThreadD2Ev
VoxThread::~VoxThread() {
    Stop();
    delete state_;
    state_ = nullptr;
}

// 0x893714 _ZN3vox9VoxThread4StopEv
void VoxThread::Stop() {
    state_->mutex.Lock();
    state_->running = false;
    state_->looping = false;
    state_->mutex.Unlock();
    if (state_->thread.joinable()) {
        state_->thread.join();
    }
}

// 0x893700 _ZN3vox9VoxThread6UpdateEPv
int VoxThread::Update(void* unused) {
    (void)unused;
    _Update();
    return 0;
}

// 0x893610 _ZN3vox9VoxThread7_UpdateEv
// Each pass runs the callback (when running), measures the time it took, and sleeps so
// that passes start about 66 ms apart. The sleep is clamped to between 1 and 33 ms.
void VoxThread::_Update() {
    state_->mutex.Lock();
    bool looping = state_->looping;
    bool running = state_->running;
    state_->mutex.Unlock();
    if (!looping) {
        return;
    }
    do {
        const double start = NowSeconds();
        if (running && callback_ != nullptr) {
            callback_(first_, second_);
        }
        const int work_ms = SaturateToInt((NowSeconds() - start) * 1000.0);
        const int since_last_ms = SaturateToInt((start - state_->last_time) * 1000.0);
        int delay_ms = 66 - since_last_ms - work_ms;
        state_->last_time = start;
        if (delay_ms > 0) {
            if (delay_ms >= 33) {
                delay_ms = 33;
            }
        } else {
            delay_ms = 1;
        }
        Sleep(static_cast<unsigned int>(delay_ms));
        state_->mutex.Lock();
        looping = state_->looping;
        running = state_->running;
        state_->mutex.Unlock();
    } while (looping);
}

}  // namespace vox

// 0x893710 _ZN3vox10funcUpdateEPv: a thunk to VoxThread::Update in the original. The port
// starts the thread on a lambda that calls Update, so the thunk is not reimplemented.
