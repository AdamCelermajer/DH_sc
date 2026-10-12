// vox::DriverAndroid and vox::DriverAndroidSource: the destructor, the JNI-bound audio
// thread helpers (not ported; their status is partial) and the callback source
// constructor.
#include "vox-audio/vox/driver_android.hpp"

namespace vox {

// 0x88fbac _ZN3vox19DriverAndroidSourceC1EPvS1_j and 0x88fc8c (C2): the callback source
// base is built first, then the vtable is set and Init builds the ring.
DriverAndroidSource::DriverAndroidSource(const std::int32_t* format,
                                         const std::int32_t* buffer_count,
                                         std::uint32_t flags)
    : DriverCallbackSourceInterface(format, buffer_count, flags) {
    Init();
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
// The original creates an AudioTrack through JNI, starts it, and then loops on DoCallbackAT
// until m_running clears, sleeping while the pause flag is set. None of the JNI calls are
// ported, so this returns without starting a loop.
int DriverAndroid::UpdateThreadedAT(void* unused) {
    (void)unused;
    return 0;
}

// 0x88f14c _ZN3vox13DriverAndroid12DoCallbackATERP7_jarray (partial)
// The original gets a critical section on the Java byte array, fills it from the mix
// (the DriverCallbackInterface base, not ported here), releases it, and advances the data
// duration counter. The timing tail that decides whether to sleep is also not ported.
void DriverAndroid::DoCallbackAT(_jarray*& array) {
    (void)array;
}

// 0x88f660 _ZN3vox13DriverAndroid7_InitATEPv (partial)
// The sample rate is stored first, as in the original. The rest (the AudioTrack class and
// method lookup, the buffer size query, the ring sizing and the thread start) is JNI setup
// and is not ported.
int DriverAndroid::_InitAT(int sample_rate) {
    DriverCallbackSourceInterface::SetDriverSampleRate(sample_rate);
    return 0;
}

}  // namespace vox
