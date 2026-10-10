#pragma once

#include "creation_adapter.hpp"
#include "../../../original_actor_properties.hpp"
#include "../../../save_store.hpp"
#include "../../../../game-data/fresh_player_profile_v1.hpp"
#include "../../../../game-data/loot_tables_v2.hpp"
#include "../../../../game-data/skill_tables.hpp"

#include <filesystem>
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::frontend::creation {

struct RuntimeCreationSourceV1 {
    std::shared_ptr<const OriginalPropertyDatabase> properties;
    dh2::data::LootTablesV2::Borrow loot;
    dh2::data::SkillTables::Borrow skills;
    // The caller's existing original application RNG owner; never seeded by
    // this helper. The source invokes the original random routine for each
    // starter item-list choice, including a singleton list.
    dh2::data::LootRandom8V2* source_loot_random{};
    // Optional values queried from the original CharacterDesign constants.
    // Unlocked difficulty comes from this operation's exact fresh PDFL bytes.
    // Without these caps the source projection keeps initial rank zero.
    bool source_skill_grant_context_known{};
    std::array<std::uint32_t, 3> source_skill_level_caps{};
};

// Reproduces the source Character::_InitSkillsSlots rank-zero projection for
// an already-owned generic CharacterState. The list ID must come from the
// caller's loaded CharacterTable/source property owner. This initializes the
// exact source row order and both initial set/slot-zero maps; it does not grant
// the constructor's separate first-skill increment.
bool initialize_source_skill_rows_v1(dh2::data::SkillTables::Borrow skills,
                                     std::int32_t source_skill_list_id,
                                     CharacterState& state,
                                     std::string& error);

struct RuntimeCreationRequestV1 {
    // This is the authority that the caller already owns. The operation builds
    // detached source values, saves/reloads them, then updates this exact state
    // once. It never returns a replacement state as gameplay authority.
    std::shared_ptr<CharacterState> shared_state;
    std::string character_id;
    std::string player_name;
    std::string class_token;
    std::filesystem::path save_path;
    std::uint32_t source_timer{};
    std::uint32_t saved_date{};

};

using RuntimeCreationStartV1 = std::function<bool(
    const std::shared_ptr<CharacterState>&, std::string&)>;

struct RuntimeCreationServicesV1 {
    RuntimeCreationSourceV1 source;
    // Existing generic start provider. It must use the passed state in place;
    // its success is not a native start/action receipt or FrontendProfileLoan.
    RuntimeCreationStartV1 start_same_state;
};

enum class RuntimeCreationStatusV1 {
    invalid_request,
    destination_exists,
    unavailable_source,
    unsupported_requested_data,
    source_initialization_failed,
    persistence_failed,
    reload_failed,
    reload_mismatch,
    prepared_for_start,
    unavailable_start,
    start_failed,
    start_provider_returned_success
};

struct RuntimeCreationResultV1 {
    RuntimeCreationStatusV1 status{RuntimeCreationStatusV1::invalid_request};
    // Exact caller owner when provided. This result is generic persistence and
    // state-start evidence only, never a native action receipt/gameplay loan.
    std::shared_ptr<CharacterState> shared_state;
    dh2::data::FreshPlayerProfileV1 source_profile_metadata;
    std::int32_t character_row{-1};
    std::int32_t starting_loot_row{-1};
    std::int32_t skill_list_row{-1};
    bool saved{}, reloaded{}, published_to_shared_state{}, start_provider_succeeded{};
    std::vector<std::string> representation_limits;
    std::string error;

    bool same_state_owner(const std::shared_ptr<CharacterState>& expected) const noexcept;
};

// Source-backed generic starter. Uses original Character/Class properties,
// class starter loot, item tables, rank-zero SkillList rows and source maps. It
// persists and reloads the detached state through the existing save API before
// publishing once into the caller's exact shared state and calling
// start_same_state. Native V60/InitPost and FrontendProfileLoan remain separate.
class RuntimeCreationPersistenceV1 final {
public:
    // Persists/reloads and publishes into the exact caller state but does not
    // start. Frontend creation uses this at Confirm; StartGame is a later action.
    static RuntimeCreationResultV1 create_reload(
        const RuntimeCreationRequestV1&, const RuntimeCreationServicesV1&);
    static RuntimeCreationResultV1 create_reload_start(
        const RuntimeCreationRequestV1&, const RuntimeCreationServicesV1&);

    static const std::vector<std::string>& representation_limits() noexcept;
};

} // namespace dh::foundation::frontend::creation
