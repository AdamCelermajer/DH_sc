// vox::DriverAndroid: the Android output driver. Its two back ends are the audio thread
// (AT, mode 1 at +0x54) and the OpenSL-style output (OSL, mode 2). The JNI audio device
// the original drives is not ported; the functions that depend on it are declared and
// marked partial in driver_android.cpp. The original derives from DriverCallbackInterface,
// which is not ported yet, so this class derives from DriverInterface for now.
#pragma once

#include <thread>

#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/mutex.hpp"

struct _JavaVM;

namespace vox {

class DriverAndroid : public DriverInterface {
public:
    // 0x88fae4 (partial): the base constructor, mode 0 and the Init lock are in the original.
    DriverAndroid();
    // D1 is not ported as a separate body; the deleting variant is 0x88fa28.
    ~DriverAndroid() override = default;

    // 0x88ee78: dispatches Update to the audio thread (mode 1) or OSL (mode 2).
    void Update(float dt) override;
    // 0x88f028 (partial): dispatches Resume; _ResumeAT is not ported.
    void Resume() override;
    // 0x88f130 (partial): dispatches Suspend; _SuspendAT is not ported.
    void Suspend() override;
    // 0x88f644 (partial): dispatches Shutdown; _ShutdownAT joins the thread.
    void Shutdown();

    // 0x88f9a0 (partial): takes the lock; the rest of Init is not ported.
    void Init(void* context);
    // 0x88fbe8 (partial): takes the lock; the source is not created.
    void CreateDriverSource(void* a, void* b, int c);
    // 0x88f59c (partial): takes the lock; the source is not released.
    void DestroyDriverSource(DriverSourceInterface* source);

    // 0x88ee6c: the original prints nothing here.
    void PrintDebug();

    // OSL and audio-thread bodies. 0x88ee5c, 0x88ee60, 0x88ee64, 0x88ee68 and
    // 0x88ee70, 0x88ee74 are empty in the original.
    void _InitOSL(void* context);
    void _ShutdownOSL();
    void _SuspendOSL();
    void _ResumeOSL();
    void _UpdateOSL(float dt);
    void _UpdateAT(float dt);

    // 0x88f618: running flag cleared, then the thread is joined (pthread_join in the
    // original, std::thread::join here).
    void _ShutdownAT();
    // 0x88f044 and 0x88ef6c: not ported (they use the JNI audio device).
    void _SuspendAT();
    void _ResumeAT();

    // Static JNI and tuning values set by the VoxSet* exports.
    static _JavaVM* s_javaVM;             // _ZN3vox13DriverAndroid8s_javaVME
    static int s_androidAPILevel;         // _ZN3vox13DriverAndroid17s_androidAPILevelE
    static double m_dataThresholdRatio;   // _ZN3vox13DriverAndroid20m_dataThresholdRatioE
    static bool m_running;                // _ZN3vox13DriverAndroid9m_runningE

private:
    friend struct DriverAndroidTestAccess;
    Mutex mutex_;                  // +0x04
    bool at_active_ = false;       // +0x08: read by the audio-thread suspend and resume
    int mode_ = 0;                 // +0x54: 1 audio thread, 2 OSL
    std::thread thread_;           // +0x68 (pthread_t in the original)
};

}  // namespace vox

// Exported VoxSet* functions. The original names have no vox:: prefix.
void VoxSetJavaVM(_JavaVM* java_vm);           // 0x88ed8c
void VoxSetJavaVMC(_JavaVM* java_vm);          // 0x88ee1c: VoxSetJavaVM, then the threshold 1.0
void VoxSetDataThreshold(double ratio);        // 0x88edac: clamped to [0, 1]
void VoxSetAndroidAPILevel(int level);         // 0x88ee38
