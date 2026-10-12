// Platform output driver interfaces (vox::DriverInterface, vox::DriverSourceInterface).
// The base classes carry the virtual surface the engine calls on every driver.
// Vtable slots for DriverInterface: +0x00/+0x04 destructors, +0x10 and +0x14 (no
// symbol; null in the base, so pure here and named after the VoxEngineInternal calls
// that use them), +0x18 RampOut, +0x1c RampIn, +0x20 Update, +0x28 SetDSPParameter,
// +0x2c SetStaticBusRouting, +0x30 SetDynamicBusRouting, +0x34 SetSFXPresetActive,
// +0x38 SetOutputMode, +0x3c GetOutputMode, +0x24 Set3DParameter, +0x40 PrintDebug (both
// named by the concrete vtables). Slots +0x08 (Init), +0x0c (Shutdown), +0x44 and +0x48 are
// named by DriverAndroid only.
// DriverSourceInterface (vptr-relative, as above): +0x00/+0x04 destructors, +0x08 Play,
// +0x0c Stop, +0x10 Pause, +0x14 Reset, +0x18 GetState, +0x1c GetByteOffset, +0x20
// SetByteOffset, +0x24 NeedData, +0x28 UploadData, +0x2c SetGain, +0x30 SetPitch, +0x34
// GetGain, +0x38 GetPitch, +0x3c Set3DParameter, +0x40 SetDSPParameter, +0x44 Update,
// +0x48 AllowBufferReference, +0x4c FreeDisposableData, +0x50 PrintDebug, +0x54
// GetBufferCount, +0x58 Init, +0x5c Cleanup. The names come from the concrete
// DriverCallbackSourceInterface and DriverAndroidSource vtables (vtables-001.json); the
// base slots are pure in the binary, so the base defaults below are empty.
#pragma once

#include <cstdint>

#include "vox-audio/vox/voxengine_types.hpp"

namespace vox {

class DriverSourceInterface;

class DriverInterface {
public:
    virtual ~DriverInterface();                                        // D1 0x88fcdc, D0 0x891000
    virtual void Suspend() = 0;                                        // +0x10, called when the engine suspends
    virtual void Resume() = 0;                                         // +0x14, called when the engine resumes
    virtual void RampOut();                                            // 0x88ed40 (empty)
    virtual void RampIn();                                             // 0x88ed44 (empty)
    virtual void Update(float dt);                                     // 0x88fce0 (empty)
    virtual void SetDSPParameter(int param, void* value);              // 0x88ed48 (empty)
    virtual void SetStaticBusRouting(char* name);                      // 0x88ed4c (empty)
    virtual void SetDynamicBusRouting(char* name);                     // 0x88ed50 (empty)
    virtual void SetSFXPresetActive(int preset, bool active, float level);  // 0x88ed54 (empty)
    virtual int SetOutputMode(VoxOutputMode mode);                     // 0x88ed58: returns 0
    virtual int GetOutputMode();                                       // 0x88ed60: returns 1
    // Slot +0x24: the 3D parameter push (DriverCallbackInterface::Set3DParameter(int, void*) in
    // the binary's vtables). Param 0..2 are the general block fields, 3..5 the listener position,
    // velocity and orientation. Empty in the base class.
    virtual void Set3DParameter(int param, const void* value) {}
    // Slot +0x40: the driver's own debug dump (DriverAndroid::PrintDebug in the binary's vtable),
    // called from VoxEngineInternal::PrintDebug. Empty in the base class.
    virtual void PrintDebug() {}
    // Slot +0x44: creates the source a new emitter plays through, from the four track words
    // (channels, sample rate, bits, length), the user pointer and the emitter flags. Null in
    // the base; DriverAndroid returns null too (its source is not created, see NOTES.md).
    virtual DriverSourceInterface* CreateDriverSource(const std::int32_t* params, void* user, int flags);
    // Slot +0x48: releases a source made by CreateDriverSource. Empty in the base.
    virtual void DestroyDriverSource(DriverSourceInterface* source);
    // Slots +0x08 Init(void*) and +0x0c Shutdown() are pure in the base and named by the
    // DriverAndroid and DriverCallbackInterface vtables.
};

class DriverSourceInterface {
public:
    virtual ~DriverSourceInterface();                                  // D1 0x88fcc8, D0 0x891014
    // Slots +0x08..+0x3c: the source playback surface the emitter drives (Play, Stop,
    // Pause, Reset, GetState, the byte offset, NeedData, UploadData, gain, pitch and the
    // 3D parameter push). Empty here because the base slots are pure in the binary; each
    // concrete source overrides them. The emitter calls them through this base.
    virtual void Play() {}                                             // +0x08
    virtual void Stop() {}                                             // +0x0c
    virtual void Pause() {}                                            // +0x10
    virtual void Reset() {}                                            // +0x14
    virtual int GetState() { return 0; }                               // +0x18
    virtual int GetByteOffset() { return 0; }                          // +0x1c
    virtual void SetByteOffset(int offset) { (void)offset; }           // +0x20
    virtual int NeedData() { return 0; }                               // +0x24
    virtual void UploadData(const void* data, int size) { (void)data; (void)size; }  // +0x28
    virtual void SetGain(float gain) { (void)gain; }                   // +0x2c
    virtual void SetPitch(float pitch) { (void)pitch; }                // +0x30
    virtual float GetGain() { return 0.0f; }                           // +0x34
    virtual float GetPitch() { return 0.0f; }                          // +0x38
    virtual void Set3DParameter(int which, const void* value) { (void)which; (void)value; }  // +0x3c
    virtual void SetDSPParameter(int param, void* value);              // +0x40, 0x88ed3c (empty)
    virtual void Update(float dt);                                     // +0x44, 0x88fccc (empty)
    virtual int AllowBufferReference();                                // +0x48, 0x88fcd0: returns 0
    virtual void FreeDisposableData(int id, int& first, int& second);  // +0x4c, 0x88fcd8 (empty)
    virtual void PrintDebug() {}                                       // +0x50 (DriverAndroidSource::PrintDebug)
    virtual int GetBufferCount() const { return 0; }                   // +0x54
    virtual void Init() {}                                             // +0x58
    virtual void Cleanup() {}                                          // +0x5c
};

}  // namespace vox
