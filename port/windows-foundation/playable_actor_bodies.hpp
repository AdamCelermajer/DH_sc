#pragma once
#include "original_actor_body_plan.hpp"
#include "original_actor_navigation.hpp"
#include "original_combat_properties.hpp"
#include "../level-world/character_ai_events.hpp"
#include <map>
namespace dh::foundation {
// POCharacter contact prefix and Character.RaiseEvent share the host's actual
// handle/Debug/CharAI receiver. No private AI fields, target or FSM are created.
struct OriginalActorContactBindings {
    std::shared_ptr<void> runtime_lease;
    std::function<bool(void* physical_peer,std::uintptr_t& owner,std::string&)> peer_owner;
    std::function<bool(std::uintptr_t owner,std::uintptr_t& character,std::string&)> handle_character;
    std::function<bool(std::string&)> debug_load;
    std::function<bool(const char*,bool&,std::string&)> debug_switch;
    std::function<bool(std::uintptr_t,bool&,std::string&)> is_player;
    std::function<bool(std::uintptr_t,dh2::character::AIEventState64*&,
                       dh2::character::AIEventServices24&,std::string&)> ai_events;
};
// Directly supplies OriginalActorPhysicalBindings.contact. Reached unavailable
// services fail; source Result's empty body and absent-peer branches are exact.
std::function<bool(dh2::physical::ContactEvent,void*,unsigned,std::string&)>
original_actor_contact_callback(ActorId,OriginalActorContactBindings);
// This pool borrows actual world ownership, actor registry and immutable combat
// property backing. Destroy/release BEFORE those backing registries are replaced.
class PlayableActorBodies {
public:
    PlayableActorBodies(); ~PlayableActorBodies();
    PlayableActorBodies(const PlayableActorBodies&)=delete;
    bool bind(ActorState&,const OriginalCombatProperties&,const OriginalActorBodyPlan&,
              OriginalActorPhysicalBindings actual_services,std::string& error);
    bool set_position(ActorId,const std::array<float,3>& admitted_position,
                      bool update_destination,std::string& error);
    using CurrentActorLookup=std::function<ActorState*(ActorId)>;
    // One physical Step per explicit host frame before actor update. Caller
    // supplies its current stable registry; no second actor/world is created.
    // Duplicate same-frame/same-dt calls succeed with stepped=false. Contact
    // callbacks are the existing required body services, never suppressed.
    bool step_world(dh2::physical::NativeWorld&,std::uint64_t frame,
                    std::uint32_t dt_ms,const CurrentActorLookup&,
                    bool& stepped,std::string& error);
    struct PhysicsPositionResult {
        bool body_present=false,sleeping=false,xy_changed=false;
        bool floor_checked=false,floor_valid=false,body_reseated=false;
        std::array<float,3> position{};
    };
    // Source awake/>1game-unit body-to-actor reconciliation, then actor-to-body
    // publication through the existing whole position setter. Call at actor's
    // actual source phase; floor predicate is an explicit live virtual result.
    bool reconcile_physics_position(ActorId,const CurrentActorLookup&,
                    bool validating_floor,PhysicsPositionResult&,std::string& error);
    // Resolve only registered actual physical contexts, including callbacks
    // during body release. Unknown foreign context fails without casting it.
    bool resolve_physical_actor(void* context,ActorId&,std::string& error)const;
    bool release(ActorId,std::string& error);
    bool remove_physical(ActorId,std::string& error);
    bool initialize_physical(ActorId,std::string& error);
    // PhysicalObject filter only; GameObject PF-obstacle lifecycle is external.
    bool set_physical_filter_enabled(ActorId,bool,std::string& error);
    // Exact Character state PhysicalObject::setFilter/resetFilter operations.
    // lookup must resolve this stable ID to the same current ActorState owner.
    bool set_source_physical_filter(ActorId,const CurrentActorLookup&,
                    std::int16_t group,std::uint16_t category,std::uint16_t mask,
                    bool apply_secondary,std::string& error);
    bool reset_source_physical_filter(ActorId,const CurrentActorLookup&,
                    std::string& error);
    // Host invokes this from proved source state/controller lifecycle only.
    bool set_pinned(ActorId,bool,std::string& error);
    // Physical part of source GameObject.Stop only. The caller first owns
    // DropPath/destination/heading and the actual physical-position predicate.
    // Resets velocity at current actorXY and sleeps, preserving mass/pin.
    // An absent actual physical receiver succeeds with stopped=false.
    bool stop_physical(ActorId,const CurrentActorLookup&,bool& stopped,std::string& error);
    bool initialize_navigation(ActorId,OriginalActorNavigationWorldBindings,std::string& error);
    // Authored root displacement -> source position/floor admission only.
    // ValidateDirection is the separate heading API, never a root-motion gate.
    bool move_grounded(ActorId,Vec3 current_world,Vec3 world_delta,
                       OriginalActorNavigationMoveResult&,std::string& error);
    bool update_path(ActorId,OriginalActorPathBindings&,dh2::navigation::ControllerResult&,std::string& error);
    bool update_manual_heading(ActorId,const OriginalManualHeadingBindings&,
                               OriginalManualHeadingResult&,std::string& error);
    bool clear(std::string& error);
    // Source body dimensions subtract CURRENT absolute float bounds. Translation
    // can change cancellation rounding even when cached relative box is fixed.
    static bool rebase_plan(OriginalActorBodyPlan&,ActorId,
                            const std::array<float,3>& position,std::string& error);
    bool actor_borrow(std::uintptr_t,OriginalTriggerActorBorrow&,std::string& error) const;
    bool physical_borrow(std::uintptr_t,OriginalTriggerPhysicalBorrow&,std::string& error) const;
    bool subobjects_borrow(ActorId,OriginalActorSubobjectsBorrow&,std::string& error);
    const OriginalActorPhysical* physical(ActorId) const noexcept;
    const dh2::navigation::NavigationObject* navigation(ActorId) const noexcept;
    // Read-only identity borrow of the exact sewn floor world and obstacle
    // registry attached to this actor's retained PFObject.
    bool navigation_world_binding(ActorId,OriginalActorNavigationWorldBindings&,
                                  std::string& error) const;
    std::size_t size()const noexcept{return entries_.size();}
private:
    struct Entry;
    std::map<ActorId,std::shared_ptr<Entry>> entries_;
    bool stepping_=false;
    std::uint64_t last_step_frame_=0;
    std::uint32_t last_step_dt_=0;
    const dh2::physical::NativeWorld* last_step_world_=nullptr;
    bool last_step_ok_=true;
    std::string last_step_error_;
};
}
