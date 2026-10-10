#pragma once

#include "../../asset_catalog.hpp"
#include "../../combat_session.hpp"
#include "../../original_campaign_runtime.hpp"
#include "../../../game-data/animation_tables.hpp"
#include <map>

namespace dh::foundation::companions {

struct SourceCompanionAnimationActorV1 {
    std::string source_object_name;
    std::string profile_id;
    const OriginalCombatVisualPlan* visual_plan = nullptr;
    CombatSessionProfile* session_profile = nullptr;
};

// One exact Script_PlayActorAnim record resolved through animations_dictionary.
// AnimDict owns only the clip name/path; this record deliberately does not
// invent an AnimationTables loop, rate, or a generic Session sequence.
struct SourceCompanionAnimationCommandV1 {
    std::size_t command_index = 0;
    const OriginalCampaignCommand* command = nullptr;
    std::string source_object_name, profile_id;
    std::uint32_t animation_id = 0, next_id = 0, slot = 0, offset20 = 0;
    bool wait = false;
    std::string dictionary_name, dictionary_path, clip_alias;
    bool clip_available = false, preloaded = false;
    std::string unsupported_reason;
    struct AnimationTableReference {
        std::size_t sequence_index = 0, step_index = 0;
        std::int32_t loop = 0, type = 0;
        float speed = 1.0f;
        bool redirected = false;
    };
    std::vector<AnimationTableReference> animation_table_references;
};

struct SourceCompanionAnimationBankV1 {
    const OriginalCampaignRuntime* campaign = nullptr;
    int script_id = -1;
    std::string script_name;
    std::vector<SourceCompanionAnimationCommandV1> commands;

    const SourceCompanionAnimationCommandV1* find(
        const OriginalCampaignCommand&) const noexcept;
};

// Resolves authored PlayActorAnim IDs through the exact loaded AnimDict and
// appends available BDAEs to animationOnly profiles before CombatSession init.
// `assets` must be rooted at the selected extracted cache. Unavailable source
// resources stay recorded but are never added as guessed aliases.
bool build_source_companion_animation_bank_v1(
    const OriginalCampaignRuntime&, const std::string& script_name,
    const dh2::data::Dictionary&, const dh2::data::AnimationTables&,
    const AssetCatalog&, const std::vector<SourceCompanionAnimationActorV1>&,
    SourceCompanionAnimationBankV1&, std::string& error);

} // namespace dh::foundation::companions
