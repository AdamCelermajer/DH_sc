#pragma once

#include "../../combat_session.hpp"
#include "../../original_actor_target_position.hpp"
#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/effects_tables.hpp"
#include "../../../game-data/items.hpp"
#include "../../../level-world/character_mesh_fx_owner_v4.hpp"

#include <functional>
#include <set>
#include <string>
#include <tuple>

namespace dh::foundation::effects {

using RuntimeSwingFxEnabledResolverV1 =
    std::function<bool(ActorId, std::uint8_t&, std::string&)>;

// Uses the same GetTargetPosition owner fields as the source step FX kernel:
// known-null node selects ActorState position even if the cache is stale;
// non-null node selects cache only when actual enabled80 is true.
inline bool runtime_swing_fx_target_position_v1(
    const ActorState& actor, ActorId owner,
    const RuntimeSwingFxEnabledResolverV1& enabled,
    std::array<float, 3>& output, std::string& error) {
    if (!owner || actor.id != owner) {
        error = "AnimTable FX target-position owner differs from same-session ActorId";
        return false;
    }
    const OriginalTargetPosition* selected = nullptr;
    if (!original_get_target_position(
            actor.source_target_node180, actor.source_target_position184,
            actor.transform.position,
            [&enabled, owner](std::uint8_t& value, std::string& problem) {
                if (!enabled) {
                    problem = "Required actual source enabled80 for non-null target_node";
                    return false;
                }
                return enabled(owner, value, problem);
            }, selected, error))
        return false;
    output = *selected;
    error.clear();
    return true;
}

struct RuntimeSwingFxDiagnosticV1 {
    ActorId actor = invalid_actor_id;
    std::uint64_t occurrence = 0;
    std::uint64_t update_serial = 0;
    std::int64_t sequence_id = -1;
    std::uint32_t step = 0;
    bool dispatched = false;
    std::vector<std::int32_t> source_sets;
    std::string detail;
};

// Presentation sidecar for original AnimTable step FX. Register
// step_entry_observer() with RuntimeSessionAudioV1's presentation-observer API;
// that composition invokes audio first, then this FX observer. This class never
// changes CombatSession's observer slot or advances gameplay/FX clocks.
class RuntimeSwingFxObserverV1 final {
public:
    RuntimeSwingFxObserverV1(CombatSession&, const dh2::data::AnimationTables&,
        const dh2::data::ItemTable&, dh2::data::EffectsTables::Borrow,
        dh2::fx::CharacterMeshFxOwnerV4&,
        std::function<void(const RuntimeSwingFxDiagnosticV1&)> diagnostic = {},
        RuntimeSwingFxEnabledResolverV1 enabled = {});
    RuntimeSwingFxObserverV1(const RuntimeSwingFxObserverV1&) = delete;
    RuntimeSwingFxObserverV1& operator=(const RuntimeSwingFxObserverV1&) = delete;

    CombatSessionStepObserver step_entry_observer();

private:
    using OccurrenceKey = std::tuple<std::uintptr_t, ActorId, std::uint64_t>;
    struct StepContext;
    void dispatch(const CombatSessionStepEntry&);
    void report(const RuntimeSwingFxDiagnosticV1&) const noexcept;

    CombatSession& session_;
    const dh2::data::AnimationTables& animations_;
    const dh2::data::ItemTable& items_;
    dh2::data::EffectsTables::Borrow effects_;
    dh2::fx::CharacterMeshFxOwnerV4& manager_;
    std::function<void(const RuntimeSwingFxDiagnosticV1&)> diagnostic_;
    RuntimeSwingFxEnabledResolverV1 enabled_;
    std::set<OccurrenceKey> delivered_;
    std::weak_ptr<const void> active_lease_;
};

} // namespace dh::foundation::effects
