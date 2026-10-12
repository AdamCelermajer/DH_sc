// vox::NativeSubDecoderState: the base constructor and destructors. The PCM and IMA ADPCM
// variants call these from their own constructors and destructors.
#include <new>

#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder_state.hpp"

namespace vox {

// 0x8860dc _ZN3vox21NativeSubDecoderStateC1EPNS_22NativePlaylistsManagerE (partial)
// C2 0x88621c has the same body. The original builds the playlist manager as a copy of the
// argument (the manager copy constructor, in the playlists category). A null argument,
// which the original does not allow, builds a default manager here. Partial: the original
// also stores about 50 non-zero defaults at its own offsets (words from +0x84 to +0x130,
// and bytes at +0xac, +0xf0 and +0x134). The port's layout differs, so those stores are not
// reproduced; the member initializers in native_sub_decoder_state.hpp carry the zero
// defaults only.
NativeSubDecoderState::NativeSubDecoderState(NativePlaylistsManager* manager) {
    void* memory = VoxAlloc(sizeof(NativePlaylistsManager));
    if (manager != nullptr) {
        playlists_ = new (memory) NativePlaylistsManager(*manager);
    } else {
        playlists_ = new (memory) NativePlaylistsManager();
    }
    for (SegmentState& segment : segments_) {
        segment.SetStopped();
    }
}

// 0x88547c _ZN3vox21NativeSubDecoderStateD1Ev
// D2 0x8854e8 has the same body. The manager is destroyed and freed.
NativeSubDecoderState::~NativeSubDecoderState() {
    if (playlists_ != nullptr) {
        playlists_->~NativePlaylistsManager();
        VoxFree(playlists_);
        playlists_ = nullptr;
    }
}

// 0x8854cc _ZN3vox21NativeSubDecoderStateD0Ev
// Deleting destructor: compiler-generated for the class above (it calls D1 and then frees
// the object). Not written by hand.

}  // namespace vox
