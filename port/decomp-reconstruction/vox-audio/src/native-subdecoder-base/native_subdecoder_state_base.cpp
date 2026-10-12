// vox::NativeSubDecoderState: the base constructor and destructors. The PCM and IMA ADPCM
// variants call these from their own constructors and destructors.
#include <new>

#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder_state.hpp"

namespace vox {

// 0x8860dc _ZN3vox21NativeSubDecoderStateC1EPNS_22NativePlaylistsManagerE (partial)
// C2 0x88621c has the same body. The original builds the playlist manager as a copy of the
// argument; the port's manager has no copy constructor, so a default manager is built and
// the argument is not used.
NativeSubDecoderState::NativeSubDecoderState(NativePlaylistsManager* manager) {
    (void)manager;
    void* memory = VoxAlloc(sizeof(NativePlaylistsManager));
    playlists_ = new (memory) NativePlaylistsManager();
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
