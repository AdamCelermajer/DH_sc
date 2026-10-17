#pragma once

#include "player_info_record_v1.hpp"
#include "player_manager_update_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::player_manager_remove_character_v1 {

enum class Stage : std::uint8_t {
    none,
    set_position,
    disable_and_hide,
    init_camera,
    clear_ai_ooi_aggro,
    logical_delete_and_rename,
    current_level_effects,
    player_info_tail,
    hud_refresh,
    complete,
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    invalid_manager,
    unsupported_count,
    wrong_character,
    service_unavailable,
    service_failed,
    reentrant,
};

struct Services {
    void* context{};
    int (*set_position)(void*, std::uintptr_t, std::string&){};
    int (*disable_and_hide)(void*, std::uintptr_t, std::string&){};
    int (*init_camera)(void*, std::uintptr_t, std::string&){};
    int (*clear_ai_ooi_aggro)(void*, std::uintptr_t, std::string&){};
    int (*logical_delete_and_rename)(void*, std::uintptr_t,
                                     const std::string& original_name,
                                     const std::string& deleted_name,
                                     std::string&){};
    // Runs the reached CurrentLevel branch after the PlayerManager count has
    // been decremented. The supported one-player host reaches remaining_count
    // zero, so replacement-camera selection is not entered. The callback must
    // still execute the source UpdateLightSet(1,4) no-op when local_controlled.
    int (*current_level_effects)(void*, std::int32_t remaining_count,
                                 bool local_controlled,
                                 std::int32_t player_class,
                                 std::string&){};
    int (*player_info_tail)(void*, player_info_record_v1::Record&,
                            std::uintptr_t, std::uint8_t,
                            std::string&){};
    int (*hud_refresh)(void*, std::string&){};
};

struct Result {
    Status status{Status::complete};
    Stage reached{Stage::none};
    std::uint32_t callbacks{};
    bool has_current_level{};
    bool local_controlled{};
    std::int32_t player_class{-1};
};

bool services_ready(const Services&) noexcept;

// Exact source order for the supported one-player offline PlayerManager path.
// The PlayerInfo link and manager count receipt are published only after every
// Character, level, and HUD stage succeeds. A failed provider leaves the
// reached prefix visible and cannot claim RemoveCharacter completion.
Status remove_character(player_manager_update_v1::State&,
                        player_info_record_v1::Record&,
                        const Services&, std::uintptr_t expected_manager,
                        std::uintptr_t character_identity,
                        const std::string& original_object_name,
                        bool current_level_present, bool local_controlled,
                        std::int32_t player_class, Result*,
                        std::string& error);

} // namespace dh2::player_manager_remove_character_v1
