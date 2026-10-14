// vox::DriverAndroid and vox::DriverAndroidSource: the destructor, the JNI-bound audio
// thread helpers (not ported; their status is partial) and the callback source
// constructor.
#include <chrono>
#include <thread>

#include "vox-audio/vox/driver_android.hpp"

namespace vox {

namespace {

// Same stand-in as driver_android.cpp for _GetTime: a steady clock, differences only.
double DriverAndroidNowSeconds() {
    using Clock = std::chrono::steady_clock;
    return std::chrono::duration<double>(Clock::now().time_since_epoch()).count();
}

}  // namespace

// 0x88fbac _ZN3vox19DriverAndroidSourceC1EPvS1_j and 0x88fc8c (C2): the callback source
// base is built first, then the vtable is set and Init builds the ring.
DriverAndroidSource::DriverAndroidSource(const std::int32_t* format,
                                         const std::int32_t* buffer_count,
                                         std::uint32_t flags)
    : DriverCallbackSourceInterface(format, buffer_count, flags) {
    (void)Init();  // the constructor does not use the Init result (0x88fbac)
}

// 0x88ee58 _ZN3vox19DriverAndroidSource10PrintDebugEv
void DriverAndroidSource::PrintDebug() {}

// D1 0x88f9ec and D2 0x88fa44, same body; D0 0x88fa28 is the deleting form.
// Verified against the listing (0x88f9ec..0x88fa1c and 0x88fa44..0x88fa74): the vtable is
// stored, then Shutdown() (0x88f644), then DriverCallbackInterface::~DriverCallbackInterface
// (0x891120). 0x88fa28 calls this destructor and then LC_API_FREE_0 on the object. The port runs
// Shutdown, and its callback_ member is destroyed automatically after the body, which is that
// base destructor call. Deviation: the base is composed as a member, so the base destructor runs
// as part of the member's destruction instead of as an explicit base call. D0 (0x88fa28) is the
// compiler's deleting form, which releases the block with operator delete where the listing calls
// C free; that is the project's convention for deleting destructors.
DriverAndroid::~DriverAndroid() {
    Shutdown();
}

// 0x88f328 _ZN3vox13DriverAndroid16UpdateThreadedATEPv (partial)
// Verified against the listing (0x88f328..0x88f574). A null this returns 0 without locking.
// Otherwise: lock, AttachCurrentThread (the JavaVM vtable slot at +0x10, called with 0), and when
// an env comes back: PushLocalFrame(2); NewObject(cAudioTrack, mAudioTrack, 3, 44100, 12, 2,
// 4 * frames at +0x58, 1) into the track word +0x64; CallNonvirtualVoidMethod(track, cAudioTrack,
// mPlay); NewByteArray(4 * frames); then the active byte +0x08 = 1, unlock,
// m_updateStartTime = _GetTime(), and the loop: while m_running, while the byte at +0x60 is set
// usleep(m_updateTime * 1e6), otherwise DoCallbackAT(&array); at the end the active byte takes
// m_running, the lock is retaken, CallNonvirtualVoidMethod(mStop) and (mRelease) run,
// PopLocalFrame(env, 0) and DetachCurrentThread (JavaVM slot +0x14) follow. Every failure path
// only detaches. Missing item: the whole JNI block - AttachCurrentThread, PushLocalFrame,
// NewObject, NewByteArray, CallNonvirtualVoidMethod (mPlay/mStop/mRelease), PopLocalFrame and
// DetachCurrentThread - because the host has no JNIEnv and vox-audio has no AudioTrack class
// handle. The port never starts this thread either, since _InitAT's JavaVM branch is not ported.
int DriverAndroid::UpdateThreadedAT(void* unused) {
    (void)unused;
    mutex_.Lock();
    mutex_.Unlock();
    return 0;
}

// 0x88f14c _ZN3vox13DriverAndroid12DoCallbackATERP7_jarray (partial)
// Verified against the listing (0x88f14c..0x88f2dc): GetEnv (JavaVM slot +0x18, version 0x10002);
// GetPrimitiveArrayCritical(env, array, 0) into a buffer; when it is non-null, Lock,
// DriverCallbackInterface::_FillBuffer(this, buffer, frames at +0x5C), Unlock,
// ReleasePrimitiveArrayCritical(env, array, buffer, 0), then CallNonvirtualIntMethod(track,
// cAudioTrack, mWrite, array, 0, 4 * frames) and m_dataDuration += m_updateTime. The timing tail
// then runs on both paths: the first call stores m_updateStartTime = _GetTime(), and the function
// returns usleep(m_updateTime * 1e6) when m_dataDuration - (now - m_updateStartTime) exceeds
// m_dataThreshold. The port reproduces the tail exactly (with std::this_thread::sleep_for for
// usleep, and the first-call flag as a function-local static). Missing item: the JNI block -
// GetEnv, GetPrimitiveArrayCritical, the base _FillBuffer call, ReleasePrimitiveArrayCritical and
// CallNonvirtualIntMethod with mWrite - plus m_dataDuration += m_updateTime. Without a JNIEnv
// there is no array to fill, and DriverCallbackInterface::_FillBuffer is not reachable from the
// composed callback_ member, so m_dataDuration does not grow. The listing's usleep result is not
// returned here because the only caller (UpdateThreadedAT) ignores it.
void DriverAndroid::DoCallbackAT(_jarray*& array) {
    (void)array;
    static bool is_first = true;  // DoCallbackAT(_jarray*&)::isFirst: set by the first call only
    if (is_first) {
        is_first = false;
        m_updateStartTime = DriverAndroidNowSeconds();
    }
    const double remaining = m_dataDuration - (DriverAndroidNowSeconds() - m_updateStartTime);
    if (remaining > m_dataThreshold) {
        std::this_thread::sleep_for(
            std::chrono::microseconds(static_cast<unsigned int>(m_updateTime * 1000000.0)));
    }
}

// 0x88f660 _ZN3vox13DriverAndroid7_InitATEPv (partial)
// Verified against the listing (0x88f660..0x88f7C4): the first call is SetDriverSampleRate with
// the immediate 44100 (MOVW R0,#0xAC44 at 0x88f664) and the argument is never read; the function
// then returns whatever r0 holds from that void call, so the no-JavaVM path returns 44100. With a
// JavaVM: GetEnv (slot +0x18, 0x10002); FindClass("android/media/AudioTrack") into cAudioTrack,
// NewGlobalRef of it, GetMethodID("<init>", "(IIIIII)V") into mAudioTrack, GetStaticMethodID
// ("getMinBufferSize", "(III)I") into mGetMinBufferSize, GetMethodID for "play", "pause", "stop",
// "release" ("()V") and "write" ("([BII)I"); then CallStaticIntMethod(mGetMinBufferSize, 44100,
// 12, 2) / 4 into the ring frames word (+0x58), the chunk word (+0x5C) = min(frames, 1024),
// m_updateTime = chunk / 44100.0, m_dataThreshold = -(frames / 44100.0 * m_dataThresholdRatio),
// m_dataDuration = 0, mode (+0x54) = 1, m_running = 1, the byte at +0x60 = 0 and pthread_create
// with the thread handle at +0x68 and UpdateThreadedAT. Missing item: that whole JavaVM branch -
// no JNIEnv, no java.lang.Class handle and no AudioTrack method IDs exist on the host - so the
// port returns 0 there instead of the pthread_create result.
int DriverAndroid::_InitAT(void* context) {
    (void)context;
    constexpr int kDriverSampleRate = 44100;
    DriverCallbackSourceInterface::SetDriverSampleRate(kDriverSampleRate);
    if (s_javaVM == nullptr) {
        return kDriverSampleRate;
    }
    return 0;
}

}  // namespace vox
