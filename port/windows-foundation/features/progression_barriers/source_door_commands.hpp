#pragma once

#include "../../original_campaign_runtime.hpp"
#include "../../../level-world/canonical_door_v27.hpp"
#include "../../../level-world/canonical_object_manager_v1.hpp"
#include <map>

namespace dh::foundation::progression_barriers {

// Source OpenDoor/CloseDoor command continuation over the current canonical
// ObjectManager and the SAME published CanonicalDoor receiver. The resolver
// must borrow that receiver by identity; no second door state is kept here.
struct DoorCommandServices {
    std::shared_ptr<void> owner;
    std::weak_ptr<dh2::world::CanonicalObjectManagerV1> objects;
    std::function<bool(const dh2::world::CanonicalObjectBorrowV1&,
                       std::shared_ptr<dh2::world::CanonicalDoorV27>&,
                       std::string&)> same_door;
    // Script_OpenDoor/CloseDoor execute through DebugSwitches::load/GetSwitch
    // before resolving the named object, including the source trace-off path.
    std::function<bool(std::string&)> trace;
};

class SourceDoorCommands {
public:
    explicit SourceDoorCommands(DoorCommandServices);
    bool command(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                 bool skip,bool& handled,bool& blocking,std::string&);
private:
    struct ExecutedDoor {
        std::shared_ptr<dh2::world::CanonicalDoorV27> receiver;
        bool wait{};
    };
    DoorCommandServices services_;
    std::map<const OriginalCampaignCommand*,ExecutedDoor> executed_;
};

} // namespace dh::foundation::progression_barriers
