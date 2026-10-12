// Native playlists (vox::PlaylistElement, vox::NativePlaylist and
// vox::NativePlaylistsManager). The native segment decoder walks these lists.
// Only the members the reconstructed bodies read or write are named; the others
// are kept as field_NN_ with their offsets.
#pragma once

#include <cstdint>

namespace vox {

// 0x881834 C1, 0x881810 C2: five words (20 bytes in the 32-bit original).
class PlaylistElement {
public:
    PlaylistElement();  // C1 0x881834, C2 0x881810: the body is the same as Reset
    void Reset();       // 0x8818b0: field_00 = -1, field_04 = 0, field_08 = 0, field_0c = 0, field_10 = 1

private:
    friend struct PlaylistElementTestAccess;
    std::int32_t field_00_ = -1;  // +0x00
    std::int32_t field_04_ = 0;   // +0x04
    std::int32_t field_08_ = 0;   // +0x08
    std::int32_t field_0c_ = 0;   // +0x0c
    std::int32_t field_10_ = 1;   // +0x10
};

// One playlist: a flag word at +0x00, the current position at +0x10 and the element
// id table at +0x30. Reset, PeekAtNextElement and SetToPreviousState are not ported
// yet, so the manager functions that call them stay partial.
class NativePlaylist {
public:
    int GetCurrentElement() const;  // 0x881eec: elements_[current_]
    int GetCurrentPosition() const; // 0x881efc: current_
    bool IsValid() const;           // 0x88219c: the flag byte at +0x00

    void Reset();                   // not ported (declared for the manager calls)
    void PeekAtNextElement();       // not ported
    void SetToPreviousState();      // not ported

private:
    friend struct NativePlaylistTestAccess;
    bool valid_ = false;            // +0x00
    int current_ = 0;               // +0x10
    std::int32_t* elements_ = nullptr;  // +0x30
};

// Manager for the playlists of one decoder. The original keeps the flag at +0x00,
// a field at +0x04 (initialised to 0; not identified), the count at +0x08 and the
// NativePlaylist pointer array at +0x0c.
class NativePlaylistsManager {
public:
    NativePlaylistsManager();                   // C2 0x882448 (C1 0x882464 has the same body)
    int GetNbPlaylists() const;                 // 0x882480: count_
    bool IsValid() const;                       // 0x8824c0: flag at +0x00

    // 0x8824c8: calls PeekAtNextElement on playlist index (not ported, partial).
    void PeekAtNextPlaylistElement(int index);
    // 0x8824d4: negative index does nothing; otherwise Reset the playlist (not ported).
    void ResetPlaylist(int index);
    // 0x8824e8: calls SetToPreviousState on playlist index (not ported, partial).
    void SetPlaylistToPreviousState(int index);
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
