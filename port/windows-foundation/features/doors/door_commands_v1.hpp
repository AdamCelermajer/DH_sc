#pragma once

// Preview 17 DOORS: Script_OpenDoor (kind 54) and Script_CloseDoor (kind 55) over DoorRuntimeV1.
// Same command contract as features/progression_barriers/source_door_commands (the canonical ObjectManager
// receiver there is not part of the Windows executable, so the name lookup and the state transitions are
// bound to DoorRuntimeV1 instead). Authored fields: string 12 = declaration name, scalar 20 = wait byte.
// Execute: the door named by string 12 is opened/closed (skip = the script skip flag). A missing name is a
// no-op (source: the cached object is cleared). Blocking: a command with wait=1 blocks while its door is busy.

#include "door_runtime_v1.hpp"
#include "../../original_campaign_runtime.hpp"

#include <map>
#include <string>

namespace dh::foundation::doors {

class DoorCommandsV1 {
public:
    explicit DoorCommandsV1(DoorRuntimeV1& doors) : doors_(doors) {}
    // Same shape as SourceDoorCommands::command. handled=false for every kind except 54/55.
    bool command(CampaignCommandPhase phase, const OriginalCampaignCommand& command, int module, bool skip,
                 bool& handled, bool& blocking, std::string& error);
    // Source Script_OpenDoor/CloseDoor state after the last executed command of this name (for logs).
    std::size_t executed_count() const noexcept { return executed_.size(); }

private:
    struct Executed { std::size_t door = 0; bool wait = false; };
    DoorRuntimeV1& doors_;
    std::map<const OriginalCampaignCommand*, Executed> executed_;
};

} // namespace dh::foundation::doors
