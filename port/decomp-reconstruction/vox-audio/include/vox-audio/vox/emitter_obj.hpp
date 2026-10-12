// vox::EmitterObj: one playing instance of a data source. It is a Handlable, so the
// mutex (+0x18) and reference count (+0x10) are the base class members. The 3D
// parameter block is indexed by parameter id: 0 int, 1..7 floats, 8 position,
// 9 velocity, 10 direction. Offsets from the original are kept as comments.
#pragma once

#include <cstdint>
#include <string>

#include "vox-audio/vox/gain_fade.hpp"
#include "vox-audio/vox/handlable.hpp"
#include "vox-audio/vox/voxengine_types.hpp"

namespace vox {

class DriverSourceInterface;
class DataObj;

// Opaque user-data record as the original passes it: only the first word is stored
// (EmitterObj +0x134). The record's full size is not confirmed.
struct EmitterHandleUserData {
    std::uintptr_t word = 0;
};

class EmitterObj : public Handlable {
public:
    EmitterObj() = default;
    ~EmitterObj() override;  // D1 0x8699c0 and D2 0x869a38 (same body); D0 0x869a1c is the deleting variant

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

    // State-change callback slot (+0x138 callback, +0x13c user data, pending byte +0x98).
    void RegisterStateChangedCallback(EmitterStateCallback callback, void* user_data);  // 0x864ce0
    void UnregisterStateChangedCallback();  // 0x864cb4
    // Hands out a pending callback once; returns false when none is pending.
    bool NeedToSendStateChangedCallback(EmitterStateCallback& callback, void*& user_data,
                                        EmitterExternState& status);  // 0x864bec
    int GetStatus();                        // 0x864b5c (locks; codes 0, 2, 4, 0x11, 0x21)

    // Declared for the VoxEngineInternal forwarders. Bodies not reconstructed yet, except
    // SetGain and SetPitch, which restart the gain and pitch fades (partial, see the .cpp).
    void Play(float fade);                  // 0x86e304 (partial: resume fade, see transport .cpp)
    void Pause(float fade);                 // 0x86e428 (partial)
    void Stop(float fade);                  // 0x86e868 (partial)
    void Resume(float time);                // 0x86c1d4 (partial, see the .cpp)
    void SetGain(float gain, float time);   // 0x86bb24
    void SetPitch(float pitch, float time); // 0x86bc40
    void Update(float dt);                  // 0x86d5b8
    void SetPlayCursor(float position);     // 0x8653ec (partial, see the .cpp notes)
    float GetPlayCursor();                  // 0x8654f8
    void SetDSPParameter(int param, void* value);  // 0x866acc (partial: copies the name to +0xf0)
    void CleanUp();                         // 0x863954 (partial: DSP buffers only, see the .cpp)
    void SetInteractiveMusicStateChange(const char* state);  // 0x866abc
    void PrintDebug();                      // 0x864e28

    // Source and DSP buffer (+0xec, +0xf0, +0x110). UpdateDSP forwards to the source.
    void UpdateDSP(float dt);               // 0x8638e4

    // Words and bytes the accessors above touch (original offsets in comments).
    std::uint8_t ready_flag_ = 0;           // +0x1c
    float gain_ = 0.0f;                     // +0x3c, returned by GetGain
    float requested_gain_ = 0.0f;           // +0x40, the raw gain last passed to SetGain (not clamped)
    VoxGainFade gain_fade_;                 // +0x48..+0x58: fade toward the SetGain target (target +0x4c)
    int group_ = 0;                         // +0x28
    int priority_ = 0;                      // +0x2c, compared by PriorityBankManager (writer not identified)
    int stamp_index_ = 0;                   // +0x14, emitter slot for the handle stamp table (VoxEngineInternal +0x550)
    DataObj* data_ = nullptr;               // +0x118, the data object this emitter plays (set by the constructor)
    int bank_ = 0;                          // +0x30, priority bank index (passed to PriorityBankManager)
    bool in_bank_ = false;                  // +0x34, emitter is registered with a priority bank
    float gain_modifier_ = 1.0f;            // +0x44
    float pitch_ = 0.0f;                    // +0x74, returned by GetPitch, written raw by SetPitch
    VoxGainFade pitch_fade_;                // +0x78..+0x88: fade toward the SetPitch target (target +0x7c)
    std::uint8_t loop_read_ = 0;            // +0x8c, read by GetLoop
    std::uint8_t loop_ = 0;                 // +0x8d, written by SetLoop
    bool callback_pending_ = false;         // +0x98, set when a state change needs a callback
    VoxGainFade resume_fade_;               // +0x5c..+0x6c: fade toward 1.0 restarted by Resume; its flag
                                            // (+0x6c) is tested by GetStatus while the state is 1
    bool interactive_state_set_ = false;    // +0x144, set by SetInteractiveMusicStateChange
    std::string interactive_state_;         // +0x146 in the original (a strcpy into an unbounded buffer)
    int state_ = 0;                         // +0x90, read by GetState
    int state_set_ = 0;                     // +0x94, written by SetState
    VoxVector3f position_;                  // +0x9c (parameter 8)
    VoxVector3f direction_;                 // +0xa8 (parameter 10)
    VoxVector3f velocity_;                  // +0xb4 (parameter 9)
    int param0_ = 0;                        // +0xc0 (parameter 0)
    float float_params_[7] = {};            // +0xc4..+0xdc (parameters 1..7)
    std::uint8_t dirty_[11] = {};           // +0xe0..+0xea, one byte per parameter id
    std::uint8_t reset_flag_ = 0;           // +0x99, set by Reset
    void* owned_buffer_ = nullptr;          // +0xf4, freed by the destructor (owner not identified)
    void* dsp_buffer_ = nullptr;            // +0xec, the DSP name in use; UpdateDSP frees it when dsp_pending_ is set
    void* dsp_pending_ = nullptr;           // +0xf0, DSP name copied by SetDSPParameter until UpdateDSP applies it
    EmitterStateCallback state_callback_ = nullptr;  // +0x138
    void* state_callback_user_ = nullptr;   // +0x13c
    DriverSourceInterface* dsp_source_ = nullptr;    // +0x110, the playing source (vtable +0x44 is Update)
    bool need_to_die_ = false;              // +0x11c, set by NeedToDie
    bool auto_kill_ = false;                // +0x11d, set by SetAutoKillAfterDone
    std::uintptr_t user_data_ = 0;          // +0x134
};

}  // namespace vox
