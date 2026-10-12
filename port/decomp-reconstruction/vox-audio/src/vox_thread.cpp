#include "vox-audio/vox/vox_thread.hpp"

#include <chrono>
#include <functional>
#include <thread>

namespace vox {

// 0x893604 _ZN3vox9VoxThread5SleepEj
void VoxThread::Sleep(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// 0x893600 _ZN3vox9VoxThread14GetCurThreadIdEv
// pthread_t has no portable meaning here; the thread id is hashed to a word.
std::uintptr_t VoxThread::GetCurThreadId() {
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

}  // namespace vox
