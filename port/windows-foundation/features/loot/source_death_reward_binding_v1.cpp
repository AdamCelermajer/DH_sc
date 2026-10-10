#include "source_death_reward_binding_v1.hpp"
#include "../../../level-world/character_progression_world_v23.hpp"
#include "../../../level-world/world_loot_gameplay_v23.hpp"

namespace dh::foundation::loot {

bool SourceDeathRewardBindingV1::bind(
    dh2::character::WorldLootGameplayV23& gameplay,
    dh2::character::CharacterProgressionWorldV23& progression,
    std::shared_ptr<void> same_source_graph,
    RemainingKillService remaining,
    dh2::character::CharacterKillProductionServicesV23& kill_services,
    std::string& error) {
    error.clear();
    if (rewards_) {
        error = "Source death reward binding is already installed";
        return false;
    }
    if (!same_source_graph || !remaining) {
        error = "Required retained source Kill services and owner graph";
        return false;
    }
    if (!gameplay.ready() || !gameplay.loot() || !gameplay.items()) {
        error = "Required initialized SAME canonical Item145 reward graph";
        return false;
    }

    auto candidate = std::make_unique<dh2::character::CharacterKillRewardsLiveV31>(
        gameplay, progression, same_source_graph, std::move(remaining));
    if (!candidate->attach(kill_services, error)) return false;

    same_source_graph_ = std::move(same_source_graph);
    rewards_ = std::move(candidate);
    error.clear();
    return true;
}

} // namespace dh::foundation::loot
