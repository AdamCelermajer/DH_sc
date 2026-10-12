// NativePlaylist constructors and destructor. The PlaylistInfos constructor is
// reconstructed. The copy constructor and the destructor are partial: the group and
// element copies are not ported (see the comments in each body).
#include <vector>

#include "vox-audio/vox/groups.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_playlist.hpp"

namespace vox {

// 0x881e4c _ZN3vox14NativePlaylistC2EPNS_13PlaylistInfosE (C1 0x881e9c has the same body)
NativePlaylist::NativePlaylist(const PlaylistInfos* info) {
    valid_ = true;                          // +0x00 = 1
    field_04_ = info->words_[0];            // +0x04
    total_repeats_ = info->words_[1];       // +0x08
    current_group_ = 0;                     // +0x0c
    current_ = 0;                           // +0x10
    field_14_ = info->words_[1];            // +0x14 (remaining repeats)
    previous_group_ = 0;                    // +0x18
    previous_element_ = 0;                  // +0x1c
    saved_repeats_ = info->words_[1];       // +0x20
    // +0x24..+0x38: the group and element vectors start empty (default members).
}

// 0x883cd8 _ZN3vox14NativePlaylistC1ERS0_ (C2 0x884024 has the same body)
// Partial: the scalar words are copied. The groups are not copied: the original builds
// a group copy with the SegmentGroup copy constructors, which are not ported. The
// PlaylistElement table is not copied either, because the port reads that table as int
// ids (see elements_ in native_playlist.hpp).
NativePlaylist::NativePlaylist(const NativePlaylist& other)
    : valid_(true),
      field_04_(other.field_04_),
      total_repeats_(other.total_repeats_),
      current_group_(other.current_group_),
      current_(other.current_),
      field_14_(other.field_14_),
      previous_group_(other.previous_group_),
      previous_element_(other.previous_element_),
      saved_repeats_(other.saved_repeats_) {}

// 0x8825b8 _ZN3vox14NativePlaylistD1Ev (D2 0x88278c has the same body)
// Partial: each group is destroyed (virtual destructor, then VoxFree), as the original
// does. The element table at +0x30 is not freed: the original frees PlaylistElement*
// records there, but the port's table is an int array with no known owner, and tests
// place it on the stack.
NativePlaylist::~NativePlaylist() {
    for (SegmentGroup* group : groups_) {
        if (group != nullptr) {
            group->~SegmentGroup();
            VoxFree(group);
        }
    }
}

}  // namespace vox
