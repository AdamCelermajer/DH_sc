// VoxNativeSubDecoder segment bookkeeping (base class). The functions that need a
// NativePlaylistsManager lookup or a result from a virtual slot that is not named yet are
// partial, and each comment says what is missing. Nothing in this file calls the playlist
// manager, so the host test can link it on its own.
#include "vox-audio/vox/native_sub_decoder.hpp"

#include "vox-audio/vox/native_sub_decoder_state.hpp"

namespace vox {

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

// 0x8842e8 _ZN3vox19VoxNativeSubDecoder26EmulateMixMultipleSegmentsEi (partial)
// The decode calls through virtual slots 0x30 and 0x34 are not ported, so their frame
// counts are taken as 0. The mixing control flow and the final maximum are kept.
int VoxNativeSubDecoder::EmulateMixMultipleSegments(int bytes) {
    (void)bytes;
    int frames_a = 0;  // slot 0x34 on segment A: not ported
    if (segments_[0].state > 2) {
        EmulateMixSegmentInBuffer(frames_a, &segments_[0]);
    }
    int frames_max = frames_a;
    if (segments_[1].state > 2) {
        int frames_b = 0;  // slot 0x34 on segment B: not ported
        if (frames_max < frames_b) {
            frames_max = frames_b;
        }
        EmulateMixSegmentInBuffer(frames_b, &segments_[1]);
    }
    int frames_c = 0;  // slot 0x30 on segment C: not ported
    EmulateMixSegmentInBuffer(frames_c, &segments_[2]);
    return frames_c < frames_max ? frames_max : frames_c;
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

// 0x8844e8 _ZN3vox19VoxNativeSubDecoder18MixSegmentInBufferEPsiPNS_12SegmentStateE (partial)
// Reproduced: the frame count from the byte count, the remaining-frame update (clamped at
// zero), and the state set for kind 3. Not reproduced: the mixing loops over the mixing
// buffer, the gain ramp, and the return value.
int VoxNativeSubDecoder::MixSegmentInBuffer(std::int16_t* src, int bytes, SegmentState* segment) {
    (void)src;
    const int frames = bytes / (static_cast<int>(channels_) * (static_cast<int>(bits_) >> 3));
    const int left = segment->remaining_frames - frames;
    segment->remaining_frames = left < 0 ? 0 : left;
    if (segment->play_mode == 3) {
        segment->state = 1;
    }
    return 0;
}

// 0x884734 _ZN3vox19VoxNativeSubDecoder11StopSegmentEPNS_12SegmentStateE (partial)
// Not reproduced: for mode 17 the original first calls virtual slot 0x24 with the
// segment's buffer id. The rest of the stop is kept.
void VoxNativeSubDecoder::StopSegment(SegmentState* segment) {
    if (segment->state == 1) {
        --active_segments_;
    }
    if (segment->play_mode == 1) {
        has_data_ = false;
    }
    segment->chunk_index = -1;
    segment->play_mode = 0;
    segment->byte_offset = 0;
    segment->sample_position = 0;
    segment->loop_start = 0;
    segment->end_frame = 0;
    segment->entry_count = 1;
    segment->entries_left = 1;
    segment->restart_at_end = 0;
    segment->state = 2;
    segment->remaining_frames = 0;
    segment->field_2c = 0;
    segment->ramp_frames = 0;
    segment->ramp_step = 0;
    segment->gain = 0;
}

// 0x884bc4 _ZN3vox19VoxNativeSubDecoder25UpdateCurrentSegmentStateEPNS_14TransitionRuleEb (partial)
// Reproduced: the branch where the current element index is -1, and its tail when there is
// no rule. Not reproduced: the rule branch, which needs the results of virtual slots 0x1c
// and 0x20, the vector copy of the element list, and the frame ramp.
int VoxNativeSubDecoder::UpdateCurrentSegmentState(TransitionRule* rule, bool extra) {
    (void)extra;
    if (current_element_.field_00_ != -1) {
        return 0;  // element branch: not ported
    }
    segments_[2].entries_left = 1;
    segments_[2].state = 4;
    segments_[2].entry_count = 1;
    next_event_ = -1;
    if (rule != nullptr) {
        return 0;  // rule tail: not ported
    }
    segments_[2].ramp_frames = 0;
    segments_[2].ramp_step = 0;
    segments_[2].gain = 0;
    segments_[2].field_2c = 0;
    segments_[2].remaining_frames = 0;
    return 0;
}

// 0x884f3c _ZN3vox19VoxNativeSubDecoder8SetStateEPNS_21NativeSubDecoderStateE (partial)
// Copies every saved word and record. The playlist manager's SetState is not ported and
// is not called.
void VoxNativeSubDecoder::SetState(NativeSubDecoderState* state) {
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

// 0x88506c _ZN3vox19VoxNativeSubDecoder8GetStateEPNS_21NativeSubDecoderStateE (partial)
// The reverse of SetState, with the same missing manager call.
void VoxNativeSubDecoder::GetState(NativeSubDecoderState* state) {
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
// Not reproduced: the playlist lookups (GetPlaylistElement, TransposePlaylistParameters,
// SetPlaylistToPreviousState) and the ResetPlaylist of a pending reset. With no element
// found, the current element index is set to -1, as in the original's no-element branch.
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
// Reproduced: the no-element branch and the shift of the element history. Not reproduced:
// the element lookup (GetPlaylistElement), the transition rule branch (ApplyTransitionRule
// needs the rule table) and the peek of the next element. The lookup is taken as absent
// when the two word-14 and word-15 counters match or are negative, as in the original.
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
// Reproduced: the store of the index, and the dispatch on the stored element. Not
// reproduced: the list lookups that fill the element and the state word.
void VoxNativeSubDecoder::SetState(int index) {
    field_3c_ = index;
    if (field_90_ >= 0) {
        InterpretTransitionRule(field_90_);
    } else if (field_38_ == -1) {
        UpdateSegmentsStates();
    }
}

// The stop block shared by EmulateDecode and Decode: a segment that is not playing (state
// 0 or 1) is stopped, in A, B, C order.
namespace {
void StopIdleSegments(VoxNativeSubDecoder* sub, SegmentState* segments) {
    for (int i = 0; i < 3; ++i) {
        if (segments[i].state <= 1) {
            sub->StopSegment(&segments[i]);
        }
    }
}
}  // namespace

// 0x885ac0 _ZN3vox19VoxNativeSubDecoder13EmulateDecodeEi (partial)
// Reproduced: the event position check, the choice between the mix and the stops, and the
// mode 17 flags. Not reproduced: the decode call through virtual slot 0x34 (taken as 0).
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
    } else if (active_segments_ == 1 && segments_[2].ramp_frames > 0) {
        result = EmulateMixMultipleSegments(aligned);
    }
    StopIdleSegments(this, segments_);
    if (mode_ == 17) {
        for (SegmentState& segment : segments_) {
            if (segment.state == 3) {
                segment.reset_pending = true;
            }
        }
    }
    return result;
}

// 0x885c1c _ZN3vox19VoxNativeSubDecoder6DecodeEPvi (partial)
// Reproduced: the event position check, the mix when two or more segments play (through
// the existing MixMultipleSegments) and the stops. Not reproduced: the decode call through
// virtual slot 0x18 for a single segment, which is taken as 0.
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
    } else if (active_segments_ == 1 && segments_[2].ramp_frames > 0) {
        result = MixMultipleSegments(static_cast<std::int16_t*>(dst), aligned);
    }
    StopIdleSegments(this, segments_);
    return result;
}

}  // namespace vox
