#pragma once
#include "monster_decisions.hpp"
#include "../../../level-world/character_state.hpp"
#include "../../../level-world/character_script_selection.hpp"
#include <memory>

namespace dh::foundation::enemy_ai {
// Fresh scoped loan from the retained Character owner. These are the SAME
// native storage, not snapshots manufactured from presentation ActorState.
struct LiveEnemyBorrow {
    ActorState* actor{};
    dh2::data::PropertyView* properties{};
    dh2::character::TargetState48* target{};
    dh2::character::State* state{};
    void* controller{};
    void* path_owner{};
    const std::uint32_t* scene_clock{};
    void* random_channel0{};
    std::shared_ptr<void> receiver_lease;
};
// Filled by the actual selected AIS owner at its virtual dispatch boundary.
// The callback executes the original private script VM or native AIS body;
// it must preserve source VCB flags, scoped callbacks and script globals.
struct SelectedMonsterCallback {
    std::uintptr_t character{}, script{};
    std::uint32_t kind{};
    bool original_monster_program{};
    std::function<bool(std::uint32_t character_event,ActorId payload,std::string&)> invoke;
};
struct LiveEnemyProviders {
    std::function<bool(ActorId,LiveEnemyBorrow&,std::string&)> borrow;
    std::function<bool(const LiveEnemyBorrow&,SelectedMonsterCallback&,std::string&)> selected;
};
// Call ONLY after the whole Character/CharAI prefix reaches selected AIS.
// Does not discover targets, synthesize native events, or forward the FSM tail.
bool dispatch_live_monster_target(const LiveEnemyProviders&,ActorId,
    std::uint32_t character_event,ActorId payload,std::string&);
// Useful for provider admission and tests. Checks identity/storage coherence;
// it never allocates owners or rewrites source state/target.
bool validate_live_enemy_borrow(const LiveEnemyBorrow&,ActorId,std::string&);
}
