#pragma once
#include "../../actor_state.hpp"
#include "../../../level-world/character_buffs.hpp"
#include "../../../level-world/character_native_effects.hpp"
#include "../../../level-world/character_slow_reaction_v1.hpp"
#include "../../../level-world/character_timer_effects.hpp"
#include "../../../level-world/character_skill_combat_v6.hpp"

namespace dh::foundation::status {
// Borrow the genuine character identity and every original owner. The adapter
// never creates properties, a health record, a state machine or a timer clock.
struct Borrow {
 ActorState* actor{};
 std::uintptr_t character{};
 dh2::data::PropertyView* properties{};
 dh2::character::BuffOwner* buffs{};
 dh2::character::TimerStore32* timers{};
 dh2::character::NativeEffects32* effects{};
 const dh2::character::NativeEffectServices16* effect_services{};
 dh2::character::SlowReactionBorrowV1 slow{};
 dh2::character::TimerEffectState32* ticking{};
 const dh2::character::TimerEffectServices16* tick_services{};
 const dh2::character::BuffDictionary16* dot_ids{};
 const dh2::character::BuffDictionary16* dot_fx_ids{};
};
class StatusEffects {
public:
 explicit StatusEffects(Borrow b):b_(b){}
 bool valid(std::string& error) const;
 int add(dh2::character::BuffResult24&,std::int32_t id,std::uint32_t duration,
         std::int32_t capacity,std::uint32_t strength,std::int32_t fx,const char* name);
 int remove(dh2::character::BuffResult24&,std::int32_t id,std::uintptr_t instance);
 int dot(dh2::character::BuffResult24&,std::int32_t duration,std::int32_t amount,std::int32_t element);
 // Called synchronously by SAME TimerStore's event54 delivery. No retained
 // expired pointers or duplicate-event ledger is installed.
 int expired(dh2::character::BuffResult24&,const dh2::character::Timer32*);
 // Recovered event0x33/0x34 bodies; source timer creation and dt remain external.
 // Calculate/apply services must run genuine full F_ApplyResult, including
 // leech and source reaction/audio/text tails. Missing services fail explicitly.
 int tick(dh2::character::TimerEffectResult24&,std::uint32_t event);
 // 1 handled (includes source rejection),0 unrelated,negative required failure.
 int application(const dh2::character::skills::SkillApplyRequestV6&,
                 dh2::character::skills::SkillApplyResponseV6&);
 // Source death RemoveAllBuffs entry. Attack interruption alone leaves timed
 // buffs intact. Host stops source timer delivery before teardown/unbinding.
 int remove_all(dh2::character::BuffResult24&);
 const std::string& error() const noexcept{return error_;}
 dh2::character::TimerStore32* timer_store()const noexcept{return b_.timers;}
private:
 bool enter();
 Borrow b_;bool busy_{};std::string error_;
 struct Scope {StatusEffects& self;~Scope(){self.busy_=false;}};
};
}
