#pragma once
#include "source_companions.hpp"
#include "source_master.hpp"
#include "../../combat_session.hpp"
#include "../../../level-world/character_script_owner_v2.hpp"
#include "../../../level-world/character_script_session.hpp"
#include "../../../script-runtime/script_object_bridge.h"
#include <memory>

namespace dh2::navigation {struct PathObject;}
namespace dh::foundation {class PlayableActorBodies;}
namespace dh::foundation::companions {
struct LiveActorOwner {
    std::shared_ptr<void> lease;
    ActorState* actor{};
    dh2::character::CharacterAiPointerFieldsV105* ai_fields{}; // SAME CharAI+50/+54/+55, native identities
    dh2::navigation::PathObject* path{}; // SAME borrowed PF path list
    std::uintptr_t character_identity{};
    dh2::character::ScriptOwnerV2* script_owner{};
    // Canonical retained NPCs use CharacterScriptSession/ScriptOwner (V1).
    // This is a borrow of that same session, never a parallel V2 owner.
    dh2::character::CharacterScriptSession* script_session{};
    // The exact CharacterScriptSessionInput target binding, projected from the
    // same canonical ScriptCharacterObject; it is not an ActorState duplicate.
    dh2::character::TargetBindings48* target_bindings{};
};
struct LiveWorldBindings {
    std::shared_ptr<void> lease;
    CombatSession* session{};
    PlayableActorWorld* world{};
    ActorPopulation* population{};
    PlayableActorBodies* bodies{};
    std::function<bool(ActorId,LiveActorOwner&,std::string&)> owner;
    // Source AI_SetMaster/SetTarget/controller/skill operations: genuinely
    // execute whole admitted operation on SAME receiver. No field-only command.
    Invoke source_operation;
    // Source host-player producer; absence never assumes local player identity.
    std::function<bool(ActorId&,std::string&)> host_player;
    // Explicit source enabled80 producer for actors without population records.
    std::function<bool(ActorId,std::uint8_t&,std::string&)> enabled;
    std::function<bool(std::uintptr_t,std::uint32_t&,std::string&)> native_is_dead;
    std::function<bool(std::uintptr_t,ActorId&,std::string&)> actor_from_identity;
};
// Predicate/query bridge reacquires actors and exact source receiver each call.
// Full commands remain required source_operation calls, with SetTarget/ClearTarget
// publication verified against SAME actor.target_id after successful source call.
Invoke live_invoke(LiveWorldBindings);
bool borrow_live_owner(const LiveWorldBindings&,ActorId,LiveActorOwner&,std::string&);
bool borrow_source_master(const LiveWorldBindings&,ActorId,SourceMasterBorrow&,std::string&);
bool live_target_position(const LiveWorldBindings&,ActorId,std::array<float,3>&,std::string&);

// Synchronous callback entry on the same active retained V1/V2 owner. Uses fresh
// active identity and actual alias resolution; no secondary VM/globals/timer.
bool call_live_script(const LiveWorldBindings&,ActorId,const char* source_event,
                      ActorId optional_payload,std::string&);
// Source frame owner supplies exact eligible AI records/order and events. This
// adapter only updates admitted authored Rene OnUpdate; sight/range event creation
// belongs to existing source AI owner and is deliberately not radius polling.
bool update_live_rene(const LiveWorldBindings&,ActorId,std::string&);
// Callback half of the actual CharAI RaiseAIEvent producer. Character event
// IDs are the source domain (9, 0xa..0x19); range/sight eligibility and event
// creation stay in the existing CharAI/Target update kernels.
bool dispatch_live_rene_character_event(const LiveWorldBindings&,ActorId,
    std::uint32_t source_character_event,std::uintptr_t source_subject,std::string&);

struct NativeBindingContext {
    std::shared_ptr<LiveWorldBindings> world;
    ActorId actor{}; const char* name{}; Operation operation{};
};
struct NativeBindingOwner {
    // Lease must be retained by SAME ScriptOwner Session through lua_close.
    std::vector<std::unique_ptr<NativeBindingContext>> callbacks;
};
// Called by source owner_register_binding for this exact live session. Object
// provider is already established by caller's original Character object bridge.
// handled=false leaves unrelated native registrations to their actual owners.
// StartTimerCB/skills/buffs require original scoped callback/closure producers.
bool bind_live_native(const std::shared_ptr<LiveWorldBindings>&,ActorId,
    dh2_script_vm*,const char* actual_name,NativeBindingOwner&,bool& handled,std::string&);
} // namespace dh::foundation::companions
