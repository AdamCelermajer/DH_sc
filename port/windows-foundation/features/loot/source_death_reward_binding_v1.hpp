#pragma once

#include "../../../level-world/character_kill_rewards_live_v31.hpp"
#include <functional>
#include <memory>
#include <string>

namespace dh2::character {
class WorldLootGameplayV23;
class CharacterProgressionWorldV23;
}

namespace dh::foundation::loot {

// Retains only the ordered Kill reward router. All gameplay authorities remain
// in the existing WorldLootGameplayV23 / CharacterProgressionWorldV23 owners.
// The caller's provider lease must pin their SAME Application, PlayerManager,
// World, Level/EventManager, Gear/Save, Item145 and source sound services.
class SourceDeathRewardBindingV1 {
public:
    using RemainingKillService = std::function<bool(
        dh2::character::KillActor56&,
        const dh2::character::KillRequest56&,
        dh2::character::KillResponse16&,
        std::string&)>;

    bool bind(
        dh2::character::WorldLootGameplayV23&,
        dh2::character::CharacterProgressionWorldV23&,
        std::shared_ptr<void> same_source_graph,
        RemainingKillService,
        dh2::character::CharacterKillProductionServicesV23&,
        std::string& error);

    bool bound() const noexcept { return bool(rewards_); }

private:
    std::shared_ptr<void> same_source_graph_;
    std::unique_ptr<dh2::character::CharacterKillRewardsLiveV31> rewards_;
};

} // namespace dh::foundation::loot
