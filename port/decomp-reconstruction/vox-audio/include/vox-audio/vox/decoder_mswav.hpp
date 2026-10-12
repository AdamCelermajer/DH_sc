// vox::DecoderMSWav: decoder for Microsoft WAV sources. Only the accessors that
// do not need the cursor are declared; CreateNewCursor is not reconstructed yet.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

// Abstract until CreateNewCursor is reconstructed (DecoderMSWavCursor is not ported).
class DecoderMSWav : public DecoderInterface {
public:
    int GetType() const override;                       // 0x870504: returns 1
    std::int32_t* GetParam() override;                  // 0x87050c: returns null
    void DestroyCursor(DecoderCursorInterface* cursor) override;  // 0x870a9c
};

}  // namespace vox
