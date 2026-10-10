#pragma once

#include "character_faery_visual_facade_v1.hpp"
#include "character_runtime_factory_v1.hpp"
#include "../game-data/player_faery_change_v1.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace dh2::character_faery_selection_owner_v1 {

namespace save = ::dh2::data::player_faery_change_v1;
using PlayerSave = ::dh2::data::PlayerSavegameV1;
namespace visual = character_faery_visual_facade_v1;
namespace factory = character_runtime_factory_v1;

struct Services {
    void* context{};
    int (*resolve_asset)(void*, std::uint32_t,
                         std::shared_ptr<const visual::Asset>*,
                         std::string&){};
    // Prepare validates the canonical Character/Level graph and captures a
    // rollback plan without mutating Save, visual state or placement.
    int (*prepare_placement)(void*, visual::Identity player_character,
                             visual::Identity faery_character,
                             std::uint32_t faery_id, std::uintptr_t* plan,
                             std::string&){};
    int (*commit_placement)(void*, std::uintptr_t plan, std::string&){};
    // Must restore any placement prefix written by a failed commit.
    int (*rollback_placement)(void*, std::uintptr_t plan, std::string&){};
    void (*discard_placement)(void*, std::uintptr_t plan) noexcept{};
};

struct Result {
    std::int32_t previous_faery{-1};
    std::uint32_t selected_faery{};
    std::uintptr_t placement_plan{};
    bool asset_prepared{};
    bool placement_prepared{};
    bool save_changed{};
    bool skills_updated{};
    bool visual_swapped{};
    bool placement_committed{};
    bool rollback_attempted{};
    bool rollback_complete{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    asset_unavailable,
    placement_unavailable,
    save_or_skill_failed,
    visual_rejected,
    placement_failed,
    rollback_failed,
};

// ChangeFaery-shaped transaction for the native BDAE renderer. It requires the
// canonical Player Record's Character+0x420 link to name the exact Faery Record
// whose visual is changed. Candidate asset/pose and Level plan are prepared
// first; then canonical Save/skills, that Faery visual, and placement run.
Status change(factory::Record& player_record, factory::Record& faery_record,
              PlayerSave&, std::uint32_t difficulty,
              std::uint32_t faery_id, const save::Services&,
              const Services&, Result*, std::string& error);

} // namespace dh2::character_faery_selection_owner_v1
