// Native playlists (vox::PlaylistElement, vox::NativePlaylist and
// vox::NativePlaylistsManager). The native segment decoder walks these lists.
// Only the members the reconstructed bodies read or write are named; the others
// are kept as field_NN_ with their offsets.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace vox {

class SegmentGroup;
struct GroupInfos;  // defined in groups.hpp

// Two-word record the playlist constructor reads (vox::PlaylistInfos). The words are
// not identified; word 0 goes to +0x04 and word 1 to +0x08, +0x14 and +0x20.
struct PlaylistInfos {
    std::int32_t words_[2] = {0, 0};
};

// Record the AddPlaylistElement functions read (vox::PlaylistElementInfos). Words 0 to
// 7 are read (word k at +0x04 * k in the original); the names are not identified.
struct PlaylistElementInfos {
    std::int32_t words_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

// Snapshot written by NativePlaylist::GetState and read by NativePlaylist::SetState.
// The original record is 28 bytes: six words, then the pointer to the group vector.
// Field names come from what the playlist code does with each slot.
struct PlaylistState {
    std::int32_t group_index = 0;        // +0x0c of the playlist (current group)
    std::int32_t element_index = 0;      // +0x10 (current element index)
    std::int32_t remaining_repeats = 0;  // +0x14
    std::int32_t previous_group = 0;     // +0x18
    std::int32_t previous_element = 0;   // +0x1c
    std::int32_t saved_repeats = 0;      // +0x20
    const std::vector<SegmentGroup*>* groups = nullptr;  // +0x24 (the group vector itself)
};

// 0x881834 C1, 0x881810 C2: five words (20 bytes in the 32-bit original).
class PlaylistElement {
public:
    PlaylistElement();  // C1 0x881834, C2 0x881810: the body is the same as Reset
    // C1 0x881884, C2 0x881858: memberwise copy of the five words.
    PlaylistElement(const PlaylistElement& other);
    void Reset();       // 0x8818b0: field_00 = -1, field_04 = 0, field_08 = 0, field_0c = 0, field_10 = 1

private:
    friend struct PlaylistElementTestAccess;
    friend class NativePlaylist;
    std::int32_t field_00_ = -1;  // +0x00
    std::int32_t field_04_ = 0;   // +0x04
    std::int32_t field_08_ = 0;   // +0x08
    std::int32_t field_0c_ = 0;   // +0x0c
    std::int32_t field_10_ = 1;   // +0x10
};

// One playlist, 60 bytes in the 32-bit original (VoxAlloc(0x3c) in AddPlaylist).
// Offsets: +0x00 valid, +0x04 word 0 of the PlaylistInfos, +0x08 total repeats,
// +0x0c current group, +0x10 current element, +0x14 remaining repeats, +0x18 and
// +0x1c the previous group and element, +0x20 saved repeats, +0x24..+0x2c the group
// vector, +0x30..+0x38 the element vector (see the port note on elements_ below).
class NativePlaylist {
public:
    NativePlaylist() = default;
    // C2 0x881e4c, C1 0x881e9c: the body is the same.
    explicit NativePlaylist(const PlaylistInfos* info);
    // C1 0x883cd8, C2 0x884024 (partial): copies the scalar words; the groups and
    // elements are not copied yet (see native_playlist_lifetime.cpp).
    NativePlaylist(const NativePlaylist& other);
    // D1 0x8825b8, D2 0x88278c (partial): deletes the groups; the element table is not freed.
    ~NativePlaylist();

    int GetCurrentElement() const;  // 0x881eec: elements_[current_]
    int GetCurrentPosition() const; // 0x881efc: current_
    bool IsValid() const;           // 0x88219c: the flag byte at +0x00
    // 0x881f40: the element id at index when the int flag at +0x14 is set and the index is
    // inside the table (+0x30 to +0x34); otherwise 0.
    int GetPlaylistElement(int index) const;
    // 0x881f04: copies the current indices, repeats and the group vector into state.
    void GetState(PlaylistState* state) const;

