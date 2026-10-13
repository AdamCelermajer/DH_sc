// vox::DriverAndroid and the VoxSet* exports. The JNI and OpenSL paths are not ported:
// the functions that need them are marked partial in the comments.
#include <chrono>

#include "vox-audio/vox/driver_android.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

_JavaVM* DriverAndroid::s_javaVM = nullptr;
int DriverAndroid::s_androidAPILevel = 0;
double DriverAndroid::m_dataThresholdRatio = 0.0;
bool DriverAndroid::m_running = false;
double DriverAndroid::m_updateTime = 0.0;
double DriverAndroid::m_updateStartTime = 0.0;
double DriverAndroid::m_dataThreshold = 0.0;
double DriverAndroid::m_dataDuration = 0.0;

namespace {

// Seconds from a steady clock, the same stand-in as vox_thread_object.cpp for _GetTime
// (core-platform 0x862bf4). The driver only uses differences of two readings, so the epoch does
// not matter, and vox-audio does not link core-platform.
double DriverAndroidNowSeconds() {
    using Clock = std::chrono::steady_clock;
    return std::chrono::duration<double>(Clock::now().time_since_epoch()).count();
}

}  // namespace

// 0x88fae4 (partial): the original calls the DriverCallbackInterface constructor first.
DriverAndroid::DriverAndroid() {
    mode_ = 0;
    Init(nullptr);
}

// 0x88ee78 _ZN3vox13DriverAndroid6UpdateEf
void DriverAndroid::Update(float dt) {
    if (mode_ == 1) {
        _UpdateAT(dt);
        return;
    }
    if (mode_ != 2) {
        return;
    }
    _UpdateOSL(dt);
}

// 0x88f028 _ZN3vox13DriverAndroid6ResumeEv (partial: _ResumeAT not ported)
void DriverAndroid::Resume() {
    if (mode_ == 1) {
        _ResumeAT();
        return;
    }
    if (mode_ != 2) {
        return;
    }
    _ResumeOSL();
}

// 0x88f130 _ZN3vox13DriverAndroid7SuspendEv (partial: _SuspendAT not ported)
void DriverAndroid::Suspend() {
    if (mode_ == 1) {
        _SuspendAT();
        return;
    }
    if (mode_ != 2) {
        return;
    }
    _SuspendOSL();
}

// 0x88f644 _ZN3vox13DriverAndroid8ShutdownEv: mode 1 runs the audio-thread shutdown, mode 2
// the OSL shutdown, anything else does nothing.
void DriverAndroid::Shutdown() {
    if (mode_ == 1) {
        _ShutdownAT();
        return;
    }
    if (mode_ != 2) {
        return;
    }
    _ShutdownOSL();
}

// 0x88f9a0 _ZN3vox13DriverAndroid4InitEPv (partial)
// The original calls DriverCallbackInterface::Init and SetDefaultParameter before and after
// _InitAT. That base is not inherited by the port, so those two pairs of calls are missing.
void DriverAndroid::Init(void* context) {
    mutex_.Lock();
    _InitAT(context);
    mutex_.Unlock();
}

// 0x88fbe8 _ZN3vox13DriverAndroid18CreateDriverSourceEPvS1_i (partial)
// The original allocates a DriverAndroidSource (VoxAlloc 0xb4) when at_active_ is set, builds it
// with the base source counter (+0x0c) as its flags, and links a node into the base source list
// (+0x10). The counter and the list belong to DriverCallbackInterface, which the port does not
// inherit, so no source is created and this returns null.
DriverSourceInterface* DriverAndroid::CreateDriverSource(const std::int32_t* params, void* user, int flags) {
    (void)params;
    (void)user;
    (void)flags;
    mutex_.Lock();
    mutex_.Unlock();
    return nullptr;
}

