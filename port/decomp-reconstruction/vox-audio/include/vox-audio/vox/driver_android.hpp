// vox::DriverAndroid: the Android output driver. Its two back ends are the audio thread
// (AT, mode 1 at +0x54) and the OpenSL-style output (OSL, mode 2). The JNI audio device
// the original drives is not ported; the functions that depend on it are declared and
// marked partial in driver_android.cpp. The original derives from DriverCallbackInterface,
// which is not ported yet, so this class derives from DriverInterface for now.
#pragma once

#include <thread>

#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/driver_callback.hpp"
#include "vox-audio/vox/mutex.hpp"

struct _JavaVM;
struct _jarray;

namespace vox {

class DriverAndroid : public DriverInterface {
public:
    // C1 0x88fa80, C2 0x88fae4: the base constructor (the composed callback_ member, built before
    // the body), mode 0 and then Init(nullptr).
    DriverAndroid();
    // D1 0x88f9ec and D2 0x88fa44, same body: Shutdown then the base destructor (the composed
    // callback_ member, destroyed after the body). D0 0x88fa28 is the deleting form.
    ~DriverAndroid() override;

    // 0x88ee78: dispatches Update to the audio thread (mode 1) or OSL (mode 2).
    void Update(float dt) override;
    // 0x88f028: dispatches Resume by the mode word; the body matches the listing.
    void Resume() override;
    // 0x88f130: dispatches Suspend by the mode word; the body matches the listing.
    void Suspend() override;
    // The binary's DriverAndroid derives from DriverCallbackInterface, which is where these four
    // slots are defined (+0x24 Set3DParameter, +0x40 PrintDebug, +0x4c FillBuffer) or forwarded
    // from (+0x10 Suspend, +0x14 Resume, which driver_callback.cpp does not override). The port's
    // DriverAndroid derives from DriverInterface directly, so it forwards to a
    // DriverCallbackInterface member to keep the same slots working. Not virtual in the original.
    void Set3DParameter(int which, const void* value) override;
    void FillBuffer(std::int16_t* out, int count) override;
    // 0x88f644 (partial): dispatches Shutdown; _ShutdownAT joins the thread.
    void Shutdown();

    // 0x88f9a0: the lock, DriverCallbackInterface::Init, SetDefaultParameter, _InitAT,
    // SetDefaultParameter and the unlock, all reached through the composed callback_ member.
    void Init(void* context);
    // 0x88fbe8 (partial): takes the lock; no source is created because the base counter
    // (DriverCallbackInterface::field_0c_ +0x0c) and the base source list (list_head_ +0x10) are
    // not exposed by the composed callback_ member, so this returns null.
    DriverSourceInterface* CreateDriverSource(const std::int32_t* params, void* user, int flags) override;
    // 0x88f59c (partial): destroys and frees the source under the lock; the unlink from the base
    // list (list_head_ +0x10, DriverCallbackNode) is not reachable.
    void DestroyDriverSource(DriverSourceInterface* source) override;

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

    // 0x88f328 (partial): the audio thread entry; the AudioTrack calls are JNI and not ported.
    int UpdateThreadedAT(void* unused);
    // 0x88f14c (partial): the timing tail is ported; the JNI write pass is not, so m_dataDuration
    // does not grow here.
    void DoCallbackAT(_jarray*& array);
    // 0x88f660 (partial): stores the sample rate 44100 (the original never reads its argument) and
    // returns 44100 when s_javaVM is null; the AudioTrack setup under a JavaVM is JNI and not ported.
    int _InitAT(void* context);

    // 0x88f618: running flag cleared, then the thread is joined (pthread_join in the
    // original, std::thread::join here).
    void _ShutdownAT();
    // 0x88f044 and 0x88ef6c: the flag and timing part matches the listing; the AudioTrack
    // pause()/play() JNI calls are dropped (no host JavaVM, JNIEnv or AudioTrack jobject).
    void _SuspendAT();
    void _ResumeAT();

    // Static JNI and tuning values set by the VoxSet* exports.
    static _JavaVM* s_javaVM;             // _ZN3vox13DriverAndroid8s_javaVME
    static int s_androidAPILevel;         // _ZN3vox13DriverAndroid17s_androidAPILevelE
    static double m_dataThresholdRatio;   // _ZN3vox13DriverAndroid20m_dataThresholdRatioE
    static bool m_running;                // _ZN3vox13DriverAndroid9m_runningE

    // Audio-thread timing in seconds (differences only are used). m_updateTime is one chunk
    // (chunk frames / 44100), m_dataThreshold is negative, m_dataDuration is the buffered audio.
    static double m_updateTime;           // _ZN3vox13DriverAndroid12m_updateTimeE
    static double m_updateStartTime;      // _ZN3vox13DriverAndroid17m_updateStartTimeE
    static double m_dataThreshold;        // _ZN3vox13DriverAndroid15m_dataThresholdE
    static double m_dataDuration;         // _ZN3vox13DriverAndroid14m_dataDurationE

private:
    friend struct DriverAndroidTestAccess;
    // The base mix and listener object. The binary's DriverAndroid derives from
    // DriverCallbackInterface; the port holds it as a member, so the slots the engine calls
    // through the driver's vtable (+0x10 Suspend, +0x14 Resume, +0x24 Set3DParameter,
    // +0x40 PrintDebug, +0x4c FillBuffer) are forwarded to it.
    DriverCallbackInterface callback_;
    Mutex mutex_;                  // +0x04
    bool at_active_ = false;       // +0x08: read by the audio-thread suspend and resume
    int mode_ = 0;                 // +0x54: 1 audio thread, 2 OSL
    bool paused_ = false;          // +0x60: set by _SuspendAT, cleared by _ResumeAT and _InitAT
    std::thread thread_;           // +0x68 (pthread_t in the original)
};

}  // namespace vox

// Exported VoxSet* functions. The original names have no vox:: prefix.
void VoxSetJavaVM(_JavaVM* java_vm);           // 0x88ed8c
void VoxSetJavaVMC(_JavaVM* java_vm);          // 0x88ee1c: VoxSetJavaVM, then the threshold 1.0
void VoxSetDataThreshold(double ratio);        // 0x88edac: clamped to [0, 1]
void VoxSetAndroidAPILevel(int level);         // 0x88ee38

namespace vox {

// vox::DriverAndroidSource: the callback source used by the Android driver.
class DriverAndroidSource : public DriverCallbackSourceInterface {
public:
    // 0x88fbac (C1) and 0x88fc8c (C2): the base constructor, the vtable, then Init.
    DriverAndroidSource(const std::int32_t* format, const std::int32_t* buffer_count,
                        std::uint32_t flags);
    // D1 0x88fb28 and D2 0x88fb78 (identical bodies); D0 0x88fb5c is the deleting form.
    ~DriverAndroidSource() override = default;

    // 0x88ee58: empty in the original.
    void PrintDebug();
};

}  // namespace vox
