// Platform output driver interfaces (vox::DriverInterface, vox::DriverSourceInterface).
// The base classes carry the virtual surface the engine calls on every driver.
// Vtable slots for DriverInterface: +0x00/+0x04 destructors, +0x10 and +0x14 (no
// symbol; null in the base, so pure here and named after the VoxEngineInternal calls
// that use them), +0x18 RampOut, +0x1c RampIn, +0x20 Update, +0x28 SetDSPParameter,
// +0x2c SetStaticBusRouting, +0x30 SetDynamicBusRouting, +0x34 SetSFXPresetActive,
// +0x38 SetOutputMode, +0x3c GetOutputMode. Slots +0x08, +0x0c, +0x24 and +0x40..+0x48
// have no symbol.
// DriverSourceInterface: +0x00/+0x04 destructors, +0x40 SetDSPParameter, +0x44 Update,
// +0x48 AllowBufferReference, +0x4c FreeDisposableData.
#pragma once

#include <cstdint>

#include "vox-audio/vox/voxengine_types.hpp"

namespace vox {

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
};

class DriverSourceInterface {
public:
    virtual ~DriverSourceInterface();                                  // D1 0x88fcc8, D0 0x891014
    virtual void SetDSPParameter(int param, void* value);              // 0x88ed3c (empty)
    virtual void Update(float dt);                                     // 0x88fccc (empty)
    virtual int AllowBufferReference();                                // 0x88fcd0: returns 0
    virtual void FreeDisposableData(int id, int& first, int& second);  // 0x88fcd8 (empty)
};

}  // namespace vox
