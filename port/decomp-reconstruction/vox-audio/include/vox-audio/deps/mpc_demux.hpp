// Declarations of the Musepack demuxer calls that DecoderMPC8Cursor makes (libmpcdec). The
// Musepack library is not part of the port: these are declared only, so any object that
// calls them needs the library at link time. Each struct below states the words the cursor reads.
#pragma once

#include <cstdint>

#include "vox-audio/deps/mpc_reader.hpp"

struct mpc_demux;  // opaque demuxer state (libmpcdec)

// Frame output of mpc_demux_decode. The cursor reads only the first word: the number of
// sample frames decoded into its buffer.
struct MpcFrameInfo {
    std::int32_t samples = 0;  // +0x00
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
