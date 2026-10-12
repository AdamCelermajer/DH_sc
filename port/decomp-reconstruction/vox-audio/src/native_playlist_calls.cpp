// Playlist manager functions that forward to NativePlaylist methods which are not
// ported yet (Reset, PeekAtNextElement, SetToPreviousState). They stay partial.
#include "vox-audio/vox/native_playlist.hpp"

namespace vox {

// 0x8824c8 _ZN3vox22NativePlaylistsManager25PeekAtNextPlaylistElementEi (partial)
void NativePlaylistsManager::PeekAtNextPlaylistElement(int index) {
    playlists_[index]->PeekAtNextElement();
}

// 0x8824d4 _ZN3vox22NativePlaylistsManager13ResetPlaylistEi (partial)
void NativePlaylistsManager::ResetPlaylist(int index) {
    if (index < 0) {
        return;
    }
    playlists_[index]->Reset();
}

// 0x8824e8 _ZN3vox22NativePlaylistsManager26SetPlaylistToPreviousStateEi (partial)
void NativePlaylistsManager::SetPlaylistToPreviousState(int index) {
    playlists_[index]->SetToPreviousState();
}

}  // namespace vox