// 0x88f59c _ZN3vox13DriverAndroid19DestroyDriverSourceEPNS_21DriverSourceInterfaceE (partial)
// The original unlinks the node that holds the source from the base source list (+0x10), which is
// not reachable here, then runs the source's virtual destructor and frees the block. The
// destructor and VoxFree run whether or not the node was found, as in the original.
void DriverAndroid::DestroyDriverSource(DriverSourceInterface* source) {
    mutex_.Lock();
    if (source != nullptr) {
        source->~DriverSourceInterface();
        VoxFree(source);
    }
    mutex_.Unlock();
}

// 0x88ee6c _ZN3vox13DriverAndroid10PrintDebugEv
void DriverAndroid::PrintDebug() {}

// 0x88ee5c _ZN3vox13DriverAndroid8_InitOSLEPv
void DriverAndroid::_InitOSL(void* context) {
    (void)context;
}

// 0x88ee60 _ZN3vox13DriverAndroid12_ShutdownOSLEv
void DriverAndroid::_ShutdownOSL() {}

// 0x88ee64 _ZN3vox13DriverAndroid11_SuspendOSLEv
void DriverAndroid::_SuspendOSL() {}

// 0x88ee68 _ZN3vox13DriverAndroid10_ResumeOSLEv
void DriverAndroid::_ResumeOSL() {}

// 0x88ee74 _ZN3vox13DriverAndroid10_UpdateOSLEf
void DriverAndroid::_UpdateOSL(float dt) {
    (void)dt;
}

// 0x88ee70 _ZN3vox13DriverAndroid9_UpdateATEf
void DriverAndroid::_UpdateAT(float dt) {
    (void)dt;
}

// 0x88f618 _ZN3vox13DriverAndroid11_ShutdownATEv (std::thread in place of pthread_join)
void DriverAndroid::_ShutdownAT() {
    m_running = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

// 0x88f044 _ZN3vox13DriverAndroid10_SuspendATEv (partial)
// The original pauses the AudioTrack through JNI (GetEnv with version 0x10002, then
// CallNonvirtualVoidMethod with mPause). That call is not ported. The flag and the duration
// update run as in the original: the buffered duration loses the time played since the last
// start.
void DriverAndroid::_SuspendAT() {
    mutex_.Lock();
    if (at_active_) {
        paused_ = true;
        m_dataDuration = m_dataDuration - (DriverAndroidNowSeconds() - m_updateStartTime);
    }
    mutex_.Unlock();
}

// 0x88ef6c _ZN3vox13DriverAndroid9_ResumeATEv (partial)
// The original starts the AudioTrack through JNI (GetEnv, then CallNonvirtualVoidMethod with
// mPlay). That call is not ported. The flag is cleared and the start time is reset as in the
// original.
void DriverAndroid::_ResumeAT() {
    mutex_.Lock();
    if (at_active_) {
        paused_ = false;
        m_updateStartTime = DriverAndroidNowSeconds();
    }
    mutex_.Unlock();
}

}  // namespace vox

// 0x88ed8c _Z12VoxSetJavaVMP7_JavaVM
void VoxSetJavaVM(_JavaVM* java_vm) {
    vox::DriverAndroid::s_javaVM = java_vm;
}

// 0x88ee1c VoxSetJavaVMC. The asm passes the argument straight through to VoxSetJavaVM
// (R0 is not changed), then tail-branches to VoxSetDataThreshold with 1.0 in R0:R1
// (0x3ff00000 high word, zero low word).
void VoxSetJavaVMC(_JavaVM* java_vm) {
    VoxSetJavaVM(java_vm);
    VoxSetDataThreshold(1.0);
}

// 0x88edac VoxSetDataThreshold. ARM compares the double with 0 and 1: below 0 gives 0,
// above 1 gives 1, otherwise the value is kept.
void VoxSetDataThreshold(double ratio) {
    if (ratio < 0.0) {
        ratio = 0.0;
    } else if (ratio > 1.0) {
        ratio = 1.0;
    }
    vox::DriverAndroid::m_dataThresholdRatio = ratio;
}

// 0x88ee38 _Z21VoxSetAndroidAPILeveli
void VoxSetAndroidAPILevel(int level) {
    vox::DriverAndroid::s_androidAPILevel = level;
}
