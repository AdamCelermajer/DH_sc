// vox::DriverAndroid and the VoxSet* exports. The JNI and OpenSL paths are not ported:
// the functions that need them are marked partial in the comments.
#include "vox-audio/vox/driver_android.hpp"

namespace vox {

_JavaVM* DriverAndroid::s_javaVM = nullptr;
int DriverAndroid::s_androidAPILevel = 0;
double DriverAndroid::m_dataThresholdRatio = 0.0;
bool DriverAndroid::m_running = false;

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
void DriverAndroid::Init(void* context) {
    (void)context;
    mutex_.Lock();
    // The rest of Init is not ported; the original calls into the JNI environment here.
    mutex_.Unlock();
}

// 0x88fbe8 _ZN3vox13DriverAndroid18CreateDriverSourceEPvS1_i (partial)
DriverSourceInterface* DriverAndroid::CreateDriverSource(const std::int32_t* params, void* user, int flags) {
    (void)params;
    (void)user;
    (void)flags;
    mutex_.Lock();
    mutex_.Unlock();
    return nullptr;
}

// 0x88f59c _ZN3vox13DriverAndroid19DestroyDriverSourceEPNS_21DriverSourceInterfaceE (partial)
void DriverAndroid::DestroyDriverSource(DriverSourceInterface* source) {
    (void)source;
    mutex_.Lock();
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

// 0x88f044 _ZN3vox13DriverAndroid10_SuspendATEv (partial: not ported)
void DriverAndroid::_SuspendAT() {}

// 0x88ef6c _ZN3vox13DriverAndroid9_ResumeATEv (partial: not ported)
void DriverAndroid::_ResumeAT() {}

}  // namespace vox

// 0x88ed8c _Z12VoxSetJavaVMP7_JavaVM
void VoxSetJavaVM(_JavaVM* java_vm) {
    vox::DriverAndroid::s_javaVM = java_vm;
}

// 0x88ee1c VoxSetJavaVMC. The argument is passed straight through to VoxSetJavaVM in the
// original (the pseudocode loses the register), then the threshold is set to 1.0.
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
