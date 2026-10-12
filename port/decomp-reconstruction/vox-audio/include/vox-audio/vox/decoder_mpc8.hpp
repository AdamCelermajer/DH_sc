// vox::DecoderMPC8: the Musepack (SV8) decoder. Only the constructors, the destructor
// and the two accessors are reconstructed; the cursor and CreateNewCursor are not
// ported, so the class stays abstract.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

// Creation parameters. The constructor reads only the first word.
struct DecoderMPC8Params {
    std::int32_t first = 0;  // +0x00
};

class DecoderMPC8 : public DecoderInterface {
public:
    // C1 0x86fcb0, C2 0x86fc78. A null parameter block leaves the value at -1.
    explicit DecoderMPC8(const DecoderMPC8Params* params);
    ~DecoderMPC8() override;  // D1 0x86fdc0 (empty); D0 0x86fdc4 is compiler-generated

    int GetType() const override;          // 0x86fb98: returns 3
    std::int32_t* GetParam() override;     // 0x86fba0: returns null

private:
    std::int32_t param_ = -1;              // +0x04
};

}  // namespace vox
