#include "creation_adapter.hpp"
#include <exception>
#include <utility>

namespace dh::foundation::frontend::creation {
const std::array<ClassChoice, 3>& class_choices() noexcept {
    // MenuCharacterSelect::Update 0x428498: case 1 is Rogue, case 2 is Mage.
    // Cache producers independently bind names to numeric rows/grant tables.
    static constexpr std::array<ClassChoice, 3> choices{{
        {0, "KnightPlayerBase", 263, "MENU_CLASS_00", "MENU_KNIGHT_DESC", 165, 14, 7, "BashDown", 1},
        {1, "RoguePlayerBase", 325, "MENU_CLASS_01", "MENU_ROGUE_DESC", 213, 27, 56, "JumpKick", 3},
        {2, "MagePlayerBase", 290, "MENU_CLASS_02", "MENU_MAGE_DESC", 174, 21, 14, "ColdRay", 2},
    }};
    return choices;
}
const ClassChoice* find_class(std::string_view token) noexcept {
    for (const auto& choice : class_choices()) if (choice.profile_token == token) return &choice;
    return nullptr;
}
namespace {
bool valid_text(const std::string& text) {
    if (text.empty() || text.size() > character_text_limit) return false;
    for (unsigned char ch : text) if (ch < 0x20 || ch == 0x7f) return false;
    return true;
}
}
StagedCreation stage_creation(const CreationRequest& request, const CompleteCreationService& service) {
    StagedCreation result;
    const auto* choice = find_class(request.class_token);
    if (!choice) result.errors.emplace_back("class must be an original base-class profile token");
    if (!valid_text(request.character_id)) result.errors.emplace_back("character ID must contain 1..1024 bytes without controls");
    if (!valid_text(request.player_name)) result.errors.emplace_back("player name must contain 1..1024 bytes without controls");
    if (!result.errors.empty()) return result;
    if (!service) {
        result.status = CreationStatus::unavailable_service;
        result.errors.emplace_back("complete source character initialization service is unavailable");
        return result;
    }
    std::string error;
    std::optional<CharacterState> candidate;
    try { candidate = service(request, *choice, error); }
    catch (const std::exception& exception) { error = exception.what(); }
    catch (...) { error = "character initialization service threw an unknown exception"; }
    if (!candidate || !error.empty()) {
        result.status = CreationStatus::service_failure;
        result.errors.push_back(error.empty() ? "character initialization service produced no complete projection" : error);
        return result;
    }
    result.errors = validate_character_state(*candidate).errors;
    if (candidate->id != request.character_id || candidate->name != request.player_name || candidate->class_id != request.class_token)
        result.errors.emplace_back("initialization projection does not match the requested identity/name/class");
    if (candidate->stats.level != initial_player_level)
        result.errors.emplace_back("fresh profile projection must have source player level 1");
    if (!result.errors.empty()) {
        result.status = CreationStatus::invalid_projection;
        return result;
    }
    result.character = std::move(candidate);
    result.status = CreationStatus::staged;
    return result;
}
}
