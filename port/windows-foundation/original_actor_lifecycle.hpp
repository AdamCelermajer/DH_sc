#pragma once
#include "actor_state.hpp"
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>
namespace dh::foundation {
enum class OriginalLifecycleState : int { limbus=0, spawn=1, idle=3, pre_spawn=17 };
enum class OriginalLifecycleOperation {
    set_flags, set_enabled, restore_initial_position, restore_initial_rotation,
    revive, clear_aggro, select_state_animation, clear_and_sync_target,
    cancel_sneaking, init_physical, notify_state_changed, remove_physical,
    freeze_animation_speed, set_collisions_enabled, clear_idle_suppressed
};
struct OriginalLifecycleRequest {
    OriginalLifecycleOperation operation;
    ActorState* actor=nullptr;
    int previous_state=-1, state=-1;
    std::uint32_t flags=0;
    bool enabled=false;
    bool collisions_enabled=false;
    Transform initial_transform;
};
struct OriginalLifecycleFacts {
    std::string preset_ai_state;
    // Source anchors come from original world placement and source floor query.
    Transform initial_transform;
    std::optional<bool> initially_enabled;
    // Required when source preset selects actual PreSpawn17.
    std::optional<bool> pre_spawn_has_animation, pre_spawn_stay_enabled;
    bool can_respawn=false;
    std::uint32_t respawn_delay_ms=0;
};
struct OriginalLifecycleServices {
    // Source SetInitialPosition query: found=false retains authored Z.
    std::function<bool(std::array<float,3> position,bool& found,float& height,std::string&)> floor_height;
    // Effects operate on SAME ActorState and retained world/visual/physics owners.
    std::function<bool(const OriginalLifecycleRequest&,std::string&)> invoke;
};
struct OriginalLifecycleStatus { int state=-1;std::uint32_t flags=0;bool enabled=false,collisions_enabled=false,failed=false; };
class OriginalActorLifecycle {
public:
    void bind(OriginalLifecycleServices services) { services_=std::move(services); }
    bool add(ActorState&,const OriginalLifecycleFacts&,std::string& error);
    bool spawn(ActorId,std::string& error);
    // Explicit immediate _SetState(3) subset. Script PutCharacterInIdle's
    // waitForAnim=true scheduling must use the existing original Idle owner.
    bool put_idle(ActorId,std::string& error);
    bool put_limbus(ActorId,std::string& error);
    // P16 DESPAWN: Idle -> Despawn(2) with its clip (CSDespawn::OnFocus), and the death-end body release (CSDead event 34).
    bool despawn(ActorId,std::string& error);
    bool release_body(ActorId,std::string& error);
    // Must be whole original animation-sequence completion, not a leaf end.
    bool animation_finished(ActorId,std::string& error);
    bool animation_event(ActorId,const std::string& name,std::string& error);
    const OriginalLifecycleStatus* status(ActorId) const;
    bool combat_enabled(ActorId) const;
    void remove(ActorId);
    void clear(); // Required before actor/world replacement invalidates pointers.
    // B141: a world restore replaces every ActorState, so the borrowed records are dropped. The authored facts of the preset
    // PreSpawn/Limbus actors (the ones a script spawns later) are kept so they can be re-registered after the restore.
    // clear() remains the full reset (level change).
    void detach_for_restore();
    std::vector<ActorId> pre_spawn_baseline_ids() const;
    // B141: Character::Deserialize 0x3a6210 (restored state 1/17 -> SM_SetPreSpawnState; 0/2/12 -> Limbus; others Idle) maps a saved Spawn(1)/PreSpawn(17) state to
    // SM_SetPreSpawnState. The restored actor already carries that state's effects (no physical body, collisions off), so this
    // only registers the record at PreSpawn17; it replays none of the transition effects. Requires a kept baseline and no record.
    bool restore_pre_spawn(ActorId,ActorState&,bool enabled,std::string& error);
    // B142: SM_SetPreSpawnState on a live Character after a death-checkpoint reload restored saved state 1/17 over a spawned/Idle/Limbus
    // actor. Runs only the PreSpawn Focus effects (the restored data already replaced what the old state's Blur would undo) and
    // registers the record at 17. Requires a kept baseline and no record; a failed effect leaves no record.
    bool enter_restored_pre_spawn(ActorId,ActorState&,bool enabled,int previous_state,std::string& error);
    // B145: true when the actor still stands on the authored initial anchor of its preset PreSpawn/Limbus baseline
    // (every axis within `tolerance`); false when it has no baseline.
    bool at_pre_spawn_anchor(const ActorState&,float tolerance=2.0f) const;
    // B155: the other half of the B141 restore. A campaign lifecycle actor whose preset is not PreSpawn/Limbus (auto_spawn=0 placed
    // Idle3 actors such as the Swamp King and his intro lizards) also loses its borrowed record on a world restore, after which
    // Script_PutCharacterInLimbus / PutCharacterInIdle on it failed with "Unknown original lifecycle actor" and aborted the cutscene.
    // Character::Deserialize 0x3a6210 maps a saved Limbus/Despawn/Dead state (0/2/12) to Limbus and every other non-PreSpawn state to
    // Idle; the restored actor already carries that state's data, so this only re-registers the record (state 0 or 3, the state's
    // Focus flag values) from the kept authored facts and replays no effect. Requires a kept non-preset baseline and no record.
    std::vector<ActorId> idle_baseline_ids() const;
    bool restore_idle_or_limbus(ActorId,ActorState&,bool enabled,bool limbus,std::string& error);
    // Admission for the re-registration above (the review of B155 narrowed it): a Limbus record always qualifies; an Idle record only
    // for a live actor whose saved session state is Idle3 and whose physical body is present. A dead or spawning actor keeps no
    // record (forcing it to Idle3 would be wrong). A bodiless actor qualifies only when it is a presentation prop (not targetable
    // anyway, such as the intro lizards a script puts in Limbus); a bodiless TARGETABLE actor would become untargetable, so it keeps none.
    static bool idle_preset_restorable(bool limbus, bool alive, std::optional<bool> physical_present, int session_state, bool enabled, bool targetable) noexcept {
        if (limbus) return true;
        if (!alive || session_state != 3) return false;
        if (physical_present && !*physical_present) return !targetable;
        return physical_present ? true : enabled;
    }
private:
    struct Record { ActorState* actor=nullptr;OriginalLifecycleFacts facts;OriginalLifecycleStatus status; };
    std::map<ActorId,Record> records_;
    std::map<ActorId,OriginalLifecycleFacts> pre_spawn_baseline_;
    std::map<ActorId,OriginalLifecycleFacts> idle_baseline_; // B155: non-preset campaign lifecycle actors
    OriginalLifecycleServices services_;
    bool change(Record&,int target,std::string&);
    bool focus(Record&,int target,int previous,bool carried_interactive,std::string&);
    bool call(Record&,OriginalLifecycleOperation,int previous,std::string&);
};
}
