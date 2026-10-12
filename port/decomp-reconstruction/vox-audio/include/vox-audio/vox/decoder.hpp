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

class DecoderCursorInterface {
public:
    virtual ~DecoderCursorInterface() = default;                 // 0x86fb38 (D1), 0x86fdec (D0)
    virtual void Init() {}                                       // 0x86fb3c
    virtual void Shutdown() {}                                   // 0x86fb40
    virtual int Decode(void* dst, int count) = 0;
    virtual int DecodeRef(void*& dst, int count);
    virtual void Reset();                                        // 0x86fb5c: Seek(0)
    virtual int GetRewindLimit() { return 0; }                   // 0x86fb74
    virtual void Rewind(int frames) { (void)frames; }            // 0x86fb7c (empty)
    virtual int Seek(unsigned int frame) = 0;
    virtual void SetLoop(bool on) { loop_ = on; }                // 0x86fb80
    virtual StreamCursorInterface* GetStreamCursor() { return stream_; }  // 0x86fb88
    virtual bool HasData() = 0;
    virtual bool AllowBufferReference() { return false; }        // 0x86fb90

protected:
    // Fields the base accessors read; the original keeps them in the same slots
    // for every cursor type (+0x14 decoder, +0x18 stream, +0x1c loop flag).
    DecoderInterface* decoder_ = nullptr;      // +0x14
    StreamCursorInterface* stream_ = nullptr;  // +0x18
    bool loop_ = false;                        // +0x1c
};

class DecoderInterface {
public:
    virtual ~DecoderInterface() = default;                       // 0x86fb2c (D1), 0x86fdd8 (D0)
    virtual void Init() {}                                       // 0x86fb30
    virtual void Shutdown() {}                                   // 0x86fb34
    virtual int GetType() const = 0;
    // Pointer to the four parameter words (channels, unknown_08, bits, length).
    virtual std::int32_t* GetParam() = 0;
    virtual DecoderCursorInterface* CreateNewCursor(StreamCursorInterface* stream) = 0;
    virtual void DestroyCursor(DecoderCursorInterface* cursor) = 0;
};

}  // namespace vox
