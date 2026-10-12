// IMA ADPCM sub-decoder: seek and decode into a caller buffer. All four functions are partial:
// they call VoxNativeSubDecoderIMAADPCM::DecodeBlock (0x887068), which is declared in the
// header but not defined in this slice. UpdateSegmentsStates is the base class method.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "vox-audio/vox/native_sub_decoder_imaadpcm.hpp"

namespace vox {

namespace {

// __aeabi_idiv and __aeabi_uidiv return 0 for a zero divisor on the target.
int DivOrZero(int a, int b) { return b == 0 ? 0 : a / b; }
std::uint32_t UDivOrZero(std::uint32_t a, std::uint32_t b) { return b == 0 ? 0 : a / b; }

}  // namespace

// 0x887464 _ZN3vox27VoxNativeSubDecoderIMAADPCM4SeekEiPNS_12SegmentStateE (partial)
// Seeks to a frame of the segment's chunk and decodes the block that holds it into the
// block's decoding slot. A negative offset means the loop start.
int VoxNativeSubDecoderIMAADPCM::Seek(int sample_offset, SegmentState* segment) {
    const int frame = sample_offset < 0 ? segment->loop_start : sample_offset;
    const NativeChunk& chunk = (*chunks_)[static_cast<std::size_t>(segment->chunk_index)];
    if (frame > chunk.frame_count) {
        return -1;
    }
    const int block_index = static_cast<int>(
        UDivOrZero(static_cast<std::uint32_t>(frame), static_cast<std::uint32_t>(frames_per_block_)));
    const int byte_in_chunk = static_cast<int>(bytes_per_sample_) * block_index;
    segment->byte_offset = byte_in_chunk;
    const int result = stream_->Seek(base_offset_ + chunk.byte_offset + byte_in_chunk, 0);
    if (result != 0) {
        return result;
    }
    const int slot = segment->decoding_slot;
    const int block_start = frames_per_block_ * block_index;
    const int into_block = frame - block_start;
    frames_into_block_[slot] = into_block;
    segment->sample_position = block_start + into_block;
    block_result_[slot] = DecodeBlock(decode_buffers_->slot[slot], segment);
    return 0;
}

// 0x887544 _ZN3vox27VoxNativeSubDecoderIMAADPCM34SetDecodingBufferToSegmentPositionEPNS_12SegmentStateE (partial)
void VoxNativeSubDecoderIMAADPCM::SetDecodingBufferToSegmentPosition(SegmentState* segment) {
    const int frame = segment->sample_position;
    const int slot = segment->decoding_slot;
    const int block_start = frames_per_block_ *
        static_cast<int>(UDivOrZero(static_cast<std::uint32_t>(frame),
                                    static_cast<std::uint32_t>(frames_per_block_)));
    segment->sample_position = block_start;
    segment->byte_offset = bytes_per_sample_ *
        static_cast<int>(UDivOrZero(static_cast<std::uint32_t>(block_start),
                                    static_cast<std::uint32_t>(frames_per_block_)));
    const int result = DecodeBlock(decode_buffers_->slot[slot], segment);
    block_result_[slot] = result;
    frames_into_block_[slot] = frame - segment->sample_position;
    segment->sample_position = frame;
}

// 0x8875b8 _ZN3vox27VoxNativeSubDecoderIMAADPCM13DecodeSegmentEPviPNS_12SegmentStateE (partial)
// Returns the number of bytes written to dst (frames times channels times two).
int VoxNativeSubDecoderIMAADPCM::DecodeSegment(void* dst, int bytes, SegmentState* segment) {
    auto* out = static_cast<std::int16_t*>(dst);
    const int channels = channels_;
    const int bytes_per_channel_sample = bits_ >> 3;
    const int total_frames = DivOrZero(bytes, channels * bytes_per_channel_sample);
    const int slot = segment->decoding_slot;
    int remaining = total_frames;
    bool stopped = false;  // the original's LABEL_24 (sets the state to 1)

    while (remaining > 0) {
        if (segment->reset_pending) {
            SetDecodingBufferToSegmentPosition(segment);
            segment->reset_pending = false;
        }
        int block = block_result_[slot];
        if (frames_into_block_[slot] == block) {
            block = DecodeBlock(decode_buffers_->slot[slot], segment);
            block_result_[slot] = block;
            frames_into_block_[slot] = 0;
        }
        if (block == 0) {
            stopped = true;
            break;
        }

        int take = segment->end_frame + 1 - segment->sample_position;
        if (remaining < take) {
            take = remaining;
        }
        const int in_block = block - frames_into_block_[slot];
        if (take >= in_block) {
            take = in_block;
        }
        // Copy the decoded frames of this block into the output, after the frames already done.
        const auto* decoded = static_cast<const std::int16_t*>(decode_buffers_->slot[slot]);
        std::memcpy(out + channels * (total_frames - remaining),
                    decoded + channels * frames_into_block_[slot],
                    static_cast<std::size_t>(2 * channels * take));
        remaining -= take;
        frames_into_block_[slot] += take;
        segment->sample_position += take;

        if (segment->sample_position > segment->end_frame) {
            const std::vector<std::int32_t>& entries =
                (*loop_lists_)[static_cast<std::size_t>(segment->chunk_index)];
            if ((segment->entry_count >> 1) != 0 && segment->entry_count == segment->entries_left) {
                segment->loop_start = entries[1];
            }
            segment->entries_left -= 1;
            if (segment->entries_left == 0) {
                if (segment->restart_at_end == 1) {
                    segment->end_frame = entries.back();
                }
                if (segment->play_mode == 1) {
                    UpdateSegmentsStates();
                }
            }
            if (segment->state == 3) {
                if (segment->entries_left != 0) {
                    Seek(-1, segment);
                }
            } else if (segment->state == 4 && segment->sample_position > segment->end_frame) {
                stopped = true;
                break;
            }
        }
    }

    if (stopped) {
        segment->state = 1;
    }
    if (segment->play_mode == 3) {
        segment->state = 1;
    }
    return (total_frames - remaining) * channels * bytes_per_channel_sample;
}

// 0x887860 _ZN3vox27VoxNativeSubDecoderIMAADPCM30DecodeCurrentSegmentWithOffsetEPvi (partial)
// The current segment (segments_[2]) after a pending seek. The pending frames are zeroed
// first. Returns the number of bytes written (pending frames included).
int VoxNativeSubDecoderIMAADPCM::DecodeCurrentSegmentWithOffset(void* dst, int bytes) {
    auto* out = static_cast<std::int16_t*>(dst);
    SegmentState& cur = segments_[2];
    const int channels = channels_;
    const int bytes_per_channel_sample = bits_ >> 3;
    const int total_frames = DivOrZero(bytes, channels * bytes_per_channel_sample);
    const int pending = pending_seek_;
    int remaining = total_frames;
    if (pending > 0) {
        std::memset(out, 0, static_cast<std::size_t>(bytes_per_channel_sample * pending * channels));
        pending_seek_ = 0;
        remaining = total_frames - pending;
    }
    const int slot = cur.decoding_slot;
    bool stopped = false;  // the original's LABEL_27 (sets the state to 1)

    while (remaining > 0) {
        if (cur.reset_pending) {
            SetDecodingBufferToSegmentPosition(&cur);
            cur.reset_pending = false;
        }
        int block = block_result_[slot];
        if (frames_into_block_[slot] == block) {
            block = DecodeBlock(decode_buffers_->slot[slot], &cur);
            block_result_[slot] = block;
            frames_into_block_[slot] = 0;
        }
        if (block == 0) {
            stopped = true;
            break;
        }

        int take = cur.end_frame + 1 - cur.sample_position;
        if (remaining < take) {
            take = remaining;
        }
        const int in_block = block - frames_into_block_[slot];
        if (take >= in_block) {
            take = in_block;
        }
        const auto* decoded = static_cast<const std::int16_t*>(decode_buffers_->slot[slot]);
        std::memcpy(out + channels * (total_frames - remaining),
                    decoded + channels * frames_into_block_[slot],
                    static_cast<std::size_t>(2 * channels * take));
        frames_into_block_[slot] += take;
        remaining -= take;
        cur.sample_position += take;

        if (cur.sample_position > cur.end_frame) {
            const std::vector<std::int32_t>& entries =
                (*loop_lists_)[static_cast<std::size_t>(cur.chunk_index)];
            if ((cur.entry_count >> 1) != 0 && cur.entry_count == cur.entries_left) {
                cur.loop_start = entries[1];
            }
            cur.entries_left -= 1;
            if (cur.entries_left == 0 && cur.restart_at_end == 1) {
                cur.end_frame = entries.back();
            }
            if (cur.entries_left == 0) {
                UpdateSegmentsStates();
            }
            if (cur.state == 3) {
                if (cur.entries_left != 0) {
                    Seek(-1, &cur);
                }
            } else if (cur.state == 4 && cur.sample_position > cur.end_frame) {
                stopped = true;
                break;
            }
        }
    }

    if (stopped) {
        cur.state = 1;
    }
    return (total_frames - remaining) * channels * bytes_per_channel_sample;
}

}  // namespace vox
