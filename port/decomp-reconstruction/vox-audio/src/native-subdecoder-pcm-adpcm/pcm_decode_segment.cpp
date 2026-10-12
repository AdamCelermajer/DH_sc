// PCM sub-decoder decode of a segment into a caller buffer. Both functions are partial: the
// pseudocode's first stream Read call passes only the stream and the destination (the size
// argument is in a register the listing does not show), so the port reads the remaining
// byte count there. The second Read call's size is explicit in the pseudocode.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "vox-audio/vox/native_sub_decoder.hpp"

namespace vox {

namespace {

// __aeabi_uidiv returns 0 for a zero divisor on the target, as the original does.
std::uint32_t UDivOrZero(std::uint32_t a, std::uint32_t b) { return b == 0 ? 0 : a / b; }

}  // namespace

// 0x888050 _ZN3vox22VoxNativeSubDecoderPCM13DecodeSegmentEPviPNS_12SegmentStateE (partial)
int VoxNativeSubDecoderPCM::DecodeSegment(void* dst, int bytes, SegmentState* segment) {
    auto* out = static_cast<std::uint8_t*>(dst);
    const NativeChunk& chunk = chunks_->chunks[static_cast<std::size_t>(segment->chunk_index)];
    const int frame_bytes = bytes_per_sample_;
    const int stream_pos = segment->byte_offset + chunk.byte_offset + base_offset_;
    if (stream_pos != stream_->Tell()) {
        stream_->Seek(stream_pos, 0);
    }

    int decoded = 0;
    bool ended = false;  // the original's `break` (sets the state to 1)
    if (bytes > 0) {
        for (;;) {
            const int end_bytes = frame_bytes + segment->end_frame * frame_bytes;
            int read = 0;
            if (bytes - decoded + segment->byte_offset <= end_bytes) {
                // Size argument inferred (see the file comment).
                read = stream_->Read(out + decoded, bytes - decoded);
                segment->byte_offset += read;
            } else {
                read = stream_->Read(out + decoded, end_bytes - segment->byte_offset);
                segment->byte_offset = end_bytes;
            }
            if (read == 0) {
                ended = true;
                break;
            }

            decoded += read;
            segment->sample_position = static_cast<int>(
                UDivOrZero(static_cast<std::uint32_t>(segment->byte_offset),
                           static_cast<std::uint32_t>(frame_bytes)));

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
                    ended = true;
                    break;
                }
            }
            if (bytes <= decoded) {
                break;
            }
        }
    }

    if (ended) {
        segment->state = 1;
    }
    if (segment->play_mode == 3) {
        segment->state = 1;
    }
    return decoded;
}

// 0x8882ac _ZN3vox22VoxNativeSubDecoderPCM30DecodeCurrentSegmentWithOffsetEPvi (partial)
// The current segment (segments_[2]) after a pending seek. The pending frames are zeroed
// first and count as decoded bytes.
int VoxNativeSubDecoderPCM::DecodeCurrentSegmentWithOffset(void* dst, int bytes) {
    auto* out = static_cast<std::uint8_t*>(dst);
    SegmentState& cur = segments_[2];
    const NativeChunk& chunk = chunks_->chunks[static_cast<std::size_t>(cur.chunk_index)];
    const int frame_bytes = bytes_per_sample_;
    const int pending = pending_seek_;
    int decoded = 0;
    if (pending > 0) {
        decoded = pending * frame_bytes;
        std::memset(out, 0, static_cast<std::size_t>(decoded));
        pending_seek_ = 0;
    }
    const int stream_pos = chunk.byte_offset + cur.byte_offset + base_offset_;
    if (stream_pos != stream_->Tell()) {
        stream_->Seek(stream_pos, 0);
    }

    if (decoded < bytes) {
        do {
            const int end_bytes = frame_bytes + cur.end_frame * frame_bytes;
            int read = 0;
            if (bytes - decoded + cur.byte_offset <= end_bytes) {
                // Size argument inferred (see the file comment).
                read = stream_->Read(out + decoded, bytes - decoded);
                cur.byte_offset += read;
            } else {
                read = stream_->Read(out + decoded, end_bytes - cur.byte_offset);
                cur.byte_offset = end_bytes;
            }
            if (read == 0) {
                cur.state = 1;
                return decoded;
            }

            decoded += read;
            cur.sample_position = static_cast<int>(
                UDivOrZero(static_cast<std::uint32_t>(cur.byte_offset),
                           static_cast<std::uint32_t>(frame_bytes)));

            if (cur.sample_position > cur.end_frame) {
                const std::vector<std::int32_t>& entries =
                    (*loop_lists_)[static_cast<std::size_t>(cur.chunk_index)];
                if ((cur.entry_count >> 1) != 0 && cur.entry_count == cur.entries_left) {
                    cur.loop_start = entries[1];
                }
                cur.entries_left -= 1;
                if (cur.entries_left == 0) {
                    if (cur.restart_at_end == 1) {
                        cur.end_frame = entries.back();
                    }
                    UpdateSegmentsStates();
                }
                if (cur.state == 3) {
                    if (cur.entries_left != 0) {
                        Seek(-1, &cur);
                    }
                } else if (cur.state == 4 && cur.sample_position > cur.end_frame) {
                    cur.state = 1;
                    return decoded;
                }
            }
        } while (bytes > decoded);
    }
    return decoded;
}

}  // namespace vox
