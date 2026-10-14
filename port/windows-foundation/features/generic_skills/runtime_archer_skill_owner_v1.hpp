#pragma once
// v21.8 w2-archer: the five Archer actives (prince_archer_exploding_arrows / multihit / pin_shot / poison_arrows /
// volley .luac) as an external owner registered in RuntimeSkillExternalOwnerRegistryV1.
//
// Original: every script spawns skill arrows with SpawnSkillProjectile (IDA CharAISkillScript::SpawnProjectile 0x3da510:
// ProjectileManager::Spawn(row, owner, target = nullptr, _ProjectileCheck, _ProjectileHit): the arrow flies along the
// OWNER'S HEADING, which OnPreSkill_/OnSkill_ set with LookAt(target)) and finish their effect in the RegisterSkillEx
// `delayed` callback (SetProp(SnS_Level) + ApplyPropClass(CLASS_ID) + SkillCombatRoll) or in StartSkillTimerCB timers.
// This owner runs those authored bodies for the cast coordinator over the shared projectile executor
// (effects::RuntimeSessionProjectileV1::launch_skill), the shared skill timer service and the shared roll path
// (CombatSession::apply_source_result with the ApplyPropClass formula sheet). The generic coordinator keeps SkillTable
// admission (HasBow via the profile's requires_bow), HasMana/UseMana, SetSkillCooldown, the target query, the first LookAt,
// the Skill6 sequence, per-do_skill-marker re-entry of Use (MultiHitArrow fires three) and Post ClearTarget.
//
// Inferences and gaps are marked in the .cpp next to the code and in coordination/v21.8/w2-archer2-report.md.
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

struct RuntimeArcherSkillServicesV1 {
    // Current shared projectile executor and its source tables (the host rebuilds both per Session).
    std::function<effects::RuntimeSessionProjectileV1*()> projectiles;
    std::function<std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>()> projectile_tables;
    // Registers the NativeWorld contact sensor of a launched projectile.
    std::function<bool(std::uint64_t projectile_id, bool wants_contacts, std::string& error)> projectile_launched;
    // Presentation (diagnostic-only failures): AnimatedEffectTable row name at a world position / named sound.
    std::function<bool(const std::string& effect, const std::array<float, 3>& position, std::string& error)> play_effect;
    std::function<bool(const std::string& sound, std::string& error)> play_sound;
    // Lua target:SetStunState(duration_ms, true) (ES-STATUS owner; bosses are immune inside it).
    std::function<bool(ActorId target, double duration_ms, std::string& error)> set_stun;
    // Lua target:SetKnockBackState(great) (the session Push consumer).
    std::function<bool(CombatSession&, ActorId attacker, ActorId target, bool great, std::string& error)> knock_back;
    // Lua GetRand(0, 100): uniform integer in [0, bound) drawn from the shared Session RNG.
    std::function<bool(std::uint32_t bound, std::uint32_t& value, std::string& error)> random;
    std::function<void(const std::string& line)> trace;
};

class RuntimeArcherSkillOwnerV1 final : public RuntimeSkillExternalOwnerV1 {
public:
    explicit RuntimeArcherSkillOwnerV1(RuntimeArcherSkillServicesV1);

    bool handles(const RuntimeSkillUseProfileV1&) const override;
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    // Gameplay clock for the StartSkillTimerCB timers (ExplodingArrows DoExplosion, PinShot second/third arrow).
    bool advance(CombatSession&, double dt_ms, std::string& error) override;
    void reset() override;
    std::size_t active_timers() const noexcept { return timers_.size(); }

private:
    struct Cast;
    struct RollContext;
    Cast& cast_for(const RuntimeSkillExternalContextV1&);
    ActorId pop_target(Cast&, const RuntimeSkillExternalContextV1&) const;
    bool launch(CombatSession&, const std::shared_ptr<Cast>&, const char* row_name, const std::array<float, 3>& origin,
                const std::array<float, 3>& forward, ActorId explicit_target,
                effects::RuntimeSessionSkillProjectileDelayedV1 delayed, std::uint64_t& projectile_id, std::string& error);
    bool launch_from_heading(CombatSession&, const std::shared_ptr<Cast>&, const char* row_name,
                             effects::RuntimeSessionSkillProjectileDelayedV1 delayed, std::uint64_t& projectile_id,
                             std::string& error);
    bool look_at(CombatSession&, ActorId caster, ActorId target, std::string& error);
    bool roll(CombatSession&, const std::shared_ptr<Cast>&, ActorId target, DamageEvent& result, std::string& error);
    bool use_exploding(CombatSession&, const std::shared_ptr<Cast>&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_multihit(CombatSession&, const std::shared_ptr<Cast>&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_pin_shot(CombatSession&, const std::shared_ptr<Cast>&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_simple(CombatSession&, const std::shared_ptr<Cast>&, const RuntimeSkillExternalContextV1&, const char* row,
                    std::string&);
    bool use_volley(CombatSession&, const std::shared_ptr<Cast>&, const RuntimeSkillExternalContextV1&, std::string&);
    bool explode(CombatSession&, const std::shared_ptr<Cast>&, ActorId target, std::string&);
    bool do_rebound(CombatSession&, const std::shared_ptr<Cast>&, ActorId origin, std::string&);
    void log(const std::string&) const;

    RuntimeArcherSkillServicesV1 services_;
    std::uint64_t next_occurrence_ = 1;
    // Lua file-level state of the five scripts, kept per caster (the Lua is per Character; one local player here).
    std::map<ActorId, std::shared_ptr<Cast>> casts_;
    RuntimeSkillTimerServiceV1 timers_;
    // Session of the advance() call in progress; timer callbacks run only inside advance().
    CombatSession* timer_session_ = nullptr;
};

} // namespace dh::foundation::generic_skills
