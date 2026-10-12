// vox::EmitterObj::GetDebugInfo: one DebugChunk_emitter filled under the Handlable mutex.
//
// Partial. The original also sets the chunk's param0 to -1 when the cursor at +0x114 reports
// more than one channel (asm: cursor +0x04 > 1). That word is TrackParams::channels, which the
// DecoderCursorInterface does not expose, so the check is not made here. Everything else is
// copied, including the source gain (+0x38 -> +0x20) and source pitch (+0x70 -> +0x28).
#include "vox-audio/vox/emitter_obj.hpp"

#include "vox-audio/vox/data_obj.hpp"

namespace vox {

// 0x866818 _ZN3vox10EmitterObj12GetDebugInfoERNS_18DebugChunk_emitterE
void EmitterObj::GetDebugInfo(DebugChunk_emitter& out) {
    mutex.Lock();
    out.emitter_id = id;                                                // +0x08 -> +0x00
    out.data_id = data_ != nullptr ? data_->id : 0;                    // DataObj +0x08 -> +0x08
    out.position = position_;                                           // +0x9c..+0xa4 -> +0x10..+0x18
    out.state = state_set_;                                             // +0x94 -> +0x1c
    out.source_gain = source_gain_;                                     // +0x38 -> +0x20
    // The pseudocode multiplies the gain modifier by the resume fade target, then by the gain fade target.
    out.gain = (gain_modifier_ * resume_fade_.target) * gain_fade_.target;  // +0x44, +0x60, +0x4c -> +0x24
    out.source_pitch = source_pitch_;                                   // +0x70 -> +0x28
    out.pitch = pitch_;                                                 // +0x74 -> +0x2c
    out.loop = loop_read_;                                              // +0x8c -> +0x30
    out.ref_count = ref_count;                                          // +0x10 -> +0x34
    out.group = group_;                                                 // +0x28 -> +0x38
    out.velocity = velocity_;                                           // +0xb4..+0xbc -> +0x3c..+0x44
    out.direction = direction_;                                         // +0xa8..+0xb0 -> +0x48..+0x50
    out.param0 = param0_;                                               // +0xc0 -> +0x54
    for (int i = 0; i < 7; ++i) {
        out.params[i] = float_params_[i];                               // +0xc4..+0xdc -> +0x58..+0x70
    }
    mutex.Unlock();
}

}  // namespace vox
