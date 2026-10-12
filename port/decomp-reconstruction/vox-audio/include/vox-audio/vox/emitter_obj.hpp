// vox::EmitterObj: one playing instance of a data source. It is a Handlable, so the
// mutex (+0x18) and reference count (+0x10) are the base class members. The 3D
// parameter block is indexed by parameter id: 0 int, 1..7 floats, 8 position,
// 9 velocity, 10 direction. Offsets from the original are kept as comments.
#pragma once

#include <cstdint>

#include "vox-audio/vox/handlable.hpp"
#include "vox-audio/vox/voxengine_types.hpp"

namespace vox {

class EmitterObj : public Handlable {
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

    // 3D parameters. Ids: 0 int, 1..7 float, 8 position, 9 velocity, 10 direction.
    // Every setter marks the id dirty in dirty_[id].
    void Get3DParameterfv(int param, VoxVector3f& out);                 // 0x864e64
    void Get3DParameter3f(int param, float& x, float& y, float& z);     // 0x864f04
    void Get3DParameterf(int param, float& out);                        // 0x864fa0
    void Get3DParameteri(int param, int& out);                          // 0x865044
    void Set3DParameterfv(int param, const VoxVector3f& value);         // 0x865078
    void Set3DParameter3f(int param, float x, float y, float z);        // 0x865130
    void Set3DParameterf(int param, float value);                       // 0x8651c0
    void Set3DParameteri(int param, int value);                         // 0x865280
    void SetDefaultParameters();                                        // 0x8652b8

    bool IsDone();                          // 0x86556c: locks, then IsDoneUnlocked
    bool IsPlaying();                       // 0x86559c
    float GetPitch();                       // 0x8656a8: +0x74
    float GetGain();                        // 0x8656d0: +0x3c
    bool ShouldDie();                       // 0x865784
    bool IsAlive();                         // 0x8657f4 (see NOTES.md: locks twice in the original)
    void NeedToDie();                       // 0x865828: sets +0x11c
    void SetAutoKillAfterDone(bool on);     // 0x865850: writes +0x11d
    std::uintptr_t GetUserData();           // 0x865398: reads +0x134

    // Unlocked helpers used by the locked accessors above.
    bool IsDoneUnlocked();                  // 0x86353c (_IsDone): no lock
    bool ShouldDieUnlocked();               // body of 0x865784 without the lock

    // Words and bytes the accessors above touch (original offsets in comments).
    std::uint8_t ready_flag_ = 0;           // +0x1c
    float gain_ = 0.0f;                     // +0x3c, returned by GetGain
    int group_ = 0;                         // +0x28
    float gain_modifier_ = 1.0f;            // +0x44
    float pitch_ = 0.0f;                    // +0x74, returned by GetPitch
    std::uint8_t loop_read_ = 0;            // +0x8c, read by GetLoop
    std::uint8_t loop_ = 0;                 // +0x8d, written by SetLoop
    int state_ = 0;                         // +0x90, read by GetState
    int state_set_ = 0;                     // +0x94, written by SetState
    VoxVector3f position_;                  // +0x9c (parameter 8)
    VoxVector3f direction_;                 // +0xa8 (parameter 10)
    VoxVector3f velocity_;                  // +0xb4 (parameter 9)
    int param0_ = 0;                        // +0xc0 (parameter 0)
    float float_params_[7] = {};            // +0xc4..+0xdc (parameters 1..7)
    std::uint8_t dirty_[11] = {};           // +0xe0..+0xea, one byte per parameter id
    std::uint8_t reset_flag_ = 0;           // +0x99, set by Reset
    bool need_to_die_ = false;              // +0x11c, set by NeedToDie
    bool auto_kill_ = false;                // +0x11d, set by SetAutoKillAfterDone
    std::uintptr_t user_data_ = 0;          // +0x134
};

}  // namespace vox
