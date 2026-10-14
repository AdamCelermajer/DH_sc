#pragma once
// v21.8 w4-grapple: external owner of the Rogue-tree skills whose Lua body runs in a `delayed` projectile callback.
// Admitted here: GrapplingHook (prince_assassin_grapling_hook.luac).
//
// Lua OnSkill_: PlaySound("sfx_skill_rogue_ass_throwing_knife") and SpawnSkillProjectile(GrapplingHook) with no target: the
// knife flies along the caster's heading (OnPre set it with LookAt(top target)). Lua OnDelayedSkill_(target), called by the
// shared projectile executor on a hit (`delayed(peer)`) or on expiry (`delayed(nil)`, nothing to do):
//   - a Character: SetProp(SnS_Level) + ApplyPropClass(CLASS) + SkillCombatRoll(target) (the roll, once per hit);
//     target:SetStunState(duration, true) unless IsBoss or IsMiniBoss (refused inside the status owner);
//     if the target is now Stunned and is not a boss/miniboss: target:HeadTo(GetPosition()), the pull toward the caster.
//   - an object: SkillCombatRoll(target) only.
//
// Shared pieces used: the projectile executor launch_skill (ES-RUNTIME item 5), the shared roll path
// (CombatSession::apply_source_result), the ES-STATUS stun through the host's script-status service, and the general forced
// move (forced_head_to_v1.hpp: Cmd_HeadTo as a per-frame command stepped by advance()).
//
// Gaps, stated in the report: the Use-time formula sheet is reused for the delayed roll (the Lua re-applies the class at
// delayed time; the caster's sheet does not change during the flight in practice); the pull speed and the arrival radius are
// not in the Lua (its TODO says "need to speed up the pulling"), so they are constants of this owner.
#include "forced_head_to_v1.hpp"
#include "runtime_skill_cast_coordinator_v1.hpp"
#include "runtime_skill_external_owner_v1.hpp"
#include "../effects/runtime_session_projectile_v1.hpp"

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace dh::foundation::generic_skills {

struct RuntimeRogueSkillServicesV1 {
    // Current shared projectile executor and its source tables (the host rebuilds both per Session).
    std::function<effects::RuntimeSessionProjectileV1*()> projectiles;
    std::function<std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>()> projectile_tables;
    // Registers the NativeWorld contact sensor of a launched projectile.
    std::function<bool(std::uint64_t projectile_id, bool wants_contacts, std::string& error)> projectile_launched;
    // Named sound (the Lua PlaySound label), diagnostic-only failure.
    std::function<bool(const std::string& sound, std::string& error)> play_sound;
    // Lua target:SetStunState(duration_ms, true). Bosses and minibosses are refused inside the owner: applied = false.
    std::function<bool(ActorId target, double duration_ms, bool& applied, std::string& error)> set_stun;
    // Lua target:GetState() == AIStates.Stunned (ES-STATUS state).
    std::function<bool(ActorId target)> is_stunned;
    // Lua IsBoss(target) or IsMiniBoss(target).
    std::function<bool(ActorId target)> is_boss_or_miniboss;
    // Host move of a live actor (the actor-teleport verb). The forced move owner steps positions through it.
    std::function<bool(ActorId actor, const std::array<float, 3>& position, std::string& error)> move_actor;
    std::function<void(const std::string& line)> trace;
};

class RuntimeRogueSkillOwnerV1 final : public RuntimeSkillExternalOwnerV1 {
public:
    explicit RuntimeRogueSkillOwnerV1(RuntimeRogueSkillServicesV1);

    bool handles(const RuntimeSkillUseProfileV1&) const override;
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    // Gameplay clock of the forced pull (Cmd_HeadTo commands), once per frame after the session update.
    bool advance(CombatSession&, double dt_ms, std::string& error) override;
    void reset() override;
    std::size_t active_pulls() const noexcept { return pulls_.size(); }

    // Constants of the pull (not in the Lua; see the header note).
    static constexpr float kPullSpeedPerSecond = 250.0f;

private:
    struct Cast;
    ForcedHeadToServicesV1 pull_services();
    bool launch_knife(CombatSession&, const std::shared_ptr<Cast>&, std::string& error);
    bool roll(CombatSession&, const std::shared_ptr<Cast>&, ActorId target, DamageEvent& result, std::string& error);
    bool on_delayed(CombatSession&, const std::shared_ptr<Cast>&, const effects::RuntimeSessionSkillProjectileEventV1&,
                    std::string& error);
    void log(const std::string& line) const;

    RuntimeRogueSkillServicesV1 services_;
    std::uint64_t next_occurrence_ = 1;
    std::map<ActorId, std::shared_ptr<Cast>> casts_;
    ForcedHeadToV1 pulls_;               // Cmd_HeadTo commands of the delayed body (stepped in advance)
    std::map<ActorId, ActorId> pull_casters_;  // pulled actor -> caster (diagnostics; the goal is pull_goals_)
    std::map<ActorId, std::array<float, 3>> pull_goals_;  // pulled actor -> the caster position fixed at the hit
    std::map<std::uint64_t, ActorId> knives_;  // live knives (projectile id -> caster), for the flight trace
    std::uint64_t pull_frames_ = 0;      // advance() calls, for the sampled pull trace
    CombatSession* session_ = nullptr;   // the session of the advance() call in progress
};

} // namespace dh::foundation::generic_skills
