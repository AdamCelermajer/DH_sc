#pragma once
#include "character_ai_initialization.hpp"
#include "character_ai_set_target.hpp"
#include "character_aggro_cleanup.hpp"
#include "character_coordinator.hpp"
#include "character_sm_set_dead_state_v1.hpp"
#include <string>

namespace dh2::player_ai_death_v1 {
enum class Direction : std::uint32_t {outgoing,incoming};
enum class Operation : std::uint32_t {
 group_died,ais_died,anim_stance,
 relations,retain_peer,contains_mirror,erase_mirror,clear_relations,
 notify_deaggro,release_peer,skill_cleanup,spell_cleanup
};
struct Request {
 Operation operation{};Direction direction{};
 std::uintptr_t ai=0,character=0,subject=0,killer=0;
 void* lifetime_handle=nullptr;
};
struct Reply {
 std::int32_t word=0;std::uint32_t count=0;
 const character_aggro_cleanup::PeerRef* peers=nullptr;
 void* lifetime_handle=nullptr;
};
struct Backend {
 void* context=nullptr;
 // Zero means the actual reached service completed. Nonzero/throw fails
// after its completed effects. release_peer must release the hold even on
// error; it is port lifetime machinery, not an additional source AI action.
 int (*invoke)(void*,const Request*,Reply*,std::string& error)=nullptr;
};
struct Bindings {
 character_ai_initialization::State* ai=nullptr;
 character::Coordinator* coordinator=nullptr;
 character::set_target::OwnerFacts* target_owner=nullptr;
 const character::set_target::Services* target_services=nullptr;
 // Reuse the same Character/Coordinator and the canonical SM_SetDeadState
 // selector; this is a borrowed view, not another state owner.
 character_sm_set_dead_state_v1::Bindings dead_state{};
 // Full-width canonical GroupInfo slot. Original State.word_34 cannot carry
// a native pointer. The genuine constructor produces null; nonnull needs
// actual GroupInfo::OnDied through Backend, before the active AIS is reread.
 const std::uintptr_t* group_identity=nullptr;
 std::uintptr_t player_ais_identity=0;
 Backend backend{};
};
struct Result {
 std::uint32_t calls=0,last_operation=0,group_calls=0,ais_calls=0,
 default_ais_died=0,target_completed=0,sync_completed=0,
 animation_table_valid=0,state_completed=0,timer_stops=0,timer_ids_cleared=0,
 outgoing_completed=0,incoming_completed=0,skill_completed=0,spell_completed=0;
 std::int32_t death_animation=-1,despawn_animation=-1;
 character_aggro_cleanup::Result outgoing{},incoming{};
};
enum class Status {complete,invalid_argument,busy,failed};
class Runtime {
 Bindings bindings_;bool busy_=false;
public:
 explicit Runtime(Bindings);
 Status died(std::uintptr_t killer,Result*,std::string& error);
};
// Borrow actual associated CharAI/Character/Coordinator/target/SM backing on
// one owning thread. died delivers only CharAI::OnDied -> AI_SetDead, not the
// separate Character::Kill/rewards or outer RaiseAIEvent machine forwarding.
// The known Player/PlayerIPhone AIS inherits the real empty OnDied leaf. An
// unknown active AIS needs Backend::ais_died; no Lua OnDied is fabricated.
// dead_state binds the same canonical Character animation tables/constants
// and Coordinator consumed by character_sm_set_dead_state_v1. AI_SetDead's
// exact source call is {dead=false,killer=nullptr,force_transition=true};
// only reached Player Character::GetAnimStance calls use Backend::anim_stance.
// relations returns current source-order peers and count of the sole map,
// coherent with ai.tree_7c.count/outgoing or tree_94.count/incoming. Empty must
// be real. For incoming(false), contains/erase operate on PEER outgoing; clear
// acts on OWNER incoming; notify dispatches OWNER.OnDeAggro(peer). Outgoing
// uses PEER incoming, OWNER outgoing, then PEER.OnDeAggro(owner). Retained
// peers and genuine callbacks are required for nonempty maps.
// Skill/spell operations borrow the sole same-Session cleanup adapter. All
// reached missing providers fail with retained effects; no blanket clear,
// extra timer store, dead regen gate, AIS termination or rollback is inserted.
}
