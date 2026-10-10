#pragma once
#include "live_companions.hpp"
#include "../../../level-world/actor_runtime.hpp"
#include "../../../android-native/app/src/main/cpp/source_campaign_character_fsm_v101.hpp"

namespace dh::foundation::companions {
struct SourceOwnerBorrow {
    std::shared_ptr<void> lease;
    std::uintptr_t character_identity{};
    ActorState* actor{};
    dh2::character::CharacterAiPointerFieldsV105* ai_fields{};
    dh2::actor::RuntimeState* runtime{};
    dh2::character::ScriptOwnerV2* script_owner{};
    dh2::character::CharacterScriptSession* script_session{};
    dh2::character::TargetBindings48* target_bindings{};
    const dh2::scene::Scene* scene{};
};
// Projects SAME retained owner cells into the live adapter; never initializes,
// allocates or copies a Character/CharAI/controller/path/ScriptOwner.
bool project_source_owner(const SourceOwnerBorrow&,LiveActorOwner&,std::string&);
// Dispatches a Character event raised by the existing source AI/target/master
// kernels to the same active V1 AISExternal session on a canonical record.
// The record-based path receipt pins the original CampaignFsm/route context
// across callbacks such as Rene's MoveTo/HasPath; no path is copied or mutated.
bool dispatch_source_rene_character_event_v1(
    const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>&,
    std::uint32_t source_event,std::uintptr_t source_subject,std::string&);
// Original private source ScriptOwner performs creation/SetCharacter/bind/load/
// initialization/publication order with its genuine existing services. The host
// owns actual owner_name (source GetName), all services, pending/active leases.
bool advance_source_companion(const LiveWorldBindings&,ActorId,const std::string& source_owner_name,
    const dh2::character::ScriptOwnerServicesV2&,std::string&);
// Canonical NPC overload: initialize through its already-retained V1 Session
// and its same ScriptOwner. Active Sessions are validated without restarting.
bool advance_source_companion(const LiveWorldBindings&,ActorId,
    dh2::character::CharacterScriptSession&,std::string&);
// Retained Scene borrow for SAME PopulationActor.visual, never a reloaded fairy
// scene. Source timing/animation still comes from actual retained animation owner.
bool borrow_companion_scene(const LiveWorldBindings&,ActorId,const dh2::scene::Scene*&,std::string&);
// Exact source binding descriptor validation; same-name wrong-address native is
// rejected. Method descriptors stay with the original object-method provider.
bool bind_source_registration(const std::shared_ptr<LiveWorldBindings>&,ActorId,
    const dh2::character::ScriptOwnerRequest&,NativeBindingOwner&,bool&,std::string&);
}
