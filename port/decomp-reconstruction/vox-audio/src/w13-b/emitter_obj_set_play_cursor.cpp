// vox::EmitterObj::SetPlayCursor (0x8653ec). Moves the play position of a non-native emitter.
// The position is in seconds. The decoder cursor's sample rate (cursor +0x08, TrackParams
// unknown_08) turns it into a frame; the source is reset and moved to the byte offset
// (frames * channels * bytes per sample), and the decoder cursor seeks to the frame.
//
// Partial: when the data object's state (+0x50) is not 0, the listing takes the decoder as
// null and calls its GetType slot, which faults. The port does not seek on that path. The
// original returns the Mutex::Unlock result, which the port returns too.
#include "vox-audio/vox/data_obj.hpp"
#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/emitter_obj.hpp"

namespace vox {

// Decoder type of a native stream (DecoderInterface::GetType). Native emitters move their
// play position through their own path, so this one skips them.
constexpr int kDecoderTypeNative = 4;

int EmitterObj::SetPlayCursor(float position) {
    mutex.Lock();
    if (dsp_source_ != nullptr && decoder_cursor_ != nullptr &&
        decoder_cursor_->GetTrackParams().length_frames > 0) {
        // The listing reads the data object's decoder only while its state is 0.
        const DecoderInterface* decoder =
            (data_ != nullptr && data_->state_ == 0) ? data_->decoder_ : nullptr;
        if (decoder != nullptr && decoder->GetType() != kDecoderTypeNative) {
            if (position < 0.0f) {
                position = 0.0f;
            }
            const TrackParams& track = decoder_cursor_->GetTrackParams();
            const auto frames = static_cast<std::int32_t>(static_cast<float>(track.unknown_08) * position);
            dsp_source_->Reset();
            dsp_source_->SetByteOffset(track.channels * (track.bits_per_sample >> 3) * frames);
            decoder_cursor_->Seek(static_cast<unsigned int>(frames));
            if (state_set_ == 2) {
                state_set_ = 3;
            }
        }
    }
    return mutex.Unlock();
}

}  // namespace vox
