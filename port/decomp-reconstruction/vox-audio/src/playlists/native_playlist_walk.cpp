// NativePlaylist walk and build functions. All four are partial: the walk functions need
// group virtuals that are not ported, and the element and random-group builders need
// record types that are not identified yet.
#include <new>

#include "vox-audio/vox/groups.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_playlist.hpp"

namespace vox {

// 0x881f78 _ZN3vox14NativePlaylist18GetPlaylistElementEv (partial)
// The original returns 0 when the remaining-repeats word is zero (done here). Otherwise
// it asks each group for its next element through the group virtual at slot +0x08 and
// advances the group index, repeating with the repeat counters. Those virtuals are not
// ported, so the non-zero path is missing and this returns 0.
int NativePlaylist::GetPlaylistElement() {
    if (field_14_ == 0) {
        return 0;
    }
    return 0;
}

// 0x8821a4 _ZN3vox14NativePlaylist17PeekAtNextElementEv (partial)
// The original returns 0 when the remaining-repeats word is zero (done here). Otherwise it
// peeks the next group element through the group virtual at slot +0x0c (PeekAtNextGroupElement,
// not on the port's SegmentGroup), then looks up the element id in the table. Missing.
int NativePlaylist::PeekAtNextElement() {
    if (field_14_ == 0) {
        return 0;
    }
    return 0;
}

// 0x8838c4 _ZN3vox14NativePlaylist18AddPlaylistElementEPNS_20PlaylistElementInfosE (partial)
// Missing: the original allocates a 0x14-byte PlaylistElement, copies words 3, 4, 5 and 6
// into it, and appends it to the element vector at +0x30. The port's element table is an
// int array (see native_playlist.hpp), so the record is not built. Missing too: the random
// branch (RandomGroup::AddElement takes a RandomGroupElement*, and the two-word record is
// not identified). The sequential branch and the group validity check are done.
void NativePlaylist::AddPlaylistElement(const PlaylistElementInfos* info) {
    SegmentGroup* group = groups_[static_cast<std::size_t>(info->words_[2])];
    if (group->GetSelectMode() == 0) {
        static_cast<SequentialGroup*>(group)->AddElement(info->words_[1]);
    }
    if (!group->IsValid()) {
        valid_ = false;
    }
}

// 0x883c04 _ZN3vox14NativePlaylist8AddGroupEPNS_10GroupInfosE (partial)
// The sequential branch allocates a SequentialGroup (0x38 bytes in the original), builds it
// from info with the playlist's reset flag (+0x04) and appends it to the group vector.
// Missing: the random branch (select mode nonzero). The original allocates 0x5c bytes and
// builds a RandomGroup, but the port's RandomGroup has no GroupInfos constructor, so the
// random branch adds nothing here.
void NativePlaylist::AddGroup(const GroupInfos* info) {
    if (info->select_mode_ != 0) {
        return;
    }
    void* memory = VoxAlloc(sizeof(SequentialGroup), kVoxMemHintDefault);
    if (memory == nullptr) {
        valid_ = false;
        return;
    }
    groups_.push_back(new (memory) SequentialGroup(info, field_04_));
}

}  // namespace vox
