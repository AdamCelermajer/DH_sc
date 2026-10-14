// vox::DecoderMPC8Cursor: constructor, destructor, Seek and Decode. These call the Musepack
// demuxer (deps/mpc_demux.hpp), which the port does not link, so this object is only built
// into the library and not into the tests.
#include <cstring>

#include "vox-audio/deps/mpc_demux.hpp"
#include "vox-audio/vox/decoder_mpc8.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

namespace {

// The ARM integer divide returns 0 for a zero divisor (see Decisions in NOTES.md).
int DivOrZero(int a, int b) {
    return b == 0 ? 0 : a / b;
}

// Writes frames of float samples to out, starting at frame dst_frame. 32-bit output is a copy
// of the floats; anything else is converted to int16.
void CopyFrames(std::uint8_t* out, int dst_frame, int channels, int bits, const float* src, int frames) {
    const int samples = channels * frames;
    if (bits == 32) {
        std::memcpy(out + 4 * channels * dst_frame, src, static_cast<std::size_t>(4 * samples));
    } else {
        ConvertFloatSamplesToShort(reinterpret_cast<std::int16_t*>(out + 2 * channels * dst_frame),
                                   src, samples);
    }
}

}  // namespace

// 0x87018c _ZN3vox17DecoderMPC8CursorC1EPNS_16DecoderInterfaceEPNS_21StreamCursorInterfaceE
// 0x87031c _ZN3vox17DecoderMPC8CursorC2EPNS_16DecoderInterfaceEPNS_21StreamCursorInterfaceE
// Opens the demuxer on the reader and copies its stream info. The sample rate comes from the
// decoder's parameter when that is positive, else from the stream. A missing or idle demuxer
// leaves the info at zero.
DecoderMPC8Cursor::DecoderMPC8Cursor(DecoderMPC8* decoder, StreamCursorInterface* stream) {
    decoder_ = decoder;
    stream_ = stream;
    loop_ = false;
    // The original stores the five reader wrappers before anything else (0x87018c, +0x30..+0x40).
    reader_.read = &mpc_read;
    reader_.seek = &mpc_seek;
    reader_.tell = &mpc_tell;
    reader_.get_size = &mpc_getsize;
    reader_.can_seek = &mpc_canseek;
    reader_.data = stream;

    pcm_buffer_ = static_cast<float*>(VoxAlloc(0x4800));
    if (pcm_buffer_ != nullptr) {
        demux_ = mpc_demux_init(&reader_);
    }
    if (demux_ != nullptr) {
        // The original checks the first two words of the demuxer (its open state). The port
        // reads them the same way; the libmpcdec layout is not otherwise known here.
        const auto* words = reinterpret_cast<const std::uint32_t*>(demux_);
        if (words[1] != 0 && words[0] != 0) {
            std::uint32_t info[93] = {};
            mpc_demux_get_info(demux_, info);
            params_.bits_per_sample = 16;
            params_.channels = static_cast<std::int32_t>(info[1]);
            const int param = decoder->param_;
            params_.sample_rate = param > 0 ? param : static_cast<std::int32_t>(info[0]);
            params_.length_frames = static_cast<std::int32_t>(info[14]);
            return;
        }
    }
    params_ = TrackParams{};
}

// 0x8700a4 _ZN3vox17DecoderMPC8CursorD1Ev (D2 0x870110 has the same body)
// The demuxer is closed and the PCM buffer released.
DecoderMPC8Cursor::~DecoderMPC8Cursor() {
    if (demux_ != nullptr) {
        mpc_demux_exit(demux_);
    }
    if (pcm_buffer_ != nullptr) {
        VoxFree(pcm_buffer_);
    }
}

// 0x86fe00 _ZN3vox17DecoderMPC8Cursor4SeekEj
// Sample 0 is refused. A demuxer seek that fails is reported as -1. A successful seek resets
// the position inside the buffer's count (sample_pos_), as the original does.
int DecoderMPC8Cursor::Seek(unsigned int sample) {
    if (sample == 0) {
        return -1;
    }
    if (mpc_demux_seek_sample(demux_, sample, 0, 0) != 0) {
        return -1;
    }
    sample_pos_ = 0;
    return 0;
}

