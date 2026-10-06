#pragma once
#include "character_native_fsm.hpp"
#include <array>
#include <vector>
namespace dh2::character {
// Source addresses below are provenance/dispatch keys, never executable native
// pointers. State behaviors are shared; StateInfo/event storage is per Character.
struct StateOwnerEvent16 {std::int32_t event,target;std::uint32_t predicate,adjustment;};
struct StateOwnerInfo40 {
 std::int32_t id;std::uint32_t singleton,focus,blur,update,on_event;
 const StateOwnerEvent16* events;std::uint32_t event_count,reserved;
};
struct StateOwnerMachine40 {
 NativeFsm24* fsm;const StateOwnerInfo40* states;
 std::uint32_t state_count;std::int32_t current_index;
 std::uintptr_t physical;std::uint64_t reserved;
};
enum StateOwnerOperation:std::uint32_t {
 state_owner_blur=1,state_owner_focus,state_owner_event,state_owner_predicate,
 state_owner_character_event,state_owner_pin,state_owner_profile_begin,state_owner_profile_end
};
struct StateOwnerRequest48 {
 std::uint32_t operation,source_function;std::int32_t state,other;
 std::uint32_t event,adjustment;std::uintptr_t character,payload;
 std::uint32_t reserved[2];
};
struct StateOwnerResponse8 {std::int32_t next;std::uint32_t accepted;};
// Live source Character+520/+528/+544/+441/+442 projections. They are not
// inferred from animation or target selection. Byte values retain 0..255.
struct StateOwnerPredicateFacts16 {
 std::uint32_t flags,mask;std::int32_t interaction;
 std::uint8_t can_interrupt,stopped_attacking,reserved[2];
};
struct StateOwnerServices16 {
 void* context;
 //0 delivered; nonzero means unavailable/failed source backend. Synchronous
 // calls may reenter the same owner. Keep owner/services/storage alive.
 int(*invoke)(void*,StateOwnerMachine40*,const StateOwnerRequest48*,StateOwnerResponse8*);
};
static_assert(sizeof(StateOwnerEvent16)==16&&sizeof(StateOwnerInfo40)==40);
static_assert(sizeof(StateOwnerMachine40)==40&&sizeof(StateOwnerRequest48)==48);
static_assert(sizeof(StateOwnerResponse8)==8&&sizeof(StateOwnerServices16)==16);
static_assert(sizeof(StateOwnerPredicateFacts16)==16);
class CharacterStateOwner {
 State state_{};NativeFsm24 fsm_{};
 std::array<std::vector<StateOwnerEvent16>,20> events_;
 std::array<StateOwnerInfo40,20> infos_{};StateOwnerMachine40 machine_{};
public:
 explicit CharacterStateOwner(std::uintptr_t character);
 CharacterStateOwner(const CharacterStateOwner&)=delete;
 CharacterStateOwner& operator=(const CharacterStateOwner&)=delete;
 CharacterStateOwner(CharacterStateOwner&&)=delete;
 CharacterStateOwner& operator=(CharacterStateOwner&&)=delete;
 State& state() noexcept{return state_;}
 NativeFsm24& native_fsm() noexcept{return fsm_;}
 StateOwnerMachine40& machine() noexcept{return machine_;}
 const StateOwnerInfo40* info(std::int32_t id) const noexcept;
 //Do not directly select state.current: transition owns nullable StateInfo.
 int initialize_level(std::int32_t preset,const StateOwnerServices16&);
 int transition(std::int32_t next,std::int32_t event,std::uintptr_t payload,const StateOwnerServices16&);
 int event(std::int32_t event,std::uintptr_t payload,const StateOwnerServices16&);
};
}
extern "C" {
//1 completed,0 no matching event/rejected predicate,-1 malformed pre-mutation,
//-2 required service failed at its delivered prefix,-3 source would dereference
//a null current or invalidated event lookup after a callback. No rollback of
//already delivered effects. Required method callbacks are real backends; source
//metadata alone does not implement the twenty virtual behavior bodies.
int dh2_character_state_owner_transition(dh2::character::StateOwnerMachine40*,std::int32_t next,std::int32_t event,std::uintptr_t payload,const dh2::character::StateOwnerServices16*);
int dh2_character_state_owner_event(dh2::character::StateOwnerMachine40*,std::int32_t event,std::uintptr_t payload,const dh2::character::StateOwnerServices16*);
int dh2_character_state_owner_initialize_level(dh2::character::StateOwnerMachine40*,std::int32_t preset,const dh2::character::StateOwnerServices16*);
//Recovered all20 OnInit registrations in SOURCE CALL order, not map order.
std::uint32_t dh2_character_state_owner_registration_count();
int dh2_character_state_owner_registration(std::uint32_t index,std::int32_t* state,dh2::character::StateOwnerEvent16*);
// Seven recovered pure source CSM predicates. Keeps response.next unchanged
// except AfterInteraction with interaction4, which writes18. Spawn3ad2e4 and
// unknown source functions return-2 unchanged (required deeper service).
int dh2_character_state_owner_predicate(dh2::character::StateOwnerResponse8*,std::uint32_t source_function,std::int32_t current,const dh2::character::StateOwnerPredicateFacts16*);
}
