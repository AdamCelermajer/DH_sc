// vox::DecoderMSWav: decoder for Microsoft WAV sources. Only the accessors that
// do not need the cursor are declared; CreateNewCursor is not reconstructed yet.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

// Abstract until CreateNewCursor is reconstructed (DecoderMSWavCursor is not ported).
class DecoderMSWav : public DecoderInterface {
public:
    // C1 0x870548, C2 0x870514: the vtable, a set flag byte at +0x40 and a null
    // data-node pointer at +0x3c.
    DecoderMSWav();
    // D1 0x870ffc, D2 0x870f7c (partial: see decoder_mswav.cpp).
    ~DecoderMSWav() override;

    int GetType() const override;                       // 0x870504: returns 1
    std::int32_t* GetParam() override;                  // 0x87050c: returns null
    void DestroyCursor(DecoderCursorInterface* cursor) override;  // 0x870a9c

private:
    void* data_nodes_ = nullptr;  // +0x3c, the DataNode tree (not ported)
    bool flag_40_ = true;         // +0x40, set to 1 by the constructor (meaning not known)
};

// 0x8709b8 _ZN3vox19DecoderMSWavFactoryEPv (not reconstructed: DecoderMSWav is abstract
// until CreateNewCursor is ported, so the factory cannot construct it yet).

}  // namespace vox
