#include "source_companion_campaign_animation_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_melee_bindings.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::companions;
namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
void run(const std::filesystem::path& repo) {
    const auto root = repo / ".local-inputs/windows-source-clock-v19-preview-2/assets";
    AssetCatalog campaignAssets(root), assets(root / "original-cache");
    std::string error;
    OriginalCampaignRuntime campaign;
    check(campaign.load(campaignAssets, "original-campaign.xml", error), error);
    OriginalMeleeBindings bindings;
    check(bindings.load(campaignAssets, "original-melee-bindings.xml", error), error);
    ActorCustomization customization;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan priest, faery;
    check(build_original_combat_visual_plan(assets, bindings, "WanderingPriest",
          customization, "companion-priest-session", priest, error), error);
    check(build_original_combat_visual_plan(assets, bindings, "DefaultFairy",
          customization, "companion-faery-session", faery, error), error);

    CompanionCampaignAnimationBankV1 faeryIdle;
    check(build_companion_campaign_animation_bank_v1(bindings, campaign, "FaeryIdle",
          {{"DefaultFairy", "DefaultFairy", &faery}}, faeryIdle, error), error);
    check(faeryIdle.campaign == &campaign && faeryIdle.script_id == campaign.script_id("FaeryIdle", true),
          "Bank retains exact loaded common script owner");
    check(faeryIdle.commands.size() == 1 && faeryIdle.commands[0].resolved,
          "Only the exact one-leaf FaeryIdle command resolves");
    const auto& idle = faeryIdle.commands[0];
    check(idle.command_index == 0 && idle.source_object_name == "DefaultFairy" &&
          idle.profile_id == "DefaultFairy" && idle.animation_id == 686 && idle.next_id == 686 &&
          idle.slot == 1 && !idle.wait && idle.looping && idle.state == "Idle" &&
          idle.selection.state == "Idle" && idle.selection.variant == 0 &&
          idle.selection.leafPath == std::vector<std::size_t>{0} &&
          idle.clip_name == faery.sequence("Idle", 0)->phases[0].clipName,
          "Source command maps 1:1 to DefaultFairy Idle leaf animationId 686");
    check(std::none_of(priest.sequences.begin(), priest.sequences.end(), [](const auto& sequence) {
              return std::any_of(sequence.phases.begin(), sequence.phases.end(),
                  [](const auto& phase) { return phase.animationId == 396; });
          }) &&
          std::none_of(faery.sequences.begin(), faery.sequences.end(), [](const auto& sequence) {
              return std::any_of(sequence.phases.begin(), sequence.phases.end(),
                  [](const auto& phase) { return phase.animationId == 377; });
          }), "Swamp_Intro companion IDs are absent from their same-profile Session plans");
    check(faeryIdle.find(*idle.command) == &idle, "Command identity is the loaded runtime record, not a copied command");

    CompanionCampaignAnimationBankV1 swamp;
    check(build_companion_campaign_animation_bank_v1(bindings, campaign, "Swamp_Intro", {
          {"_prim_NPC_PriestGood", "WanderingPriest", &priest},
          {"_prim_Faery", "DefaultFairy", &faery}}, swamp, error), error);
    const auto resolved = std::count_if(swamp.commands.begin(), swamp.commands.end(),
        [](const auto& item) { return item.resolved; });
    check(resolved == 0 && swamp.commands.size() >= 10,
          "Swamp_Intro companion dictionary IDs remain on the original provider when absent from same-profile Session plans");
    const auto priestCommand = std::find_if(swamp.commands.begin(), swamp.commands.end(),
        [](const auto& item) { return item.command_index == 45; });
    const auto faeryCommand = std::find_if(swamp.commands.begin(), swamp.commands.end(),
        [](const auto& item) { return item.command_index == 46; });
    check(priestCommand != swamp.commands.end() && priestCommand->source_object_name == "_prim_NPC_PriestGood" &&
          priestCommand->animation_id == 396 && !priestCommand->resolved &&
          priestCommand->unsupported_reason.find("no visual leaf") != std::string::npos,
          "Rene/Priest 396 stays unresolved, with no invented clip alias");
    check(faeryCommand != swamp.commands.end() && faeryCommand->source_object_name == "_prim_Faery" &&
          faeryCommand->animation_id == 377 && !faeryCommand->resolved,
          "Swamp Faery 377 stays on the existing source command provider");
    check(swamp.find(*priestCommand->command) == nullptr && swamp.find(*faeryCommand->command) == nullptr,
          "Unresolved source records are not intercepted");

    CombatSession uninitialized;
    SourceCompanionCampaignAnimationV1 adapter(campaign, uninitialized, swamp, {});
    bool handled = true, blocking = true;
    const auto& authored = campaign.scripts().at(static_cast<std::size_t>(swamp.script_id)).commands.at(45);
    check(adapter.command(CampaignCommandPhase::execute, authored, 9, false,
          handled, blocking, error) && !handled && !blocking,
          "Unresolved Rene command passes untouched to the existing source command owner");

    SourceCompanionCampaignAnimationV1 idleAdapter(campaign, uninitialized, faeryIdle, {});
    const auto& idleCommand = campaign.scripts().at(static_cast<std::size_t>(faeryIdle.script_id)).commands.at(0);
    check(!idleAdapter.command(CampaignCommandPhase::execute, idleCommand, 9, false,
          handled, blocking, error) && handled &&
          error.find("initialized CombatSession") != std::string::npos,
          "Resolved Faery clip requires a live same-session owner and does not create one");
}
}
int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        run(argv[1]);
        std::cout << "source_companion_campaign_animation_v1 PASS: exact FaeryIdle generic clip; Swamp_Intro unresolved IDs safely delegate\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
