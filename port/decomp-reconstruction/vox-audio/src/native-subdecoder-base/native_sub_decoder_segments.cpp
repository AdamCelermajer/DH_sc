// VoxNativeSubDecoder segment bookkeeping (base class), wave 9. The control flow follows
// the Hex-Rays bodies; the word offsets in each comment are the ARM offsets from the
// pseudocode (`this[n]` is +4n). The functions that need a playlist element lookup, the
// rule table or a container that is not ported are partial, and each comment says what is
// missing. Nothing in this file calls the playlist lookups that return an element pointer.
#include <cstring>

#include "vox-audio/vox/native_sub_decoder.hpp"

#include "vox-audio/vox/native_sub_decoder_state.hpp"

namespace vox {

namespace {
// ARM sdiv returns 0 for a zero divisor; the reconstruction keeps that result.
int DivArm(int numerator, int denominator) {
    return denominator == 0 ? 0 : numerator / denominator;
}
}  // namespace

// The record that StopSegment leaves and the state constructor starts from. The loop-list
// words entry_count and entries_left start at 1, as in the original defaults.
void SegmentState::SetStopped() {
    chunk_index = -1;
    play_mode = 0;
    byte_offset = 0;
    sample_position = 0;
    loop_start = 0;
    end_frame = 0;
    entry_count = 1;
    entries_left = 1;
    restart_at_end = 0;
    state = 2;
    remaining_frames = 0;
    field_2c = 0;
    ramp_frames = 0;
    ramp_step = 0;
    gain = 0;
    decoding_slot = -1;
    reset_pending = false;
}

// 0x8842e8 _ZN3vox19VoxNativeSubDecoder26EmulateMixMultipleSegmentsEi
// Slot 0x34 is EmulateDecodeSegment, slot 0x30 is EmulateDecodeCurrentSegmentWithOffset.
int VoxNativeSubDecoder::EmulateMixMultipleSegments(int bytes) {
    int frames_a = 0;
    if (segments_[0].state > 2) {
        frames_a = EmulateDecodeSegment(bytes, &segments_[0]);
        EmulateMixSegmentInBuffer(frames_a, &segments_[0]);
    }
    if (segments_[1].state > 2) {
        const int frames_b = EmulateDecodeSegment(bytes, &segments_[1]);
        if (frames_a < frames_b) {
            frames_a = frames_b;
        }
        EmulateMixSegmentInBuffer(frames_b, &segments_[1]);
    }
    const int frames_c = EmulateDecodeCurrentSegmentWithOffset(bytes);
    EmulateMixSegmentInBuffer(frames_c, &segments_[2]);
    return frames_c < frames_a ? frames_a : frames_c;
}

// 0x8843b8 _ZN3vox19VoxNativeSubDecoder28GetNextDyingSegmentLifeStateEv
int VoxNativeSubDecoder::GetNextDyingSegmentLifeState() {
    if (active_segments_ <= 1) {
        return 2;
    }
    if (prev_element_.field_04_ == 1) {
        if (field_3c_ != field_38_ && segments_[2].gain < segments_[1].gain) {
            return 1;
        }
        return 2;
    }
    // Element list of the dying segment: entry [1] of its vector<int> (the original reads
    // the second word of the list at segment C's index).
    if (prev_element_.field_08_ != 1 ||
        segments_[2].sample_position >= (*loop_lists_)[segments_[2].chunk_index][1]) {
        return 2;
    }
    return 1;
}

// 0x884470 _ZN3vox19VoxNativeSubDecoder20IsExtraSegmentNeededEPNS_14TransitionRuleE
bool VoxNativeSubDecoder::IsExtraSegmentNeeded(TransitionRule* rule) {
    if (active_segments_ <= 0) {
        return active_segments_ == 0;
    }
    if (rule != nullptr) {
        if (rule->type_ == 0) {
            return rule->length_ > 0.0f;
        }
        return true;
    }
    if (prev_element_.field_0c_ == 1) {
        return true;
    }
    return current_element_.field_08_ == 1;
}

// 0x8844e8 _ZN3vox19VoxNativeSubDecoder18MixSegmentInBufferEPsiPNS_12SegmentStateE
// Adds the decoded frames of one segment into the 32-bit mixing buffer, with the gain ramp.
// The return value is the last value the original leaves in its result register (the ramp
// step, a quotient, or a mixed sample, depending on the path taken); callers ignore it.
int VoxNativeSubDecoder::MixSegmentInBuffer(std::int16_t* src, int bytes, SegmentState* segment) {
    auto* mix = static_cast<std::int32_t*>(s_pMixingBuffer);
    const int channels = channels_;
    int frames = DivArm(bytes, channels * (static_cast<int>(bits_) >> 3));  // v10
    int remaining = segment->remaining_frames;                             // v9
    const int gain_in = segment->gain;                                     // v11
    int ramp_step = segment->ramp_step;                                    // v28 (v12)
    int ramp_frames = segment->ramp_frames;                                // v13
    int gain = gain_in;
    int result = 0;

    const int after = remaining - frames;
    segment->remaining_frames = after < 0 ? 0 : after;

    if (remaining > 0) {
        if (remaining >= frames) {
            remaining = frames;
        }
        const int samples = channels * remaining;  // v14
        if (ramp_step < 0) {
            // A falling ramp mixes the decoded samples straight in.
            for (int k = 0; k < samples; ++k) {
                mix[k] += src[k];
            }
            src += samples;
            mix += samples;
        } else {
            // Otherwise the first samples are skipped and the mix pointer restarts at the base.
            src += samples;
            mix = static_cast<std::int32_t*>(s_pMixingBuffer) + samples;
        }
        frames -= remaining;
    }

    result = channels * frames;  // v30
    const int total_samples = result;
    int ramp_samples = ramp_frames * channels;  // v16
    if (ramp_frames > 0) {
        int ramp_used;  // v32
        if (frames < ramp_frames) {
            ramp_used = frames;
            if (segment->play_mode == 3) {
                result = DivArm(-gain_in, frames);
                ramp_samples = channels * frames;
                ramp_step = result;
            } else {
                ramp_samples = channels * frames;
            }
        } else {
            ramp_used = ramp_frames;
        }
        if (ramp_samples > 0) {
            // Each sample is scaled by the gain; the gain steps once per completed frame.
            for (int i = 0; i < ramp_samples;) {
                const std::int32_t scaled = static_cast<std::int32_t>(src[i]) * (gain >> 15);
                mix[i] += scaled >> 15;
                ++i;
                result = DivArm(i, channels);
                if (i % channels == 0) {
                    gain += ramp_step;
                }
            }
            mix += ramp_samples;
            src += ramp_samples;
        }
        frames = ramp_used;
        const int left = segment->ramp_frames;
        segment->gain = gain;
        const int next_ramp = left - ramp_used;
        segment->ramp_frames = next_ramp < 0 ? 0 : next_ramp;
        ramp_frames = segment->ramp_frames;
    } else {
        ramp_frames = segment->ramp_frames;
    }

    if (ramp_frames == 0) {
        result = ramp_step;
        segment->remaining_frames = 0;
        segment->field_2c = 0;
        if (ramp_step < 0) {
            frames = 1;
        }
        segment->ramp_frames = 0;
        segment->ramp_step = 0;
        segment->gain = 0;
        if (ramp_step >= 0) {
            // The rest of the block is mixed at unity gain.
            int sample = ramp_samples;
            for (int j = 0; sample < total_samples; ++sample, ++j) {
                result = mix[j];
                mix[j] = result + src[j];
            }
        } else {
            segment->state = frames;
        }
    }
    if (segment->play_mode == 3) {
        segment->state = 1;
    }
    return result;
}

// 0x884734 _ZN3vox19VoxNativeSubDecoder11StopSegmentEPNS_12SegmentStateE
// Mode 17 first releases the segment's decoding buffer through slot 0x24.
void VoxNativeSubDecoder::StopSegment(SegmentState* segment) {
    if (mode_ == 17) {
        ReleaseDecodingBuffer(segment->decoding_slot);
    }
    if (segment->state == 1) {
        --active_segments_;
    }
    if (segment->play_mode == 1) {
        has_data_ = false;
    }
    segment->chunk_index = -1;
    segment->gain = 0;
    segment->entries_left = 1;
    segment->state = 2;
    segment->play_mode = 0;
    segment->byte_offset = 0;
    segment->sample_position = 0;
    segment->loop_start = 0;
    segment->end_frame = 0;
    segment->entry_count = 1;
    segment->restart_at_end = 0;
    segment->remaining_frames = 0;
    segment->field_2c = 0;
    segment->ramp_frames = 0;
    segment->ramp_step = 0;
}

// 0x884bc4 _ZN3vox19VoxNativeSubDecoder25UpdateCurrentSegmentStateEPNS_14TransitionRuleEb (partial)
// Reproduced: the branch where the current element index is -1 and the no-rule tail. Not
// reproduced: the element branch (index not -1), the rule branch, which needs the ARM
// results of slots 0x1c and 0x20 and the vector copy of the element list, and the frame ramp.
int VoxNativeSubDecoder::UpdateCurrentSegmentState(TransitionRule* rule, bool extra) {
    (void)extra;
    if (current_element_.field_00_ != -1) {
        return 0;
    }
    segments_[2].entries_left = 1;
    segments_[2].state = 4;
    segments_[2].entry_count = 1;
    next_event_ = -1;
    if (rule != nullptr) {
        return 0;
    }
    segments_[2].ramp_frames = 0;
    segments_[2].ramp_step = 0;
    segments_[2].gain = 0;
    segments_[2].field_2c = 0;
    segments_[2].remaining_frames = 0;
    return 0;
}

// 0x884f3c _ZN3vox19VoxNativeSubDecoder8SetStateEPNS_21NativeSubDecoderStateE
// Copies the playlist manager and every saved word, record and element.
void VoxNativeSubDecoder::SetState(NativeSubDecoderState* state) {
    if (playlists_ != nullptr && state->playlists_ != nullptr) {
        playlists_->SetState(*state->playlists_);
    }
    field_34_ = state->field_08_;
    field_38_ = state->field_0c_;
    field_3c_ = state->field_10_;
    field_40_ = state->field_14_;
    field_44_ = state->field_18_;
    field_48_ = state->field_1c_;
    reset_pending_ = state->field_20_;
    prev_prev_element_ = state->element_a_;
    prev_element_ = state->element_b_;
    current_element_ = state->element_c_;
    field_8c_ = state->field_60_;
    field_90_ = state->field_64_;
    active_segments_ = state->field_68_;
    segments_[0] = state->segments_[0];
    segments_[1] = state->segments_[1];
    segments_[2] = state->segments_[2];
    next_event_ = state->field_138_;
    pending_seek_ = state->field_13c_;
    has_data_ = state->has_data_;
}

// 0x88506c _ZN3vox19VoxNativeSubDecoder8GetStateEPNS_21NativeSubDecoderStateE
// The reverse of SetState.
void VoxNativeSubDecoder::GetState(NativeSubDecoderState* state) {
    if (state->playlists_ != nullptr && playlists_ != nullptr) {
        state->playlists_->SetState(*playlists_);
    }
    state->field_08_ = field_34_;
    state->field_0c_ = field_38_;
    state->field_10_ = field_3c_;
    state->field_14_ = field_40_;
    state->field_18_ = field_44_;
    state->field_1c_ = field_48_;
    state->field_20_ = reset_pending_;
    state->element_a_ = prev_prev_element_;
    state->element_b_ = prev_element_;
    state->element_c_ = current_element_;
    state->field_60_ = field_8c_;
    state->field_64_ = field_90_;
    state->field_68_ = active_segments_;
    state->segments_[0] = segments_[0];
    state->segments_[1] = segments_[1];
    state->segments_[2] = segments_[2];
    state->field_138_ = next_event_;
    state->field_13c_ = pending_seek_;
    state->has_data_ = has_data_;
}

// 0x885538 _ZN3vox19VoxNativeSubDecoder25SwapOldAndCurrentSegmentsEv
// The second and third records swap. The third becomes kind 1 and the second kind 2.
void VoxNativeSubDecoder::SwapOldAndCurrentSegments() {
    const SegmentState old_third = segments_[2];
    segments_[2] = segments_[1];
    segments_[1] = old_third;
    segments_[2].play_mode = 1;
    segments_[1].play_mode = 2;
}

// 0x8855e8 _ZN3vox19VoxNativeSubDecoder19ApplyTransitionRuleEPNS_14TransitionRuleE (partial)
// Reproduced: the kind-1 branch with the next-dying test, the swap, and the frame count
// check that clears the old records. Not reproduced: the playlist lookups
// (GetPlaylistElement, TransposePlaylistParameters, SetPlaylistToPreviousState,
// ResetPlaylist) and the copy of the found element into the current element record.
void VoxNativeSubDecoder::ApplyTransitionRule(TransitionRule* rule) {
    const int next_dying = GetNextDyingSegmentLifeState();
    if (rule->kind_ == 1) {
        if (next_dying != 2) {
            SwapOldAndCurrentSegments();
        }
        const int frames = static_cast<int>(static_cast<float>(field_0c_) * rule->length_);
        if (frames <= 0) {
            if (segments_[1].state > 2) {
                segments_[1].state = 0;
                --active_segments_;
            }
            if (segments_[0].state > 2) {
                segments_[0].state = 0;
                --active_segments_;
            }
        }
    }
    current_element_.field_00_ = -1;
}

// 0x885740 _ZN3vox19VoxNativeSubDecoder20UpdateSegmentsStatesEv (partial)
// Reproduced: the history shift of the element words and the records. Not reproduced: the
// element lookup (GetPlaylistElement) that the original makes when the two index words
// differ, the transition rule branch, and the peek of the next element.
void VoxNativeSubDecoder::UpdateSegmentsStates() {
    if (field_3c_ == field_38_ || field_38_ < 0) {
        current_element_.field_00_ = -1;
        UpdateCurrentSegmentState(nullptr, false);
    }
    field_34_ = field_38_;
    field_38_ = field_3c_;
    field_40_ = field_44_;
    field_44_ = field_48_;
    prev_prev_element_ = prev_element_;
    prev_element_ = current_element_;
    field_8c_ = field_90_;
}

// 0x885984 _ZN3vox19VoxNativeSubDecoder23InterpretTransitionRuleEi (partial)
// Not reproduced: the rule table and the playlist peek are not ported.
void VoxNativeSubDecoder::InterpretTransitionRule(int index) {
    (void)index;
}

// 0x885a3c _ZN3vox19VoxNativeSubDecoder8SetStateEi (partial)
// Reproduced: the store of the index and the dispatch on the stored element. Not
// reproduced: the list lookups that fill the element and the state word.
void VoxNativeSubDecoder::SetState(int index) {
    field_3c_ = index;
    if (field_90_ >= 0) {
        InterpretTransitionRule(field_90_);
    } else if (field_38_ == -1) {
        UpdateSegmentsStates();
    }
}

// The stop block shared by EmulateDecode and Decode: a segment whose state is 0 or 1 is
// stopped, in A, B, C order (the original tests each state in that order).
namespace {
void StopIdleSegments(VoxNativeSubDecoder* sub, SegmentState* segments) {
    for (int i = 0; i < 3; ++i) {
        if (segments[i].state <= 1) {
            sub->StopSegment(&segments[i]);
        }
    }
}
}  // namespace

// 0x885ac0 _ZN3vox19VoxNativeSubDecoder13EmulateDecodeEi
int VoxNativeSubDecoder::EmulateDecode(int bytes) {
    const int frame_bytes = static_cast<int>(channels_) * (static_cast<int>(bits_) >> 3);
    const int aligned = bytes - bytes % frame_bytes;
    const int next = next_event_;
    if (next >= 0) {
        const int offset = segments_[2].sample_position;
        if (next <= aligned / frame_bytes + offset) {
            pending_seek_ = next - offset;
            UpdateSegmentsStates();
        }
    }
    int result = 0;
    if (active_segments_ > 1) {
        result = EmulateMixMultipleSegments(aligned);
    } else if (active_segments_ == 1) {
        if (segments_[2].ramp_frames > 0) {
            result = EmulateMixMultipleSegments(aligned);
        } else {
            result = EmulateDecodeSegment(aligned, &segments_[2]);
        }
    }
    StopIdleSegments(this, segments_);
    if (mode_ == 17) {
        if (segments_[0].state == 3) {
            segments_[0].reset_pending = true;
        }
        if (segments_[1].state == 3) {
            segments_[1].reset_pending = true;
        }
        if (segments_[2].state == 3) {
            segments_[2].reset_pending = true;
        }
    }
    return result;
}

// 0x885c1c _ZN3vox19VoxNativeSubDecoder6DecodeEPvi
int VoxNativeSubDecoder::Decode(void* dst, int bytes) {
    const int frame_bytes = static_cast<int>(channels_) * (static_cast<int>(bits_) >> 3);
    const int aligned = bytes - bytes % frame_bytes;
    const int next = next_event_;
    if (next >= 0) {
        const int offset = segments_[2].sample_position;
        if (next <= aligned / frame_bytes + offset) {
            pending_seek_ = next - offset;
            UpdateSegmentsStates();
        }
    }
    int result = 0;
    if (active_segments_ > 1) {
        result = MixMultipleSegments(static_cast<std::int16_t*>(dst), aligned);
    } else if (active_segments_ == 1) {
        if (segments_[2].ramp_frames > 0) {
            result = MixMultipleSegments(static_cast<std::int16_t*>(dst), aligned);
        } else {
            result = DecodeSegment(dst, aligned, &segments_[2]);
        }
    }
    StopIdleSegments(this, segments_);
    return result;
}

}  // namespace vox
