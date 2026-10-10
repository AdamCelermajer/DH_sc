#pragma once

#include "runtime_player_locomotion_program_v1.hpp"

#include <memory>

namespace dh::foundation::equipment_menu {

struct RuntimePlayerLocomotionLibraryRequestV1 {
    std::string profile_id;
    // Caller-selected source profile table (for example main's options.profiles).
    std::string profile_library_uri{"actor-profiles-v2.xml"};
    std::string role;
    std::int32_t main_hand_item_id{-1};
    std::int32_t off_hand_item_id{-1};
    std::int32_t character_flag_1324{};
    // Caller budget for the current pair and its null-slot combinations.
    // The implementation hard ceiling is eight distinct pairs.
    std::size_t max_programs{4};
};

struct RuntimePlayerLocomotionSelectionV1 {
    std::int32_t main_hand_item_id{-1};
    std::int32_t off_hand_item_id{-1};
    std::int32_t stance{-1};
    std::shared_ptr<const RuntimePlayerLocomotionProgramV1> program;
    explicit operator bool() const noexcept { return bool(program); }
};

// Owns a bounded set of source-derived programs for the current equipment
// pair, each single-slot/null combination, and empty hands. No actor, pose,
// clock, or playback owner is created. Selection leases keep program values
// alive independently of the library object's lifetime.
class RuntimePlayerLocomotionLibraryV1 {
    struct Entry {
        std::int32_t main_hand_item_id{-1}, off_hand_item_id{-1}, stance{-1};
        std::shared_ptr<const RuntimePlayerLocomotionProgramV1> program;
    };
    std::string profile_id_;
    std::string role_;
    CharacterVisualConfig base_visual_;
    std::vector<Entry> entries_;

public:
    static constexpr std::size_t hard_program_limit = 8;

    static bool build(const AssetCatalog& actual_assets,
                      const CharacterVisualConfig& same_character_visual,
                      const RuntimePlayerLocomotionLibraryRequestV1&,
                      RuntimePlayerLocomotionLibraryV1& output,
                      std::string& error);

    bool select_exact(std::int32_t main_hand_item_id,
                      std::int32_t off_hand_item_id,
                      RuntimePlayerLocomotionSelectionV1& output,
                      std::string& error) const;

    // Selects the ordinary one-hand case by running the same source resolver
    // result prebuilt for (main hand, empty offhand). It does not guess a
    // category, replace an ID, or impose a presentation override.
    bool select_ordinary_one_hand(std::int32_t main_hand_item_id,
                                 RuntimePlayerLocomotionSelectionV1& output,
                                 std::string& error) const;

    // Atomically merges every exact named clip receipt into the caller's same
    // CharacterVisualConfig before CharacterVisual::load/initialize.
    bool merge_named_clips(CharacterVisualConfig& same_character_visual,
                           std::string& error) const;

    const std::string& profile_id() const noexcept { return profile_id_; }
    const std::string& role() const noexcept { return role_; }
    std::size_t program_count() const noexcept { return entries_.size(); }
};

} // namespace dh::foundation::equipment_menu
