#pragma once

#include "source_quest_service_binding.hpp"
#include "../../../level-world/character_design_services.hpp"
#include "../../../engine-ui/hud_text_v1.hpp"

namespace dh::foundation {

// Produces non-owning callbacks over the already-live DebugSwitches, source
// PyData constants, and StringManager cache. All owners and their service
// contexts must outlive the callbacks and each call must occur on the source
// owner thread.
bool bind_source_quest_log_services_v1(
    const dh2::character::DebugLevelBinding16&,
    dh2::ui::HudTextV1&,
    const dh2::ui::HudTextEnvironmentV1&,
    QuestLogFunctorV108&,
    QuestLogTextV108&,
    std::function<bool(const char*,const char*,std::int32_t&,std::string&)>&,
    std::string& error);

// Uses the same HudText/Localization owner for v2QuestPriority constants used
// by Quest::IsDebug and the Quest Log Primary detail presentation.
bool source_quest_log_constant_v1(
    const dh2::ui::HudTextEnvironmentV1&,
    const char* group,
    const char* key,
    std::int32_t& value,
    std::string& error);

} // namespace dh::foundation
