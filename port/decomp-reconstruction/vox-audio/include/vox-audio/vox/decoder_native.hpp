// vox::DecoderNative: decoder for the native sub-decoder format. Only the
// accessors that do not need the cursor or the state containers are declared.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

// Abstract until CreateNewCursor is reconstructed (its cursor is not yet ported).
class DecoderNative : public DecoderInterface {
public:
    int GetType() const override;                       // 0x871098: returns 4
    std::int32_t* GetParam() override;                  // 0x8710a0: returns null
    void DestroyCursor(DecoderCursorInterface* cursor) override;  // 0x871a50
};

}  // namespace vox
