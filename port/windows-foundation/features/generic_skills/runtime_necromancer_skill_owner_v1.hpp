#pragma once
// v21.8 w2-necromancer: FireSkullRain, Pestilence and Rise (prince_necromancer_fireskullrain / pestilence / rise .luac).
//
// The cast coordinator keeps everything it owns for every skill (admission, HasMana/UseMana, cooldown prefix, LookAt,
// ClearTarget). For these profile rows (external_owner) the authored OnPreSkill_ / OnSkill_ bodies run here, over the
// same facts the Lua reads: the Use-time ApplyPropClass(CLASS_ID) sheet, the population order and the target facts.
//
//   FireSkullRain  OnSkill_ starts StartSkillTimerCB(1000, true, DamageTick) once. DamageTick searches 280/360 around the
//                  caster and rolls every enemy Character; the tick after the third roll stops the loop (MAX_TICKS 2:
//                  rolls at tick_count 0, 1, 2). An empty search at any tick stops the loop. The first tick plays the
//                  METEOR FX on the first target, or at caster + (160,160,0) when the search is empty.
//   Pestilence     OnSkill_ rolls every Character of the OnPre list not yet infected and starts a one-shot removal
//                  timer (SnS_Dot_Duration + 1000 ms). A 500 ms loop polls the infected victims (death watch); a dead
//                  victim explodes (200/360 around the victim, ClosestFirst, PlayFX + PlaySound, a roll for each) and is
//                  removed. Objects in the OnPre list are not rolled here (see the report).
//   Rise           OnPre (DoRise) launches the TeleportTool skill projectile. Its landing position (the expiry or the
//                  first accepted contact) is recorded; OnSkill_ spawns nothing because the summon is DEFERRED (no runtime
//                  summon in the port). The four-direction HasPath probe is not implemented: only the front projectile.
//
// Gaps and inferences are documented next to the code and in coordination/v21.8/w2-necromancer-report.md.
#include "runtime_skill_death_watch_v1.hpp"
#include "runtime_skill_external_owner_v1.hpp"
#include "runtime_skill_timer_service_v1.hpp"
#include "../effects/runtime_session_projectile_v1.hpp"

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::generic_skills {

struct RuntimeNecromancerSkillServicesV1 {
    std::function<effects::RuntimeSessionProjectileV1*()> projectiles;
    std::function<std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>()> projectile_tables;
    std::function<bool(std::uint64_t projectile_id, bool wants_contacts, std::string& error)> projectile_launched;
    // Presentation (AnimatedEffectTable name at a point / PlaySound by name). Failures are diagnostics only.
    std::function<bool(const std::string& effect, const std::array<float, 3>& position, std::string& error)> play_effect;
    std::function<bool(const std::string& sound, std::string& error)> play_sound;
    std::function<void(const std::string& line)> trace;
};

class RuntimeNecromancerSkillOwnerV1 final : public RuntimeSkillExternalOwnerV1 {
public:
    explicit RuntimeNecromancerSkillOwnerV1(RuntimeNecromancerSkillServicesV1);

    bool handles(const RuntimeSkillUseProfileV1&) const override;
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    // Gameplay clock: the FireSkullRain tick loop, the Pestilence death-watch loop and the infection removal timers.
    bool advance(CombatSession&, double dt_ms, std::string& error) override;
    // Rise's OnSkillCleanUp (KillAllSummoned + counters). Nothing is summoned in the port, so only the landing state drops.
    void cleanup(CombatSession&, ActorId actor) override;
    void reset() override;

    std::size_t active_fire_skull_rains() const noexcept { return fire_rains_.size(); }
    std::size_t infected_victims() const noexcept { return infected_.size(); }
    bool rise_landing_known(ActorId caster) const;

    // Shared per-cast facts the authored bodies read (Use-time ApplyPropClass sheet, population, target facts).
    struct CastSnapshot;

private:
    struct FireRain;
    struct Victim;
    bool use_fire_skull_rain(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_pestilence(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_rise(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    bool fire_skull_rain_tick(CombatSession&, const std::shared_ptr<FireRain>&, std::string&);
    bool pestilence_poll(CombatSession&, std::string&);
    bool pestilence_explode(CombatSession&, const Victim&, std::string&);
    bool roll(CombatSession&, CastSnapshot&, ActorId target, std::string& error);
    bool launch_rise_projectile(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    void log(const std::string&) const;

    RuntimeNecromancerSkillServicesV1 services_;
    std::uint64_t next_occurrence_ = 1;
    std::map<ActorId, std::shared_ptr<FireRain>> fire_rains_;
    std::map<ActorId, std::shared_ptr<Victim>> infected_;   // keyed by victim, tracking order = map order
    RuntimeSkillDeathWatchV1 death_watch_;
    std::map<ActorId, std::array<float, 3>> rise_landing_;  // Rise: landing position per caster for the current Use
    RuntimeSkillTimerServiceV1 timers_;
    RuntimeSkillTimerIdV1 pestilence_poll_timer_ = 0;
    CombatSession* timer_session_ = nullptr;
    static constexpr std::uint64_t kOwnerFire = 1, kOwnerPestilence = 2, kOwnerRise = 3;
};

} // namespace dh::foundation::generic_skills
