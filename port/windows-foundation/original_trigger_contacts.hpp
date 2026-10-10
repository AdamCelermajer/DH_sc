#pragma once
#include "../level-world/canonical_trigger_zone_v22.hpp"
#include "../level-world/zone_collision_runtime_v83.hpp"
#include "../level-world/native_body.hpp"
#include <map>
#include <optional>
namespace dh::foundation {
using OriginalTriggerPeerProvider = std::function<bool(std::uintptr_t,dh2::world::ZonePeerBorrowV83&,std::string&)>;
// Borrow source fields from the SAME retained actor. These pointers must be
// owned by receiver; a copied mesh/rest box is not a live source field owner.
struct OriginalTriggerActorBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t identity{};
    const float* position160{};
    const float* absolute12c{};
    const std::uintptr_t* physical2dc{};
};
struct OriginalTriggerPhysicalBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t identity{}; // actual physical2dc receiver, not actor identity
    const dh2::physical::NativeBody* native{};
};
using OriginalTriggerActorLookup = std::function<bool(std::uintptr_t,OriginalTriggerActorBorrow&,std::string&)>;
using OriginalTriggerPhysicalLookup = std::function<bool(std::uintptr_t,OriginalTriggerPhysicalBorrow&,std::string&)>;
// Adapts already initialized source owners; never allocates a body, computes
// bounds, publishes physical2dc, or substitutes the movement preview radius.
OriginalTriggerPeerProvider original_trigger_peer_provider(OriginalTriggerActorLookup,
                                                           OriginalTriggerPhysicalLookup);
// This adapter owns occurrence registration only. ALL trigger/contact/timer and
// active-edge authority remains the retained recovered CanonicalTriggerZoneV22.
class OriginalTriggerContacts {
public:
    // Call BEFORE constructing the canonical owner; attach after constructor.
    // Does not infer constructor room from file name, ordinal, or map contents.
    bool prepare(const std::string& instance_key,std::optional<std::int32_t> actual_constructor_room,
                 OriginalTriggerPeerProvider,dh2::world::TriggerZoneServicesV22&,std::string&,
                 dh2::world::ZoneCollisionServicesV83 actual_collision_services = {});
    bool attach(const std::string& instance_key,std::shared_ptr<dh2::world::CanonicalTriggerZoneV22>,
                std::shared_ptr<void> actual_runtime_lease,std::string&);
    // Actual canonical declaration default200 each; owner InitPost multiplies
    // by its source scale and invokes its real SetBoundingBox(false) provider.
    bool source_dimensions(const std::string& instance_key,
                           std::optional<std::array<float,3>> authored,std::string&);
    bool initialize(const std::string& instance_key,std::string&);
    bool update(const std::string& instance_key,std::string&);
    bool update_all(std::string&);
    bool collision_begin(const std::string& instance_key,std::uintptr_t peer,std::string&);
    bool collision_end(const std::string& instance_key,std::uintptr_t peer,std::string&);
    bool touching(const std::string& instance_key,std::uintptr_t peer,bool&,std::string&);
    const dh2::world::CanonicalTriggerZoneV22* owner(const std::string&) const;
    void clear(); // Clear before world/peer body backing replacement.
private:
    struct Slot {
        std::int32_t room=-1;
        OriginalTriggerPeerProvider peer;
        dh2::world::ZoneCollisionServicesV83 collision;
        std::shared_ptr<dh2::world::CanonicalTriggerZoneV22> owner;
        std::shared_ptr<void> runtime;
    };
    std::map<std::string,std::shared_ptr<Slot>> slots_;
    std::vector<std::string> order_;
    static bool peer_services(const std::shared_ptr<Slot>&,dh2::world::ZoneCollisionServicesV83&,std::string&);
    std::shared_ptr<Slot> slot(const std::string&,std::string&) const;
};
}
