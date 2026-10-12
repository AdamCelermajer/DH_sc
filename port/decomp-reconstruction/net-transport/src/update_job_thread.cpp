#include "net-transport/update_job_thread.hpp"

// 0x317e44 _ZN16updateJob_threadC1Ei
// 0x317e14 _ZN16updateJob_threadC2Ei
updateJob_thread::updateJob_thread(int argument) : flag_(true), argument_(argument) {}

// 0x317e78 _ZN16updateJob_threadD1Ev
// 0x317e74 _ZN16updateJob_threadD2Ev
// Deviation: a worker still running is joined here. The original never joins.
updateJob_thread::~updateJob_thread() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

// 0x317e88 _ZN16updateJob_thread7PrepareEv
int updateJob_thread::Prepare() {
    return 1;
}

// 0x317e80 _ZN16updateJob_thread6KernelEv
int updateJob_thread::Kernel() {
    return 1;
}

// 0x317f74 _ZN16updateJob_thread6FinishEv
// Partial: the original's debug trace is dropped.
int updateJob_thread::Finish() {
    return 1;
}

// 0x317e7c _ZN16updateJob_thread3RunEv
void updateJob_thread::Run() {}

// 0x317e90 _ZN16updateJob_thread22IsCurrectThreadRunningEv
bool updateJob_thread::IsCurrectThreadRunning() const {
    return true;
}

// 0x317e98 _ZN16updateJob_thread6Start2Ev
// Partial: the worker is updateJob_thread::Run (threadfun3 is not reconstructed). Returns -1
// when a worker is already running, where the original would overwrite its handle.
int updateJob_thread::Start2() {
    if (thread_.joinable()) {
        return -1;
    }
    thread_ = std::thread([this] { Run(); });
    return 0;
}

// 0x317ef8 _ZN16updateJob_thread5StartEv
// Partial: same worker as Start2 (threadfun2 is not reconstructed).
int updateJob_thread::Start() {
    if (thread_.joinable()) {
        return -1;
    }
    thread_ = std::thread([this] { Run(); });
    return 0;
}

// 0x317f58 _ZN16updateJob_thread4StopEv
// Partial: the original's debug trace is dropped.
int updateJob_thread::Stop() {
    return 1;
}
