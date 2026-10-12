#include "vox-audio/vox/access_controller.hpp"

#include <chrono>
#include <thread>

namespace vox {

namespace {
// The asm calls 0x30e880 with 1000 before retrying, i.e. a 1000 ms wait.
void WaitBeforeRetry() { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); }
}  // namespace

// 0x893548 _ZN3vox16AccessController13GetReadAccessEv
// Waits while a writer holds the flag, then adds a reader.
void AccessController::GetReadAccess() {
    mutex_.Lock();
    while (write_flag_ != 0) {
        mutex_.Unlock();
        WaitBeforeRetry();
        mutex_.Lock();
    }
    ++read_count_;
    mutex_.Unlock();
}

// 0x89351c _ZN3vox16AccessController17ReleaseReadAccessEv
// The binary tail-calls Mutex::Unlock, so the int result is returned.
int AccessController::ReleaseReadAccess() {
    mutex_.Lock();
    --read_count_;
    return mutex_.Unlock();
}

// 0x8934ac _ZN3vox16AccessController14GetWriteAccessEv
// Waits while there is a writer or any reader, then takes the write flag.
void AccessController::GetWriteAccess() {
    mutex_.Lock();
    while (write_flag_ != 0 || read_count_ != 0) {
        mutex_.Unlock();
        WaitBeforeRetry();
        mutex_.Lock();
    }
    write_flag_ = 1;
    mutex_.Unlock();
}

// 0x893480 _ZN3vox16AccessController18ReleaseWriteAccessEv
int AccessController::ReleaseWriteAccess() {
    mutex_.Lock();
    --write_flag_;
    return mutex_.Unlock();
}

}  // namespace vox
