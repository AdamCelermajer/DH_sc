#pragma once
// v21.8 mage: ChainLightning, Cyclone and StoneEscape (prince_mage_chain_lightning / cyclone / stone_escape .luac).
//
// Original: the three Lua skills spawn skill projectiles (SpawnSkillProjectile) or run StartSkillTimerCB timers and
// finish their effect in the RegisterSkillEx `delayed` / `delayed_check` callbacks. This owner executes those
// authored Pre/Use bodies for the cast coordinator (RuntimeSkillExternalOwnerV1) over the shared session projectile
// executor (effects::RuntimeSessionProjectileV1). Results are ordinary same-Session SkillCombatRolls
// (CombatSession::apply_source_result with the ApplyPropClass(CLASS_ID) formula sheet).
//
// Inferences and gaps are marked in the .cpp next to the code and in coordination/v21.8/mage-report.md.
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

struct RuntimeMageSkillServicesV1 {
    // Current shared projectile executor and its source tables (the host rebuilds both per Session).
    std::function<effects::RuntimeSessionProjectileV1*()> projectiles;
    std::function<std::shared_ptr<const dh2::android_ui::SourceProcessArraysV101>()> projectile_tables;
    // Called after every skill projectile launch (including chain hops launched from a delayed callback); the host
    // registers the NativeWorld contact sensor. `wants_contacts` is false for a projectile that never collides.
    std::function<bool(std::uint64_t projectile_id, bool wants_contacts, std::string& error)> projectile_launched;
    // Presentation: AnimatedEffectTable row name / sound name. Failures are diagnostics only, never gameplay failures.
    std::function<bool(const std::string& effect, const std::array<float, 3>& position, std::string& error)> play_effect;
    std::function<bool(const std::string& effect, std::string& error)> stop_effect;
    std::function<bool(const std::string& sound, std::string& error)> play_sound;
    // GameObject::WarpTo (`teleport`) or a carried displacement of a Session actor; the host owns physics/movement.
    std::function<bool(ActorId actor, const std::array<float, 3>& position, bool teleport, std::string& error)> move_actor;
    // StoneEscape destination: the farthest reachable floor point from `from` along `direction` within `max_distance`.
    // Absent: the full distance (no floor information).
    std::function<bool(ActorId actor, const std::array<float, 3>& from, const std::array<float, 3>& direction,
                       float max_distance, std::array<float, 3>& destination, std::string& error)> teleport_path;
    std::function<void(const std::string& line)> trace;
};

class RuntimeMageSkillOwnerV1 final : public RuntimeSkillExternalOwnerV1 {
public:
    explicit RuntimeMageSkillOwnerV1(RuntimeMageSkillServicesV1);

    bool handles(const RuntimeSkillUseProfileV1&) const override;
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;
    bool use(CombatSession&, const RuntimeSkillExternalContextV1&, std::string& error) override;

    // Gameplay clock for the StartSkillTimerCB owners (statue explosion, tornado tick/carry). Call once per
    // unpaused frame after the Session update and the projectile executor update. The timers run on the shared
    // RuntimeSkillTimerServiceV1 (runtime_skill_timer_service_v1.hpp); this owner only keeps the authored state.
    bool advance(CombatSession&, double dt_ms, std::string& error) override;
    void reset() override;
    std::size_t active_tornados() const noexcept { return tornados_.size(); }
    std::size_t pending_statues() const noexcept { return statues_.size(); }
    std::size_t active_timers() const noexcept { return timers_.size(); }

private:
    struct RollContext;
    struct ChainState;
    struct Tornado;
    struct Statue;
    bool use_chain(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_cyclone(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    bool use_stone_escape(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&);
    bool chain_hit(CombatSession&, const std::shared_ptr<ChainState>&, ActorId target, std::string&);
    bool chain_search(CombatSession&, const std::shared_ptr<ChainState>&, ActorId from, std::string&);
    bool launch_chain_projectile(CombatSession&, const std::shared_ptr<ChainState>&, std::int32_t row,
                                 const std::array<float, 3>& origin, ActorId target, std::string&);
    bool tick_tornado(CombatSession&, Tornado&, std::string&);
    bool explode_statue(CombatSession&, Statue&, std::string&);
    bool roll(CombatSession&, RollContext&, ActorId target, DamageEvent& result, std::string&);
    void log(const std::string&) const;

    RuntimeMageSkillServicesV1 services_;
    std::uint64_t next_occurrence_ = 1;
    std::vector<std::shared_ptr<Tornado>> tornados_;
    std::vector<std::shared_ptr<Statue>> statues_;
    std::map<ActorId, std::array<float, 3>> stone_escape_origin_;
    // StartSkillTimerCB clock of this owner (statue TNT_DELAY one-shots, tornado TICK_DELAY loops).
    RuntimeSkillTimerServiceV1 timers_;
    // Session of the advance() call in progress; timer callbacks run only inside advance().
    CombatSession* timer_session_ = nullptr;
};

} // namespace dh::foundation::generic_skills
