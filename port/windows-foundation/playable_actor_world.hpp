#pragma once
#include "combat_system.hpp"
#include "original_combat_properties.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/loot_tables_v2.hpp"
#include <functional>
#include <utility>

namespace dh::foundation {
class AssetCatalog;
bool load_original_ai_tables(const AssetCatalog&, const std::string& table_root,
                             dh2::data::AiTables&, std::string& error);
struct PlayableActorTraits {
    // Must come from original actor ownership/type; faction does not imply it.
    bool is_player = false;
    bool targetable = true;
    std::optional<dh2::data::ItemRecord164> main_item;
};
struct PlayableCombatResolution {
    ActorId attacker = invalid_actor_id, victim = invalid_actor_id;
    std::string source_id, marker_name;
    OriginalMeleeResolution melee;
};
struct PersistedPlayableActor {
    ActorState actor;
    OriginalCombatProperties combat;
    PlayableActorTraits traits;
};
// Stable shared live registry and bridge to original combat data. No presets,
// map-specific policy, invented faction rule, or invented random stream.
class PlayableActorWorld final : public CombatWorld {
public:
    // RNG reference must outlive world; host supplies original stream/seed.
    PlayableActorWorld(dh2::data::AiTables tables, dh2::data::CombatRandom& random);
    bool bind_actor(ActorState, OriginalCombatProperties, PlayableActorTraits,
                    std::string& error);
    bool remove_actor(ActorId);
    // Non-character objects share this world's ID namespace and lifetime.
    // No character property sheets, combat vitals or faction are fabricated.
    bool bind_object(WorldObject,std::string& error);
    // Same live object storage. Identity/model/placement changes should use the
    // world APIs; feature codecs may update their own bounded component bytes.
    // Reacquire after restore/replacement; pin the current Session lease.
    WorldObject* find_object(ObjectId id)noexcept;
    const WorldObject* find_object(ObjectId id)const noexcept;
    bool remove_object(ObjectId);
    bool set_object_transform(ObjectId,const Transform&);
    bool set_object_visual(ObjectId,VisualReference);
    bool set_object_component(ObjectId,std::string key,std::vector<std::uint8_t>,std::string& error);
    const std::map<ObjectId,WorldObject>& objects()const noexcept{return objects_;}
    // Drops actors/events; retains source registrations and host RNG state.
    void clear();
    bool bind_source(std::string id, OriginalMeleeSource, std::string& error);
    bool update_combat_properties(ActorId, OriginalCombatProperties,
                                 PlayableActorTraits, std::string& error);
    ActorState* find_actor(ActorId) override;
    const ActorState* find_actor(ActorId) const;
    const std::map<ActorId, ActorState>& actors() const noexcept { return actors_; }
    const dh2::data::AiTables& factions() const noexcept { return tables_; }
    const PlayableActorTraits* traits(ActorId) const noexcept;
    dh2::data::CombatRandom random_state() const noexcept;
    // Shared source random draw for gameplay consumers such as audio/loot.
    // Uses the same persisted stream as combat; zero retains source Random(0)
    // semantics (one call, unchanged seed). Invalid ranges leave all state alone.
    bool random_uniform(std::uint32_t bound, std::uint32_t& output, std::string& error);
    // Loan the SAME stream to the source loot kernels using their named state
    // type. Commit every consumed prefix, including false/throw outcomes.
    // World audio/combat draws inside the callback stay in this exact order.
    // The callback must not retain the loan or replace the actor registry.
    using LootRandomConsumer = std::function<bool(dh2::data::LootRandom8V2&,std::string&)>;
    bool with_loot_random(const LootRandomConsumer&, std::string& error);
    // Validates every actor in a temporary registry, then replaces actors/RNG.
    // Existing borrowed actor/object pointers must be reacquired after success.
    bool replace_actors(const std::vector<PersistedPlayableActor>&,
                        dh2::data::CombatRandom, std::string& error);
    // Validate both subsets and shared IDs before publishing either/RNG.
    bool replace_state(const std::vector<PersistedPlayableActor>&,
                       std::vector<WorldObject>,dh2::data::CombatRandom,std::string& error);
    bool eligible_target(const ActorState&, const ActorState&) const override;
    // This is source melee reach, NOT a substituted physical capsule radius.
    float target_radius(const ActorState&) const override;
    float melee_reach(ActorId) const noexcept;
    bool original_melee_in_range(ActorId attacker, ActorId victim) const;
    bool resolve_damage(const std::string&, const ActorState&, const ActorState&,
                        const std::string& marker, float&, std::string& error) const override;
    bool resolve_damage_with_outcomes(const std::string&, const ActorState&, const ActorState&,
                        const std::string& marker, float&,
                        std::optional<std::uint32_t>& outcomes,
                        std::optional<std::uint32_t>& source_mask,
                        std::string& error) const override;
    // Skill/spell caller supplies authored request fields after its own target
    // query. This uses the same actor sheets, RNG and result presentation stream.
    // It calculates only; the lifecycle caller applies health/status exactly once.
    // A source-produced attacker formula sheet is borrowed synchronously for a
    // skill/spell class; it never replaces the actor's permanent property sheets.
    bool resolve_source_result(const std::string& source, ActorId attacker, ActorId victim,
                        const std::string& marker, std::uint32_t mask,
                        std::int32_t category, std::int32_t element, std::int32_t direct_amount,
                        OriginalMeleeResolution&, std::string& error,
                        const dh2::data::PropertySheet* attacker_formula_sheet = nullptr);
    // Only for an already-admitted script continuation. Source calculation has
    // no IsDead gate; does not admit a command or apply health/reactions/status.
    // Both character identities/sheets must still be in this same world.
    bool calculate_source_result(const std::string& source,ActorId attacker,ActorId victim,
                        const std::string& marker,std::uint32_t mask,
                        std::int32_t category,std::int32_t element,std::int32_t direct_amount,
                        OriginalMeleeResolution&,std::string& error,
                        const dh2::data::PropertySheet* attacker_formula_sheet=nullptr);
    // Full status/DOT/leech/outcome payload for host lifecycle consumers.
    std::vector<PlayableCombatResolution> take_resolutions();
    // Read only during synchronous hit presentation. These are the same
    // resolutions produced by gameplay, before the session drains the frame.
    // Do not retain element pointers across another hit or frame.
    const std::vector<PlayableCombatResolution>& pending_resolutions()const noexcept{return resolutions_;}
    // Snapshot derived presentation at the genuine result boundary before the
    // retained marker returns and actor/root displacement advances. This hook
    // borrows the result; it must not replace combat or mutate actor/RNG owners.
    using ResolutionObserver = std::function<bool(const PlayableCombatResolution&,std::string&)>;
    void set_resolution_observer(ResolutionObserver observer){resolutionObserver_=std::move(observer);}
    // Original F_ApplyResult ignores scrolling-text failure. Presentation
    // diagnostics must never reject the genuine damage/health result.
    std::vector<std::string> take_resolution_observer_errors();
    const OriginalCombatProperties* combat_properties(ActorId) const noexcept;
private:
    struct Binding { PlayableActorTraits traits; float melee_reach = 0; };
    bool make_binding(const OriginalCombatProperties&, PlayableActorTraits,
                      Binding&, std::string& error) const;
    dh2::data::AiTables tables_;
    dh2::data::CombatRandom& random_;
    dh2::data::LootRandom8V2* lootRandomLoan_ = nullptr;
    void sync_random_from_loan() const noexcept;
    void sync_loan_from_random() const noexcept;
    std::map<ActorId, ActorState> actors_;
    std::map<ActorId, Binding> bindings_;
    OriginalMeleeDamageProvider damage_;
    std::map<ObjectId,WorldObject> objects_;
    mutable std::vector<PlayableCombatResolution> resolutions_;
    ResolutionObserver resolutionObserver_;
    mutable std::vector<std::string> resolutionObserverErrors_;
};
} // namespace dh::foundation