    // 0x881f78 (partial): the group walk calls group virtuals that are not ported.
    int GetPlaylistElement();
    // 0x8821a4 (partial): the group walk calls PeekAtNextGroupElement, not ported.
    int PeekAtNextElement();
    // 0x8822e8 (partial): the reset of the group virtuals (slot +0x10) is not ported.
    void Reset();
    // 0x882360 (partial): copies the scalar words; the per-group SetState is not ported.
    void SetState(const NativePlaylist& other);
    // 0x882ae0 (partial): copies the six state words; the per-group loop is not ported.
    void SetState(const PlaylistState* state);
    // 0x88240c (partial): restores the previous group and element; the group virtual
    // at slot +0x18 is not ported.
    void SetToPreviousState();
    // 0x8838c4 (partial): the sequential branch is ported; the element record and the
    // random branch are not.
    void AddPlaylistElement(const PlaylistElementInfos* info);
    // 0x883c04 (partial): the sequential group is built; the random group constructor
    // is not ported, so the random branch adds nothing.
    void AddGroup(const GroupInfos* info);

private:
    friend struct NativePlaylistTestAccess;
    bool valid_ = false;                  // +0x00
    std::int32_t field_04_ = 0;           // +0x04 (word 0 of PlaylistInfos; not identified)
    std::int32_t total_repeats_ = 0;      // +0x08 (word 1 of PlaylistInfos)
    std::int32_t current_group_ = 0;      // +0x0c
    int current_ = 0;                     // +0x10 (index into the element table)
    int field_14_ = 0;                    // +0x14: remaining repeats; nonzero keeps the playlist live
    std::int32_t previous_group_ = 0;     // +0x18
    std::int32_t previous_element_ = 0;   // +0x1c
    std::int32_t saved_repeats_ = 0;      // +0x20
    std::vector<SegmentGroup*> groups_;   // +0x24..+0x2c
    // +0x30 and +0x34 (the original's element vector). The port reads this table as int
    // ids, but the original's AddPlaylistElement and destructor store PlaylistElement*
    // in it. The owner is not identified, so the port does not free it.
    std::int32_t* elements_ = nullptr;      // +0x30
    std::int32_t* elements_end_ = nullptr;  // +0x34
};

// Manager for the playlists of one decoder. The original keeps the flag at +0x00,
// a field at +0x04 (initialised to 0; not identified), the count at +0x08 and the
// NativePlaylist pointer array at +0x0c.
class NativePlaylistsManager {
public:
    NativePlaylistsManager();                   // C2 0x882448 (C1 0x882464 has the same body)
    // C1 0x883eac, C2 0x883f68 (partial, depends on the NativePlaylist copy constructor).
    NativePlaylistsManager(const NativePlaylistsManager& other);
    // D1 0x8826b4, D2 0x882720 (partial, depends on the NativePlaylist destructor).
    ~NativePlaylistsManager();

    int GetNbPlaylists() const;                 // 0x882480: count_
    bool IsValid() const;                       // 0x8824c0: flag at +0x00

    // 0x882488 (partial): mode 0 calls NativePlaylist::GetPlaylistElement(), 1 the current
    // element, 2 the indexed element. Mode 0 depends on the partial playlist walk.
    int GetPlaylistElement(int index, int mode, int argument);
    // 0x8824c8: calls PeekAtNextElement on playlist index (not ported, partial).
    void PeekAtNextPlaylistElement(int index);
    // 0x8824d4: negative index does nothing; otherwise Reset the playlist (not ported).
    void ResetPlaylist(int index);
    // 0x8824e8: calls SetToPreviousState on playlist index (not ported, partial).
    void SetPlaylistToPreviousState(int index);
    // 0x8824f4 (partial): copies the flag and field_04; the playlist SetState is partial.
    void SetState(const NativePlaylistsManager& other);
    // 0x882568: allocates a playlist from info at index and counts it when it is built.
    void AddPlaylist(int index, const PlaylistInfos* info);
    // 0x8839c8 (partial): forwards to the playlist at info->words_[0].
    void AddPlaylistElement(const PlaylistElementInfos* info);
    // 0x883ca4 (partial): forwards to the playlist at info->unknown_00_ (GroupInfos word 0).
    void AddGroup(const GroupInfos* info);
    // 0x882c0c (partial): copies the state of playlist from to playlist to.
    void TransposePlaylistParameters(int from, int to);
    // 0x882548: allocates count * 4 bytes for the pointer array; a failed allocation
    // clears the flag. The count at +0x08 is not written here.
    void Init(int count);

private:
    friend struct NativePlaylistsManagerTestAccess;
    bool valid_ = true;                         // +0x00
    int field_04_ = 0;                          // +0x04 (not identified)
    int count_ = 0;                             // +0x08
    NativePlaylist** playlists_ = nullptr;      // +0x0c
};

}  // namespace vox
