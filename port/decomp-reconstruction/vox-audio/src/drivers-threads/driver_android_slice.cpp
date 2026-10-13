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

// D1 0x88f9ec and D2 0x88fa44 (partial): the original sets the vtable, calls Shutdown and
// then the DriverCallbackInterface destructor. That base is not inherited in the port, so
// only Shutdown runs. D0 0x88fa28 is the compiler's deleting form of the same destructor.
DriverAndroid::~DriverAndroid() {
    Shutdown();
}

// 0x88f328 _ZN3vox13DriverAndroid16UpdateThreadedATEPv (partial)
// The original's thread body runs under the lock: AttachCurrentThread, the AudioTrack creation
// (stream 3, 44100 Hz, stereo 16-bit, 4 * buffer frames bytes, MODE_STREAM), play, NewByteArray,
// then the DoCallbackAT loop while m_running is set (sleeping while paused), stop, release and
// DetachCurrentThread. All of it is JNI and not ported. The port never starts this thread, since
// the JavaVM branch of _InitAT is not ported, so only the lock pair that brackets the body runs.
int DriverAndroid::UpdateThreadedAT(void* unused) {
    (void)unused;
    mutex_.Lock();
    mutex_.Unlock();
    return 0;
}

// 0x88f14c _ZN3vox13DriverAndroid12DoCallbackATERP7_jarray (partial)
// The original's JNI part is not ported: GetPrimitiveArrayCritical on the array, the base
// _FillBuffer of the chunk into it, ReleasePrimitiveArrayCritical, the AudioTrack write of
// 4 * chunk bytes, and m_dataDuration += m_updateTime. The base _FillBuffer is not reachable and
// there is no JNIEnv, so m_dataDuration does not grow in the port. The timing tail runs as in the
// original. Its usleep result is not returned, since the caller never reads it. The usleep call is
// replaced by std::this_thread::sleep_for with the same microsecond count.
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
// The original never reads its argument. Its first step is SetDriverSampleRate with the constant
// 44100 (MOVW #0xAC44 in the listing), and SetDriverSampleRate leaves r0 unchanged, so the
// no-JavaVM path returns 44100 as well. Init discards the result. With a JavaVM the original looks
// up android/media/AudioTrack and its methods, sizes the ring (frames = getMinBufferSize / 4,
// chunk = min(frames, 1024), m_updateTime = chunk / 44100, m_dataThreshold = -(frames / 44100 *
// m_dataThresholdRatio), m_dataDuration = 0, mode 1, m_running set, paused cleared) and starts the
// thread with pthread_create. That branch is JNI and is not ported.
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
