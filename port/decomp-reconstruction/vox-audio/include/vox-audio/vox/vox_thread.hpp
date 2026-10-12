// vox::VoxThread: the thread wrapper. Only the two static helpers are reconstructed;
// the thread object (mutex, callback, state) is not ported yet.
#pragma once

#include <cstdint>

namespace vox {

class VoxThread {
public:
    static void Sleep(unsigned int milliseconds);  // 0x893604: usleep(ms * 1000) in the original
    static std::uintptr_t GetCurThreadId();        // 0x893600: pthread_self() in the original
};

}  // namespace vox
