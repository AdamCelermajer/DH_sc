#pragma once

#include "../../original_campaign_runtime.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace dh::foundation {

// Read-only projection of the authored Movement_Tuto2 dialog beats. The
// campaign runtime still schedules these source commands, and the existing
// MenuDialogMessagesV97 owner remains responsible for showing/queueing them.
struct SourceTutorialPromptV1 {
    std::size_t ordinal = 0;
    std::size_t dialog_command_index = 0;
    std::size_t wait_command_index = 0;
    const OriginalCampaignCommand* dialog_command = nullptr;
    const OriginalCampaignCommand* wait_command = nullptr;
    // Preserve the three authored words verbatim. Their runtime interpretation
    // belongs to the original DialogMsg/source UI owner.
    std::int32_t source_field8 = -1;
    std::int32_t source_field12 = -1;
    std::int32_t source_text_id = -1;
};

class SourceTutorialTimelineV1 {
public:
    // The name is explicit because this projection is evidence-scoped to the
    // visible Movement Tutorial prompt, not a generic tutorial scheduler.
    bool build_movement_tutorial(const OriginalCampaignRuntime&, std::string&);
    bool matches_command(const OriginalCampaignCommand&,
                         std::size_t* prompt_ordinal = nullptr) const noexcept;

    const OriginalCampaignRuntime* campaign() const noexcept { return campaign_; }
    int script_id() const noexcept { return script_id_; }
    const std::vector<SourceTutorialPromptV1>& prompts() const noexcept { return prompts_; }

private:
    const OriginalCampaignRuntime* campaign_ = nullptr;
    int script_id_ = -1;
    std::vector<SourceTutorialPromptV1> prompts_;
};

} // namespace dh::foundation
