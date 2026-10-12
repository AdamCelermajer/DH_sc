// vox::DriverCallbackSourceInterface state machine, ring-buffer ready flags and the
// fixed-point accessors, on a buffer array owned by the test.
#include <cstdio>

#include "vox-audio/vox/driver_callback.hpp"

namespace vox {
struct DriverCallbackSourceInterfaceTestAccess {
    static void SetState(DriverCallbackSourceInterface& d, int state) { d.state_ = state; }
    static int State(const DriverCallbackSourceInterface& d) { return d.state_; }
    static void SetBuffers(DriverCallbackSourceInterface& d, DriverBuffer* begin, int count) {
        d.buffers_begin_ = begin;
        d.buffers_end_ = begin + count;
        d.buffer_count_ = count;
    }
    static void SetIndices(DriverCallbackSourceInterface& d, int read, int write) {
        d.read_index_ = read;
        d.write_index_ = write;
    }
    static int ReadIndex(const DriverCallbackSourceInterface& d) { return d.read_index_; }
    static void SetWords(DriverCallbackSourceInterface& d, int gain, int pitch) {
        d.field_28_ = gain;
        d.field_34_ = pitch;
    }
    static int ByteOffset(const DriverCallbackSourceInterface& d) { return d.byte_offset_; }
    static void SetByteOffsetRaw(DriverCallbackSourceInterface& d, int value) { d.byte_offset_ = value; }
};
}  // namespace vox

namespace {
int g_failures = 0;
#define CHECK(cond)                                                       \
    do {                                                                  \
        if (!(cond)) {                                                    \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__,   \
                         __LINE__, #cond);                                \
            ++g_failures;                                                 \
        }                                                                 \
    } while (0)
}  // namespace

int main() {
    using vox::DriverBuffer;
    using vox::DriverCallbackSourceInterface;
    using vox::DriverCallbackSourceInterfaceTestAccess;

    static_assert(sizeof(DriverBuffer) == 0x18, "buffer stride is 0x18 bytes");

    DriverCallbackSourceInterface source;
    CHECK(source.AllowBufferReference() == 1);
    CHECK(source.GetBufferCount() == 0);
    CHECK(source.GetState() == -1);

    // With state -1 nothing moves.
    source.Play();
    CHECK(DriverCallbackSourceInterfaceTestAccess::State(source) == -1);

    DriverBuffer buffers[3];
    buffers[0].ready_ = 1;
    buffers[1].ready_ = 0;
    buffers[2].ready_ = 0;
    DriverCallbackSourceInterfaceTestAccess::SetBuffers(source, buffers, 3);
    DriverCallbackSourceInterfaceTestAccess::SetState(source, 0);
    CHECK(source.GetBufferCount() == 3);

    // Play then Pause: 0 -> 1 -> 2; a second Pause changes nothing.
    source.Play();
    CHECK(DriverCallbackSourceInterfaceTestAccess::State(source) == 1);
    source.Pause();
    CHECK(DriverCallbackSourceInterfaceTestAccess::State(source) == 2);
    source.Pause();
    CHECK(DriverCallbackSourceInterfaceTestAccess::State(source) == 2);

    // Byte offset round trip (SetByteOffset writes +0x58 under the lock).
    source.SetByteOffset(4096);
    CHECK(source.GetByteOffset() == 4096);

    // NeedData reports the read buffer's ready flag while playing.
    DriverCallbackSourceInterfaceTestAccess::SetState(source, 1);
    DriverCallbackSourceInterfaceTestAccess::SetIndices(source, 0, 0);
    CHECK(source.NeedData() == 1);
    DriverCallbackSourceInterfaceTestAccess::SetIndices(source, 1, 0);
    CHECK(source.NeedData() == 0);

    // GetState: playing with the write buffer ready reports 3, otherwise the state.
    DriverCallbackSourceInterfaceTestAccess::SetIndices(source, 0, 0);
    CHECK(source.GetState() == 3);
    DriverCallbackSourceInterfaceTestAccess::SetIndices(source, 0, 1);
    CHECK(source.GetState() == 1);

    // Stop from playing: 3, every buffer ready, indices and byte offset cleared.
    buffers[1].ready_ = 0;
    source.Stop();
    CHECK(DriverCallbackSourceInterfaceTestAccess::State(source) == 3);
    CHECK(buffers[0].ready_ == 1 && buffers[1].ready_ == 1 && buffers[2].ready_ == 1);
    CHECK(DriverCallbackSourceInterfaceTestAccess::ReadIndex(source) == 0);
    CHECK(source.GetByteOffset() == 0);

    // Reset with buffers: state 0 and the byte offset cleared again.
    DriverCallbackSourceInterfaceTestAccess::SetByteOffsetRaw(source, 77);
    source.Reset();
    CHECK(DriverCallbackSourceInterfaceTestAccess::State(source) == 0);
    CHECK(source.GetByteOffset() == 0);

    // Cleanup drops the buffer list end back to the begin pointer.
    source.Cleanup();
    CHECK(source.GetBufferCount() == 3);
    CHECK(source.NeedData() == 0);  // no buffers left in the list, so no ready flag

    // Fixed-point accessors: 16384 is 0.5 after the 2^-15 scale, 32768 is 1.0.
    DriverCallbackSourceInterfaceTestAccess::SetWords(source, 32768, 16384);
    CHECK(source.GetGain() == 1.0f);
    CHECK(source.GetPitch() == 0.5f);

    std::printf("driver_callback_test: %s\n", g_failures == 0 ? "ok" : "FAILED");
    return g_failures == 0 ? 0 : 1;
}
