// vox::DecoderNativeCursor. Bodies are partial where they call the sub-decoder slots
// or the chunk loop of the file parser, which the port does not have yet. Each partial
// body says what is missing.
#include <cstdint>
#include <cstring>
#include <new>
#include <string>

#include "vox-audio/vox/decoder_native.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder.hpp"
#include "vox-audio/vox/native_sub_decoder_imaadpcm.hpp"
#include "vox-audio/vox/native_sub_decoder_state.hpp"

namespace vox {

namespace {
// "VoxN" read as a little-endian word (1316515670 in the pseudocode).
constexpr std::uint32_t kNativeMagic = 0x4E786F56u;
// Format tag that selects the IMA ADPCM sub-decoder; any other tag gives PCM.
constexpr std::uint16_t kFormatImaAdpcm = 17;
}  // namespace

// The sub-decoder is one of two concrete classes, chosen by the format tag when the
// original cursor is built. The original calls the sub-decoder's virtual slots (+0x08
// HasData, +0x0c Seek, +0x10 Reset) and its non-virtual base methods. Here the slots are
// reached through the concrete class that matches the tag, which is what the slot would
// select. Callers check sub_decoder_ for null first.
bool DecoderNativeCursor::IsImaAdpcm() const {
    return decoder_ != nullptr &&
           static_cast<const DecoderNative*>(decoder_)->format_tag_ == kFormatImaAdpcm;
}

// C1 0x8742f4 and C2 0x874678 (partial). Parses the file when the decoder still needs
// it. A failed parse leaves the cursor empty. The original then creates the PCM or IMA
// ADPCM sub-decoders with their states, the playlist manager, and copies the track
// parameters; none of that is ported.
DecoderNativeCursor::DecoderNativeCursor(DecoderInterface* decoder, StreamCursorInterface* stream) {
    decoder_ = decoder;
    stream_ = stream;
    auto* native = static_cast<DecoderNative*>(decoder);
    if (native->needs_parse_) {
        if (!ParseFile()) {
            return;
        }
        native->needs_parse_ = false;
    }
    state_names_ = &native->interactive_states_;
}

// D1 0x872314 and D2 0x872440. Destroys the owned sub-decoder, the two states and the
// playlist manager, each through its virtual destructor and VoxFree. The pending state
// list and the mutex are destroyed as members, after this body, as in the original.
DecoderNativeCursor::~DecoderNativeCursor() {
    if (sub_decoder_ != nullptr) {
        sub_decoder_->~VoxNativeSubDecoder();
        VoxFree(sub_decoder_);
        sub_decoder_ = nullptr;
    }
    if (state_a_ != nullptr) {
        state_a_->~NativeSubDecoderState();
        VoxFree(state_a_);
        state_a_ = nullptr;
    }
    if (state_b_ != nullptr) {
        state_b_->~NativeSubDecoderState();
        VoxFree(state_b_);
        state_b_ = nullptr;
    }
    if (playlists_ != nullptr) {
        playlists_->~NativePlaylistsManager();
        VoxFree(playlists_);
        playlists_ = nullptr;
    }
}

// 0x872218. With no sub-decoder the result is 0. Otherwise, on the second decode call or
// when the request would pass the frame capacity, the two saved states are swapped and the
// old current state is written back from the sub-decoder. The pending state index (or the
// base state 0 on the first call) is applied, the base Decode runs, and the frame counters
// move by the number of frames produced after the first call.
int DecoderNativeCursor::Decode(void* dst, int count) {
    if (sub_decoder_ == nullptr) {
        return 0;
    }
    const bool ima = IsImaAdpcm();
    if (decode_calls_ == 1 || count + frames_current_ > frames_capacity_) {
        NativeSubDecoderState* previous_a = state_a_;
        state_a_ = state_b_;
        state_b_ = previous_a;
        if (ima) {
            static_cast<VoxNativeSubDecoderIMAADPCM*>(sub_decoder_)
                ->GetState(static_cast<NativeSubDecoderIMAADPCMState*>(previous_a));
        } else {
            static_cast<VoxNativeSubDecoderPCM*>(sub_decoder_)
                ->GetState(static_cast<NativeSubDecoderPCMState*>(previous_a));
        }
        frames_before_ = frames_current_;
        frames_current_ = 0;
    }
    const int state_index = GetStateIndex();
    if (state_index < 0) {
        if (decode_calls_ == 0) {
            const int base_state = 0;
            sub_decoder_->SetState(base_state);
        }
    } else {
        sub_decoder_->SetState(state_index);
        frames_mark_ = 0;
    }
    const int result = sub_decoder_->Decode(dst, count);
    if (decode_calls_ != 0) {
        frames_before_ += result;
        frames_current_ += result;
        frames_mark_ += result;
    }
    ++decode_calls_;
    return result;
}

// 0x8710a8. The sub-decoder's HasData slot (+0x08).
bool DecoderNativeCursor::HasData() {
    return sub_decoder_ != nullptr && sub_decoder_->HasData();
}

// 0x8718cc. With a sub-decoder, calls its Reset (slot +0x10), clears the four counters and
// empties the pending state list under the lock. Without a sub-decoder nothing happens.
void DecoderNativeCursor::Reset() {
    if (sub_decoder_ == nullptr) {
        return;
    }
    if (IsImaAdpcm()) {
        static_cast<VoxNativeSubDecoderIMAADPCM*>(sub_decoder_)->Reset();
    } else {
        static_cast<VoxNativeSubDecoderPCM*>(sub_decoder_)->Reset();
    }
    frames_mark_ = 0;
    decode_calls_ = 0;
    frames_before_ = 0;
    frames_current_ = 0;
    mutex_.Lock();
    state_indices_.clear();
    mutex_.Unlock();
}

// 0x8710d4. Returns how many frames can be rewound. The mark is compared against the
// current position first, then against the earlier frames.
int DecoderNativeCursor::GetRewindLimit() {
    if (frames_current_ > frames_mark_) {
        return 0;
    }
    if (frames_mark_ >= frames_before_) {
        return frames_before_;
    }
    return frames_current_;
}

// 0x8720b8. With a sub-decoder, a rewind into the earlier frames restores the older saved
// state and a rewind inside the current frames restores the newer one. The sub-decoder then
// emulates the remaining frames. A request past the earlier frames does nothing.
void DecoderNativeCursor::Rewind(int frames) {
    if (sub_decoder_ == nullptr) {
        return;
    }
    const bool ima = IsImaAdpcm();
    int remaining = 0;
    NativeSubDecoderState* saved = nullptr;
    if (frames_current_ < frames) {
        if (frames > frames_before_) {
            return;
        }
        saved = state_a_;
        remaining = frames_before_ - frames;
    } else {
        saved = state_b_;
        remaining = frames_current_ - frames;
    }
    if (ima) {
        static_cast<VoxNativeSubDecoderIMAADPCM*>(sub_decoder_)
            ->SetState(static_cast<NativeSubDecoderIMAADPCMState*>(saved));
    } else {
        static_cast<VoxNativeSubDecoderPCM*>(sub_decoder_)
            ->SetState(static_cast<NativeSubDecoderPCMState*>(saved));
    }
    if (remaining > 0) {
        sub_decoder_->EmulateDecode(remaining);
    }
}

// 0x8710fc. Returns the sub-decoder's Seek result (slot +0x0c), or -1 with no sub-decoder.
int DecoderNativeCursor::Seek(unsigned int frame) {
    if (sub_decoder_ == nullptr) {
        return -1;
    }
    if (IsImaAdpcm()) {
        return static_cast<VoxNativeSubDecoderIMAADPCM*>(sub_decoder_)->Seek(frame);
    }
    return static_cast<VoxNativeSubDecoderPCM*>(sub_decoder_)->Seek(frame);
}

// 0x871128. The original body is empty.
void DecoderNativeCursor::SetLoop(bool on) {
    (void)on;
}

// 0x872cf0 (partial, not reproduced). The segment cue loop depends on the 24-byte
// record layout and on a goto ordering that the decompiler output does not make clear
// (the same value is pushed twice on one path). Left empty until that is traced.
void DecoderNativeCursor::SetImplicitSegmentCues() {
}

// 0x873144. Looks up the state name and appends its index to the pending list.
void DecoderNativeCursor::SetInteractiveMusicState(const char* name) {
    mutex_.Lock();
    if (state_names_ != nullptr && name != nullptr) {
        const auto found = state_names_->find(std::string(name));
        if (found != state_names_->end()) {
            state_indices_.push_back(found->second);
        }
    }
    mutex_.Unlock();
}

// 0x8738bc (partial). Reads the 8-byte header through the stream, rewinding it first when
// it is not at offset 0, and checks the "VoxN" magic. The chunk loop that follows
// ('Afmt', 'Segm', 'Cues', 'Grpe', 'Rule', 'Plst', 'Stat' and the end chunk) is not
// ported, so a file that passes the magic check still returns false here.
bool DecoderNativeCursor::ParseFile() {
    if (stream_ == nullptr) {
        return false;
    }
    if (stream_->Tell() != 0) {
        stream_->Seek(0, 0);
    }
    std::uint8_t header[8];
    if (stream_->Read(header, static_cast<int>(sizeof(header))) != static_cast<int>(sizeof(header))) {
        return false;
    }
    std::uint32_t magic = 0;
    std::memcpy(&magic, header, sizeof(magic));
    if (magic != kNativeMagic) {
        return false;
    }
    return false;
}

// 0x871954. Pops the newest pending state index, or returns -1 when the list is empty.
int DecoderNativeCursor::GetStateIndex() {
    mutex_.Lock();
    if (state_indices_.empty()) {
        mutex_.Unlock();
        return -1;
    }
    const std::int32_t index = state_indices_.back();
    state_indices_.pop_back();
    mutex_.Unlock();
    return index;
}

}  // namespace vox
