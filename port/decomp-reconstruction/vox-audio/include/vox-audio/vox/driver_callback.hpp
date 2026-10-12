// vox::DriverCallbackSourceInterface: a driver source fed by a callback. The engine
// mixes into a ring of DriverBuffer records, guarded by the Mutex at +0x08.
// Constructors and the destructor are not ported yet; the class uses its implicit
// default constructor in the port, and the members the reconstructed bodies touch are
// named. Other members stay out of the declaration until their bodies are ported.
#pragma once

#include <cstdint>

#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/mutex.hpp"

namespace vox {

// One ring entry, 0x18 bytes in the original. Only the ready byte at +0x14 is read by
// the reconstructed bodies. +0x04 holds the buffer size used by GetNbAvailableSamples.
struct DriverBuffer {
    std::uint32_t unknown_00_ = 0;  // +0x00 (not identified)
    std::int32_t size_ = 0;         // +0x04
    std::uint32_t unknown_08_[3] = {};  // +0x08..+0x13 (not identified)
    std::uint8_t ready_ = 0;        // +0x14, nonzero when the buffer is ready to be read
    std::uint8_t pad_[3] = {};
};

class DriverCallbackSourceInterface : public DriverSourceInterface {
public:
    // 0x88ed68: returns 1.
    int AllowBufferReference();
    // 0x88ed70: returns the buffer count (+0x44).
    int GetBufferCount() const;
    // 0x88fce4: partial (see driver_callback.cpp).
    void Update(float dt) override;
    // 0x890148: every buffer is marked ready and the read and write indices reset.
    void FreeAllBuffer();
    // 0x890fd0: state 1 (playing), unless the state is -1.
    void Play();
    // 0x890f48: state 1 moves to 2 (paused) and the two counters clear.
    void Pause();
    // 0x890f84: a state other than -1 becomes 3; FreeAllBuffer runs and the counters clear.
    void Stop();
    // 0x890ee8: when buffers exist, state 0, FreeAllBuffer, byte offset 0.
    void Reset();
    // 0x890e8c: state 1 becomes 3 while the write buffer is ready; otherwise the state.
    int GetState();
    // 0x890e0c: ready flag of the read buffer, or 0 when there is no buffer or state -1.
    int NeedData();
    // 0x890b28: stores the byte offset (+0x58) under the lock.
    void SetByteOffset(int offset);
    // 0x890b50: reads the byte offset under the lock.
    int GetByteOffset();
    // 0x890af8: the buffer list end is reset to the begin pointer.
    void Cleanup();
    // 0x890288: stores the static driver sample rate.
    static void SetDriverSampleRate(int rate);

    // 0x890b78 (partial): the pitch word at +0x34 converted to float.
    float GetPitch();
    // 0x890bb0 (partial): the gain word at +0x28 converted to float.
    float GetGain();

private:
    friend struct DriverCallbackSourceInterfaceTestAccess;
    Mutex mutex_;                         // +0x08..+0x27 (pthread_mutex_t in the original)
    std::int32_t field_2c_ = 0;           // +0x2c, cleared by Pause and Stop
    std::int32_t field_30_ = 0;           // +0x30, cleared by Pause and Stop
    std::int32_t field_34_ = 0;           // +0x34, the pitch word (fixed point)
    std::int32_t field_28_ = 0;           // +0x28, the gain word (fixed point)
    std::int32_t buffer_count_ = 0;       // +0x44
    std::int32_t read_index_ = 0;         // +0x48
    std::int32_t write_index_ = 0;        // +0x4c
    std::int32_t state_ = -1;             // +0x50: -1 none, 0 idle, 1 playing, 2 paused, 3 stopping
    std::int32_t byte_offset_ = 0;        // +0x58
    std::int32_t buffer_bytes_ = 0;       // +0x5c
    DriverBuffer* buffers_begin_ = nullptr;  // +0x60
    DriverBuffer* buffers_end_ = nullptr;    // +0x64

    static std::int32_t s_driverSampleRate_;  // 0x8902 global at _ZN3vox29DriverCallbackSourceInterface18s_driverSampleRateE
};

}  // namespace vox
