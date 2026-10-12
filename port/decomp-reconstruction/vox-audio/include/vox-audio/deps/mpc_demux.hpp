// Declarations of the Musepack demuxer calls that DecoderMPC8Cursor makes (libmpcdec). The
// Musepack library is not part of the port: these are declared only, so any object that
// calls them needs the library at link time. Each struct below states the words the cursor reads.
#pragma once

#include <cstdint>

#include "vox-audio/deps/mpc_reader.hpp"

struct mpc_demux;  // opaque demuxer state (libmpcdec)

// Frame output of mpc_demux_decode (32-bit offsets). The cursor sets buffer to its PCM buffer
// before every call and reads samples, the number of sample frames decoded into that buffer.
struct MpcFrameInfo {
    std::int32_t samples = 0;        // +0x00
    std::int32_t reserved_04 = 0;    // +0x04, not read or written by the cursor
    float* buffer = nullptr;         // +0x08, decoded float samples go here
};

// The libmpcdec entry points, called by name as the original does.
mpc_demux* mpc_demux_init(mpc_reader_t* reader);
void mpc_demux_exit(mpc_demux* demux);
// Fills the stream info block. The cursor reads word 0 (sample rate), word 1 (channels) and
// word 14 (total sample frames).
void mpc_demux_get_info(mpc_demux* demux, std::uint32_t* info);
void mpc_demux_decode(mpc_demux* demux, MpcFrameInfo* frame);
// The original passes 0, 0 for the last two arguments on every call the cursor makes.
int mpc_demux_seek_sample(mpc_demux* demux, std::uint32_t sample, int unknown_a, int unknown_b);
