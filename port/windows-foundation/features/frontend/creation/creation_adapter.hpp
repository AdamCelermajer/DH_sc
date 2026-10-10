#pragma once
#include "../../../character_state.hpp"
#include <array>
#include <functional>
#include <optional>
#include <string_view>

namespace dh::foundation::frontend::creation {

struct ClassChoice {
    unsigned menu_index;
    std::string_view profile_token;
    std::uint32_t character_row;
    std::string_view title_token;
    std::string_view description_token;
    std::uint32_t starting_loot_row;
    std::uint32_t skill_list_row;
    std::uint32_t first_skill_dictionary_row;
    std::string_view first_skill_token;
    std::uint32_t faery_list_row;
};

const std::array<ClassChoice, 3>& class_choices() noexcept;
const ClassChoice* find_class(std::string_view profile_token) noexcept;
inline constexpr std::string_view initial_class_token = "KnightPlayerBase";
inline constexpr std::uint32_t initial_player_level = 1;
inline constexpr std::uint32_t initial_campaign_level_row = 41;

struct CreationRequest {
    std::string character_id;
    std::string player_name;
    std::string class_token;
};

// The provider must project the complete source initialization into the shared
// model, including resolved properties, delivered grants and campaign unlocks.
// This interface is an in-memory service; it must never persist a user profile.
// A complete projection is required: authored raw properties and foundation
// make_default_character fixtures are not resolved campaign character defaults.
using CompleteCreationService = std::function<std::optional<CharacterState>(
    const CreationRequest&, const ClassChoice&, std::string& error)>;

enum class CreationStatus {
    staged, invalid_request, unavailable_service, service_failure, invalid_projection
};
struct StagedCreation {
    CreationStatus status = CreationStatus::invalid_request;
    std::optional<CharacterState> character;
    std::vector<std::string> errors;
    bool ok() const noexcept { return status == CreationStatus::staged && character.has_value(); }
};

// Returns an owned staged value only; no save-store or filesystem dependency.
StagedCreation stage_creation(const CreationRequest&, const CompleteCreationService& = {});
}
