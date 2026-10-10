#include "source_companion_animation_bank_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../../game-data/data.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::companions;
namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
dh2::data::Bytes view(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
void run(const std::filesystem::path& repo) {
    const auto source_root = repo / ".local-inputs/windows-source-clock-v19-preview-3/assets";
    AssetCatalog source_assets(source_root);
    AssetCatalog table_assets(source_root / "original-cache");
    AssetCatalog dictionary_assets(repo);
    AssetCatalog cinematic_assets(repo / ".local-inputs/publication/checkpoint/port/level-world/reference/character-visual-v6/cache");

    OriginalCampaignRuntime campaign;
    std::string error;
    check(campaign.load(source_assets, "original-campaign.xml", error), error);

    dh2::data::Dictionary dictionary;
    const auto names = dictionary_assets.read(".local-inputs/actors/animations_dictionary_pyarraynames.bin");
    const auto values = table_assets.read("data/pydata/animations_dictionary_pyarray.bin");
    check(dh2::data::load_dictionary(view(names), view(values), dictionary, error), error);

    dh2::data::AnimationTables tables;
    const auto records = table_assets.read("data/pydata/animations_pyarray.bin");
    const auto sequence_names = table_assets.read("data/pydata/animations_pyarraynames.bin");
    const auto schema = table_assets.read("data/pydata/animations_pystructnames.bin");
    check(dh2::data::load_animation_tables(view(records), view(sequence_names), view(schema),
          dictionary, tables, error), error);

    OriginalCombatVisualPlan priest_visual, faery_visual;
    priest_visual.profileId = "WanderingPriest";
    faery_visual.profileId = "DefaultFairy";
    CombatSessionProfile priest_profile, faery_profile;
    priest_profile.animationOnly = true;
    faery_profile.animationOnly = true;
    SourceCompanionAnimationBankV1 bank;
    check(build_source_companion_animation_bank_v1(campaign, "Swamp_Intro", dictionary,
          tables, cinematic_assets,
          {{"_prim_NPC_PriestGood", "WanderingPriest", &priest_visual, &priest_profile},
           {"_prim_Faery", "DefaultFairy", &faery_visual, &faery_profile}}, bank, error), error);
    check(bank.campaign == &campaign && bank.script_name == "Swamp_Intro" &&
          bank.commands.size() >= 2, "Bank must retain the exact loaded script and supplied actor commands");

    const auto priest = std::find_if(bank.commands.begin(), bank.commands.end(),
        [](const auto& item) { return item.command_index == 45; });
    const auto faery = std::find_if(bank.commands.begin(), bank.commands.end(),
        [](const auto& item) { return item.command_index == 46; });
    check(priest != bank.commands.end() && priest->command && priest->source_object_name == "_prim_NPC_PriestGood" &&
          priest->profile_id == "WanderingPriest" && priest->animation_id == 396 &&
          priest->next_id == UINT32_MAX && priest->slot == 2 && priest->offset20 == 20 && !priest->wait,
          "Rene/Priest binding must retain authored command fields");
    check(priest->dictionary_name == "cs_swamp_intro_priest_scene05" &&
          priest->dictionary_path == "data/3D/characters/npcs/animations/cs_swamp_intro_priest_scene05.bdae" &&
          priest->clip_alias == "source-animdict-396-cs_swamp_intro_priest_scene05" &&
          priest->clip_available && priest->preloaded && priest->animation_table_references.empty(),
          "Priest 396 must resolve to the exact local cinematic BDAE without invented table timing");
    check(std::find(priest_profile.sourceAnimationClips.begin(), priest_profile.sourceAnimationClips.end(),
          std::make_pair(priest->clip_alias, priest->dictionary_path)) != priest_profile.sourceAnimationClips.end(),
          "Available source BDAE must be prepared before animationOnly Session initialization");

    check(faery != bank.commands.end() && faery->command && faery->source_object_name == "_prim_Faery" &&
          faery->profile_id == "DefaultFairy" && faery->animation_id == 377 &&
          faery->next_id == UINT32_MAX && faery->slot == 3 && faery->offset20 == 11 && !faery->wait,
          "Faery 377 binding must retain authored command fields");
    check(faery->dictionary_name == "cs_swamp_intro_faery_scene05" &&
          faery->dictionary_path == "data/3D/characters/faeries/animations/cs_swamp_intro_faery_scene05.bdae" &&
          faery->clip_alias == "source-animdict-377-cs_swamp_intro_faery_scene05" &&
          !faery->clip_available && !faery->preloaded && faery->animation_table_references.empty() &&
          faery->unsupported_reason.find("absent") != std::string::npos,
          "Faery 377 must preserve dictionary identity while failing closed on its absent exact BDAE");
    check(faery_profile.sourceAnimationClips.empty(), "Missing Faery BDAE must not gain a substitute clip");
    check(bank.find(*priest->command) == &*priest && bank.find(*faery->command) == &*faery,
          "Command lookup must preserve original runtime record identity");

    // The selected publication fixture does not contain this Faery BDAE. A
    // test-only extraction of the exact original ZIP member must make the same
    // production bank API prepare it, without changing command timing policy.
    AssetCatalog extracted_assets(repo / ".local-inputs/companions-faery-377-source-test-cache");
    CombatSessionProfile extracted_priest_profile, extracted_faery_profile;
    extracted_priest_profile.animationOnly = true;
    extracted_faery_profile.animationOnly = true;
    SourceCompanionAnimationBankV1 extracted_bank;
    check(build_source_companion_animation_bank_v1(campaign, "Swamp_Intro", dictionary,
          tables, extracted_assets,
          {{"_prim_NPC_PriestGood", "WanderingPriest", &priest_visual, &extracted_priest_profile},
           {"_prim_Faery", "DefaultFairy", &faery_visual, &extracted_faery_profile}},
          extracted_bank, error), error);
    const auto extracted_faery = std::find_if(extracted_bank.commands.begin(), extracted_bank.commands.end(),
        [](const auto& item) { return item.command_index == 46; });
    check(extracted_faery != extracted_bank.commands.end() &&
          extracted_faery->dictionary_path == "data/3D/characters/faeries/animations/cs_swamp_intro_faery_scene05.bdae" &&
          extracted_faery->clip_available && extracted_faery->preloaded &&
          extracted_faery->animation_table_references.empty() &&
          extracted_faery_profile.sourceAnimationClips == std::vector<std::pair<std::string, std::string>>{
              {"source-animdict-377-cs_swamp_intro_faery_scene05",
               "data/3D/characters/faeries/animations/cs_swamp_intro_faery_scene05.bdae"}},
          "Existing bank must preload exact extracted Faery 377 BDAE into its animationOnly profile");
}
}
int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        run(argv[1]);
        std::cout << "source_companion_animation_bank_v1 PASS: exact AnimDict paths; missing Faery fixture stays absent; canonical 377 extraction is preloaded\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
