// Saved sub-decoder states of the native segment decoder (vox::NativeSubDecoderState and
// its PCM and IMA ADPCM variants). The base class is not ported yet (its constructor and
// destructor are large and touch the segment containers), so the derived constructors and
// destructors here call it and are marked partial.
#pragma once

#include <cstdint>

#include "vox-audio/vox/native_playlist.hpp"

namespace vox {

class NativeSubDecoderState {
public:
    // C2 0x8860dc, C1 0x88621c (288 bytes each in the original; not ported, declared only).
    explicit NativeSubDecoderState(NativePlaylistsManager* manager);
    // Not ported (declared only); the derived destructors call it.
    virtual ~NativeSubDecoderState();

private:
};

// PCM variant. Only the constructors and destructors are reconstructed here.
class NativeSubDecoderPCMState : public NativeSubDecoderState {
public:
    // C1 0x888670, C2 0x8886a4: base constructor then the PCM vtable.
    explicit NativeSubDecoderPCMState(NativePlaylistsManager* manager);
    // D1 0x8885ec, D2 0x88863c, D0 0x888620 (deleting): base destructor.
    ~NativeSubDecoderPCMState() override;
};

// IMA ADPCM variant.
class NativeSubDecoderIMAADPCMState : public NativeSubDecoderState {
public:
    // C1 0x887000, C2 0x887034: base constructor then the IMA ADPCM vtable.
    explicit NativeSubDecoderIMAADPCMState(NativePlaylistsManager* manager);
    // D1 0x886f7c, D2 0x886fcc, D0 0x886fb0 (deleting): base destructor.
    ~NativeSubDecoderIMAADPCMState() override;

    // Words 81..83 (+0x144..+0x14c): the decoding-slot flags that SetState and GetState copy.
    std::int32_t decoding_slot_used_[3] = {0, 0, 0};
};

}  // namespace vox
