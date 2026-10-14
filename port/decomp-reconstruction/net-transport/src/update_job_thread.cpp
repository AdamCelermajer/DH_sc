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
// Verified: the original prints the "\n\n updateJob_thread::Finish() \n\n" debug trace and returns 1;
// the trace is dropped (logging only), the return matches.
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
// Partial: the listing is `pthread_create(&scratch_tid, nullptr, threadfun3, this)` and returns -1
// (after the "\n\n Not Created2222 \n\n" trace) when pthread_create fails. threadfun3 is the worker
// at 0x317f90 and calls SavegameManager::saveSettings; it is outside this category, so the port
// starts Run(), which is an empty stand-in for it. The -1 return for an already-running worker is a
// guard this port adds; the original has none.
int updateJob_thread::Start2() {
    if (thread_.joinable()) {
        return -1;
    }
    thread_ = std::thread([this] { Run(); });
    return 0;
}

// 0x317ef8 _ZN16updateJob_thread5StartEv
// Partial: the listing is pthread_create(threadfun2, this) and returns -1 after the
// "\n\n Not Created \n\n" trace when it fails. threadfun2 (the worker that calls Savegame::UpdateJobs)
// is outside this category, so the worker here is the same empty Run() stand-in as Start2. The
// already-running guard is an addition, as in Start2.
int updateJob_thread::Start() {
    if (thread_.joinable()) {
        return -1;
    }
    thread_ = std::thread([this] { Run(); });
    return 0;
}

// 0x317f58 _ZN16updateJob_thread4StopEv
// Verified: the original prints the "\n\n updateJob_thread::Stop() \n\n" debug trace and returns 1;
// the trace is dropped (logging only), the return matches.
int updateJob_thread::Stop() {
    return 1;
}
