// NativePlaylist state functions: GetState (reconstructed), Reset, SetToPreviousState
// and the SetState overloads (partial). The partial bodies copy the playlist's own
// words and leave out the per-group virtual calls, which are not ported.
#include "vox-audio/vox/native_playlist.hpp"

namespace vox {

// 0x881f04 _ZN3vox14NativePlaylist8GetStateEPNS_13PlaylistStateE
// The original returns the this pointer in r0; callers do not use it, so it is void here.
void NativePlaylist::GetState(PlaylistState* state) const {
    state->group_index = current_group_;          // +0x0c
    state->element_index = current_;              // +0x10
    state->remaining_repeats = field_14_;         // +0x14
    state->previous_group = previous_group_;      // +0x18
    state->previous_element = previous_element_;  // +0x1c
    state->saved_repeats = saved_repeats_;        // +0x20
    state->groups = &groups_;                     // +0x24 (pointer to the group vector)
}

// 0x8822e8 _ZN3vox14NativePlaylist5ResetEv (partial)
// Missing: the original then calls the group virtual at slot +0x10 with argument 0 for
// each group in groups_ (not ported). The state words are reset here.
void NativePlaylist::Reset() {
    previous_group_ = current_group_;     // +0x18 = +0x0c
    previous_element_ = current_;         // +0x1c = +0x10
    saved_repeats_ = total_repeats_;      // +0x20 = +0x08
    current_group_ = 0;                   // +0x0c
    current_ = 0;                         // +0x10
    field_14_ = total_repeats_;           // +0x14 = +0x08
}

// 0x882360 _ZN3vox14NativePlaylist8SetStateERS0_ (partial)
// The original copies the nine words from +0x00 to +0x20. Missing: for each group the
// original calls GetSelectMode and then the group virtual at slot +0x14 with the matching
// group of other (not ported). The group vector is not touched here.
void NativePlaylist::SetState(const NativePlaylist& other) {
    valid_ = other.valid_;                        // +0x00
    field_04_ = other.field_04_;                  // +0x04
    total_repeats_ = other.total_repeats_;        // +0x08
    current_group_ = other.current_group_;        // +0x0c
    current_ = other.current_;                    // +0x10
    field_14_ = other.field_14_;                  // +0x14
    previous_group_ = other.previous_group_;      // +0x18
    previous_element_ = other.previous_element_;  // +0x1c
    saved_repeats_ = other.saved_repeats_;        // +0x20
}

// 0x882ae0 _ZN3vox14NativePlaylist8SetStateEPNS_13PlaylistStateE (partial)
// The original copies the six state words, then for each group calls the group virtuals
// at slots +0x1c (GetState into a local record) and +0x24 (SetState from it) through the
// state's group vector, and throws out_of_range when the state has fewer groups. Those
// virtuals are not ported, so the group loop is missing.
void NativePlaylist::SetState(const PlaylistState* state) {
    current_group_ = state->group_index;          // +0x0c
    current_ = state->element_index;              // +0x10
    field_14_ = state->remaining_repeats;         // +0x14
    previous_group_ = state->previous_group;      // +0x18
    previous_element_ = state->previous_element;  // +0x1c
    saved_repeats_ = state->saved_repeats;        // +0x20
}

// 0x88240c _ZN3vox14NativePlaylist18SetToPreviousStateEv (partial)
// Missing: the original first calls the group virtual at slot +0x18 on the group at
// previous_group_ (not ported), then restores the words below.
void NativePlaylist::SetToPreviousState() {
    current_group_ = previous_group_;     // +0x0c = +0x18
    current_ = previous_element_;         // +0x10 = +0x1c
    field_14_ = saved_repeats_;           // +0x14 = +0x20
}

}  // namespace vox
