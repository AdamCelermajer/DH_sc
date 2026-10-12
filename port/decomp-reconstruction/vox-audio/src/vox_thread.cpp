#if defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#endif

#include "vox-audio/vox/vox_thread.hpp"

#include <chrono>
#include <functional>
#include <thread>

namespace vox {

// 0x893604 _ZN3vox9VoxThread5SleepEj
void VoxThread::Sleep(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// 0x893600 _ZN3vox9VoxThread14GetCurThreadIdEv (partial on Windows)
// The original returns pthread_self(). POSIX builds return the same word. Windows has no
// pthread_t, so there the thread id is hashed to a word (the one missing item).
std::uintptr_t VoxThread::GetCurThreadId() {
#if defined(__unix__) || defined(__APPLE__)
    return static_cast<std::uintptr_t>(pthread_self());
#else
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
#endif
}

}  // namespace vox
