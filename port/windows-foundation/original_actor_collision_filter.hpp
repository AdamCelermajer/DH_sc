#pragma once
#include "actor_state.hpp"
#include "../level-world/navigation_avoidance.hpp"
#include <functional>
#include <map>

namespace dh::foundation {
enum class OriginalCollisionReceiverKind { physical_object, po_character };
struct OriginalCollisionOwnerServices {
    // Actual ObjectHandle -> Character conversion result. Required for POCharacter.
    std::function<bool(ActorId,bool&,std::string&)> character_handle;
    // Same owner's original FSM integer getter, needed only on Character handle.
    std::function<bool(ActorId,std::int32_t&,std::string&)> original_state;
    // Actual ObjectBase+80 enabled byte, not health/alive/global command policy.
    std::function<bool(ActorId,std::uint8_t&,std::string&)> enabled80;
};
// Source POCharacter prefix0x46fb4c followed by PhysicalObject0x46e6bc.
// No AIS/Lua filter callback exists in these verified source bodies. Collision
// notifications/OnCollisionPersist are separate services and are not implemented.
class OriginalActorCollisionFilter {
public:
    // Bind BEFORE NativeWorld CreateShape/SetMass can invoke either receiver.
    bool bind(ActorId,OriginalCollisionReceiverKind,OriginalCollisionOwnerServices,std::string& error);
    void remove(ActorId);
    bool permits_category(ActorId,std::uint16_t peer_category,bool& allowed,std::string& error)const;
    // peer_owner0 denotes an actual physical peer whose GameObject owner is null.
    // Failed delivery preserves allowed; true+allowedfalse is a genuine rejection.
    bool test_one(ActorId owner,ActorId peer_owner,
                  const dh2::navigation::ContactFilter& own,
                  const dh2::navigation::ContactFilter& peer,bool& allowed,std::string& error)const;
    // PhysicalWorld0x34c304 calls BOTH receiver tests even if first rejects.
    bool test_pair(ActorId first,ActorId second,
                   const dh2::navigation::ContactFilter& first_filter,
                   const dh2::navigation::ContactFilter& second_filter,
                   bool& allowed,std::string& error)const;
private:
    struct Owner {OriginalCollisionReceiverKind kind;OriginalCollisionOwnerServices services;};
    std::map<ActorId,Owner> owners_;
};
} // namespace dh::foundation
