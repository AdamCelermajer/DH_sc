// Stand-in for the reader struct the Musepack decoder (libmpcdec) reads through.
// The five callback slots come first in the original (read, seek, tell, get_size,
// canseek, at +0x00..+0x10); the game's wrappers use only the data pointer at +0x14,
// which here is the stream cursor the decoder reads from.
#pragma once

#include <cstdint>

namespace vox {
class StreamCursorInterface;
}

struct mpc_reader_t {
    void* callbacks[5] = {};                   // +0x00..+0x10, not used by the wrappers
    vox::StreamCursorInterface* data = nullptr;  // +0x14
};
