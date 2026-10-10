#include "runtime_source_tutorial_v1.hpp"
#include "../../asset_catalog.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
void run(const std::filesystem::path& repo) {
    const auto root = repo / ".local-inputs/windows-source-clock-checkpoint-v17/assets";
    AssetCatalog assets(root);
    OriginalCampaignRuntime campaign;
    std::string error;
    check(campaign.load(assets, "original-campaign.xml", error), error);

    SourceTutorialTimelineV1 timeline;
    check(timeline.build_movement_tutorial(campaign, error), error);
    const auto id = campaign.script_id("Movement_Tuto2", false);
    check(timeline.campaign() == &campaign && timeline.script_id() == id,
          "Projection must remain attached to the caller's loaded campaign");
    const int caller_id = campaign.script_id("Movement_Tuto", false);
    check(caller_id >= 0, "Loaded source Movement_Tuto caller is absent");
    const auto& caller = campaign.scripts().at(static_cast<std::size_t>(caller_id));
    check(caller.commands.size() == 1 && caller.commands[0].kind == 78 &&
          caller.commands[0].strings.at(12) == "Movement_Tuto2" &&
          caller.commands[0].scalars.at(16) == 6,
          "Movement_Tuto DoTutorial caller/continuation/index changed");
    check(timeline.prompts().size() == 4,
          "Movement_Tuto2 actual campaign rows should yield four dialog/wait pairs");
    const std::int32_t expected_text[] = {2097188, 2097189, 2097190, 2097191};
    const std::int32_t expected_field12[] = {4, 14, 4, 15};
    const std::size_t expected_rows[] = {5, 7, 9, 11};
    const auto& script = campaign.scripts().at(static_cast<std::size_t>(id));
    for (std::size_t i = 0; i < timeline.prompts().size(); ++i) {
        const auto& prompt = timeline.prompts()[i];
        check(prompt.ordinal == i && prompt.dialog_command_index == expected_rows[i] &&
              prompt.wait_command_index == expected_rows[i] + 1,
              "Source row order and immediate wait pairing changed");
        check(prompt.dialog_command == &script.commands[expected_rows[i]] &&
              prompt.wait_command == &script.commands[expected_rows[i] + 1],
              "Projection must reference actual loaded command rows, not copies");
        check(prompt.dialog_command->kind == 10 && prompt.wait_command->kind == 12 &&
              prompt.source_field8 == 49 && prompt.source_field12 == expected_field12[i] &&
              prompt.source_text_id == expected_text[i],
              "Movement tutorial prompt fields differ at ordinal " + std::to_string(i) +
              ": field8=" + std::to_string(prompt.source_field8) +
              " field12=" + std::to_string(prompt.source_field12) +
              " text=" + std::to_string(prompt.source_text_id));
        std::size_t matched = 99;
        check(timeline.matches_command(*prompt.dialog_command, &matched) && matched == i,
              "Actual source StartDialog command does not resolve to its prompt");
        check(timeline.matches_command(*prompt.wait_command, &matched) && matched == i,
              "Actual source WaitDialog command does not resolve to its prompt");
    }
    std::size_t ignored = 99;
    check(!timeline.matches_command(script.commands[0], &ignored) && ignored == 99,
          "Non-prompt source command must stay on the ordinary campaign path");
    for (const auto& entry : campaign.triggers()) {
        const auto found = entry.second.attributes.find("script");
        check(found == entry.second.attributes.end() ||
              (found->second != "Movement_Tuto" && found->second != "Movement_Tuto2"),
              "Do not claim an authored TriggerZone for the Movement tutorial scripts");
    }
    std::cout << "PASS Movement_Tuto DoTutorial source row and Movement_Tuto2 actual rows: "
                 "4 exact StartDialog/WaitDialog pairs; activation trigger remains unknown\n";
}
}
int main(int argc, char** argv) {
    try {
        check(argc == 2, "Repository root required");
        run(argv[1]);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
