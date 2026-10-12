#pragma once
#include "character_native_fsm.hpp"
namespace dh2::character {
// Native table projection: source CharAnim Stunned+8c / Scared+7c values.
// Borrowed row arrays and current FSM/Character remain alive during calls.
// FSM/State binding remains stable; fields and Character identity may mutate
// synchronously at source reload points. No borrowed receiver may be destroyed.
struct NativeEffects32 {NativeFsm24* fsm;const std::int32_t* stunned;const std::int32_t* scared;std::uint32_t count,reserved;};
enum NativeEffectService:std::uint32_t {effect_ai_flags,effect_animation_index,effect_start_timer,effect_stance_bits,effect_anim_stance,effect_force_state,effect_state_event,effect_stop_loop,effect_pin,effect_heading_object,effect_set_animation,effect_is_player,effect_cancel_sneaking,effect_unpin,effect_random,effect_heading_point};
struct NativeEffectRequest32 {std::uint32_t service,argument0,argument1,argument2;std::uintptr_t character,payload;};
struct NativeEffectServices16 {void* context;int(*invoke)(void*,NativeEffects32*,const NativeEffectRequest32*,std::uint32_t* response);};
static_assert(sizeof(NativeEffects32)==32&&sizeof(NativeEffectRequest32)==32&&sizeof(NativeEffectServices16)==16);
// AI flags response is the actual CharAI row+14 bits (boss mask0x4, bit2).
// animation_index response is authored Character+1000 signed bits; native
// source getter applies invalid-index fallback17 before the effect range gate.
// stance_bits queries exact constants ('AnimStancedAnim','SL__LIST_IPHONE').
// anim_stance responds with signed bits. Timer args duration,repeat0,event,
// payload0; its delivered response is ignored, including UINT_MAX.
// force_state args state,event,0; state_event args event,0,0; both retain
// supplied payload. Required services own actual registered state methods;
// this adapter never assigns current to manufacture transition acceptance.
// Any nonzero provider status stops at the delivered prefix with -2.
// random requests argument0 bound; response is the genuine source random
// result. heading_point args are exact XYZ float words. set_animation arg0
// selects the recomputed sequence via genuine CharAnimator.ANIM_Set.
}
extern "C" {
// kind0stun,1scare.1delivered,0source rejected,-1malformed,-2native service failure.
int dh2_character_native_effect_set(dh2::character::NativeEffects32*,std::uint32_t kind,std::uint32_t duration,std::uint32_t mode,std::uintptr_t payload,std::uint32_t force,const dh2::character::NativeEffectServices16*);
// Recovered simple virtual bodies only: operation0OnUpdate,1OnBlur.
// Stun update clears idle_suppressed and directly requests Idle when mask2
// expired. Scare update requests StopLoop(false) when mask4 expired.
// Blur clears controller lock (stun) or heading target (scare), then pins a
// present body. Focus/random heading/OnInit/factory ownership are separate.
int dh2_character_native_effect_body(dh2::character::NativeEffects32*,std::uint32_t kind,std::uint32_t operation,const dh2::character::NativeEffectServices16*);
// Recovered OnFocus. Flags2202(stun)/2240(scare), source table/stance selection,
// optional player controller lock, randomized scare heading, CancelSneaking,
// then unpin reloaded present body. No registered-state allocation/lookup.
int dh2_character_native_effect_focus(dh2::character::NativeEffects32*,std::uint32_t kind,const dh2::character::NativeEffectServices16*);
// Actual Stunned OnEvent is empty. Scared event23 refreshes randomized heading;
// all other Scared events have an empty body (registered transitions separate).
int dh2_character_native_effect_event(dh2::character::NativeEffects32*,std::uint32_t kind,std::uint32_t event,const dh2::character::NativeEffectServices16*);
}
