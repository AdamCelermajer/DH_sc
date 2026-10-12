// Decoder and decoder-cursor interfaces (vox::DecoderInterface family).
#pragma once

#include <cstdint>

#include "vox-audio/vox/stream.hpp"

namespace vox {

class DecoderInterface;

// Leading 16 bytes of vox::TrackParams as the decoders copy them. The original
// struct is larger; only these four words are read by the code ported so far.
// Field names are inferred from how DecoderRawCursor::Seek uses them.
struct TrackParams {
    std::int32_t channels = 0;
    std::int32_t unknown_08 = 0;       // not read by the functions ported so far
    std::int32_t bits_per_sample = 0;  // byte width is bits_per_sample >> 3
    std::int32_t length_frames = 0;
};

// Vtable slots (vptr-relative, from the concrete cursor vtables in vtables-001.json):
// +0x00/+0x04 destructors, +0x08 Init, +0x0c Shutdown, +0x10 Decode, +0x14 DecodeRef,
// +0x18 HasData, +0x1c Reset, +0x20 GetRewindLimit, +0x24 Rewind, +0x28 Seek, +0x2c SetLoop,
// +0x30 GetStreamCursor, +0x34 AllowBufferReference. Decode, HasData and Seek are pure.
class DecoderCursorInterface {
public:
    virtual ~DecoderCursorInterface() = default;                 // 0x86fb38 (D1), 0x86fdec (D0)
    virtual void Init() {}                                       // +0x08, 0x86fb3c
    virtual void Shutdown() {}                                   // +0x0c, 0x86fb40
    virtual int Decode(void* dst, int count) = 0;                // +0x10
    virtual int DecodeRef(void*& dst, int count);                // +0x14
    virtual bool HasData() = 0;                                  // +0x18
    virtual void Reset();                                        // +0x1c, 0x86fb5c: Seek(0)
    virtual int GetRewindLimit() { return 0; }                   // +0x20, 0x86fb74
    virtual void Rewind(int frames) { (void)frames; }            // +0x24, 0x86fb7c (empty)
    virtual int Seek(unsigned int frame) = 0;                    // +0x28
    virtual void SetLoop(bool on) { loop_ = on; }                // +0x2c, 0x86fb80
    virtual StreamCursorInterface* GetStreamCursor() { return stream_; }  // +0x30, 0x86fb88
    virtual bool AllowBufferReference() { return false; }        // +0x34, 0x86fb90

protected:
    // Fields the base accessors read; the original keeps them in the same slots
    // for every cursor type (+0x14 decoder, +0x18 stream, +0x1c loop flag).
    DecoderInterface* decoder_ = nullptr;      // +0x14
    StreamCursorInterface* stream_ = nullptr;  // +0x18
    bool loop_ = false;                        // +0x1c
};

// Decoder vtable slots (vptr-relative): +0x00/+0x04 destructors, +0x08 Init, +0x0c Shutdown,
// +0x10 CreateNewCursor, +0x14 DestroyCursor, +0x18 GetType, +0x1c GetParam. The four pure
// slots are confirmed by the concrete DecoderNative, DecoderMPC8, DecoderMSWav and DecoderRaw
// vtables (for example DecoderNative: CreateNewCursor at +0x10, GetType at +0x18).
class DecoderInterface {
public:
    virtual ~DecoderInterface() = default;                       // 0x86fb2c (D1), 0x86fdd8 (D0)
    virtual void Init() {}                                       // +0x08, 0x86fb30
    virtual void Shutdown() {}                                   // +0x0c, 0x86fb34
    virtual DecoderCursorInterface* CreateNewCursor(StreamCursorInterface* stream) = 0;  // +0x10
    virtual void DestroyCursor(DecoderCursorInterface* cursor) = 0;  // +0x14
    virtual int GetType() const = 0;                             // +0x18 (1 MSWav, 3 MPC8, 4 Native)
    // Pointer to the four parameter words (channels, unknown_08, bits, length).
    virtual std::int32_t* GetParam() = 0;                        // +0x1c
};

}  // namespace vox
