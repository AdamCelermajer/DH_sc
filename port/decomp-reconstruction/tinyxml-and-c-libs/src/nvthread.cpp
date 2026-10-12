// NVThreadInit and NVThreadSleep, from recovered/native/hexrays-all (0x533bd8 and 0x533c60).
// NVThreadSleep's original splits the milliseconds into seconds and nanoseconds for a nanosleep-style call and
// returns its result. The port uses std::this_thread::sleep_for, which has the same effect and returns 0.
#include "tinyxml-and-c-libs/nvthread.hpp"

#include <chrono>
#include <thread>

namespace {

int s_vm = 0;

}  // namespace

int NVThreadInit(int vm) {
    s_vm = vm;
    return vm;
}

int NVThreadSleep(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    return 0;
}
