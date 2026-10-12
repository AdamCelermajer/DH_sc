// vox::VoxEngineInternal: the engine object behind the static vox::VoxEngine facade.
// Only its declarations are reconstructed here; no method body is in the port yet,
// so the facade forwarders that call into it are partial.
//
// Vtable (52 bytes, 13 slots): +0x00/+0x04 destructor, +0x08 Initialize, +0x0c Suspend,
// +0x10 Resume, +0x14 IsSuspended, +0x18 UpdateSources, +0x1c UpdateEmitters,
// +0x20 KillEmitter(EmitterObj*), +0x24 SetGroupGain, +0x28 SetMasterGain.
// The two slots after +0x28 are not named in the symbols.
#pragma once

#include <cstdint>

#include "vox-audio/vox/voxengine_types.hpp"

namespace vox {

class VoxEngineInternal {
public:
    virtual ~VoxEngineInternal();
    virtual void Initialize();                                             // +0x08
    virtual void Suspend();                                                // +0x0c
    virtual void Resume();                                                 // +0x10
    virtual bool IsSuspended();                                            // +0x14
    virtual void UpdateSources();                                          // +0x18
    virtual void UpdateEmitters(float dt);                                 // +0x1c
    virtual void KillEmitter(EmitterObj* emitter);                         // +0x20
    virtual void SetGroupGain(unsigned int group, float gain, float time); // +0x24
    virtual void SetMasterGain(float gain, float time);                    // +0x28

    // Non-virtual entry points that the facade calls directly (no vtable slot).
    void GetDebugInfo(DebugInfo& info);
    void PrintDebug();
    void SetOutputMode(VoxOutputMode mode);
    VoxOutputMode GetOutputMode();
    void SetInteractiveMusicState(EmitterHandle& emitter, const char* state);
    void SetDSPEmitterParameter(EmitterHandle& emitter, int param, void* value);
    void SetSFXPresetActive(int preset, bool active, float level);
    void SetRoutingVolume(char* from, char* to, VoxDSPGeneralParameter::BusRoutingType type,
                          float volume, float fade_in, float fade_out);
    void SetDynamicBusRouting(char* name);
    void SetStaticBusRouting(char* name);

    void Get3DGeneralParameteri(int param, int& value);
    void Get3DGeneralParameterf(int param, float& value);
    void Get3DGeneralParameter(Vox3DGeneralParameters& params);
    void Get3DListenerOrientation(float& fx, float& fy, float& fz, float& ux, float& uy, float& uz);
    void Get3DListenerVelocity(float& x, float& y, float& z);
    void Get3DListenerPosition(float& x, float& y, float& z);
    void Set3DGeneralParameteri(int param, int value);
    void Set3DGeneralParameterf(int param, float value);
    void Set3DGeneralParameter(const Vox3DGeneralParameters& params);
    void Set3DListenerOrientation(float fx, float fy, float fz, float ux, float uy, float uz);
    void Set3DListenerVelocity(float x, float y, float z);
    void Set3DListenerPosition(float x, float y, float z);

    void Get3DEmitterParameteri(EmitterHandle& emitter, int param, int& value);
    void Get3DEmitterParameterf(EmitterHandle& emitter, int param, float& value);
    void Get3DEmitterParameters(EmitterHandle& emitter, Vox3DEmitterParameters& params);
    void Get3DEmitterDirection(EmitterHandle& emitter, float& x, float& y, float& z);
    void Get3DEmitterVelocity(EmitterHandle& emitter, float& x, float& y, float& z);
    void Get3DEmitterPosition(EmitterHandle& emitter, float& x, float& y, float& z);
    void Set3DEmitterParameteri(EmitterHandle& emitter, int param, int value);
    void Set3DEmitterParameterf(EmitterHandle& emitter, int param, float value);
    void Set3DEmitterParameters(EmitterHandle& emitter, const Vox3DEmitterParameters& params);
    void Set3DEmitterDirection(EmitterHandle& emitter, float x, float y, float z);
    void Set3DEmitterVelocity(EmitterHandle& emitter, float x, float y, float z);
    void Set3DEmitterPosition(EmitterHandle& emitter, float x, float y, float z);

    float GetGroupGain(int group);
    float GetMasterGain();
    int GetStatus(EmitterHandle& emitter);
    bool IsDone(EmitterHandle& emitter);
    int IsPlaying(EmitterHandle& emitter);
    int IsAlive(EmitterHandle& emitter);
    int IsValid(EmitterHandle& emitter);
    int IsReady(EmitterHandle& emitter);
    void SetPlayCursor(EmitterHandle& emitter, float position);
    float GetPlayCursor(EmitterHandle& emitter);
    int GetGroup(EmitterHandle& emitter);
    void SetGroup(EmitterHandle& emitter, int group);
    void ResumeAllEmitters(unsigned int group, float fade);
    void PauseAllEmitters(unsigned int group, float fade);
    void StopAllEmitters(unsigned int group, float fade);
    void PlayAllEmitters(unsigned int group, float fade);
    void GetAllEmitters(EmitterHandle* out, int max_count);
    void Resume(EmitterHandle& emitter, float fade);
    void Pause(EmitterHandle& emitter, float fade);
    void Stop(EmitterHandle& emitter, float fade);
    void Play(EmitterHandle& emitter, bool loop, float fade);
    float GetPitch(EmitterHandle& emitter);
    void SetPitch(EmitterHandle& emitter, float pitch, float time);
    void SetLoop(EmitterHandle& emitter, bool loop);
    float GetGain(EmitterHandle& emitter);
    void SetGain(EmitterHandle& emitter, float gain, float time);
    void SetAutoKillAfterDone(EmitterHandle& emitter, bool on);
    void KillEmitter(EmitterHandle& emitter);
    int GetUserData(EmitterHandle& emitter);
    void SetUserData(EmitterHandle& emitter, EmitterHandleUserData& user_data);

    std::uint32_t GetDuration(DataHandle& data);
    int IsValid(DataHandle& data);
    int IsReady(DataHandle& data);
    void GetAllDataSources(DataHandle* out, int max_count);
    void GetEmitterHandles(DataHandle& data, EmitterHandle* out, int max_count);
    void ReleaseDatasource(unsigned int id);
    void ReleaseDatasource(DataHandle& data);
    int GetUid(DataHandle& data);
    void SetUid(DataHandle& data, int uid);
    void SetPriorityBankId(DataHandle& data, int bank);
    int GetUserData(DataHandle& data);
    void SetUserData(DataHandle& data, DataHandleUserData& user_data);

    void SetPriorityBank(int index, int min_priority, int max_priority, PriorityBankBehavior behavior);
    int RegisterDecoderType(DecoderInterface* (*factory)(void*));
    int RegisterStreamType(StreamInterface* (*factory)(void*));
};

}  // namespace vox
