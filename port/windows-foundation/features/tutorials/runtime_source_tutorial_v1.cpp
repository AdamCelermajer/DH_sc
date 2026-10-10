#include "runtime_source_tutorial_v1.hpp"
#include <cstring>

namespace dh::foundation {
namespace {
bool signed_word(const OriginalCampaignCommand& command, unsigned offset,
                 std::int32_t& value) noexcept {
    const auto found = command.scalars.find(offset);
    if (found == command.scalars.end()) return false;
    std::memcpy(&value, &found->second, sizeof(value));
    return true;
}
}

bool SourceTutorialTimelineV1::build_movement_tutorial(
    const OriginalCampaignRuntime& campaign, std::string& error) {
    const int id = campaign.script_id("Movement_Tuto2", false);
    if (id < 0 || static_cast<std::size_t>(id) >= campaign.scripts().size()) {
        error = "Loaded source campaign has no level Movement_Tuto2 script";
        return false;
    }
    const auto& script = campaign.scripts()[static_cast<std::size_t>(id)];
    if (script.name != "Movement_Tuto2" || script.scope != "level") {
        error = "Movement tutorial projection requires the exact level script";
        return false;
    }

    std::vector<SourceTutorialPromptV1> prompts;
    for (std::size_t i = 0; i < script.commands.size(); ++i) {
        const auto& dialog = script.commands[i];
        if (dialog.kind != 10) continue;
        if (dialog.class_name != "Script_StartDialog" || i + 1 >= script.commands.size()) {
            error = "Movement tutorial contains an unexpected StartDialog record";
            return false;
        }
        const auto& wait = script.commands[i + 1];
        if (wait.kind != 12 || wait.class_name != "Script_WaitDialog") {
            error = "Movement tutorial dialog is not followed by its authored WaitDialog";
            return false;
        }
        SourceTutorialPromptV1 prompt;
        prompt.ordinal = prompts.size();
        prompt.dialog_command_index = i;
        prompt.wait_command_index = i + 1;
        prompt.dialog_command = &dialog;
        prompt.wait_command = &wait;
        if (!signed_word(dialog, 8, prompt.source_field8) ||
            !signed_word(dialog, 12, prompt.source_field12) ||
            !signed_word(dialog, 16, prompt.source_text_id)) {
            error = "Movement tutorial StartDialog is missing an authored scalar";
            return false;
        }
        prompts.push_back(prompt);
        ++i;
    }
    if (prompts.empty()) {
        error = "Movement tutorial has no authored dialog prompts";
        return false;
    }
    campaign_ = &campaign;
    script_id_ = id;
    prompts_ = std::move(prompts);
    error.clear();
    return true;
}

bool SourceTutorialTimelineV1::matches_command(
    const OriginalCampaignCommand& command,
    std::size_t* prompt_ordinal) const noexcept {
    for (const auto& prompt : prompts_) {
        if (prompt.dialog_command == &command || prompt.wait_command == &command) {
            if (prompt_ordinal) *prompt_ordinal = prompt.ordinal;
            return true;
        }
    }
    return false;
}

} // namespace dh::foundation
