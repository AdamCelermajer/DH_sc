// Stand-ins for the WAV block the MSWav sub-decoders read. The parser that builds them
// (DecoderMSWavCursor::ParseFile, 0x870610) is not reconstructed yet, so only the fields the
// sub-decoders use are declared. The comments give the original 32-bit offsets.
#pragma once

#include <cstdint>

namespace vox {

// One data chunk of the WAV file, in the list that starts at WaveChunk::data_list.
struct DataNode {
    std::uint32_t offset = 0;      // +0x00, position of the chunk header in the stream
    std::uint32_t size = 0;        // +0x04, size of the chunk data in bytes
    DataNode* next = nullptr;      // +0x08
};

// The parsed format and data of a WAV file.
struct WaveChunk {
    int channels = 0;                    // +0x16 (uint16 in the file block)
    int sample_rate = 0;                 // +0x18 (uint32)
    int block_align = 0;                 // +0x20 (uint16), bytes per frame
    int bits_per_sample = 0;             // +0x22 (uint16)
    std::uint32_t current_data_size = 0; // +0x28, size of the data node the cursor is in
                                         //       (written by VoxMSWavSubDecoder::GoToNextDataChunk)
    DataNode* data_list = nullptr;       // +0x38, first data chunk
};

}  // namespace vox
