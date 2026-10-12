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
struct Vox3DEmitterParameters;
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
