#pragma once

#include "actor_state.hpp"
#include <map>
#include <optional>
#include <set>
#include <tuple>

namespace dh::foundation {

struct MarkerOccurrence;

enum class AttackGeometry { melee_radius, ranged_band };
enum class CooldownTiming { attack_start, attack_departure };
struct DamageMarkerBinding {
    std::string marker_name;
    // Identifies recovered weapon/skill/formula data; never an implicit damage default.
    std::string damage_source_id;
};
struct AttackDefinition {
    std::string id;
    std::string animation_clip_id;
    AttackGeometry geometry = AttackGeometry::melee_radius;
    // Authored scene units. Melee maximum is attacker reach; target radius is added.
    // Original melee upper boundary is strict; ranged min/max are inclusive.
    float minimum_range = 0.0f;
    float maximum_range = 0.0f;
    double cooldown_seconds = 0.0;
    std::vector<DamageMarkerBinding> damage_markers;
    // Compatibility defaults to start. Recovered AI.AttackDelay must explicitly
    // select departure: CSAttack::OnBlur starts timer42 even on interruption.
    CooldownTiming cooldown_timing = CooldownTiming::attack_start;
};

// The host supplies stable actor lookup and recovered content policies. These
// queries must not erase actors, reenter combat, or mutate state during a call.
class CombatWorld {
public:
    virtual ~CombatWorld() = default;
    virtual ActorState* find_actor(ActorId id) = 0;
    virtual bool eligible_target(const ActorState& attacker, const ActorState& target) const = 0;
    virtual float target_radius(const ActorState& target) const = 0;
    // Return false when the authored formula/data is unavailable. Returning true
    // requires a finite nonnegative final damage amount (after defense/resistance).
    virtual bool resolve_damage(const std::string& damage_source_id,
                                const ActorState& attacker, const ActorState& target,
                                const std::string& marker_name,
                                float& amount, std::string& error) const = 0;
    // Optional source result bits travel with the same resolved hit. The legacy
    // float-only provider remains valid, but an unknown outcome must not be
    // treated as an authored Injure result.
    virtual bool resolve_damage_with_outcomes(const std::string& damage_source_id,
                                const ActorState& attacker, const ActorState& target,
                                const std::string& marker_name, float& amount,
                                std::optional<std::uint32_t>& outcomes,
                                std::optional<std::uint32_t>& source_mask,
                                std::string& error) const {
        outcomes.reset();
        source_mask.reset();
        return resolve_damage(damage_source_id,attacker,target,marker_name,amount,error);
    }
};

struct DamageEvent {
    bool applied = false;
    ActorId attacker = invalid_actor_id;
    ActorId target = invalid_actor_id;
    std::string attack_id;
    std::string marker_name;
    std::string source_id;
    float requested_damage = 0.0f;
    float health_removed = 0.0f;
    bool target_died = false;
    std::optional<std::uint32_t> source_outcomes;
    std::optional<std::uint32_t> source_mask;
};

bool validate_attack_definition(const AttackDefinition&, std::string& error);
bool attack_target_in_range(const AttackDefinition&, const ActorState& attacker,
                            const ActorState& target, float target_radius) noexcept;

class CombatSystem {
public:
    explicit CombatSystem(CombatWorld& world) : world_(world) {}
    // Side-effect-free admission for a caller that must run accepted transition
    // effects before committing action/target/cooldown. begin rechecks the same
    // rules; no ticket can bypass live target or cooldown validation.
    bool validate_begin(ActorId attacker, ActorId target, const AttackDefinition&,
                        std::uint64_t animation_generation,std::string& error);
    // Generation must be the selected animation cursor generation. An actor
    // must explicitly list this attack ID. One shared path serves every actor.
    bool begin(ActorId attacker, ActorId target, const AttackDefinition&,
               std::uint64_t animation_generation, std::string& error);
    // Authored events exclusively trigger hits. Duplicate deliveries and events
    // from an older animation generation are harmless. No update timer hits.
    bool consume_marker(ActorId attacker, const MarkerOccurrence&,
                        DamageEvent& result, std::string& error);
    bool consume_marker(ActorId attacker,const MarkerOccurrence&,DamageEvent&,
                        std::string& error,bool defer_death_state);
    // Applies a source-calculated, already validated same-world hit exactly
    // once. This does not rerun formula/RNG/range or change attack/cooldown state.
    bool apply_calculated_hit(ActorId attacker, ActorId target,
                              const std::string& source_id,
                              const std::string& marker_name, float amount,
                              std::optional<std::uint32_t> outcomes,
                              std::optional<std::uint32_t> source_mask,
                              DamageEvent& result, std::string& error);
    bool apply_calculated_hit(ActorId attacker,ActorId target,
        const std::string& source_id,const std::string& marker_name,float amount,
        std::optional<std::uint32_t> outcomes,std::optional<std::uint32_t> source_mask,
        DamageEvent& result,std::string& error,bool defer_death_state);
    bool finish(ActorId attacker, std::uint64_t animation_generation);
    bool interrupt(ActorId attacker);
    // Observed external interruption belongs to the start of this time interval.
    // Advance in chronological segments when departures occur inside a frame.
    // Dead actors lose their gate (original dead focus clears attack gate bits).
    // Invalid elapsed time preserves all records. AI must check this gate through
    // cooldown_remaining as well as action eligibility before requesting attacks.
    bool update(double elapsed_seconds, std::string& error);
    void clear();
    bool attacking(ActorId attacker) const noexcept;
    bool active_target_valid(ActorId attacker);
    double cooldown_remaining(ActorId attacker) const noexcept;

private:
    struct AttackRun {
        ActorId target = invalid_actor_id;
        AttackDefinition definition;
        std::uint64_t generation = 0;
        std::set<std::tuple<std::uint64_t, std::uint64_t, std::size_t, std::size_t>> delivered;
    };
    struct ActorCombat {
        double cooldown = 0.0;
        std::optional<AttackRun> attack;
    };
    bool valid_pair(ActorId attacker, ActorId target, ActorState*& owner, ActorState*& victim);
    void cleanup_invalid();
    CombatWorld& world_;
    std::map<ActorId, ActorCombat> actors_;
};

} // namespace dh::foundation
