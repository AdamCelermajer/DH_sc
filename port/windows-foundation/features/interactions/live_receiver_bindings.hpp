#pragma once
#include "original_interactions.hpp"
#include "npc_interact.hpp"
#include "../../../level-world/retained_character_actor_v1.hpp"
#include "../../../level-world/character_use_ooi_v47.hpp"
#include "../../../level-world/canonical_openable_container_v1.hpp"
#include "../../../level-world/world_item_live_owner_v5.hpp"
#include "../../../level-world/canonical_door_v27.hpp"
namespace dh::foundation::interactions {
struct OpenableBorrow {
    std::shared_ptr<dh2::world::CanonicalOpenableContainerV1> receiver;
    dh2::world::OpenableContainerInteractionServicesV2 services;
};
inline bool borrow_npc_interact(const std::shared_ptr<dh2::character::RetainedCharacterActorV1>& receiver,
 NpcInteractBorrow& out,std::string& e){
    if(!receiver){e="Character.Interact requires SAME retained Character receiver";return false;}
    NpcInteractBorrow next;next.receiver=receiver;next.identity=receiver->canonical(receiver).identity;
    std::uintptr_t* ignored=nullptr;receiver->kill_metadata_borrow_v23(next.room64,next.data13c8,ignored);
    next.talk_flag2fa=receiver->source_bool_field(0x2fa);next.display_name44=receiver->source_string(0x44);
    if(!next.talk_flag2fa){e="Character.Interact requires produced talk flag2fa";return false;}
    out=std::move(next);return true;
}
struct LiveReceiverServices {
    std::shared_ptr<void> world;
    // Resolution must borrow the same retained factory receiver, never create
    // another object from its declaration. Its canonical identity is checked.
    std::function<bool(std::uintptr_t,OpenableBorrow&,std::string&)> openable;
    std::shared_ptr<dh2::character::WorldItemLiveOwnerV5> items;
    std::function<bool(std::uintptr_t,NpcInteractBorrow&,NpcInteractServices&,std::string&)> npc;
    // Complete source Character.Interact3a4d78, including state/event and UI
    // branches. Absence is unsupported, rather than a fabricated conversation.
    Router::Handler character_interact;
    std::map<std::uint32_t,Router::Handler> authored_receivers;
};
// Metadata bridge only; all receiver state remains in manager/factory owners.
class LiveReceiverBindings {
    std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects_;
    LiveReceiverServices services_;
public:
    LiveReceiverBindings(std::shared_ptr<dh2::world::CanonicalObjectManagerV1> objects,LiveReceiverServices services)
        :objects_(std::move(objects)),services_(std::move(services)){}
    // Input command delegates to the actual source controller. No immediate
    // Interact; FSM/animation owner later delivers its interaction marker.
    bool command(dh2::character::ControllerUseOoiV47& controller,std::uintptr_t requested,std::string& e){
        return controller.command(requested,e);
    }
    bool range(const RangeBorrow& source,std::uintptr_t target,bool& in,std::string& e)const{
        return in_interaction_range(source,target,in,e);
    }
    // Called by the original interaction animation/FSM branch, using published
    // manager keys. Key validation is read-only; no node insertion or ID casts.
    Outcome deliver(std::int32_t target_key,std::int32_t actor_key,std::string& e){
        e.clear();if(!objects_||!services_.world){e="Interactions require SAME live ObjectManager/world";return Outcome::failed;}
        const auto* target=objects_->object(target_key);const auto* actor=objects_->object(actor_key);
        if(!target||!actor||!target->identity||!actor->identity||!target->lease||!actor->lease||!target->type_f4){
            e="Interactions require published target/actor leases";return Outcome::failed;
        }
        // Pin both receivers while callbacks can mutate manager publication.
        const auto target_borrow=*target,actor_borrow=*actor;
        const auto type=*target_borrow.type_f4;
        if(type==7){
            if(!services_.openable){e="Interactions require retained openable factory receiver";return Outcome::failed;}
            OpenableBorrow borrow;if(!services_.openable(target_borrow.identity,borrow,e))return Outcome::failed;
            if(!borrow.receiver||borrow.receiver->base().identity()!=target_borrow.identity){e="Interactions openable receiver identity mismatch";return Outcome::failed;}
            return interactions::openable(borrow.receiver->receiver(),borrow.receiver->fields(),borrow.services,
                borrow.receiver->base().lifecycle().disabled81!=0,actor_borrow.identity,e);
        }
        if(type==3){
            if(!services_.items){e="Interactions require SAME published world-item/loot/inventory owner";return Outcome::failed;}
            return services_.items->interact(target_borrow.identity,actor_borrow.identity,e)?Outcome::completed:Outcome::failed;
        }
        if(type==2)return Outcome::blocked; // Door::IsInteractive source3e74a8
        if(type==0){
            if(services_.npc){
                NpcInteractBorrow borrow;NpcInteractServices services;
                if(!services_.npc(target_borrow.identity,borrow,services,e))return Outcome::failed;
                if(borrow.identity!=target_borrow.identity||!borrow.receiver){e="Character.Interact receiver identity mismatch";return Outcome::failed;}
                return npc_interact(borrow,actor_borrow.identity,services,e)?Outcome::completed:Outcome::failed;
            }
            if(!services_.character_interact){e="Unsupported Character.Interact: actual quest/FSM/native UI owner absent";return Outcome::unsupported;}
            return services_.character_interact(target_borrow,actor_borrow.identity,e)?Outcome::completed:Outcome::failed;
        }
        const auto i=services_.authored_receivers.find(type);
        if(i==services_.authored_receivers.end()){e="Unsupported authored interaction source type "+std::to_string(type);return Outcome::unsupported;}
        return i->second(target_borrow,actor_borrow.identity,e)?Outcome::completed:Outcome::failed;
    }
};
}