// 0x86fe38 _ZN3vox17DecoderMPC8Cursor6DecodeEPvi (partial: two items)
// Verified against the ARM listing (0x86FE38..0x8700A0): wanted = bytes / (channels * (bits >> 3))
// with the idiv-by-zero result 0; the buffered part copies through ConvertFloatToShort (or memcpy
// when bits is 32) either min(wanted, avail) frames or all avail frames; the demuxer loop stores
// the PCM buffer in the frame block, calls mpc_demux_decode, reads the sample count from the frame
// block, sets buffer_pos_ = 0 and buffer_frames_ = the count, keeps the pre-update sample_pos_ in
// r1 (which is the restart seek's argument), advances buffer_pos_ and sample_pos_ by the frame
// count or by the remaining count when the block overruns, subtracts that count from remaining,
// compares the new sample position with length_frames (+0x10) and continues while it differs and
// remaining is positive; at the length it tests the loop byte (+0x1c), seeks and clears
// sample_pos_. The return is (wanted - remaining) * channels * (bits >> 3).
// Deviation 1 (partial): on the block-overrun path the restart seek's r1 is clobbered by the
// ConvertFloatToShort/memcpy call before it, so the listing has no recoverable value there; the
// port passes the pre-update sample position.
// Deviation 2 (partial): the listing's loop test is on the remaining count alone, so a demuxer
// that returns 0 samples with frames left is called again for ever (a truncated stream spins);
// the port breaks out when the sample count is not positive.
int DecoderMPC8Cursor::Decode(void* dst, int bytes) {
    const std::int32_t channels = params_.channels;
    const std::int32_t bits = params_.bits_per_sample;
    const int bytes_per_sample = bits >> 3;
    const int wanted = DivOrZero(bytes, channels * bytes_per_sample);
    auto* out = static_cast<std::uint8_t*>(dst);
    int remaining = wanted;

    if (buffer_pos_ < buffer_frames_) {
        const int avail = buffer_frames_ - buffer_pos_;
        if (wanted < avail) {
            CopyFrames(out, 0, channels, bits, pcm_buffer_ + channels * buffer_pos_, wanted);
            buffer_pos_ += wanted;
            sample_pos_ += static_cast<std::uint32_t>(wanted);
            return wanted * channels * bytes_per_sample;
        }
        CopyFrames(out, 0, channels, bits, pcm_buffer_ + channels * buffer_pos_, avail);
        buffer_pos_ += avail;
        sample_pos_ += static_cast<std::uint32_t>(avail);
        remaining = wanted - avail;
    }

    while (remaining > 0) {
        MpcFrameInfo info;
        info.buffer = pcm_buffer_;  // the original stores the PCM buffer in the frame block first
        mpc_demux_decode(demux_, &info);
        const int decoded = info.samples;
        buffer_pos_ = 0;
        buffer_frames_ = decoded;
        if (decoded <= 0) {
            break;  // port deviation, see the comment above
        }

        const std::uint32_t prev = sample_pos_;
        const int dst_frame = wanted - remaining;
        if (decoded <= remaining) {
            CopyFrames(out, dst_frame, channels, bits, pcm_buffer_, decoded);
            const int advance = buffer_frames_;
            remaining -= advance;
            buffer_pos_ += advance;
            sample_pos_ = prev + static_cast<std::uint32_t>(advance);
            if (params_.length_frames != static_cast<std::int32_t>(sample_pos_)) {
                continue;
            }
        } else {
            CopyFrames(out, dst_frame, channels, bits, pcm_buffer_, remaining);
            buffer_pos_ += remaining;
            sample_pos_ = prev + static_cast<std::uint32_t>(remaining);
            remaining = 0;
            if (params_.length_frames != static_cast<std::int32_t>(sample_pos_)) {
                continue;
            }
        }

        // The stream has ended.
        if (!loop_ || mpc_demux_seek_sample(demux_, prev, 0, 0) != 0) {
            break;
        }
        sample_pos_ = 0;
    }

    return (wanted - remaining) * channels * bytes_per_sample;
}

}  // namespace vox
