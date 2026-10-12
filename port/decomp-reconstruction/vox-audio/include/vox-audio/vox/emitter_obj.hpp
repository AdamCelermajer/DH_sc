// vox::EmitterObj: one playing instance of a data source. Only the accessors
// recovered so far are declared; the layout is partial (offsets are kept as
// comments). Most accessors take the object's mutex at +0x18.
#pragma once

#include <cstdint>

#include "vox-audio/vox/mutex.hpp"

namespace vox {

class EmitterObj {
public:
    EmitterObj() = default;

    void SetGainModifier(float gain);       // 0x863528: stores +0x44, no lock
    bool IsReady();                         // 0x863530: returns (byte +0x1c) ^ 1, no lock

    void SetUserData(const std::uintptr_t& first_word);  // 0x8653c0: stores the record's first word at +0x134
    void Reset();                           // 0x8655e0: sets the byte at +0x99 to 1
    void SetLoop(bool loop);                // 0x865608: writes the byte at +0x8d
    bool GetLoop();                         // 0x865630: reads the byte at +0x8c (see NOTES.md)
    void SetState(int state);               // 0x865658: writes +0x94
    int GetState();                         // 0x865680: reads +0x90 (see NOTES.md)
    bool IsGroup(std::uint32_t groups);     // 0x8656f8: (groups & (1 << group)) != 0
    void SetGroup(int group);               // 0x865734: stores +0x28
    int GetGroup();                         // 0x86575c: returns +0x28

    // Words and bytes the accessors above touch (original offsets in comments).
    std::uint8_t ready_flag_ = 0;           // +0x1c
    int group_ = 0;                         // +0x28
    float gain_modifier_ = 1.0f;            // +0x44
    std::uint8_t loop_read_ = 0;            // +0x8c, read by GetLoop
    std::uint8_t loop_ = 0;                 // +0x8d, written by SetLoop
    int state_ = 0;                         // +0x90, read by GetState
    int state_set_ = 0;                     // +0x94, written by SetState
    std::uint8_t reset_flag_ = 0;           // +0x99, set by Reset
    std::uintptr_t user_data_ = 0;          // +0x134

private:
    Mutex mutex_;                           // +0x18
};

}  // namespace vox
