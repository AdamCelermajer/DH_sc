// VoxEngine instance: singleton lifetime, the two update threads and the update
// timestamp. The threads' constructor and destructor are declared (vox_thread.hpp)
// but not defined yet, so the bodies that create or destroy them stay partial.
#include <chrono>
#include <cstdint>
#include <new>

#include "vox-audio/vox/decoder_raw.hpp"
#include "vox-audio/vox/decoder_stb_vorbis.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/mutex.hpp"
#include "vox-audio/vox/stream.hpp"
#include "vox-audio/vox/vox_thread.hpp"
#include "vox-audio/vox/voxengine.hpp"

namespace vox {

// Factories whose headers do not declare them yet (decoder_mswav.hpp, decoder_mpc8.hpp and
// decoder_native.hpp only name them in comments).
void* DecoderMSWavFactory(void* params);
void* DecoderMPC8Factory(void* params);
void* DecoderNativeFactory(void* params);

namespace {

// Object size the listing allocates for each update thread (VoxAlloc(0x68)).
constexpr std::size_t kVoxThreadObjectBytes = 0x68;

using StreamFactory = StreamInterface* (*)(void*);
using DecoderFactory = DecoderInterface* (*)(void*);

// Stand-in for _GetTime (0x862bf4, gettimeofday seconds in the original). The port reads a
// monotonic clock instead: only differences between two stamps are used, so the epoch does
// not matter.
double EngineGetTime() {
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

// GetTimeDT(previous, now) (0x8616ac): now - previous, clamped to 0 when negative.
double EngineGetTimeDelta(double previous, double now) {
    const double delta = now - previous;
    return delta < 0.0 ? 0.0 : delta;
}

}  // namespace

VoxEngine* VoxEngine::s_voxEngine = nullptr;

// 0x862abc _ZN3vox9VoxEngineC1Ev and 0x862b80 _ZN3vox9VoxEngineC2Ev
// The listing zeroes the thread slots and the timestamp, allocates the mutex and takes
// the engine object from VoxEngineInternal. The vtable write is the C++ object model's.
VoxEngine::VoxEngine() {
    void* storage = VoxAlloc(sizeof(Mutex), kVoxMemHintDefault);
    mutex_ = new (storage) Mutex();
    m_internal = VoxEngineInternal::GetVoxEngineInternal();
}

// 0x862990 _ZN3vox9VoxEngineD1Ev and 0x862a34 _ZN3vox9VoxEngineD2Ev (partial: the
// VoxEngineInternal destructor, 0x86be68, does not yet reproduce FileSystemInterface::DestroyInstance
// and the +0x598 buffer release)
// The listing calls the engine's virtual destructor (slot 0) and frees it with VoxFree,
// then destroys and frees the mutex.
VoxEngine::~VoxEngine() {
    Shutdown();
    if (m_internal != nullptr) {
        m_internal->~VoxEngineInternal();
        VoxFree(m_internal);
    }
    m_internal = nullptr;
    if (mutex_ != nullptr) {
        mutex_->~Mutex();
        VoxFree(mutex_);
    }
}

// 0x862a18 _ZN3vox9VoxEngineD0Ev: compiler-generated deleting destructor. It runs the
// complete destructor above and then frees the object, which `delete` provides.

// 0x862b30 _ZN3vox9VoxEngine12GetVoxEngineEv
VoxEngine* VoxEngine::GetVoxEngine() {
    if (s_voxEngine != nullptr) {
        return s_voxEngine;
    }
    void* storage = VoxAlloc(sizeof(VoxEngine), kVoxMemHintDefault);
    s_voxEngine = new (storage) VoxEngine();
    return s_voxEngine;
}

// 0x8628f8 _ZN3vox9VoxEngine16DestroyVoxEngineEv (partial: the destructor it calls is partial, see above)
void VoxEngine::DestroyVoxEngine() {
    if (s_voxEngine != nullptr) {
        s_voxEngine->~VoxEngine();
        VoxFree(s_voxEngine);
        s_voxEngine = nullptr;
    }
}

// 0x862944 _ZN3vox9VoxEngine8ShutdownEv (reconstructed: VoxThread destructor is in drivers-threads)
// Destroys the emitter thread (+0x04) and then the source thread (+0x08).
void VoxEngine::Shutdown() {
    if (emitter_thread_ != nullptr) {
        emitter_thread_->~VoxThread();
        VoxFree(emitter_thread_);
        emitter_thread_ = nullptr;
    }
    if (source_thread_ != nullptr) {
        source_thread_->~VoxThread();
        VoxFree(source_thread_);
        source_thread_ = nullptr;
    }
}

// 0x862d10 _ZN3vox9VoxEngine10InitializeEv (partial: the MPC8 decoder factory (0x8704dc) is skipped in
// the ledger, and the VoxThread constructor (0x893784) does not apply the priority adjustment;
// the listing also returns the low word of the start time, which the port does not carry)
// Initializes the engine object, registers the stream and decoder factories in the
// order the listing uses, starts the two update threads and records the start time.
void VoxEngine::Initialize() {
    if (m_internal == nullptr) {
        return;
    }
    m_internal->Initialize();

    RegisterStreamType(reinterpret_cast<StreamFactory>(&StreamMemoryBufferFactory));
    RegisterStreamType(reinterpret_cast<StreamFactory>(&StreamCFileFactory));
    RegisterDecoderType(reinterpret_cast<DecoderFactory>(&DecoderRawFactory));
    RegisterDecoderType(reinterpret_cast<DecoderFactory>(&DecoderMSWavFactory));
    RegisterDecoderType(reinterpret_cast<DecoderFactory>(&DecoderStbVorbisFactory));
    RegisterDecoderType(reinterpret_cast<DecoderFactory>(&DecoderMPC8Factory));
    RegisterDecoderType(reinterpret_cast<DecoderFactory>(&DecoderNativeFactory));
    RegisterDecoderType(nullptr);

    auto emitter_callback = static_cast<void (*)(void*, void*)>(&VoxEngine::UpdateEmittersThreaded);
    void* emitter_storage = VoxAlloc(kVoxThreadObjectBytes);
    emitter_thread_ = new (emitter_storage)
        VoxThread(emitter_callback, this, nullptr, "VoxEngine::UpdateEmitters");

    auto source_callback = static_cast<void (*)(void*, void*)>(&VoxEngine::UpdateSourcesThreaded);
    void* source_storage = VoxAlloc(kVoxThreadObjectBytes);
    source_thread_ = new (source_storage) VoxThread(source_callback, this, nullptr, "VoxEngine::UpdateSources");

    last_update_time_ = EngineGetTime();
}

// 0x862c4c _ZN3vox9VoxEngine22UpdateEmittersThreadedEv (partial: the listing returns the result of
// VoxEngineInternal::UpdateEmitters (0x86db40), which ends in AccessController::ReleaseReadAccess or
// ReleaseWriteAccess (0x89351c, 0x8935xx). Those return the unlock int in the binary but are void in the
// port, so the int is dropped. The port's void signature matches the thread callback type that Initialize casts to.)
// The elapsed time uses the previous stamp, the stamp never moves backwards, and the engine
// receives the delta as a float, as the listing does.
void VoxEngine::UpdateEmittersThreaded() {
    if (m_internal == nullptr) {
        return;
    }
    double now = EngineGetTime();
    float dt = static_cast<float>(EngineGetTimeDelta(last_update_time_, now));
    if (now < last_update_time_) {
        now = last_update_time_;
    }
    last_update_time_ = now;
    m_internal->UpdateEmitters(dt);
}

// 0x862d0c _ZN3vox9VoxEngine22UpdateEmittersThreadedEPvS1_ is a thunk to 0x862c4c. The thread
// callback needs a (void*, void*) function; this static overload is that entry point.
void VoxEngine::UpdateEmittersThreaded(void* engine, void* unused) {
    (void)unused;
    static_cast<VoxEngine*>(engine)->UpdateEmittersThreaded();
}

// 0x862cf0 _ZN3vox9VoxEngine14UpdateThreadedEv (partial: returns the emitters update's int word,
// which the port's void signature does not carry)
// The listing calls the sources update and then the emitters update.
void VoxEngine::UpdateThreaded() {
    UpdateSourcesThreaded();
    UpdateEmittersThreaded();
}

}  // namespace vox
