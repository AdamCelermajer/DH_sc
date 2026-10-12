// Decoder and decoder-cursor interfaces (vox::DecoderInterface family).
#pragma once

#include <cstdint>

#include "vox-audio/vox/stream.hpp"

namespace vox {

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
    virtual ~DecoderCursorInterface() = default;
    virtual int Decode(void* dst, int count) = 0;
    virtual int DecodeRef(void*& dst, int count);
    virtual int Seek(unsigned int frame) = 0;
    virtual bool HasData() = 0;
    virtual bool AllowBufferReference() { return false; }
};

class DecoderInterface {
public:
    virtual ~DecoderInterface() = default;
    virtual int GetType() const = 0;
    // Pointer to the four parameter words (channels, unknown_08, bits, length).
    virtual std::int32_t* GetParam() = 0;
    virtual DecoderCursorInterface* CreateNewCursor(StreamCursorInterface* stream) = 0;
    virtual void DestroyCursor(DecoderCursorInterface* cursor) = 0;
};

}  // namespace vox
