// Names used by the vox::VoxEngine facade and vox::VoxEngineInternal signatures.
// The full definitions of these types are reconstructed elsewhere (or not at all
// yet); declaring them opaque is enough for the reference and enum parameters the
// forwarders pass through. Enum underlying types follow the 32-bit ABI (int).
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

class EmitterHandle;
class DataHandle;
class EmitterObj;
class VoxEngineInternal;
struct DebugInfo;
struct Vox3DGeneralParameters;
struct EmitterHandleUserData;
struct DataHandleUserData;  // defined in vox/data_obj.hpp

enum VoxOutputMode : int;
enum VoxSourceLoadingFlags : int;
enum PriorityBankBehavior : int;
enum EmitterExternState : int;

struct VoxDSPGeneralParameter {
    enum BusRoutingType : int;
};

}  // namespace vox

namespace vox {

// Three-float vector used by the 3D emitter parameters (position, velocity,
// direction). Layout matches three consecutive 12-byte blocks in EmitterObj.
struct VoxVector3f {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

}  // namespace vox

namespace vox {

// Per-emitter 3D parameters as Get3DEmitterParameters/Set3DEmitterParameters copy them:
// parameter 0 (int) then parameters 1..7 (floats), eight words in all.
struct Vox3DEmitterParameters {
    std::int32_t param0 = 0;   // +0x00
    float params[7] = {};      // +0x04..+0x1c, parameters 1..7
};

}  // namespace vox
