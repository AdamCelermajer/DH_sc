#pragma once
#include "actor_state.hpp"
#include <functional>
#include <map>
#include <optional>
#include <string>
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
private:
    struct Record { ActorState* actor=nullptr;OriginalLifecycleFacts facts;OriginalLifecycleStatus status; };
    std::map<ActorId,Record> records_;
    OriginalLifecycleServices services_;
    bool change(Record&,int target,std::string&);
    bool call(Record&,OriginalLifecycleOperation,int previous,std::string&);
};
}
