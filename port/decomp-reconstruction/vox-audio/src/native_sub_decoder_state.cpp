// Constructors and destructors of the native sub-decoder states. The base class bodies
// (C1 0x8860dc, C2 0x88621c, dtors 0x8854cc and 0x8854e8) are not ported and are not
// defined in the port, so each function here stays partial: it only reproduces the derived
// part of the original call sequence and reaches the base through the declared-only
// constructor and destructor.
#include "vox-audio/vox/native_sub_decoder_state.hpp"

namespace vox {

// 0x888670 _ZN3vox24NativeSubDecoderPCMStateC1EPNS_22NativePlaylistsManagerE (partial)
// C2 0x8886a4 has the same body.
NativeSubDecoderPCMState::NativeSubDecoderPCMState(NativePlaylistsManager* manager)
    : NativeSubDecoderState(manager) {}

// 0x8885ec _ZN3vox24NativeSubDecoderPCMStateD1Ev (partial: base destructor not ported)
// D2 0x88863c has the same body; D0 0x888620 is the deleting variant.
NativeSubDecoderPCMState::~NativeSubDecoderPCMState() = default;

// 0x887000 _ZN3vox29NativeSubDecoderIMAADPCMStateC1EPNS_22NativePlaylistsManagerE (partial)
// C2 0x887034 has the same body.
NativeSubDecoderIMAADPCMState::NativeSubDecoderIMAADPCMState(NativePlaylistsManager* manager)
    : NativeSubDecoderState(manager) {}

// 0x886f7c _ZN3vox29NativeSubDecoderIMAADPCMStateD1Ev (partial: base destructor not ported)
// D2 0x886fcc has the same body; D0 0x886fb0 is the deleting variant.
NativeSubDecoderIMAADPCMState::~NativeSubDecoderIMAADPCMState() = default;

}  // namespace vox
